#include "FileHandler.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>  // NEW - for remove()
#include <sys/stat.h>
#include <sys/types.h>

FileHandler::FileHandler(const string& filePath) : dataFilePath(filePath) {
    createDataDirectory();
}

bool FileHandler::createDataDirectory() const {
    // Extract directory from path
    size_t pos = dataFilePath.find_last_of("/\\");
    if (pos != string::npos) {
        string dir = dataFilePath.substr(0, pos);
        #ifdef _WIN32
            _mkdir(dir.c_str());
        #else
            mkdir(dir.c_str(), 0755);
        #endif
    }
    return true;
}

bool FileHandler::fileExists() const {
    ifstream file(dataFilePath);
    return file.good();
}

string FileHandler::escapeJson(const string& str) const {
    string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    return result;
}

string FileHandler::unescapeJson(const string& str) const {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case '"': result += '"'; i++; break;
                case '\\': result += '\\'; i++; break;
                case 'n': result += '\n'; i++; break;
                case 'r': result += '\r'; i++; break;
                case 't': result += '\t'; i++; break;
                default: result += str[i]; break;
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

string FileHandler::priorityToString(Priority priority) const {
    switch (priority) {
        case Priority::LOW: return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH: return "HIGH";
        default: return "MEDIUM";
    }
}

Priority FileHandler::stringToPriority(const string& str) const {
    if (str == "LOW") return Priority::LOW;
    if (str == "HIGH") return Priority::HIGH;
    return Priority::MEDIUM;
}

string FileHandler::statusToString(Status status) const {
    switch (status) {
        case Status::PENDING: return "PENDING";
        case Status::IN_PROGRESS: return "IN_PROGRESS";
        case Status::COMPLETED: return "COMPLETED";
        default: return "PENDING";
    }
}

Status FileHandler::stringToStatus(const string& str) const {
    if (str == "IN_PROGRESS") return Status::IN_PROGRESS;
    if (str == "COMPLETED") return Status::COMPLETED;
    return Status::PENDING;
}

bool FileHandler::saveTasks(const vector<Task>& tasks, int nextId) {
    ofstream file(dataFilePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file for writing: " << dataFilePath << endl;
        return false;
    }

    file << "{\n";
    file << "  \"nextId\": " << nextId << ",\n";
    file << "  \"tasks\": [\n";

    for (size_t i = 0; i < tasks.size(); i++) {
        const Task& task = tasks[i];
        file << "    {\n";
        file << "      \"id\": " << task.getId() << ",\n";
        file << "      \"title\": \"" << escapeJson(task.getTitle()) << "\",\n";
        file << "      \"description\": \"" << escapeJson(task.getDescription()) << "\",\n";
        file << "      \"priority\": \"" << priorityToString(task.getPriority()) << "\",\n";
        file << "      \"status\": \"" << statusToString(task.getStatus()) << "\",\n";
        file << "      \"createdAt\": " << task.getCreatedAt() << ",\n";
        file << "      \"dueDate\": " << task.getDueDate() << "\n";
        file << "    }";
        if (i < tasks.size() - 1) {
            file << ",";
        }
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";

    file.close();
    return true;
}

bool FileHandler::loadTasks(vector<Task>& tasks, int& nextId) {
    ifstream file(dataFilePath);
    if (!file.is_open()) {
        // File doesn't exist yet - first run
        return true;
    }

    tasks.clear();
    string line;
    
    // Simple JSON parsing (manual for learning purposes)
    int id = 0;
    string title, description;
    Priority priority = Priority::MEDIUM;
    Status status = Status::PENDING;
    time_t createdAt = 0, dueDate = 0;
    
    bool inTask = false;
    
    while (getline(file, line)) {
        // Remove leading/trailing whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");
        if (start != string::npos) {
            line = line.substr(start, end - start + 1);
        }
        
        // Parse nextId
        if (line.find("\"nextId\":") != string::npos) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string value = line.substr(pos + 1);
                value.erase(remove(value.begin(), value.end(), ','), value.end());
                nextId = stoi(value);
            }
        }
        
        // Parse task fields
        if (line.find("\"id\":") != string::npos) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string value = line.substr(pos + 1);
                value.erase(remove(value.begin(), value.end(), ','), value.end());
                id = stoi(value);
                inTask = true;
            }
        }
        else if (line.find("\"title\":") != string::npos) {
            size_t start = line.find("\"", line.find(":") + 1);
            size_t end = line.find_last_of("\"");
            if (start != string::npos && end != string::npos && start < end) {
                title = unescapeJson(line.substr(start + 1, end - start - 1));
            }
        }
        else if (line.find("\"description\":") != string::npos) {
            size_t start = line.find("\"", line.find(":") + 1);
            size_t end = line.find_last_of("\"");
            if (start != string::npos && end != string::npos && start < end) {
                description = unescapeJson(line.substr(start + 1, end - start - 1));
            }
        }
        else if (line.find("\"priority\":") != string::npos) {
            size_t start = line.find("\"", line.find(":") + 1);
            size_t end = line.find_last_of("\"");
            if (start != string::npos && end != string::npos && start < end) {
                priority = stringToPriority(line.substr(start + 1, end - start - 1));
            }
        }
        else if (line.find("\"status\":") != string::npos) {
            size_t start = line.find("\"", line.find(":") + 1);
            size_t end = line.find_last_of("\"");
            if (start != string::npos && end != string::npos && start < end) {
                status = stringToStatus(line.substr(start + 1, end - start - 1));
            }
        }
        else if (line.find("\"createdAt\":") != string::npos) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string value = line.substr(pos + 1);
                value.erase(remove(value.begin(), value.end(), ','), value.end());
                createdAt = stoll(value);
            }
        }
        else if (line.find("\"dueDate\":") != string::npos) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                string value = line.substr(pos + 1);
                value.erase(remove(value.begin(), value.end(), ','), value.end());
                dueDate = stoll(value);
                
                // End of task object - create task
                if (inTask) {
                    Task task(id, title, description, priority);
                    task.setStatus(status);
                    task.setDueDate(dueDate);
                    tasks.push_back(task);
                    inTask = false;
                }
            }
        }
    }

    file.close();
    return true;
}
