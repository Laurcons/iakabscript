#include "standard_lib/standard_lib.h"
#include "util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int _isCharacterSet(char *target, char *set) {
	int count = strspn(target, set);
	return target[count] == '\0';
}

static value_immediate _fromNb(char *str) {
	int i = strlen(str) - 1;
	double result = 0;
	double power = 0;
	while (i > 0) {
		if (str[i] == 'n')
			result += pow(2, power);
		power++;
	}
	return vimm_createNumber(result);
}

static value_immediate _fromEz(char *str) {
	if (strlen(str) < 2) {
		return vimm_createNumber(1);
	}
	double result = 1;
	double factor = 10;
	int start = 1;
	if (str[1] == 'e') {
		factor = 0.1;
		start = 2;
	}
	while (str[start] != '\0') {
		result *= factor;
		start++;
	}
	return vimm_createNumber(result);
}

static value_immediate _fromGg(char *str) {
	return vimm_createNumber(strlen(str));
}

value_immediate std_fanumar(array params) {
	// TODO: support non-Human formatted numbers
	int doariakab = 0;
	if (params->len == 2) {
		value_immediate vimm = params->stuff[1];
		if (vimm->type == VAL_STRING &&
		    strcmp(vimm->payload, "doariakab") == 0) {
			doariakab = 1;
			dbgprintf("doariakab mode activated for fanumar call\n");
		}
	}
	value_immediate first = params->stuff[0];
	if (first->type != VAL_STRING)
		runtimeStop("Parameter to fanumar was not a string");
	char *str = first->payload;
	if (_isCharacterSet(str, "g"))
		return _fromGg(str);
	if (_isCharacterSet(str, "ez"))
		return _fromEz(str);
	if (_isCharacterSet(str, "nb"))
		return _fromNb(str);
	if (_isCharacterSet(str, "0123456789.")) {
		if (doariakab) {
			runtimeStop(
			    "Parameter to fanumar was not a Iakab-formatted string");
		}
		double result;
		sscanf((char *)first->payload, "%lf", &result);
		return vimm_createNumber(result);
	}
	return vimm_createNui();
}