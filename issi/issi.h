#ifndef ISSI_H_INCLUDED
#define ISSI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int yylex();

int main(int, char**);

/// Abstract Syntax Tree stuff

enum node_kind_t {
    AST_EMPTY, // NULL
    AST_ROOT, // ast_array
    AST_NUM_LITERAL, // double
    AST_STR_LITERAL, // char*
    AST_NUI_LITERAL, // NULL
    AST_ASSIGNMENT, // ast_assignment
    AST_DECLARATION, // ast_declaration
};

typedef struct {
    enum node_kind_t type;
    void* payload;
}* ast_node, ast_node_t;

typedef struct {
    int len;
    int cap;
    void** stuff;
}* array, array_t;

typedef struct {
    char* identifier;
    ast_node expr;
} *ast_assignment, ast_assignment_t,
  *ast_declaration, ast_declaration_t;

array createArray();
void freeArray(array);
void addToArray(array, void*);

ast_node createAstEmpty();
ast_node createAstAssignment(char* varname, ast_node expr);
ast_node createAstDeclaration(char* varname, ast_node expr);
ast_node createAstNumLiteral(double val);
ast_node createAstStrLiteral(char* str);
ast_node createAstNuiLiteral();

void astInit();
ast_node addToRoot();

/// Memory Model stuff

typedef enum symbol_kind_t {
    SYM_FUNCTION, // ast_node
    SYM_VARIABLE_NUMBER, // double
    SYM_VARIABLE_STRING, // char*
    SYM_VARIABLE_NUI // NULL
} symbol_kind_t;

typedef struct symbol_t {
    char* identifier;
    symbol_kind_t type;
    void* payload;
} *symbol, symbol_t;

void symtableInit();
void symtableDeclareVar(char* identifier);
void symtableDeclareFunction(char* identifier, ast_node fpointer);
symbol symtableGetVar(char* identifier);
void symtableSetVar(char* identifier, void* value);

// declarations

ast_node rootNode;
array symtable;

#endif // !ISSI_H_INCLUDED