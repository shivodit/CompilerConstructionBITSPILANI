#include "parserDef.h"


FirstAndFollow ComputeFirstAndFollowSets (Rule* G);
createParseTable(FirstAndFollow F, ParseTable* T);
parseInputSourceCode(char *testcaseFile, ParseTable T);
void printParseTree(TreeNode* PT, char *outfile);
