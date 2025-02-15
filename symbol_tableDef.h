#ifndef SYMBOL_TABLE_DEF_H
#define SYMBOL_TABLE_DEF_H

#include "lexerDef.h"

#define SYMBOL_TABLE_SIZE 211  // A prime number for better distribution

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
