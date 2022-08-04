#ifndef BUILTINS_H_INCLUDED
#define BUILTINS_H_INCLUDED

#include "array.h"
#include "value_immediate.h"

value_immediate builtin_invoke(char* identifier, array params);

#endif