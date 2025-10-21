#include "daxis_ast.h"
#include <stdlib.h>
#include <stdio.h>

ASTNode* dx_ast_new(ASTNodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (!node) {
        perror("Failed to allocate ASTNode");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

void dx_ast_add_child(ASTNode* parent, ASTNode* child) {
    parent->children_count++;
    parent->children = (ASTNode**)realloc(parent->children,
        parent->children_count * sizeof(ASTNode*));

    if (!parent->children) {
        perror("Failed to realloc AST children");
        exit(EXIT_FAILURE);
    }
    parent->children[parent->children_count - 1] = child;
}