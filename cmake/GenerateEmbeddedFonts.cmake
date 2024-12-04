# Function to convert a font file to a C array
function(embed_font OUTPUT_H OUTPUT_CPP FONT_PATH FONT_NAME)
    # Read the font file content
    file(READ ${FONT_PATH} FONT_DATA HEX)
    
    # Convert hex string to comma-separated bytes
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," FONT_BYTES ${FONT_DATA})
    
    # Get the file size
    file(SIZE ${FONT_PATH} FONT_SIZE)
    
    # Generate header content
    file(WRITE ${OUTPUT_H} "#pragma once\n\n")
    file(APPEND ${OUTPUT_H} "extern const unsigned char ${FONT_NAME}[];\n")
    file(APPEND ${OUTPUT_H} "extern const unsigned int ${FONT_NAME}_SIZE;\n")
    
    # Generate cpp content
    file(WRITE ${OUTPUT_CPP} "#include \"embedded_fonts.h\"\n\n")
    file(APPEND ${OUTPUT_CPP} "const unsigned char ${FONT_NAME}[] = {\n    ")
    file(APPEND ${OUTPUT_CPP} ${FONT_BYTES})
    file(APPEND ${OUTPUT_CPP} "\n};\n\n")
    file(APPEND ${OUTPUT_CPP} "const unsigned int ${FONT_NAME}_SIZE = ${FONT_SIZE};\n")
endfunction()

# Generate the embedded font files
embed_font(
    "${CMAKE_BINARY_DIR}/embedded_fonts.h"
    "${CMAKE_BINARY_DIR}/embedded_fonts.cpp"
    "${CMAKE_SOURCE_DIR}/src/font/Play-Chickens.otf"
    "PLAY_CHICKENS_FONT"
)

# Append Roboto Regular font data
embed_font(
    "${CMAKE_BINARY_DIR}/embedded_fonts.h.tmp"
    "${CMAKE_BINARY_DIR}/embedded_fonts.cpp.tmp"
    "${CMAKE_SOURCE_DIR}/src/font/Roboto-Regular.ttf" 
    "ROBOTO_REGULAR_FONT"
)

# Merge the temporary files
file(READ "${CMAKE_BINARY_DIR}/embedded_fonts.h.tmp" ROBOTO_H)
file(READ "${CMAKE_BINARY_DIR}/embedded_fonts.cpp.tmp" ROBOTO_CPP)

file(APPEND "${CMAKE_BINARY_DIR}/embedded_fonts.h" "\n${ROBOTO_H}")
file(APPEND "${CMAKE_BINARY_DIR}/embedded_fonts.cpp" "\n${ROBOTO_CPP}")

# Clean up temporary files
file(REMOVE "${CMAKE_BINARY_DIR}/embedded_fonts.h.tmp")
file(REMOVE "${CMAKE_BINARY_DIR}/embedded_fonts.cpp.tmp")