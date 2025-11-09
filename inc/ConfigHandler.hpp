#ifndef CONFIGHANDLER_HPP
#define CONFIGHANDLER_HPP

#include "Task.hpp"
#include <string>
#include <map>

using namespace std;

class ConfigHandler {
private:
    string configFilePath;
    map<string, string> settings;
    
    void loadDefaults();
    string trim(const string& str) const;
    
public:
    ConfigHandler(const string& filePath = "../data/config.ini");
    
    // Load and save
    bool loadConfig();
    bool saveConfig();
    
    // Getters
    bool getColorsEnabled() const;
    Priority getDefaultPriority() const;
    bool getAutoSaveEnabled() const;
    int getDefaultViewCount() const;
    
    // Setters
    void setColorsEnabled(bool enabled);
    void setDefaultPriority(Priority priority);
    void setAutoSaveEnabled(bool enabled);
    void setDefaultViewCount(int count);
    
    // Display
    void displaySettings() const;
};

#endif // CONFIGHANDLER_HPP
