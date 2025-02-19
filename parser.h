#include "parserDef.h"
#include "parseTreeDef.h"

RuleList* readRules(char* grammarfile);
FirstAndFollow* ComputeFirstAndFollowSets (RuleList G);
ParseTable* createParseTable(FirstAndFollow F);
TreeNode* parseInputSourceCode(char *testcasefile, bool verbose);
void printParseTree(TreeNode* PT, char *outfile);
ParseTable* initializeparser(char* grammarfile, char* first_follow);