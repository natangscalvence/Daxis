#include "daxis_lexer.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/**
 * Auxiliar read functions.
 */

static int is_at_end(DaxisLexer* lexer) {
	return *lexer->current == '\0';
}

static DaxisToken make_token(DaxisLexer* lexer, DaxisTokenType type) {
	DaxisToken token;
	token.type = type;
	token.start = lexer->source;
	token.length = (int)(lexer->current - lexer->source);
	token.line = lexer->line;
	return token;
}

static DaxisToken error_token(DaxisLexer* lexer, const char* message) {
    DaxisToken token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = lexer->line;
    return token;
}

static char advance(DaxisLexer* lexer) {
	lexer->current++;
	return lexer->current[-1];
}

static char peek(DaxisLexer* lexer) {
	return *lexer->current;
}

static char peek_next(DaxisLexer* lexer) {
	if (is_at_end(lexer)) {
		return '\0';
	}
	return lexer->current[1];
}

static int match(DaxisLexer* lexer, char expected) {
	if (is_at_end(lexer) || *lexer->current != expected) {
		return 0;
	}
	lexer->current++;
	return 1;
}

/**
 * Jump functions (Whitespaces and comments.)
 */

static void skip_whitespace(DaxisLexer* lexer) {
	for (;;) {
		char c = peek(lexer);
		switch (c) {
			case ' ':
			case '\r':
			case '\t':
				advance(lexer);
				break;
			case '\n':
				lexer->line++;
				advance(lexer);
				break;
			case '/':
				if (peek_next(lexer) == '/') {
					while (peek(lexer) != '\n' && !is_at_end(lexer)) {
						advance(lexer);
					}
				} else {
					return;	// Is not a comment, but a operator.
				}
				break;
			default:
				return;
		}
	}
}

/**
 * Recognition functions of complex tokens.
 */

static DaxisTokenType check_keyword(DaxisLexer* lexer, int start, int length,
	const char* rest, DaxisTokenType type) {


	if (lexer->current - lexer->source == start + length && 
		memcmp(lexer->source + start, rest, length) == 0){

		return type;
	}
	return TOKEN_IDENTIFIER;
}

/**
 * Word recognition
 */

static DaxisTokenType identifier_type(DaxisLexer* lexer) {
    // The lexeme starts at lexer->source
    switch (lexer->source[0]) {
        case 'c': {
            if (lexer->current - lexer->source > 1) {
                switch (lexer->source[1]) {
                    case 'l': return check_keyword(lexer, 2, 3, "ass", TOKEN_CLASS);
                    case 'h': return check_keyword(lexer, 2, 5, "annel", TOKEN_CHANNEL);
                }
            }
            break;
        }
        case 'e': return check_keyword(lexer, 1, 3, "lse", TOKEN_ELSE);
        case 'f': return check_keyword(lexer, 1, 7, "unction", TOKEN_FUNCTION);
        case 'g': return check_keyword(lexer, 1, 1, "o", TOKEN_GO);
        case 'i': return check_keyword(lexer, 1, 1, "f", TOKEN_IF);
        case 'l': return check_keyword(lexer, 1, 2, "et", TOKEN_LET);
        case 'n': {
            if (lexer->current - lexer->source > 1) {
                switch (lexer->source[1]) {
                    case 'o': return check_keyword(lexer, 2, 1, "t", TOKEN_NOT);
                    case 'u': return check_keyword(lexer, 2, 2, "ll", TOKEN_NULL);
                }
            }
            break;
        }
        case 'o': return check_keyword(lexer, 1, 1, "r", TOKEN_OR);
        case 'r': return check_keyword(lexer, 1, 5, "eturn", TOKEN_RETURN);
        case 't': return check_keyword(lexer, 1, 3, "his", TOKEN_THIS);
        case 'v': return check_keyword(lexer, 1, 2, "ar", TOKEN_VAR);
        case 'w': return check_keyword(lexer, 1, 4, "hile", TOKEN_WHILE);
        case 'a': return check_keyword(lexer, 1, 2, "nd", TOKEN_AND);
    }

    return TOKEN_IDENTIFIER;
}


