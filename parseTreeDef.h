#ifndef PARSE_TREE_DEF
#define PARSE_TREE_DEF
#include "lexerDef.h"
#include "parserDef.h"
#include <stdbool.h>

typedef struct TreeNode TreeNode;

typedef struct TreeNode{
    Symbol symbol;
    bool isleaf;
    tokenInfo* token;
    TreeNode* parent;
    TreeNode* child;

    // sibling next and prev pointers :)
    TreeNode* next;
    TreeNode* prev;
} TreeNode;

#endif