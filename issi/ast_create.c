#include "ast_create.h"
#include <string.h>

ast_node rootNode = NULL;

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

ast_node createAstBlock(array arr, enum block_isScoped scoped) {
    ast_block block = malloc(sizeof(ast_block_t));
    block->scoped = scoped;
    block->statements = arr;
    return _createAstNode(AST_BLOCK, block);
}

ast_node createAstAssignment(char* varname, ast_node expr) {
    ast_assignment asn = malloc(sizeof(ast_assignment_t));
    asn->identifier = strdup(varname);
    asn->expr = expr;
    return _createAstNode(AST_ASSIGNMENT, asn);
}

ast_node createAstDeclaration(char* varname, ast_node expr) {
    ast_declaration dcl = malloc(sizeof(ast_declaration_t));
    dcl->identifier = strdup(varname);
    dcl->expr = expr;
    return _createAstNode(AST_DECLARATION, dcl);
}

ast_node createAstFunctionDef(char* fname, array formalParams, ast_node block) {
    ast_functiondef fdef = malloc(sizeof(ast_functiondef_t));
    fdef->identifier = strdup(fname);
    fdef->block = block;
    fdef->formalParams = formalParams;
    return _createAstNode(AST_FUNCTIONDEF, fdef);
}

ast_node createAstFunctionCall(char* fname, array actualParams) {
    ast_functioncall fcall = malloc(sizeof(ast_functioncall_t));
    fcall->identifier = strdup(fname);
    fcall->actualParams = actualParams;
    return _createAstNode(AST_FUNCTIONCALL, fcall);
}

ast_node createAstFunctionReturn(ast_node expr) {
    return _createAstNode(AST_FUNCTIONRETURN, expr);
}

ast_node createAstNumLiteral(double val) {
    double* d = malloc(sizeof(double));
    *d = val;
    return _createAstNode(AST_NUM_LITERAL, d);
}

ast_node createAstStrLiteral(char* str) {
    char* buf = strdup(str);
    return _createAstNode(AST_STR_LITERAL, buf);
}

ast_node createAstNuiLiteral() {
    return _createAstNode(AST_NUI_LITERAL, NULL);
}

ast_node createAstIdentifierLiteral(char* identifier) {
    return _createAstNode(AST_IDENTIFIER_LITERAL, strdup(identifier));
}

ast_node createAstBinaryOp(enum operator_kind_t op, ast_node left, ast_node right) {
    ast_binaryop bop = malloc(sizeof(ast_binaryop_t));
    bop->operator = op;
    bop->left = left;
    bop->right = right;
    return _createAstNode(AST_BINARYOP, bop);
}

void astSetBlockScope(ast_node node, enum block_isScoped scoped) {
    ast_block block = node->payload;
    block->scoped = scoped;
}

void astInit() {
    //rootNode = createAstBlock(arr_create());
}