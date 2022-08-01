#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include "ast.h"
#include "array.h"
#include <stdio.h>

typedef enum symbol_kind_t {
    SYM_FUNCTION, // symbol_function
    SYM_VARIABLE, // value_immediate
    SYM_FUNCTION_BUILTIN, // NULL (recognized by name)
} symbol_kind_t;

typedef struct symbol_t {
    char* identifier;
    symbol_kind_t type;
    void* payload;
} *symbol, symbol_t;

typedef struct symbol_function_t {
    ast_node block;
    int paramCount;
} *symbol_function, symbol_function_t;

void symtableInit();
void symtableDeclareVar(char* identifier);
void symtableDeclareFunction(char* identifier, ast_functiondef fdef);
symbol symtableGetVar(char* identifier);
int symtableIsBuiltin(char* identifier);
symbol_function symtableGetFunction(char* identifier);

array symtable;

#endif