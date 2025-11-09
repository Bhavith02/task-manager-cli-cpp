#include "MenuHandler.hpp"
#include "ColorUtils.hpp"
#include <iostream>
#include <limits>

void MenuHandler::displayMainMenu(int taskCount) {
    clearScreen();
    cout << "\n" << ColorUtils::colorize("╔════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
         << ColorUtils::highlight("      TASK MANAGER CLI v1.0     ") 
         << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("╚════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
    cout << "  " << ColorUtils::BOLD << "Total Tasks: " << ColorUtils::RESET 
         << ColorUtils::colorize(to_string(taskCount), ColorUtils::BRIGHT_GREEN) << endl;
    cout << ColorUtils::colorize("──────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << "  1. ➕ Add New Task" << endl;
    cout << "  2. 📋 View Tasks" << endl;
    cout << "  3. ✏️  Update Task" << endl;
    cout << "  4. 🗑️  Delete Task" << endl;
    cout << "  5. ✅ Mark Task Complete" << endl;
    cout << "  6. 🔍 Search Tasks" << endl;
    cout << "  7. 📊 Statistics" << endl;
    cout << "  8. 📤 Export to CSV" << endl;
    cout << "  0. ⚡ Bulk Operations" << endl;
    cout << "  S. ⚙️  Settings" << endl;
    cout << "  9. 🚪 Exit" << endl;
    cout << ColorUtils::colorize("──────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << endl;
}

void MenuHandler::displayViewTasksMenu() {
    clearScreen();
    cout << "\n" << ColorUtils::colorize("╔════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
         << ColorUtils::highlight("        VIEW TASKS MENU        ") 
         << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("╚════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
    cout << "  1. 📝 View All Tasks" << endl;
    cout << "  2. 🔵 Filter by Status" << endl;
    cout << "  3. 🎯 Filter by Priority" << endl;
    cout << "  4. ✅ View Completed Tasks" << endl;
    cout << "  5. ⚠️  View Overdue Tasks" << endl;
    cout << "  6. 🔄 Sort Tasks" << endl;
    cout << "  7. ⬅️  Back to Main Menu" << endl;
    cout << ColorUtils::colorize("──────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << "\nEnter your choice: ";
}

void MenuHandler::displaySortTasksMenu() {
    clearScreen();
    cout << "\n" << ColorUtils::colorize("╔════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
         << ColorUtils::highlight("        SORT TASKS MENU        ") 
         << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("╚════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
    cout << "  1. 🎯 By Priority (High to Low)" << endl;
    cout << "  2. 📅 By Due Date (Soonest First)" << endl;
    cout << "  3. 🕐 By Creation Date (Newest First)" << endl;
    cout << "  4. 🕑 By Creation Date (Oldest First)" << endl;
    cout << "  5. 🔵 By Status (Pending → Completed)" << endl;
    cout << "  6. 🔤 By Title (A-Z)" << endl;
    cout << "  7. 🔠 By Title (Z-A)" << endl;
    cout << "  8. 🔢 By ID (Ascending)" << endl;
    cout << "  9. 🔣 By ID (Descending)" << endl;
    cout << ColorUtils::colorize("──────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << "\nEnter your choice: ";
}

void MenuHandler::displayExportMenu() {
    clearScreen();
    cout << "\n" << ColorUtils::colorize("╔════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
         << ColorUtils::highlight("        EXPORT TO CSV           ") 
         << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("╚════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
    cout << "  1. 📄 Export All Tasks" << endl;
    cout << "  2. 🔵 Export by Status" << endl;
    cout << "  3. 🎯 Export by Priority" << endl;
    cout << "  4. ⬅️  Back to Main Menu" << endl;
    cout << ColorUtils::colorize("──────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << "\nEnter your choice: ";
}

void MenuHandler::displayBulkOperationsMenu() {
    clearScreen();
    cout << "\n" << ColorUtils::colorize("╔════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
         << ColorUtils::highlight("      BULK OPERATIONS          ") 
         << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("╚════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
    cout << "  1. ✅ Mark All Incomplete as Complete" << endl;
    cout << "  2. 🗑️  Delete All Completed Tasks" << endl;
    cout << "  3. 🎯 Change Priority (Bulk)" << endl;
    cout << "  4. ⚠️  Delete ALL Tasks" << endl;
    cout << "  5. ⬅️  Back to Main Menu" << endl;
    cout << ColorUtils::colorize("──────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << "\nEnter your choice: ";
}

void MenuHandler::displayUpdateMenu() {
    cout << "\n--- Update Options ---" << endl;
    cout << "1. Update Title" << endl;
    cout << "2. Update Description" << endl;
    cout << "3. Update Priority" << endl;
    cout << "4. Update Status" << endl;
    cout << "5. Update Due Date" << endl;
    cout << "6. Done (Back to Main Menu)" << endl;
    cout << "Enter choice: ";
}

void MenuHandler::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void MenuHandler::pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}
