#include "parserDef.h"

FirstAndFollow ComputeFirstAndFollowSets (Rule* G);
ParseTable* createParseTable(FirstAndFollow F, ParseTable* T);
TreeNode* parseInputSourceCode(char *testcasefile, ParseTable T, bool verbose);
void printParseTree(TreeNode* PT, char *outfile);
