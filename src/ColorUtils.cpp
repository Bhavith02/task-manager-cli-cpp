#include "ColorUtils.hpp"

// Initialize static members
bool ColorUtils::colorsEnabled = true;

// Color codes
const string ColorUtils::RESET = "\033[0m";
const string ColorUtils::RED = "\033[31m";
const string ColorUtils::GREEN = "\033[32m";
const string ColorUtils::YELLOW = "\033[33m";
const string ColorUtils::BLUE = "\033[34m";
const string ColorUtils::MAGENTA = "\033[35m";
const string ColorUtils::CYAN = "\033[36m";
const string ColorUtils::WHITE = "\033[37m";
const string ColorUtils::BRIGHT_RED = "\033[91m";
const string ColorUtils::BRIGHT_GREEN = "\033[92m";
const string ColorUtils::BRIGHT_YELLOW = "\033[93m";
const string ColorUtils::BRIGHT_BLUE = "\033[94m";

// Background colors
const string ColorUtils::BG_RED = "\033[41m";
const string ColorUtils::BG_GREEN = "\033[42m";
const string ColorUtils::BG_YELLOW = "\033[43m";

// Text styles
const string ColorUtils::BOLD = "\033[1m";
const string ColorUtils::DIM = "\033[2m";
const string ColorUtils::UNDERLINE = "\033[4m";

void ColorUtils::enableColors() {
    colorsEnabled = true;
}

void ColorUtils::disableColors() {
    colorsEnabled = false;
}

bool ColorUtils::isColorsEnabled() {
    return colorsEnabled;
}

string ColorUtils::colorize(const string& text, const string& color) {
    if (!colorsEnabled) {
        return text;
    }
    return color + text + RESET;
}

string ColorUtils::success(const string& text) {
    return colorize("✓ " + text, BRIGHT_GREEN);
}

string ColorUtils::error(const string& text) {
    return colorize("✗ " + text, BRIGHT_RED);
}

string ColorUtils::warning(const string& text) {
    return colorize("⚠ " + text, BRIGHT_YELLOW);
}

string ColorUtils::info(const string& text) {
    return colorize("ℹ " + text, BRIGHT_BLUE);
}

string ColorUtils::highlight(const string& text) {
    return colorize(text, BOLD + CYAN);
}
