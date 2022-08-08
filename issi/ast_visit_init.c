#include "ast.h"
#include "array.h"
#include "symtable.h"
#include "util.h"
#include <stdio.h>

static void _visitAst(ast_node);

static int _isWithinFunction = 0;

int initialVisitAst() {
    _visitAst(rootNode);
    return 0;
}

static void _visitBlock(ast_node n) {
    ast_block block = n->payload;
    array arr = block->statements;
    for (int i = 0; i < arr_len(arr); i++) {
        _visitAst(arr_get(arr, i));
    }
}

static void _visitFunctionDef(ast_node n) {
    dbgprintf("Visiting FunctionDef\n");
    if (_isWithinFunction) {
        parsingStop("Nested functions are not supported in IakabScript");
    }
    ast_functiondef fdef = n->payload;
    symt_declareFunction(fdef->identifier, fdef);
    dbgprintf("declared function %s with %d parameters\n",
        fdef->identifier,
        arr_len(fdef->formalParams)
    );
    _isWithinFunction = 1;
    _visitAst(fdef->block);
    _isWithinFunction = 0;
}

static void _visitFunctionReturn(ast_node n) {
    dbgprintf("Visiting FunctionReturn\n");
    if (!_isWithinFunction) {
        parsingStop("iesi statement cannot appear in the global scope");
    }
}

static void _visitAst(ast_node n) {
    switch (n->type) {
        case AST_EMPTY: /* nothing */; break;
        case AST_BLOCK: _visitBlock(n); break;
        case AST_ASSIGNMENT: break;
        case AST_DECLARATION: break;
        case AST_FUNCTIONDEF: _visitFunctionDef(n); break;
        case AST_FUNCTIONCALL: break;
        case AST_FUNCTIONRETURN: _visitFunctionReturn(n); break;
        default: parsingStop("AST node type %d not handled", n->type); break;
    }
}