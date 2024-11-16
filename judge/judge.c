#include "judge.h"
String *LogJudge(String *log,int index,int errorType){
    String * indexStr=IntToString(index);
    log=AppendStringChars(log,"Test case ");
    log=AppendString(log,indexStr);
    switch (errorType){
        case 0:
            log=AppendStringChars(log,": Correct answer.\n");
            break;
        case 1:
            log=AppendStringChars(log,": Wrong answer!!\n");
            break;
        case 2:
            log=AppendStringChars(log,": <runtime error>\n");
            break;
        default:
            log=AppendStringChars(log,": {time Limit exceeded}\n");
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
    inputPath=AppendStringChars(inputPath,"/in");
    outputPath=AppendStringChars(outputPath,"/out");
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

    RemoveFile(tmoOutPutPath);
    WriteLine(log);

    FreeString(tmoOutPutPath);
    FreeString(log);

    printf("Score:%f\n",(correctTestCases/numberOfTestCases)*100);
    return (correctTestCases/numberOfTestCases)*100;
}