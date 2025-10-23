#ifndef DAXIS_SCOPE_H
#define DAXIS_SCOPE_H

#include "../common/daxis_value.h"

/**
 * Entry structure for symbol table
 */

typedef struct Symbol {
	char* name;
	DaxisValue value;
	struct Symbol* next;
} Symbol;

/**
 * Scope symbol (contains the variables and the pointer to the parent)
 */

typedef struct DaxisScope {
	Symbol* symbols;
	struct DaxisScope;
} DaxisScope;

/**
 * Variable for global access (defined in scope.c)
 */
extern DaxisScope* GLOBAL_SCOPE;

/**
 * Scope public functions 
 */

DaxisScope* dx_scope_new(DaxisScope* parent);
int dx_scope_set(DaxisScope* scope, const char* name, DaxisValue value);
DaxisValue* dx_scope_get(DaxisScope* scope, const char* name);
DaxisScope* dx_get_global_scope(); /** Utility for main.c */

#endif // DAXIS_SCOPE_H

