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

int visitAst(ast_node n) {
    _visitAst(n);
    return 0;
}

void _visitBlock(ast_node n) {
    dbgprintf("Visiting Block\n");
    ast_block block = n->payload;
    if (block->scoped == BLOCK_SCOPED)
        stack_createFrame();
    array arr = block->statements;
    stack_frame frame = stack_getCurrentFrame();
    for (int i = 0; i < arr->len; i++) {
        if (frame != NULL && frame->returnValue != NULL)
           break;
        _visitAst(arr->stuff[i]);
    }
    if (block->scoped == BLOCK_SCOPED) {
        // we should also push the return value up, to the parent frame
        //  if a parent frame exists, and we're exiting a scoped block, it
        //  means that that parent frame is either a function frame or
        //  another scoped block frame
        if (frame->prev != NULL) {
            frame->prev->returnValue = vimm_copy(frame->returnValue);
        }
        stack_popFrame();
    }
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
        symbol sym = symt_getVar(asn->identifier);
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
        symt_declareVar(decl->identifier);
        symbol sym = symt_getVar(decl->identifier);
        vimm_free(sym->payload); // it is initialized with NUI in declareVar
        sym->payload = vimm_copy(result);
    }
    dbgprintf("Assigned value ");
    vimm_dbgprint(result);
    dbgprintf(" to %s\n", decl->identifier);
    vimm_free(result);
}

void _visitFunctionCall(ast_node n) {
    dbgprintf("Visiting FunctionCall\n");
    // treat this call as an expression and ignore its value
    value_immediate result = evalExpr(n);
    vimm_free(result);
}

static void _visitFunctionReturn(ast_node n) {
    // assign the expression to the returnValue of the current stack frame
    ast_node expr = n->payload;
    stack_frame frame = stack_getCurrentFrame();
    frame->returnValue = evalExpr(expr);
    // the code in _visitBlock should take care of actually exiting the
    //  function now
}

void _visitAst(ast_node n) {
    switch (n->type) {
        case AST_EMPTY: break;
        case AST_BLOCK: _visitBlock(n); break;
        case AST_ASSIGNMENT: _visitAssignment(n); break;
        case AST_DECLARATION: _visitDeclaration(n); break;
        case AST_FUNCTIONDEF: break;
        case AST_FUNCTIONCALL: _visitFunctionCall(n); break;
        case AST_FUNCTIONRETURN: _visitFunctionReturn(n); break;
        default: stopHard("AST node type %d not handled", n->type); break;
    }
}