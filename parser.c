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

bool exists(int* arr, int count, int element) {
    for (int i = 0; i < count; i++) {
        if (arr[i] == element)
            return true;
    }
    return false;
}

FFEntry* findFFEntry(FirstAndFollow F, NON_TERMINAL nt) {
    for (int i = 0; i < F.num_entries; i++) {
        if (F.entries[i].non_terminal == nt) {
            return &F.entries[i];
        }
    }
    return NULL;
}

void computeFirstOfProduction(Rule rule, FirstAndFollow F, int numTerminals,
                              bool* containsEpsilon, int* firstSet, int* firstCount) {
    *firstCount = 0;
    *containsEpsilon = true; 

    for (int i = 0; i < rule.num_rhs; i++) {
        Symbol X = rule.rhs[i];
        if (X.is_terminal) {
            
            if (!exists(firstSet, *firstCount, X.symbol.t))
                firstSet[(*firstCount)++] = X.symbol.t;
            *containsEpsilon = false;
            break;
        } else {
            
            FFEntry* entry = findFFEntry(F, X.symbol.nt);
            if (entry == NULL) {
                *containsEpsilon = false;
                break;  
            }
            bool epsilonInX = false;
            for (int j = 0; j < entry->num_first; j++) {
                if (entry->first[j] == EPSILON) {
                    epsilonInX = true;
                } else {
                    if (!exists(firstSet, *firstCount, entry->first[j]))
                        firstSet[(*firstCount)++] = entry->first[j];
                }
            }
            if (!epsilonInX) {
                *containsEpsilon = false;
                break;
            }
            
        }
    }
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

ParseTable* createParseTable(FirstAndFollow F, ParseTable* T) {
    int i, j;
    
    // Allocating the 2D table with dimensions [num_non_terminals][num_terminals]
    T->table = (int**)malloc(T->num_non_terminals * sizeof(int*));
    if (T->table == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for parse table rows.\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < T->num_non_terminals; i++) {
        T->table[i] = (int*)malloc(T->num_terminals * sizeof(int));
        if (T->table[i] == NULL) {
            fprintf(stderr, "Error: Unable to allocate memory for parse table columns.\n");
            exit(EXIT_FAILURE);
        }
        // Initialize each cell to -1 (indicating no production)
        for (j = 0; j < T->num_terminals; j++) {
            T->table[i][j] = -1;
        }
    }
    
    // For every production rule in the grammar, determining where to place it in the table.
    for (int r = 0; r < grammar->num_rules; r++) {
        Rule rule = grammar->rules[r];
        NON_TERMINAL A = rule.lhs;  
        
        // Allocate temporary space for FIRST(rule.rhs)
        int* firstSet = (int*)malloc(T->num_terminals * sizeof(int));
        if (firstSet == NULL) {
            fprintf(stderr, "Error: Memory allocation failed for FIRST set computation.\n");
            exit(EXIT_FAILURE);
        }
        int firstCount = 0;
        bool derivesEpsilon = false;
        
        // Computing FIRST set for the right-hand side of the production.
        computeFirstOfProduction(rule, F, T->num_terminals, &derivesEpsilon, firstSet, &firstCount);
        
        // For each terminal in FIRST (except EPSILON), update the table.
        for (i = 0; i < firstCount; i++) {
            if (firstSet[i] != EPSILON) {
                T->table[A][firstSet[i]] = r;
            }
        }
        
        // If the production can derive epsilon, then for every terminal in FOLLOW(A),
        // updating the table.
        if (derivesEpsilon) {
            FFEntry* entry = findFFEntry(F, A);
            if (entry != NULL) {
                for (j = 0; j < entry->num_follow; j++) {
                    T->table[A][entry->follow[j]] = r;
                }
            }
        }
        free(firstSet);
    }
    
    return T;
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
