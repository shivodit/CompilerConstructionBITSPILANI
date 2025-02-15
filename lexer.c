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
    tb = (twinBuffer*)malloc(sizeof(twinBuffer));
    if (!tb) {
        printf("Failed to allocate twinBuffer\n");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }

    tb->B[0] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    tb->B[1] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    if (!tb->B[0] || !tb->B[1]) {
        printf("Failed to allocate buffers");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }
    tb->bufferToBeLoaded = 0; 
    tb->arePointersInDifferentBuffers = false;
    tb->ip = 0; 
    tb->fp = 0;
    tb->fp_line_no = 1;
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
// TODO
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

int getLineNo(){
    return tb->fp_line_no;
}

char* getLexeme(){ // using global twinBuffer struct
    
}

void retract(){

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