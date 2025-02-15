#include "lexerDef.h"
#include "helper_function.h"
#include "symbol_table.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------------------------------------------------------------------------------------------------------------*/
char* twinBuffer[2]; // twin buffer
bool bufferToBeLoaded = false; // represents the buffer to be loaded next

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

void initializelexer(FILE* fp){
    curr_file = fp;
    is_eof_file = false;
    initializeTwinBuffer();
    symbol_table = createSymbolTable();
    DFA_STATE = 0;
}

void initializeTwinBuffer(){
    twinBuffer[0] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    twinBuffer[1] = (char*)malloc(BUFFER_SIZE*sizeof(char));
    
    bufferToBeLoaded = 0;
}

FILE *getStream(FILE *fp){
    if(fp == NULL){ // or if(completeFileRead)
        return NULL; // or return fp ?
    }

    memset(twinBuffer[bufferToBeLoaded], EOF, BUFFER_SIZE); 
    // initializing the buffer with EOF so that if the reamining data in file is lesser than BUFFER_SIZE, the remaining part of the buffer is filled with EOFs

    int charactersRead = read(fp, twinBuffer[bufferToBeLoaded], BUFFER_SIZE);
    if(charactersRead == 0 || charactersRead < BUFFER_SIZE){
        // completeFileRead = true;
        return NULL; // or return fp ?
    }
    if(charactersRead == -1){
        printf("Error loading buffer\n");
    }

    bufferToBeLoaded = !bufferToBeLoaded;
    return fp;
}

/*---------------------------------------------------------------------------------------------------------------------------------------*/
// TODO: file interface functions
char nextc(){
    return " ";
}

char* getlexeme(){
    return NULL;
}

int getLineNumber(){
    return -1;
}

// TODO
void retract(){
    return ;
}

void accept(){
    return ;
}

tokenInfo* action(TOKEN tk, TAGGED_VALUE value, short int retract_num){
    while (retract_num--){
        retract();
    }

    char* lx = getlexeme();
    tokenInfo* searched_tk = searchToken(symbol_table, lx);
    tokenInfo* tk_info = NULL;
    if (searched_tk == NULL){
        tokenInfo* tk_info = newTokenInfo(tk, lx, getLineNumber(), value);
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
    while (true){
        char c = nextc();
        
        if (c == EOF){
            is_eof_file = true;
            // end of file
            return NULL;
        }

        switch (DFA_STATE){
            case 0:{
                if (isEqual(c, ' ')) continue;
                else if (isEqual(c,'\t')) continue;
                else if (isEqual(c,'\n')) continue;
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
                    printf("Invalid character %c\n", c);
                    // TO IMPLEMENT: error handling
                    return NULL;
                }
    
                break;
            }
            
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

    while (!is_eof_file){
        tokenInfo* curr_token = getNextToken();
        if (curr_token != NULL) tokenlist[token_count++] = curr_token;

        if (verbose) {
            printTokenInfo(*curr_token);
        }
    }
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
int main(){
    char* g = "testfile.txt";
    char* p = "cleanfile.txt";
    removeComments(g,p);
}