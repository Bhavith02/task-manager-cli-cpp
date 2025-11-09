#ifndef CSVEXPORTER_HPP
#define CSVEXPORTER_HPP

#include "Task.hpp"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class CSVExporter {
private:
    string escapeCSV(const string& field) const;
    string formatDate(time_t timestamp) const;
    
public:
    bool exportToCSV(const vector<Task>& tasks, const string& filename);
    bool exportToCSVWithPath(const vector<Task>& tasks, const string& filepath);
};

#endif // CSVEXPORTER_HPP
