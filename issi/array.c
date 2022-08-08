#include "array.h"
#include "util.h"
#include <stdlib.h>

void arr_nofree(void* none) {}
void arr_simplefree(void* p) { free(p); }

array arr_create(void (*free_method)(void*)) {
    array p = malloc(sizeof(array_t));
    p->cap = 3;
    p->len = 0;
    p->stuff = malloc(sizeof(void*) * p->cap);
    p->free_method = free_method;
    return p;
}

void arr_free(array a) {
    for (int i = 0; i < arr_len(a); i++) {
        void* elem = arr_get(a, i);
        a->free_method(elem);
    }
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

int arr_len(array a) {
    return a->len;
}

void* arr_get(array a, int pos) {
    if (pos < 0)
        pos = a->len + pos;
    if (pos >= a->len)
        runtimeStop("ISSI fault: array index (%d) out of bounds (%d)", pos, a->len);
    return a->stuff[pos];
}
