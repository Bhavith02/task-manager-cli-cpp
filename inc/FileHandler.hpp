#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP

#include "Task.hpp"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class FileHandler {
private:
    string dataFilePath;
    
    // Helper methods
    string priorityToString(Priority priority) const;
    Priority stringToPriority(const string& str) const;
    string statusToString(Status status) const;
    Status stringToStatus(const string& str) const;
    string escapeJson(const string& str) const;
    string unescapeJson(const string& str) const;

public:
    FileHandler(const string& filePath = "../data/tasks.json");
    
    // Save and load operations
    bool saveTasks(const vector<Task>& tasks, int nextId);
    bool loadTasks(vector<Task>& tasks, int& nextId);
    
    // Utility
    bool fileExists() const;
    bool createDataDirectory() const;
};

#endif // FILEHANDLER_HPP
