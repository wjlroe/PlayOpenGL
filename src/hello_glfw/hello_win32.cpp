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
#include "hello.cpp"
#include "maths.cpp"

bool glLogErr(const char *Message, ...) {
    va_list ArgPtr;
    FILE *File = fopen(GL_LOG_FILE, "a");
    if (!File) {
        fprintf(stderr,
                "ERROR: could not open GL_LOG_FILE %s file for appending\n",
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

const char *glTypeToString(GLenum Type) {
    switch (Type) {
        case GL_BOOL:
            return "bool";
        case GL_INT:
            return "int";
        case GL_FLOAT:
            return "float";
        case GL_FLOAT_VEC2:
            return "vec2";
        case GL_FLOAT_VEC3:
            return "vec3";
        case GL_FLOAT_VEC4:
            return "vec4";
        case GL_FLOAT_MAT2:
            return "mat2";
        case GL_FLOAT_MAT3:
            return "mat3";
        case GL_FLOAT_MAT4:
            return "mat4";
        case GL_SAMPLER_2D:
            return "sampler2D";
        case GL_SAMPLER_3D:
            return "sampler3D";
        case GL_SAMPLER_CUBE:
            return "samplerCube";
        case GL_SAMPLER_2D_SHADOW:
            return "sampler2DShadow";
        default:
            break;
    }
    return "other";
}

void PrintProgrammeInfoLog(GLuint ProgrammeIndex) {
    int MaxLength = 2048;
    int ActualLength = 0;
    char log[2048];
    glGetProgramInfoLog(ProgrammeIndex, MaxLength, &ActualLength, log);
    printf("program info log for GL index: %u\n%s", ProgrammeIndex, log);
}

void PrintAll(GLuint Programme) {
    printf("--------------------\nshader programme %u info:\n", Programme);
    int Params = -1;
    glGetProgramiv(Programme, GL_LINK_STATUS, &Params);
    printf("GL_LINK_STATUS = %i\n", Params);
    glGetProgramiv(Programme, GL_ATTACHED_SHADERS, &Params);
    printf("GL_ATTACHED_SHADERS = %i\n", Params);

    glGetProgramiv(Programme, GL_ACTIVE_ATTRIBUTES, &Params);
    printf("GL_ACTIVE_ATTRIBUTES = %i\n", Params);
    for (GLuint i = 0; i < (GLuint)Params; i++) {
        char Name[64];
        int MaxLength = 64;
        int ActualLength = 0;
        int Size = 0;
        GLenum Type;
        glGetActiveAttrib(Programme, i, MaxLength, &ActualLength, &Size, &Type,
                          Name);
        if (Size > 1) {
            for (int j = 0; j < Size; j++) {
                char LongName[64];
                sprintf(LongName, "%s[%i]", Name, j);
                int Location = glGetAttribLocation(Programme, LongName);
                printf("  %i) Type:%s Name:%s Location:%i\n", i,
                       glTypeToString(Type), LongName, Location);
            }
        } else {
            int Location = glGetAttribLocation(Programme, Name);
            printf("  %i) Type: %s Name:%s Location:%i\n", i,
                   glTypeToString(Type), Name, Location);
        }
    }

    glGetProgramiv(Programme, GL_ACTIVE_UNIFORMS, &Params);
    printf("GL_ACTIVE_UNIFORMS = %i\n", Params);
    for (GLuint i = 0; i < (GLuint)Params; i++) {
        char Name[64];
        int MaxLength = 64;
        int ActualLength = 0;
        int Size = 0;
        GLenum Type;
        glGetActiveUniform(Programme, i, MaxLength, &ActualLength, &Size, &Type,
                           Name);
        if (Size > 1) {
            for (int j = 0; j < Size; j++) {
                char LongName[64];
                sprintf(LongName, "%s[%i]", Name, j);
                int Location = glGetUniformLocation(Programme, LongName);
                printf("  %i) Type:%s Name:%s Location:%i\n", i,
                       glTypeToString(Type), LongName, Location);
            }
        } else {
            int Location = glGetUniformLocation(Programme, Name);
            printf("  %i) Type:%s Name:%s Location:%i\n", i,
                   glTypeToString(Type), Name, Location);
        }
    }

    PrintProgrammeInfoLog(Programme);
}

bool ProgrammeIsValid(GLuint Programme) {
    glValidateProgram(Programme);
    int Params = -1;
    glGetProgramiv(Programme, GL_VALIDATE_STATUS, &Params);
    printf("program %i GL_VALIDATE_STATUS = %i\n", Programme, Params);
    if (GL_TRUE != Params) {
        PrintProgrammeInfoLog(Programme);
        return false;
    }
    return true;
}

void PrintShaderInfoLog(GLuint ShaderIndex) {
    int MaxLength = 2048;
    int ActualLength = 0;
    char log[2048];
    glGetShaderInfoLog(ShaderIndex, MaxLength, &ActualLength, log);
    printf("Shader info log for GL index %u:\n%s\n", ShaderIndex, log);
}

bool CheckForShaderErrors(GLuint ShaderIndex) {
    int Params = -1;
    glGetShaderiv(ShaderIndex, GL_COMPILE_STATUS, &Params);
    if (GL_TRUE != Params) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n",
                ShaderIndex);
        PrintShaderInfoLog(ShaderIndex);
        return false;
    }
    return true;
}

bool CheckForLinkingErrors(GLuint ProgrammeIndex) {
    int Params = -1;
    glGetProgramiv(ProgrammeIndex, GL_LINK_STATUS, &Params);
    if (GL_TRUE != Params) {
        fprintf(stderr, "ERROR: could not link shader programme GL index: %u\n",
                ProgrammeIndex);
        PrintProgrammeInfoLog(ProgrammeIndex);
        return false;
    }
    return true;
}

#if 0
void PointsTimesMatrix(float array[], int size, mat4 matrix) {
    int num_points = size / 3;
    vec3 *points = new vec3[num_points];
    int point_i = 0;
    for (int i = 0; i < size; i += 3) {
        points[point_i++] = vec3(array[i], array[i + 1], array[i + 2]);
    }
    for (int i = 0; i < num_points; i++) {
        vec3 point = points[i];
        vec3 new_point = matrix * point;
        new_point.print();
    }
}
#endif

void PrintMatrix(float matrix[], int size, int columns) {
    bool first_line = true;
    printf("\n{");
    for (int i = 0; i < size; i++) {
        if (!first_line && i % columns == 0) {
            printf(" ");
        }
        printf(" %5.2f", matrix[i]);
        if (i < size - 1) {
            printf(",");
        }
        if (i < size - 1 && (i + 1) % columns == 0) {
            printf("\n");
            first_line = false;
        }
    }
    printf("  }\n");
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

    assert(RestartGLLog());
    glLog("starting GLFW\n%s\n", glfwGetVersionString());
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // GLFWmonitor *Monitor = glfwGetPrimaryMonitor();
    // const GLFWvidmode *VMode = glfwGetVideoMode(Monitor);
    // GLFWwindow *Window = glfwCreateWindow(VMode->width, VMode->height,
    //                                       "Extended GL Init", Monitor, NULL);

    GLFWwindow *Window =
        glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!Window) {
        fprintf(stderr, "ERROR: count not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }

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
    glLog("renderer: %s\nversion: %s\n", renderer, version);
    LogGLParams();

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST);  // enable depth testing
    glDepthFunc(
        GL_LESS);  // depth-testing interprets a smaller value as "closer"

    // clang-format off
    GLfloat points[] = {00.0f, 01.0f, 00.0f,
                        00.5f, 00.0f, 00.0f,
                        -0.5f, 00.0f, 00.0f,
                        -0.5f, 00.0f, 00.0f,
                        00.5f, 00.0f, 00.0f,
                        00.0f, -1.0f, 00.0f};
    GLfloat colours[] = {1.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 1.0f,
                         0.8f, 0.2f, 0.2f,
                         0.2f, 0.8f, 0.2f,
                         0.2f, 0.2f, 0.8f};
    // clang-format on

    float TriangleSpeed = 0.0f;
    float LastPosition = 0.0f;
    float CamSpeed = 1.0f;
    float CamYawSpeed = 10.0f;  // 10 degrees per second
    float CamPos[] = {5.0f, 11.0f, 10.0f};
    float CamYaw = 0.0f;

    mat4 T =
        translate(identity_mat4(), vec3(-CamPos[0], -CamPos[1], -CamPos[2]));
    // T.print();
    mat4 R = rotate_y_deg(identity_mat4(), -CamYaw);
    // R.print();
    mat4 ViewMat4 = R * T;
    // ViewMat4.print();
    // clang-format off
    float ViewMatrix[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f
    };
    // clang-format on
    ViewMat4.into_array(ViewMatrix);
    printf("\nViewMatrix:");
    PrintMatrix(ViewMatrix, 16, 4);

    // PointsTimesMatrix(points, 18, ViewMatrix);

    float Near = 0.1f;
    float Far = 100.0f;
    float Fov = 67.0f * ONE_DEG_IN_RAD;
    float Aspect = (float)g_gl_width / (float)g_gl_height;
    float Range = tan(Fov * 0.5f) * Near;
    float Sx = (2.0f * Near) / (Range * Aspect + Range * Aspect);
    float Sy = Near / Range;
    float Sz = -(Far + Near) / (Far - Near);
    float Pz = -(2.0f * Far * Near) / (Far - Near);

    // clang-format off
    float ProjMatrix[] = {
        Sx, 0.0f, 0.0f, 0.0f,
        0.0f, Sy, 0.0f, 0.0f,
        0.0f, 0.0f, Sz, -1.0f,
        0.0f, 0.0f, Pz, 0.0f
    };
    // clang-format on
    printf("\nProjMatrix:");
    PrintMatrix(ProjMatrix, 16, 4);

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colours_vbo = 0;
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &VertexShader, NULL);
    glCompileShader(vs);
    if (!CheckForShaderErrors(vs)) {
        fprintf(stderr, "ERROR: vertex shader didn't compile\n");
        return false;
    }
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FragmentShader, NULL);
    glCompileShader(fs);
    if (!CheckForShaderErrors(fs)) {
        fprintf(stderr, "ERROR: fragment shader didn't compile\n");
        return false;
    }

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
    if (!CheckForLinkingErrors(shader_programme)) {
        return false;
    }
    PrintAll(shader_programme);
    // FIXME : only do this in development
    if (!ProgrammeIsValid(shader_programme)) {
        return false;
    }
    int ViewMatrixLocation = glGetUniformLocation(shader_programme, "view");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, ViewMatrix);
    int ProjMatrixLocation = glGetUniformLocation(shader_programme, "proj");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(ProjMatrixLocation, 1, GL_FALSE, ProjMatrix);

    while (!glfwWindowShouldClose(Window)) {
        static double PreviousSeconds = glfwGetTime();
        double CurrentSeconds = glfwGetTime();
        double ElapsedSeconds = CurrentSeconds - PreviousSeconds;
        PreviousSeconds = CurrentSeconds;
        if (fabs(LastPosition) > 1.0f) {
            TriangleSpeed = -TriangleSpeed;
        }
        // matrix[12] = ElapsedSeconds * TriangleSpeed + LastPosition;
        // LastPosition = matrix[12];
        glUseProgram(shader_programme);
        glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, ViewMatrix);
        glUniformMatrix4fv(ProjMatrixLocation, 1, GL_FALSE, ProjMatrix);

        UpdateFPSCounter(Window);
        // glEnable(GL_FRAMEBUFFER_SRGB);
        // glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        GLfloat v[] = {0.1f, 0.2f, 0.3f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, v);
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, g_gl_width, g_gl_height);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
        // draw points 0-3 from the currently bound VAO with current in-use
        // shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
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

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine, int nCmdShow) {
    return run();
}

// Local Variables:
// compilation-read-command: nil
// End:
