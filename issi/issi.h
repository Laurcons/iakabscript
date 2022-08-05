#ifndef ISSI_H_INCLUDED
#define ISSI_H_INCLUDED

int yylex();

int main(int, char**);

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "symtable.h"
#include "ast_create.h"
#include "ast_visit.h"
#include "ast_visit_init.h"

#endif // !ISSI_H_INCLUDED