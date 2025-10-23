#ifndef DAXIS_VALUE_H
#define DAXIS_VALUE_H

/**
 * Token types
 */
#include "../frontend/lexer/daxis_lexer.h"


/**
 * Daxis value types.
 */

typedef enum {
	VAL_INT,
	VAL_FLOAT,
	VAL_BOOL,
	VAL_STRING,
	VAL_FUNCTION,
	VAL_OBJECT,
	VAL_NULL,
	VAL_CHANNEL
} DaxisValueType;

/**
 * Universal value structure.
 */

typedef struct DaxisValue {
	DaxisValueType type;
	union {
		long long i;		/** Integers */
		double f;			/** Float */
		int b;				/** Booleans (0 or 1) */
		char* s;			/** String */
		void* ptr;			/** Generic pointer (for functions, objects, etc...) */
	} data;
} DaxisValue;

/**
 * Public functions 
 */

DaxisValue dx_make_int(long long val);
DaxisValue dx_make_float(double val);
DaxisValue dx_make_null();
void dx_value_print(DaxisValue val);

#endif // DAXIS_VALUE_H
