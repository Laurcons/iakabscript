#include "symtable.h"
#include "util.h"
#include "array.h"
#include "value_immediate.h"
#include <string.h>
#include <stdlib.h>

array symtable = NULL;

static void _declareBuiltin(char* identifier) {
    symbol sym = malloc(sizeof(symbol_t));
    sym->identifier = strdup(identifier);
    sym->type = SYM_FUNCTION_BUILTIN;
    sym->payload = NULL;
    arr_add(symtable, sym);
}

void symtableInit() {
    symtable = arr_create();
    // create global builtin function symbols
    _declareBuiltin("zic");
    _declareBuiltin("zi");
}

void symtableDeclareVar(char* identifier) {
    // try find variable
    for (int i = 0; i < symtable->len; i++) {
        symbol sym = symtable->stuff[i];
        if (strcmp(sym->identifier, identifier) == 0)
            stopHard("Variable %s already declared", sym->identifier);
    }
    // add a new one
    symbol sym = malloc(sizeof(symbol_t));
    sym->identifier = strdup(identifier);
    sym->type = SYM_VARIABLE;
    sym->payload = vimm_createNui();
    arr_add(symtable, sym);
}

void symtableDeclareFunction(char* identifier, ast_functiondef fdef) {
    symbol sym = malloc(sizeof(symbol_t));
    sym->identifier = identifier;
    sym->type = SYM_FUNCTION;
    symbol_function symf = malloc(sizeof(symbol_function_t));
    symf->block = fdef->block;
    symf->params = fdef->formalParams; // assign by reference
    sym->payload = symf;
    arr_add(symtable, sym);
}

symbol symtableGetVar(char* identifier) {
    // try find variable
    for (int i = 0; i < symtable->len; i++) {
        symbol sym = symtable->stuff[i];
        if (sym->type == SYM_VARIABLE && strcmp(sym->identifier, identifier) == 0)
            return sym;
    }
    stopHard("Variable %s not found when getting", identifier);
    return NULL; // will not be called
}

int symtableIsBuiltin(char* identifier) {
    // search for it
    for (int i = 0; i < symtable->len; i++) {
        symbol sym = symtable->stuff[i];
        if (sym->type == SYM_FUNCTION_BUILTIN && strcmp(sym->identifier, identifier) == 0)
            return 1;
    }
    return 0;
}

symbol_function symtableGetFunction(char* identifier) {
    // search for it
    for (int i = 0; i < symtable->len; i++) {
        symbol sym = symtable->stuff[i];
        if (sym->type == SYM_FUNCTION && strcmp(sym->identifier, identifier) == 0)
            return sym->payload;
    }
    stopHard("Function %s not found", identifier);
    return NULL; // will not be called
}
