#include "daxis_value.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Function to create the int value 
 */

DaxisValue dx_make_int(long long val) {
	DaxisValue value;
	value.type = VAL_INT;
	value.data.i = val;
	return value;
}

/**
 * Function to create the float value
 */

DaxisValue dx_make_float(double val) {
	DaxisValue value;
	value.type = VAL_FLOAT;
	value.data.f = val;
	return value;
}

DaxisValue dx_make_null() {
	DaxisValue value;
	value.type = VAL_NULL;
	return value;
}

/**
 * Utility functions.
 */

/**
 * Function useful for debug
 */

void dx_value_print(DaxisValue val) {
	switch (val.type) {
		case VAL_INT:
			printf("%ld (INT)\n", val.data.i);
			break;
		case VAL_FLOAT:
			printf("%.2f (FLOAT)\n", val.data.f);
			break;
		case VAL_NULL:
			printf("null\n");
			break;
		default:
			printf("<<unknown type>>\n");
			break;
	}
}