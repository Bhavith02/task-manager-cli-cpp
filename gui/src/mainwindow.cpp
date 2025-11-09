#include "mainwindow.h"
#include "taskdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUI();
    setupConnections();
    loadTasks();
    updateTaskTable();
    updateStatistics();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    setWindowTitle("Task Manager Pro");
    setMinimumSize(1000, 600);
    
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create toolbar
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    QPushButton *btnAdd = new QPushButton("➕ Add Task", this);
    QPushButton *btnEdit = new QPushButton("✏️ Edit", this);
    QPushButton *btnDelete = new QPushButton("🗑️ Delete", this);
    QPushButton *btnComplete = new QPushButton("✅ Complete", this);
    QPushButton *btnRefresh = new QPushButton("🔄 Refresh", this);
    QPushButton *btnExport = new QPushButton("📤 Export", this);
    QPushButton *btnStats = new QPushButton("📊 Stats", this);
    
    toolbarLayout->addWidget(btnAdd);
    toolbarLayout->addWidget(btnEdit);
    toolbarLayout->addWidget(btnDelete);
    toolbarLayout->addWidget(btnComplete);
    toolbarLayout->addWidget(btnRefresh);
    toolbarLayout->addWidget(btnExport);
    toolbarLayout->addWidget(btnStats);
    toolbarLayout->addStretch();
    
    // Create search bar
    QLineEdit *searchBox = new QLineEdit(this);
    searchBox->setPlaceholderText("🔍 Search tasks...");
    toolbarLayout->addWidget(searchBox);
    
    // Create filter
    QComboBox *filterCombo = new QComboBox(this);
    filterCombo->addItem("All Tasks");
    filterCombo->addItem("Pending");
    filterCombo->addItem("In Progress");
    filterCombo->addItem("Completed");
    toolbarLayout->addWidget(filterCombo);
    
    mainLayout->addLayout(toolbarLayout);
    
    // Create table
    QTableWidget *taskTable = new QTableWidget(this);
    taskTable->setColumnCount(6);
    taskTable->setHorizontalHeaderLabels({"ID", "Title", "Priority", "Status", "Due Date", "Description"});
    taskTable->horizontalHeader()->setStretchLastSection(true);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    taskTable->setAlternatingRowColors(true);
    
    mainLayout->addWidget(taskTable);
    
    // Create status bar with statistics
    QLabel *statsLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statsLabel);
    
    // Connect signals
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddTask);
    connect(btnEdit, &QPushButton::clicked, this, &MainWindow::onEditTask);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteTask);
    connect(btnComplete, &QPushButton::clicked, this, &MainWindow::onMarkComplete);
    connect(btnRefresh, &QPushButton::clicked, this, &MainWindow::onRefresh);
    connect(btnExport, &QPushButton::clicked, this, &MainWindow::onExportCSV);
    connect(btnStats, &QPushButton::clicked, this, &MainWindow::onShowStatistics);
    connect(searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearch);
    connect(filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onFilterChanged);
    connect(taskTable, &QTableWidget::cellDoubleClicked, 
            this, &MainWindow::onTableItemDoubleClicked);
}

void MainWindow::loadTasks() {
    // Tasks are loaded automatically by TaskManager
}

void MainWindow::updateTaskTable() {
    QTableWidget *table = findChild<QTableWidget*>();
    if (!table) return;
    
    table->setRowCount(0);
    
    auto& tasks = taskManager.getAllTasks();
    int row = 0;
    
    for (const auto& task : tasks) {
        table->insertRow(row);
        
        // ID
        table->setItem(row, 0, new QTableWidgetItem(QString::number(task.getId())));
        
        // Title
        QTableWidgetItem *titleItem = new QTableWidgetItem(QString::fromStdString(task.getTitle()));
        titleItem->setFont(QFont("Arial", 10, QFont::Bold));
        table->setItem(row, 1, titleItem);
        
        // Priority
        QTableWidgetItem *priorityItem = new QTableWidgetItem(
            QString::fromStdString(task.getPriorityString()));
        priorityItem->setBackground(QColor(getPriorityColor(task.getPriority())));
        table->setItem(row, 2, priorityItem);
        
        // Status
        QTableWidgetItem *statusItem = new QTableWidgetItem(
            QString::fromStdString(task.getStatusString()));
        statusItem->setBackground(QColor(getStatusColor(task.getStatus())));
        table->setItem(row, 3, statusItem);
        
        // Due Date
        QString dueDate = QString::fromStdString(task.getDueDateString());
        table->setItem(row, 4, new QTableWidgetItem(dueDate));
        
        // Description
        table->setItem(row, 5, new QTableWidgetItem(
            QString::fromStdString(task.getDescription())));
        
        row++;
    }
    
    table->resizeColumnsToContents();
    updateStatistics();
}

void MainWindow::updateStatistics() {
    auto& tasks = taskManager.getAllTasks();
    int total = tasks.size();
    int pending = 0, inProgress = 0, completed = 0;
    
    for (const auto& task : tasks) {
        switch (task.getStatus()) {
            case Status::PENDING: pending++; break;
            case Status::IN_PROGRESS: inProgress++; break;
            case Status::COMPLETED: completed++; break;
        }
    }
    
    QString stats = QString("Total: %1 | Pending: %2 | In Progress: %3 | Completed: %4")
        .arg(total).arg(pending).arg(inProgress).arg(completed);
    
    QLabel *statsLabel = statusBar()->findChild<QLabel*>();
    if (statsLabel) {
        statsLabel->setText(stats);
    }
}

