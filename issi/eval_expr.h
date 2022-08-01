#ifndef EVAL_EXPR_H_INCLUDED
#define EVAL_EXPR_H_INCLUDED

#include "value_immediate.h"
#include "ast.h"

value_immediate evalExpr(ast_node n);

#endif