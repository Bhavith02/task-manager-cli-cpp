#include "Task.hpp"
#include "ColorUtils.hpp"
#include <iostream>

// Constructor
Task::Task(int taskId, const string& taskTitle, const string& taskDesc, 
           Priority taskPriority)
    : id(taskId), title(taskTitle), description(taskDesc), 
      priority(taskPriority), status(Status::PENDING) {
    createdAt = time(nullptr);
    dueDate = 0;
}

// Getters
int Task::getId() const { return id; }
string Task::getTitle() const { return title; }
string Task::getDescription() const { return description; }
Priority Task::getPriority() const { return priority; }
Status Task::getStatus() const { return status; }
time_t Task::getCreatedAt() const { return createdAt; }
time_t Task::getDueDate() const { return dueDate; }

// Setters
void Task::setTitle(const string& newTitle) { title = newTitle; }
void Task::setDescription(const string& newDesc) { description = newDesc; }
void Task::setPriority(Priority newPriority) { priority = newPriority; }
void Task::setStatus(Status newStatus) { status = newStatus; }
void Task::setDueDate(time_t newDueDate) { dueDate = newDueDate; }
void Task::setCreatedAt(time_t newCreatedAt) { createdAt = newCreatedAt; }  // NEW

// Utility methods
string Task::getPriorityString() const {
    switch (priority) {
        case Priority::LOW: return "Low";
        case Priority::MEDIUM: return "Medium";
        case Priority::HIGH: return "High";
        default: return "Unknown";
    }
}

string Task::getStatusString() const {
    switch (status) {
        case Status::PENDING: return "Pending";
        case Status::IN_PROGRESS: return "In Progress";
        case Status::COMPLETED: return "Completed";
        default: return "Unknown";
    }
}

string Task::getPriorityColorString() const {
    switch (priority) {
        case Priority::LOW: 
            return ColorUtils::colorize("🟢 Low", ColorUtils::GREEN);
        case Priority::MEDIUM: 
            return ColorUtils::colorize("🟡 Medium", ColorUtils::YELLOW);
        case Priority::HIGH: 
            return ColorUtils::colorize("🔴 High", ColorUtils::RED);
        default: 
            return "Unknown";
    }
}

string Task::getStatusColorString() const {
    switch (status) {
        case Status::PENDING: 
            return ColorUtils::colorize("⏳ Pending", ColorUtils::YELLOW);
        case Status::IN_PROGRESS: 
            return ColorUtils::colorize("🔄 In Progress", ColorUtils::BLUE);
        case Status::COMPLETED: 
            return ColorUtils::colorize("✅ Completed", ColorUtils::GREEN);
        default: 
            return "Unknown";
    }
}

void Task::display() const {
    cout << "\n" << ColorUtils::colorize("─── Task #" + to_string(id) + " ───", ColorUtils::CYAN) << endl;
    cout << ColorUtils::BOLD << "Title: " << ColorUtils::RESET << title << endl;
    cout << ColorUtils::BOLD << "Description: " << ColorUtils::RESET << description << endl;
    cout << ColorUtils::BOLD << "Priority: " << ColorUtils::RESET << getPriorityColorString() << endl;
    cout << ColorUtils::BOLD << "Status: " << ColorUtils::RESET << getStatusColorString() << endl;
    
    // Display created date
    char buffer[80];
    struct tm* timeinfo = localtime(&createdAt);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    cout << ColorUtils::BOLD << "Created: " << ColorUtils::RESET << buffer << endl;
    
    // Display due date with status
    string dueDateStr = getDueDateString();
    if (isOverdue()) {
        dueDateStr = ColorUtils::colorize(dueDateStr, ColorUtils::BRIGHT_RED);
    } else if (hasDueDate() && getDaysUntilDue() <= 3) {
        dueDateStr = ColorUtils::colorize(dueDateStr, ColorUtils::BRIGHT_YELLOW);
    }
    cout << ColorUtils::BOLD << "Due: " << ColorUtils::RESET << dueDateStr << endl;
    cout << ColorUtils::colorize("───────────────────", ColorUtils::CYAN) << endl;
}

void Task::markComplete() {
    status = Status::COMPLETED;
}

bool Task::isCompleted() const {
    return status == Status::COMPLETED;
}

// Due date utilities
bool Task::hasDueDate() const {
    return dueDate > 0;
}

bool Task::isOverdue() const {
    if (!hasDueDate() || isCompleted()) {
        return false;
    }
    time_t now = time(nullptr);
    return now > dueDate;
}

int Task::getDaysUntilDue() const {
    if (!hasDueDate()) {
        return 0;
    }
    time_t now = time(nullptr);
    double seconds = difftime(dueDate, now);
    return static_cast<int>(seconds / (60 * 60 * 24));
}

string Task::getDueDateString() const {
    if (!hasDueDate()) {
        return "No due date";
    }
    
    char buffer[80];
    struct tm* timeinfo = localtime(&dueDate);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", timeinfo);
    
    string result = buffer;
    
    if (isCompleted()) {
        result += " (Completed)";
    } else if (isOverdue()) {
        int daysOver = -getDaysUntilDue();
        result += " (⚠️ Overdue by " + to_string(daysOver) + " day" + (daysOver != 1 ? "s" : "") + ")";
    } else {
        int daysLeft = getDaysUntilDue();
        if (daysLeft == 0) {
            result += " (📌 Due today!)";
        } else {
            result += " (" + to_string(daysLeft) + " day" + (daysLeft != 1 ? "s" : "") + " left)";
        }
    }
    
    return result;
}
