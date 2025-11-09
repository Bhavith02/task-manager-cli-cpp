#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>
#include "TaskManager.hpp"
#include "httplib.h"

using namespace std;
using namespace httplib;

// Global TaskManager instance
TaskManager taskManager;

// Helper: Convert Task to JSON
string taskToJson(const Task& task) {
    ostringstream json;
    json << "{";
    json << "\"id\":" << task.getId() << ",";
    json << "\"title\":\"" << task.getTitle() << "\",";
    json << "\"description\":\"" << task.getDescription() << "\",";
    json << "\"priority\":\"" << task.getPriorityString() << "\",";
    json << "\"status\":\"" << task.getStatusString() << "\",";
    json << "\"createdAt\":" << task.getCreatedAt() << ",";
    json << "\"dueDate\":" << task.getDueDate() << ",";
    json << "\"isCompleted\":" << (task.isCompleted() ? "true" : "false") << ",";
    json << "\"isOverdue\":" << (task.isOverdue() ? "true" : "false");
    json << "}";
    return json.str();
}

// Helper: Convert all tasks to JSON array
string tasksToJson(const vector<Task>& tasks) {
    ostringstream json;
    json << "[";
    for (size_t i = 0; i < tasks.size(); i++) {
        json << taskToJson(tasks[i]);
        if (i < tasks.size() - 1) json << ",";
    }
    json << "]";
    return json.str();
}

// Helper: Parse priority from string
Priority parsePriority(const string& str) {
    if (str == "HIGH" || str == "High") return Priority::HIGH;
    if (str == "LOW" || str == "Low") return Priority::LOW;
    return Priority::MEDIUM;
}

