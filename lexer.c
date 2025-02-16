#include "lexerDef.h"
#include "helper_function.h"
#include "symbol_table.h"

#include <math.h>
// use -lm flag while compiling
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*---------------------------------------------------------------------------------------------------------------------------------------------*/

twinBuffer* tb; // global twinBuffer struct 
bool first_call_to_nextc = true; // to check if nextc is called for the first time

// STATE VARIABLES 
bool is_eof_file = false;
FILE* curr_file = NULL;
SymbolTable* symbol_table;
int DFA_STATE = 0; // current state of the DFA

// bool completeFileRead = false; // stores whether the input file has been completely read

// create new token 
tokenInfo* newTokenInfo(TOKEN tk, char* lx, int line_no, TAGGED_VALUE value){
    tokenInfo* tk_info = (tokenInfo*)malloc(sizeof(tokenInfo));
    tk_info->token = tk;
    tk_info->lexeme = lx;
    tk_info->line_no = line_no;
    tk_info->value = value;
    return tk_info;
}

tokenInfo* copyTokenInfo(tokenInfo* tk){
    tokenInfo* tk_info = (tokenInfo*)malloc(sizeof(tokenInfo));
    tk_info->token = tk->token;
    tk_info->lexeme = tk->lexeme;
    tk_info->line_no = tk->line_no;
    tk_info->value = tk->value;
    return tk_info;
}

void freeTokenInfo(tokenInfo* tk){
    free(tk->lexeme);
    free(tk);
}

void initializeTwinBuffer(){
    tb = (twinBuffer*)malloc(sizeof(twinBuffer));
    if (!tb) {
        printf("Failed to allocate twinBuffer\n");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }

    tb->B[0] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    tb->B[1] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    if (!tb->B[0] || !tb->B[1]) {
        printf("Failed to allocate buffers");
        exit(EXIT_FAILURE); // Exit if allocation fails
    }
    tb->bufferToBeLoaded = 0; 
    tb->arePointersInDifferentBuffers = false;
    tb->ip = 0; 
    tb->fp = 0;
    tb->fp_line_no = 1;
    tb->ip_line_no = 1;
}

// initialize the lexer with the file pointer
// ALWAYS CALL BEFORE LEXICAL ANALYSIS
void initializelexer(FILE* fp){
    curr_file = fp;
    is_eof_file = false;
    initializeTwinBuffer();
    symbol_table = createSymbolTable();
    DFA_STATE = 0;
    first_call_to_nextc = true;
}

