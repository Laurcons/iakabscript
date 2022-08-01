#include "ast.h"
#include "array.h"
#include "symtable.h"
#include "util.h"
#include "ast_create.h"
#include "value_immediate.h"
#include "eval_expr.h"
#include "builtins.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void _visitAst(ast_node);

int visitAst() {
    _visitAst(rootNode);
    return 0;
}

void _visitBlock(ast_node n) {
    dbgprintf("Visiting Block\n");
    array arr = n->payload;
    for (int i = 0; i < arr->len; i++)
        _visitAst(arr->stuff[i]);
}

void _visitAssignment(ast_node n) {
    dbgprintf("Visiting Assignment\n");
    ast_assignment asn = n->payload;
    symbol sym = symtableGetVar(asn->identifier);
    vimm_free(sym->payload);
    sym->payload = evalExpr(asn->expr);
}

void _visitDeclaration(ast_node n) {
    dbgprintf("Visiting Declaration\n");
    ast_declaration decl = n->payload;
    symtableDeclareVar(decl->identifier);
    symbol sym = symtableGetVar(decl->identifier);
    vimm_free(sym->payload); // it is initialized with NUI in declareVar
    sym->payload = evalExpr(decl->expr);
}

void _visitFunctionCall(ast_node n) {
    dbgprintf("Visiting FunctionCall\n");
    ast_functioncall fcall = n->payload;
    array vimms = arr_create();
    // evaluate all exprs to immediates
    for (int i = 0; i < fcall->actualParams->len; i++) {
        arr_add(
            vimms,
            evalExpr(fcall->actualParams->stuff[i])
        );
    }
    if (symtableIsBuiltin(fcall->identifier)) {
        invokeBuiltin(fcall->identifier, vimms);
        for (int i = 0; i < vimms->len; i++) {
            value_immediate vimm = vimms->stuff[i];
            vimm_free(vimm);
        }
    } else {
        symbol_function symf = symtableGetFunction(fcall->identifier);
        // with no care in the world, just execute the block gg
        _visitAst(symf->block);
    }
}

void _visitAst(ast_node n) {
    switch (n->type) {
        case AST_EMPTY: /* nothing */; break;
        case AST_BLOCK: _visitBlock(n); break;
        case AST_ASSIGNMENT: _visitAssignment(n); break;
        case AST_DECLARATION: _visitDeclaration(n); break;
        case AST_FUNCTIONDEF: break;
        case AST_FUNCTIONCALL: _visitFunctionCall(n); break;
        default: stopHard("AST node type %d not handled", n->type); break;
    }
}