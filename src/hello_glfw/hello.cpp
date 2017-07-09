#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <strsafe.h>
#include <windows.h>

#if 0
void ErrorExit(const char *lpszFunction) {
  // Retrieve the system error message for the last-error code

  LPVOID lpMsgBuf;
  LPVOID lpDisplayBuf;
  DWORD dw = GetLastError();
  printf("LastError: %d\n", dw);

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);
  printf("\nError: %s\n", (LPTSTR)lpMsgBuf);

  // Display the error message and exit the process

  lpDisplayBuf = (LPVOID)LocalAlloc(
      LMEM_ZEROINIT,
      (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) *
          sizeof(TCHAR));
  StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                  TEXT("%s failed with error %d: %s"), lpszFunction, dw,
                  lpMsgBuf);
  // MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

  LocalFree(lpMsgBuf);
  LocalFree(lpDisplayBuf);
  ExitProcess(dw);
}
#endif

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

      if (FALSE ==
          ReadFile(vertFile, FileContents, RealFileSize, &BytesRead, NULL)) {
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

void PrintCWD() {
  char dirBuf[256] = {0};
  GetCurrentDirectory(256 - 1, (LPTSTR)&dirBuf);
  printf("Current dir: %s\n", (LPTSTR)dirBuf);
}

int run() {
  bool LoadWorked;

  char *VertexShader = LoadFile("test.vert", &LoadWorked);
  if (!LoadWorked && VertexShader) {
    printf("Failed to open vertex shader!\n");
    return 1;
  }

  char *FragmentShader = LoadFile("test.frag", &LoadWorked);
  if (!LoadWorked && FragmentShader) {
    printf("Failed to open fragment shader!\n");
    return 1;
  }

  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  GLFWwindow *window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if (!window) {
    fprintf(stderr, "ERROR: count not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  // start FLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  printf("Renderer: %s\n", renderer);
  printf("OpenGL version supported: %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable(GL_DEPTH_TEST); // enable depth testing
  glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

  // clang-format off
  GLfloat points[] = {00.0f, 01.0f, 00.0f,
                      00.5f, 00.0f, 00.0f,
                      -0.5f, 00.0f, 00.0f,
                      -0.5f, 00.0f, 00.0f,
                      00.5f, 00.0f, 00.0f,
                      00.0f, -1.0f, 00.0f};
  // clang-format on

  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint vao2 = 0;
  glGenVertexArrays(1, &vao2);
  glBindVertexArray(vao2);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &VertexShader, NULL);
  glCompileShader(vs);
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &FragmentShader, NULL);
  glCompileShader(fs);

  GLuint shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);

  while (!glfwWindowShouldClose(window)) {
    // wipe the drawing surface clear
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader_programme);
    glBindVertexArray(vao);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 3, 6);
    // update other events like input handling
    glfwPollEvents();
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}

int main() {
// for macOS?
#if 0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
  return run();
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow) {
  return run();
}

// Local Variables:
// compile-command: "build.bat"
// compilation-read-command: nil
// End:
