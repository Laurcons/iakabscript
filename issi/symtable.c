#include "symtable.h"
#include "util.h"
#include "array.h"
#include "value_immediate.h"
#include "builtins.h"
#include <string.h>
#include <stdlib.h>

array symtable = NULL;

static void _declareBuiltin(char* identifier) {
    dbgprintf("Registering builtin %s\n", identifier);
    symbol sym = malloc(sizeof(symbol_t));
    sym->identifier = strdup(identifier);
    sym->type = SYM_FUNCTION_BUILTIN;
    sym->payload = NULL;
    arr_add(symtable, sym);
}

void symt_init() {
    symtable = arr_create(&arr_nofree);
    // create global builtin function symbols
    int i = 0;
    while (builtinList[i].runner != NULL) {
        _declareBuiltin(builtinList[i].identifier);
        i++;
    }
}

void symt_declareVar(char* identifier) {
    // try find variable
    for (int i = 0; i < arr_len(symtable); i++) {
        symbol sym = arr_get(symtable, i);
        if (strcmp(sym->identifier, identifier) == 0)
            runtimeStop("Variable %s already declared", sym->identifier);
    }
    // add a new one
    symbol sym = malloc(sizeof(symbol_t));
    sym->identifier = strdup(identifier);
    sym->type = SYM_VARIABLE;
    sym->payload = vimm_createNui();
    arr_add(symtable, sym);
}

void symt_declareFunction(char* identifier, ast_functiondef fdef) {
    symbol sym = malloc(sizeof(symbol_t));
    sym->identifier = identifier;
    sym->type = SYM_FUNCTION;
    symbol_function symf = malloc(sizeof(symbol_function_t));
    symf->block = fdef->block;
    symf->params = fdef->formalParams; // assign by reference
    sym->payload = symf;
    arr_add(symtable, sym);
}

symbol symt_getVar(char* identifier) {
    // try find variable
    for (int i = 0; i < arr_len(symtable); i++) {
        symbol sym = arr_get(symtable, i);
        if (sym->type == SYM_VARIABLE && strcmp(sym->identifier, identifier) == 0)
            return sym;
    }
    runtimeStop("Variable %s not found when getting", identifier);
    return NULL; // will not be called
}

int symt_isBuiltin(char* identifier) {
    // search for it
    for (int i = 0; i < arr_len(symtable); i++) {
        symbol sym = arr_get(symtable, i);
        if (sym->type == SYM_FUNCTION_BUILTIN && strcmp(sym->identifier, identifier) == 0)
            return 1;
    }
    return 0;
}

symbol_function symt_getFunction(char* identifier) {
    // search for it
    for (int i = 0; i < arr_len(symtable); i++) {
        symbol sym = arr_get(symtable, i);
        if (sym->type == SYM_FUNCTION && strcmp(sym->identifier, identifier) == 0)
            return sym->payload;
    }
    runtimeStop("Function %s not found", identifier);
    return NULL; // will not be called
}
