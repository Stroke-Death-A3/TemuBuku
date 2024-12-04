#pragma once
#include <string>

// External declarations of font data
extern const unsigned char PLAY_CHICKENS_FONT[16];
extern const unsigned int PLAY_CHICKENS_FONT_SIZE;
extern const unsigned char ROBOTO_REGULAR_FONT[16];
extern const unsigned int ROBOTO_REGULAR_FONT_SIZE;

struct EmbeddedFont {
    const unsigned char* data;
    const unsigned int size;
    bool isValid() const { return data != nullptr && size > 100; }
};

inline EmbeddedFont getEmbeddedFont(const char* fontName) {
    if (std::string(fontName) == "Play-Chickens.otf") {
        return {PLAY_CHICKENS_FONT, PLAY_CHICKENS_FONT_SIZE};
    }
    if (std::string(fontName) == "Roboto-Regular.ttf") {
        return {ROBOTO_REGULAR_FONT, ROBOTO_REGULAR_FONT_SIZE};
    }
    return {nullptr, 0};
}
