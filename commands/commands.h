#ifdef __unix__

#define _OS_ 0
#define _CLEAR "clear"
#define _FILE_SEPARATOR "/"
#define _GET_FILES "ls "
#define _DELETE_FILE "rm -f "

#elif defined(_WIN32) || defined(WIN32)

#define _OS_ 1
#define _CLEAR "cls"
#define _FILE_SEPARATOR "\\"
#define _GET_FILES "dir "
#define _DELETE_FILE "del \f "

#endif
void ClearScreen();
String* Terminal(String * command);
String *GenerateOutputPath(String *base,int index);
String *GenerateInputPath(String *base,int index);
int FileExists(String *filename);
int CreateDir(String * path);
int RemoveDir(String *path);
int DirectoryExists(String *path);
int RemoveFile(String *path);
String * ReadFile(String *path);
int WriteFile(String *path,String *data);
int WriteInput(String *basePath,int index,String *data);
int WriteOutput(String *basePath,int index,String *data);
int RunProgram(String *path,String *inputPath,String *outputPath);
int CompileProgram(String *path,String *to);