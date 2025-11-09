#include "TaskManager.hpp"
#include "ColorUtils.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

// Constructor
TaskManager::TaskManager() : nextId(1), fileHandler("../data/tasks.json") {
    loadFromFile();
}

void TaskManager::autoSave() {
    saveToFile();
}

bool TaskManager::loadFromFile() {
    bool success = fileHandler.loadTasks(tasks, nextId);
    if (success && !tasks.empty()) {
        cout << "✓ Loaded " << tasks.size() << " task(s) from file." << endl;
    }
    return success;
}

bool TaskManager::saveToFile() {
    return fileHandler.saveTasks(tasks, nextId);
}

bool TaskManager::exportToCSV(const string& filename) {
    CSVExporter exporter;
    return exporter.exportToCSV(tasks, filename);
}

bool TaskManager::exportFilteredToCSV(Status status, const string& filename) {
    vector<Task> filtered;
    for (const auto& task : tasks) {
        if (task.getStatus() == status) {
            filtered.push_back(task);
        }
    }
    
    CSVExporter exporter;
    return exporter.exportToCSV(filtered, filename);
}

bool TaskManager::exportFilteredToCSV(Priority priority, const string& filename) {
    vector<Task> filtered;
    for (const auto& task : tasks) {
        if (task.getPriority() == priority) {
            filtered.push_back(task);
        }
    }
    
    CSVExporter exporter;
    return exporter.exportToCSV(filtered, filename);
}

int TaskManager::addTask(const string& title, const string& description, 
                         Priority priority) {
    Task newTask(nextId, title, description, priority);
    tasks.push_back(newTask);
    int id = nextId++;
    autoSave();
    return id;
}

vector<Task>& TaskManager::getAllTasks() {
    return tasks;
}

int TaskManager::getTaskCount() const {
    return tasks.size();
}

Task* TaskManager::findTaskById(int id) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            return &task;
        }
    }
    return nullptr;
}

bool TaskManager::deleteTask(int id) {
    auto it = find_if(tasks.begin(), tasks.end(),
                     [id](const Task& task) { return task.getId() == id; });
    
    if (it != tasks.end()) {
        tasks.erase(it);
        autoSave();
        return true;
    }
    return false;
}

void TaskManager::displayAllTasks() const {
    if (tasks.empty()) {
        cout << "\nNo tasks found! Add your first task to get started." << endl;
        return;
    }

    cout << "\n========================================" << endl;
    cout << "         ALL TASKS (" << tasks.size() << " total)" << endl;
    cout << "========================================" << endl;

    for (const auto& task : tasks) {
        task.display();
    }
}

void TaskManager::displayTasksByStatus(Status status) const {
    bool found = false;
    
    cout << "\n========================================" << endl;
    cout << "    Tasks with status: ";
    
    switch(status) {
        case Status::PENDING: cout << "PENDING"; break;
        case Status::IN_PROGRESS: cout << "IN PROGRESS"; break;
        case Status::COMPLETED: cout << "COMPLETED"; break;
    }
    
    cout << endl << "========================================" << endl;

    for (const auto& task : tasks) {
        if (task.getStatus() == status) {
            task.display();
            found = true;
        }
    }

    if (!found) {
        cout << "\nNo tasks found with this status." << endl;
    }
}

void TaskManager::displayTasksByPriority(Priority priority) const {
    bool found = false;
    
    cout << "\n========================================" << endl;
    cout << "    Tasks with priority: ";
    
    switch(priority) {
        case Priority::LOW: cout << "LOW"; break;
        case Priority::MEDIUM: cout << "MEDIUM"; break;
        case Priority::HIGH: cout << "HIGH"; break;
    }
    
    cout << endl << "========================================" << endl;

    for (const auto& task : tasks) {
        if (task.getPriority() == priority) {
            task.display();
            found = true;
        }
    }

    if (!found) {
        cout << "\nNo tasks found with this priority." << endl;
    }
}

