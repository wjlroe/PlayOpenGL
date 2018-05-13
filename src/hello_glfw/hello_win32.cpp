#include <assert.h>
#include <stdarg.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <strsafe.h>
#include <windows.h>
#include "maths.cpp"
#include "hello.cpp"

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

// Local Variables:
// compilation-read-command: nil
// End:
