#include "parserDef.h"
#include "parseTree.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

RuleList* grammar;
FirstAndFollow* F;
ParseTable* T;

initailizeparser(char* grammarfile, char* first, char* follow){
    grammar = readRules(grammarfile);
    F = readFirstAndFollowSets(first, follow);
    T = createParseTable(*F);
}

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

TreeNode* parseInputSourceCode(char *testcasefile,  bool verbose){

    Stack* stack = createStack();
    push(stack, (Symbol){true, .symbol.nt = PROGRAM});

    TreeNode* parse_tree = createTreeNode((Symbol){false, .symbol.nt=PROGRAM}, false, NULL);

    // init lexer
    FILE* fp = fopen(testcasefile, "r");
    initializelexer(fp);
    int token_count = 0;

    TreeNode* parentNode = parse_tree;

    while (true){
        tokenInfo* curr_token = getNextToken();
        token_count++;
        
        // main logic
        Symbol* top_symbol = top(stack);

        
        if (curr_token == NULL) {
            if (verbose) {
                printf("End of file reached or Error\n");
            }
            
            // STOP CONDITION
            if (has_file_ended() && top_symbol->is_terminal && top_symbol->symbol.t == DOLLAR){
                break;             
            }
            else if (has_file_ended()){
                // ERROR
                printf("Error: Unexpected end of file\n");
                break;
            }

            // TOKEN ERROR {DECIDE WHETHER TO INVOKE PANIC MODE}
            continue;
        }

        if (verbose) {
            printTokenInfo(*curr_token);
        }
        
        if (top_symbol->is_terminal){
            // terminal symbol
            
        }
        else{
            // non-terminal symbol 

        }
    }
    cleanup();

    return parse_tree;
}

void printParseTree(TreeNode* PT, char *outfile){

}
