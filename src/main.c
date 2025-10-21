#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inclusões
#include "frontend/lexer/daxis_lexer.h"
#include "frontend/parser/daxis_parser.h"
#include "frontend/parser/daxis_ast.h"

// --- Funções Utilitárias de Depuração (Mantidas para dx_ast_print) ---

// Mapeia o tipo de token para uma string legível (função externa, usada no dx_ast_print.c)
const char* token_type_to_string(DaxisTokenType type) {
    // ... [O código desta função é o mesmo que estava no main.c anterior] ...
    // É uma função longa, mas você deve tê-la no seu main.c para compilar
    switch (type) {
        case TOKEN_EOF: return "EOF"; case TOKEN_ERROR: return "ERROR";
        case TOKEN_VAR: return "VAR"; case TOKEN_LET: return "LET";
        case TOKEN_FUNCTION: return "FUNCTION"; case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE"; case TOKEN_GO: return "GO";
        case TOKEN_CHANNEL: return "CHANNEL"; case TOKEN_RETURN: return "RETURN";
        case TOKEN_WHILE: return "WHILE"; case TOKEN_CLASS: return "CLASS";
        case TOKEN_AND: return "AND"; case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT"; case TOKEN_NULL: return "NULL";
        case TOKEN_THIS: return "THIS";
        case TOKEN_IDENTIFIER: return "IDENTIFIER"; case TOKEN_INT: return "INT";
        case TOKEN_FLOAT: return "FLOAT"; case TOKEN_STRING: return "STRING";
        case TOKEN_EQUAL: return "="; case TOKEN_PLUS: return "+";
        case TOKEN_MINUS: return "-"; case TOKEN_STAR: return "*";
        case TOKEN_SLASH: return "/"; case TOKEN_GT: return ">";
        case TOKEN_LT: return "<"; case TOKEN_GE: return ">=";
        case TOKEN_LE: return "<="; case TOKEN_EQ_EQ: return "==";
        case TOKEN_NOT_EQ: return "!="; case TOKEN_LPAREN: return "(";
        case TOKEN_RPAREN: return ")"; case TOKEN_LBRACE: return "{";
        case TOKEN_RBRACE: return "}"; case TOKEN_LBRACKET: return "[";
        case TOKEN_RBRACKET: return "]"; case TOKEN_COMMA: return ",";
        case TOKEN_DOT: return "."; case TOKEN_COLON: return ":";
        case TOKEN_ARROW: return "->";
    }
    return "UNKNOWN";
}

// Declaração da função de impressão (implementada em dx_ast_print.c)
void dx_ast_print(ASTNode* node); 

// --- Código Daxis de Teste ---
const char* DAXIS_CODE = 
"// Teste do Parser: Declarações e Expressões\n"
"let a: int = 10 + 5\n"
"var b = a - 3.5\n"
"var c = (1 + 2) - b"; 


// --- Função Principal ---
int main(int argc, char** argv) {
    printf("--- Iniciando Daxis Parser Test ---\n");
    
    DaxisLexer lexer;
    dx_lexer_init(&lexer, DAXIS_CODE);
    
    // Inicia o Parser
    ASTNode* program_ast = dx_parser_parse(&lexer);
    
    if (program_ast) {
        // Imprime a AST para visualização
        dx_ast_print(program_ast);
        // dx_ast_free(program_ast); // Implementar esta função (liberação de memória) depois
    } else {
        printf("\n--- Daxis Parser Test failed probably by some lexical/semantic problem. ---\n");
    }
    
    printf("--- Daxis Parser Test Finished ---\n");
    return 0;
}