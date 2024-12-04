
#pragma once
#include <windows.h>
#include <string>

class DLLLoader {
public:
    static bool ExtractAndLoadDLL() {
        const char* tempPath = ".\\glfw3.dll";
        
        // Extract DLL from resources
        HRSRC hRes = FindResourceA(NULL, MAKEINTRESOURCEA(1), RT_RCDATA);
        if (hRes == NULL) return false;

        HGLOBAL hData = LoadResource(NULL, hRes);
        if (hData == NULL) return false;

        void* pData = LockResource(hData);
        DWORD size = SizeofResource(NULL, hRes);

        // Write to temp file
        FILE* fp;
        fopen_s(&fp, tempPath, "wb");
        if (fp == NULL) return false;
        
        fwrite(pData, 1, size, fp);
        fclose(fp);

        // Load the DLL
        return LoadLibraryA(tempPath) != NULL;
    }
};