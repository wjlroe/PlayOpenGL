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
        fprintf(stderr,
                "Error: could not open GL_LOG_FILE %s file for appending\n",
                GL_LOG_FILE);
        return false;
    }
    va_start(ArgPtr, Message);
    vfprintf(File, Message, ArgPtr);
    va_end(ArgPtr);
    fclose(File);
    return true;
}
