/*
Group No: 15
Group members:
1. Shivodit Raj Vishnoi - 2022A7PS1221P
2. Parsewar Omkar Balaji - 2022A7PS0089P
3. Samiksha Kaul - 2022A7PS1169P
4. Sohan Reddy Jalakanti - 2022A7PS1177P
5. Arnav Gupta - 2022A7PS1189P
6. Akshat Gosain - 2022A7PS0154G
*/
#include <stdio.h>
#include "lexerDef.h"
#include <stdbool.h>

void initializeTwinBuffer();
FILE *getStream(FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);
    
// Add the missing function declarations
void initializelexer(FILE* fp);
char nextc();
void retract();
char* getLexeme();
void accept();
int getLineNumber();
tokenInfo* action(TOKEN tk, TAGGED_VALUE value, short int retract_num);
tokenInfo* getNextToken();
tokenInfo** getAllTokens(char* testcasefile, bool verbose);
void freeTokenInfo(tokenInfo* tk);
bool has_file_ended();
void cleanup();