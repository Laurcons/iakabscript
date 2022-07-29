#include "symtable.h"
#include "util.h"
#include "array.h"
#include <string.h>
#include <stdlib.h>

void symtableInit() {
    symtable = arr_create();
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
    sym->type = SYM_VARIABLE_NUI;
    arr_add(symtable, sym);
}

void symtableDeclareFunction(char* identifier, ast_node fpointer) {
    // TODO:
}

symbol symtableGetVar(char* identifier) {
    // try find variable
    for (int i = 0; i < symtable->len; i++) {
        symbol sym = symtable->stuff[i];
        if (strcmp(sym->identifier, identifier) == 0)
            return sym;
    }
    stopHard("Variable not found when getting");
    return NULL; // will not be called
}