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
#ifndef PARSER_DATA_DEF
#define PARSER_DATA_DEF
#include "lexerDef.h"
// #include "stackDef.h"
// #include "parseTree.h"
// Helper macros for numbers of nonterminals and terminals
#define NUM_NONTERMINALS (A + 1)   // Last enumerator of NON_TERMINAL is A.
#define NUM_TERMINALS (ERROR + 1)  // Last enumerator of TOKEN is ERROR.
typedef enum NON_TERMINAL {
    // non-terminal symbols
    PROGRAM, OTHERFUNCTIONS, MAINFUNCTION, STMTS, FUNCTION, INPUT_PAR, OUTPUT_PAR, PARAMETER_LIST, DATATYPE, REMAINING_LIST, PRIMITIVEDATATYPE, 
    CONSTRUCTEDDATATYPE, TYPEDEFINITIONS, DECLARATIONS, OTHERSTMTS, RETURNSTMT, TYPEDEFINITION, ACTUALORREDEFINED, 
    DEFINETYPESTMT, FIELDDEFINITIONS, FIELDDEFINITION, MOREFIELDS, FIELDTYPE, DECLARATION, GLOBAL_OR_NOT, STMT, 
    ASSIGNMENTSTMT, ITERATIVESTMT, CONDITIONALSTMT, IOSTMT, FUNCALLSTMT, SINGLEORRECORUNIONID, ARITHMETICEXPRESSION, 
    OPTION_SINGLE_CONSTRUCTED, ONEEXPANSION, MOREEXPANSIONS, OUTPUTPARAMETERS, INPUTPARAMETERS, IDLIST, BOOLEANEXPRESSION, 
    ELSEPART, VAR, TERM, EXPPRIME, LOWPRECEDENCEOPERATORS, FACTOR, TERMPRIME, HIGHPRECEDENCEOPERATORS, LOGICALOP, 
    RELATIONALOP, OPTIONALRETURN, MORE_IDS, A
} NON_TERMINAL;

typedef TOKEN TERMINAL;

typedef struct Symbol{
    bool is_terminal;
    union{
        NON_TERMINAL nt;
        TERMINAL t;
    } symbol;
} Symbol;

typedef struct Rule{
    NON_TERMINAL lhs;
    int num_rhs;
    Symbol* rhs;
} Rule;

typedef struct RuleList{
    int num_rules;
    Rule* rules;
} RuleList;

typedef struct ParseTable{
    int num_terminals;
    int num_non_terminals;
    int** table;
} ParseTable;

typedef struct FFEntry{
    NON_TERMINAL non_terminal;
    int num_first;
    TERMINAL* first;
    int num_follow;
    TERMINAL* follow;
} FFEntry;

typedef struct FirstAndFollow{
    int num_entries;
    FFEntry* entries;
} FirstAndFollow;

#endif