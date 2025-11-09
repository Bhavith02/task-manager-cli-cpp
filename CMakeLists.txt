# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(TaskManager VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Collect all source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(task_manager ${SOURCES})

# Compiler warnings (optional but recommended)
if(MSVC)
    target_compile_options(task_manager PRIVATE /W4)
else()
    target_compile_options(task_manager PRIVATE -Wall -Wextra -pedantic)
endif()

# Set output directory
set_target_properties(task_manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
)

# Print configuration info
message(STATUS "==================================")
message(STATUS "Task Manager CLI - Build Configuration")
message(STATUS "==================================")
message(STATUS "CMake Version: ${CMAKE_VERSION}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Source Directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "Binary Directory: ${PROJECT_BINARY_DIR}")
message(STATUS "==================================")


# cd /data/home/sbhavith/practice/MyAIProject
# mkdir -p build && cd build
# cmake ..
# make
# ./task_manager