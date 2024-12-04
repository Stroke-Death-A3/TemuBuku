# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw

#CXX = g++
CXX = clang++ -O3

EXE = TemuBuku
MKDIR_P = mkdir -p
IMGUI_DIR = ../
BIN_DIR = ./bin
BUILD_DIR = ./build
SOURCES = main.cpp
SOURCES += $(BUILD_DIR)/embedded_fonts.cpp  # Add this line
SOURCES += $(IMGUI_DIR)/imgui/imgui.cpp $(IMGUI_DIR)/imgui/imgui_demo.cpp $(IMGUI_DIR)/imgui/imgui_draw.cpp $(IMGUI_DIR)/imgui/imgui_tables.cpp $(IMGUI_DIR)/imgui/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))
UNAME_S := $(shell uname -s)

LINUX_GL_LIBS = -lGL
CXXFLAGS = -std=c++17 -I$(IMGUI_DIR)/imgui -I$(IMGUI_DIR)/backends  -I$(IMGUI_DIR)/lib
CXXFLAGS += -g -Wall -Wformat
CXXFLAGS += -I$(BUILD_DIR)
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------
## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`
	CXXFLAGS += `pkg-config --cflags glfw3` 
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw
	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
    ifeq ($(CXX),cl.exe)
        ECHO_MESSAGE = "Visual Studio"
        LIBS += glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib
        _CXXFLAGS = $(subst -O3,,$(CXXFLAGS))
        CXXFLAGS = $(_CXXFLAGS) /O2 /EHsc /MD /I"$(VCPKG_ROOT)/installed/x64-windows/include"
        CFLAGS = $(CXXFLAGS)
    else
        ECHO_MESSAGE = "MinGW"
        LIBS += -L/mingw64/lib -lglfw3 -lgdi32 -lopengl32 -limm32 -lglew32
        CXXFLAGS += -I/mingw64/include
        CFLAGS = $(CXXFLAGS)
    endif
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

.PHONY: all clean

all: dirs $(BIN_DIR)/$(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(BUILD_DIR)/font

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(IMGUI_DIR)/imgui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Add rule to generate embedded fonts
$(BUILD_DIR)/embedded_fonts.h $(BUILD_DIR)/embedded_fonts.cpp:
	@echo "Generating embedded fonts files..."
	@echo "#pragma once" > $(BUILD_DIR)/embedded_fonts.h
	@echo "extern const unsigned char PLAY_CHICKENS_FONT[];" >> $(BUILD_DIR)/embedded_fonts.h
	@echo "extern const unsigned int PLAY_CHICKENS_FONT_SIZE;" >> $(BUILD_DIR)/embedded_fonts.h
	@echo "extern const unsigned char ROBOTO_REGULAR_FONT[];" >> $(BUILD_DIR)/embedded_fonts.h
	@echo "extern const unsigned int ROBOTO_REGULAR_FONT_SIZE;" >> $(BUILD_DIR)/embedded_fonts.h
	
	@echo "#include \"embedded_fonts.h\"" > $(BUILD_DIR)/embedded_fonts.cpp
	@echo "const unsigned char PLAY_CHICKENS_FONT[] = {" >> $(BUILD_DIR)/embedded_fonts.cpp
	@xxd -i < $(IMGUI_DIR)/src/font/Play-Chickens.otf | grep -v "unsigned" | grep -v ".*_len.*=.*" >> $(BUILD_DIR)/embedded_fonts.cpp
	@echo "};" >> $(BUILD_DIR)/embedded_fonts.cpp
	@echo -n "const unsigned int PLAY_CHICKENS_FONT_SIZE = " >> $(BUILD_DIR)/embedded_fonts.cpp
	@wc -c < $(IMGUI_DIR)/src/font/Play-Chickens.otf >> $(BUILD_DIR)/embedded_fonts.cpp
	@echo ";" >> $(BUILD_DIR)/embedded_fonts.cpp
	
	@echo "const unsigned char ROBOTO_REGULAR_FONT[] = {" >> $(BUILD_DIR)/embedded_fonts.cpp
	@xxd -i < $(IMGUI_DIR)/src/font/Roboto-Regular.ttf | grep -v "unsigned" | grep -v ".*_len.*=.*" >> $(BUILD_DIR)/embedded_fonts.cpp
	@echo "};" >> $(BUILD_DIR)/embedded_fonts.cpp
	@echo -n "const unsigned int ROBOTO_REGULAR_FONT_SIZE = " >> $(BUILD_DIR)/embedded_fonts.cpp
	@wc -c < $(IMGUI_DIR)/src/font/Roboto-Regular.ttf >> $(BUILD_DIR)/embedded_fonts.cpp
	@echo ";" >> $(BUILD_DIR)/embedded_fonts.cpp

# Update object dependencies
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/embedded_fonts.o: $(BUILD_DIR)/embedded_fonts.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJS): $(BUILD_DIR)/embedded_fonts.h

$(BIN_DIR)/$(EXE): $(OBJS) $(BUILD_DIR)/embedded_fonts.o
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

# Update clean target to remove generated files
clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(BIN_DIR)/*

# ...existing code...

embedded_fonts.o: embedded_fonts.cpp embedded_fonts.h
	$(CXX) $(CXXFLAGS) -c $< -o $(BUILD_DIR)/$@

# Generate embedded fonts
embedded_fonts.cpp embedded_fonts.h: font/Play-Chickens.otf font/Roboto-Regular.ttf
	@echo "Generating embedded fonts..."
	@echo "#pragma once" > embedded_fonts.h
	@echo "extern const unsigned char PLAY_CHICKENS_FONT[];" >> embedded_fonts.h
	@echo "extern const unsigned int PLAY_CHICKENS_FONT_SIZE;" >> embedded_fonts.h
	@echo "extern const unsigned char ROBOTO_REGULAR_FONT[];" >> embedded_fonts.h 
	@echo "extern const unsigned int ROBOTO_REGULAR_FONT_SIZE;" >> embedded_fonts.h
	@echo "#include \"embedded_fonts.h\"" > embedded_fonts.cpp
	@echo "const unsigned char PLAY_CHICKENS_FONT[] = {" >> embedded_fonts.cpp
	@xxd -i font/Play-Chickens.otf | grep -v "unsigned" | head -n -2 >> embedded_fonts.cpp
	@echo "};" >> embedded_fonts.cpp
	@echo "const unsigned int PLAY_CHICKENS_FONT_SIZE = `wc -c < font/Play-Chickens.otf`;" >> embedded_fonts.cpp
	@echo "const unsigned char ROBOTO_REGULAR_FONT[] = {" >> embedded_fonts.cpp
	@xxd -i font/Roboto-Regular.ttf | grep -v "unsigned" | head -n -2 >> embedded_fonts.cpp
	@echo "};" >> embedded_fonts.cpp
	@echo "const unsigned int ROBOTO_REGULAR_FONT_SIZE = `wc -c < font/Roboto-Regular.ttf`;" >> embedded_fonts.cpp

# ...existing code...