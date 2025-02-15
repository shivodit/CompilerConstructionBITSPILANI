#include "lexerDef.h"
#include <stdbool.h>
#include <stdio.h>  

void printTokenInfo(tokenInfo tk){
    printf("Line no.\tLexeme\t\tToken\n");
    printf("Line No: %d\t\t Lexeme: %s\t\t Token: %d\n", tk.line_no, tk.lexeme, tk.token);
}

void printTokenInfoList(tokenInfo** tk_list){
    for (int i = 0; tk_list[i] != NULL; i++){
        printTokenInfo(*tk_list[i]);
    }
}   

bool isEqual(char c1, char c2){
    return c1 == c2;
}

bool isInRange(char c, char start, char end){
    return c >= start && c <= end;
}