#include "SQLiteHandler.hpp"
#include <iostream>
#include <sstream>

SQLiteHandler::SQLiteHandler(const string& path) : dbPath(path), db(nullptr) {}

SQLiteHandler::~SQLiteHandler() {
    disconnect();
}

bool SQLiteHandler::connect() {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    
    if (rc != SQLITE_OK) {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    
    cout << "✓ Connected to SQLite database: " << dbPath << endl;
    
    // Create schema if not exists
    return createSchema();
}

void SQLiteHandler::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool SQLiteHandler::isConnected() {
    return db != nullptr;
}

bool SQLiteHandler::createSchema() {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT NOT NULL,
            priority TEXT NOT NULL,
            status TEXT NOT NULL,
            created_at INTEGER NOT NULL,
            due_date INTEGER DEFAULT 0
        );
        
        CREATE TABLE IF NOT EXISTS settings (
            key TEXT PRIMARY KEY,
            value TEXT
        );
    )";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

Priority SQLiteHandler::parsePriority(const string& str) {
    if (str == "HIGH") return Priority::HIGH;
    if (str == "LOW") return Priority::LOW;
    return Priority::MEDIUM;
}

Status SQLiteHandler::parseStatus(const string& str) {
    if (str == "IN_PROGRESS") return Status::IN_PROGRESS;
    if (str == "COMPLETED") return Status::COMPLETED;
    return Status::PENDING;
}

string SQLiteHandler::priorityToString(Priority priority) {
    switch (priority) {
        case Priority::LOW: return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH: return "HIGH";
    }
    return "MEDIUM";
}

string SQLiteHandler::statusToString(Status status) {
    switch (status) {
        case Status::PENDING: return "PENDING";
        case Status::IN_PROGRESS: return "IN_PROGRESS";
        case Status::COMPLETED: return "COMPLETED";
    }
    return "PENDING";
}

int SQLiteHandler::insertTask(const Task& task) {
    stringstream sql;
    sql << "INSERT INTO tasks (title, description, priority, status, created_at, due_date) VALUES ('"
        << task.getTitle() << "', '"
        << task.getDescription() << "', '"
        << priorityToString(task.getPriority()) << "', '"
        << statusToString(task.getStatus()) << "', "
        << task.getCreatedAt() << ", "
        << task.getDueDate() << ");";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << "Insert failed: " << errMsg << endl;
        sqlite3_free(errMsg);
        return -1;
    }
    
    return sqlite3_last_insert_rowid(db);
}

bool SQLiteHandler::updateTask(const Task& task) {
    stringstream sql;
    sql << "UPDATE tasks SET "
        << "title='" << task.getTitle() << "', "
        << "description='" << task.getDescription() << "', "
        << "priority='" << priorityToString(task.getPriority()) << "', "
        << "status='" << statusToString(task.getStatus()) << "', "
        << "due_date=" << task.getDueDate()
        << " WHERE id=" << task.getId() << ";";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << "Update failed: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool SQLiteHandler::deleteTask(int id) {
    stringstream sql;
    sql << "DELETE FROM tasks WHERE id=" << id << ";";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        cerr << "Delete failed: " << errMsg << endl;
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

Task* SQLiteHandler::getTaskById(int id) {
    stringstream sql;
    sql << "SELECT * FROM tasks WHERE id=" << id << ";";
    
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.str().c_str(), -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        return nullptr;
    }
    
    Task* task = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        task = new Task(
            sqlite3_column_int(stmt, 0),  // id
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),  // title
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),  // description
            parsePriority(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))  // priority
        );
        
        task->setStatus(parseStatus(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
        task->setDueDate(sqlite3_column_int64(stmt, 6));
    }
    
    sqlite3_finalize(stmt);
    return task;
}

vector<Task> SQLiteHandler::getAllTasks() {
    vector<Task> tasks;
    
    const char* sql = "SELECT * FROM tasks ORDER BY id;";
    sqlite3_stmt* stmt;
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        return tasks;
    }
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task(
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
            parsePriority(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))
        );
        
        task.setStatus(parseStatus(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
        task.setDueDate(sqlite3_column_int64(stmt, 6));
        
        tasks.push_back(task);
    }
    
    sqlite3_finalize(stmt);
    return tasks;
}

bool SQLiteHandler::saveTasks(const vector<Task>& tasks, int nextId) {
    // Clear existing
    sqlite3_exec(db, "DELETE FROM tasks;", nullptr, nullptr, nullptr);
    
    // Insert all tasks
    for (const auto& task : tasks) {
        if (insertTask(task) < 0) {
            return false;
        }
    }
    
    // Update next ID
    stringstream sql;
    sql << "INSERT OR REPLACE INTO settings (key, value) VALUES ('next_id', '" << nextId << "');";
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.str().c_str(), nullptr, nullptr, &errMsg);
    
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    
    return true;
}

bool SQLiteHandler::loadTasks(vector<Task>& tasks, int& nextId) {
    tasks = getAllTasks();
    
    // Get next ID
    const char* sql = "SELECT value FROM settings WHERE key='next_id';";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            nextId = atoi(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        } else {
            nextId = tasks.empty() ? 1 : tasks.back().getId() + 1;
        }
        sqlite3_finalize(stmt);
    }
    
    return true;
}
