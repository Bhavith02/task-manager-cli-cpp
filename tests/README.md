# Unit Tests

This directory contains unit tests for the Task Manager CLI.

## Framework

- **Google Test (gtest)** - Industry-standard C++ testing framework
- Version: 1.12.1

## Test Files

- `test_task.cpp` - Tests for Task class (8 tests)
- `test_taskmanager.cpp` - Tests for TaskManager class (13 tests)
- `test_colorutils.cpp` - Tests for ColorUtils (6 tests)

**Total: 27+ unit tests**

## Running Tests

```bash
# From build directory
cd build
cmake ..
make
ctest --output-on-failure
```

## Test Coverage

### Task Class (test_task.cpp)
- ✅ Task creation
- ✅ Getters and setters
- ✅ Task completion
- ✅ Due date functionality
- ✅ Overdue detection
- ✅ Priority/Status string conversion

### TaskManager Class (test_taskmanager.cpp)
- ✅ Adding tasks
- ✅ Finding tasks by ID
- ✅ Deleting tasks
- ✅ Marking complete
- ✅ Bulk operations
- ✅ Sorting functionality

### ColorUtils Class (test_colorutils.cpp)
- ✅ Color application
- ✅ Message formatting
- ✅ Enable/disable colors

## Adding New Tests

1. Create test file in `tests/` directory
2. Include gtest headers
3. Write TEST or TEST_F macros
4. Rebuild project
5. Run tests

Example:
```cpp
#include <gtest/gtest.h>
#include "YourClass.hpp"

TEST(YourClassTest, TestName) {
    // Arrange
    YourClass obj;
    
    // Act
    int result = obj.method();
    
    // Assert
    EXPECT_EQ(result, expected);
}
```
