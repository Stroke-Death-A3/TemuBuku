#pragma once
#include <windows.h>
#include <string>

class DLLLoader {
public:
    static bool ExtractAndLoadDLL() {
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        std::string exeDir = std::string(exePath);
        exeDir = exeDir.substr(0, exeDir.find_last_of("\\/"));
        std::string tempPath = exeDir + "\\glfw3.dll";
        
        // Delete existing DLL if present
        DeleteFileA(tempPath.c_str());
        
        // Fix resource type casting
        HRSRC hRes = FindResourceA(NULL, MAKEINTRESOURCEA(1), MAKEINTRESOURCEA(RT_RCDATA));
        if (hRes == NULL) {
            // Get last error for debugging
            DWORD error = GetLastError();
            char msg[256];
            sprintf_s(msg, "Failed to find resource. Error: %lu", error);
            MessageBoxA(NULL, msg, "Debug Info", MB_OK);
            return false;
        }

        HGLOBAL hData = LoadResource(NULL, hRes);
        if (hData == NULL) return false;

        LPVOID pData = LockResource(hData);
        if (pData == NULL) return false;
        
        DWORD size = SizeofResource(NULL, hRes);
        if (size == 0) return false;

        // Write to temp file with binary mode explicitly set
        FILE* fp;
        if (fopen_s(&fp, tempPath.c_str(), "wb") != 0) return false;
        
        if (fp != NULL) {
            size_t written = fwrite(pData, 1, size, fp);
            fclose(fp);
            
            if (written != size) return false;
        } else {
            return false;
        }

        // Load the DLL and handle errors
        SetDllDirectoryA(exeDir.c_str());  // Add DLL search path
        HMODULE hModule = LoadLibraryA(tempPath.c_str());
        if (hModule == NULL) {
            DWORD error = GetLastError();
            char msg[256];
            sprintf_s(msg, "Failed to load DLL. Error: %lu", error);
            MessageBoxA(NULL, msg, "Debug Info", MB_OK);
            return false;
        }

        return true;
    }
};