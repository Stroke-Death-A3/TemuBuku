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

# Add commands to generate font resources
configure_file(
    "${CMAKE_SOURCE_DIR}/font/Play-Chickens.otf"
    "${CMAKE_BINARY_DIR}/font/Play-Chickens.otf"
    COPYONLY
)
configure_file(
    "${CMAKE_SOURCE_DIR}/font/Roboto-Regular.ttf"
    "${CMAKE_BINARY_DIR}/font/Roboto-Regular.ttf"
    COPYONLY
)

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
    )
endif()

target_include_directories(TemuBuku PRIVATE
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/backends
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib  # Add local lib directory
    ${GLFW_INCLUDE_DIRS}
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