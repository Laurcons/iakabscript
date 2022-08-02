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

static value_immediate _zi(array params) {
    dbgprintf("ZI CALL:\n");
    if (params->len >= 1) {
        value_immediate vimm = params->stuff[0];
        if (vimm->type == VAL_STRING) {
            printf("%s", (char*)vimm->payload);
        }
    }
    // this is a limitation of the interpreter that might be fixed sometime
    //  in the future by using a dynamic array
    char buffer[200];
    scanf("%200s", buffer);
    return vimm_createString(buffer);
}

identifier_handler_pair_t builtinList[] = {
    { "zic", _zic },
    { "zi", _zi },
    { "0", NULL },
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