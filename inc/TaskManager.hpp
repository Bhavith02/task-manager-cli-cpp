#ifndef TASKMANAGER_HPP
#define TASKMANAGER_HPP

#include "Task.hpp"
#include "FileHandler.hpp"
#include "CSVExporter.hpp"
#include <vector>
#include <string>

using namespace std;

class TaskManager {
private:
    vector<Task> tasks;
    int nextId;
    FileHandler fileHandler;
    
    // Auto-save after modifications
    void autoSave();

public:
    // Constructor
    TaskManager();

    // Task management
    int addTask(const string& title, const string& description, 
                Priority priority = Priority::MEDIUM);
    bool deleteTask(int id);
    Task* findTaskById(int id);
    bool markTaskComplete(int id);

    // Getters
    vector<Task>& getAllTasks();
    int getTaskCount() const;
    bool hasTasks() const;

    // Display methods
    void displayAllTasks() const;
    void displayTasksByStatus(Status status) const;
    void displayTasksByPriority(Priority priority) const;
    void displayStatistics() const;
    void displayEnhancedStatistics() const;  // NEW
    void displayOverdueTasks() const;
    
    // Sorting methods
    void sortByPriority(bool highToLow = true);
    void sortByDueDate(bool soonestFirst = true);
    void sortByCreationDate(bool newestFirst = true);
    void sortByStatus();
    void sortByTitle(bool ascending = true);
    void sortById(bool ascending = true);
    void displaySortedTasks(const string& sortType) const;
    
    // Search method
    void searchTasks(const string& keyword) const;
    
    // File operations
    bool loadFromFile();
    bool saveToFile();
    
    // Export operations
    bool exportToCSV(const string& filename = "tasks_export.csv");
    bool exportFilteredToCSV(Status status, const string& filename);
    bool exportFilteredToCSV(Priority priority, const string& filename);
    
    // Bulk operations
    int markAllComplete();
    int deleteAllCompleted();
    int deleteAllTasks();
    int changePriorityBulk(Priority oldPriority, Priority newPriority);

};

#endif // TASKMANAGER_HPP
