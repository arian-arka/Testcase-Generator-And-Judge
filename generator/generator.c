#include "generator.h"
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
int GenerateTestCasesFromFile(String *path){
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
        if(fileData->data[i]<0)
            continue;
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
                basePath=AppendStringChar(basePath,fileData->data[i]);
        }
        else if(status==2){
            if(fileData->data[i]=='\"' || fileData->data[i]=='\\')
                basePath=AppendStringChar(basePath,fileData->data[i]);
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
                programPath=AppendStringChar(programPath,fileData->data[i]);
        }
        else if(status==5){
            if(fileData->data[i]=='\"' || fileData->data[i]=='\\')
                programPath=AppendStringChar(programPath,fileData->data[i]);
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
                data=AppendStringChar(data,fileData->data[i]);
            }
        }
        else if(status==8){
            if(fileData->data[i]=='\"' || fileData->data[i]=='\\')
                data=AppendStringChar(data,fileData->data[i]);
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

    return numberOfTestCases;

}