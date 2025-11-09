# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TaskManager VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Manually list all shared source files (exclude main.cpp and api_server.cpp)
set(SHARED_SOURCES
    src/Task.cpp
    src/TaskManager.cpp
    src/FileHandler.cpp
    src/CSVExporter.cpp
    src/ColorUtils.cpp
    src/MenuHandler.cpp
    src/TaskOperations.cpp
    src/InputHelper.cpp
    src/ConfigHandler.cpp
    src/SQLiteHandler.cpp
)

# SQLite library
find_library(SQLITE3_LIBRARY sqlite3)
if(NOT SQLITE3_LIBRARY)
    message(STATUS "SQLite3 not found, using system library")
    set(SQLITE3_LIBRARY sqlite3)
endif()

# Main CLI executable
add_executable(task_manager src/main.cpp ${SHARED_SOURCES})
target_link_libraries(task_manager ${SQLITE3_LIBRARY})

# API Server executable
add_executable(task_api_server src/api_server.cpp ${SHARED_SOURCES})
target_link_libraries(task_api_server ${SQLITE3_LIBRARY})

# Migration tool
add_executable(migrate_tool scripts/migrate_json_to_sqlite.cpp ${SHARED_SOURCES})
target_link_libraries(migrate_tool ${SQLITE3_LIBRARY})

# Enable testing
enable_testing()

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test executable
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(run_tests ${TEST_SOURCES} ${SHARED_SOURCES})
target_link_libraries(run_tests gtest_main ${SQLITE3_LIBRARY})

# Add tests
include(GoogleTest)
gtest_discover_tests(run_tests)