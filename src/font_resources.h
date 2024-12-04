
#pragma once
#include <string>

inline std::string getFontPath(const char* fontName) {
#ifdef _WIN32
    return std::string("font/") + fontName;
#else
    return std::string("font/") + fontName;
#endif
}
