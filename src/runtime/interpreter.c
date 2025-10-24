#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Check for a literal node (create a DaxisValue from token)
 */
static DaxisValue dx_interpret_literal(ASTNode* node) {
    const char* lexeme = node->data.literal.start;

    switch (node->data.literal.type) {
        case TOKEN_INT:
            return dx_make_int(atoll(lexeme));
        case TOKEN_FLOAT:
            return dx_make_float(strtod(lexeme, NULL));
        default:
            return dx_make_null();
    }
}

/**
 * Binary calc logic (addition/subtraction)
 */
static DaxisValue dx_interpret_binary(DaxisTokenType op, DaxisValue left, DaxisValue right) {
    /**
     * Type promotion logic (if has float, the result is float :p)
     */
    if (left.type == VAL_FLOAT || right.type == VAL_FLOAT) {
        double l = (left.type == VAL_FLOAT) ? left.data.f : (double)left.data.i;
        double r = (right.type == VAL_FLOAT) ? right.data.f : (double)right.data.i;
        double result;

        switch (op) {
            case TOKEN_PLUS:
                result = l + r;
                break;
            case TOKEN_MINUS: 
                result = l - r;
                break;
            default:
                return dx_make_null();
        }
        return dx_make_float(result);
    }
    /**
     * Logic for integers.
     */
    if (left.type == VAL_INT && right.type == VAL_INT) {
        long long l = left.data.i;
        long long r = right.data.i;
        long long result;

        switch (op) {
            case TOKEN_PLUS:
                result = l + r;
                break;
            case TOKEN_MINUS:
                result = l - r;
                break;
            default:
                return dx_make_null();
        }
        return dx_make_int(result);
    }
    fprintf(stderr, "Runtime erro: Incompatible types for binary operation.\n");
    return dx_make_null();
}

/**
 * Main interpreter engine.
 */
DaxisValue dx_interpret_node(ASTNode* node, DaxisScope* scope) {
    if (!node) {
        return dx_make_null();
    }
    switch (node->type) {
        case AST_PROGRAM: {
            DaxisValue result = dx_make_null();
            for (int i = 0; i < node->children_count; i++) {
                result = dx_interpret_node(node->children[i], scope);   /** Execute every single declaration */
            }
            return result;
        }

        case AST_LITERAL: {
            return dx_interpret_literal(node);
        }

        case AST_IDENTIFIER: {
            DaxisValue* val_ptr = dx_scope_get(scope, node->data.named.name);
            if (val_ptr) {
                return *val_ptr;
            }
            fprintf(stderr, "Runtime Error: Undefined variable '%s'\n", node->data.named.name);
            return dx_make_null();
        }

        case AST_VAR_DECL: {
            /** Evaluate the initial value (child[0]) */
            DaxisValue value = dx_make_null();
            if (node->children_count > 0) {
                value = dx_interpret_node(node->children[0], scope);
            }
            /**
             * Stores the variable in the global scope
             */
            dx_scope_set(scope, node->data.named.name, value);
            return dx_make_null();
        }
        
        case AST_BINARY_EXPR: {
            DaxisValue left = dx_interpret_node(node->children[0], scope);
            DaxisValue right = dx_interpret_node(node->children[1], scope);
            return dx_interpret_binary(node->data.operator_type, left, right);
        }
        default:
            fprintf(stderr, "Runtime Error: Unknown AST node type %d\n", node->type);
            return dx_make_null();
    }
}

/**
 * Init the execution
 */
int dx_interpret_program(ASTNode* program_ast) {
    GLOBAL_SCOPE = dx_scope_new(NULL);  /** Create the global scope */
    dx_interpret_node(program_ast, GLOBAL_SCOPE);
    /** NOTE: The cleanup has to be created here after the return */
    return 0;
}
