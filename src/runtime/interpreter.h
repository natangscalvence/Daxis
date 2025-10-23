#ifndef DAXIS_INTERPRETER_H
#define DAXIS_INTERPRETER_H

#include "../frontend/parse/daxis_ast.h"
#include "scope.h"
#include "../common/daxis_value.h"

/**
 * Main execution function: traverses the ast recursively
 */

DaxisValue dx_interpret_node(ASTNode* node, DaxisScope* scope);

/**
 * Init the execution of the program from AST_PROGRAM node
 */
int dx_interpret_program(ASTNode* program_ast);

#endif // DAXIS_INTERPRETER_H
