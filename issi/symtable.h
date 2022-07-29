#ifndef SYMTABLE_H_INCLUDED
#define SYMTABLE_H_INCLUDED

#include "ast.h"
#include "array.h"
#include <stdio.h>

typedef enum symbol_kind_t {
    SYM_FUNCTION, // ast_node
    SYM_VARIABLE_NUMBER, // double
    SYM_VARIABLE_STRING, // char*
    SYM_VARIABLE_NUI // NULL
} symbol_kind_t;

typedef struct symbol_t {
    char* identifier;
    symbol_kind_t type;
    void* payload;
} *symbol, symbol_t;

void symtableInit();
void symtableDeclareVar(char* identifier);
void symtableDeclareFunction(char* identifier, ast_node fpointer);
symbol symtableGetVar(char* identifier);
void symtableSetVar(char* identifier, void* value);

array symtable;

#endif