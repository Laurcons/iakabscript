#include "ast.h"
#include "array.h"
#include "symtable.h"
#include "util.h"
#include "ast_create.h"
#include <stdio.h>

void _visitAst(ast_node);

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
    if (asn->expr->type == AST_NUM_LITERAL) {
        sym->type = SYM_VARIABLE_NUMBER;
        sym->payload = asn->expr->payload;
        double d = *((double*)sym->payload);
        printf("set %s to %f\n", sym->identifier, d);
    }
    else if (asn->expr->type == AST_STR_LITERAL) {
        sym->type = SYM_VARIABLE_STRING;
        sym->payload = asn->expr->payload;
        printf("set %s to \"%s\"\n", sym->identifier, (char*)sym->payload);
    }
    else if (asn->expr->type == AST_NUI_LITERAL) {
        sym->type = SYM_VARIABLE_NUI;
        sym->payload = NULL;
        printf("set %s to nui\n", sym->identifier);
    }
}

void _visitDeclaration(ast_node n) {
    printf("Visiting Declaration\n");
    ast_declaration decl = n->payload;
    symtableDeclareVar(decl->identifier);
    symbol sym = symtableGetVar(decl->identifier);
    if (decl->expr->type == AST_NUM_LITERAL) {
        sym->type = SYM_VARIABLE_NUMBER;
        sym->payload = decl->expr->payload;
        double d = *((double*)sym->payload);
        printf("declared %s to %f\n", sym->identifier, d);
    }
    else if (decl->expr->type == AST_STR_LITERAL) {
        sym->type = SYM_VARIABLE_STRING;
        sym->payload = decl->expr->payload;
        printf("declared %s to \"%s\"\n", sym->identifier, (char*)sym->payload);
    }
    else if (decl->expr->type == AST_NUI_LITERAL) {
        sym->type = SYM_VARIABLE_NUI;
        sym->payload = NULL;
        printf("declared %s to nui\n", sym->identifier);
    }
}

void _visitAst(ast_node n) {
    //printf("Visiting type %d\n", n->type);
    switch (n->type) {
        case AST_EMPTY: /* nothing */; break;
        case AST_BLOCK: _visitBlock(n); break;
        case AST_ASSIGNMENT: _visitAssignment(n); break;
        case AST_DECLARATION: _visitDeclaration(n); break;
        default: stopHard("AST node type %d not handled", n->type); break;
    }
}