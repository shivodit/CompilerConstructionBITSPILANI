/*
Group No: 15
Group members:
1. Shivodit Raj Vishnoi - 2022A7PS1221P
2. Parsewar Omkar Balaji - 2022A7PS0089P
3. Samiksha Kaul - 2022A7PS1169P
4. Sohan Reddy Jalakanti - 2022A7PS1177P
5. Arnav Gupta - 2022A7PS1189P
6. Akshat Gosain - 2022A7PS0154G
*/
#include "parserDef.h"
#include "parseTreeDef.h"

RuleList* readRules(char* grammarfile);
FirstAndFollow* ComputeFirstAndFollowSets (RuleList G);
ParseTable* createParseTable(FirstAndFollow F);
TreeNode* parseInputSourceCode(char *testcasefile, bool verbose);
void printParseTree(TreeNode* PT, char *outfile);
ParseTable* initializeparser(char* grammarfile, char* first_follow);