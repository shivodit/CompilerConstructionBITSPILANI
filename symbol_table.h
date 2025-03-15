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
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "symbol_tableDef.h"
#include <stdlib.h>

// #define SYMBOL_TABLE_SIZE 211  // A prime number for better hashing.

// Function declarations
unsigned int hashFunction(const char *lexeme);
SymbolTable* createSymbolTable();
void insertToken(SymbolTable *st, tokenInfo token);
tokenInfo* searchToken(SymbolTable *st, const char *lexeme);
void deleteToken(SymbolTable *st, const char *lexeme);
void freeSymbolTable(SymbolTable *st);
void initializeKeywords(SymbolTable *st);
tokenInfo* handleIdentifier(SymbolTable *st, const char *lexeme, int line_no);

#endif // SYMBOL_TABLE_H
