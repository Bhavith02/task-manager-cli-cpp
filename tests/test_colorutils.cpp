#include <gtest/gtest.h>
#include "ColorUtils.hpp"

TEST(ColorUtilsTest, ColorizeText) {
    std::string text = "Test";
    std::string colored = ColorUtils::colorize(text, ColorUtils::RED);
    
    // Should contain ANSI codes when enabled
    EXPECT_NE(colored, text);
    EXPECT_GT(colored.length(), text.length());
}

TEST(ColorUtilsTest, SuccessMessage) {
    std::string msg = "Success!";
    std::string result = ColorUtils::success(msg);
    
    EXPECT_NE(result.find("✓"), std::string::npos);
}

TEST(ColorUtilsTest, ErrorMessage) {
    std::string msg = "Error!";
    std::string result = ColorUtils::error(msg);
    
    EXPECT_NE(result.find("✗"), std::string::npos);
}

TEST(ColorUtilsTest, WarningMessage) {
    std::string msg = "Warning!";
    std::string result = ColorUtils::warning(msg);
    
    EXPECT_NE(result.find("⚠"), std::string::npos);
}

TEST(ColorUtilsTest, InfoMessage) {
    std::string msg = "Info!";
    std::string result = ColorUtils::info(msg);
    
    EXPECT_NE(result.find("ℹ"), std::string::npos);
}

TEST(ColorUtilsTest, DisableColors) {
    ColorUtils::disableColors();
    
    std::string text = "Test";
    std::string colored = ColorUtils::colorize(text, ColorUtils::RED);
    
    // Should return plain text when disabled
    EXPECT_EQ(colored, text);
    
    // Re-enable for other tests
    ColorUtils::enableColors();
}
