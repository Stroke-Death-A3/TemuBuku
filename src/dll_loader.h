#pragma once
#include <windows.h>
#include <string>

class DLLLoader {
public:
    static bool ExtractAndLoadDLL() {
        const char* tempPath = ".\\glfw3.dll";
        
        // Extract DLL from resources - use correct type for RT_RCDATA
        HRSRC hRes = FindResourceA(NULL, MAKEINTRESOURCEA(1), RT_RCDATA);
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
        if (fopen_s(&fp, tempPath, "wb") != 0) return false;
        
        if (fp != NULL) {
            size_t written = fwrite(pData, 1, size, fp);
            fclose(fp);
            
            if (written != size) return false;
        } else {
            return false;
        }

        // Load the DLL and handle errors
        HMODULE hModule = LoadLibraryA(tempPath);
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