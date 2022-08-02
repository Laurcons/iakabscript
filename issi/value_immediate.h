#ifndef VALUE_IMMEDIATE_H_INCLUDED
#define VALUE_IMMEDIATE_H_INCLUDED

enum value_kind_t {
    VAL_STRING, // char*
    VAL_NUMBER, // double
    VAL_NUI, // NULL
};

typedef struct value_immediate_t {
    enum value_kind_t type;
    void* payload;
} *value_immediate, value_immediate_t;

value_immediate vimm_createString(char* str);
value_immediate vimm_createNumber(double d);
value_immediate vimm_createNui();

value_immediate vimm_copy(value_immediate other);
int vimm_equals(value_immediate first, value_immediate second);
void vimm_dbgprint(value_immediate vimm);

void vimm_free(value_immediate vimm);

#endif 