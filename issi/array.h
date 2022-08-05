#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

typedef struct {
    int len;
    int cap;
    void** stuff;
}* array, array_t;

array arr_create();
void arr_free(array);
void arr_add(array, void*);

#endif