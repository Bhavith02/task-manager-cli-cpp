#ifndef TASKOPERATIONS_HPP
#define TASKOPERATIONS_HPP

#include "TaskManager.hpp"
#include "ConfigHandler.hpp"

class TaskOperations {
public:
    static void addNewTask(TaskManager& manager);
    static void updateTask(TaskManager& manager);
    static void deleteTask(TaskManager& manager);
    static void markTaskComplete(TaskManager& manager);
    static void searchTasks(TaskManager& manager);
    static void showStatistics(TaskManager& manager);
    
    // Submenu handlers
    static void viewTasksMenu(TaskManager& manager);
    static void sortTasksMenu(TaskManager& manager);
    static void exportMenu(TaskManager& manager);
    static void bulkOperationsMenu(TaskManager& manager);
    static void settingsMenu(ConfigHandler& config);
};

#endif // TASKOPERATIONS_HPP
