
%{
#include "issi.h"

extern char* yytext;
extern int yyin, yyline;
int yyerror(char*);
%}

%union {
    char* text;
    double num;
    ast_node node;
    array arr;
}

%token PERIOD
%token NU_DECI NU_HOHO_DECI FA GATA II NUI
%token <text> IDENTIFIER STRINGLIT
%token <num> NUMBERLIT

%type <node> assignment literal statement declaration block
%type <arr> statements

%start st

%%

st:
    statements
    { rootNode = createAstBlock($1); }
    ;

block:
  FA PERIOD statements GATA
  { $$ = createAstBlock($3); }
  ;

statements:
    /* empty */
    { $$ = arr_create(); }
  | statements statement PERIOD
    { arr_add($1, $2); $$ = $1; }
  | statements block
    { arr_add($1, $2); $$ = $1; }
    ;

statement:
    /* nothing */
    { $$ = createAstEmpty(); }
  | assignment
    { $$ = $1; }
  | declaration
    { $$ = $1; }
    ;

assignment:
    IDENTIFIER II literal
    { $$ = createAstAssignment($1, $3); }
    ;

declaration:
    NU_DECI IDENTIFIER II literal
    { $$ = createAstDeclaration($2, $4); }
    ;

literal:
    NUI
    { $$ = createAstNuiLiteral(); }
  | NUMBERLIT
    { $$ = createAstNumLiteral($1); }
  | STRINGLIT
    { $$ = createAstStrLiteral($1); }
    ;

%%

int yyerror(char* err) {
    fprintf(stderr, "at %d: %s near `%s`\n", yyline, err, yytext);
    exit(1);
}