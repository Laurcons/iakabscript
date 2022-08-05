#ifndef AST_CREATE_H_INCLUDED
#define AST_CREATE_H_INCLUDED

#include "ast.h"
#include "array.h"
#include <stdlib.h>

ast_node createAstEmpty();
ast_node createAstBlock(array arr, enum block_isScoped scoped);
ast_node createAstAssignment(char* varname, ast_node expr);
ast_node createAstDeclaration(char* varname, ast_node expr);
ast_node createAstFunctionDef(char* fname, array formalParams, ast_node block);
ast_node createAstFunctionCall(char* fname, array actualParams);
ast_node createAstFunctionReturn(ast_node expr);
ast_node createAstDaca(ast_node expr, ast_node ifTrue, ast_node ifFalse);
ast_node createAstCatTimp(ast_node expr, ast_node block);

ast_node createAstNumLiteral(double val);
ast_node createAstStrLiteral(char* str);
ast_node createAstNuiLiteral();
ast_node createAstIdentifierLiteral(char* identifier);

ast_node createAstBinaryOp(enum operator_kind_t op, ast_node left, ast_node right);
ast_node createAstUnaryOp(enum operator_kind_t op, ast_node right);

void astSetBlockScope(ast_node block, enum block_isScoped scoped);

void astInit();

#endif