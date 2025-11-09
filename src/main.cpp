#include <iostream>
#include <limits>
#include "TaskManager.hpp"
#include "MenuHandler.hpp"
#include "TaskOperations.hpp"
#include "ColorUtils.hpp"
#include "ConfigHandler.hpp"

using namespace std;

int main() {
    ConfigHandler config;
    TaskManager taskManager;

    // Apply config settings
    if (!config.getColorsEnabled()) {
        ColorUtils::disableColors();
    }

    cout << ColorUtils::colorize("==================================", ColorUtils::CYAN) << endl;
    cout << ColorUtils::highlight("   TASK MANAGER CLI v1.0") << endl;
    cout << ColorUtils::colorize("==================================", ColorUtils::CYAN) << endl;
    cout << endl;

    int choice = 0;
    string input;

    while (choice != 9) {
        MenuHandler::displayMainMenu(taskManager.getTaskCount());
        cout << "Enter your choice: ";
        getline(cin, input);

        // Handle 'S' for settings
        if (input == "S" || input == "s") {
            TaskOperations::settingsMenu(config);
            continue;
        }

        // Convert to int
        try {
            choice = stoi(input);
        } catch (...) {
            choice = -1;
        }

        switch (choice) {
            case 0:
                TaskOperations::bulkOperationsMenu(taskManager);
                break;
            case 1:
                TaskOperations::addNewTask(taskManager);
                break;
            case 2:
                TaskOperations::viewTasksMenu(taskManager);
                break;
            case 3:
                TaskOperations::updateTask(taskManager);
                break;
            case 4:
                TaskOperations::deleteTask(taskManager);
                break;
            case 5:
                TaskOperations::markTaskComplete(taskManager);
                break;
            case 6:
                TaskOperations::searchTasks(taskManager);
                break;
            case 7:
                TaskOperations::showStatistics(taskManager);
                break;
            case 8:
                TaskOperations::exportMenu(taskManager);
                break;
            case 9:
                cout << "\n" << ColorUtils::info("Saving tasks...") << endl;
                taskManager.saveToFile();
                cout << ColorUtils::success("Thank you for using Task Manager! Goodbye!") << endl;
                break;
            default:
                cout << "\n" << ColorUtils::error("Invalid choice! Please try again.") << endl;
                MenuHandler::pauseScreen();
                break;
        }
    }

    return 0;
}
