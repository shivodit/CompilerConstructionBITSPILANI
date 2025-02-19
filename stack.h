#include "stackDef.h"

// Stack helper functions
Stack* createStack();
void push(Stack* stack, Symbol symbol, TreeNode* parent);
Symbol pop(Stack* stack);
Symbol top(Stack* stack);
TreeNode* topParent(Stack* stack);
bool isEmpty(Stack* stack);
void freeStack(Stack* stack);
