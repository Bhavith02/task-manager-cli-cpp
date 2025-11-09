#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <ctime>

using namespace std;

// Enum for task priority levels
enum class Priority {
    LOW,
    MEDIUM,
    HIGH
};

// Enum for task status
enum class Status {
    PENDING,
    IN_PROGRESS,
    COMPLETED
};

class Task {
private:
    int id;
    string title;
    string description;
    Priority priority;
    Status status;
    time_t createdAt;
    time_t dueDate;

public:
    // Constructor
    Task(int taskId, const string& taskTitle, const string& taskDesc, 
         Priority taskPriority = Priority::MEDIUM);

    // Getters
    int getId() const;
    string getTitle() const;
    string getDescription() const;
    Priority getPriority() const;
    Status getStatus() const;
    time_t getCreatedAt() const;
    time_t getDueDate() const;

    // Setters
    void setTitle(const string& newTitle);
    void setDescription(const string& newDesc);
    void setPriority(Priority newPriority);
    void setStatus(Status newStatus);
    void setDueDate(time_t newDueDate);
    void setCreatedAt(time_t newCreatedAt);  // NEW - for loading from file

    // Utility methods
    string getPriorityString() const;
    string getStatusString() const;
    string getPriorityColorString() const;
    string getStatusColorString() const;
    void display() const;
    void markComplete();
    bool isCompleted() const;
    
    // Due date utilities
    bool hasDueDate() const;
    bool isOverdue() const;
    int getDaysUntilDue() const;
    string getDueDateString() const;
};

#endif // TASK_HPP
