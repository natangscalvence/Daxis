#include "daxis_ast.h"
#include <stdio.h>
#include <string.h>

// Helper to map AST type to string (for debug output)
static const char* ast_type_to_string(ASTNodeType type) {
	switch (type) {
		case AST_PROGRAM: return "PROGRAM";
		case AST_VAR_DECL: return "VAR_DECL";
		case AST_FUNCTION_DECL: return "FUNCTION_DECL";
		case AST_IF_STMT: return "IF_STMT";
        case AST_WHILE_STMT: return "WHILE_STMT";
        case AST_RETURN_STMT: return "RETURN_STMT";
        case AST_CALL_EXPR: return "CALL_EXPR";
        case AST_BINARY_EXPR: return "BINARY_EXPR";
        case AST_LITERAL: return "LITERAL";
        case AST_IDENTIFIER: return "IDENTIFIER";
	}
	return "UNKNWON_AST_TYPE";
}

// Helper to map Token operator type to string (for Binary Expression).
extern const char* token_type_to_string(DaxisTokenType type);


/**
 * Main function for recursive printing.
 */

static void print_node_recursive(ASTNode* node, int indent) {
	if (!node) {
		return;
	}
	/**
	 * 1 - Print identing
	 */
	for (int i = 0; i < indent; i++) {
		printf("  ");
	}

	/**
	 * 2 - Print the node type
	 */
	printf("- %s", ast_type_to_string(node->type));

	/**
	 * 3 - Print specific node data
	 */
	switch (node->type) {
		case AST_LITERAL: {
			printf("[Type: %s, Value: %.*s]\n",
				token_type_to_string(node->data.literal.type),
				(int)node->data.literal.length,
				node->data.literal.start);
			break;
		}
		case AST_IDENTIFIER:
		case AST_VAR_DECL: {
			printf(" [Name: %s]\n", node->data.named.name);
			break;
		} 
		case AST_BINARY_EXPR: {
			printf(" [Op: %s]\n", token_type_to_string(node->data.operator_type));
			break;
		}
		case AST_PROGRAM:
		default: {
			printf("\n");
			break;
		}
	}
	/**
	 * 4 - Print childs (recursion)
	 */
	for (int i = 0; i < node->children_count; i++) {
		print_node_recursive(node->children[i], indent + 1);
	}
}


/**
 * Public wrapper function.
 */

void dx_ast_print(ASTNode* node) {
	printf("\n --- Daxis Abstract Syntax Tree (dAST) ---");
	print_node_recursive(node, 0);
	printf("-----------------------------------------\n");
}