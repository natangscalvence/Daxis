#include "daxis_parser.h"
#include "daxis_ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Forward Declarations
static ASTNode* declaration(DaxisParser* parser);
static ASTNode* statement(DaxisParser* parser); 
static ASTNode* expression(DaxisParser* parser);
static ASTNode* primary(DaxisParser* parser);
static ASTNode* term(DaxisParser* parser);

/**
 * Avança para o próximo token.
 */
static DaxisToken advance(DaxisParser* parser) {
    parser->previous_token = parser->current_token;
    parser->current_token = dx_lexer_next_token(parser->lexer);

    if (parser->current_token.type == TOKEN_ERROR) {
        parser->had_error = 1;
        fprintf(stderr, "[ParserError] Line %d: Lexer error - %.*s\n",
            parser->current_token.line,
            (int)parser->current_token.length,
            parser->current_token.start);
    }
    return parser->previous_token;
}

/**
 * Consome um token esperado.
 */
static DaxisToken consume(DaxisParser* parser, DaxisTokenType type, const char* message) {
    if (parser->current_token.type == type) {
        return advance(parser);
    }

    parser->had_error = 1;
    fprintf(stderr, "[ParserError] Line %d: Expected %s but found '%.*s'\n",
        parser->current_token.line, message,
        (int)parser->current_token.length, parser->current_token.start);

    return parser->previous_token;
}

static int check(DaxisParser* parser, DaxisTokenType type) {
    return parser->current_token.type == type;
}

static int match(DaxisParser* parser, DaxisTokenType type) {
    if (!check(parser, type)) return 0;
    advance(parser);
    return 1;
}

/* -------- EXPRESSÕES -------- */

static ASTNode* expression(DaxisParser* parser);

static ASTNode* primary(DaxisParser* parser) {
    if (match(parser, TOKEN_INT) || match(parser, TOKEN_FLOAT) || match(parser, TOKEN_STRING)) {
        ASTNode* node = dx_ast_new(AST_LITERAL);
        node->data.literal.type = parser->previous_token.type;
        node->data.literal.start = parser->previous_token.start;
        node->data.literal.length = parser->previous_token.length; // <-- corrigido

        return node;
    }

    if (match(parser, TOKEN_IDENTIFIER)) {
        DaxisToken name_token = parser->previous_token;
        ASTNode* node = dx_ast_new(AST_IDENTIFIER);

        char* name_copy = (char*)malloc(name_token.length + 1);
        strncpy(name_copy, name_token.start, name_token.length);
        name_copy[name_token.length] = '\0';

        /**
         * Store the safe copy.
         */
        node->data.named.name = name_copy;
        return node;
    }

    if (match(parser, TOKEN_LPAREN)) {
        ASTNode* expr = expression(parser);
        consume(parser, TOKEN_RPAREN, "Expected ')' after expression.");
        return expr;
    }

    parser->had_error = 1;
    fprintf(stderr, "[ParserError] Line %d: Unexpected token '%.*s' in expression.\n",
        parser->current_token.line,
        (int)parser->current_token.length,
        parser->current_token.start);

    return NULL;
}

static ASTNode* term(DaxisParser* parser) {
    ASTNode* node = primary(parser);

    while (match(parser, TOKEN_PLUS) || match(parser, TOKEN_MINUS)) {
        DaxisToken operator_token = parser->previous_token;
        ASTNode* right = primary(parser);

        ASTNode* new_node = dx_ast_new(AST_BINARY_EXPR);
        new_node->data.operator_type = operator_token.type;
        dx_ast_add_child(new_node, node);
        dx_ast_add_child(new_node, right);
        node = new_node;
    }
    return node;
}

static ASTNode* expression(DaxisParser* parser) {
    return term(parser);
}

/* -------- DECLARAÇÕES -------- */

static ASTNode* var_declaration(DaxisParser* parser) {
    DaxisToken keyword = parser->previous_token; // TOKEN_VAR ou TOKEN_LET
    ASTNode* decl_node = NULL; // <-- CORRIGIDO: Declarado aqui
    ASTNode* initializer = NULL;
    char* name_copy = NULL;

    // 2. Consome o nome
    DaxisToken name_token = consume(parser, TOKEN_IDENTIFIER, "Expected variable name.");

    // 3. Implementação da cópia segura do nome (CORREÇÃO DO BUG DE IMPRESSÃO!)
    name_copy = (char*)malloc(name_token.length + 1);
    strncpy(name_copy, name_token.start, name_token.length);
    name_copy[name_token.length] = '\0'; // Terminador nulo

    // 4. Tipo opcional: ( : type ) - Lógica de tipagem removida para simplificar por enquanto
    if (match(parser, TOKEN_COLON)) {
        consume(parser, TOKEN_IDENTIFIER, "Expected type after ':'."); // Tipo (ex: 'int')
    }

    // 5. Inicializador opcional: ( = expression )
    if (match(parser, TOKEN_EQUAL)) {
        initializer = expression(parser);
    }
    
    // 6. Cria o nó de declaração e anexa os dados
    decl_node = dx_ast_new(AST_VAR_DECL); // <-- Usa a variável declarada
    decl_node->data.named.name = name_copy; // Armazena a cópia segura

    if (initializer) {
        dx_ast_add_child(decl_node, initializer);
    }
    
    return decl_node;
}

static ASTNode* declaration(DaxisParser* parser) {
    if (match(parser, TOKEN_VAR) || match(parser, TOKEN_LET)) {
        return var_declaration(parser);
    }
    
    // Se não for uma declaração, assume que é uma declaração de fluxo (statement)
    return statement(parser);
}

static ASTNode* statement(DaxisParser* parser) {
    return expression(parser);
}

/* -------- ENTRADA PRINCIPAL -------- */

ASTNode* dx_parser_parse(DaxisLexer* lexer) {
    DaxisParser parser;
    parser.lexer = lexer;
    parser.had_error = 0;
    parser.current_token = dx_lexer_next_token(lexer);

    ASTNode* program = dx_ast_new(AST_PROGRAM);

    while (!check(&parser, TOKEN_EOF) && !parser.had_error) {
        ASTNode* decl = declaration(&parser);
        if (decl) {
            dx_ast_add_child(program, decl);
        }
    }

    if (parser.had_error) {
        // TODO: dx_ast_free(program);
        return NULL;
    }

    return program;
}
