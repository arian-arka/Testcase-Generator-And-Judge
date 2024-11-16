#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <dirent.h>
#include "watch.h"
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
        result=AppendString_(result,tmp);
    result=SubStr(result,0,result->len-2,1);
    return result;

}
String *GenerateOutputPath(String *base,int index){
    String *path=InitString(base->data),*tmpIndex=IntToString(index);
    path=AppendString_(path,_FILE_SEPARATOR);
    path=AppendString_(path,"out");
    path=AppendString_(path,_FILE_SEPARATOR);
    path=AppendString_(path,"output");
    path=AppendString(path,tmpIndex);
    path=AppendString_(path,".txt");
    FreeString(tmpIndex);
    return path;
}
String *GenerateInputPath(String *base,int index){
    String *path=InitString(base->data),*tmpIndex=IntToString(index);
    path=AppendString_(path,_FILE_SEPARATOR);
    path=AppendString_(path,"in");
    path=AppendString_(path,_FILE_SEPARATOR);
    path=AppendString_(path,"input");
    path=AppendString(path,tmpIndex);
    path=AppendString_(path,".txt");
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
    str=AppendString_(str,"\" || echo false");
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
    str=AppendString_(str,"\" || echo false");
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
    str=AppendString_(str," \"");
    str=AppendString(str,path);
    str=AppendString_(str,"\" || echo false");
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
    str=AppendString_(str,"\"<\"");
    str=AppendString(str,inputPath);
    str=AppendString_(str,"\" >\"");
    str=AppendString(str,outputPath);
    str=AppendString_(str, "\"|| echo false");
    WriteLine(str);
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
    str=AppendString_(str, "\" -o \"");
    str=AppendString(str, to);
    str=AppendString_(str, "\"|| echo false");
    String* boolean=Terminal(str);
    int status=EmptyString(boolean);
    FreeString(boolean);
    FreeString(str);
    return status;
}

int GenerateTestCaseFromStr(String *programPath,String *str,String *basePath,int index){//considered program path is valid and it works=> means its been checked before calling this funciotn
    if(EmptyString(programPath)|| EmptyString(basePath))
        return 0;
    int status=0;
    String *inputPath=GenerateInputPath(basePath,index),*outputPath=GenerateOutputPath(basePath,index);
    status=WriteInput(basePath,index,str);
    if(!status)
        return 0;
    status=RunProgram(programPath,inputPath,outputPath);
    if(!status)
        return 0;
    FreeString(outputPath);
    FreeString(inputPath);
    return 1;
}

int GenerateTestCasesFromFile(String *path){//-1 invalid syntax- 0 invalid file- 1 success
    if(!FileExists(path))
        return 0;
    String *fileData=ReadFile(path);
    if(EmptyString(fileData))
        return 0;
    int status=0;
    String *basePath=InitString(""),*programPath=InitString("");
    String *data=InitString("");
    int numberOfTestCases=0;
    for (int i = 0; i < fileData->len; ++i) {
        if(status==0){
            if(fileData->data[i]=='\"')
                status=1;
            else if(!IsWhiteSpace(fileData->data[i]))
                return -1;
        }
        else if(status==1){
            if(fileData->data[i]=='\"')
                status=3;
            else if(fileData->data[i]=='\\')
                status=2;
            else
                basePath=AppendString__(basePath,fileData->data[i]);
        }
        else if(status==2){
            if(fileData->data[i]=='\"' || fileData->data[i]=='\\')
                basePath=AppendString__(basePath,fileData->data[i]);
            else
                return -1;
            status--;
        }
        else if(status==3){
            if(fileData->data[i]=='\"')
                status=4;
            else if(!IsWhiteSpace(fileData->data[i]))
                return -1;
        }
        else if(status==4){
            if(fileData->data[i]=='\"')
                status=6;
            else if(fileData->data[i]=='\\')
                status=5;
            else
                programPath=AppendString__(programPath,fileData->data[i]);
        }
        else if(status==5){
            if(fileData->data[i]=='\"' || fileData->data[i]=='\\')
                programPath=AppendString__(programPath,fileData->data[i]);
            else
                return -1;
            status--;
        }
        else if(status==6){
            if(fileData->data[i]=='\"')
                status=7;
            else if(!IsWhiteSpace(fileData->data[i]))
                return -1;
        }
        else if(status==7){
            if(fileData->data[i]=='\"')
            {
                numberOfTestCases++;
                status=6;//####
                if(!GenerateTestCaseFromStr(programPath,data,basePath,numberOfTestCases))
                    return -1;
                data=FreeString(data);
                data=NewString(0);
            }
            else if(fileData->data[i]=='\\')
                status=8;
            else{
                data=AppendString__(data,fileData->data[i]);
            }
        }
        else if(status==8){
            if(fileData->data[i]=='\"' || fileData->data[i]=='\\')
                data=AppendString__(data,fileData->data[i]);
            else
                return -1;
            status--;
        }
    }

    FreeString(data);
    FreeString(basePath);
    FreeString(programPath);
    FreeString(fileData);
    if(status<6)
        return -1;
    //free(programPath,data,basePath);
    return numberOfTestCases;

}

