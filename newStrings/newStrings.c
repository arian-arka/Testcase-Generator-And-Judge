#include "newStrings.h"
#include <stdio.h>
#include<string.h>
#include <stdlib.h>

/**
 * String errors and max lengths
 */
#define MAX_STRING_LEN 1000000
#define MAX_READLINE_LEN 500000
#define INVALID_STRING_LENGTH 1
#define NULL_STING_POINTER 2
#define INDEX_OUT_OF_RANGE 3
#define COUDLNT_CONVERT_TO_NUMBER 3

/**
 * Used of check if wether string functions has error or not
 */


void SET_STRING_ERROR(int error){
    STRING_RUNTIME_ERROR=error;
}

void WriteLine(String *str){
    SET_STRING_ERROR(0);
    if(str==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return;
    }
    if(!str->len)
        return;
    printf("%s\n",str->data);
}

void Write(String *str){
    SET_STRING_ERROR(0);
    if(str==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return ;
    }
    if(!str->len)
        return;
    printf("%s",str->data);
}

/**
 * This function get a string and frees it completely from memory
 *
 * @param pointer to string structs
 * @return NULL
 */
String* FreeString(String *str){
    if(str!=NULL)
    {
        if(str->data!=NULL)
            free(str->data);
        free(str);
    }
    return NULL;
}

/**
 * This function get a string and frees its data from memory but not its struct and sets len to 0
 *
 * @param pointer to string structs
 * @return string *
 */
String*  FreeStringData(String *str) {
    if (str != NULL){
        if (str->data != NULL)
            free(str->data);
        str->len=0;
        str->data=NULL;
    }
    return str;
}

int IsEqualChars(String *a,char *b){
    if(a==NULL || b==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return 0;
    }
    int len=strlen(b);
    if(!a->len && !len)
        return 1;
    if(a->len==0)
        return 0;
    if(len==0)
        return 0;
    return strcmp(a->data,b)==0;
}

int IsEqual(String *a,String *b){
    if(a==NULL || b==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return 1;
    }
    if(!a->len && !b->len)
        return 1;
    if(a->len==0)
        return 0;
    if(b->len==0)
        return 0;
    return strcmp(a->data,b->data)==0;
}

String * NewString(int len){
    SET_STRING_ERROR(0);

    if (len<0 || len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }
    String *str=(String*)malloc(sizeof(String));
    str->len=len;
    str->data=NULL;
    if(len>0){
        str->data=(char*)malloc((len+1)*sizeof(char));
        str->data[0]='\0';
        str->data[len]='\0';
    }
    return str;
}

String * IncreaseStringSize(String *str,int len){
    SET_STRING_ERROR(0);
    if(len<=0)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }
    if(str==NULL)
    {
        return NewString(len);
    }
    if(str->len)
    {
        str->data=realloc(str->data,(len+str->len+1)*sizeof(char));
        str->len+=len;
        str->data[str->len]='\0';
        return str;
    }
    str=FreeStringData(str);
    str->data=(char*)malloc((len+1)*sizeof(char));
    str->data[0]='\0';
    str->data[len]='\0';
    str->len=len;
    return str;
}

