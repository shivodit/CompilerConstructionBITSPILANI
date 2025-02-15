#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to map reserved keywords to their token types.
typedef struct {
    const char *lexeme;
    TOKEN token;
} Keyword;

// List of reserved keywords and their corresponding token values.
static const Keyword keywordList[] = {
    {"with", TK_WITH},
    {"parameters", TK_PARAMETERS},
    {"end", TK_END},
    {"while", TK_WHILE},
    {"union", TK_UNION},
    {"endunion", TK_ENDUNION},
    {"definetype", TK_DEFINETYPE},
    {"as", TK_AS},
    {"type", TK_TYPE},
    {"main", TK_MAIN},
    {"global", TK_GLOBAL},
    {"parameter", TK_PARAMETER},
    {"list", TK_LIST},
    {"input", TK_INPUT},
    {"output", TK_OUTPUT},
    {"int", TK_INT},
    {"real", TK_REAL},
    {"endwhile", TK_ENDWHILE},
    {"if", TK_IF},
    {"then", TK_THEN},
    {"endif", TK_ENDIF},
    {"read", TK_READ},
    {"write", TK_WRITE},
    {"return", TK_RETURN},
    {"call", TK_CALL},
    {"record", TK_RECORD},
    {"endrecord", TK_ENDRECORD},
    {"else", TK_ELSE},
    {"and", TK_AND},
    {"or", TK_OR},
    {"not", TK_NOT}
};
static const int NUM_KEYWORDS = sizeof(keywordList) / sizeof(keywordList[0]);

// Hash function: uses the DJB2 algorithm to compute a hash value for a lexeme.
unsigned int hashFunction(const char *lexeme) {
    unsigned long hash = 5381;
    int c;
    while ((c = *lexeme++))
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    return hash % SYMBOL_TABLE_SIZE;
}


SymbolTable* createSymbolTable() {
    SymbolTable *st = (SymbolTable*)malloc(sizeof(SymbolTable));
    if (!st) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    st->table = (SymbolNode**)malloc(SYMBOL_TABLE_SIZE * sizeof(SymbolNode*));
    if (!st->table) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++)
        st->table[i] = NULL;
    
    // Initialize the table with reserved keywords.
    initializeKeywords(st);
    return st;
}

// Insert a tokenInfo into the symbol table.
void insertToken(SymbolTable *st, tokenInfo token) {
    unsigned int index = hashFunction(token.lexeme);
    SymbolNode *newNode = (SymbolNode*)malloc(sizeof(SymbolNode));
    if (!newNode) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    newNode->token = token;
    newNode->next = st->table[index];  // Insert at head of the chain.
    st->table[index] = newNode;
}

// Search for a tokenInfo in the symbol table using its lexeme.
tokenInfo* searchToken(SymbolTable *st, const char *lexeme) {
    unsigned int index = hashFunction(lexeme);
    SymbolNode *node = st->table[index];
    while (node) {
        if (strcmp(node->token.lexeme, lexeme) == 0)
            return &node->token;
        node = node->next;
    }
    return NULL;
}

// Delete a tokenInfo from the symbol table given its lexeme.
void deleteToken(SymbolTable *st, const char *lexeme) {
    unsigned int index = hashFunction(lexeme);
    SymbolNode *node = st->table[index];
    SymbolNode *prev = NULL;
    while (node) {
        if (strcmp(node->token.lexeme, lexeme) == 0) {
            if (prev)
                prev->next = node->next;
            else
                st->table[index] = node->next;
            free(node->token.lexeme);
            free(node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

// Free all memory associated with the symbol table.
void freeSymbolTable(SymbolTable *st) {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        SymbolNode *node = st->table[i];
        while (node) {
            SymbolNode *temp = node;
            node = node->next;
            free(temp->token.lexeme);
            free(temp);
        }
    }
    free(st->table);
    free(st);
}

// Initialize the symbol table with reserved keywords.
void initializeKeywords(SymbolTable *st) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        tokenInfo token;
        token.lexeme = strdup(keywordList[i].lexeme);
        if (!token.lexeme) {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        token.token = keywordList[i].token;
        token.line_no = -1;    // Reserved keywords do not need a line number.
        token.value.type = NA; // Not applicable for keywords.
        insertToken(st, token);
    }
}

// Handle a new identifier: if it exists in the table, return it; otherwise, create and insert a new tokenInfo.
tokenInfo* handleIdentifier(SymbolTable *st, const char *lexeme, int line_no) {
    tokenInfo *existing = searchToken(st, lexeme);
    if (existing != NULL)
        return existing;
    
    tokenInfo token;
    token.lexeme = strdup(lexeme);
    if (!token.lexeme) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    token.token = TK_ID;    // Mark as identifier.
    token.line_no = line_no;
    token.value.type = NA;  // No numeric value.
    insertToken(st, token);
    return searchToken(st, lexeme);
}

// For testing purposes (remove or modify in integration)
#ifdef TEST_SYMBOL_TABLE
#include <assert.h>
int main() {
    SymbolTable *st = createSymbolTable();
    
    // Test: search for a reserved keyword.
    tokenInfo *t1 = searchToken(st, "if");
    if (t1 != NULL)
        printf("Keyword 'if' found with token type %d\n", t1->token);
    else
        printf("Keyword 'if' not found\n");

    // Test: handle a new identifier.
    tokenInfo *id1 = handleIdentifier(st, "myVar", 42);
    printf("Identifier 'myVar' inserted with token type %d at line %d\n", id1->token, id1->line_no);

    // Ensure that searching returns the same identifier.
    tokenInfo *id2 = searchToken(st, "myVar");
    assert(id1 == id2);
    
    freeSymbolTable(st);
    return 0;
}
#endif
