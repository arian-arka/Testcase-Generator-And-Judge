#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include "newStrings/newStrings.c"
#include "commands/commands.c"
#include "watch/watch.c"
#include "generator/generator.c"
#include "judge/judge.c"


void GenerateFromUser(){
    String *file=NULL,*basePath=NULL,*excutablePath=NULL,*numberOfTestCases=NULL,*testCase=NULL;
    int status;
    while (1) {
        ClearScreen();
        printf("Where is your C file located(type @back if u want to go back): ");
        file = ReadLine();
        if (IsEqualChars(file, "@back"))
        {
            FreeString(file);
            return;
        }
        status=!FileExists(file);
        if (status){
            file=FreeString(file);
            PressEnter("File not found!");
            continue;
        }
        printf("Where do you want  the compiled file to be located: ");
        excutablePath=ReadLine();
        if(EmptyString(excutablePath) || !CompileProgram(file,excutablePath))
        {
            file=FreeString(file);
            excutablePath=FreeString(excutablePath);
            PressEnter("Compile error!!");
            continue;
        }
        file=FreeString(file);

        printf("Where do you want  the testcases to be located: ");
        basePath = ReadLine();
        status=!DirectoryExists(basePath);
        if (status){
            basePath=FreeString(basePath);
            excutablePath=FreeString(excutablePath);
            PressEnter("Directory does not exist!!");
            continue;
        }
        printf("Number of test cases: ");
        String *numberOfTestCases=ReadLine();
        status=!IsStringNumber(numberOfTestCases);
        if (status){
            basePath= FreeString(basePath);
            excutablePath=FreeString(excutablePath);
            numberOfTestCases=FreeString(numberOfTestCases);
            PressEnter("Invalid number!!");
            continue;
        }
        int count=StringToInt(numberOfTestCases);
        if (count<0){
            basePath=FreeString(basePath);
            excutablePath=FreeString(excutablePath);
            numberOfTestCases=FreeString(numberOfTestCases);
            PressEnter("Invalid number!!(number>0)");
            continue;
        }
        numberOfTestCases=FreeString(numberOfTestCases);

        for (int i = 0; i < count; ++i) {
            printf("Test case %d: ",i+1);
            testCase=ReadLine();
            if(!GenerateTestCaseFromStr(excutablePath,testCase,basePath,i+1))
            {
                basePath=FreeString(basePath);
                excutablePath=FreeString(excutablePath);
                testCase=FreeString(testCase);
                testCase=FreeStringData(testCase);
                PressEnter("RunTimeError - please fix your code!!");
                continue;
            }
            testCase=FreeStringData(testCase);
        }
        basePath=FreeString(basePath);
        excutablePath=FreeString(excutablePath);
        testCase= FreeString(testCase);
    }



}

void GenerateFromFile(){
    String *path=NULL;
    int status;
    while (1) {
        ClearScreen();
        printf("Where is your file located(type @back if u want to go back): ");
        path=ReadLine();
        if (IsEqualChars(path, "@back"))
            return;
        status=GenerateTestCasesFromFile(path);
        if(status==0)
            PressEnter("Invalid file");
        else if(status==-1)
            PressEnter("Invalid syntax!!");
        else
            PressEnter("Test cases generated succesfully!!");

    }
}

void Judger(){
    String *programPath,*testCasesBase,*timeLimit;
    float timeOut;
    double status;
    while (1){
        ClearScreen();
        printf("Where is your compiled file located(type @back if u want to go back): ");
        programPath=ReadLine();
        if (IsEqualChars(programPath, "@back"))
        {
            programPath=FreeString(programPath);
            return;
        }
        status=!FileExists(programPath);
        if (status){
            programPath=FreeString(programPath);
            PressEnter("File not found!");
            continue;
        }
        printf("Where are yout test cases: ");
        testCasesBase=ReadLine();
        printf("What is your time limit: ");
        timeLimit=ReadLine();
        status=!IsStringNumber(timeLimit);
        if(status){

            programPath=FreeString(programPath);
            testCasesBase=FreeString(testCasesBase);
            timeLimit=FreeString(timeLimit);
            PressEnter("Invalid number");
            continue;
        }
        timeOut=StringToFloat(timeLimit);
        timeLimit=FreeString(timeLimit);
        if(Judge(programPath,testCasesBase,timeOut)==-1)
            PressEnter("Invalid path!!");

        else
            PressEnter("");
        programPath=FreeString(programPath);
        testCasesBase=FreeString(testCasesBase);
    }
}

