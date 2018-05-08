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
#include "hello.cpp"
#include "maths.cpp"

int main() {
  return run();
}
