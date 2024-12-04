#pragma once
#include <string>

const unsigned char PLAY_CHICKENS_FONT[] = {/* font data */};
const unsigned int PLAY_CHICKENS_FONT_SIZE = sizeof(PLAY_CHICKENS_FONT);

const unsigned char ROBOTO_REGULAR_FONT[] = {/* font data */};
const unsigned int ROBOTO_REGULAR_FONT_SIZE = sizeof(ROBOTO_REGULAR_FONT);

struct EmbeddedFont {
    const unsigned char* data;
    const unsigned int size;
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
