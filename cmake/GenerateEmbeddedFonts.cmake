# Function to convert a font file to a C array
function(embed_font OUTPUT_H OUTPUT_CPP FONT_PATH FONT_NAME)
    # Convert to native paths
    file(TO_NATIVE_PATH "${OUTPUT_H}" NATIVE_OUTPUT_H)
    file(TO_NATIVE_PATH "${OUTPUT_CPP}" NATIVE_OUTPUT_CPP)
    file(TO_NATIVE_PATH "${FONT_PATH}" NATIVE_FONT_PATH)
    
    # Create output directory
    get_filename_component(OUTPUT_DIR "${OUTPUT_H}" DIRECTORY)
    file(MAKE_DIRECTORY "${OUTPUT_DIR}")
    
    # Read font file as hex
    file(READ "${FONT_PATH}" FONT_DATA HEX)
    
    # Get file size
    file(SIZE "${FONT_PATH}" FONT_SIZE)
    
    # Convert hex to byte array (16 bytes per line)
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1,\n" FONT_BYTES "${FONT_DATA}")
    string(REGEX REPLACE ",\n$" "" FONT_BYTES "${FONT_BYTES}")
    
    # Generate header file
    file(WRITE "${OUTPUT_H}" "#pragma once\n\n")
    file(APPEND "${OUTPUT_H}" "extern const unsigned char ${FONT_NAME}[];\n")
    file(APPEND "${OUTPUT_H}" "extern const unsigned int ${FONT_NAME}_SIZE;\n")
    
    # Generate cpp file 
    file(WRITE "${OUTPUT_CPP}" "#include \"embedded_fonts.h\"\n\n")
    file(APPEND "${OUTPUT_CPP}" "const unsigned char ${FONT_NAME}[] = {\n")
    file(APPEND "${OUTPUT_CPP}" "${FONT_BYTES}\n")
    file(APPEND "${OUTPUT_CPP}" "};\n\n")
    file(APPEND "${OUTPUT_CPP}" "const unsigned int ${FONT_NAME}_SIZE = ${FONT_SIZE};\n")
endfunction()

# Ensure build directory exists
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}")

# Define font paths
set(FONTS_H "${CMAKE_BINARY_DIR}/embedded_fonts.h")
set(FONTS_CPP "${CMAKE_BINARY_DIR}/embedded_fonts.cpp") 
set(PLAY_CHICKENS_FONT "${CMAKE_SOURCE_DIR}/src/font/Play-Chickens.otf")
set(ROBOTO_FONT "${CMAKE_SOURCE_DIR}/src/font/Roboto-Regular.ttf")

# Generate Play Chickens font
embed_font("${FONTS_H}" "${FONTS_CPP}" "${PLAY_CHICKENS_FONT}" "PLAY_CHICKENS_FONT")

# Generate Roboto font (append to existing files)
set(TMP_H "${CMAKE_BINARY_DIR}/tmp.h")
set(TMP_CPP "${CMAKE_BINARY_DIR}/tmp.cpp")
embed_font("${TMP_H}" "${TMP_CPP}" "${ROBOTO_FONT}" "ROBOTO_REGULAR_FONT")

# Append Roboto font definitions
file(READ "${TMP_H}" ROBOTO_H)
file(READ "${TMP_CPP}" ROBOTO_CPP)
file(APPEND "${FONTS_H}" "\n${ROBOTO_H}")
file(APPEND "${FONTS_CPP}" "\n${ROBOTO_CPP}")

# Cleanup temp files
file(REMOVE "${TMP_H}" "${TMP_CPP}")