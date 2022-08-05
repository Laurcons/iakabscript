#include "builtins.h"
#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

static value_immediate _zic(array);
static value_immediate _zi(array);
static value_immediate _fanumar(array);
identifier_handler_pair_t builtinList[] = {
    { "zic", _zic },
    { "zi", _zi },
    { "fanumar", _fanumar },
    { "0", NULL },
};

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

static value_immediate _fanumar(array params) {
    // TODO: support non-Human formatted numbers
    //int doariakab = 0;
    if (params->len == 2) {
        value_immediate vimm = params->stuff[1];
        if (vimm->type == VAL_STRING && strcmp(vimm->payload, "doariakab") == 0) {
            //doariakab = 1;
            dbgprintf("doariakab mode activated for fanumar call\n");
        }
    }
    value_immediate first = params->stuff[0];
    if (first->type != VAL_STRING)
        stopHard("Parameter to fanumar was not a string");
    // check that this string does not contains characters that appear in number literals
    if (strpbrk(first->payload, "nbgez") != NULL)
        stopHard("The current ISSI version's implementation of the 'fanumar' builtin function does not support non-Human formatted numbers");
    double result;
    sscanf((char*)first->payload, "%lf", &result);
    return vimm_createNumber(result);
}

// array of value_immediates
value_immediate builtin_invoke(char* identifier, array params) {
    dbgprintf("Builtin call to %s with %d params\n", identifier, params->len);
    int i = 0;
    while (builtinList[i].identifier[0] != '0') {
        if (strcmp(identifier, builtinList[i].identifier) == 0) {
            return builtinList[i].runner(params);
        }
        i++;
    }
    // this error generally shouldn't be reached because before hitting this code,
    //  ISSI will look in the symbol table and will throw an error there (since the
    //  builtin does not appear there)
    stopHard("Unknown builtin function %s. How did you even get this error?\n", identifier);
    return NULL;
}