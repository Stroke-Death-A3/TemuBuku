cmake_minimum_required(VERSION 3.20)
project(TemuBuku)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

# Define font directory paths
set(FONT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/font")
set(FONT_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/font")

# Create font directory in build
file(MAKE_DIRECTORY ${FONT_BUILD_DIR})

# Copy font files if they exist
if(EXISTS "${FONT_DIR}/Play-Chickens.otf")
    configure_file("${FONT_DIR}/Play-Chickens.otf" "${FONT_BUILD_DIR}/Play-Chickens.otf" COPYONLY)
else()
    message(FATAL_ERROR "Font file Play-Chickens.otf not found in ${FONT_DIR}")
endif()

if(EXISTS "${FONT_DIR}/Roboto-Regular.ttf")
    configure_file("${FONT_DIR}/Roboto-Regular.ttf" "${FONT_BUILD_DIR}/Roboto-Regular.ttf" COPYONLY)
else()
    message(FATAL_ERROR "Font file Roboto-Regular.ttf not found in ${FONT_DIR}")
endif()

# Generate embedded font headers first
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/embedded_fonts.h"
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_SOURCE_DIR}/cmake/GenerateEmbeddedFonts.cmake"
    DEPENDS "${FONT_DIR}/Play-Chickens.otf" "${FONT_DIR}/Roboto-Regular.ttf"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Create the custom target for font generation
add_custom_target(generate_embedded_fonts DEPENDS "${CMAKE_BINARY_DIR}/embedded_fonts.h")

# Create the main executable target
if(WIN32)
    add_executable(TemuBuku
        src/main.cpp
        src/windows_main.cpp  # Add Windows-specific entry point
        src/font_resources.h  # Add this new header
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
        "${CMAKE_BINARY_DIR}/embedded_fonts.h"  # Add the generated header
    )
else()
    add_executable(TemuBuku
        src/main.cpp
        src/font_resources.h  # Add this new header
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
        "${CMAKE_BINARY_DIR}/embedded_fonts.h"  # Add the generated header
    )
endif()

# Now add the dependency after the target exists
add_dependencies(TemuBuku generate_embedded_fonts)

target_include_directories(TemuBuku PRIVATE
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/backends
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib  # Add local lib directory
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(TemuBuku PRIVATE
    glfw
    OpenGL::GL
    $<$<PLATFORM_ID:Windows>:opengl32>
    $<$<PLATFORM_ID:Windows>:gdi32>
    $<$<PLATFORM_ID:Windows>:user32>
)

if(WIN32)
    set_target_properties(TemuBuku PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    
    target_compile_definitions(TemuBuku PRIVATE
        UNICODE
        _UNICODE
        _CRT_SECURE_NO_WARNINGS
    )
endif()