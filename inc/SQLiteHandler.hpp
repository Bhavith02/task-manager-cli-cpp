#ifndef SQLITEHANDLER_HPP
#define SQLITEHANDLER_HPP

#include "Task.hpp"
#include <vector>
#include <string>
#include <sqlite3.h>

using namespace std;

class SQLiteHandler {
private:
    sqlite3* db;
    string dbPath;
    
    Priority parsePriority(const string& str);
    Status parseStatus(const string& str);
    string priorityToString(Priority priority);
    string statusToString(Status status);
    
public:
    SQLiteHandler(const string& path = "../data/tasks.db");
    ~SQLiteHandler();
    
    // Connection management
    bool connect();
    void disconnect();
    bool isConnected();
    
    // Schema operations
    bool createSchema();
    
    // Task operations
    bool saveTasks(const vector<Task>& tasks, int nextId);
    bool loadTasks(vector<Task>& tasks, int& nextId);
    
    // Individual task operations (for API)
    int insertTask(const Task& task);
    bool updateTask(const Task& task);
    bool deleteTask(int id);
    Task* getTaskById(int id);
    vector<Task> getAllTasks();
};

#endif // SQLITEHANDLER_HPP
