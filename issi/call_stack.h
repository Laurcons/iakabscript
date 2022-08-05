#ifndef CALL_STACK_H_INCLUDED
#define CALL_STACK_H_INCLUDED

#include "value_immediate.h"
#include "array.h"
#include <stdlib.h>
#include <string.h>

typedef struct framed_variable_t {
    char* identifier;
    value_immediate value;
} *framed_variable, framed_variable_t;

typedef struct stack_frame_t {
    struct stack_frame_t* prev;
    int isFunctionFrame; // true if the stack corresponds to a function call, false if it's a normal scoped block
    array variables; // array of framed_variable
    value_immediate returnValue; // or NULL if not yet set
} *stack_frame, stack_frame_t;

stack_frame stack_getCurrentFrame();
void stack_createBlockFrame();
void stack_createFunctionFrame();
void stack_popFrame();
framed_variable stack_lookup(char* identifier);
void stack_setReturnValue(value_immediate vimm);

framed_variable framedvar_create(char* identifier, value_immediate value);
void framedvar_free();

#endif