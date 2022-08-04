#ifndef BUILTINS_H_INCLUDED
#define BUILTINS_H_INCLUDED

#include "array.h"
#include "value_immediate.h"

typedef struct identifier_handler_pair_t {
    char* identifier;
    value_immediate (*runner)(array);
} identifier_handler_pair_t;

// an array of our builtin list, terminated with runner=NULL
extern identifier_handler_pair_t builtinList[];

value_immediate builtin_invoke(char* identifier, array params);



#endif