int main() {
    Server svr;

    cout << "==================================" << endl;
    cout << "   TASK MANAGER REST API SERVER" << endl;
    cout << "==================================" << endl;
    cout << endl;

    // CORS Middleware
    svr.set_pre_routing_handler([](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        
        if (req.method == "OPTIONS") {
            res.status = 204;
            return Server::HandlerResponse::Handled;
        }
        return Server::HandlerResponse::Unhandled;
    });

    // GET / - API Info
    svr.Get("/", [](const Request&, Response& res) {
        res.set_content(R"({
            "name": "Task Manager API",
            "version": "1.0",
            "endpoints": {
                "GET /api/tasks": "Get all tasks",
                "GET /api/tasks/:id": "Get task by ID",
                "POST /api/tasks": "Create new task",
                "PUT /api/tasks/:id": "Update task",
                "DELETE /api/tasks/:id": "Delete task",
                "GET /api/stats": "Get statistics"
            }
        })", "application/json");
    });

    // GET /api/tasks - Get all tasks
    svr.Get("/api/tasks", [](const Request&, Response& res) {
        auto& tasks = taskManager.getAllTasks();
        res.set_content(tasksToJson(tasks), "application/json");
    });

    // GET /api/tasks/:id - Get task by ID
    svr.Get(R"(/api/tasks/(\d+))", [](const Request& req, Response& res) {
        int id = stoi(req.matches[1]);
        Task* task = taskManager.findTaskById(id);
        
        if (task) {
            res.set_content(taskToJson(*task), "application/json");
        } else {
            res.status = 404;
            res.set_content(R"({"error":"Task not found"})", "application/json");
        }
    });

    // POST /api/tasks - Create new task
    svr.Post("/api/tasks", [](const Request& req, Response& res) {
        // Simple JSON parsing (for production, use a JSON library)
        string body = req.body;
        
        // Extract title (basic parsing)
        size_t titlePos = body.find("\"title\":\"");
        size_t descPos = body.find("\"description\":\"");
        size_t priPos = body.find("\"priority\":\"");
        
        if (titlePos == string::npos || descPos == string::npos) {
            res.status = 400;
            res.set_content(R"({"error":"Missing required fields"})", "application/json");
            return;
        }
        
        // Extract values
        titlePos += 9;
        size_t titleEnd = body.find("\"", titlePos);
        string title = body.substr(titlePos, titleEnd - titlePos);
        
        descPos += 15;
        size_t descEnd = body.find("\"", descPos);
        string description = body.substr(descPos, descEnd - descPos);
        
        Priority priority = Priority::MEDIUM;
        if (priPos != string::npos) {
            priPos += 12;
            size_t priEnd = body.find("\"", priPos);
            string priStr = body.substr(priPos, priEnd - priPos);
            priority = parsePriority(priStr);
        }
        
        // Create task
        int id = taskManager.addTask(title, description, priority);
        Task* task = taskManager.findTaskById(id);
        
        res.status = 201;
        res.set_content(taskToJson(*task), "application/json");
    });

    // PUT /api/tasks/:id - Update task
    svr.Put(R"(/api/tasks/(\d+))", [](const Request& req, Response& res) {
        int id = stoi(req.matches[1]);
        Task* task = taskManager.findTaskById(id);
        
        if (!task) {
            res.status = 404;
            res.set_content(R"({"error":"Task not found"})", "application/json");
            return;
        }
        
        string body = req.body;
        
        // Update fields if present
        size_t titlePos = body.find("\"title\":\"");
        if (titlePos != string::npos) {
            titlePos += 9;
            size_t titleEnd = body.find("\"", titlePos);
            task->setTitle(body.substr(titlePos, titleEnd - titlePos));
        }
        
        size_t statusPos = body.find("\"status\":\"");
        if (statusPos != string::npos) {
            statusPos += 10;
            size_t statusEnd = body.find("\"", statusPos);
            string statusStr = body.substr(statusPos, statusEnd - statusPos);
            if (statusStr == "COMPLETED") task->markComplete();
        }
        
        taskManager.saveToFile();
        res.set_content(taskToJson(*task), "application/json");
    });

    // DELETE /api/tasks/:id - Delete task
    svr.Delete(R"(/api/tasks/(\d+))", [](const Request& req, Response& res) {
        int id = stoi(req.matches[1]);
        
        if (taskManager.deleteTask(id)) {
            res.set_content(R"({"success":true,"message":"Task deleted"})", "application/json");
        } else {
            res.status = 404;
            res.set_content(R"({"error":"Task not found"})", "application/json");
        }
    });

    // GET /api/stats - Get statistics
    svr.Get("/api/stats", [](const Request&, Response& res) {
        auto& tasks = taskManager.getAllTasks();
        int total = tasks.size();
        int pending = 0, inProgress = 0, completed = 0;
        int low = 0, medium = 0, high = 0;
        
        for (const auto& task : tasks) {
            switch (task.getStatus()) {
                case Status::PENDING: pending++; break;
                case Status::IN_PROGRESS: inProgress++; break;
                case Status::COMPLETED: completed++; break;
            }
            switch (task.getPriority()) {
                case Priority::LOW: low++; break;
                case Priority::MEDIUM: medium++; break;
                case Priority::HIGH: high++; break;
            }
        }
        
        ostringstream json;
        json << "{";
        json << "\"total\":" << total << ",";
        json << "\"byStatus\":{";
        json << "\"pending\":" << pending << ",";
        json << "\"inProgress\":" << inProgress << ",";
        json << "\"completed\":" << completed;
        json << "},";
        json << "\"byPriority\":{";
        json << "\"low\":" << low << ",";
        json << "\"medium\":" << medium << ",";
        json << "\"high\":" << high;
        json << "}";
        json << "}";
        
        res.set_content(json.str(), "application/json");
    });

    // Start server
    const char* host = "0.0.0.0";
    int port = 8080;
    
    cout << "🚀 Server starting on http://" << host << ":" << port << endl;
    cout << "📋 API endpoints available:" << endl;
    cout << "   GET    /api/tasks       - List all tasks" << endl;
    cout << "   GET    /api/tasks/:id   - Get task by ID" << endl;
    cout << "   POST   /api/tasks       - Create task" << endl;
    cout << "   PUT    /api/tasks/:id   - Update task" << endl;
    cout << "   DELETE /api/tasks/:id   - Delete task" << endl;
    cout << "   GET    /api/stats       - Get statistics" << endl;
    cout << "\nPress Ctrl+C to stop the server..." << endl;
    cout << endl;

    if (!svr.listen(host, port)) {
        cerr << "❌ Failed to start server!" << endl;
        return 1;
    }

    return 0;
}
