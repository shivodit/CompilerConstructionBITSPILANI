#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "symbol_tableDef.h"
#include <stdlib.h>

#define SYMBOL_TABLE_SIZE 211  // A prime number for better hashing.

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
