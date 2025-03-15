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
#include "stackDef.h"
#include <stdlib.h>
#include <stdio.h>

bool isEmpty(Stack* stack);
StackNode* createStackNode(Symbol symbol, TreeNode* parent);
Stack* createStack();
void push(Stack* stack, Symbol symbol, TreeNode* parent);
Symbol pop(Stack* stack);
Symbol top(Stack* stack);
TreeNode* topParent(Stack* stack);
void freeStack(Stack* stack);


// Check if the stack is empty
bool isEmpty(Stack* stack) {
    return stack->top == NULL;
}


StackNode* createStackNode(Symbol symbol, TreeNode* parent) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) {
        printf("Failed to allocate stack node\n");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }
    node->symbol = symbol;
    node->next = NULL;
    node->parent = parent;
    return node;
}

// Create a new stack
Stack* createStack() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) {
        printf("Failed to allocate stack\n");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }
    stack->top = NULL;

    // pushing dollar symbol onto the stack 
    // VERIFY
    Symbol dollar = {true, .symbol.t = DOLLAR};
    push(stack, dollar, NULL); // Push the dollar symbol onto the stack
    return stack;
}

// Push a symbol onto the stack
void push(Stack* stack, Symbol symbol, TreeNode* parent) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) {
        printf("Failed to allocate stack node\n");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }
    node->symbol = symbol;
    node->next = stack->top;
    node->parent = parent;
    stack->top = node;
}

// Pop a symbol from the stack
Symbol pop(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack underflow\n");
        exit(EXIT_FAILURE); // Exit if stack is empty
    }
    StackNode* node = stack->top;
    stack->top = node->next;
    Symbol symbol = node->symbol;
    free(node);
    return symbol;
}

// Get the top symbol from the stack
Symbol top(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty\n");
        exit(EXIT_FAILURE); // Exit if stack is empty
    }
    return stack->top->symbol;
}

// Get the parent of the top symbol from the stack
TreeNode* topParent(Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty\n");
        exit(EXIT_FAILURE); // Exit if stack is empty
    }
    return stack->top->parent;
}

// Free the stack
void freeStack(Stack* stack) {
    StackNode* node = stack->top;
    while (node) {
        StackNode* temp = node;
        node = node->next;
        free(temp);
    }
    free(stack);
}