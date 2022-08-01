#include "eval_expr.h"
#include "symtable.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

static void _assertImmediateType(value_immediate vimm, enum value_kind_t type) {
    if (vimm->type != type) {
        stopHard("Type (value_kind_t) of immediate was expected to be %d but is %d\n", type, vimm->type);
    }
}

value_immediate evalExpr(ast_node n) {
    value_immediate vimm;
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
        symbol sym = symtableGetVar(identifier);
        if (sym->type != SYM_VARIABLE)
            stopHard("Identifier %s in expression is not a variable\n", sym->type);
        value_immediate othervimm = sym->payload;
        vimm = vimm_copy(othervimm);
        dbgprintf("expression AST_IDENTIFIER_LITERAL (%s)\n", sym->identifier);
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
    return vimm;
}