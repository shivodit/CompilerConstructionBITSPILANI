#include "parserDef.h"
#include <stdio.h>
#include <stdlib.h>
// #include "helper_function.h"
#include <string.h>

const char *tokenList[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "EPSILON",
    "DOLLAR",
    "ERROR"
};

const char *nonTermList[] = {
    "PROGRAM",
    "OTHERFUNCTIONS",
    "MAINFUNCTION",
    "STMTS",
    "FUNCTION",
    "INPUT_PAR",
    "OUTPUT_PAR",
    "PARAMETER_LIST",
    "DATATYPE",
    "REMAINING_LIST",
    "PRIMITIVEDATATYPE",
    "CONSTRUCTEDDATATYPE",
    "TYPEDEFINITIONS",
    "DECLARATIONS",
    "OTHERSTMTS",
    "RETURNSTMT",
    "TYPEDEFINITION",
    "ACTUALORREDEFINED",
    "DEFINETYPESTMT",
    "FIELDDEFINITIONS",
    "FIELDDEFINITION",
    "MOREFIELDS",
    "FIELDTYPE",
    "DECLARATION",
    "GLOBAL_OR_NOT",
    "STMT",
    "ASSIGNMENTSTMT",
    "ITERATIVESTMT",
    "CONDITIONALSTMT",
    "IOSTMT",
    "FUNCALLSTMT",
    "SINGLEORRECORUNIONID",
    "ARITHMETICEXPRESSION",
    "OPTION_SINGLE_CONSTRUCTED",
    "ONEEXPANSION",
    "MOREEXPANSIONS",
    "OUTPUTPARAMETERS",
    "INPUTPARAMETERS",
    "IDLIST",
    "BOOLEANEXPRESSION",
    "ELSEPART",
    "VAR",
    "TERM",
    "EXPPRIME",
    "LOWPRECEDENCEOPERATORS",
    "FACTOR",
    "TERMPRIME",
    "HIGHPRECEDENCEOPERATORS",
    "LOGICALOP",
    "RELATIONALOP",
    "OPTIONALRETURN",
    "MORE_IDS",
    "A"
};

int getNonTermIndex(char* nt){
    for (int i = 0; i < sizeof(nonTermList)/sizeof(nonTermList[0]); i++){
        if (strcmp(nt, nonTermList[i]) == 0){
            return i;
        }
    }
    return -1;
}

int getTokenIndex(char* tk){
    for (int i = 0; i < sizeof(tokenList)/sizeof(tokenList[0]); i++){
        if (strcmp(tk, tokenList[i]) == 0){
            return i;
        }
    }
    return -1;
}

const char* getNonTermName(NON_TERMINAL nt){
    return nonTermList[nt];
}

const char* getTokenName(TOKEN tk){
    return tokenList[tk];
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


int main (){
    FirstAndFollow* fl = readFirstAndFollowSets("ex_firstfollow.txt");
    for (int i = 0; i < fl->num_entries; i++){
        printf("Non-terminal: %s\n", getNonTermName(fl->entries[i].non_terminal));
        printf("First set: ");
        for (int j = 0; j < fl->entries[i].num_first; j++){
            printf("%s ", getTokenName(fl->entries[i].first[j]));
        }
        printf("\n");
        printf("Follow set: ");
        for (int j = 0; j < fl->entries[i].num_follow; j++){
            printf("%s ", getTokenName(fl->entries[i].follow[j]));
        }
        printf("\n");
    }
    return 0;
}