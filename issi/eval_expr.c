#include "eval_expr.h"
#include "symtable.h"
#include "util.h"
#include "call_stack.h"
#include "builtins.h"
#include "ast_visit.h"
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wswitch"

static void _assertImmediateType(value_immediate vimm, enum value_kind_t type) {
    if (vimm->type != type) {
        runtimeStop("Type (value_kind_t) of immediate was expected to be %d but is %d\n", type, vimm->type);
    }
}

static value_immediate _eval_numLiteral(ast_node n) {
    double d = *((double*)n->payload);
    value_immediate vimm = vimm_createNumber(d);
    dbgprintf("expression AST_NUM_LITERAL is %f\n", d);
    return vimm;
}

static value_immediate _eval_strLiteral(ast_node n) {
    char* str = (char*)n->payload;
    value_immediate vimm = vimm_createString(str);
    dbgprintf("expression AST_STR_LITERAL is %s\n", str);
    return vimm;
}

static value_immediate _eval_nuiLiteral(ast_node n) {
    value_immediate vimm = vimm_createNui();
    dbgprintf("expression AST_NUI_LITERAL\n");
    return vimm;
}

static value_immediate _eval_identifierLiteral(ast_node n) {
    value_immediate vimm = NULL;
    char* identifier = n->payload;
    framed_variable fvar = stack_lookup(identifier);
    if (fvar != NULL) {
        vimm = vimm_copy(fvar->value);
    } else {
        symbol sym = symt_getVar(identifier);
        value_immediate othervimm = sym->payload;
        vimm = vimm_copy(othervimm);
    }
    dbgprintf("expression AST_IDENTIFIER_LITERAL (%s) is ", identifier);
    vimm_dbgprint(vimm);
    dbgprintf("\n");
    return vimm;
}

static value_immediate _eval_functionCall(ast_node n) {
    value_immediate vimm = NULL;
    ast_functioncall fcall = n->payload;
    dbgprintf("Evaluating function call to %s with %d params\n",
        fcall->identifier,
        arr_len(fcall->actualParams)
    );
    array vimms = arr_create(&vimm_free);
    // evaluate all exprs to immediates
    for (int i = 0; i < arr_len(fcall->actualParams); i++) {
        arr_add(
            vimms,
            evalExpr(arr_get(fcall->actualParams, i))
        );
    }
    if (symt_isBuiltin(fcall->identifier)) {
        vimm = builtin_invoke(fcall->identifier, vimms);
    } else {
        symbol_function symf = symt_getFunction(fcall->identifier);
        stack_createFunctionFrame();
        stack_frame frame = stack_getCurrentFrame();
        // push the variables on the stack
        for (int i = 0; i < arr_len(vimms); i++) {
            value_immediate vimm = arr_get(vimms, i);
            arr_add(
                frame->variables,
                framedvar_create(
                    arr_get(symf->params, i), // the identifier
                    vimm // the value
                )
            );
        }
        visitAst(symf->block);
        if (frame->returnValue != NULL)
            vimm = vimm_copy(frame->returnValue);
        else vimm = vimm_createNui();
        stack_popFrame();
    }
    arr_free(vimms);
    dbgprintf("evaluated function call to ");
    vimm_dbgprint(vimm);
    dbgprintf("\n");
    return vimm;
}

static value_immediate _eval_binaryOp(ast_node n) {
    value_immediate vimm = NULL;
    ast_binaryop bop = n->payload;
    dbgprintf("Operator %d at binaryop\n", bop->operator);
    value_immediate vimmleft = evalExpr(bop->left);
    value_immediate vimmright = evalExpr(bop->right);
    if (bop->operator == OP_PLUS ||
        bop->operator == OP_MINUS ||
        bop->operator == OP_ORI ||
        bop->operator == OP_IMPARTIT_LA ||
        bop->operator == OP_MODULO
    ) {
        _assertImmediateType(vimmleft, VAL_NUMBER);
        _assertImmediateType(vimmright, VAL_NUMBER);
        double left = *((double*)vimmleft->payload);
        double right = *((double*)vimmright->payload);
        double result;
        switch (bop->operator) {
            case OP_PLUS: result = left + right; break;
            case OP_MINUS: result = left - right; break;
            case OP_ORI: result = left * right; break;
            case OP_IMPARTIT_LA: result = left / right; break;
            case OP_MODULO: result = (int)left % (int)right; break;
        }
        vimm = vimm_createNumber(result);
        dbgprintf("expression AST_BINARYOP (arithmetic: %d) is %f\n", bop->operator, result);
    } else if (
        bop->operator == OP_EGAL ||
        bop->operator == OP_NUEGAL ||
        bop->operator == OP_MAIMIC ||
        bop->operator == OP_MAIMARE
    ) {
        int res = vimm_compare(vimmleft, vimmright);
        if (res == -2)
            runtimeStop("Uncomparable operands in expression");
        switch (bop->operator) {
            case OP_EGAL: res = res == 0; break;
            case OP_NUEGAL: res = res != 0; break;
            case OP_MAIMIC: res = res == -1; break;
            case OP_MAIMARE: res = res == 1; break;
        }
        vimm = vimm_createNumber(res);
        dbgprintf("expression AST_BINARYOP (equality/comparison: %d) is %d\n", bop->operator, res);
    } else if (
        bop->operator == OP_DEODATACU ||
        bop->operator == OP_SAU
    ) {
        int left = vimm_isTruthy(vimmleft);
        int right = vimm_isTruthy(vimmright);
        int res;
        switch (bop->operator) {
            case OP_DEODATACU: res = left && right; break;
            case OP_SAU: res = left || right; break;
        }
        vimm = vimm_createNumber(res);
        dbgprintf("expression AST_BINARYOP (logical: %d) is %d\n", bop->operator, res);
    } else {
        runtimeStop("Unknown operator_kind_t %d at evalExpr\n", bop->operator);
    }
    vimm_free(vimmleft);
    vimm_free(vimmright);
    return vimm;
}

static value_immediate _eval_unaryOp(ast_node n) {
    value_immediate vimm = NULL;
    ast_unaryop uop = n->payload;
    dbgprintf("Operator %d at binaryop\n", uop->operator);
    value_immediate vimmright = evalExpr(uop->right);
    if (uop->operator == OP_MINUS) {
        _assertImmediateType(vimmright, VAL_NUMBER);
        double d = *((double*)vimmright->payload);
        d = -1 * d;
        vimm = vimm_createNumber(d);
    }
    else if (uop->operator == OP_INVERS) {
        int res = vimm_isTruthy(vimmright);
        res = !res;
        vimm = vimm_createNumber(res);
    }
    return vimm;
}

value_immediate evalExpr(ast_node n) {
    value_immediate vimm = NULL;
    switch (n->type) {
        case AST_NUM_LITERAL: vimm = _eval_numLiteral(n); break;
        case AST_STR_LITERAL: vimm = _eval_strLiteral(n); break;
        case AST_NUI_LITERAL: vimm = _eval_nuiLiteral(n); break;
        case AST_IDENTIFIER_LITERAL: vimm = _eval_identifierLiteral(n); break;
        case AST_FUNCTIONCALL: vimm = _eval_functionCall(n); break;
        case AST_BINARYOP: vimm = _eval_binaryOp(n); break;
        case AST_UNARYOP: vimm = _eval_unaryOp(n); break;
        default: runtimeStop("Unknown node_kind_t %d at evalExpr\n", n->type);
    }
    if (vimm == NULL)
        runtimeStop("ISSI fault: vimm didn't get assigned when evaluating expression\n");
    return vimm;
}