#include "issi.h"

void stopHard(char* message) {
    fprintf(stderr, "runtime error: %s\n", message);
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
        if (strcmp(sym->identifier, identifier))
            stopHard("Variable already declared");
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

void _visitRoot(ast_node n) {
    printf("Visiting Root\n");
    array arr = n->payload;
    for (int i = 0; i < arr->len; i++)
        _visitAst(arr->stuff[i]);
}

void _visitAssignment(ast_node n) {
    printf("Visiting Assignment\n");
    ast_assignment asn = n->payload;
    if (asn->expr->type == AST_NUM_LITERAL) {
        symbol sym = symtableGetVar(asn->identifier);
        sym->type = SYM_VARIABLE_NUMBER;
        sym->payload = asn->expr->payload;
        double d = *((double*)sym->payload);
        printf("set %s to %f\n", sym->identifier, d);
    }
}

void _visitDeclaration(ast_node n) {
    printf("Visiting Declaration\n");
    ast_declaration decl = n->payload;
    if (decl->expr->type == AST_NUM_LITERAL) {
        symtableDeclareVar(decl->identifier);
        symbol sym = symtableGetVar(decl->identifier);
        sym->type = SYM_VARIABLE_NUMBER;
        sym->payload = decl->expr->payload;
        double d = *((double*)sym->payload);
        printf("set %s to %f\n", sym->identifier, d);
    }
}

void _visitAst(ast_node n) {
    switch (n->type) {
        case AST_ROOT: _visitRoot(n); break;
        case AST_ASSIGNMENT: _visitAssignment(n); break;
        case AST_DECLARATION: _visitDeclaration(n); break;
        default: stopHard("AST node type not handled"); break;
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

void astInit() {
    rootNode = _createAstNode(AST_ROOT, createArray());
}