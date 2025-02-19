#include "parserDef.h"

RuleList* readRules(char* grammarfile);
FirstAndFollow ComputeFirstAndFollowSets (RuleList G);
ParseTable* createParseTable(FirstAndFollow F, ParseTable* T);
TreeNode* parseInputSourceCode(char *testcasefile, ParseTable T, bool verbose);
void printParseTree(TreeNode* PT, char *outfile);
