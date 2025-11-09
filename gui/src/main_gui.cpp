#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application info
    app.setApplicationName("Task Manager Pro");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("TaskManager");
    
    // Create and show main window
    MainWindow window;
    window.show();
    
    return app.exec();
}
