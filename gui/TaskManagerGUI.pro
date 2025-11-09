QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TaskManagerGUI
TEMPLATE = app

CONFIG += c++17

# Include paths
INCLUDEPATH += ../inc

# Source files
SOURCES += \
    src/main_gui.cpp \
    src/mainwindow.cpp \
    src/taskdialog.cpp \
    ../src/Task.cpp \
    ../src/TaskManager.cpp \
    ../src/FileHandler.cpp \
    ../src/CSVExporter.cpp \
    ../src/SQLiteHandler.cpp

# Header files
HEADERS += \
    src/mainwindow.h \
    src/taskdialog.h \
    ../inc/Task.hpp \
    ../inc/TaskManager.hpp \
    ../inc/FileHandler.hpp \
    ../inc/CSVExporter.hpp \
    ../inc/SQLiteHandler.hpp

# UI files
FORMS += \
    ui/mainwindow.ui \
    ui/taskdialog.ui

# Resources
RESOURCES += resources/resources.qrc

# SQLite
LIBS += -lsqlite3

# Output directory
DESTDIR = ../build

# Defines
DEFINES += QT_DEPRECATED_WARNINGS
