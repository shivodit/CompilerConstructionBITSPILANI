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