#include "parserDef.h"
#include "parseTree.h"
#include "lexer.h"
#include "stack.h"
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
    RuleList* rl = (RuleList*)malloc(sizeof(RuleList));
    // open file
    FILE* fp = fopen(grammarfile, "r");
    if (fp == NULL){
        printf("Error: Could not open file %s\n", grammarfile);
        exit(EXIT_FAILURE);
    }

    // read number of lines
    int num_rules;
    fscanf(fp, "%d", &num_rules);

    // read rules
    Rule* rules = (Rule*)malloc(num_rules*sizeof(Rule));
    for (int i=0; i<num_rules; i++){
        int lhs;
        int num_rhs = 0;
        fscanf(fp, "%d %d ->", &num_rhs, &lhs);
        Symbol* rhs = (Symbol*)malloc(num_rhs*sizeof(Symbol));
        for (int j=0; j<num_rhs; j++){
            int is_terminal;
            int symbol;
            
            int num = -1;
            if (fscanf(fp, "%d", &num) == 1){
                is_terminal = 1;
                symbol = num;
            }
            else{
                is_terminal = 0;
                fscanf(fp, "_%d", &symbol);
            }
            rhs[j] = (Symbol){is_terminal, .symbol = {.t = symbol}};
        }
        rules[i] = (Rule){lhs, num_rhs, rhs};
    }

    rl->num_rules = num_rules;
    rl->rules = rules;

    fclose(fp);
    return rl;
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
