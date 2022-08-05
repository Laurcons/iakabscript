#ifndef AST_CREATE_H_INCLUDED
#define AST_CREATE_H_INCLUDED

#include "ast.h"
#include "array.h"
#include <stdlib.h>

ast_node ast_createEmpty();
ast_node ast_createBlock(array arr, enum block_isScoped scoped);
ast_node ast_createAssignment(char* varname, ast_node expr);
ast_node ast_createDeclaration(char* varname, ast_node expr);
ast_node ast_createFunctionDef(char* fname, array formalParams, ast_node block);
ast_node ast_createFunctionCall(char* fname, array actualParams);
ast_node ast_createFunctionReturn(ast_node expr);
ast_node ast_createDaca(ast_node expr, ast_node ifTrue, ast_node ifFalse);
ast_node ast_createCatTimp(ast_node expr, ast_node block);

ast_node ast_createNumLiteral(double val);
ast_node ast_createStrLiteral(char* str);
ast_node ast_createNuiLiteral();
ast_node ast_createIdentifierLiteral(char* identifier);

ast_node ast_createBinaryOp(enum operator_kind_t op, ast_node left, ast_node right);
ast_node ast_createUnaryOp(enum operator_kind_t op, ast_node right);

void astSetBlockScope(ast_node block, enum block_isScoped scoped);

void astInit();

#endif