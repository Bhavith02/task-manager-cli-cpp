#include "InputHelper.hpp"
#include "ColorUtils.hpp"
#include "TaskManager.hpp"
#include <iostream>
#include <limits>

Priority InputHelper::selectPriority() {
    cout << "\nSelect Priority:" << endl;
    cout << "1. Low" << endl;
    cout << "2. Medium" << endl;
    cout << "3. High" << endl;
    cout << "Enter choice (default: Medium): ";
    
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 1: return Priority::LOW;
        case 3: return Priority::HIGH;
        default: return Priority::MEDIUM;
    }
}

Status InputHelper::selectStatus() {
    cout << "\nSelect Status:" << endl;
    cout << "1. Pending" << endl;
    cout << "2. In Progress" << endl;
    cout << "3. Completed" << endl;
    cout << "Enter choice (default: Pending): ";
    
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    switch (choice) {
        case 2: return Status::IN_PROGRESS;
        case 3: return Status::COMPLETED;
        default: return Status::PENDING;
    }
}

time_t InputHelper::inputDueDate() {
    cout << "\n--- Set Due Date ---" << endl;
    cout << "Enter year (e.g., 2024): ";
    int year;
    cin >> year;
    
    cout << "Enter month (1-12): ";
    int month;
    cin >> month;
    
    cout << "Enter day (1-31): ";
    int day;
    cin >> day;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (year < 2024 || month < 1 || month > 12 || day < 1 || day > 31) {
        cout << "Invalid date entered!" << endl;
        return 0;
    }
    
    struct tm dueTime = {};
    dueTime.tm_year = year - 1900;
    dueTime.tm_mon = month - 1;
    dueTime.tm_mday = day;
    dueTime.tm_hour = 23;
    dueTime.tm_min = 59;
    dueTime.tm_sec = 59;
    
    time_t dueDate = mktime(&dueTime);
    
    if (dueDate == -1) {
        cout << "Error: Invalid date!" << endl;
        return 0;
    }
    
    return dueDate;
}

void InputHelper::setDueDateForTask(Task* task) {
    cout << "\n1. Set new due date" << endl;
    cout << "2. Clear due date" << endl;
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (choice == 1) {
        time_t dueDate = inputDueDate();
        if (dueDate > 0) {
            task->setDueDate(dueDate);
            cout << "✓ Due date updated successfully!" << endl;
        }
    } else if (choice == 2) {
        task->setDueDate(0);
        cout << "✓ Due date cleared!" << endl;
    }
}
