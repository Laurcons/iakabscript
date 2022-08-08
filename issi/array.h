#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

typedef struct {
    int len;
    int cap;
    void** stuff;
    void (*free_method)(void*);
}* array, array_t;

void arr_nofree(void*);
void arr_simplefree(void*);

array arr_create(void (*free_method)(void*));
void arr_free(array);
void arr_add(array, void*);

int arr_len(array);
void* arr_get(array, int);

#endif