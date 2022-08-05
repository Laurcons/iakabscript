#include "array.h"
#include <stdlib.h>

array arr_create() {
    array p = malloc(sizeof(array_t));
    p->cap = 3;
    p->len = 0;
    p->stuff = malloc(sizeof(void*) * p->cap);
    return p;
}

void arr_free(array a) {
    free(a->stuff);
    free(a);
}

void arr_add(array a, void* elem) {
    if (a->len == a->cap) {
        // resize
        a->cap *= 2;
        a->stuff = realloc(a->stuff, sizeof(void*) * a->cap);
    }
    a->stuff[a->len++] = elem;
}