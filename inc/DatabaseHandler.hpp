#ifndef DATABASEHANDLER_HPP
#define DATABASEHANDLER_HPP

#include "Task.hpp"
#include <vector>
#include <string>
#include <libpq-fe.h>

using namespace std;

class DatabaseHandler {
private:
    PGconn* conn;
    string connectionString;
    
    bool executeQuery(const string& query);
    PGresult* executeSelect(const string& query);
    Priority parsePriority(const string& str);
    Status parseStatus(const string& str);
    string priorityToString(Priority priority);
    string statusToString(Status status);
    
public:
    DatabaseHandler(const string& host = "localhost", 
                   const string& port = "5432",
                   const string& dbname = "taskmanager",
                   const string& user = "postgres",
                   const string& password = "");
    
    ~DatabaseHandler();
    
    // Connection management
    bool connect();
    void disconnect();
    bool isConnected();
    
    // Schema operations
    bool createSchema();
    bool dropSchema();
    
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

#endif // DATABASEHANDLER_HPP