void MakeBasePath(){
    ClearScreen();
    printf("Where : ");
    String *path=ReadLine();
    if(DirectoryExists(path))
        PressEnter("Directory already exists");
    else{
        CreateDir(path);
        path=AppendStringChars(path,_FILE_SEPARATOR);
        path=AppendStringChars(path,"in");
        CreateDir(path);
        path=SubStr(path,0,path->len-3,1);
        path=AppendStringChars(path,_FILE_SEPARATOR);
        path=AppendStringChars(path,"out");
        CreateDir(path);
        PressEnter("");
    }
    FreeString(path);
}

void CompileCFile(){
    ClearScreen();
    printf("Where is your c file: ");
    String *path=ReadLine();
    if(!FileExists(path))
    {
        FreeString(path);
        PressEnter("File does not exists");
        return;
    }
    printf("to : ");
    String *to=ReadLine();
    if(CompileProgram(path,to)==0)
        PressEnter("Compile error has occured!");
    else
        PressEnter("Compiled succesfully");
    FreeString(to);
    FreeString(path);
}

void PrintMainMenu(){
    printf("1.Generate test case from file\n");
    printf("2.Generate test case from user input\n");
    printf("3.Judge\n");
    printf("4.Make a Base Path\n");
    printf("5.Compile a C file\n");
    printf("6.help\n");
    printf("0.exit\n");
    printf("$>:");
}

void ShowHelp(){
    ClearScreen();
    printf("test cases structres are like this :\n");
    printf("Parent :\n");
    printf("     in:\n");
    printf("         input1.txt\n");
    printf("         input2.txt\n");
    printf("         input3.txt\n");
    printf("         ...\n");
    printf("     out\n");
    printf("         output1.txt\n");
    printf("         output2.txt\n");
    printf("         output3.txt\n");
    printf("         ...\n");
    printf("\nFor example:\n");
    printf("################\n");
    printf("D:\\testcases:\n");
    printf("     in:\n");
    printf("         input1.txt\n");
    printf("         input2.txt\n");
    printf("     out\n");
    printf("         output1.txt\n");
    printf("         output2.txt\n");
    printf("################\n");
    printf("\n-------------------------------\n");
    printf("Making a base path will create a structure like up");
    printf("\n-------------------------------\n");
    printf("For generating tests cases from a file you need a text file which contains this structure:\n");
    printf("\"Path of test cases\" \"Path of compiled c file\" \"test case 1\" \"test case 2\" ...\n");
    printf("For example:\n");
    printf("################\n");
    printf("file.txt>\n");
    printf("\"D:\\testcases\"\n");
    printf("\"D:\\compiled\"\n");
    printf("\"hello world\"\n");
    printf("\"this is the second test case\"\n");
    printf("\"hi guyz\"\n");
    printf("\"test case that contains escape chars(\\\"\\t\\n)\"\n");
    printf("################\n");
    printf("as you see if you want an escape char to be in a test case you should first type a back slash before it and the program will ignore the backSlashes for them dont worry\n");
    printf("for generating test cases from a file you first need to compile the C file from the menu or manual by yourself\n");
    printf("dont type (./) before your compiled file\n");
    printf("\n-------------------------------\n");
    PressEnter("");
}

void MainMenu(){
    String *command;
    while (1){
        ClearScreen();
        PrintMainMenu();
        command=ReadLine();
        command=LowerCase(command);
        if(IsEqualChars(command,"1") || IsEqualChars(command,"generate test case from file")){
            GenerateFromFile();
        }
        else if(IsEqualChars(command,"2") || IsEqualChars(command,"generate test case from user input")){
            GenerateFromUser();
        }
        else if(IsEqualChars(command,"3") || IsEqualChars(command,"judge")){
            Judger();
        }
        else if(IsEqualChars(command,"4") || IsEqualChars(command,"make a Base Path")){
            MakeBasePath();
        }
        else if(IsEqualChars(command,"5") || IsEqualChars(command,"compile a C file")){
            CompileCFile();
        }
        else if(IsEqualChars(command,"6") || IsEqualChars(command,"help")){
            ShowHelp();
        }
        else if(IsEqualChars(command,"0") || IsEqualChars(command,"exit")){
            break;
        }
        FreeString(command);
    }

}

int main(){
    MainMenu();
    return 0;
}