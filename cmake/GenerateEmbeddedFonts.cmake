# Function to convert a font file to a C array
function(embed_font OUTPUT_H OUTPUT_CPP FONT_PATH FONT_NAME)
    # Create output directory if it doesn't exist 
    get_filename_component(OUTPUT_DIR ${OUTPUT_H} DIRECTORY)
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
    
    # Convert to absolute paths
    get_filename_component(ABS_OUTPUT_H ${OUTPUT_H} ABSOLUTE)
    get_filename_component(ABS_OUTPUT_CPP ${OUTPUT_CPP} ABSOLUTE)
    get_filename_component(ABS_FONT_PATH ${FONT_PATH} ABSOLUTE)
    
    # Convert path separators
    file(TO_CMAKE_PATH ${ABS_FONT_PATH} FONT_PATH_FIXED)
    
    # Read the font file content as binary 
    file(READ ${FONT_PATH_FIXED} FONT_DATA HEX)
    
    # Convert hex string to comma-separated bytes
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " FONT_BYTES ${FONT_DATA})
    
    # Remove trailing comma and space
    string(REGEX REPLACE ", $" "" FONT_BYTES ${FONT_BYTES})
    
    # Get the file size
    file(SIZE ${FONT_PATH_FIXED} FONT_SIZE)
    
    # Clean up any unwanted newlines in size
    string(STRIP ${FONT_SIZE} FONT_SIZE)
    
    # Generate header content
    file(WRITE "${ABS_OUTPUT_H}" "#pragma once\n\n") 
    file(APPEND "${ABS_OUTPUT_H}" "extern const unsigned char ${FONT_NAME}[];\n")
    file(APPEND "${ABS_OUTPUT_H}" "extern const unsigned int ${FONT_NAME}_SIZE;\n")
    
    # Generate cpp content
    file(WRITE "${ABS_OUTPUT_CPP}" "#include \"embedded_fonts.h\"\n\n")
    file(APPEND "${ABS_OUTPUT_CPP}" "const unsigned char ${FONT_NAME}[] = {\n") 
    file(APPEND("${ABS_OUTPUT_CPP}" "    ${FONT_BYTES}\n")
    file(APPEND("${ABS_OUTPUT_CPP}" "};\n\n")
    # Fix font size output to prevent -n prefix
    file(APPEND("${ABS_OUTPUT_CPP}" "const unsigned int ${FONT_NAME}_SIZE = ${FONT_SIZE};\n")

endfunction()

# Make build directory
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}")

# Convert paths to absolute
get_filename_component(FONTS_H "${CMAKE_BINARY_DIR}/embedded_fonts.h" ABSOLUTE)
get_filename_component(FONTS_CPP "${CMAKE_BINARY_DIR}/embedded_fonts.cpp" ABSOLUTE)
get_filename_component(PLAY_CHICKENS_FONT "${CMAKE_SOURCE_DIR}/src/font/Play-Chickens.otf" ABSOLUTE)
get_filename_component(ROBOTO_FONT "${CMAKE_SOURCE_DIR}/src/font/Roboto-Regular.ttf" ABSOLUTE)

# Generate fonts
embed_font(
    "${FONTS_H}"
    "${FONTS_CPP}" 
    "${PLAY_CHICKENS_FONT}"
    "PLAY_CHICKENS_FONT"
)

# Generate Roboto font
embed_font(
    "${CMAKE_BINARY_DIR}/embedded_fonts.h.tmp"
    "${CMAKE_BINARY_DIR}/embedded_fonts.cpp.tmp"
    "${ROBOTO_FONT}"
    "ROBOTO_REGULAR_FONT"
)

# Merge temporary files
file(READ "${CMAKE_BINARY_DIR}/embedded_fonts.h.tmp" ROBOTO_H)
file(READ "${CMAKE_BINARY_DIR}/embedded_fonts.cpp.tmp" ROBOTO_CPP)

file(APPEND "${FONTS_H}" "\n${ROBOTO_H}")
file(APPEND "${FONTS_CPP}" "\n${ROBOTO_CPP}")

# Clean up temporary files
file(REMOVE "${CMAKE_BINARY_DIR}/embedded_fonts.h.tmp")
file(REMOVE "${CMAKE_BINARY_DIR}/embedded_fonts.cpp.tmp")