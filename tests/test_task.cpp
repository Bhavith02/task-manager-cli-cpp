#include <gtest/gtest.h>
#include "Task.hpp"
#include <ctime>

class TaskTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code runs before each test
    }

    void TearDown() override {
        // Cleanup code runs after each test
    }
};

// Test Task creation
TEST_F(TaskTest, TaskCreation) {
    Task task(1, "Test Task", "Test Description", Priority::MEDIUM);
    
    EXPECT_EQ(task.getId(), 1);
    EXPECT_EQ(task.getTitle(), "Test Task");
    EXPECT_EQ(task.getDescription(), "Test Description");
    EXPECT_EQ(task.getPriority(), Priority::MEDIUM);
    EXPECT_EQ(task.getStatus(), Status::PENDING);
}

// Test Task setters
TEST_F(TaskTest, TaskSetters) {
    Task task(1, "Original", "Description", Priority::LOW);
    
    task.setTitle("Modified");
    task.setDescription("New Description");
    task.setPriority(Priority::HIGH);
    task.setStatus(Status::IN_PROGRESS);
    
    EXPECT_EQ(task.getTitle(), "Modified");
    EXPECT_EQ(task.getDescription(), "New Description");
    EXPECT_EQ(task.getPriority(), Priority::HIGH);
    EXPECT_EQ(task.getStatus(), Status::IN_PROGRESS);
}

// Test Task completion
TEST_F(TaskTest, TaskCompletion) {
    Task task(1, "Test", "Desc", Priority::MEDIUM);
    
    EXPECT_FALSE(task.isCompleted());
    
    task.markComplete();
    
    EXPECT_TRUE(task.isCompleted());
    EXPECT_EQ(task.getStatus(), Status::COMPLETED);
}

// Test Due Date functionality
TEST_F(TaskTest, DueDateFunctionality) {
    Task task(1, "Test", "Desc", Priority::MEDIUM);
    
    // Initially no due date
    EXPECT_FALSE(task.hasDueDate());
    EXPECT_EQ(task.getDueDate(), 0);
    
    // Set due date to tomorrow
    time_t tomorrow = time(nullptr) + (24 * 60 * 60);
    task.setDueDate(tomorrow);
    
    EXPECT_TRUE(task.hasDueDate());
    EXPECT_FALSE(task.isOverdue());
    EXPECT_GT(task.getDaysUntilDue(), 0);
}

// Test Overdue detection
TEST_F(TaskTest, OverdueDetection) {
    Task task(1, "Test", "Desc", Priority::MEDIUM);
    
    // Set due date to yesterday
    time_t yesterday = time(nullptr) - (24 * 60 * 60);
    task.setDueDate(yesterday);
    
    EXPECT_TRUE(task.hasDueDate());
    EXPECT_TRUE(task.isOverdue());
    EXPECT_LT(task.getDaysUntilDue(), 0);
}

// Test Priority strings
TEST_F(TaskTest, PriorityStrings) {
    Task lowTask(1, "Low", "Desc", Priority::LOW);
    Task medTask(2, "Med", "Desc", Priority::MEDIUM);
    Task highTask(3, "High", "Desc", Priority::HIGH);
    
    EXPECT_EQ(lowTask.getPriorityString(), "Low");
    EXPECT_EQ(medTask.getPriorityString(), "Medium");
    EXPECT_EQ(highTask.getPriorityString(), "High");
}

// Test Status strings
TEST_F(TaskTest, StatusStrings) {
    Task task(1, "Test", "Desc", Priority::MEDIUM);
    
    EXPECT_EQ(task.getStatusString(), "Pending");
    
    task.setStatus(Status::IN_PROGRESS);
    EXPECT_EQ(task.getStatusString(), "In Progress");
    
    task.setStatus(Status::COMPLETED);
    EXPECT_EQ(task.getStatusString(), "Completed");
}
