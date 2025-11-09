#include "TaskOperations.hpp"
#include "MenuHandler.hpp"
#include "InputHelper.hpp"
#include "ColorUtils.hpp"
#include <iostream>
#include <limits>

using namespace std;

void TaskOperations::addNewTask(TaskManager& manager) {
    MenuHandler::clearScreen();
    cout << "\n========== ADD NEW TASK ==========" << endl;
    
    string title, description;
    
    cout << "\nEnter task title: ";
    getline(cin, title);
    
    if (title.empty()) {
        cout << "\n" << ColorUtils::error("Task title cannot be empty!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    cout << "Enter task description: ";
    getline(cin, description);
    
    if (description.empty()) {
        cout << "\n" << ColorUtils::error("Task description cannot be empty!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    Priority priority = InputHelper::selectPriority();
    int taskId = manager.addTask(title, description, priority);
    
    cout << "\nWould you like to set a due date? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (choice == 'y' || choice == 'Y') {
        Task* task = manager.findTaskById(taskId);
        if (task != nullptr) {
            time_t dueDate = InputHelper::inputDueDate();
            if (dueDate > 0) {
                task->setDueDate(dueDate);
                manager.saveToFile();
                cout << ColorUtils::success("Due date set successfully!") << endl;
            }
        }
    }
    
    cout << "\n" << ColorUtils::success("Task added successfully with ID: " + to_string(taskId)) << endl;
    MenuHandler::pauseScreen();
}

void TaskOperations::updateTask(TaskManager& manager) {
    MenuHandler::clearScreen();
    
    if (!manager.hasTasks()) {
        cout << "\nNo tasks available to update!" << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    cout << "\n========== UPDATE TASK ==========" << endl;
    manager.displayAllTasks();
    
    cout << "\nEnter Task ID to update (0 to cancel): ";
    int taskId;
    cin >> taskId;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (taskId == 0) return;
    
    Task* task = manager.findTaskById(taskId);
    
    if (task == nullptr) {
        cout << "\n" << ColorUtils::error("Task with ID " + to_string(taskId) + " not found!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    MenuHandler::clearScreen();
    cout << "\n========== CURRENT TASK DETAILS ==========" << endl;
    task->display();
    
    int updateChoice = 0;
    while (updateChoice != 6) {
        MenuHandler::displayUpdateMenu();
        
        cin >> updateChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (updateChoice) {
            case 1: {
                cout << "\nCurrent Title: " << task->getTitle() << endl;
                cout << "Enter new title: ";
                string newTitle;
                getline(cin, newTitle);
                if (!newTitle.empty()) {
                    task->setTitle(newTitle);
                    cout << ColorUtils::success("Title updated successfully!") << endl;
                } else {
                    cout << ColorUtils::error("Title cannot be empty!") << endl;
                }
                break;
            }
            case 2: {
                cout << "\nCurrent Description: " << task->getDescription() << endl;
                cout << "Enter new description: ";
                string newDesc;
                getline(cin, newDesc);
                if (!newDesc.empty()) {
                    task->setDescription(newDesc);
                    cout << ColorUtils::success("Description updated successfully!") << endl;
                } else {
                    cout << ColorUtils::error("Description cannot be empty!") << endl;
                }
                break;
            }
            case 3: {
                cout << "\nCurrent Priority: " << task->getPriorityString() << endl;
                Priority newPriority = InputHelper::selectPriority();
                task->setPriority(newPriority);
                cout << ColorUtils::success("Priority updated successfully!") << endl;
                break;
            }
            case 4: {
                cout << "\nCurrent Status: " << task->getStatusString() << endl;
                Status newStatus = InputHelper::selectStatus();
                task->setStatus(newStatus);
                cout << ColorUtils::success("Status updated successfully!") << endl;
                break;
            }
            case 5: {
                cout << "\nCurrent Due Date: " << task->getDueDateString() << endl;
                InputHelper::setDueDateForTask(task);
                manager.saveToFile();
                break;
            }
            case 6:
                cout << "\nReturning to main menu..." << endl;
                break;
            default:
                cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                break;
        }
        
        if (updateChoice >= 1 && updateChoice <= 5) {
            cout << "\n--- Updated Task ---" << endl;
            task->display();
            manager.saveToFile();
        }
    }
}

void TaskOperations::deleteTask(TaskManager& manager) {
    MenuHandler::clearScreen();
    
    if (!manager.hasTasks()) {
        cout << "\nNo tasks available to delete!" << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    cout << "\n========== DELETE TASK ==========" << endl;
    manager.displayAllTasks();
    
    cout << "\nEnter Task ID to delete (0 to cancel): ";
    int taskId;
    cin >> taskId;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (taskId == 0) {
        cout << "\nDeletion cancelled." << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    Task* task = manager.findTaskById(taskId);
    
    if (task == nullptr) {
        cout << "\n" << ColorUtils::error("Task with ID " + to_string(taskId) + " not found!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    cout << "\n========== TASK TO BE DELETED ==========" << endl;
    task->display();
    
    cout << "\n⚠️  Are you sure you want to delete this task?" << endl;
    cout << "This action cannot be undone!" << endl;
    cout << "Type 'yes' to confirm, anything else to cancel: ";
    
    string confirmation;
    getline(cin, confirmation);
    
    for (auto& c : confirmation) c = tolower(c);
    
    if (confirmation == "yes") {
        if (manager.deleteTask(taskId)) {
            cout << "\n" << ColorUtils::success("Task #" + to_string(taskId) + " deleted successfully!") << endl;
        } else {
            cout << "\n" << ColorUtils::error("Failed to delete task!") << endl;
        }
    } else {
        cout << "\n" << ColorUtils::info("Deletion cancelled.") << endl;
    }
    
    MenuHandler::pauseScreen();
}

void TaskOperations::markTaskComplete(TaskManager& manager) {
    MenuHandler::clearScreen();
    
    if (!manager.hasTasks()) {
        cout << "\nNo tasks available!" << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    cout << "\n========== MARK TASK AS COMPLETE ==========" << endl;
    
    vector<Task>& allTasks = manager.getAllTasks();
    vector<int> incompleteTasks;
    
    cout << "\n--- Incomplete Tasks ---" << endl;
    bool hasIncompleteTasks = false;
    
    for (const auto& task : allTasks) {
        if (!task.isCompleted()) {
            task.display();
            incompleteTasks.push_back(task.getId());
            hasIncompleteTasks = true;
        }
    }
    
    if (!hasIncompleteTasks) {
        cout << "\n" << ColorUtils::success("🎉 Congratulations! All tasks are completed!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    cout << "\nEnter Task ID to mark as complete (0 to cancel): ";
    int taskId;
    cin >> taskId;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (taskId == 0) {
        cout << "\nOperation cancelled." << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    bool found = false;
    for (int id : incompleteTasks) {
        if (id == taskId) {
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "\n" << ColorUtils::error("Task #" + to_string(taskId) + " not found or already completed!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    if (manager.markTaskComplete(taskId)) {
        cout << "\n" << ColorUtils::success("Task #" + to_string(taskId) + " marked as COMPLETED! 🎉") << endl;
        
        Task* task = manager.findTaskById(taskId);
        if (task != nullptr) {
            cout << "\n--- Updated Task ---" << endl;
            task->display();
        }
    } else {
        cout << "\n" << ColorUtils::error("Failed to mark task as complete!") << endl;
    }
    
    MenuHandler::pauseScreen();
}

void TaskOperations::searchTasks(TaskManager& manager) {
    MenuHandler::clearScreen();
    
    if (!manager.hasTasks()) {
        cout << "\nNo tasks available to search!" << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    cout << "\n========== SEARCH TASKS ==========" << endl;
    cout << "\nEnter search keyword (searches in title and description): ";
    
    string keyword;
    getline(cin, keyword);
    
    if (keyword.empty()) {
        cout << "\n" << ColorUtils::error("Search keyword cannot be empty!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    manager.searchTasks(keyword);
    MenuHandler::pauseScreen();
}

void TaskOperations::showStatistics(TaskManager& manager) {
    MenuHandler::clearScreen();
    
    if (!manager.hasTasks()) {
        cout << "\nNo tasks available! Add tasks to see statistics." << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    // Show enhanced statistics instead of basic
    manager.displayEnhancedStatistics();
    
    // Ask if user wants to see basic stats too
    cout << "\nShow basic statistics? (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (choice == 'y' || choice == 'Y') {
        manager.displayStatistics();
    }
    
    MenuHandler::pauseScreen();
}

void TaskOperations::viewTasksMenu(TaskManager& manager) {
    if (!manager.hasTasks()) {
        MenuHandler::clearScreen();
        cout << "\nNo tasks available! Add your first task to get started." << endl;
        MenuHandler::pauseScreen();
        return;
    }

    int viewChoice = 0;
    
    while (viewChoice != 7) {
        MenuHandler::displayViewTasksMenu();
        
        cin >> viewChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (viewChoice) {
            case 1:
                MenuHandler::clearScreen();
                manager.displayAllTasks();
                MenuHandler::pauseScreen();
                break;
            case 2: {
                MenuHandler::clearScreen();
                cout << "\n╔════════════════════════════════╗" << endl;
                cout << "║      FILTER BY STATUS          ║" << endl;
                cout << "╚════════════════════════════════╝" << endl;
                cout << "  1. ⏳ Pending" << endl;
                cout << "  2. 🔄 In Progress" << endl;
                cout << "  3. ✅ Completed" << endl;
                cout << "\nEnter choice: ";
                
                int statusChoice;
                cin >> statusChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                MenuHandler::clearScreen();
                switch (statusChoice) {
                    case 1: manager.displayTasksByStatus(Status::PENDING); break;
                    case 2: manager.displayTasksByStatus(Status::IN_PROGRESS); break;
                    case 3: manager.displayTasksByStatus(Status::COMPLETED); break;
                    default: cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                }
                MenuHandler::pauseScreen();
                break;
            }
            case 3: {
                MenuHandler::clearScreen();
                cout << "\n╔════════════════════════════════╗" << endl;
                cout << "║     FILTER BY PRIORITY         ║" << endl;
                cout << "╚════════════════════════════════╝" << endl;
                cout << "  1. 🟢 Low Priority" << endl;
                cout << "  2. 🟡 Medium Priority" << endl;
                cout << "  3. 🔴 High Priority" << endl;
                cout << "\nEnter choice: ";
                
                int priorityChoice;
                cin >> priorityChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                MenuHandler::clearScreen();
                switch (priorityChoice) {
                    case 1: manager.displayTasksByPriority(Priority::LOW); break;
                    case 2: manager.displayTasksByPriority(Priority::MEDIUM); break;
                    case 3: manager.displayTasksByPriority(Priority::HIGH); break;
                    default: cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                }
                MenuHandler::pauseScreen();
                break;
            }
            case 4:
                MenuHandler::clearScreen();
                manager.displayTasksByStatus(Status::COMPLETED);
                MenuHandler::pauseScreen();
                break;
            case 5:
                MenuHandler::clearScreen();
                manager.displayOverdueTasks();
                MenuHandler::pauseScreen();
                break;
            case 6:
                sortTasksMenu(manager);
                break;
            case 7:
                break;
            default:
                cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                MenuHandler::pauseScreen();
                break;
        }
    }
}

void TaskOperations::sortTasksMenu(TaskManager& manager) {
    MenuHandler::displaySortTasksMenu();
    
    int sortChoice;
    cin >> sortChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    MenuHandler::clearScreen();
    
    switch (sortChoice) {
        case 1:
            manager.sortByPriority(true);
            manager.displaySortedTasks("Priority (High to Low)");
            manager.saveToFile();
            break;
        case 2:
            manager.sortByDueDate(true);
            manager.displaySortedTasks("Due Date (Soonest First)");
            manager.saveToFile();
            break;
        case 3:
            manager.sortByCreationDate(true);
            manager.displaySortedTasks("Creation Date (Newest First)");
            manager.saveToFile();
            break;
        case 4:
            manager.sortByCreationDate(false);
            manager.displaySortedTasks("Creation Date (Oldest First)");
            manager.saveToFile();
            break;
        case 5:
            manager.sortByStatus();
            manager.displaySortedTasks("Status (Pending → Completed)");
            manager.saveToFile();
            break;
        case 6:
            manager.sortByTitle(true);
            manager.displaySortedTasks("Title (A-Z)");
            manager.saveToFile();
            break;
        case 7:
            manager.sortByTitle(false);
            manager.displaySortedTasks("Title (Z-A)");
            manager.saveToFile();
            break;
        case 8:
            manager.sortById(true);
            manager.displaySortedTasks("ID (Ascending 1→20)");
            manager.saveToFile();
            break;
        case 9:
            manager.sortById(false);
            manager.displaySortedTasks("ID (Descending 20→1)");
            manager.saveToFile();
            break;
        default:
            cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
            break;
    }
    
    if (sortChoice >= 1 && sortChoice <= 9) {
        cout << "\n" << ColorUtils::info("Tasks order has been saved.") << endl;
    }
    MenuHandler::pauseScreen();
}

void TaskOperations::exportMenu(TaskManager& manager) {
    if (!manager.hasTasks()) {
        MenuHandler::clearScreen();
        cout << "\n" << ColorUtils::warning("No tasks available to export!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    MenuHandler::displayExportMenu();
    
    int exportChoice;
    cin >> exportChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    string filename;
    bool success = false;
    
    switch (exportChoice) {
        case 1: {
            cout << "\nEnter filename (or press Enter for default 'tasks_export.csv'): ";
            getline(cin, filename);
            if (filename.empty()) {
                filename = "tasks_export.csv";
            } else if (filename.find(".csv") == string::npos) {
                filename += ".csv";
            }
            
            success = manager.exportToCSV(filename);
            if (success) {
                cout << "\n" << ColorUtils::success("All tasks exported successfully!") << endl;
                cout << ColorUtils::info("File saved to: ../data/" + filename) << endl;
            } else {
                cout << "\n" << ColorUtils::error("Export failed!") << endl;
            }
            MenuHandler::pauseScreen();
            break;
        }
        case 2: {
            cout << "\nSelect Status to Export:" << endl;
            cout << "  1. ⏳ Pending" << endl;
            cout << "  2. 🔄 In Progress" << endl;
            cout << "  3. ✅ Completed" << endl;
            cout << "Enter choice: ";
            
            int statusChoice;
            cin >> statusChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            Status status;
            string statusName;
            switch (statusChoice) {
                case 1: status = Status::PENDING; statusName = "pending"; break;
                case 2: status = Status::IN_PROGRESS; statusName = "in_progress"; break;
                case 3: status = Status::COMPLETED; statusName = "completed"; break;
                default:
                    cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                    MenuHandler::pauseScreen();
                    return;
            }
            
            filename = "tasks_" + statusName + ".csv";
            success = manager.exportFilteredToCSV(status, filename);
            
            if (success) {
                cout << "\n" << ColorUtils::success("Filtered tasks exported successfully!") << endl;
                cout << ColorUtils::info("File saved to: ../data/" + filename) << endl;
            } else {
                cout << "\n" << ColorUtils::error("Export failed!") << endl;
            }
            MenuHandler::pauseScreen();
            break;
        }
        case 3: {
            cout << "\nSelect Priority to Export:" << endl;
            cout << "  1. 🟢 Low" << endl;
            cout << "  2. 🟡 Medium" << endl;
            cout << "  3. 🔴 High" << endl;
            cout << "Enter choice: ";
            
            int priorityChoice;
            cin >> priorityChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            Priority priority;
            string priorityName;
            switch (priorityChoice) {
                case 1: priority = Priority::LOW; priorityName = "low"; break;
                case 2: priority = Priority::MEDIUM; priorityName = "medium"; break;
                case 3: priority = Priority::HIGH; priorityName = "high"; break;
                default:
                    cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                    MenuHandler::pauseScreen();
                    return;
            }
            
            filename = "tasks_priority_" + priorityName + ".csv";
            success = manager.exportFilteredToCSV(priority, filename);
            
            if (success) {
                cout << "\n" << ColorUtils::success("Filtered tasks exported successfully!") << endl;
                cout << ColorUtils::info("File saved to: ../data/" + filename) << endl;
            } else {
                cout << "\n" << ColorUtils::error("Export failed!") << endl;
            }
            MenuHandler::pauseScreen();
            break;
        }
        case 4:
            break;
        default:
            cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
            MenuHandler::pauseScreen();
            break;
    }
}

void TaskOperations::bulkOperationsMenu(TaskManager& manager) {
    if (!manager.hasTasks()) {
        MenuHandler::clearScreen();
        cout << "\n" << ColorUtils::warning("No tasks available for bulk operations!") << endl;
        MenuHandler::pauseScreen();
        return;
    }
    
    MenuHandler::displayBulkOperationsMenu();
    
    int bulkChoice;
    cin >> bulkChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (bulkChoice) {
        case 1: {
            MenuHandler::clearScreen();
            cout << "\n" << ColorUtils::warning("⚠️  This will mark ALL incomplete tasks as completed!") << endl;
            cout << "Are you sure? Type 'yes' to confirm: ";
            
            string confirmation;
            getline(cin, confirmation);
            for (auto& c : confirmation) c = tolower(c);
            
            if (confirmation == "yes") {
                int count = manager.markAllComplete();
                if (count > 0) {
                    cout << "\n" << ColorUtils::success("✓ Marked " + to_string(count) + " task(s) as completed! 🎉") << endl;
                } else {
                    cout << "\n" << ColorUtils::info("No incomplete tasks found.") << endl;
                }
            } else {
                cout << "\n" << ColorUtils::info("Operation cancelled.") << endl;
            }
            MenuHandler::pauseScreen();
            break;
        }
        case 2: {
            MenuHandler::clearScreen();
            cout << "\n" << ColorUtils::warning("⚠️  This will permanently delete ALL completed tasks!") << endl;
            cout << "Are you sure? Type 'yes' to confirm: ";
            
            string confirmation;
            getline(cin, confirmation);
            for (auto& c : confirmation) c = tolower(c);
            
            if (confirmation == "yes") {
                int count = manager.deleteAllCompleted();
                if (count > 0) {
                    cout << "\n" << ColorUtils::success("✓ Deleted " + to_string(count) + " completed task(s)!") << endl;
                } else {
                    cout << "\n" << ColorUtils::info("No completed tasks found.") << endl;
                }
            } else {
                cout << "\n" << ColorUtils::info("Operation cancelled.") << endl;
            }
            MenuHandler::pauseScreen();
            break;
        }
        case 3: {
            MenuHandler::clearScreen();
            cout << "\n" << ColorUtils::highlight("Change Priority (Bulk)") << endl;
            cout << "\nSelect tasks with priority:" << endl;
            cout << "  1. 🟢 Low" << endl;
            cout << "  2. 🟡 Medium" << endl;
            cout << "  3. 🔴 High" << endl;
            cout << "Enter choice: ";
            
            int oldChoice;
            cin >> oldChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            Priority oldPriority;
            switch (oldChoice) {
                case 1: oldPriority = Priority::LOW; break;
                case 2: oldPriority = Priority::MEDIUM; break;
                case 3: oldPriority = Priority::HIGH; break;
                default:
                    cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                    MenuHandler::pauseScreen();
                    return;
            }
            
            cout << "\nChange to priority:" << endl;
            cout << "  1. 🟢 Low" << endl;
            cout << "  2. 🟡 Medium" << endl;
            cout << "  3. 🔴 High" << endl;
            cout << "Enter choice: ";
            
            int newChoice;
            cin >> newChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            Priority newPriority;
            switch (newChoice) {
                case 1: newPriority = Priority::LOW; break;
                case 2: newPriority = Priority::MEDIUM; break;
                case 3: newPriority = Priority::HIGH; break;
                default:
                    cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                    MenuHandler::pauseScreen();
                    return;
            }
            
            int count = manager.changePriorityBulk(oldPriority, newPriority);
            if (count > 0) {
                cout << "\n" << ColorUtils::success("✓ Changed priority for " + to_string(count) + " task(s)!") << endl;
            } else {
                cout << "\n" << ColorUtils::info("No tasks found with selected priority.") << endl;
            }
            MenuHandler::pauseScreen();
            break;
        }
        case 4: {
            MenuHandler::clearScreen();
            cout << "\n" << ColorUtils::colorize("⚠️  DANGER: DELETE ALL TASKS ⚠️", ColorUtils::BRIGHT_RED) << endl;
            cout << ColorUtils::warning("This will permanently delete ALL tasks!") << endl;
            cout << ColorUtils::warning("This action CANNOT be undone!") << endl;
            cout << "\nType 'DELETE ALL' to confirm: ";
            
            string confirmation;
            getline(cin, confirmation);
            
            if (confirmation == "DELETE ALL") {
                int count = manager.deleteAllTasks();
                cout << "\n" << ColorUtils::success("✓ Deleted all " + to_string(count) + " task(s)!") << endl;
                cout << ColorUtils::info("Database cleared.") << endl;
            } else {
                cout << "\n" << ColorUtils::info("Operation cancelled. Tasks are safe.") << endl;
            }
            MenuHandler::pauseScreen();
            break;
        }
        case 5:
            break;
        default:
            cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
            MenuHandler::pauseScreen();
            break;
    }
}

void TaskOperations::settingsMenu(ConfigHandler& config) {
    int settingChoice = 0;
    
    while (settingChoice != 6) {
        MenuHandler::clearScreen();
        cout << "\n" << ColorUtils::colorize("╔════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
        cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
             << ColorUtils::highlight("          SETTINGS MENU          ")
             << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
        cout << ColorUtils::colorize("╚════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
        cout << "  1. 🎨 Toggle Colors" << endl;
        cout << "  2. 🎯 Set Default Priority" << endl;
        cout << "  3. 💾 Toggle Auto-Save" << endl;
        cout << "  4. 👁️  Set Default View Count" << endl;
        cout << "  5. 📋 View Current Settings" << endl;
        cout << "  6. ⬅️  Back to Main Menu" << endl;
        cout << ColorUtils::colorize("──────────────────────────────────", ColorUtils::CYAN) << endl;
        cout << "\nEnter your choice: ";
        
        cin >> settingChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (settingChoice) {
            case 1: {
                bool current = config.getColorsEnabled();
                config.setColorsEnabled(!current);
                config.saveConfig();
                cout << "\n" << ColorUtils::success("Colors " + string(!current ? "enabled" : "disabled") + "!") << endl;
                cout << ColorUtils::info("Restart required for full effect.") << endl;
                MenuHandler::pauseScreen();
                break;
            }
            case 2: {
                cout << "\nSelect Default Priority:" << endl;
                cout << "  1. 🟢 Low" << endl;
                cout << "  2. 🟡 Medium" << endl;
                cout << "  3. 🔴 High" << endl;
                cout << "Enter choice: ";
                
                int priChoice;
                cin >> priChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                Priority newDefault;
                switch (priChoice) {
                    case 1: newDefault = Priority::LOW; break;
                    case 3: newDefault = Priority::HIGH; break;
                    default: newDefault = Priority::MEDIUM; break;
                }
                
                config.setDefaultPriority(newDefault);
                config.saveConfig();
                cout << "\n" << ColorUtils::success("Default priority updated!") << endl;
                MenuHandler::pauseScreen();
                break;
            }
            case 3: {
                bool current = config.getAutoSaveEnabled();
                config.setAutoSaveEnabled(!current);
                config.saveConfig();
                cout << "\n" << ColorUtils::success("Auto-save " + string(!current ? "enabled" : "disabled") + "!") << endl;
                MenuHandler::pauseScreen();
                break;
            }
            case 4: {
                cout << "\nEnter default view count (1-100): ";
                int count;
                cin >> count;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (count >= 1 && count <= 100) {
                    config.setDefaultViewCount(count);
                    config.saveConfig();
                    cout << "\n" << ColorUtils::success("Default view count updated!") << endl;
                } else {
                    cout << "\n" << ColorUtils::error("Invalid count! Must be 1-100.") << endl;
                }
                MenuHandler::pauseScreen();
                break;
            }
            case 5:
                MenuHandler::clearScreen();
                config.displaySettings();
                MenuHandler::pauseScreen();
                break;
            case 6:
                break;
            default:
                cout << "\n" << ColorUtils::error("Invalid choice!") << endl;
                MenuHandler::pauseScreen();
                break;
        }
    }
}
