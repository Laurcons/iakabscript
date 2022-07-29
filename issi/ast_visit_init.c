#include "ast.h"
#include "array.h"
#include "symtable.h"
#include "util.h"
#include <stdio.h>

static void _visitAst(ast_node);

int initialVisitAst() {
    _visitAst(rootNode);
    return 0;
}

static void _visitBlock(ast_node n) {
    array arr = n->payload;
    for (int i = 0; i < arr->len; i++) {
        _visitAst(arr->stuff[i]);
    }
}

static void _visitFunctionDef(ast_node n) {
    printf("Visiting FunctionDef\n");
    ast_functiondef fdef = n->payload;
    symtableDeclareFunction(fdef->identifier, fdef);
    printf("declared function %s with %d parameters\n",
        fdef->identifier,
        fdef->formalParams->len
    );
}

static void _visitAst(ast_node n) {
    switch (n->type) {
        case AST_EMPTY: /* nothing */; break;
        case AST_BLOCK: _visitBlock(n); break;
        case AST_ASSIGNMENT: break;
        case AST_DECLARATION: break;
        case AST_FUNCTIONDEF: _visitFunctionDef(n); break;
        case AST_FUNCTIONCALL: break;
        default: stopHard("AST node type %d not handled", n->type); break;
    }
}