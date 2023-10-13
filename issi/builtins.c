#include "builtins.h"
#include <stdlib.h>

static value_immediate _zic(array);
static value_immediate _zi(array);
static value_immediate _fanumar(array);
identifier_handler_pair_t builtinList[] = {
    {"zic", _zic},
    {"zi", _zi},
    {"fanumar", _fanumar},
    {"0", NULL},
};

// array of value_immediates
value_immediate builtin_invoke(char *identifier, array params) {
	dbgprintf(
	    "Builtin call to %s with %d params\n", identifier, arr_len(params));
	int i = 0;
	while (builtinList[i].identifier[0] != '0') {
		if (strcmp(identifier, builtinList[i].identifier) == 0) {
			return builtinList[i].runner(params);
		}
		i++;
	}
	// this error generally shouldn't be reached because before hitting this
	// code,
	//  ISSI will look in the symbol table and will throw an error there (since
	//  the builtin does not appear there)
	runtimeStop(
	    "Unknown builtin function %s. How did you even get this error?\n",
	    identifier);
	return NULL;
}