#include "standard_lib/standard_lib.h"
#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

value_immediate std_fanumar(array params) {
	// TODO: support non-Human formatted numbers
	// int doariakab = 0;
	if (params->len == 2) {
		value_immediate vimm = params->stuff[1];
		if (vimm->type == VAL_STRING &&
		    strcmp(vimm->payload, "doariakab") == 0) {
			// doariakab = 1;
			dbgprintf("doariakab mode activated for fanumar call\n");
		}
	}
	value_immediate first = params->stuff[0];
	if (first->type != VAL_STRING)
		stopHard("Parameter to fanumar was not a string");
	// check that this string does not contains characters that appear in number
	// literals
	if (strpbrk(first->payload, "nbgez") != NULL)
		stopHard(
		    "The current ISSI version's implementation of the 'fanumar' "
		    "builtin function does not support non-Human formatted numbers");
	double result;
	sscanf((char *)first->payload, "%lf", &result);
	return vimm_createNumber(result);
}