#ifndef DAXIS_LEXER_H
#define DAXIS_LEXER_H

#include <stddef.h> // For size_t

// --- Token Types ---
typedef enum {
    // End of File and Error
    TOKEN_EOF,
    TOKEN_ERROR,

    // Keywords (Language Structures)
    TOKEN_VAR,          // var (mutable variable declaration)
    TOKEN_LET,          // let (immutable variable declaration)
    TOKEN_FUNCTION,     // function (function definition)
    TOKEN_IF,           // if (conditional)
    TOKEN_ELSE,         // else (conditional)
    TOKEN_GO,           // go (parallelism/concurrency)
    TOKEN_CHANNEL,      // channel (concurrency communication)
    TOKEN_RETURN,       // return (exit function)
    TOKEN_WHILE,        // while (loop)
    TOKEN_CLASS,        // class (OO structure)
    TOKEN_AND,          // and (logical operator)
    TOKEN_OR,           // or (logical operator)
    TOKEN_NOT,          // not (logical operator)
    TOKEN_NULL,         // null value
    TOKEN_THIS,         // this (current object reference)
    
    // Literals (Values)
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_STRING,
    
    // Operators and Symbols
    TOKEN_EQUAL,        // = (assignment)
    TOKEN_PLUS,         // +
    TOKEN_MINUS,        // -
    TOKEN_STAR,         // *
    TOKEN_SLASH,        // /
    TOKEN_GT,           // >
    TOKEN_LT,           // <
    TOKEN_GE,           // >= (greater or equal)
    TOKEN_LE,           // <= (less or equal)
    TOKEN_EQ_EQ,        // == (equality comparison)
    TOKEN_NOT_EQ,       // != (inequality comparison)

    // Structure/Punctuation
    TOKEN_LPAREN,       // (
    TOKEN_RPAREN,       // )
    TOKEN_LBRACE,       // {
    TOKEN_RBRACE,       // }
    TOKEN_LBRACKET,     // [
    TOKEN_RBRACKET,     // ]
    TOKEN_COMMA,        // ,
    TOKEN_DOT,          // .
    TOKEN_COLON,        // :
    TOKEN_ARROW,        // -> (return type specification)
} DaxisTokenType;


// --- Token Structure ---
typedef struct {
    DaxisTokenType type;
    const char* start; // Pointer to the start of the lexeme in the source code
    size_t length;     // Length of the lexeme
    int line;          // Line number (for error messages)
} DaxisToken;

// --- Lexer Structure ---
typedef struct {
    const char* source; // Complete source code
    const char* current; // Current position in the source code
    int line;            // Current line
} DaxisLexer;

// --- Lexer Functions ---
void dx_lexer_init(DaxisLexer* lexer, const char* source);
DaxisToken dx_lexer_next_token(DaxisLexer* lexer);

#endif // DAXIS_LEXER_H