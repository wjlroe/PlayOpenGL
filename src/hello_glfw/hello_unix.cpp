#include <assert.h>
#include <stdarg.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "maths.cpp"
#include "hello.cpp"

char *LoadFile(const char *Filename, bool *Success) {
    char *FileContents;
    int FileSize = 0;
    FILE *handle = fopen(Filename, "rb");

    fseek(handle, 0L, SEEK_END);
    FileSize = ftell(handle);
    fseek(handle, 0L, SEEK_SET);
    FileContents = (char *)malloc(FileSize + 1);
    size_t size = fread(FileContents, 1, FileSize, handle);
    FileContents[size] = 0;  // Add terminating zero.

    fclose(handle);
    *Success = true;

    return FileContents;
}

int main() { return run(); }
