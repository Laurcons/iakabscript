#include "value_immediate.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

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