#include <assert.h>
#include <stdarg.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <strsafe.h>
#include <windows.h>

static int g_gl_width = 640;
static int g_gl_height = 480;

bool RestartGLLog() {
  FILE *File = fopen(GL_LOG_FILE, "w");
  if (!File) {
    fprintf(stderr,
            "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
            GL_LOG_FILE);
    return false;
  }
  time_t Now = time(NULL);
  char *Date = ctime(&Now);
  fprintf(File, "GL_LOG_FILE log. local time %s\n", Date);
  fclose(File);
  return true;
}

bool glLog(const char *Message, ...) {
  va_list ArgPtr;
  FILE *File = fopen(GL_LOG_FILE, "a");
  if (!File) {
    fprintf(stderr, "Error: could not open GL_LOG_FILE %s file for appending\n",
            GL_LOG_FILE);
    return false;
  }
  va_start(ArgPtr, Message);
  vfprintf(File, Message, ArgPtr);
  va_end(ArgPtr);
  fclose(File);
  return true;
}

bool glLogErr(const char *Message, ...) {
  va_list ArgPtr;
  FILE *File = fopen(GL_LOG_FILE, "a");
  if (!File) {
    fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n",
            GL_LOG_FILE);
    return false;
  }
  va_start(ArgPtr, Message);
  vfprintf(File, Message, ArgPtr);
  va_end(ArgPtr);
  va_start(ArgPtr, Message);
  vfprintf(stderr, Message, ArgPtr);
  va_end(ArgPtr);
  fclose(File);
  return true;
}

void glfwErrorCallback(int Error, const char *Description) {
  glLogErr("GLFW ERROR: code %i msg: %s\n", Error, Description);
}

void LogGLParams() {
  // FIXME: All these are zero on the Surface?!?
  GLenum Params[] = {
      GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
      GL_MAX_CUBE_MAP_TEXTURE_SIZE,
      GL_MAX_DRAW_BUFFERS,
      GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
      GL_MAX_TEXTURE_IMAGE_UNITS,
      GL_MAX_TEXTURE_SIZE,
      GL_MAX_VARYING_FLOATS,
      GL_MAX_VERTEX_ATTRIBS,
      GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
      GL_MAX_VERTEX_UNIFORM_COMPONENTS,
      GL_MAX_VIEWPORT_DIMS,
      GL_STEREO,
  };
  const char *Names[] = {
      "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
      "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
      "GL_MAX_DRAW_BUFFERS",
      "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
      "GL_MAX_TEXTURE_IMAGE_UNITS",
      "GL_MAX_TEXTURE_SIZE",
      "GL_MAX_VARYING_FLOATS",
      "GL_MAX_VERTEX_ATTRIBS",
      "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
      "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
      "GL_MAX_VIEWPORT_DIMS",
      "GL_STEREO",
  };
  glLog("GL Context Params:\n");
  for (int i = 0; i < 10; i++) {
    int v = 0;
    glGetIntegerv(Params[i], &v);
    glLog("%s %i\n", Names[i], v);
  }

  int v[2];
  v[0] = v[1] = 0;
  glGetIntegerv(Params[10], v);
  glLog("%s %i %i\n", Names[10], v[0], v[1]);
  unsigned char s = 0;
  glGetBooleanv(Params[11], &s);
  glLog("%s %u\n", Names[11], (unsigned int)s);
  glLog("------------------------\n");
}

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

void glfwWindowSizeCallback(GLFWwindow *Window, int Width, int Height) {
  g_gl_width = Width;
  g_gl_height = Height;
}

void UpdateFPSCounter(GLFWwindow *Window) {
  static double PreviousSeconds = 0;
  static int FrameCount = 0;
  double CurrentSeconds = glfwGetTime();
  double ElapsedSeconds = CurrentSeconds - PreviousSeconds;
  if (ElapsedSeconds > 0.25) {
    PreviousSeconds = CurrentSeconds;
    char tmp[128];
    double fps = (double)FrameCount / ElapsedSeconds;
    sprintf(tmp, "opengl @ fps: %.2f", fps);
    glfwSetWindowTitle(Window, tmp);
    FrameCount = 0;
  }
  FrameCount++;
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

  char *FragmentShader2 = LoadFile("test2.frag", &LoadWorked);
  if (!LoadWorked && FragmentShader2) {
    printf("Failed to open fragment shader 2!\n");
    return 1;
  }

  assert(RestartGLLog());
  glLog("starting GLFW\n%s\n", glfwGetVersionString());
  glfwSetErrorCallback(glfwErrorCallback);

  if (!glfwInit()) {
    fprintf(stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);

  // GLFWmonitor *Monitor = glfwGetPrimaryMonitor();
  // const GLFWvidmode *VMode = glfwGetVideoMode(Monitor);
  // GLFWwindow *Window = glfwCreateWindow(VMode->width, VMode->height,
  //                                       "Extended GL Init", Monitor, NULL);

  GLFWwindow *Window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
  if (!Window) {
    fprintf(stderr, "ERROR: count not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }

  LogGLParams();
  glfwSetWindowSizeCallback(Window, glfwWindowSizeCallback);
  glfwMakeContextCurrent(Window);

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

  GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs2, 1, &FragmentShader2, NULL);
  glCompileShader(fs2);

  GLuint shader_programme = glCreateProgram();
  glAttachShader(shader_programme, fs);
  glAttachShader(shader_programme, vs);
  glLinkProgram(shader_programme);

  GLuint ShaderProgramme2 = glCreateProgram();
  glAttachShader(ShaderProgramme2, fs2);
  glAttachShader(ShaderProgramme2, vs);
  glLinkProgram(ShaderProgramme2);

  while (!glfwWindowShouldClose(Window)) {
    UpdateFPSCounter(Window);
    glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, g_gl_width, g_gl_height);
    glUseProgram(shader_programme);
    glBindVertexArray(vao);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(ShaderProgramme2);
    glBindVertexArray(vao2);
    glDrawArrays(GL_TRIANGLES, 3, 6);
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers(Window);

    // update other events like input handling
    glfwPollEvents();

    if (GLFW_PRESS == glfwGetKey(Window, GLFW_KEY_Q)) {
      glfwSetWindowShouldClose(Window, 1);
    }
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