String *LogJudge(String *log,int index,int errorType){
    String * indexStr=IntToString(index);
    log=AppendString_(log,"Test case ");
    log=AppendString(log,indexStr);
    switch (errorType){//0:correct 1:wrong 2:runtime 3:timeLimit
        case 0:
            log=AppendString_(log,": Correct answer.\n");
            break;
        case 1:
            log=AppendString_(log,": Wrong answer!!\n");
            break;
        case 2:
            log=AppendString_(log,": <runtime error>\n");
            break;
        default:
            log=AppendString_(log,": {time Limit exceeded}\n");
            break;
    }
    FreeString(indexStr);
     
    return log;

}
int ValidTestCases(String *base){
    if(!DirectoryExists(base))
        return 0;
    int numberOfInputs=0,numberOfOutputs=0;

    String *inputPath=InitString(base->data),*outputPath=InitString(base->data);
    inputPath=AppendString_(inputPath,"/in");
    outputPath=AppendString_(outputPath,"/out");
    if(!DirectoryExists(inputPath) || !DirectoryExists(outputPath))
    {
        FreeString(inputPath);
        FreeString(outputPath);
        return 0;
    }

    DIR *inputs,*outputs;

    inputs = opendir(inputPath->data);
    outputs = opendir(outputPath->data);
    FreeString(inputPath);
    FreeString(outputPath);

    struct dirent *dir;
    if (inputs) {
        while ((dir = readdir(inputs)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
                numberOfInputs++;
        }
        closedir(inputs);
    }
    dir=NULL;
    if (outputs) {
        while ((dir = readdir(outputs)) != NULL) {
            if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
                numberOfOutputs++;
        }
        closedir(outputs);
    }
    if(!numberOfOutputs || numberOfInputs!=numberOfOutputs)
        return 0;
    String *tmpInput,*tmpOutput;
    int flag=0;
    for (int i = 0; i < numberOfInputs; ++i) {

        tmpInput=GenerateOutputPath(base,i+1);
        tmpOutput=GenerateInputPath(base,i+1);

        flag=!FileExists(tmpInput) || !FileExists(tmpOutput);
        FreeString(tmpInput);
        FreeString(tmpOutput);

        if(flag)
            return 0;
    }
    return numberOfOutputs;
}

int CompareTwoFileData(String *path1,String *path2){
    String *data1=ReadFile(path1);
    String *data2=ReadFile(path2);
    int status=IsEqual(data1,data2);
    FreeString(data1);
    FreeString(data2);
    return status;
}

double Judge(String *programPath,String *testCasesBase,float miliSeconds){
    if(!FileExists(programPath) || !DirectoryExists(testCasesBase))
        return -1;

    String *tmoOutPutPath=InitString("___tmp___.txt");

    double numberOfTestCases=ValidTestCases(testCasesBase),correctTestCases=0;
    if(numberOfTestCases==0)
        return -1;

    float time;

    int runtime,answer;

    String *log=InitString("");

    clock_t start_t, end_t, total_t;

    String *inputPath=NULL,*outPut=NULL;

    for (int i = 0; i < numberOfTestCases; ++i) {
     
        inputPath=GenerateInputPath(testCasesBase,i+1);
        StartStopWatch();
        runtime=RunProgram(programPath,inputPath,tmoOutPutPath);
        time=TimeDifference();
        
        if(!runtime){
            LogJudge(log,i+1,2);
        }
        else if(miliSeconds<time)
        {
            LogJudge(log,i+1,3);
        }
        else{
            outPut=GenerateOutputPath(testCasesBase,i+1);
            answer=CompareTwoFileData(outPut,tmoOutPutPath);
            if(answer){
                correctTestCases++;
                LogJudge(log,i+1,0);
            }
            else{
                LogJudge(log,i+1,1);
            }
        }
        outPut=FreeString(outPut);
        inputPath=FreeString(inputPath);
    }
    //ClearScreen();
    RemoveFile(tmoOutPutPath);
    WriteLine(log);
    FreeString(tmoOutPutPath);
    FreeString(log);
    printf("Score:%f",(correctTestCases/numberOfTestCases)*100);
    return (correctTestCases/numberOfTestCases)*100;
}