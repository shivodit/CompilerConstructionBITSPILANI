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
#include "parseTreeDef.h"
#include "lexerDef.h"

TreeNode* createTreeNode(Symbol symbol, bool isleaf, tokenInfo* token);
void addChild(TreeNode* parent, TreeNode* child);
void removeNode(TreeNode* node);
void freeNode(TreeNode* node);