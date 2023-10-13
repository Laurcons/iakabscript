#include "standard_lib/standard_lib.h"
#include "util.h"
#include <stdio.h>

value_immediate std_zic(array params) {
	dbgprintf("ZIC CALL: ");
	for (int i = 0; i < params->len; i++) {
		value_immediate vimm = params->stuff[i];
		switch (vimm->type) {
		case VAL_NUI:
			printf("<nui>");
			break;
		case VAL_STRING:
			printf("%s", (char *)vimm->payload);
			break;
		case VAL_NUMBER: {
			double d = *((double *)vimm->payload);
			if (fabs(d - (int)d) < 0.00001)
				printf("%d", (int)d);
			else
				printf("%f", d);
			break;
		}
		}
		printf(" ");
	}
	printf("\n");
	return vimm_createNui();
}