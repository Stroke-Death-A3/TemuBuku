# Function to convert a font file to a C array
function(embed_font OUTPUT_H OUTPUT_CPP FONT_PATH FONT_NAME)
    # Read the font file content as binary 
    file(READ ${FONT_PATH} FONT_DATA HEX)
    
    # Convert hex string to comma-separated bytes
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " FONT_BYTES ${FONT_DATA})
    
    # Remove trailing comma and space
    string(REGEX REPLACE ", $" "" FONT_BYTES ${FONT_BYTES})
    
    # Get the file size
    file(SIZE ${FONT_PATH} FONT_SIZE)
    
    # Clean up any unwanted newlines in size
    string(STRIP ${FONT_SIZE} FONT_SIZE)
    
    # Generate header content
    file(WRITE ${OUTPUT_H} "#pragma once\n\n")
    file(APPEND ${OUTPUT_H} "extern const unsigned char ${FONT_NAME}[];\n")
    file(APPEND ${OUTPUT_H} "extern const unsigned int ${FONT_NAME}_SIZE;\n")
    
    # Generate cpp content 
    file(WRITE ${OUTPUT_CPP} "#include \"embedded_fonts.h\"\n\n")
    file(APPEND ${OUTPUT_CPP} "const unsigned char ${FONT_NAME}[] = {\n")
    file(APPEND ${OUTPUT_CPP} "    ${FONT_BYTES}\n")
    file(APPEND ${OUTPUT_CPP} "};\n\n")
    file(APPEND ${OUTPUT_CPP} "const unsigned int ${FONT_NAME}_SIZE = ${FONT_SIZE};\n")
endfunction()

# Generate Play Chickens font
embed_font(
    "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.h"
    "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.cpp" 
    "${CMAKE_SOURCE_DIR}/src/font/Play-Chickens.otf"
    "PLAY_CHICKENS_FONT"
)

# Generate Roboto Regular font
embed_font(
    "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.h.tmp"
    "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.cpp.tmp"
    "${CMAKE_SOURCE_DIR}/src/font/Roboto-Regular.ttf"
    "ROBOTO_REGULAR_FONT"
)

# Merge Roboto font data
file(READ "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.h.tmp" ROBOTO_H)
file(READ "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.cpp.tmp" ROBOTO_CPP)

file(APPEND "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.h" "\n${ROBOTO_H}")
file(APPEND("${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.cpp" "\n${ROBOTO_CPP}")

# Clean up temporary files
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.h.tmp") 
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/embedded_fonts.cpp.tmp")