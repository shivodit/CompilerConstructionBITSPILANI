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
#ifndef LEXER_DATA_DEF
#define LEXER_DATA_DEF

#include <stdbool.h>

#define BUFFER_SIZE 64 // buffer size of a single buffer
#define TOKEN_LIST_SIZE 4096

typedef enum {
    TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, TK_WITH,
    TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, TK_MAIN, 
    TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, 
    TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ,
    TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, 
    TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE, 
    // auxiliary symbols
    EPSILON, DOLLAR, ERROR
} TOKEN;

typedef enum{
    INT_VAL, FLOAT_VAL, NA
} TYPE_TAG;

typedef union {
    int i; 
    float f;
} VALUE;

typedef struct {
    TYPE_TAG type;
    VALUE val;
} TAGGED_VALUE;

typedef struct {
    char *lexeme;
    TOKEN token;
    int line_no;
    // tagged value for numeric data
    TAGGED_VALUE value;
} tokenInfo;

typedef struct{
    char* B[2]; // twin buffer to store the data read from the file
    bool bufferToBeLoaded;
    bool arePointersInDifferentBuffers; // true if ip and fp are in different buffers
    int ip; // pointer to the first char in lexeme
    int fp; // pointer to the current char in lexeme
    int fp_line_no; // line number of the char at fp (basically, line no. of lexeme)
    int ip_line_no;
    int charactersReadLastTime;
} twinBuffer;

#endif