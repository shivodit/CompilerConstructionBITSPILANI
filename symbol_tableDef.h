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
#ifndef SYMBOL_TABLE_DEF_H
#define SYMBOL_TABLE_DEF_H

#include "lexerDef.h"

#define SYMBOL_TABLE_SIZE 397  // A prime number for better distribution

// Node structure for each entry in the symbol table's linked list.
typedef struct SymbolNode {
    tokenInfo token;           // Token information as defined in lexerDef.h
    struct SymbolNode *next;   // Pointer to the next node in the chain
} SymbolNode;

// The symbol table structure: an array of pointers to SymbolNode.
typedef struct {
    SymbolNode **table;
} SymbolTable;

#endif
