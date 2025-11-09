#ifndef MENUHANDLER_HPP
#define MENUHANDLER_HPP

#include "TaskManager.hpp"
#include <string>

using namespace std;

class MenuHandler {
public:
    static void displayMainMenu(int taskCount);
    static void displayViewTasksMenu();
    static void displaySortTasksMenu();
    static void displayExportMenu();
    static void displayBulkOperationsMenu();
    static void displayUpdateMenu();
    
    // Screen utilities
    static void clearScreen();
    static void pauseScreen();
};

#endif // MENUHANDLER_HPP
