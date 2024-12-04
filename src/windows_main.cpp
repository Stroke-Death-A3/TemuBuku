#ifdef _WIN32
#include <windows.h>
#include "dll_loader.h"

extern int main(int argc, char** argv);

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    if (!DLLLoader::ExtractAndLoadDLL()) {
        MessageBoxA(NULL, "Failed to load GLFW DLL", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }
    return main(__argc, __argv);
}
#endif