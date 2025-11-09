#ifndef COLORUTILS_HPP
#define COLORUTILS_HPP

#include <string>
#include <iostream>

using namespace std;

class ColorUtils {
private:
    static bool colorsEnabled;

public:
    // Color codes
    static const string RESET;
    static const string RED;
    static const string GREEN;
    static const string YELLOW;
    static const string BLUE;
    static const string MAGENTA;
    static const string CYAN;
    static const string WHITE;
    static const string BRIGHT_RED;
    static const string BRIGHT_GREEN;
    static const string BRIGHT_YELLOW;
    static const string BRIGHT_BLUE;
    
    // Background colors
    static const string BG_RED;
    static const string BG_GREEN;
    static const string BG_YELLOW;
    
    // Text styles
    static const string BOLD;
    static const string DIM;
    static const string UNDERLINE;
    
    // Enable/disable colors
    static void enableColors();
    static void disableColors();
    static bool isColorsEnabled();
    
    // Helper functions
    static string colorize(const string& text, const string& color);
    static string success(const string& text);
    static string error(const string& text);
    static string warning(const string& text);
    static string info(const string& text);
    static string highlight(const string& text);
};

#endif // COLORUTILS_HPP