QString MainWindow::getPriorityColor(Priority priority) {
    switch (priority) {
        case Priority::LOW: return "#90EE90";    // Light green
        case Priority::MEDIUM: return "#FFD700"; // Gold
        case Priority::HIGH: return "#FF6B6B";   // Light red
    }
    return "#FFFFFF";
}

QString MainWindow::getStatusColor(Status status) {
    switch (status) {
        case Status::PENDING: return "#FFE4B5";     // Moccasin
        case Status::IN_PROGRESS: return "#87CEEB"; // Sky blue
        case Status::COMPLETED: return "#98FB98";   // Pale green
    }
    return "#FFFFFF";
}

int MainWindow::getSelectedTaskId() {
    QTableWidget *table = findChild<QTableWidget*>();
    if (!table) return -1;
    
    QList<QTableWidgetItem*> selected = table->selectedItems();
    if (selected.isEmpty()) return -1;
    
    int row = selected.first()->row();
    return table->item(row, 0)->text().toInt();
}

void MainWindow::onAddTask() {
    TaskDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Get task data from dialog
        QString title = dialog.getTitle();
        QString desc = dialog.getDescription();
        Priority priority = dialog.getPriority();
        
        taskManager.addTask(title.toStdString(), desc.toStdString(), priority);
        updateTaskTable();
        
        QMessageBox::information(this, "Success", "Task added successfully!");
    }
}

void MainWindow::onEditTask() {
    int taskId = getSelectedTaskId();
    if (taskId < 0) {
        QMessageBox::warning(this, "Warning", "Please select a task to edit.");
        return;
    }
    
    Task* task = taskManager.findTaskById(taskId);
    if (!task) return;
    
    TaskDialog dialog(this, task);
    if (dialog.exec() == QDialog::Accepted) {
        task->setTitle(dialog.getTitle().toStdString());
        task->setDescription(dialog.getDescription().toStdString());
        task->setPriority(dialog.getPriority());
        task->setStatus(dialog.getStatus());
        
        taskManager.saveToFile();
        updateTaskTable();
        
        QMessageBox::information(this, "Success", "Task updated successfully!");
    }
}

void MainWindow::onDeleteTask() {
    int taskId = getSelectedTaskId();
    if (taskId < 0) {
        QMessageBox::warning(this, "Warning", "Please select a task to delete.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete", "Are you sure you want to delete this task?",
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        taskManager.deleteTask(taskId);
        updateTaskTable();
        QMessageBox::information(this, "Success", "Task deleted successfully!");
    }
}

void MainWindow::onMarkComplete() {
    int taskId = getSelectedTaskId();
    if (taskId < 0) {
        QMessageBox::warning(this, "Warning", "Please select a task to mark complete.");
        return;
    }
    
    taskManager.markTaskComplete(taskId);
    updateTaskTable();
    QMessageBox::information(this, "Success", "Task marked as complete! 🎉");
}

void MainWindow::onRefresh() {
    updateTaskTable();
}

void MainWindow::onSearch(const QString &text) {
    // Simple search implementation
    QTableWidget *table = findChild<QTableWidget*>();
    if (!table) return;
    
    for (int row = 0; row < table->rowCount(); row++) {
        bool match = false;
        for (int col = 1; col < table->columnCount(); col++) {
            if (table->item(row, col)->text().contains(text, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        table->setRowHidden(row, !match);
    }
}

void MainWindow::onFilterChanged(int index) {
    QTableWidget *table = findChild<QTableWidget*>();
    if (!table) return;
    
    for (int row = 0; row < table->rowCount(); row++) {
        bool show = true;
        if (index > 0) {
            QString status = table->item(row, 3)->text();
            switch (index) {
                case 1: show = (status == "Pending"); break;
                case 2: show = (status == "In Progress"); break;
                case 3: show = (status == "Completed"); break;
            }
        }
        table->setRowHidden(row, !show);
    }
}

void MainWindow::onExportCSV() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export Tasks",
        "../data/tasks_export.csv", "CSV Files (*.csv)");
    
    if (!fileName.isEmpty()) {
        if (taskManager.exportToCSV(fileName.toStdString())) {
            QMessageBox::information(this, "Success", 
                "Tasks exported successfully to:\n" + fileName);
        } else {
            QMessageBox::critical(this, "Error", "Failed to export tasks!");
        }
    }
}

void MainWindow::onShowStatistics() {
    // Show detailed statistics dialog
    auto& tasks = taskManager.getAllTasks();
    int total = tasks.size();
    int pending = 0, inProgress = 0, completed = 0;
    int low = 0, medium = 0, high = 0;
    
    for (const auto& task : tasks) {
        switch (task.getStatus()) {
            case Status::PENDING: pending++; break;
            case Status::IN_PROGRESS: inProgress++; break;
            case Status::COMPLETED: completed++; break;
        }
        switch (task.getPriority()) {
            case Priority::LOW: low++; break;
            case Priority::MEDIUM: medium++; break;
            case Priority::HIGH: high++; break;
        }
    }
    
    QString stats = QString(
        "📊 Task Statistics\n\n"
        "Total Tasks: %1\n\n"
        "By Status:\n"
        "  • Pending: %2\n"
        "  • In Progress: %3\n"
        "  • Completed: %4\n\n"
        "By Priority:\n"
        "  • Low: %5\n"
        "  • Medium: %6\n"
        "  • High: %7\n\n"
        "Completion Rate: %8%"
    ).arg(total).arg(pending).arg(inProgress).arg(completed)
     .arg(low).arg(medium).arg(high)
     .arg(total > 0 ? (completed * 100 / total) : 0);
    
    QMessageBox::information(this, "Statistics", stats);
}

void MainWindow::onTableItemDoubleClicked(int row, int column) {
    Q_UNUSED(column);
    // Double-click to edit
    onEditTask();
}
