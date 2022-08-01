#include "builtins.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct identifier_handler_pair_t {
    char* identifier;
    value_immediate (*runner)(array);
} identifier_handler_pair_t;

static value_immediate _zic(array params) {
    dbgprintf("ZIC CALL: ");
    for (int i = 0; i < params->len; i++) {
        value_immediate vimm = params->stuff[i];
        switch (vimm->type) {
            case VAL_NUI:
                printf("<nui>"); break;
            case VAL_STRING:
                printf("%s", (char*)vimm->payload); break;
            case VAL_NUMBER: {
                double d = *((double*)vimm->payload);
                if (fabs(d - (int)d) < 0.00001)
                    printf("%d", (int)d);
                else printf("%f", d);
                break;
            }
        }
        printf(" ");
    }
    printf("\n");
    return vimm_createNui();
}

identifier_handler_pair_t builtinList[] = {
    { "zic", _zic },
    { "0", NULL }
};

// array of value_immediates
value_immediate invokeBuiltin(char* identifier, array params) {
    dbgprintf("Builtin call to %s with %d params\n", identifier, params->len);
    int i = 0;
    while (builtinList[i].identifier[0] != '0') {
        if (strcmp(identifier, builtinList[i].identifier) == 0) {
            return builtinList[i].runner(params);
        }
        i++;
    }
    stopHard("Unknown builtin function %s. How did you even get this error?\n", identifier);
    return NULL;
}