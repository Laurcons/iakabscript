#include "standard_lib/standard_lib.h"

value_immediate std_zi(array params) {
	dbgprintf("ZI CALL:\n");
	if (params->len >= 1) {
		value_immediate vimm = params->stuff[0];
		if (vimm->type == VAL_STRING) {
			printf("%s", (char *)vimm->payload);
		}
	}
	// this is a limitation of the interpreter that might be fixed sometime
	//  in the future by using a dynamic array
	char buffer[200];
	scanf("%200s", buffer);
	return vimm_createString(buffer);
}