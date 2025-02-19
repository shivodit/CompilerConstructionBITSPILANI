#ifndef PARSER_DATA_DEF
#define PARSER_DATA_DEF
#include "lexerDef.h"
// #include "stackDef.h"
// #include "parseTree.h"

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