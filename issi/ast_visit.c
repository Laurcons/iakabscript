#include "ast.h"
#include "array.h"
#include "symtable.h"
#include "util.h"
#include "ast_create.h"
#include "value_immediate.h"
#include "eval_expr.h"
#include "builtins.h"
#include "call_stack.h"
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
    ast_block block = n->payload;
    if (block->scoped == BLOCK_SCOPED)
        stack_createFrame();
    array arr = block->statements;
    for (int i = 0; i < arr->len; i++)
        _visitAst(arr->stuff[i]);
    if (block->scoped == BLOCK_SCOPED)
        stack_popFrame();
}

void _visitAssignment(ast_node n) {
    dbgprintf("Visiting Assignment\n");
    ast_assignment asn = n->payload;
    value_immediate result = evalExpr(asn->expr);
    framed_variable fvar = stack_lookup(asn->identifier);
    if (fvar != NULL) {
        vimm_free(fvar->value);
        fvar->value = vimm_copy(result);
    } else {
        symbol sym = symtableGetVar(asn->identifier);
        vimm_free(sym->payload);
        sym->payload = vimm_copy(result);
    }
    vimm_free(result);
}

void _visitDeclaration(ast_node n) {
    dbgprintf("Visiting Declaration\n");
    ast_declaration decl = n->payload;
    value_immediate result = evalExpr(decl->expr);
    // declare in stack or globally
    stack_frame frame = stack_getCurrentFrame();
    if (frame != NULL) {
        arr_add(frame->variables,
            framedvar_create(
                decl->identifier,
                result
            )
        );
    } else {
        symtableDeclareVar(decl->identifier);
        symbol sym = symtableGetVar(decl->identifier);
        vimm_free(sym->payload); // it is initialized with NUI in declareVar
        sym->payload = vimm_copy(result);
    }
    vimm_free(result);
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
        stack_createFrame();
        stack_frame frame = stack_getCurrentFrame();
        // push the variables on the stack
        for (int i = 0; i < vimms->len; i++) {
            value_immediate vimm = vimms->stuff[i];
            arr_add(
                frame->variables,
                framedvar_create(
                    symf->params->stuff[i], // the identifier
                    vimm // the value
                )
            );
        }
        _visitAst(symf->block);
        stack_popFrame();
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