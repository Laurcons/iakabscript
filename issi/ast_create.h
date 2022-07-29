#ifndef AST_CREATE_H_INCLUDED
#define AST_CREATE_H_INCLUDED

#include "ast.h"
#include "array.h"
#include <stdlib.h>

ast_node createAstEmpty();
ast_node createAstBlock(array arr);
ast_node createAstAssignment(char* varname, ast_node expr);
ast_node createAstDeclaration(char* varname, ast_node expr);
ast_node createAstFunctionDef(char* fname, array formalParams, ast_node block);
ast_node createAstFunctionCall(char* fname, array actualParams);

ast_node createAstNumLiteral(double val);
ast_node createAstStrLiteral(char* str);
ast_node createAstNuiLiteral();

void astInit();

#endif