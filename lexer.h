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