String * InitString(char *src){
    SET_STRING_ERROR(0);
    if(src==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    int len=strlen(src);
    if (len<0 || len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }
    String *str=NewString(len);
    if(len==0)
        return str;
    strcpy(str->data,src);
    return str;
}

String * AppendStringChar(String * des,char src){
    SET_STRING_ERROR(0);
    if(des==NULL)
    {
        des=NewString(1);
        des->data[0]=src;
        return des;
    }
    if(des->len==0){
        des=IncreaseStringSize(des,1);
        des->data[0]=src;
        return des;
    }

    if(1+des->len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }
    des=IncreaseStringSize(des,1);
    des->data[des->len-1]=src;
    return des;
}

String * AppendStringChars(String * des,char *src){
    SET_STRING_ERROR(0);

    if(src==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    int len=strlen(src);

    if(len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }
    if(des==NULL)
    {
        des=NewString(len+1);
        strcpy(des->data,src);
        return des;
    }
    if(des->len==0){
        FreeStringData(des);
        des=IncreaseStringSize(des,len);
        strcpy(des->data,src);
        return des;
    }

    if(len+des->len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }
    des=IncreaseStringSize(des,len);
    strcat(des->data,src);
    return des;
}

String * AppendString(String * des,String *src){
    SET_STRING_ERROR(0);
    if(src==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    if(des==NULL)
    {
        des=NewString(src->len);
        if(src->len!=0)
            strcpy(des->data,src->data);
        return des;
    }
    if(des->len==0){
        FreeStringData(des);
        if(!src->len)
            return des;
        des=IncreaseStringSize(des,src->len);
        strcpy(des->data,src->data);
        return des;
    }
    if(src->len+des->len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }
    des=IncreaseStringSize(des,src->len);
    strcat(des->data,src->data);
    return des;
}

String *SetString_(String *des,char *src){
    SET_STRING_ERROR(0);
    if(des==NULL)
        return AppendStringChars(des,src);
    if(src==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    des=FreeStringData(des);
    des->len=strlen(src);
    des->data=(char*)malloc((1+des->len)*sizeof(char));
    strcpy(des->data,src);
    return des;
}

String *SetString(String *des,String *src){
    SET_STRING_ERROR(0);
    if(des==NULL)
        return AppendString(des,src);
    if(src==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    des=FreeStringData(des);
    des->len=src->len;
    if(des->len==0)
        return des;
    des->data=(char*)malloc((1+des->len)*sizeof(char));
    strcpy(des->data,src->data);
    return des;
}

String * InsertString_(String *des,int index,char *src){
    SET_STRING_ERROR(0);
    if(des==NULL)
    {
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    if(index>=des->len || index<0){
        SET_STRING_ERROR(INDEX_OUT_OF_RANGE);
        return NULL;
    }
    int len=strlen(src);
    if(!len)
        return des;
    if(len+des->len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }

    char *tmp=(char*)malloc(sizeof(char)*(len+des->len+1));
    memcpy(tmp,des->data, sizeof(char)*index);
    memcpy(tmp+index,src,sizeof(char)*len);
    memcpy(tmp+index+len,des->data+index,sizeof(char)*(des->len-index));
    tmp[len+des->len]='\0';
    free(des->data);
    des->data=tmp;
    des->len=len+des->len;
    return des;
}

String * InsertString(String *des,int index,String *src){
    SET_STRING_ERROR(0);
    if(des==NULL || src==NULL)
    {
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    if(index>=des->len || index<0){
        SET_STRING_ERROR(INDEX_OUT_OF_RANGE);
        return NULL;
    }
    if(!src->len)
        return des;
    if(src->len+des->len>MAX_STRING_LEN)
    {
        SET_STRING_ERROR(INVALID_STRING_LENGTH);
        return NULL;
    }

    char *tmp=(char*)malloc(sizeof(char)*(src->len+des->len+1));
    memcpy(tmp,des->data, sizeof(char)*index);
    memcpy(tmp+index,src->data,sizeof(char)*src->len);
    memcpy(tmp+index+src->len,des->data+index,sizeof(char)*(des->len-index));
    tmp[src->len+des->len]='\0';
    free(des->data);
    des->data=tmp;
    des->len=src->len+des->len;
    return des;
}

String *SubStr(String *str,int start,int end,int step){
    SET_STRING_ERROR(0);
    if(str==NULL)
    {
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    if(start<0 || end<start || step<0){
        SET_STRING_ERROR(INDEX_OUT_OF_RANGE);
        return NULL;
    }
    if(!str->len)
        return str;
    String *newStr=NewString((end-start+1)/step);
    newStr->data[newStr->len+1]='\0';
    for (int i = 0; i < newStr->len;i++)
        newStr->data[i]=str->data[start+i*step];
    return newStr;
}

/**
 * it gets a string and another string named content. if content is in str it returns the first occured char index otherwise -1.
 *
 * @param str
 * @param content
 * @return integer as result
 */
int StringContains(String *str,String *content){
    if(str==NULL||content==NULL)
    {
        SET_STRING_ERROR(NULL_STING_POINTER);
        return -1;
    }
    if(!str->len || !content->len || str->len<content->len)
        return -1;
    int i,j;
    if(str->len==content->len)
    {
        for (j = 0; j < content->len; ++j)
            if (str->data[j] != content->data[j])
                return -1;
        return 0;
    }

    for (i = 0; i <str->len - content->len ; ++i) {
        for (j = 0; j < content->len; ++j){
            if (str->data[i + j] != content->data[j])
                break;
        }
        if (j==content->len)
            return i;
    }
    return -1;
}

String *UpperCase(String *str){
    SET_STRING_ERROR(0);
    if(str==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    for (int i = 0; i < str->len; ++i) {
        if(str->data[i]>='a' && str->data[i]<='z')
            str->data[i]+=32;
    }
    return str;
}

String *LowerCase(String *str){
    SET_STRING_ERROR(0);
    if(str==NULL){
        SET_STRING_ERROR(NULL_STING_POINTER);
        return NULL;
    }
    for (int i = 0; i < str->len; ++i) {
        if(str->data[i]>='A' && str->data[i]<='Z')
            str->data[i]-=32;
    }
    return str;
}

int IsWhiteSpace(char c){
    return c==' '|| c=='\n' || c=='\t' || c=='\r' ;
}


String *ReadLine(){
    String *str=NewString(MAX_READLINE_LEN);
    fgets(str->data,MAX_READLINE_LEN-1, stdin);
    str->len=strlen(str->data)-1;
    str->data[str->len]='\0';
    return str;
}

void PressEnter(char *text){
    printf("%s\nPress enter to continue...",text);
    getchar();
}

int EmptyString(String * str){
    if(str==NULL)
        return 1;
    if (str->len==0)
        return 1;
    return str->data[0]=='\0';
}

/**
 * it gets a string and and if its data is a number it return a number greater than 0 otherwise it returns 0 as string
 *
 * @param str
 * @return 0 for string - 0 for negative int - 2 for negative int - 3 for negative double - 4 for negative double
 */
int IsStringNumber(String *str){
    int dot=0,sign=0;
    if(str==NULL || !str->len)
        return 0;
    if(str->data[0]=='.')
        return 0;
    for (int i = 0; i < str->len; ++i) {
        if(str->data[i]=='.')
            dot++;
        else if(str->data[i]=='+')
        {
            if(sign)
                return 0;
            sign=1;
        }
        else if(str->data[i]=='-')
        {
            if(sign)
                return 0;
            sign=2;
        }
        else if(str->data[i]>'9' || str->data[i]<'0')
            return 0;
        if(dot>1)
            return 0;

    }
    if(sign){
        if(str->len==1 || (dot && str->len==2))
            return 0;
        if(sign==2)
            return dot?4:2;
        return dot?3:1;
    }
    if(dot)
        return 3;
    return 1;

}

int StringToInt(String *str){
    if(!IsStringNumber(str))
    {
        SET_STRING_ERROR(COUDLNT_CONVERT_TO_NUMBER);
        return 0;
    }
    int num;
    sscanf(str->data, "%d", &num);
    return num;
}

double StringToDouble(String *str){
    if(!IsStringNumber(str))
    {
        SET_STRING_ERROR(COUDLNT_CONVERT_TO_NUMBER);
        return 0;
    }
    double num;
    sscanf(str->data, "%lf", &num);
    return num;
}

float StringToFloat(String *str){
    if(!IsStringNumber(str))
    {
        SET_STRING_ERROR(COUDLNT_CONVERT_TO_NUMBER);
        return 0;
    }
    float num;
    sscanf(str->data, "%f", &num);
    return num;
}

String * IntToString(int num){
    String * str=NewString(12);
    sprintf(str->data, "%d", num);
    return str;
}

String * DoubleToString(double num){
    String * str=NewString(23);
    sprintf(str->data, "%lf", num);
    return str;
}

String * FloatToString(float num){
    String * str=NewString(23);
    sprintf(str->data, "%f", num);
    return str;
}
