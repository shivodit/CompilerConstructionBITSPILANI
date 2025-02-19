#include "parserDef.h"
#include "parseTree.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

RuleList* readRules(char* grammarfile){
    // TODO
    return NULL;
}

FirstAndFollow* readFirstAndFollowSets(char* first, char* follow){
    // TODO
    return NULL;
}

FirstAndFollow* ComputeFirstAndFollowSets (RuleList G){
    // TODO
    return NULL;
}
ParseTable* createParseTable(FirstAndFollow F){
    // TODO
    return NULL;
}

TreeNode* parseInputSourceCode(char *testcasefile, ParseTable T, bool verbose){
    Stack* stack = createStack();
    TreeNode* parse_tree = createTreeNode((Symbol){false, .symbol.nt=PROGRAM}, false, NULL);

    // init lexer
    FILE* fp = fopen(testcasefile, "r");
    initializelexer(fp);
    int token_count = 0;

    while (!has_file_ended()){
        tokenInfo* curr_token = getNextToken();
        if (curr_token == NULL) continue;
        token_count++;
        if (verbose) {
            printTokenInfo(*curr_token);
        }
        // main logic 
        
        
    }
    cleanup();
}

void printParseTree(TreeNode* PT, char *outfile){

}
