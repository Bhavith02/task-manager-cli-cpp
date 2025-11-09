#include "CSVExporter.hpp"
#include <iostream>
#include <sstream>

string CSVExporter::escapeCSV(const string& field) const {
    // Check if field contains comma, quote, or newline
    bool needsEscape = false;
    for (char c : field) {
        if (c == ',' || c == '"' || c == '\n' || c == '\r') {
            needsEscape = true;
            break;
        }
    }
    
    if (!needsEscape) {
        return field;
    }
    
    // Escape quotes by doubling them and wrap in quotes
    string escaped = "\"";
    for (char c : field) {
        if (c == '"') {
            escaped += "\"\"";
        } else {
            escaped += c;
        }
    }
    escaped += "\"";
    return escaped;
}

string CSVExporter::formatDate(time_t timestamp) const {
    if (timestamp == 0) {
        return "";
    }
    
    char buffer[80];
    struct tm* timeinfo = localtime(&timestamp);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

bool CSVExporter::exportToCSV(const vector<Task>& tasks, const string& filename) {
    string filepath = "../data/" + filename;
    return exportToCSVWithPath(tasks, filepath);
}

bool CSVExporter::exportToCSVWithPath(const vector<Task>& tasks, const string& filepath) {
    ofstream file(filepath);
    
    if (!file.is_open()) {
        cerr << "Error: Could not create CSV file: " << filepath << endl;
        return false;
    }
    
    // Write CSV header
    file << "ID,Title,Description,Priority,Status,Created At,Due Date,Days Until Due,Is Overdue\n";
    
    // Write task data
    for (const auto& task : tasks) {
        file << task.getId() << ",";
        file << escapeCSV(task.getTitle()) << ",";
        file << escapeCSV(task.getDescription()) << ",";
        file << task.getPriorityString() << ",";
        file << task.getStatusString() << ",";
        file << formatDate(task.getCreatedAt()) << ",";
        file << formatDate(task.getDueDate()) << ",";
        
        // Days until due (or empty if no due date)
        if (task.hasDueDate()) {
            file << task.getDaysUntilDue();
        }
        file << ",";
        
        // Is overdue
        file << (task.isOverdue() ? "Yes" : "No");
        file << "\n";
    }
    
    file.close();
    return true;
}
