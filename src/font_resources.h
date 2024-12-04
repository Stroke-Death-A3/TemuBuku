#pragma once
#include <string>

inline std::string getFontPath(const char* fontName) {
    return std::string("font/") + fontName;
}
