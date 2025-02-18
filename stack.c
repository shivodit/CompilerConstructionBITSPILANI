#include "stackDef.h"
#include <stdlib.h>

StackNode* createStackNode(Symbol symbol) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) {
        printf("Failed to allocate stack node\n");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }
    node->symbol = symbol;
    node->next = NULL;
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
    push(stack, DOLLAR); // Push the dollar symbol onto the stack
    return stack;
}

// Push a symbol onto the stack
void push(Stack* stack, Symbol symbol) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode));
    if (!node) {
        printf("Failed to allocate stack node\n");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }
    node->symbol = symbol;
    node->next = stack->top;
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

// Check if the stack is empty
bool isEmpty(Stack* stack) {
    return stack->top == NULL;
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