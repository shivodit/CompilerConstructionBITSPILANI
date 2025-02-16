#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

int main(){
    FILE* fp = fopen("testcase.txt", "r");
    if (fp == NULL){
        printf("File not found\n");
        return 1;
    }
    char c;
    while ((c = fgetc(fp)) != EOF){
        printf("%c", c);
    }
    fclose(fp);
    return 0;
}