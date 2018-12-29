#ifdef _WIN32
#include <strsafe.h>
#include <windows.h>
#include "hello_common.h"

char *LoadFile(const char *Filename, bool *Success) {
    char *FileContents;
    HANDLE vertFile = CreateFile(Filename, GENERIC_READ, 0, NULL, OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL, NULL);

    if (vertFile == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        printf("Invalid file handle: %lu\n", err);
        *Success = false;
        return NULL;
    }
    LARGE_INTEGER FileSize;
    if (GetFileSizeEx(vertFile, &FileSize)) {
        SIZE_T RealFileSize = (SIZE_T)FileSize.QuadPart;
        FileContents = (char *)VirtualAlloc(
            0, RealFileSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (FileContents) {
            DWORD BytesRead;

            if (FALSE == ReadFile(vertFile, FileContents, RealFileSize,
                                  &BytesRead, NULL)) {
                DWORD err = GetLastError();
                printf("Error reading file: %lu\n", err);
                CloseHandle(vertFile);
                *Success = false;
                return NULL;
            }
        }
    }

    CloseHandle(vertFile);
    *Success = true;
    return FileContents;
}

int main() { return run(); }

#endif
