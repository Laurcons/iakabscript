
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
    enum operator_kind_t optype;
}

%token PERIOD
%token NU_DECI NU_HOHO_DECI FA GATA II IA SI NUI NIMIC
%token HOHOH HOHO HOH
%token PLUS MINUS ORI IMPARTIT_LA
%token <text> IDENTIFIER STRINGLIT
%token <num> NUMBERLIT

%type <node> assignment literal statement declaration block
%type <node> functionDef functionCall expression expression1
%type <arr> statements formalParamList actualParamList

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
  | functionDef
    { $$ = $1; }
  | functionCall
    { $$ = $1; }
    ;

functionCall:
    HOHOH IDENTIFIER
    { $$ = createAstFunctionCall($2, arr_create()); }
  | HOHO IDENTIFIER actualParamList HOH
    { $$ = createAstFunctionCall($2, $3); }
    ;

functionDef:
    NU_HOHO_DECI IDENTIFIER IA NIMIC SI block
    { $$ = createAstFunctionDef($2, arr_create(), $6); }
  | NU_HOHO_DECI IDENTIFIER IA formalParamList SI block
    { $$ = createAstFunctionDef($2, $4, $6); }
    ;

formalParamList:
    IDENTIFIER
    { $$ = arr_create(); arr_add($$, $1); }
  | formalParamList IDENTIFIER
    { arr_add($1, $2); }
    ;

actualParamList:
    expression
    { $$ = arr_create(); arr_add($$, $1); }
  | actualParamList expression
    { arr_add($1, $2); }
  ;

assignment:
    IDENTIFIER II literal
    { $$ = createAstAssignment($1, $3); }
    ;

declaration:
    NU_DECI IDENTIFIER II expression
    { $$ = createAstDeclaration($2, $4); }
    ;

literal:
    NUI
    { $$ = createAstNuiLiteral(); }
  | NUMBERLIT
    { $$ = createAstNumLiteral($1); }
  | STRINGLIT
    { $$ = createAstStrLiteral($1); }
  | IDENTIFIER
    { $$ = createAstIdentifierLiteral($1); }
    ;

expression:
    expression1
    { $$ = $1; }
  | expression PLUS expression1
    { $$ = createAstBinaryOp(OP_PLUS, $1, $3); }
  | expression MINUS expression1
    { $$ = createAstBinaryOp(OP_MINUS, $1, $3); }
    ;

expression1:
    literal
    { $$ = $1; }
  | expression1 ORI literal
    { $$ = createAstBinaryOp(OP_ORI, $1, $3); }
  | expression1 IMPARTIT_LA literal
    { $$ = createAstBinaryOp(OP_IMPARTIT_LA, $1, $3); }
    ;

%%

int yyerror(char* err) {
    fprintf(stderr, "at %d: %s near `%s`\n", yyline, err, yytext);
    exit(1);
}