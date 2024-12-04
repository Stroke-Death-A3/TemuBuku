# TemuBuku
[![Cross-Platform Build](https://github.com/Stroke-Death-A3/TemuBuku/actions/workflows/Compile.yml/badge.svg)](https://github.com/Stroke-Death-A3/TemuBuku/actions/workflows/Compile.yml) 
[![Cross-Platform Build](https://github.com/Stroke-Death-A3/TemuBuku/actions/workflows/Compile.yml/badge.svg?event=release)](https://github.com/Stroke-Death-A3/TemuBuku/actions/workflows/Compile.yml)\\
TemuBuku is a book search application implemented using Red-Black Tree data structure in C++. The application provides efficient book searching capabilities through a balanced tree implementation. 
## Features

- Book search functionality using Red-Black Tree algorithm
- Graphical user interface using GLFW
- Cross-platform support (Windows, Linux, Mac OS)

## Prerequisites

>Before installation, ensure you have the following prerequisites installed:

### For All Platforms
- C++ Compiler (GCC/G++) | Clang
- Make build system

### Additional Requirements for Windows
- MSYS2 environment
- MinGW-w64 toolchain
- GLFW3 library ([Download here](https://www.glfw.org/download))

## Installation

### Linux
1. Clone the repository
2. Navigate to the source directory :
   
   ```bash
   cd src
4. Build the project.
   
   ```bash
   make
### Windows

1. Clone the repository
2. Install MSYS2 if not already installed
3. Download GLFW3 library from the official website
4. Open MSYS2 terminal
5. Navigate to the source directory :

   ```bash
   cd src
   
7. Build the project.
   
   ```bash
   make
   
7. Copy the some **GLFW3.dll** file to the /bin directory where the executable is located
