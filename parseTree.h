#include "parseTreeDef.h"
#include "lexerDef.h"

TreeNode* createTreeNode(Symbol symbol, bool isleaf, tokenInfo* token);
void addChild(TreeNode* parent, TreeNode* child);
void removeNode(TreeNode* node);
void freeNode(TreeNode* node);