void TaskManager::displayStatistics() const {
    int pending = 0, inProgress = 0, completed = 0;
    int lowPriority = 0, mediumPriority = 0, highPriority = 0;

    for (const auto& task : tasks) {
        switch (task.getStatus()) {
            case Status::PENDING: pending++; break;
            case Status::IN_PROGRESS: inProgress++; break;
            case Status::COMPLETED: completed++; break;
        }

        switch (task.getPriority()) {
            case Priority::LOW: lowPriority++; break;
            case Priority::MEDIUM: mediumPriority++; break;
            case Priority::HIGH: highPriority++; break;
        }
    }

    cout << "\n========================================" << endl;
    cout << "           TASK STATISTICS" << endl;
    cout << "========================================" << endl;
    cout << "Total Tasks: " << tasks.size() << endl;
    cout << "\nBy Status:" << endl;
    cout << "  Pending:     " << pending << endl;
    cout << "  In Progress: " << inProgress << endl;
    cout << "  Completed:   " << completed << endl;
    cout << "\nBy Priority:" << endl;
    cout << "  Low:         " << lowPriority << endl;
    cout << "  Medium:      " << mediumPriority << endl;
    cout << "  High:        " << highPriority << endl;
    cout << "========================================" << endl;
}

void TaskManager::displayEnhancedStatistics() const {
    if (tasks.empty()) {
        cout << "\nNo tasks available! Add tasks to see statistics." << endl;
        return;
    }
    
    // Calculate statistics
    int total = tasks.size();
    int pending = 0, inProgress = 0, completed = 0;
    int lowPriority = 0, mediumPriority = 0, highPriority = 0;
    int overdue = 0, dueSoon = 0;
    int withDueDate = 0;
    
    for (const auto& task : tasks) {
        // Count by status
        switch (task.getStatus()) {
            case Status::PENDING: pending++; break;
            case Status::IN_PROGRESS: inProgress++; break;
            case Status::COMPLETED: completed++; break;
        }
        
        // Count by priority
        switch (task.getPriority()) {
            case Priority::LOW: lowPriority++; break;
            case Priority::MEDIUM: mediumPriority++; break;
            case Priority::HIGH: highPriority++; break;
        }
        
        // Count overdue and due soon
        if (task.hasDueDate()) {
            withDueDate++;
            if (task.isOverdue() && !task.isCompleted()) {
                overdue++;
            } else if (!task.isCompleted() && task.getDaysUntilDue() <= 3 && task.getDaysUntilDue() >= 0) {
                dueSoon++;
            }
        }
    }
    
    // Calculate percentages
    float completionRate = (total > 0) ? (completed * 100.0f / total) : 0.0f;
    float overdueRate = (withDueDate > 0) ? (overdue * 100.0f / withDueDate) : 0.0f;
    
    // Display enhanced statistics
    cout << "\n" << ColorUtils::colorize("╔════════════════════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
         << ColorUtils::highlight("          ENHANCED STATISTICS DASHBOARD         ")
         << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("╚════════════════════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
    
    // Overall Summary
    cout << "\n" << ColorUtils::BOLD << "📊 OVERALL SUMMARY" << ColorUtils::RESET << endl;
    cout << ColorUtils::colorize("─────────────────────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << "Total Tasks:       " << ColorUtils::colorize(to_string(total), ColorUtils::BRIGHT_GREEN) << endl;
    cout << "Completion Rate:   " << ColorUtils::colorize(to_string((int)completionRate) + "%", 
                                     completionRate >= 70 ? ColorUtils::GREEN : 
                                     completionRate >= 40 ? ColorUtils::YELLOW : ColorUtils::RED) << endl;
    
    // Progress Bar
    cout << "Progress:          [";
    int barLength = 30;
    int filledLength = (int)(completionRate / 100.0f * barLength);
    for (int i = 0; i < barLength; i++) {
        if (i < filledLength) {
            cout << ColorUtils::colorize("█", ColorUtils::GREEN);
        } else {
            cout << "░";
        }
    }
    cout << "] " << (int)completionRate << "%" << endl;
    
    // Status Breakdown
    cout << "\n" << ColorUtils::BOLD << "📋 STATUS BREAKDOWN" << ColorUtils::RESET << endl;
    cout << ColorUtils::colorize("─────────────────────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << ColorUtils::colorize("⏳ Pending:       ", ColorUtils::YELLOW) 
         << pending << " (" << (total > 0 ? (pending * 100 / total) : 0) << "%)" << endl;
    cout << ColorUtils::colorize("🔄 In Progress:   ", ColorUtils::BLUE) 
         << inProgress << " (" << (total > 0 ? (inProgress * 100 / total) : 0) << "%)" << endl;
    cout << ColorUtils::colorize("✅ Completed:     ", ColorUtils::GREEN) 
         << completed << " (" << (total > 0 ? (completed * 100 / total) : 0) << "%)" << endl;
    
    // Priority Distribution
    cout << "\n" << ColorUtils::BOLD << "🎯 PRIORITY DISTRIBUTION" << ColorUtils::RESET << endl;
    cout << ColorUtils::colorize("─────────────────────────────────────────────────", ColorUtils::CYAN) << endl;
    cout << ColorUtils::colorize("🔴 High:          ", ColorUtils::RED) 
         << highPriority << " (" << (total > 0 ? (highPriority * 100 / total) : 0) << "%)" << endl;
    cout << ColorUtils::colorize("🟡 Medium:        ", ColorUtils::YELLOW) 
         << mediumPriority << " (" << (total > 0 ? (mediumPriority * 100 / total) : 0) << "%)" << endl;
    cout << ColorUtils::colorize("🟢 Low:           ", ColorUtils::GREEN) 
         << lowPriority << " (" << (total > 0 ? (lowPriority * 100 / total) : 0) << "%)" << endl;
    
    // Due Date Analytics
    if (withDueDate > 0) {
        cout << "\n" << ColorUtils::BOLD << "📅 DUE DATE ANALYTICS" << ColorUtils::RESET << endl;
        cout << ColorUtils::colorize("─────────────────────────────────────────────────", ColorUtils::CYAN) << endl;
        cout << "Tasks with Dates:  " << withDueDate << " (" << (total > 0 ? (withDueDate * 100 / total) : 0) << "%)" << endl;
        
        if (overdue > 0) {
            cout << ColorUtils::colorize("⚠️  Overdue:        ", ColorUtils::BRIGHT_RED) 
                 << overdue << " (" << (int)overdueRate << "% of tasks with dates)" << endl;
        } else {
            cout << ColorUtils::colorize("✅ No Overdue Tasks!", ColorUtils::GREEN) << endl;
        }
        
        if (dueSoon > 0) {
            cout << ColorUtils::colorize("⏰ Due Soon (≤3d): ", ColorUtils::BRIGHT_YELLOW) 
                 << dueSoon << endl;
        }
    }
    
    // Productivity Insights
    cout << "\n" << ColorUtils::BOLD << "💡 PRODUCTIVITY INSIGHTS" << ColorUtils::RESET << endl;
    cout << ColorUtils::colorize("─────────────────────────────────────────────────", ColorUtils::CYAN) << endl;
    
    if (completionRate >= 70) {
        cout << ColorUtils::success("🎉 Excellent! You're completing most of your tasks!") << endl;
    } else if (completionRate >= 40) {
        cout << ColorUtils::warning("⚡ Good progress! Keep pushing to complete more tasks.") << endl;
    } else {
        cout << ColorUtils::info("💪 Focus on completing pending tasks to improve productivity.") << endl;
    }
    
    if (overdue > 0) {
        cout << ColorUtils::error("⚠️  You have " + to_string(overdue) + " overdue task(s). Prioritize these!") << endl;
    }
    
    if (highPriority > 0 && completed < highPriority) {
        int highIncomplete = highPriority - (completed > highPriority ? highPriority : completed);
        cout << ColorUtils::warning("🔴 " + to_string(highIncomplete) + " high-priority task(s) need attention.") << endl;
    }
    
    cout << ColorUtils::colorize("═════════════════════════════════════════════════", ColorUtils::BRIGHT_BLUE) << endl;
}

bool TaskManager::markTaskComplete(int id) {
    Task* task = findTaskById(id);
    if (task != nullptr) {
        task->markComplete();
        autoSave();
        return true;
    }
    return false;
}

bool TaskManager::hasTasks() const {
    return !tasks.empty();
}

void TaskManager::searchTasks(const string& keyword) const {
    if (tasks.empty()) {
        cout << "\nNo tasks available to search!" << endl;
        return;
    }
    
    string lowerKeyword = keyword;
    for (auto& c : lowerKeyword) {
        c = tolower(c);
    }
    
    vector<const Task*> matchingTasks;
    
    for (const auto& task : tasks) {
        string lowerTitle = task.getTitle();
        string lowerDesc = task.getDescription();
        
        for (auto& c : lowerTitle) c = tolower(c);
        for (auto& c : lowerDesc) c = tolower(c);
        
        if (lowerTitle.find(lowerKeyword) != string::npos ||
            lowerDesc.find(lowerKeyword) != string::npos) {
            matchingTasks.push_back(&task);
        }
    }
    
    if (matchingTasks.empty()) {
        cout << "\nNo tasks found matching \"" << keyword << "\"" << endl;
    } else {
        cout << "\n========================================" << endl;
        cout << "  Search Results for \"" << keyword << "\"" << endl;
        cout << "  Found " << matchingTasks.size() << " matching task(s)" << endl;
        cout << "========================================" << endl;
        
        for (const auto* task : matchingTasks) {
            task->display();
        }
    }
}

void TaskManager::displayOverdueTasks() const {
    bool found = false;
    
    cout << "\n========================================" << endl;
    cout << "          ⚠️  OVERDUE TASKS" << endl;
    cout << "========================================" << endl;

    for (const auto& task : tasks) {
        if (task.isOverdue()) {
            task.display();
            found = true;
        }
    }

    if (!found) {
        cout << "\n✅ No overdue tasks! Great job!" << endl;
    }
}

void TaskManager::sortByPriority(bool highToLow) {
    sort(tasks.begin(), tasks.end(), [highToLow](const Task& a, const Task& b) {
        if (highToLow) {
            return static_cast<int>(a.getPriority()) > static_cast<int>(b.getPriority());
        } else {
            return static_cast<int>(a.getPriority()) < static_cast<int>(b.getPriority());
        }
    });
}

void TaskManager::sortByDueDate(bool soonestFirst) {
    sort(tasks.begin(), tasks.end(), [soonestFirst](const Task& a, const Task& b) {
        if (!a.hasDueDate() && !b.hasDueDate()) return false;
        if (!a.hasDueDate()) return false;
        if (!b.hasDueDate()) return true;
        
        if (soonestFirst) {
            return a.getDueDate() < b.getDueDate();
        } else {
            return a.getDueDate() > b.getDueDate();
        }
    });
}

void TaskManager::sortByCreationDate(bool newestFirst) {
    sort(tasks.begin(), tasks.end(), [newestFirst](const Task& a, const Task& b) {
        if (newestFirst) {
            return a.getCreatedAt() > b.getCreatedAt();
        } else {
            return a.getCreatedAt() < b.getCreatedAt();
        }
    });
}

void TaskManager::sortByStatus() {
    sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return static_cast<int>(a.getStatus()) < static_cast<int>(b.getStatus());
    });
}

void TaskManager::sortByTitle(bool ascending) {
    sort(tasks.begin(), tasks.end(), [ascending](const Task& a, const Task& b) {
        string titleA = a.getTitle();
        string titleB = b.getTitle();
        
        for (auto& c : titleA) c = tolower(c);
        for (auto& c : titleB) c = tolower(c);
        
        if (ascending) {
            return titleA < titleB;
        } else {
            return titleA > titleB;
        }
    });
}

void TaskManager::sortById(bool ascending) {
    sort(tasks.begin(), tasks.end(), [ascending](const Task& a, const Task& b) {
        if (ascending) {
            return a.getId() < b.getId();
        } else {
            return a.getId() > b.getId();
        }
    });
}

void TaskManager::displaySortedTasks(const string& sortType) const {
    if (tasks.empty()) {
        cout << "\nNo tasks available!" << endl;
        return;
    }
    
    cout << "\n========================================" << endl;
    cout << "    Tasks Sorted by: " << sortType << endl;
    cout << "========================================" << endl;
    
    for (const auto& task : tasks) {
        task.display();
    }
}

int TaskManager::markAllComplete() {
    int count = 0;
    for (auto& task : tasks) {
        if (!task.isCompleted()) {
            task.markComplete();
            count++;
        }
    }
    if (count > 0) {
        autoSave();
    }
    return count;
}

int TaskManager::deleteAllCompleted() {
    int count = 0;
    auto it = tasks.begin();
    while (it != tasks.end()) {
        if (it->isCompleted()) {
            it = tasks.erase(it);
            count++;
        } else {
            ++it;
        }
    }
    if (count > 0) {
        autoSave();
    }
    return count;
}

int TaskManager::deleteAllTasks() {
    int count = tasks.size();
    tasks.clear();
    if (count > 0) {
        autoSave();
    }
    return count;
}

int TaskManager::changePriorityBulk(Priority oldPriority, Priority newPriority) {
    int count = 0;
    for (auto& task : tasks) {
        if (task.getPriority() == oldPriority) {
            task.setPriority(newPriority);
            count++;
        }
    }
    if (count > 0) {
        autoSave();
    }
    return count;
}
