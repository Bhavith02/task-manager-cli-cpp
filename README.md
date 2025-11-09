# 📋 Task Manager CLI

> A professional command-line task management application built in modern C++ (C++17) with full CRUD operations, persistent storage, advanced analytics, and rich terminal UI.

[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Build System](https://img.shields.io/badge/Build-CMake-green.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Status](https://img.shields.io/badge/Status-Complete-brightgreen.svg)]()

---

## ✨ Features

### 📌 Core Functionality
- ✅ **CRUD Operations** - Create, Read, Update, Delete tasks
- ✅ **Smart Search** - Case-insensitive search in title and description
- ✅ **Advanced Filtering** - Filter by status, priority, overdue
- ✅ **Multiple Sorting** - 9 different sorting options
- ✅ **Due Date Management** - Set, update, and track due dates
- ✅ **Status Tracking** - `Pending` | `In Progress` | `Completed`
- ✅ **Priority Levels** - `Low` | `Medium` | `High` with color coding

### 💾 Data Management
- ✅ **JSON Persistence** - Automatic save/load with error handling
- ✅ **CSV Export** - Export all or filtered tasks to CSV
- ✅ **Bulk Operations** - Mass complete, delete, or modify tasks
- ✅ **Configuration System** - Customizable settings and preferences

### 📊 Analytics & Insights
- ✅ **Enhanced Statistics** - Completion rates, overdue analytics
- ✅ **Visual Progress Bars** - Color-coded completion indicators
- ✅ **Productivity Insights** - Smart recommendations
- ✅ **Task Distribution** - Status and priority breakdowns

### 🎨 User Interface
- ✅ **Color-Coded Output** - ANSI colors for better readability
- ✅ **Interactive Menus** - Easy navigation with submenus
- ✅ **Smart Validation** - Input validation with helpful errors
- ✅ **Cross-Platform** - Works on Windows, Linux, macOS

---

## 🚀 Quick Start

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install cmake g++ build-essential

# macOS
brew install cmake

# Fedora/RHEL
sudo dnf install cmake gcc-c++
```

### Installation

```bash
# Clone or navigate to project directory
cd /path/to/MyAIProject

# Build the project
mkdir -p build && cd build
cmake ..
make

# Run the application
./task_manager
```

---

## 📖 Usage Guide

### Main Menu

```
╔════════════════════════════════╗
║      TASK MANAGER CLI v1.0     ║
╚════════════════════════════════╝
    Total Tasks: 20

    1. ➕ Add New Task
    2. 📋 View Tasks
    3. ✏️  Update Task
    4. 🗑️  Delete Task
    5. ✅ Mark Task Complete
    6. 🔍 Search Tasks
    7. 📊 Statistics
    8. 📤 Export to CSV
    0. ⚡ Bulk Operations
    S. ⚙️  Settings
    9. 🚪 Exit
```

### Basic Operations

#### ➕ Adding a Task
1. Select **"Add New Task"**
2. Enter title: `"Complete project documentation"`
3. Enter description: `"Write comprehensive docs"`
4. Select priority: `1=Low`, `2=Medium`, `3=High`
5. Optional: Set due date

#### 🔍 Searching Tasks
1. Select **"Search Tasks"**
2. Enter keyword: `"project"`
3. View matching results

#### 📤 Exporting Data
1. Select **"Export to CSV"**
2. Choose export type:
     - All tasks
     - By status (**Pending** / **In Progress** / **Completed**)
     - By priority (**Low** / **Medium** / **High**)
3. Optionally customize filename
4. Find exported file in `data/` directory

---

## 📁 Project Structure

```
MyAIProject/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── Documentation.md            # Detailed technical documentation
├── .gitignore                  # Git ignore rules
├── data/                       # Application data
│   ├── tasks.json             # Task storage (auto-created)
│   ├── config.ini             # User settings (auto-created)
│   └── *.csv                  # Exported files
├── inc/                        # Header files (.hpp)
│   ├── Task.hpp               # Task class
│   ├── TaskManager.hpp        # Task management
│   ├── FileHandler.hpp        # JSON I/O
│   ├── CSVExporter.hpp        # CSV export
│   ├── ColorUtils.hpp         # Terminal colors
│   ├── MenuHandler.hpp        # Menu displays
│   ├── TaskOperations.hpp     # CRUD operations
│   ├── InputHelper.hpp        # Input utilities
│   └── ConfigHandler.hpp      # Configuration management
└── src/                        # Implementation files (.cpp)
        ├── main.cpp               # Application entry (52 lines!)
        ├── Task.cpp
        ├── TaskManager.cpp
        ├── FileHandler.cpp
        ├── CSVExporter.cpp
        ├── ColorUtils.cpp
        ├── MenuHandler.cpp
        ├── TaskOperations.cpp
        ├── InputHelper.cpp
        └── ConfigHandler.cpp
```

---

## ⚙️ Configuration

Configuration is stored in `data/config.ini`:

```ini
[Display]
colors_enabled=true          # Enable/disable ANSI colors
default_view_count=10        # Default tasks to display

[Defaults]
default_priority=MEDIUM      # Default priority for new tasks

[System]
auto_save=true               # Auto-save after operations
```

> **💡 Tip:** Modify settings via Settings menu (press `S` in main menu).

---

## 🎨 Features in Detail

### 📝 Task Properties

| Property | Description |
|----------|-------------|
| **ID** | Unique auto-incrementing identifier |
| **Title** | Task name *(required)* |
| **Description** | Detailed information *(required)* |
| **Priority** | `LOW`, `MEDIUM`, or `HIGH` |
| **Status** | `PENDING`, `IN_PROGRESS`, or `COMPLETED` |
| **Created At** | Automatic timestamp |
| **Due Date** | Optional deadline with countdown |

### 🔀 Sorting Options

- 🎯 By Priority (High to Low)
- 📅 By Due Date (Soonest First)
- 🕐 By Creation Date (Newest/Oldest)
- ✅ By Status (Pending → Completed)
- 🔤 By Title (A-Z or Z-A)
- 🔢 By ID (Ascending/Descending)

### ⚡ Bulk Operations

- ✅ **Mark All Complete** - Complete all pending tasks
- 🗑️ **Delete Completed** - Clean up finished tasks
- 🎯 **Change Priority** - Bulk priority updates
- 💣 **Delete All** - Nuclear option *(requires "DELETE ALL" confirmation)*

### 📊 Statistics Dashboard

- 📈 Completion rate percentage
- 🎨 Visual progress bar
- 📋 Status breakdown with counts
- 🎯 Priority distribution
- ⚠️ Overdue task analytics
- 💡 Productivity insights and recommendations

---

## 🔧 Advanced Usage

### Custom Build Options

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Clean build
rm -rf build && mkdir build && cd build && cmake .. && make
```

### Data Management

```bash
# Backup your tasks
cp data/tasks.json data/tasks_backup_$(date +%Y%m%d).json

# View task data
cat data/tasks.json

# Export to CSV for spreadsheet analysis
# Use option 8 in the application
```

---

## 📊 Example Use Cases

### 👤 Personal Task Management
- ✓ Daily to-do lists
- ✓ Project tracking
- ✓ Habit tracking
- ✓ Goal setting

### 💼 Work/Professional
- ✓ Sprint planning
- ✓ Bug tracking
- ✓ Feature requests
- ✓ Meeting action items

### 🎓 Academic
- ✓ Assignment tracking
- ✓ Study schedules
- ✓ Research tasks
- ✓ Project milestones

---

## 🛠️ Development

### 📊 Code Statistics

```
Total Lines: ~3,500+
Files: 18 (9 .hpp + 9 .cpp)
Classes: 8 main classes
Functions: 50+ member functions
Features: 18 major features
```

### 🏗️ Design Principles

- ✅ SOLID principles applied
- ✅ Separation of concerns
- ✅ Single Responsibility per class
- ✅ Clean architecture
- ✅ Modern C++ practices

### 🔧 Key Technologies

| Technology | Usage |
|------------|-------|
| **C++17** | Standard library |
| **STL** | Containers (vector, map) |
| **Lambda Expressions** | Functional programming |
| **Smart Pointers** | Memory management patterns |
| **ANSI Escape Codes** | Terminal colors |
| **Manual JSON Parsing** | Learning exercise |

---

## 🎓 Learning Outcomes

This project demonstrates:

| Concept | Description |
|---------|-------------|
| ✅ **Object-oriented programming** | Classes, inheritance, polymorphism |
| ✅ **File I/O operations** | Reading, writing, error handling |
| ✅ **Data structures** | Vectors, maps, algorithms |
| ✅ **Algorithm design** | Sorting, searching, filtering |
| ✅ **Memory management** | RAII, smart pointers |
| ✅ **Cross-platform development** | Platform-independent code |
| ✅ **User interface design** | Menus, colors, validation |
| ✅ **Software architecture** | Modular, maintainable code |
| ✅ **Build systems** | CMake configuration |
| ✅ **Code organization** | Clean project structure |

---

## 🐛 Troubleshooting

### 🔨 Build Issues

<details>
<summary><b>Problem: CMake not found</b></summary>

```bash
# Install CMake
sudo apt-get install cmake  # Ubuntu/Debian
brew install cmake          # macOS
```
</details>

<details>
<summary><b>Problem: Compiler errors</b></summary>

```bash
# Ensure C++17 support
g++ --version  # Should be 7.0 or higher
```
</details>

### 🚀 Runtime Issues

<details>
<summary><b>Problem: Colors not displaying</b></summary>

```bash
# Check terminal support
echo $TERM

# Disable colors in settings (press S in menu)
# Or edit config.ini: colors_enabled=false
```
</details>

<details>
<summary><b>Problem: Data file not found</b></summary>

```bash
# Ensure data directory exists
mkdir -p data

# Run from build directory
cd build && ./task_manager
```
</details>

---

## 📝 License

This project is open source and available under the **MIT License**.

---

## 👨‍💻 Author

Created as a learning project to master modern C++ development practices.

**Technologies Used:**
- C++17
- CMake 3.10+
- STL
- ANSI Terminal Colors
- JSON (manual implementation)
- CSV Export

---

## 🙏 Acknowledgments

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/)
- *Modern Effective C++* by Scott Meyers
- [CMake Documentation](https://cmake.org/documentation/)
- ANSI Escape Code Standards

---

## 📞 Support

For issues, questions, or suggestions:

- 📖 Check `Documentation.md` for detailed technical information
- 💬 Review code comments for implementation details
- ⚙️ Use settings menu to customize behavior

---

## 🔮 Future Enhancements

Potential additions:

- [ ] Task dependencies
- [ ] Recurring tasks
- [ ] Cloud sync
- [ ] Mobile companion app
- [ ] Calendar integration
- [ ] Team collaboration features
- [ ] Task templates
- [ ] Pomodoro timer integration

---

<div align="center">

**Last Updated:** January 2025  
**Version:** 1.0  
**Status:** ✅ Complete and Fully Functional

---

Made with ❤️ and C++

</div>
