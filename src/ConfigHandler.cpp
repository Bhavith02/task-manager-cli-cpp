#include "ConfigHandler.hpp"
#include "ColorUtils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

ConfigHandler::ConfigHandler(const string& filePath) : configFilePath(filePath) {
    loadDefaults();
    loadConfig();
}

void ConfigHandler::loadDefaults() {
    settings["colors_enabled"] = "true";
    settings["default_priority"] = "MEDIUM";
    settings["auto_save"] = "true";
    settings["default_view_count"] = "10";
}

string ConfigHandler::trim(const string& str) const {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

bool ConfigHandler::loadConfig() {
    ifstream file(configFilePath);
    if (!file.is_open()) {
        // Config doesn't exist, create with defaults
        return saveConfig();
    }
    
    string line;
    while (getline(file, line)) {
        line = trim(line);
        
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }
        
        // Parse key=value
        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = trim(line.substr(0, pos));
            string value = trim(line.substr(pos + 1));
            settings[key] = value;
        }
    }
    
    file.close();
    return true;
}

bool ConfigHandler::saveConfig() {
    ofstream file(configFilePath);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# Task Manager Configuration File\n";
    file << "# Auto-generated - Edit with care\n\n";
    
    file << "[Display]\n";
    file << "colors_enabled=" << settings["colors_enabled"] << "\n";
    file << "default_view_count=" << settings["default_view_count"] << "\n\n";
    
    file << "[Defaults]\n";
    file << "default_priority=" << settings["default_priority"] << "\n\n";
    
    file << "[System]\n";
    file << "auto_save=" << settings["auto_save"] << "\n";
    
    file.close();
    return true;
}

bool ConfigHandler::getColorsEnabled() const {
    return settings.at("colors_enabled") == "true";
}

Priority ConfigHandler::getDefaultPriority() const {
    string priority = settings.at("default_priority");
    if (priority == "LOW") return Priority::LOW;
    if (priority == "HIGH") return Priority::HIGH;
    return Priority::MEDIUM;
}

bool ConfigHandler::getAutoSaveEnabled() const {
    return settings.at("auto_save") == "true";
}

int ConfigHandler::getDefaultViewCount() const {
    return stoi(settings.at("default_view_count"));
}

void ConfigHandler::setColorsEnabled(bool enabled) {
    settings["colors_enabled"] = enabled ? "true" : "false";
    ColorUtils::enableColors();
    if (!enabled) {
        ColorUtils::disableColors();
    }
}

void ConfigHandler::setDefaultPriority(Priority priority) {
    switch (priority) {
        case Priority::LOW: settings["default_priority"] = "LOW"; break;
        case Priority::MEDIUM: settings["default_priority"] = "MEDIUM"; break;
        case Priority::HIGH: settings["default_priority"] = "HIGH"; break;
    }
}

void ConfigHandler::setAutoSaveEnabled(bool enabled) {
    settings["auto_save"] = enabled ? "true" : "false";
}

void ConfigHandler::setDefaultViewCount(int count) {
    settings["default_view_count"] = to_string(count);
}

void ConfigHandler::displaySettings() const {
    cout << "\n" << ColorUtils::colorize("╔════════════════════════════════════════╗", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) 
         << ColorUtils::highlight("        CURRENT SETTINGS              ")
         << ColorUtils::colorize("║", ColorUtils::BRIGHT_BLUE) << endl;
    cout << ColorUtils::colorize("╚════════════════════════════════════════╝", ColorUtils::BRIGHT_BLUE) << endl;
    
    cout << "\n" << ColorUtils::BOLD << "Display Settings:" << ColorUtils::RESET << endl;
    cout << "  Colors Enabled:     " << (getColorsEnabled() ? 
        ColorUtils::colorize("✓ Yes", ColorUtils::GREEN) : 
        ColorUtils::colorize("✗ No", ColorUtils::RED)) << endl;
    cout << "  Default View Count: " << getDefaultViewCount() << endl;
    
    cout << "\n" << ColorUtils::BOLD << "Default Values:" << ColorUtils::RESET << endl;
    cout << "  Default Priority:   " << settings.at("default_priority") << endl;
    
    cout << "\n" << ColorUtils::BOLD << "System Settings:" << ColorUtils::RESET << endl;
    cout << "  Auto-Save:          " << (getAutoSaveEnabled() ? 
        ColorUtils::colorize("✓ Enabled", ColorUtils::GREEN) : 
        ColorUtils::colorize("✗ Disabled", ColorUtils::RED)) << endl;
    
    cout << "\n" << ColorUtils::colorize("Config file: " + configFilePath, ColorUtils::DIM) << endl;
}
