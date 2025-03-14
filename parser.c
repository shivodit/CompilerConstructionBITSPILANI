#include "parserDef.h"
#include "parseTree.h"
#include "lexer.h"
#include "stack.h"
#include "helper_function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RuleList* grammar;
FirstAndFollow* F;
ParseTable* T;


// It is assumed that NUM_NONTERMINALS and NUM_TERMINALS are defined as follows:
#define NUM_NONTERMINALS (A + 1)   // A is the last nonterminal in the enum.
#define NUM_TERMINALS (ERROR + 1)  // ERROR is the last terminal in the TOKEN enum.

// Helper: add terminal to a boolean set. Returns true if the set was modified.
bool addToSet(bool set[NUM_TERMINALS], int terminal) {
    if (!set[terminal]) {
        set[terminal] = true;
        return true;
    }
    return false;
}

FirstAndFollow* ComputeFirstAndFollowSets(RuleList G) {
    // Local boolean matrices to keep track of membership.
    bool first[NUM_NONTERMINALS][NUM_TERMINALS];
    bool follow[NUM_NONTERMINALS][NUM_TERMINALS];
    int i, j, r;

    // Initialize all FIRST and FOLLOW entries to false.
    for (i = 0; i < NUM_NONTERMINALS; i++) {
        for (j = 0; j < NUM_TERMINALS; j++) {
            first[i][j] = false;
            follow[i][j] = false;
        }
    }

    // ============================================================
    // FIRST SET COMPUTATION
    // ============================================================
    bool changed;
    do {
        changed = false;
        // Process every production: A -> X1 X2 ... Xn
        for (r = 0; r < G.num_rules; r++) {
            Rule rule = G.rules[r];
            int NT = rule.lhs;
            bool productionNullable = true; // Assume production is nullable until proven otherwise.
            for (i = 0; i < rule.num_rhs; i++) {
                Symbol sym = rule.rhs[i];
                if (sym.is_terminal) {
                    // If the symbol is terminal:
                    // If it is the EPSILON token then (by convention) we add EPSILON.
                    if (sym.symbol.t == EPSILON) {
                        if (addToSet(first[NT], EPSILON))
                            changed = true;
                        // For a proper epsilon-production, there should be no other symbols.
                        productionNullable = true;
                    } else {
                        if (addToSet(first[NT], sym.symbol.t))
                            changed = true;
                        productionNullable = false;
                    }
                    // In either case, a terminal stops further processing.
                    break;
                }
                else {
                    // Symbol is a nonterminal X.
                    int X = sym.symbol.nt;
                    // Add everything from FIRST(X) except EPSILON.
                    for (j = 0; j < NUM_TERMINALS; j++) {
                        if (j == EPSILON)
                            continue;
                        if (first[X][j]) {
                            if (addToSet(first[NT], j))
                                changed = true;
                        }
                    }
                    // Only if X can derive EPSILON do we continue to the next symbol.
                    if (first[X][EPSILON])
                        productionNullable = true;
                    else {
                        productionNullable = false;
                        break;
                    }
                }
            }
            // If every symbol in the production was nullable, add EPSILON to FIRST(NT).
            if (productionNullable) {
                if (addToSet(first[NT], EPSILON))
                    changed = true;
            }
        }
    } while (changed);

    // ============================================================
    // FOLLOW SET COMPUTATION
    // ============================================================
    // By convention, add the end marker DOLLAR to the start symbol's FOLLOW set.
    int start = G.rules[0].lhs;
    follow[start][DOLLAR] = true;

    do {
        changed = false;
        // For every production NT -> X1 X2 ... Xn
        for (r = 0; r < G.num_rules; r++) {
            Rule rule = G.rules[r];
            int NT = rule.lhs;
            // For each symbol in the right-hand side
            for (i = 0; i < rule.num_rhs; i++) {
                Symbol sym = rule.rhs[i];
                if (!sym.is_terminal) {
                    int B = sym.symbol.nt;  // B is a nonterminal in the production.
                    bool betaNullable = true; // Will be true if the rest of the production is nullable.
                    // Process the sequence beta that follows B.
                    for (j = i + 1; j < rule.num_rhs; j++) {
                        Symbol betaSym = rule.rhs[j];
                        if (betaSym.is_terminal) {
                            // If betaSym is terminal and not EPSILON, add it to FOLLOW(B)
                            if (betaSym.symbol.t != EPSILON) {
                                if (addToSet(follow[B], betaSym.symbol.t))
                                    changed = true;
                            }
                            betaNullable = false;
                            break;
                        }
                        else {
                            int Y = betaSym.symbol.nt;
                            // Add FIRST(Y) \ {EPSILON} to FOLLOW(B)
                            for (int t = 0; t < NUM_TERMINALS; t++) {
                                if (t == EPSILON)
                                    continue;
                                if (first[Y][t]) {
                                    if (addToSet(follow[B], t))
                                        changed = true;
                                }
                            }
                            // Continue with beta only if Y is nullable.
                            if (first[Y][EPSILON])
                                betaNullable = true;
                            else {
                                betaNullable = false;
                                break;
                            }
                        }
                    }
                    // If the entire beta is nullable (or beta is empty),
                    // add FOLLOW(NT) to FOLLOW(B).
                    if (betaNullable) {
                        for (int t = 0; t < NUM_TERMINALS; t++) {
                            if (follow[NT][t]) {
                                if (addToSet(follow[B], t))
                                    changed = true;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);

    // ============================================================
    // Pack the results into a FirstAndFollow structure.
    // ============================================================
    FirstAndFollow* ff = (FirstAndFollow*) malloc(sizeof(FirstAndFollow));
    if (!ff) {
        perror("Malloc failed");
        exit(1);
    }
    ff->num_entries = NUM_NONTERMINALS;
    ff->entries = (FFEntry*) malloc(NUM_NONTERMINALS * sizeof(FFEntry));
    if (!ff->entries) {
        perror("Malloc failed");
        exit(1);
    }

    // For each nonterminal, count and copy its FIRST and FOLLOW sets.
    for (i = 0; i < NUM_NONTERMINALS; i++) {
        ff->entries[i].non_terminal = i;
        // FIRST set:
        int count_first = 0;
        for (j = 0; j < NUM_TERMINALS; j++)
            if (first[i][j])
                count_first++;
        ff->entries[i].num_first = count_first;
        if (count_first > 0) {
            ff->entries[i].first = (TERMINAL*) malloc(count_first * sizeof(TERMINAL));
            int index = 0;
            for (j = 0; j < NUM_TERMINALS; j++) {
                if (first[i][j])
                    ff->entries[i].first[index++] = (TERMINAL) j;
            }
        } else {
            ff->entries[i].first = NULL;
        }
        // FOLLOW set:
        int count_follow = 0;
        for (j = 0; j < NUM_TERMINALS; j++)
            if (follow[i][j])
                count_follow++;
        ff->entries[i].num_follow = count_follow;
        if (count_follow > 0) {
            ff->entries[i].follow = (TERMINAL*) malloc(count_follow * sizeof(TERMINAL));
            int index = 0;
            for (j = 0; j < NUM_TERMINALS; j++) {
                if (follow[i][j])
                    ff->entries[i].follow[index++] = (TERMINAL) j;
            }
        } else {
            ff->entries[i].follow = NULL;
        }
    }

    return ff;
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
            
            // verify
            int num = -1;
            if (fscanf(fp, "%d", &num) == 1){
                is_terminal = 0;
                symbol = num;
            }
            else{
                is_terminal = 1;
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

FirstAndFollow* readFirstAndFollowSets(char* first_follow){
    FILE* fp = fopen(first_follow, "r");

    if (fp == NULL){
        printf("Error: Could not open file %s\n", first_follow);
        exit(EXIT_FAILURE);
    }

    FirstAndFollow* F = (FirstAndFollow*)malloc(sizeof(FirstAndFollow));
    int num_entries;
    fscanf(fp, "%d", &num_entries);
    F->num_entries = num_entries;
    FFEntry* entries = (FFEntry*)malloc(num_entries*sizeof(FFEntry));
    int nt = 0;
    for (int i=0; i<num_entries; i++){
        int num_first;
        int num_follow;
        fscanf(fp, "%d %d", &num_first, &num_follow);
        TERMINAL* first = (TERMINAL*)malloc(num_first*sizeof(TERMINAL));
        TERMINAL* follow = (TERMINAL*)malloc(num_follow*sizeof(TERMINAL));
        for (int j=0; j<num_first; j++){
            int f;
            fscanf(fp, "%d", &f);
            first[j] = f;
        }
        for (int j=0; j<num_follow; j++){
            int f;
            fscanf(fp, "%d", &f);
            follow[j] = f;
        }
        entries[i] = (FFEntry){nt++, num_first, first, num_follow, follow};
    }
    F->entries = entries;
    return F;
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
        if (X.is_terminal && X.symbol.t == EPSILON) {
            *containsEpsilon = true;
            break;
        }
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

ParseTable* createParseTable(FirstAndFollow F) {
    int i, j;
    
    // init table
    T = (ParseTable*)malloc(sizeof(ParseTable));
    T->num_non_terminals = NUM_NONTERMINALS;
    T->num_terminals = NUM_TERMINALS;

    if (T == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for parse table.\n");
        exit(EXIT_FAILURE);
    }
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
        NON_TERMINAL NT = rule.lhs;  
        
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
        // printf("%d\n", firstCount);
        // printf("%d\n", firstSet[0]);
        // For each terminal in FIRST (except EPSILON), update the table.
        for (i = 0; i < firstCount; i++) {
            if (firstSet[i] != EPSILON) {
                T->table[NT][firstSet[i]] = r;
            }
        }
        // If the production can derive epsilon, then for every terminal in FOLLOW(NT),
        // updating the table.
        if (derivesEpsilon) {
            FFEntry* entry = findFFEntry(F, NT);
            if (entry != NULL) {
                for (j = 0; j < entry->num_follow; j++) {
                    T->table[NT][entry->follow[j]] = r;
                }
            }
        }
        free(firstSet);
    }
    return T;
}

ParseTable* initializeparser(char* grammarfile, char* first_follow){
    grammar = readRules(grammarfile);
    // debug
    for (int i=0; i<grammar->num_rules; i++){
        printf("%s -> ", getNonTermName(grammar->rules[i].lhs));
        for (int j=0; j<grammar->rules[i].num_rhs; j++){
            if (grammar->rules[i].rhs[j].is_terminal){
                printf("%s ", getTokenName(grammar->rules[i].rhs[j].symbol.t));
            }
            else{
                printf("%s ", getNonTermName(grammar->rules[i].rhs[j].symbol.nt));
            }
        }
        printf("\n");
    }

    F = ComputeFirstAndFollowSets(*grammar);
    //debug
    // printf("done\n");
    // printing first and follow sets
    for (int i=0; i<F->num_entries; i++){
        printf("Non-terminal: %s\n", getNonTermName(F->entries[i].non_terminal));
        printf("First: ");
        for (int j=0; j<F->entries[i].num_first; j++){
            printf("%s ", getTokenName(F->entries[i].first[j]));
        }
        printf("\n");
        printf("Follow: ");
        for (int j=0; j<F->entries[i].num_follow; j++){
            printf("%s ", getTokenName(F->entries[i].follow[j]));
        }
        printf("\n");
    }

    T = createParseTable(*F);
    
    // debug
    printf("initialization completed\n");

    return T;
}

TreeNode* parseInputSourceCode(char *testcasefile,  bool verbose){

    Stack* stack = createStack();
    push(stack, (Symbol){false, .symbol.nt = PROGRAM}, NULL);

    TreeNode* parse_tree = createTreeNode((Symbol){false, .symbol.nt=PROGRAM}, false, NULL);

    // init lexer
    FILE* fp = fopen(testcasefile, "r");
    initializelexer(fp);
    int token_count = 0;

    TreeNode* parentNode;
    Symbol top_symbol;

    tokenInfo* curr_token = getNextToken();
    // printTokenInfo(*curr_token);
    while (true){
        token_count++;
        // verify
        // also consider the same thing for ERROR
        if(curr_token->token == 1){ // in case of TK_COMMENT, skip an iteration 
            curr_token = getNextToken();
            continue;    
        }
        
        // main logic
        top_symbol = top(stack);
        parentNode = topParent(stack);
        // debug
        printf("Top of stack: %s\n", (top_symbol.is_terminal ? getTokenName(top_symbol.symbol.t) : getNonTermName(top_symbol.symbol.nt)));
        // debug
        if (verbose) {
            printf("Current token: %s\n", getTokenName(curr_token->token));
        }
        
        // error or eof condition
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
                if (parentNode == NULL){
                    addChild(parse_tree, leaf);
                }
                else{
                    addChild(parentNode, leaf);
                }
                pop(stack);
                curr_token = getNextToken();
            }
            else{
                // ERROR
                // PANIC MODE
                printf("Error: Unexpected token %s at line %d\n", curr_token->lexeme, curr_token->line_no);
                curr_token = getNextToken();
                // pop(stack);
            }
        }
        else{
            // non-terminal symbol
            // verify
            // first index of location of rule in parse table T
            // second index of location of rule in parse table T
            int g_index = T->table[top_symbol.symbol.nt][curr_token->token]; // index of rule in RuleList G
            
            if (g_index == -1){
                // ERROR
                printf("Error: No rule found for %s and %s\n", getNonTermName(top_symbol.symbol.nt), getTokenName(curr_token->token));
                // PANIC MODE
                exit(1);
                continue;
            }

            Symbol* rule = grammar->rules[g_index].rhs;
            int num_rhs = grammar->rules[g_index].num_rhs;
            TreeNode* node = createTreeNode(top_symbol, false, NULL);
            if (parentNode == NULL){
                addChild(parse_tree, node);
            }
            else{
                addChild(parentNode, node);
            }            
            // pop the top of the stack and push the rule in reverse order
            pop(stack);
            while(num_rhs--){
                if (rule[num_rhs].is_terminal && rule[num_rhs].symbol.t == EPSILON){
                    break;
                }
                push(stack, rule[num_rhs], node);
            }
        }
    }
    cleanup();

    return parse_tree;
}

void printNode(TreeNode* node, FILE* fp) {
    // getting lexeme and token
    const char* lexeme = ((node->token && node->token->lexeme) ? node->token->lexeme : "---");
    int line_no = ((node->token) ? node->token->line_no : -1);
    const char* tokenName = ((node->token) ? getTokenName(node->token->token) : "---");
    
    // For numbers
    char valueStr[32];
    if (node->token && node->token->value.type != NA) {
        if (node->token->value.type == INT_VAL)
        sprintf(valueStr, "%d", node->token->value.val.i);
        else if (node->token->value.type == FLOAT_VAL)
        sprintf(valueStr, "%f", node->token->value.val.f);
    }
    else
        strcpy(valueStr, "-");

    const char* parentSymbol;
    if (node->parent) {
        parentSymbol = (node->parent->symbol.is_terminal ?
                       getTokenName(node->parent->symbol.symbol.t) :
                       getNonTermName(node->parent->symbol.symbol.nt));
    } else {
        parentSymbol = "ROOT";
    }
    
    const char* isLeafStr = node->isleaf ? "yes" : "no";
    
    const char* nodeSymbol = (node->symbol.is_terminal ?
                             getTokenName(node->symbol.symbol.t) :
                             getNonTermName(node->symbol.symbol.nt));
    
    const char* currNodeDesc = node->isleaf ? "Leaf" : "Non-Terminal";
    
    // Print in the format:
    // lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode NodeSymbol
    fprintf(fp, "%s %s %d %s %s %s %s %s\n", 
            lexeme, currNodeDesc, line_no, tokenName, valueStr, parentSymbol, isLeafStr, nodeSymbol);
}

void inorderPrint(TreeNode* node, FILE* fp) {
    if (!node)
        return;
    
    // If the node has a child list, process in in-order fashion:
    // traverse the first child's subtree, then print the current node,
    // then traverse the remaining siblings of the first child.
    if (node->child) {
        // Traverse first child's subtree
        inorderPrint(node->child, fp);
        
        // Print the current node
        printNode(node, fp);
        
        // Traverse remaining siblings of the first child (if any)
        TreeNode* sibling = node->child->next;
        while (sibling) {
            inorderPrint(sibling, fp);
            sibling = sibling->next;
        }
    } else {
        // Leaf node: simply print
        printNode(node, fp);
    }
}

void printParseTree(TreeNode* PT, char *outfile) {
    FILE* fp = fopen(outfile, "w");
    if (!fp) {
        fprintf(stderr, "Error: Could not open file %s for writing.\n", outfile);
        return;
    }
    
    // Print header line (optional)
    fprintf(fp, "lexeme CurrentNode lineno tokenName valueIfNumber parentNodeSymbol isLeafNode NodeSymbol\n");
    
    inorderPrint(PT, fp);
    
    fclose(fp);
}
