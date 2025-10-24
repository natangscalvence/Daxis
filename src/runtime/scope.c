#include "scope.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

DaxisScope* GLOBAL_SCOPE = NULL;

/**
 * Variable to access the global scope in main.c
 */

DaxisScope* dx_get_global_scope() {
	return GLOBAL_SCOPE;
}

/**
 * Utility to create a new symbol.
 */
static Symbol* symbol_new(const char* name, DaxisValue value) {
	Symbol* sym = (Symbol*)malloc(sizeof(Symbol));
	if (!sym) {
		perror("Failed to allocate Symbol");
		exit(EXIT_FAILURE);
	}
	sym->name = strdup(name);
	sym->value = value;
	sym->next = NULL;

	return sym;
}

/**
 * Create a new scope. 
 */
DaxisScope* dx_scope_new(DaxisScope* parent) {
	DaxisScope* scope = (DaxisScope*)malloc(sizeof(DaxisScope));
	if (!scope) {
		perror("Failed to allocate Scope");
		exit(EXIT_FAILURE);
	}
	scope->symbols = NULL;
	scope->parent = parent;

	return scope;
}

/**
 * Define or update a symbol in the current scope.
 */
int dx_scope_set(DaxisScope* scope, const char* name, DaxisValue value) {
	/**
	 * Search for a existent variable (for update)
	 */
	Symbol* current = scope->symbols;
	while (current) {
		if (strcmp(current->name, name) == 0) {
			current->value = value;
			return;
		}
		current = current->next;
	}

	/**
	 * If it not exists, add a new symbol.
	 */
	Symbol* new_sym = symbol_new(name, value);
	new_sym->next = scope->symbols;
	scope->symbols = new_sym;

	return 1;
}

/**
 * Search for a symbol value (first in the current scope, next in the parent node)
 */
DaxisValue* dx_scope_get(DaxisScope* scope, const char* name) {
    DaxisScope* current_scope = scope;
    while (current_scope) {
        Symbol* current_sym = current_scope->symbols;
        while (current_sym) {
            if (strcmp(current_sym->name, name) == 0) {
                return &current_sym->value;     /** Return the pointer for value */
            }
            current_sym = current_sym->next;
        }
        current_scope = current_scope->parent;  /** Move to the parent scope (closure) */
    }
    return NULL;        /** Not founded. */
}

/** TODO: Create the dx_scope_free for cleanup method. */