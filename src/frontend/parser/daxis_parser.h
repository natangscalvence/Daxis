#ifndef DAXIS_PARSER_H
#define DAXIS_PARSER_H

#include "../lexer/daxis_lexer.h"
#include "daxis_ast.h"


typedef struct {
    DaxisLexer* lexer;
    DaxisToken current_token;
    DaxisToken previous_token;
    int had_error;
} DaxisParser;

/**
 * Init the parser and return the root of AST (the entire program :D)
 */

ASTNode* dx_parser_parse(DaxisLexer* lexer);
void dx_ast_add_child(ASTNode* parent, ASTNode* child);

#endif // DAXIS_PARSER_H