#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <dirent.h>
#include "commands.h"
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

void ClearScreen(){
    system(_CLEAR);
}
String* Terminal(String * command){
    if(EmptyString(command))
        return InitString("");
    FILE *terminal=popen(command->data, "r");
    String *result=NewString(0);
    char tmp[24]={'\0'};
    while (fgets(tmp, sizeof(tmp), terminal) !=NULL)
        result=AppendStringChars(result,tmp);
    result=SubStr(result,0,result->len-2,1);
    return result;

}
String *GenerateOutputPath(String *base,int index){
    String *path=InitString(base->data),*tmpIndex=IntToString(index);
    path=AppendStringChars(path,_FILE_SEPARATOR);
    path=AppendStringChars(path,"out");
    path=AppendStringChars(path,_FILE_SEPARATOR);
    path=AppendStringChars(path,"output");
    path=AppendString(path,tmpIndex);
    path=AppendStringChars(path,".txt");
    FreeString(tmpIndex);
    return path;
}
String *GenerateInputPath(String *base,int index){
    String *path=InitString(base->data),*tmpIndex=IntToString(index);
    path=AppendStringChars(path,_FILE_SEPARATOR);
    path=AppendStringChars(path,"in");
    path=AppendStringChars(path,_FILE_SEPARATOR);
    path=AppendStringChars(path,"input");
    path=AppendString(path,tmpIndex);
    path=AppendStringChars(path,".txt");
    FreeString(tmpIndex);
    return path;
}

int FileExists(String *filename){
    if(EmptyString(filename))
        return 0;
    FILE *file;
    if (file = fopen(filename->data, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}
int CreateDir(String * path){
    if(EmptyString(path))
        return 0;
    String *str=NewString(0);
    str=SetString_(str,"mkdir \"");
    str=AppendString(str,path);
    str=AppendStringChars(str,"\" || echo false");
    String* boolean=Terminal(str);
    int status=EmptyString(boolean);
    FreeString(str);
    FreeString(boolean);
    return status;
}
int RemoveDir(String *path)
{
    if(EmptyString(path))
        return 0;
    String *str=NewString(0);
    str=SetString_(str,_DELETE_FILE);
    str=AppendString(str,path);
    str=AppendStringChars(str,"\" || echo false");
    String* boolean=Terminal(str);
    int status=EmptyString(boolean);
    FreeString(boolean);
    FreeString(str);
    return status;

}
int DirectoryExists(String *path){
    if(EmptyString(path))
        return 0;
    if(CreateDir(path)) {
        RemoveDir(path);
        return 0;
    }
    return 1;
}
int RemoveFile(String *path){
    if(EmptyString(path))
        return 0;
    String *str=NewString(0);
    str=SetString_(str,_DELETE_FILE);
    str=AppendStringChars(str," \"");
    str=AppendString(str,path);
    str=AppendStringChars(str,"\" || echo false");
    String* boolean=Terminal(str);
    int status=EmptyString(boolean);
    FreeString(boolean);
    FreeString(str);
    return status;
}
String * ReadFile(String *path){
    if(EmptyString(path))
        return 0;
    int string_size, read_size;
    FILE *handler = fopen(path->data, "r");
    String * data;
    if (handler)
    {
        fseek(handler, 0, SEEK_END);
        string_size = ftell(handler);
        data=NewString(string_size);
        rewind(handler);

        read_size = fread(data->data, sizeof(char), string_size, handler);
        if (string_size != read_size)
        {
            FreeString(data);
            return NULL;
        }
        fclose(handler);
    }

    return data;

}
int WriteFile(String *path,String *data){

    FILE *fp;
    if(EmptyString(path))
        return 0;
    fp = fopen(path->data, "w+");
    if(fp==NULL)
        return 0;
    if(!EmptyString(data))
        fprintf(fp, "%s",data->data);
    fclose(fp);
    return 1;
}
int WriteInput(String *basePath,int index,String *data){

    String *path=GenerateInputPath(basePath,index);
    int status=WriteFile(path,data);
    FreeString(path);
    return status;
}
int WriteOutput(String *basePath,int index,String *data) {
    String *path=GenerateOutputPath(basePath,index);
    int status=WriteFile(path,data);
    FreeString(path);
    return status;
}
int RunProgram(String *path,String *inputPath,String *outputPath){

    if(EmptyString(path)|| EmptyString(inputPath) || EmptyString(outputPath))
        return 0;
    String *str;
    if(_OS_==0)//linux
        str=InitString("\"./");
    else
        str=InitString("\"");

    str=AppendString(str,path);
    str=AppendStringChars(str,"\"<\"");
    str=AppendString(str,inputPath);
    str=AppendStringChars(str,"\" >\"");
    str=AppendString(str,outputPath);
    str=AppendStringChars(str, "\"|| echo false");
    String* boolean=Terminal(str);
    int status=EmptyString(boolean);
    FreeString(boolean);
    FreeString(str);
    return status;
}
int CompileProgram(String *path,String *to){//-1 file not exists;0 compile error;1 success
    if(!FileExists(path) )
        return -1;
    String *str=InitString("gcc \"");
    str=AppendString(str, path);
    str=AppendStringChars(str, "\" -o \"");
    str=AppendString(str, to);
    str=AppendStringChars(str, "\"|| echo false");
    String* boolean=Terminal(str);
    int status=EmptyString(boolean);
    FreeString(boolean);
    FreeString(str);
    return status;
}
