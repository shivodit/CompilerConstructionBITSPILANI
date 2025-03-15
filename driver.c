#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"
#include "parseTree.h"

void remove_comments(char *filename);
void print_token_list(char *filename);
void parse_and_verify(char *filename, char *output_filename);
void print_execution_time(char *filename, char *output_filename);


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <sourcecode file> <parsetree output file>\n", argv[0]);
        return 1;
    }

    char *source_file = argv[1];
    char *output_file = argv[2];

    int option;
    while (1) {
        printf("\nOptions:\n");
        printf("0: Exit\n");
        printf("1: Remove comments\n");
        printf("2: Print token list\n");
        printf("3: Parse and verify\n");
        printf("4: Print execution time\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 0:
                return 0;
            case 1:
                remove_comments(source_file);
                break;
            case 2:
                print_token_list(source_file);
                break;
            case 3:
                parse_and_verify(source_file, output_file);
                break;
            case 4:
                print_execution_time(source_file, output_file);
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

void remove_comments(char *filename) {
    char* cleanfile = "cleanfiletemporary.txt";
    removeComments(filename, cleanfile);
    FILE *file = fopen(cleanfile, "r");
    if (file == NULL) {
        perror("Error opening clean file");
        return;
    }
    printf("--- File after removing comments ---\n");
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file);

    printf("--- end of file ---\n");
    // Delete the clean file
    if (remove(cleanfile) != 0) {
        perror("Error deleting clean file");
    } else {
        printf("\nClean file deleted successfully.\n");
    }
}

void print_token_list(char *filename) {
    printf("Printing token list for %s...\n", filename);
    tokenInfo** tokenlist = getAllTokens(filename, true);

    // free the memory allocated for token list
    for (int i = 0; tokenlist[i] != NULL; i++) {
        freeTokenInfo(tokenlist[i]);
    }
    free(tokenlist);
    return;
}

void parse_and_verify(char *filename, char *output_filename) {
    // printf("Parsing and verifying (NOT YET IMPLEMENTED) %s output file is %s ...\n", filename, output_filename);
    // DEBUG statements inside parser.c initailizeparser
    initializeparser("final_grammar_index_clean.txt", "first_follow.txt");

    TreeNode* parse_tree = parseInputSourceCode(filename, false);

    printParseTree(parse_tree, output_filename);
    
    removeNode(parse_tree);
}

void print_execution_time(char *filename, char *output_filename) {
    clock_t start_time, end_time;
    double total_CPU_time, total_CPU_time_in_seconds;

    start_time = clock();

    parse_and_verify(filename, output_filename);

    end_time = clock();

    total_CPU_time = (double)(end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    printf("Total CPU time: %lf\n", total_CPU_time);
    printf("Total CPU time in seconds: %lf\n", total_CPU_time_in_seconds);
}