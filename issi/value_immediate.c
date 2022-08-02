#include "value_immediate.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

value_immediate vimm_createString(char* str) {
    value_immediate vimm = malloc(sizeof(value_immediate_t));
    vimm->type = VAL_STRING;
    vimm->payload = strdup(str);
    return vimm;
}

value_immediate vimm_createNumber(double d) {
    value_immediate vimm = malloc(sizeof(value_immediate_t));
    vimm->type = VAL_NUMBER;
    double* p = malloc(sizeof(double));
    *p = d;
    vimm->payload = p;
    return vimm;
}
value_immediate vimm_createNui() {
    value_immediate vimm = malloc(sizeof(value_immediate_t));
    vimm->type = VAL_NUI;
    vimm->payload = NULL;
    return vimm;
}

value_immediate vimm_copy(value_immediate other) {
    switch (other->type) {
        case VAL_NUI: return vimm_createNui(); 
        case VAL_NUMBER: return vimm_createNumber(*((double*)other->payload));
        case VAL_STRING: return vimm_createString(strdup((char*)other->payload));
    }
    stopHard("Unknown value_kind_t %d\n", other->type);
    return NULL;
}

int vimm_equals(value_immediate first, value_immediate second) {
    if (first->type != second->type)
        return 0;
    switch (first->type) {
        case VAL_NUI: return 1; // nothing more to compare here
        case VAL_NUMBER: {
            double d1 = *((double*)first->payload);
            double d2 = *((double*)second->payload);
            return (fabs(d1 - d2) < 0.000001);
        };
        case VAL_STRING: {
            char* s1 = (char*)first->payload;
            char* s2 = (char*)second->payload;
            return strcmp(s1, s2) == 0;
        };
        default: {
            stopHard("Unknown value_kind_t %d\n", first->type);
            return 0;
        }
    }
}

void vimm_dbgprint(value_immediate vimm) {
    switch (vimm->type) {
        case VAL_NUI:
            dbgprintf("<nui>");
            break;
        case VAL_NUMBER: {
            double d = *((double*)vimm->payload);
            dbgprintf("%f", d);
            break;
        }
        case VAL_STRING:
            dbgprintf("%s", (char*)vimm->payload);
            break;
        default: {
            stopHard("Unknown value_kind_t %d\n", vimm->type);
        }
    }
}

void vimm_free(value_immediate vimm) {
    switch (vimm->type) {
        case VAL_STRING:
        case VAL_NUMBER:
            free(vimm->payload);
            break;
        case VAL_NUI: break;
    }
    free(vimm);
}