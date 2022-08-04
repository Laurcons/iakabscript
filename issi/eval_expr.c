#include "eval_expr.h"
#include "symtable.h"
#include "util.h"
#include "call_stack.h"
#include "builtins.h"
#include "ast_visit.h"
#include <stdlib.h>
#include <string.h>

static void _assertImmediateType(value_immediate vimm, enum value_kind_t type) {
    if (vimm->type != type) {
        stopHard("Type (value_kind_t) of immediate was expected to be %d but is %d\n", type, vimm->type);
    }
}

value_immediate evalExpr(ast_node n) {
    value_immediate vimm = NULL;
    if (n->type == AST_NUM_LITERAL) {
        double d = *((double*)n->payload);
        vimm = vimm_createNumber(d);
        dbgprintf("expression AST_NUM_LITERAL is %f\n", d);
    }
    else if (n->type == AST_STR_LITERAL) {
        char* str = (char*)n->payload;
        vimm = vimm_createString(str);
        dbgprintf("expression AST_STR_LITERAL is %s\n", str);
    }
    else if (n->type == AST_NUI_LITERAL) {
        vimm = vimm_createNui();
        dbgprintf("expression AST_NUI_LITERAL\n");
    }
    else if (n->type == AST_IDENTIFIER_LITERAL) {
        char* identifier = n->payload;
        framed_variable fvar = stack_lookup(identifier);
        if (fvar != NULL) {
            vimm = vimm_copy(fvar->value);
        } else {
            symbol sym = symtableGetVar(identifier);
            value_immediate othervimm = sym->payload;
            vimm = vimm_copy(othervimm);
        }
        dbgprintf("expression AST_IDENTIFIER_LITERAL (%s)\n", identifier);
    }
    else if (n->type == AST_FUNCTIONCALL) {
        ast_functioncall fcall = n->payload;
        dbgprintf("evaluating function call expression to %s\n", fcall->identifier);
        array vimms = arr_create();
        // evaluate all exprs to immediates
        for (int i = 0; i < fcall->actualParams->len; i++) {
            arr_add(
                vimms,
                evalExpr(fcall->actualParams->stuff[i])
            );
        }
        if (symtableIsBuiltin(fcall->identifier)) {
            vimm = invokeBuiltin(fcall->identifier, vimms);
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
            visitAst(symf->block);
            vimm = vimm_createNui();
            stack_popFrame();
        }
        for (int i = 0; i < vimms->len; i++) {
            value_immediate v = vimms->stuff[i];
            vimm_free(v);
        }
        dbgprintf("evaluated function call to ");
        vimm_dbgprint(vimm);
        dbgprintf("\n");
    }   
    else if (n->type == AST_BINARYOP) {
        ast_binaryop bop = n->payload;
        dbgprintf("Operator %d at binaryop\n", bop->operator);
        value_immediate vimmleft = evalExpr(bop->left);
        value_immediate vimmright = evalExpr(bop->right);
        if (bop->operator == OP_PLUS) {
            _assertImmediateType(vimmleft, VAL_NUMBER);
            _assertImmediateType(vimmright, VAL_NUMBER);
            double* pleft = vimmleft->payload;
            double* pright = vimmright->payload;
            double d = *pleft + *pright;
            vimm = vimm_createNumber(d);
            dbgprintf("expression AST_BINARYOP (OP_PLUS) is %f\n", d);
        }
        else if (bop->operator == OP_ORI) {
            _assertImmediateType(vimmleft, VAL_NUMBER);
            _assertImmediateType(vimmright, VAL_NUMBER);
            double* pleft = vimmleft->payload;
            double* pright = vimmright->payload;
            double d = (*pleft) * (*pright);
            vimm = vimm_createNumber(d);
            dbgprintf("expression AST_BINARYOP (OP_ORI) is %f\n", d);
        }
        else if (bop->operator == OP_EGAL) {
            int res = vimm_equals(vimmleft, vimmright);
            vimm = vimm_createNumber(res);
            dbgprintf("expression AST_BINARYOP (OP_EGAL) is %d\n", res);
        }
        else if (bop->operator == OP_NUEGAL) {
            int res = !vimm_equals(vimmleft, vimmright);
            vimm = vimm_createNumber(res);
            dbgprintf("expression AST_BINARYOP (OP_NUEGAL) is %d\n", res);
        } else {
            stopHard("Unknown operator_kind_t %d at evalExpr\n", bop->operator);
        }
        vimm_free(vimmleft);
        vimm_free(vimmright);
    } else {
        stopHard("Unknown node_kind_t %d at evalExpr\n", n->type);
    }
    if (vimm == NULL)
        stopHard("ISSI fault: vimm didn't get assigned when evaluating expression\n");
    return vimm;
}