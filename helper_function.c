#include "lexerDef.h"
#include <stdbool.h>

void printTokenInfo(tokenInfo tk);

bool isEqual(char c1, char c2){
    return c1 == c2;
}

bool isInRange(char c, char start, char end){
    return c >= start && c <= end;
}