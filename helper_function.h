#include <stdio.h>
#include "lexerDef.h"
#include "parserDef.h"
#include <stdbool.h>

int getNonTermIndex(char* nt);
int getTokenIndex(char* tk);
const char* getNonTermName(NON_TERMINAL nt);
const char* getTokenName(TOKEN tk);
bool isEqual(char c1, char c2);
bool isInRange(char c, char start, char end);
bool iswhitespace(char c);
void printTokenInfo(tokenInfo tk);
void printTokenInfoList(tokenInfo** tk_list);