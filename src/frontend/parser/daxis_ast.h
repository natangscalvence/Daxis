#ifndef DAXIS_AST_H
#define DAXIS_AST_H

#include "../lexer/daxis_lexer.h"

/**
 * Types of nodes of AST 
 */

typedef enum {
	AST_PROGRAM,	/* The root node */
	AST_VAR_DECL, 	
	AST_FUNCTION_DECL,
	AST_IF_STMT,
	AST_WHILE_STMT,
	AST_RETURN_STMT,
	AST_CALL_EXPR,
	AST_BINARY_EXPR,
	AST_LITERAL,
	AST_IDENTIFIER,
} ASTNodeType;

/** 
 * Base structure of node
 */

typedef struct ASTNode {
	ASTNodeType type;

	/**
	 * child list (for blocks of code or arguments).
	 */
	struct ASTNode** children;
	int children_count;

	/**
	 * Special data from node
	 */
	union {
		struct {
			DaxisTokenType type;
			const char* start;
			size_t length;
		} literal;

		/**
		 * Used in AST_BINARY_EXPR
		 */
		DaxisTokenType operator_type;

		/**
		 * Used in AST_IDENTIFIER or AST_VAR_DECL
		 */
		struct {
			const char* name;
		} named;
	} data;
} ASTNode;

/**
 * Utility functions
 */
ASTNode* dx_ast_new(ASTNodeType type);
void dx_ast_free(ASTNode* node);

#endif // DAXIS_AST_H