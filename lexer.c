#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 1024  // Twin buffer size
#define MAX_LEXEME_LENGTH 100

typedef enum {
    TK_TYPE, TK_INT, TK_COLON, TK_SEM, TK_ASSIGNOP, TK_NUM, TK_ID, TK_EOF
} TOKEN;

typedef struct {
    char lexeme[MAX_LEXEME_LENGTH];
    TOKEN token;
} tokenInfo;

typedef struct {
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int forward;  // Current character position
    int activeBuffer;  // Which buffer is active
    FILE *fp;
} twinBuffer;

// Function to load data into the buffer
FILE *getStream(FILE *fp, twinBuffer *B) {
    if (fp == NULL) {
        printf("Error: File not found.\n");
        return NULL;
    }
    size_t bytesRead = fread(B->buffer1, sizeof(char), BUFFER_SIZE, fp);
    B->buffer1[bytesRead] = '\0';  // Null-terminate
    B->forward = 0;
    B->activeBuffer = 1;
    B->fp = fp;
    return fp;
}

// Function to check if a lexeme is a keyword
TOKEN checkKeyword(char *lexeme) {
    if (strcmp(lexeme, "type") == 0) return TK_TYPE;
    if (strcmp(lexeme, "int") == 0) return TK_INT;
    return TK_ID;
}

// Function to get the next token
tokenInfo getNextToken(twinBuffer *B) {
    tokenInfo token;
    memset(token.lexeme, 0, MAX_LEXEME_LENGTH);

    int state = 0;
    int lexemeLength = 0;

    while (1) {
        char c = B->buffer1[B->forward++];
        
        switch (state) {
            case 0:
                if (isalpha(c)) {  // Identifier or Keyword
                    token.lexeme[lexemeLength++] = c;
                    state = 1;
                } else if (isdigit(c)) {  // Number
                    token.lexeme[lexemeLength++] = c;
                    state = 2;
                } else if (c == '<') {  // Assignment operator
                    token.lexeme[lexemeLength++] = c;
                    state = 3;
                } else if (c == ':') {  // Colon
                    token.lexeme[lexemeLength++] = c;
                    token.token = TK_COLON;
                    return token;
                } else if (c == ';') {  // Semicolon
                    token.lexeme[lexemeLength++] = c;
                    token.token = TK_SEM;
                    return token;
                } else if (isspace(c)) {  
                    continue;  // Skip whitespace
                } else if (c == '\0') {  // End of buffer
                    if (feof(B->fp)) {
                        token.token = TK_EOF;
                        return token;
                    }
                    size_t bytesRead = fread(B->buffer1, sizeof(char), BUFFER_SIZE, B->fp);
                    B->buffer1[bytesRead] = '\0';
                    B->forward = 0;
                } else {  // Unrecognized symbol
                    printf("Lexical Error: Unrecognized character '%c'\n", c);
                }
                break;

            case 1:  // Identifier or Keyword
                if (isalnum(c)) {
                    token.lexeme[lexemeLength++] = c;
                } else {
                    B->forward--;  // Retract
                    token.lexeme[lexemeLength] = '\0';
                    token.token = checkKeyword(token.lexeme);
                    return token;
                }
                break;

            case 2:  // Integer number
                if (isdigit(c)) {
                    token.lexeme[lexemeLength++] = c;
                } else {
                    B->forward--;  // Retract
                    token.lexeme[lexemeLength] = '\0';
                    token.token = TK_NUM;
                    return token;
                }
                break;

            case 3:  // Assignment operator "<---"
                if (c == '-' && B->buffer1[B->forward] == '-' && B->buffer1[B->forward + 1] == '-') {
                    token.lexeme[lexemeLength++] = '-';
                    token.lexeme[lexemeLength++] = '-';
                    token.lexeme[lexemeLength++] = '-';
                    B->forward += 2;
                    token.token = TK_ASSIGNOP;
                    return token;
                } else {
                    B->forward--;  // Retract
                    token.token = TK_ID;
                    return token;
                }
                break;
        }
    }
}

int main() {
    FILE *fp = fopen("program.txt", "r");
    if (!fp) {
        printf("Error: Unable to open file.\n");
        return 1;
    }

    twinBuffer B;
    getStream(fp, &B);

    tokenInfo token;
    do {
        token = getNextToken(&B);
        printf("Token: %d, Lexeme: %s\n", token.token, token.lexeme);
    } while (token.token != TK_EOF);

    fclose(fp);
    return 0;
}
