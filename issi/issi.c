#include "issi.h"
#include <stdarg.h>

// internal
void stopHard(char* format, ...) {
    va_list args;
    va_start(args, format);
    char* newFormat = (char*)malloc(sizeof(char) * (strlen(format) + 17));
    strcpy(newFormat, "runtime error: ");
    strcat(newFormat, format);
    strcat(newFormat, "\n");
    fprintf(stderr, newFormat, args);
    free(newFormat);
    exit(0);
}

/// =================== MEMORY MODEL =====================

void symtableInit() {
    symtable = createArray();
}

void symtableDeclareVar(char* identifier) {
    // try find variable
    for (int i = 0; i < symtable->len; i++) {
        symbol sym = symtable->stuff[i];
        if (strcmp(sym->identifier, identifier) == 0)
            stopHard("Variable %s already declared", sym->identifier);
    }
    // add a new one
    symbol sym = malloc(sizeof(symbol_t));
    sym->identifier = strdup(identifier);
    sym->type = SYM_VARIABLE_NUI;
    addToArray(symtable, sym);
}

void symtableDeclareFunction(char* identifier, ast_node fpointer) {
    // TODO:
}

symbol symtableGetVar(char* identifier) {
    // try find variable
    for (int i = 0; i < symtable->len; i++) {
        symbol sym = symtable->stuff[i];
        if (strcmp(sym->identifier, identifier) == 0)
            return sym;
    }
    stopHard("Variable not found when getting");
    return NULL; // will not be called
}

/// ==================== AST VISIT  ====================

void _visitAst(ast_node);

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

/// ==================== PARSER CODE =======================

array createArray() {
    array p = malloc(sizeof(array_t));
    p->cap = 3;
    p->len = 0;
    p->stuff = malloc(sizeof(void*) * p->cap);
    return p;
}

void freeArray(array a) {
    free(a->stuff);
    free(a);
}

void addToArray(array a, void* elem) {
    if (a->len == a->cap) {
        // resize
        a->cap *= 2;
        a->stuff = realloc(a->stuff, sizeof(void*) * a->cap);
    }
    a->stuff[a->len++] = elem;
}

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
    //rootNode = createAstBlock(createArray());
}