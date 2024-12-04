
file(READ "${CMAKE_SOURCE_DIR}/src/font/Play-Chickens.otf" PLAY_CHICKENS_FONT HEX)
file(READ "${CMAKE_SOURCE_DIR}/src/font/Roboto-Regular.ttf" ROBOTO_REGULAR_FONT HEX)

string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," PLAY_CHICKENS_FONT_ARRAY ${PLAY_CHICKENS_FONT})
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," ROBOTO_REGULAR_FONT_ARRAY ${ROBOTO_REGULAR_FONT})

file(WRITE "${CMAKE_BINARY_DIR}/embedded_fonts.h" "
#pragma once
static const unsigned char PLAY_CHICKENS_FONT[] = {${PLAY_CHICKENS_FONT_ARRAY}};
static const unsigned int PLAY_CHICKENS_FONT_SIZE = sizeof(PLAY_CHICKENS_FONT);
static const unsigned char ROBOTO_REGULAR_FONT[] = {${ROBOTO_REGULAR_FONT_ARRAY}};
static const unsigned int ROBOTO_REGULAR_FONT_SIZE = sizeof(ROBOTO_REGULAR_FONT);
")