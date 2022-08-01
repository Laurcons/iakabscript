#include "ast.h"
#include "array.h"
#include "symtable.h"
#include "util.h"
#include "ast_create.h"
#include "value_immediate.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

value_immediate _evalExpr(ast_node n);
void _visitAst(ast_node);

void _executeBuiltin(char* identifier, array actualParams) {
    if (strcmp(identifier, "zic") == 0) {
        printf("Builtin call to zic with %d params\n", actualParams->len);
        printf("ZIC CALL: ");
        for (int i = 0; i < actualParams->len; i++) {
            value_immediate vimm = _evalExpr(actualParams->stuff[i]);
            switch (vimm->type) {
                case VAL_NUI:
                    printf("<nui>"); break;
                case VAL_STRING:
                    printf("%s", (char*)vimm->payload); break;
                case VAL_NUMBER: {
                    double d = *((double*)vimm->payload);
                    if (fabs(d - (int)d) < 0.00001)
                        printf("%d", (int)d);
                    else printf("%f", d);
                    break;
                }
            }
            printf(" ");
        }
        printf("\n");
    } else {
        stopHard("Unknown builtin function %s. How did you even get this error?\n", identifier);
    }
}

void _assertImmediateType(value_immediate vimm, enum value_kind_t type) {
    if (vimm->type != type) {
        stopHard("Type (value_kind_t) of immediate was expected to be %d but is %d\n", type, vimm->type);
    }
}

value_immediate _evalExpr(ast_node n) {
    value_immediate vimm = malloc(sizeof(value_immediate_t));
    if (n->type == AST_NUM_LITERAL) {
        vimm->type = VAL_NUMBER;
        double* p = malloc(sizeof(double));
        *p = *((double*)n->payload);
        vimm->payload = p;
        printf("expression AST_NUM_LITERAL is %f\n", *p);
    }
    else if (n->type == AST_STR_LITERAL) {
        vimm->type = VAL_STRING;
        vimm->payload = strdup(n->payload);
        printf("expression AST_STR_LITERAL is %s\n", (char*)n->payload);
    }
    else if (n->type == AST_NUI_LITERAL) {
        vimm->type = VAL_NUI;
        printf("expression AST_NUI_LITERAL\n");
    } else if (n->type == AST_IDENTIFIER_LITERAL) {
        char* identifier = n->payload;
        symbol sym = symtableGetVar(identifier);
        if (sym->type != SYM_VARIABLE)
            stopHard("Identifier %s in expression is not a variable\n", sym->type);
        value_immediate othervimm = sym->payload;
        vimm_free(vimm);
        vimm = vimm_copy(othervimm);
        printf("expression AST_IDENTIFIER_LITERAL (%s)\n", sym->identifier);
    } else if (n->type == AST_BINARYOP) {
        ast_binaryop bop = n->payload;
        printf("Operator %d at binaryop\n", bop->operator);
        value_immediate vimmleft = _evalExpr(bop->left);
        value_immediate vimmright = _evalExpr(bop->right);
        if (bop->operator == OP_PLUS) {
            _assertImmediateType(vimmleft, VAL_NUMBER);
            _assertImmediateType(vimmright, VAL_NUMBER);
            double* pleft = vimmleft->payload;
            double* pright = vimmright->payload;
            double* p = malloc(sizeof(double));
            *p = *pleft + *pright;
            vimm->type = VAL_NUMBER;
            vimm->payload = p;
            printf("expression AST_BINARYOP (OP_PLUS) is %f\n", *p);
        } else if (bop->operator == OP_ORI) {
            _assertImmediateType(vimmleft, VAL_NUMBER);
            _assertImmediateType(vimmright, VAL_NUMBER);
            double* pleft = vimmleft->payload;
            double* pright = vimmright->payload;
            double* p = malloc(sizeof(double));
            *p = (*pleft) * (*pright);
            vimm->type = VAL_NUMBER;
            vimm->payload = p;
            printf("expression AST_BINARYOP (OP_ORI) is %f\n", *p);
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

int visitAst() {
    _visitAst(rootNode);
    return 0;
}

void _visitBlock(ast_node n) {
    printf("Visiting Block\n");
    array arr = n->payload;
    for (int i = 0; i < arr->len; i++)
        _visitAst(arr->stuff[i]);
}

void _visitAssignment(ast_node n) {
    printf("Visiting Assignment\n");
    ast_assignment asn = n->payload;
    symbol sym = symtableGetVar(asn->identifier);
    sym->payload = _evalExpr(asn->expr);
}

void _visitDeclaration(ast_node n) {
    printf("Visiting Declaration\n");
    ast_declaration decl = n->payload;
    symtableDeclareVar(decl->identifier);
    symbol sym = symtableGetVar(decl->identifier);
    sym->type = SYM_VARIABLE;
    sym->payload = _evalExpr(decl->expr);
}

void _visitFunctionCall(ast_node n) {
    printf("Visiting FunctionCall\n");
    ast_functioncall fcall = n->payload;
    if (symtableIsBuiltin(fcall->identifier)) {
        _executeBuiltin(fcall->identifier, fcall->actualParams);
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