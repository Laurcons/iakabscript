#include "ast_create.h"
#include <string.h>

ast_node rootNode = NULL;

// internal
static ast_node _ast_createNode(enum node_kind_t type, void* payload) {
    ast_node n = malloc(sizeof(ast_node_t));
    n->type = type;
    n->payload = payload;
    return n;
};

ast_node ast_createEmpty() {
    return _ast_createNode(AST_EMPTY, NULL);
}

ast_node ast_createBlock(array arr, enum block_isScoped scoped) {
    ast_block block = malloc(sizeof(ast_block_t));
    block->scoped = scoped;
    block->statements = arr;
    return _ast_createNode(AST_BLOCK, block);
}

ast_node ast_createAssignment(char* varname, ast_node expr) {
    ast_assignment asn = malloc(sizeof(ast_assignment_t));
    asn->identifier = strdup(varname);
    asn->expr = expr;
    return _ast_createNode(AST_ASSIGNMENT, asn);
}

ast_node ast_createDeclaration(char* varname, ast_node expr) {
    ast_declaration dcl = malloc(sizeof(ast_declaration_t));
    dcl->identifier = strdup(varname);
    dcl->expr = expr;
    return _ast_createNode(AST_DECLARATION, dcl);
}

ast_node ast_createFunctionDef(char* fname, array formalParams, ast_node block) {
    ast_functiondef fdef = malloc(sizeof(ast_functiondef_t));
    fdef->identifier = strdup(fname);
    fdef->block = block;
    fdef->formalParams = formalParams;
    return _ast_createNode(AST_FUNCTIONDEF, fdef);
}

ast_node ast_createFunctionCall(char* fname, array actualParams) {
    ast_functioncall fcall = malloc(sizeof(ast_functioncall_t));
    fcall->identifier = strdup(fname);
    fcall->actualParams = actualParams;
    return _ast_createNode(AST_FUNCTIONCALL, fcall);
}

ast_node ast_createFunctionReturn(ast_node expr) {
    return _ast_createNode(AST_FUNCTIONRETURN, expr);
}

ast_node ast_createDaca(ast_node expr, ast_node ifTrue, ast_node ifFalse) {
    ast_daca daca = malloc(sizeof(ast_daca_t));
    daca->expr = expr;
    daca->ifTrue = ifTrue;
    daca->ifFalse = ifFalse;
    return _ast_createNode(AST_DACA, daca);
}

ast_node ast_createCatTimp(ast_node expr, ast_node block) {
    ast_catTimp ctimp = malloc(sizeof(ast_catTimp_t));
    ctimp->expr = expr;
    ctimp->block = block;
    return _ast_createNode(AST_CATTIMP, ctimp);
}

ast_node ast_createNumLiteral(double val) {
    double* d = malloc(sizeof(double));
    *d = val;
    return _ast_createNode(AST_NUM_LITERAL, d);
}

ast_node ast_createStrLiteral(char* str) {
    char* buf = strdup(str);
    return _ast_createNode(AST_STR_LITERAL, buf);
}

ast_node ast_createNuiLiteral() {
    return _ast_createNode(AST_NUI_LITERAL, NULL);
}

ast_node ast_createIdentifierLiteral(char* identifier) {
    return _ast_createNode(AST_IDENTIFIER_LITERAL, strdup(identifier));
}

ast_node ast_createBinaryOp(enum operator_kind_t op, ast_node left, ast_node right) {
    ast_binaryop bop = malloc(sizeof(ast_binaryop_t));
    bop->operator = op;
    bop->left = left;
    bop->right = right;
    return _ast_createNode(AST_BINARYOP, bop);
}

void astSetBlockScope(ast_node node, enum block_isScoped scoped) {
    ast_block block = node->payload;
    block->scoped = scoped;
}

void astInit() {
    //rootNode = ast_createBlock(arr_create());
}