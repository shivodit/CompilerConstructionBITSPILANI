#include <stdio.h>
#include "lexerDef.h"

#include <stdbool.h>

void printTokenInfo(tokenInfo tk);

bool iswhitespace(char c);
bool isEqual(char c1, char c2);
bool isInRange(char c, char start, char end);
void printTokenInfoList(tokenInfo** tk_list);