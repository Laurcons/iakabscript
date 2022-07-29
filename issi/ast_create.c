#include "ast_create.h"
#include <string.h>

// internal
ast_node _createAstNode(enum node_kind_t type, void* payload) {
    ast_node n = malloc(sizeof(ast_node_t));
    n->type = type;
    n->payload = payload;
    return n;
};

ast_node createAstEmpty() {
    return _createAstNode(AST_EMPTY, NULL);
}

ast_node createAstBlock(array arr) {
    return _createAstNode(AST_BLOCK, arr);
}

ast_node createAstAssignment(char* varname, ast_node expr) {
    ast_assignment asn = malloc(sizeof(ast_assignment_t));
    asn->identifier = strdup(varname);
    asn->expr = expr;
    return _createAstNode(AST_ASSIGNMENT, asn);
}

ast_node createAstDeclaration(char* varname, ast_node expr) {
    ast_declaration asn = malloc(sizeof(ast_declaration_t));
    asn->identifier = strdup(varname);
    asn->expr = expr;
    return _createAstNode(AST_DECLARATION, asn);
}

ast_node createAstNumLiteral(double val) {
    double* d = malloc(sizeof(double));
    *d = val;
    return _createAstNode(AST_NUM_LITERAL, d);
}

ast_node createAstStrLiteral(char* str) {
    char* buf = strdup(str);
    return _createAstNode(AST_STR_LITERAL, str);
}

ast_node createAstNuiLiteral() {
    return _createAstNode(AST_NUI_LITERAL, NULL);
}

void astInit() {
    //rootNode = createAstBlock(arr_create());
}