#include <gtest/gtest.h>
#include "TaskManager.hpp"
#include <filesystem>

class TaskManagerTest : public ::testing::Test {
protected:
    TaskManager* manager;
    
    void SetUp() override {
        // Use a test-specific data file
        manager = new TaskManager();
    }

    void TearDown() override {
        delete manager;
        // Clean up test data file if needed
    }
};

// Test adding tasks
TEST_F(TaskManagerTest, AddTask) {
    int initialCount = manager->getTaskCount();
    
    int taskId = manager->addTask("Test Task", "Description", Priority::MEDIUM);
    
    EXPECT_GT(taskId, 0);
    EXPECT_EQ(manager->getTaskCount(), initialCount + 1);
}

// Test finding tasks by ID
TEST_F(TaskManagerTest, FindTaskById) {
    int taskId = manager->addTask("Find Me", "Description", Priority::HIGH);
    
    Task* task = manager->findTaskById(taskId);
    
    ASSERT_NE(task, nullptr);
    EXPECT_EQ(task->getTitle(), "Find Me");
    EXPECT_EQ(task->getPriority(), Priority::HIGH);
}

// Test finding non-existent task
TEST_F(TaskManagerTest, FindNonExistentTask) {
    Task* task = manager->findTaskById(99999);
    
    EXPECT_EQ(task, nullptr);
}

// Test deleting task
TEST_F(TaskManagerTest, DeleteTask) {
    int taskId = manager->addTask("Delete Me", "Description", Priority::LOW);
    int countBefore = manager->getTaskCount();
    
    bool deleted = manager->deleteTask(taskId);
    
    EXPECT_TRUE(deleted);
    EXPECT_EQ(manager->getTaskCount(), countBefore - 1);
    EXPECT_EQ(manager->findTaskById(taskId), nullptr);
}

// Test deleting non-existent task
TEST_F(TaskManagerTest, DeleteNonExistentTask) {
    bool deleted = manager->deleteTask(99999);
    
    EXPECT_FALSE(deleted);
}

// Test marking task complete
TEST_F(TaskManagerTest, MarkTaskComplete) {
    int taskId = manager->addTask("Complete Me", "Description", Priority::MEDIUM);
    
    bool marked = manager->markTaskComplete(taskId);
    
    EXPECT_TRUE(marked);
    
    Task* task = manager->findTaskById(taskId);
    ASSERT_NE(task, nullptr);
    EXPECT_TRUE(task->isCompleted());
    EXPECT_EQ(task->getStatus(), Status::COMPLETED);
}

// Test hasTasks
TEST_F(TaskManagerTest, HasTasks) {
    // Depends on initial state, but after adding should be true
    manager->addTask("Test", "Desc", Priority::LOW);
    
    EXPECT_TRUE(manager->hasTasks());
}

// Test bulk mark complete
TEST_F(TaskManagerTest, BulkMarkComplete) {
    manager->addTask("Task 1", "Desc", Priority::LOW);
    manager->addTask("Task 2", "Desc", Priority::MEDIUM);
    manager->addTask("Task 3", "Desc", Priority::HIGH);
    
    int marked = manager->markAllComplete();
    
    EXPECT_GE(marked, 3);
}

// Test bulk delete completed
TEST_F(TaskManagerTest, BulkDeleteCompleted) {
    int id1 = manager->addTask("Task 1", "Desc", Priority::LOW);
    int id2 = manager->addTask("Task 2", "Desc", Priority::MEDIUM);
    
    manager->markTaskComplete(id1);
    
    int countBefore = manager->getTaskCount();
    int deleted = manager->deleteAllCompleted();
    
    EXPECT_GE(deleted, 1);
    EXPECT_LT(manager->getTaskCount(), countBefore);
}

// Test bulk priority change
TEST_F(TaskManagerTest, BulkPriorityChange) {
    manager->addTask("Low 1", "Desc", Priority::LOW);
    manager->addTask("Low 2", "Desc", Priority::LOW);
    
    int changed = manager->changePriorityBulk(Priority::LOW, Priority::HIGH);
    
    EXPECT_GE(changed, 2);
}

// Test sorting by priority
TEST_F(TaskManagerTest, SortByPriority) {
    manager->addTask("Low", "Desc", Priority::LOW);
    manager->addTask("High", "Desc", Priority::HIGH);
    manager->addTask("Medium", "Desc", Priority::MEDIUM);
    
    manager->sortByPriority(true); // High to Low
    
    auto& tasks = manager->getAllTasks();
    EXPECT_GE(tasks.size(), 3);
    // First task should be high priority
    EXPECT_EQ(tasks[0].getPriority(), Priority::HIGH);
}

// Test sorting by ID
TEST_F(TaskManagerTest, SortById) {
    int id1 = manager->addTask("First", "Desc", Priority::LOW);
    int id2 = manager->addTask("Second", "Desc", Priority::LOW);
    
    manager->sortById(true); // Ascending
    
    auto& tasks = manager->getAllTasks();
    EXPECT_LE(tasks[0].getId(), tasks.back().getId());
}