FILE *getStream(FILE *fp){
    if(fp == NULL){ // or if(completeFileRead)
        return NULL; // or return fp ?
    }

    memset(tb->B[tb->bufferToBeLoaded], EOF, BUFFER_SIZE); 

    // initializing the buffer with EOF so that if the reamining data in file is lesser than BUFFER_SIZE, the remaining part of the buffer is filled with EOFs
    // TAG: FREAD MIGHT BE WRONG
    int charactersRead = fread(tb->B[tb->bufferToBeLoaded], sizeof(char), BUFFER_SIZE, fp);
    if(charactersRead == 0 || charactersRead < BUFFER_SIZE){
        tb->bufferToBeLoaded = !tb->bufferToBeLoaded;
        // completeFileRead = true;
        return fp; // or return fp ?
    }
    if(charactersRead == -1){
        printf("Error loading buffer\n");
    }

    tb->bufferToBeLoaded = !tb->bufferToBeLoaded;
    return fp;
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/
// TODO : Verify locally
char nextc(){
    char c;
    if (first_call_to_nextc){
        first_call_to_nextc = false;
        if(getStream(curr_file) == NULL){
            is_eof_file = true;
            return EOF;
        }
    }

    if(tb->fp < BUFFER_SIZE-1){
        c = tb->B[!tb->bufferToBeLoaded][tb->fp++]; // since fp is always present in B[!bufferToBeLoaded]
    }
    else if(tb->fp == BUFFER_SIZE-1){
        // load the next buffer
        if(getStream(curr_file) == NULL){
            return EOF;
        }
        tb->fp = 0;
        c = tb->B[!tb->bufferToBeLoaded][tb->fp++];
    }
    else{
        printf("Unexpected error (forward pointer goes out of bound)\n");
        exit(EXIT_FAILURE); // ?
    }
    if (c == EOF) is_eof_file = true;
    if(c == '\n') tb->fp_line_no++;
    return c;
}

void retract(){
    if (tb->fp == 0) { 
        // If fp is at the start of a buffer, move to the previous buffer
        tb->arePointersInDifferentBuffers = !tb->arePointersInDifferentBuffers;
        tb->fp = sizeof(tb->B[0]) - 1;  // Assuming fixed buffer size, adjust as needed
        //Move the file pointer back, otherwise the upcoming data will be loaded twice
        fseek(curr_file, sizeof(tb->B[0]), SEEK_CUR);
        // Mark the buffer as the another buffer needs to be reloaded if we are moving to the previous buffer
        tb->bufferToBeLoaded = !tb->bufferToBeLoaded;
    } else {
        tb->fp--;  // Move back within t=e same buffer
    }
}

char* getLexeme(){
    int length;
    if(tb->arePointersInDifferentBuffers){
        length = (BUFFER_SIZE - tb->ip + tb->fp) + 1; // extra +1 is for '/0'
    }
    else{
        length = (tb->fp - tb->ip) + 1; // extra +1 is for '/0'
    }

    char* lex = (char*)malloc(length*sizeof(char));
    if(tb->arePointersInDifferentBuffers){
        int x = BUFFER_SIZE - tb->ip;
        for(int i=0;i<x;i++){
            lex[i] =tb->B[!tb->bufferToBeLoaded][tb->ip+i];
        }
        for(int i=0;i<length-x-1;i++){
            lex[i+x] = tb->B[!tb->bufferToBeLoaded][i];
        }
    }
    else{
        for(int i=0;i<length-1;i++){
            lex[i]=tb->B[!tb->bufferToBeLoaded][tb->ip+i];
        }
    }
        
    lex[length-1] = '\0';
    return lex;
}

void accept(){
    tb->ip=tb->fp;
    tb->ip_line_no = tb->fp_line_no;
    return;
}

int getLineNumber(){
    return tb->ip_line_no;
}

// TODO: Implement state wise error handling
void handleError(int current_state, char c){
    int line_no = getLineNumber();
    printf("Error at line %d: Invalid character %c in state %d\n", line_no, c, current_state);
    return;
}

tokenInfo* action(TOKEN tk, TAGGED_VALUE value, short int retract_num){
    while (retract_num--){
        retract();
    }
    char* lx = getLexeme();
    if (tk == TK_COMMENT){
        lx = "%";
    }

    tokenInfo* searched_tk = searchToken(symbol_table, lx);
    tokenInfo* tk_info = NULL;

    if (searched_tk == NULL){
        tk_info = newTokenInfo(tk, lx, getLineNumber(), value);
        insertToken(symbol_table, *tk_info);
    }
    else{
        // free(lx);
        tk_info = copyTokenInfo(searched_tk);
        tk_info->line_no = getLineNumber();
        tk_info->value = value;

    }

    accept();
    return tk_info;
}

tokenInfo* getNextToken(){
    // initialize dfa to start state
    DFA_STATE = 0;
    float n = 0;
    int s = 1;
    while (true){
        char c = nextc();
        if (c == EOF){
            return NULL;
        }

        switch (DFA_STATE){
            case 0:{
                if (iswhitespace(c)){
                    accept();
                    continue;
                }
                else if (isEqual(c,'#')) DFA_STATE = 1;
                else if (isEqual(c,'_')) DFA_STATE = 4;
                else if (isEqual(c,'@')) DFA_STATE = 35;
                else if (isEqual(c,'&')) DFA_STATE = 38;
                else if (isEqual(c,'!')) DFA_STATE = 29;
                else if (isEqual(c,'>')) DFA_STATE = 26;
                else if (isEqual(c,'=')) DFA_STATE = 24;
                else if (isEqual(c,'<')) DFA_STATE = 16;
                else if (isEqual(c,'%')) DFA_STATE = 14;
                else if (isEqual(c,'a') || isInRange(c,'e','z')) DFA_STATE = 12;
                else if (isInRange(c,'b','d')) DFA_STATE = 8;
                else if (isEqual(c,'+')) return action(TK_PLUS, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,'-')) return action(TK_MINUS, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,'*')) return action(TK_MUL, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,'/')) return action(TK_DIV, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,'.')) return action(TK_DOT, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,',')) return action(TK_COMMA, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,';')) return action(TK_SEM, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,':')) return action(TK_COLON, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,'(')) return action(TK_OP, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,')')) return action(TK_CL, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,'[')) return action(TK_SQL, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,']')) return action(TK_SQR, (TAGGED_VALUE){.type = NA}, 0);
                else if (isEqual(c,'~')) return action(TK_NOT, (TAGGED_VALUE){.type = NA}, 0);
                else if (isInRange(c,'0','9')){
                    n = (c - '0');
                    DFA_STATE = 50;
                }
                else{
                    // symbol not recognized
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            }
            case 1:
                if (isInRange(c,'a','z')) DFA_STATE = 2;
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 2:
                if (isInRange(c,'a','z')) DFA_STATE = 2;
                // check validity of other transition
                else{
                    return action(TK_RUID,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 4:
                if (isInRange(c,'a','z')||isInRange(c,'A','Z')) DFA_STATE = 5;
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 5:
                if (isInRange(c,'a','z')||isInRange(c,'A','Z')) DFA_STATE = 5;
                else if (isInRange(c,'0','9')) DFA_STATE = 7;
                else{
                    return action(TK_FUNID,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 7:
                if (isInRange(c,'0','9')) DFA_STATE = 7;
                else{
                    return action(TK_FUNID,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 50:
                if (isInRange(c,'0','9')){
                    n = n*10 + (c - '0');
                    DFA_STATE = 50;
                }
                else if (isEqual(c,'.')){
                    DFA_STATE = 52;
                }
                else{
                    return action(TK_NUM,(TAGGED_VALUE){.type = INT_VAL, .val.i = (int)n}, 1);
                }
                break;
        
            case 52:    
                if (isInRange(c,'0','9')){
                    n = n + (c - '0')*0.1;
                    DFA_STATE = 53;
                }
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 53:
                if (isInRange(c,'0','9')){
                    n = n + (c - '0')*0.01;
                    DFA_STATE = 54;
                }
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 54:
                if (isEqual(c,'E')) DFA_STATE = 55;
                else{
                    return action(TK_RNUM,(TAGGED_VALUE){.type = FLOAT_VAL, .val.f = n}, 1);
                }
                break;
            case 55:
                if (isEqual(c,'+') || isEqual(c,'-')){
                    DFA_STATE = 56;
                    s = (c == '+') ? 1 : -1;
                }
                else if(isInRange(c,'0','9')){
                    n = n*pow(10,(c-'0')*10);
                    DFA_STATE = 57;
                }
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 56:
                if (isInRange(c,'0','9')){
                    n = n*pow(10,s*(c-'0')*10);
                    DFA_STATE = 57;
                }
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 57:
                if (isInRange(c,'0','9')){
                    n = n*pow(10,s*(c-'0'));
                    DFA_STATE = 58;
                }
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 58:
                return action(TK_RNUM,(TAGGED_VALUE){.type = FLOAT_VAL, .val.f = n}, 1);
                break;
            case 8:
                if(isInRange(c,'2','7')) DFA_STATE = 9;
                else{
                    return action(TK_FIELDID,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 9:
                if(isInRange(c,'2','7')) DFA_STATE = 10;
                else if(isInRange(c,'b','d')) DFA_STATE = 9;
                else{
                    return action(TK_ID,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 10:
                if(isInRange(c,'2','7')) DFA_STATE = 10;
                else{
                    return action(TK_ID,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 12:
                if(isInRange(c,'a','z')) DFA_STATE = 12;
                else{
                    return action(TK_FIELDID,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 14:
                if (isEqual(c,'\n')){
                    return action(TK_COMMENT,(TAGGED_VALUE){.type = NA}, 0);
                }
                else{
                    DFA_STATE = 14;
                }
                break;
            case 16:
                if (isEqual(c,'=')) return action(TK_LE,(TAGGED_VALUE){.type = NA}, 0);
                else if(isEqual(c, '-')) DFA_STATE = 18;
                else{
                    return action(TK_LT,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 18:
                if (isEqual(c,'-')) DFA_STATE = 19;
                else {
                    return action(TK_LT,(TAGGED_VALUE){.type = NA}, 2);
                }
                break;
            case 19:
                if (isEqual(c,'-')) return action(TK_ASSIGNOP,(TAGGED_VALUE){.type = NA}, 0);
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 24:
                if (isEqual(c,'=')) return action(TK_EQ,(TAGGED_VALUE){.type = NA}, 0);
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 26:
                if (isEqual(c,'=')) return action(TK_GE,(TAGGED_VALUE){.type = NA}, 0);
                else{
                    return action(TK_GT,(TAGGED_VALUE){.type = NA}, 1);
                }
                break;
            case 29:
                if (isEqual(c,'=')) return action(TK_NE,(TAGGED_VALUE){.type = NA}, 0);
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 35:
                if (isEqual(c,'@')) DFA_STATE = 36;
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 36:
                if (isEqual(c,'@')) return action(TK_OR,(TAGGED_VALUE){.type = NA}, 0);
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 38:
                if (isEqual(c,'&')) DFA_STATE = 39;
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            case 39:
                if (isEqual(c,'&')) return action(TK_AND,(TAGGED_VALUE){.type = NA}, 0);
                else{
                    handleError(DFA_STATE, c);
                    return NULL;
                }
                break;
            default:
                handleError(DFA_STATE, c);
                return NULL;
                break;
        }
    }
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/

tokenInfo** getAllTokens(char* testcasefile, bool verbose){
    // open file and initializeTwinBuffer
    FILE* fp = fopen(testcasefile, "r");
    initializelexer(fp);
    tokenInfo** tokenlist = (tokenInfo**)calloc(TOKEN_LIST_SIZE,sizeof(tokenInfo*));
    int token_count = 0;
    int cap = TOKEN_LIST_SIZE;

    while (!is_eof_file){
        tokenInfo* curr_token = getNextToken();
        if (curr_token != NULL) tokenlist[token_count++] = curr_token;
        else continue;

        if (verbose) {
            printTokenInfo(*curr_token);
        }
        if (token_count == cap){
            cap *= 2;
            tokenlist = (tokenInfo**)realloc(tokenlist, cap*sizeof(tokenInfo*));
        }
    }

    // free the memory allocated for twinBuffer
    free(tb->B[0]);
    free(tb->B[1]);
    free(tb);
    freeSymbolTable(symbol_table);
    fclose(fp);
    return tokenlist;
}


/*---------------------------------------------------------------------------------------------------------------------------------------*/

void removeComments(char *testcaseFile, char *cleanFile){
    FILE* TEST = fopen(testcaseFile, "r");
    FILE* CLEAN = fopen(cleanFile, "w");
    char c;
    while (TEST != NULL && (c = fgetc(TEST)) != EOF){
        if (c=='%'){
            while (c != '\n' && c != EOF){
                c = fgetc(TEST);
            }
        }
        else{
            fputc(c, CLEAN);
        }
    }
    fclose(TEST);
    fclose(CLEAN);
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/

// temporary
// int main(){
//     char* g = "testfile.txt";
//     char* p = "cleanfile.txt";
//     removeComments(g,p);
// }