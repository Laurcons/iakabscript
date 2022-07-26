
%{
#include "issi.h"

extern char* yytext;
extern int yyin, yylineno;
int yyerror(char*);
%}

%union {
    char* text;
    double num;
    ast_node node;
}

%token PERIOD
%token NU_DECI II NUI
%token <text> IDENTIFIER STRINGLIT
%token <num> NUMBERLIT

%type <node> assignment literal statement declaration

%start st

%%

st:
    statements
    ;

statements:
  | statements statement PERIOD
    { addToArray(rootNode->payload, $2); }
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
    fprintf(stderr, "at %d: %s near `%s`\n", yylineno, err, yytext);
    exit(1);
}