#include "DatabaseHandler.hpp"
#include <iostream>
#include <sstream>
#include <cstring>

DatabaseHandler::DatabaseHandler(const string& host, const string& port,
                                 const string& dbname, const string& user,
                                 const string& password) {
    stringstream ss;
    ss << "host=" << host << " port=" << port << " dbname=" << dbname
       << " user=" << user;
    if (!password.empty()) {
        ss << " password=" << password;
    }
    connectionString = ss.str();
    conn = nullptr;
}

DatabaseHandler::~DatabaseHandler() {
    disconnect();
}

bool DatabaseHandler::connect() {
    conn = PQconnectdb(connectionString.c_str());
    
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        conn = nullptr;
        return false;
    }
    
    cout << "✓ Connected to PostgreSQL database" << endl;
    return true;
}

void DatabaseHandler::disconnect() {
    if (conn) {
        PQfinish(conn);
        conn = nullptr;
    }
}

bool DatabaseHandler::isConnected() {
    return conn != nullptr && PQstatus(conn) == CONNECTION_OK;
}

bool DatabaseHandler::executeQuery(const string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Query failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return false;
    }
    
    PQclear(res);
    return true;
}

PGresult* DatabaseHandler::executeSelect(const string& query) {
    PGresult* res = PQexec(conn, query.c_str());
    
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cerr << "Query failed: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        return nullptr;
    }
    
    return res;
}

bool DatabaseHandler::createSchema() {
    string createTable = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id SERIAL PRIMARY KEY,
            title VARCHAR(255) NOT NULL,
            description TEXT NOT NULL,
            priority VARCHAR(20) NOT NULL,
            status VARCHAR(20) NOT NULL,
            created_at BIGINT NOT NULL,
            due_date BIGINT DEFAULT 0
        );
        
        CREATE TABLE IF NOT EXISTS settings (
            key VARCHAR(50) PRIMARY KEY,
            value VARCHAR(255)
        );
    )";
    
    return executeQuery(createTable);
}

bool DatabaseHandler::dropSchema() {
    string dropTables = "DROP TABLE IF EXISTS tasks, settings;";
    return executeQuery(dropTables);
}

Priority DatabaseHandler::parsePriority(const string& str) {
    if (str == "HIGH") return Priority::HIGH;
    if (str == "LOW") return Priority::LOW;
    return Priority::MEDIUM;
}

Status DatabaseHandler::parseStatus(const string& str) {
    if (str == "IN_PROGRESS") return Status::IN_PROGRESS;
    if (str == "COMPLETED") return Status::COMPLETED;
    return Status::PENDING;
}

string DatabaseHandler::priorityToString(Priority priority) {
    switch (priority) {
        case Priority::LOW: return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH: return "HIGH";
    }
    return "MEDIUM";
}

string DatabaseHandler::statusToString(Status status) {
    switch (status) {
        case Status::PENDING: return "PENDING";
        case Status::IN_PROGRESS: return "IN_PROGRESS";
        case Status::COMPLETED: return "COMPLETED";
    }
    return "PENDING";
}

int DatabaseHandler::insertTask(const Task& task) {
    stringstream query;
    query << "INSERT INTO tasks (title, description, priority, status, created_at, due_date) VALUES ('"
          << task.getTitle() << "', '"
          << task.getDescription() << "', '"
          << priorityToString(task.getPriority()) << "', '"
          << statusToString(task.getStatus()) << "', "
          << task.getCreatedAt() << ", "
          << task.getDueDate() << ") RETURNING id;";
    
    PGresult* res = executeSelect(query.str());
    if (!res) return -1;
    
    int id = atoi(PQgetvalue(res, 0, 0));
    PQclear(res);
    return id;
}

bool DatabaseHandler::updateTask(const Task& task) {
    stringstream query;
    query << "UPDATE tasks SET "
          << "title='" << task.getTitle() << "', "
          << "description='" << task.getDescription() << "', "
          << "priority='" << priorityToString(task.getPriority()) << "', "
          << "status='" << statusToString(task.getStatus()) << "', "
          << "due_date=" << task.getDueDate()
          << " WHERE id=" << task.getId() << ";";
    
    return executeQuery(query.str());
}

bool DatabaseHandler::deleteTask(int id) {
    stringstream query;
    query << "DELETE FROM tasks WHERE id=" << id << ";";
    return executeQuery(query.str());
}

Task* DatabaseHandler::getTaskById(int id) {
    stringstream query;
    query << "SELECT * FROM tasks WHERE id=" << id << ";";
    
    PGresult* res = executeSelect(query.str());
    if (!res || PQntuples(res) == 0) {
        if (res) PQclear(res);
        return nullptr;
    }
    
    Task* task = new Task(
        atoi(PQgetvalue(res, 0, 0)),  // id
        PQgetvalue(res, 0, 1),         // title
        PQgetvalue(res, 0, 2),         // description
        parsePriority(PQgetvalue(res, 0, 3))  // priority
    );
    
    task->setStatus(parseStatus(PQgetvalue(res, 0, 4)));
    task->setDueDate(atol(PQgetvalue(res, 0, 6)));
    
    PQclear(res);
    return task;
}

vector<Task> DatabaseHandler::getAllTasks() {
    vector<Task> tasks;
    
    PGresult* res = executeSelect("SELECT * FROM tasks ORDER BY id;");
    if (!res) return tasks;
    
    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        Task task(
            atoi(PQgetvalue(res, i, 0)),
            PQgetvalue(res, i, 1),
            PQgetvalue(res, i, 2),
            parsePriority(PQgetvalue(res, i, 3))
        );
        
        task.setStatus(parseStatus(PQgetvalue(res, i, 4)));
        task.setDueDate(atol(PQgetvalue(res, i, 6)));
        
        tasks.push_back(task);
    }
    
    PQclear(res);
    return tasks;
}

bool DatabaseHandler::saveTasks(const vector<Task>& tasks, int nextId) {
    // Clear existing tasks
    executeQuery("DELETE FROM tasks;");
    
    // Insert all tasks
    for (const auto& task : tasks) {
        stringstream query;
        query << "INSERT INTO tasks (id, title, description, priority, status, created_at, due_date) VALUES ("
              << task.getId() << ", '"
              << task.getTitle() << "', '"
              << task.getDescription() << "', '"
              << priorityToString(task.getPriority()) << "', '"
              << statusToString(task.getStatus()) << "', "
              << task.getCreatedAt() << ", "
              << task.getDueDate() << ");";
        
        if (!executeQuery(query.str())) {
            return false;
        }
    }
    
    // Update next ID
    stringstream query;
    query << "INSERT INTO settings (key, value) VALUES ('next_id', '" << nextId 
          << "') ON CONFLICT (key) DO UPDATE SET value='" << nextId << "';";
    
    return executeQuery(query.str());
}

bool DatabaseHandler::loadTasks(vector<Task>& tasks, int& nextId) {
    tasks = getAllTasks();
    
    // Get next ID
    PGresult* res = executeSelect("SELECT value FROM settings WHERE key='next_id';");
    if (res && PQntuples(res) > 0) {
        nextId = atoi(PQgetvalue(res, 0, 0));
        PQclear(res);
    } else {
        nextId = tasks.empty() ? 1 : tasks.back().getId() + 1;
    }
    
    return true;
}
