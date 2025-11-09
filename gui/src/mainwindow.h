#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include "../../inc/TaskManager.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddTask();
    void onEditTask();
    void onDeleteTask();
    void onMarkComplete();
    void onRefresh();
    void onSearch(const QString &text);
    void onFilterChanged(int index);
    void onExportCSV();
    void onShowStatistics();
    void onAbout();
    void onTableItemDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    TaskManager taskManager;
    
    void setupUI();
    void setupConnections();
    void loadTasks();
    void updateTaskTable();
    void updateStatistics();
    QString getPriorityColor(Priority priority);
    QString getStatusColor(Status status);
    int getSelectedTaskId();
};

#endif // MAINWINDOW_H
