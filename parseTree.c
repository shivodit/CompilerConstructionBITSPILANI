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
#include "parseTreeDef.h"
#include "lexerDef.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new tree node
// TreeNode* createTreeNode(Symbol symbol, bool isleaf, tokenInfo* token) {
//     TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
//     if (!node) {
//         printf("Failed to allocate tree node\n");
//         exit(EXIT_FAILURE); // Exit if allocation fails
//     }
//     node->symbol = symbol;
//     node->isleaf = isleaf;
//     node->token = token;
//     node->parent = NULL;
//     node->child = NULL;
//     node->next = NULL;
//     node->prev = NULL;
//     return node;
// }

TreeNode* createTreeNode(Symbol symbol, bool isleaf, tokenInfo* token) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) {
        printf("Failed to allocate tree node\n");
        exit(EXIT_FAILURE);
    }
    node->symbol = symbol;
    node->isleaf = isleaf;

    if (token != NULL) {
        // Allocate memory for a new tokenInfo structure
        node->token = (tokenInfo*)malloc(sizeof(tokenInfo));
        if (!node->token) {
            printf("Failed to allocate token info\n");
            exit(EXIT_FAILURE);
        }
        // Copy the content of token into the newly allocated tokenInfo
        *(node->token) = *token;
        // Deep copy the lexeme if it exists
        if (token->lexeme != NULL) {
            node->token->lexeme = strdup(token->lexeme);
            if (node->token->lexeme == NULL) {
                printf("Failed to allocate lexeme copy\n");
                exit(EXIT_FAILURE);
            }
        }
    } else {
        node->token = NULL;
    }

    node->parent = NULL;
    node->child = NULL;
    node->next = NULL;
    node->prev = NULL;
    return node;
}


// Add a child to a parent node
void addChild(TreeNode* parent, TreeNode* child) {
    if (parent->child == NULL) {
        parent->child = child;
    } else {
        TreeNode* temp = parent->child;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = child;
        child->prev = temp;
    }
    child->parent = parent;
}

// free the treenode and its children
void freeNode(TreeNode* node) {
    if (node == NULL) return;
    TreeNode* child = node->child;
    while (child != NULL) {
        TreeNode* next = child->next;
        freeNode(child);
        child = next;
    }
    free(node);
}

// remove a node from the tree
void removeNode(TreeNode* node) {
    // assuming if node is root then remove the whole tree
    if (node->parent == NULL){
        freeNode(node);
        return;
    }

    TreeNode* parent = node->parent;
    if (parent->child == node) {
        parent->child = node->next;
    } else {
        node->prev->next = node->next;
        if (node->next != NULL) {
            node->next->prev = node->prev;
        }
    }

    freeNode(node);
    return;
}