static DaxisToken identifier(DaxisLexer* lexer) {
	while (isalnum(peek(lexer)) || peek(lexer) == '_') {
		advance(lexer);
	}
	return make_token(lexer, identifier_type(lexer));
}

static DaxisToken number(DaxisLexer* lexer) { // <-- Mude de DaxisTokenType para DaxisToken
    while (isdigit(peek(lexer))) advance(lexer);

    // Checa por ponto flutuante
    if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
        advance(lexer); // Consome o '.'
        while (isdigit(peek(lexer))) advance(lexer);
        return make_token(lexer, TOKEN_FLOAT); // Agora retorna DaxisToken
    }

    return make_token(lexer, TOKEN_INT); // Agora retorna DaxisToken
}

static DaxisToken string(DaxisLexer* lexer) {
	/**
	 * The string started after the quotes, so
	 * the lexer is not the first character.
	 */
	while (peek(lexer) != '"' && !is_at_end(lexer)) {
		if (peek(lexer) == '\n') {
			lexer->line++;
		}
		advance(lexer);
	}
	if (is_at_end(lexer)) {
		return error_token(lexer, "String not terminated.");
	}
	advance(lexer);

	/**
	 * The string tokens does not include the delimiter quotes.
	 */
	DaxisToken token = make_token(lexer, TOKEN_STRING);
	token.start++;
	token.length -= 2;
	return token;
}

/**
 * Public functions
 */

void dx_lexer_init(DaxisLexer* lexer, const char* source) {
	lexer->source = source;
	lexer->current = source;
	lexer->line = 1;
}

DaxisToken dx_lexer_next_token(DaxisLexer* lexer) {
	skip_whitespace(lexer);

	/**
	 * The begin of token (start) its always the same current
	 * position (current) before advance.
	 */
	lexer->source = lexer->current;
	if (is_at_end(lexer)) {
		return make_token(lexer, TOKEN_EOF);
	}
	char c = advance(lexer);

	if (isalpha(c) || c == '_') {
		return identifier(lexer);
	}

    if (isdigit(c)) {
    	return number(lexer);
    } 
    
    switch (c) {
        // Tokens de um único caractere
        case '(': return make_token(lexer, TOKEN_LPAREN);
        case ')': return make_token(lexer, TOKEN_RPAREN);
        case '{': return make_token(lexer, TOKEN_LBRACE);
        case '}': return make_token(lexer, TOKEN_RBRACE);
        case '[': return make_token(lexer, TOKEN_LBRACKET);
        case ']': return make_token(lexer, TOKEN_RBRACKET);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case '.': return make_token(lexer, TOKEN_DOT);
        case ':': return make_token(lexer, TOKEN_COLON);
        case '+': return make_token(lexer, TOKEN_PLUS);
        case '-': return match(lexer, '>') ? make_token(lexer, TOKEN_ARROW) : make_token(lexer, TOKEN_MINUS);
        case '*': return make_token(lexer, TOKEN_STAR);
        case '/': return make_token(lexer, TOKEN_SLASH);

        // Tokens de um ou dois caracteres (Comparações e Atribuição)
        case '=': return make_token(lexer, match(lexer, '=') ? TOKEN_EQ_EQ : TOKEN_EQUAL);
        case '!': return make_token(lexer, match(lexer, '=') ? TOKEN_NOT_EQ : TOKEN_ERROR); // '!' sozinho não é permitido
        case '<': return make_token(lexer, match(lexer, '=') ? TOKEN_LE : TOKEN_LT);
        case '>': return make_token(lexer, match(lexer, '=') ? TOKEN_GE : TOKEN_GT);
        
        // Literais
        case '"': return string(lexer);
    }

    return error_token(lexer, "Unexpected character.");
}