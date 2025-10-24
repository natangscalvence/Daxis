#include "daxis_ast.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Allocate and initialize a constructor node from AST.
 */
ASTNode* dx_ast_new(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Allocation memory error to ASTNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

/**
 * Add a child node to child list from a parent node.
 * Uses realloc to increase the dynamic array for child nodes.
 */
void dx_ast_add_child(ASTNode* parent, ASTNode* child) {
    parent->children_count++;

    /**
     * Realloc the array to free space for a new pointer
     */
    parent->children = (ASTNode**)realloc(parent->children,
    parent->children_count * sizeof(ASTNode*));

    if (!parent->children) {
        perror("Memory allocation error when adding child node.");
        exit(EXIT_FAILURE);
    }
    /**
     * Add the new child node at the final of array
     */
    parent->children[parent->children_count - 1] = child;
}

/**
 * Recursively frees the tree memory (cleanup function)
 */
void dx_ast_free(ASTNode* node) {
    if (!node) {
        return;
    }

    /**
     * Recursively free all children node.
     */
    for (int i = 0; i < node->children_count; i++) {
        dx_ast_free(node->children[i]);
    }

    /**
     * Free the allocated memory to name (bug string fixed)
     */
    if (node->type == AST_IDENTIFIER || node->type == AST_VAR_DECL) {
        /**
         * Assume that the name was copied and allocated with malloc/strdup at the
         * parser.
         */
        free((void*)node->data.named.name);
    }
    /**
     * Free the pointer array for children node.
     */
    free(node->children);
    
    /**
     * Free the own node.
     */
    free(node);
}

