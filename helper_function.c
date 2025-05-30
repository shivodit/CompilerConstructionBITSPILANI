/*
Group No: 15
Group members:
1. Shivodit Raj Vishnoi - 2022A7PS1221P
2. Parsewar Omkar Balaji - 2022A7PS0089P
3. Samiksha Kaul - 2022A7PS1169P
4. Sohan Reddy Jalakanti - 2022A7PS1177P
5. Arnav Gupta - 2022A7PS1189P
6. Akshat Gosain - 2022A7PS0154G
*/
#include "lexerDef.h"
#include "parserDef.h"
#include <stdbool.h>
#include <stdio.h>  
#include <string.h> // new addition 14/3/25 12pm

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

bool isEqual(char c1, char c2){
    return c1 == c2;
}

bool isInRange(char c, char start, char end){
    return c >= start && c <= end;
}

bool iswhitespace(char c){
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

void printTokenInfo(tokenInfo tk){
    printf("Line No: %d\t\t Lexeme: %s\t\t Token: %s\n", tk.line_no, tk.lexeme, getTokenName(tk.token));
}

void printTokenInfoList(tokenInfo** tk_list){
    for (int i = 0; tk_list[i] != NULL; i++){
        printTokenInfo(*tk_list[i]);
    }
}