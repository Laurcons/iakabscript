#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "array.h"

enum node_kind_t {
    AST_EMPTY, // NULL
    AST_BLOCK, // ast_array
    AST_NUM_LITERAL, // double
    AST_STR_LITERAL, // char*
    AST_NUI_LITERAL, // NULL
    AST_IDENTIFIER_LITERAL, // char*
    AST_ASSIGNMENT, // ast_assignment
    AST_DECLARATION, // ast_declaration
    AST_FUNCTIONDEF, // ast_functiondef
    AST_FUNCTIONCALL, // ast_functioncall
    AST_BINARYOP, // ast_binaryop
    AST_UNARYOP, // ast_unaryop
};

enum operator_kind_t {
    // binary ops
    OP_PLUS,
    OP_MINUS,
    OP_ORI,
    OP_IMPARTIT_LA,
};

typedef struct {
    enum node_kind_t type;
    void* payload;
}* ast_node, ast_node_t;

typedef struct {
    char* identifier;
    ast_node expr;
} *ast_assignment, ast_assignment_t,
  *ast_declaration, ast_declaration_t;

typedef struct {
    char* identifier;
    ast_node block;
    array formalParams; // char*
} *ast_functiondef, ast_functiondef_t;

typedef struct {
    char* identifier;
    array actualParams; // char*
} *ast_functioncall, ast_functioncall_t;

typedef struct {
    enum operator_kind_t operator;
    ast_node left;
    ast_node right;
} *ast_binaryop, ast_binaryop_t;

typedef struct {
    enum operator_kind_t operator;
    ast_node right;
} *ast_unaryop, ast_unaryop_t;

ast_node rootNode;

#endif