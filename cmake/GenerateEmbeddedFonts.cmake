# Function to convert a font file to a C array
function(embed_font OUTPUT_H OUTPUT_CPP FONT_PATH FONT_NAME)
    # Create output directory if it doesn't exist
    get_filename_component(OUTPUT_DIR ${OUTPUT_H} DIRECTORY)
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
    
    # Convert path separators
    file(TO_CMAKE_PATH ${FONT_PATH} FONT_PATH_FIXED)
    
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

# Convert paths to absolute
set(FONTS_H "${CMAKE_BINARY_DIR}/embedded_fonts.h")
set(FONTS_CPP "${CMAKE_BINARY_DIR}/embedded_fonts.cpp") 
set(PLAY_CHICKENS_FONT "${CMAKE_SOURCE_DIR}/src/font/Play-Chickens.otf")
set(ROBOTO_FONT "${CMAKE_SOURCE_DIR}/src/font/Roboto-Regular.ttf")

# Generate fonts
embed_font(
    ${FONTS_H}
    ${FONTS_CPP}
    ${PLAY_CHICKENS_FONT}
    "PLAY_CHICKENS_FONT"
)

# Generate temp files
embed_font(
    "${CMAKE_BINARY_DIR}/embedded_fonts.h.tmp"
    "${CMAKE_BINARY_DIR}/embedded_fonts.cpp.tmp"
    ${ROBOTO_FONT}
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