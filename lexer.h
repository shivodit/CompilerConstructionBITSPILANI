#include "lexerDef.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void initializeTwinBuffer();

FILE *getStream(FILE *fp);

void removeComments(char *testcaseFile, char *cleanFile);