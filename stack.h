#include "stackDef.h"

// Stack helper functions
Stack* createStack();
void push(Stack* stack, Symbol symbol);
Symbol pop(Stack* stack);
Symbol top(Stack* stack);
bool isEmpty(Stack* stack);
void freeStack(Stack* stack);
