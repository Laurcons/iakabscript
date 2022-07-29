#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

enum node_kind_t {
    AST_EMPTY, // NULL
    AST_BLOCK, // ast_array
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
    char* identifier;
    ast_node expr;
} *ast_assignment, ast_assignment_t,
  *ast_declaration, ast_declaration_t;

ast_node rootNode;

#endif