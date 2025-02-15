#include "lexerDef.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------------------------------------------------------------------------------------------------------------*/

// bool bufferToBeLoaded = false; // represents the buffer to be loaded next
// bool completeFileRead = false; // stores whether the input file has been completely read

// create new token 
tokenInfo* newTokenInfo(TOKEN tk, char* lx, int line_no, TAGGED_VALUE value){
    tokenInfo* tk_info = (tokenInfo*)malloc(sizeof(tokenInfo));
    tk_info->token = tk;
    tk_info->lexeme = lx;
    tk_info->line_no = line_no;
    tk_info->value = value;
    return tk_info;
}

twinBuffer* tb; // global twinBuffer struct 

void initializeTwinBuffer(){
    twinBuffer* tb = (twinBuffer*)malloc(sizeof(twinBuffer));
    tb->B[0] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    tb->B[1] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    tb->bufferToBeLoaded = 0; 
    tb->arePointersInDifferentBuffers = false;
    tb->ip = 0; 
    tb->fp = 0;
}

FILE *getStream(FILE *fp){
    if(fp == NULL){ // or if(completeFileRead)
        return NULL; // or return fp ?
    }

    memset(tb->B[tb->bufferToBeLoaded], EOF, BUFFER_SIZE); 
    // initializing the buffer with EOF so that if the reamining data in file is lesser than BUFFER_SIZE, the remaining part of the buffer is filled with EOFs

    int charactersRead = read(fp, tb->B[tb->bufferToBeLoaded], BUFFER_SIZE);
    if(charactersRead == 0 || charactersRead < BUFFER_SIZE){
        // completeFileRead = true;
        return NULL; // or return fp ?
    }
    if(charactersRead == -1){
        printf("Error loading buffer\n");
    }

    tb->bufferToBeLoaded = !tb->bufferToBeLoaded;
    return fp;
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/
// TODO`
char nextc(){
    char c;
    if(tb->fp < BUFFER_SIZE-1){
        tb->fp++;
        c = tb->B[!tb->bufferToBeLoaded][tb->fp]; // since fp is always present in B[!bufferToBeLoaded]
    }
    else if(tb->fp == BUFFER_SIZE-1){
        // load the next buffer
        if(getStream(curr_file) == NULL){
            return EOF;
        }
        tb->fp = 0;
        c = tb->B[!tb->bufferToBeLoaded][tb->fp];
    }
    else{
        printf("Unexpected error (forward pointer goes out of bound)\n");
        exit(EXIT_FAILURE); // ?
    }
    if(c == '\n') tb->fp_line_no++;
    return c;
}

void retract(){
    if (tb->fp == 0) { 
        // If fp is at the start of a buffer, move to the previous buffer
        tb->arePointersInDifferentBuffers = !tb->arePointersInDifferentBuffers;
        tb->fp = sizeof(tb->B[0]) - 1;  // Assuming fixed buffer size, adjust as needed
        //Move the file pointer back, otherwise the upcoming data will be loaded twice
        fseek(curr_file, sizeof(tb->B[0]), SEEK_CUR);
        // Mark the buffer as the another buffer needs to be reloaded if we are moving to the previous buffer
        tb->bufferToBeLoaded = !tb->bufferToBeLoaded;
    } else {
        tb->fp--;  // Move back within t=e same buffer
    }
}
/*---------------------------------------------------------------------------------------------------------------------------------------*/

void removeComments(char *testcaseFile, char *cleanFile){
    FILE* TEST = fopen(testcaseFile, "r");
    FILE* CLEAN = fopen(cleanFile, "w");
    char c;
    while (TEST != NULL && (c = fgetc(TEST)) != EOF){
        if (c=='%'){
            while (c != '\n' && c != EOF){
                c = fgetc(TEST);
            }
        }
        else{
            fputc(c, CLEAN);
        }
    }
    fclose(TEST);
    fclose(CLEAN);
}

char* getLexeme(){
    int length;
    if(tb->arePointersInDifferentBuffers){
        length = (BUFFER_SIZE - tb->ip + tb->fp + 1 )+ 1; //The exttra +1 is for '/0'
    }
    else{
        length= (tb->fp - tb->ip + 1 )+ 1; //The exttra +1 is for '/0'
    }
    char* lex = (char*)malloc(length*sizeof(char));
    if(tb->arePointersInDifferentBuffers){
        int x = sizeof(tb->B[0]) - tb->ip;
        for(int i=0;i<x+1;i++){
            *(lex+i)=tb->B[tb->bufferToBeLoaded][tb->ip+i];
        }
        for(int i=0;i<length-x-1;i++){
            *(lex+i+x+1)=tb->B[!tb->bufferToBeLoaded][i];
        }
    }else{
        for(int i=tb->ip;i<length;i++){
            *(lex+i)=tb->B[tb->bufferToBeLoaded][i];
        }
    }
    return lex;
}

void accept(){
    tb->ip=tb->fp;
    return;
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

tokenInfo getNextToken(){

}

/*-------------------g--------------------------------------------------------------------------------------------------------------------*/
tokenInfo** getAllTokens(char* testcasefile, bool verbose){
    FILE* fp = fopen(testcasefile, "r");
    initializeTwinBuffer();

}

// temporary
int main(){
    char* g = "testfile.txt";
    char* p = "cleanfile.txt";
    removeComments(g,p);
}