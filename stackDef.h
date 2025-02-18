#ifndef STACKDEF_H
#define STACKDEF_H
#include "parserDef.h"

typedef struct StackNode StackNode;
typedef struct Stack Stack;

// Stack Node
struct StackNode{
    Symbol symbol;
    StackNode* next;
};

// Stack 
struct Stack{
    StackNode* top;
};

#endif