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

        // verify
        // also consider the same thing for ERROR
        if(curr_token->token == 1){ // in case of TK_COMMENT, skip an iteration 
            continue;
        }
        
        // main logic
        Symbol top_symbol = top(stack);

        if (curr_token == NULL) {
            if (verbose) {
                printf("End of file reached or Error\n");
            }
            
            // STOP CONDITION
            if (has_file_ended() && top_symbol.is_terminal && top_symbol.symbol.t == DOLLAR){
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
        
        if (top_symbol.is_terminal){
            // terminal symbol
            if (top_symbol.symbol.t == curr_token->token){
                // MATCH
                TreeNode* leaf = createTreeNode(top_symbol, true, curr_token);
                addChild(parentNode, leaf);
                pop(stack);
            }
            else{
                // ERROR
                // PANIC MODE
                printf("Error: Unexpected token %s at line %d\n", curr_token->lexeme, curr_token->line_no);
                // pop(stack);
            }
        }
        else{
            // non-terminal symbol
            // verify
            int t1_index = top_symbol.symbol.nt; // first index of location of rule in parse table T
            int t2_index  = curr_token->token; // second index of location of rule in parse table T
            int g_index = T->table[t1_index][t2_index]; // index of rule in RuleList G
            Symbol* rule = grammar->rules[g_index].rhs;
            int num_rhs = grammar->rules[g_index].num_rhs;
            TreeNode* node = createTreeNode(top_symbol, false, NULL);
            addChild(parentNode, node);
            
            parentNode = node; // node becomes the parent node for the next iteration because it is a non-terminal
            pop(stack);
            while(num_rhs--){
                push(stack, rule[num_rhs-1]);
            }
        }
    }
    cleanup();

    return parse_tree;
}

void printParseTree(TreeNode* PT, char *outfile){

}







