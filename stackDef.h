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
#ifndef STACKDEF_H
#define STACKDEF_H
#include "parserDef.h"
#include "parseTree.h"

typedef struct StackNode StackNode;
typedef struct Stack Stack;

// Stack Node
struct StackNode{
    Symbol symbol;
    StackNode* next;
    TreeNode* parent;
};

// Stack 
struct Stack{
    StackNode* top;
};

#endif