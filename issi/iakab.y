
%{
#include "issi.h"

extern char* yytext;
extern int yyline;
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
%token HOHOH HOHO HOH IESI
%token EGAL NUEGAL MAIMIC MAIMARE INVERS SAU DEODATACU PLUS MINUS ORI IMPARTITLA MODULO
%token DACA ATUNCI ALTFEL CAT_TIMP
%token <text> IDENTIFIER STRINGLIT
%token <num> NUMBERLIT

%type <node> assignment literal statement declaration scopedBlock unscopedBlock
%type <node> functionDef functionCall functionReturn
%type <node> flowControl daca catTimp
%type <node> expression
%type <arr> statements formalParamList actualParamList

%left EGAL NUEGAL
%left INVERS
%left SAU
%left DEODATACU
%left MAIMIC MAIMARE
%left PLUS MINUS
%left ORI IMPARTITLA MODULO
%left UNARY_MINUS

%start st

%%

st:
    statements
    { rootNode = ast_createBlock($1, BLOCK_NOT_SCOPED); }
    ;

scopedBlock:
  FA PERIOD statements GATA
  { $$ = ast_createBlock($3, BLOCK_SCOPED); }
  ;

unscopedBlock:
  FA PERIOD statements GATA
  { $$ = ast_createBlock($3, BLOCK_NOT_SCOPED); }
  ;

statements:
    /* empty */
    { $$ = arr_create(); }
  | statements statement PERIOD
    { arr_add($1, $2); $$ = $1; }
  | statements scopedBlock
    { arr_add($1, $2); $$ = $1; }
    ;

statement:
    /* nothing */
    { $$ = ast_createEmpty(); }
  | assignment
  | declaration
  | functionDef
  | functionCall
  | functionReturn
  | flowControl
    ;

flowControl:
    daca
  | catTimp
    ;

daca:
    DACA expression ATUNCI scopedBlock
    { $$ = ast_createDaca($2, $4, NULL); }
  | DACA expression ATUNCI scopedBlock ALTFEL scopedBlock
    { $$ = ast_createDaca($2, $4, $6); }
    ;

catTimp:
    CAT_TIMP expression scopedBlock
    { $$ = ast_createCatTimp($2, $3); }
    ;

functionCall:
    HOHOH IDENTIFIER
    { $$ = ast_createFunctionCall($2, arr_create()); }
  | HOHO IDENTIFIER actualParamList HOH
    { $$ = ast_createFunctionCall($2, $3); }
  | HOHO IDENTIFIER HOH
    { parsingStop("hoho..hoh syntax is not allowed when function takes no parameters"); }
    ;

functionDef:
    NU_HOHO_DECI IDENTIFIER IA NIMIC SI unscopedBlock
    { $$ = ast_createFunctionDef($2, arr_create(), $6); }
  | NU_HOHO_DECI IDENTIFIER IA formalParamList SI unscopedBlock
    { $$ = ast_createFunctionDef($2, $4, $6); }
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
  | actualParamList SI expression
    { arr_add($1, $3); }
  ;

assignment:
    IDENTIFIER II expression
    { $$ = ast_createAssignment($1, $3); }
    ;

declaration:
    NU_DECI IDENTIFIER II expression
    { $$ = ast_createDeclaration($2, $4); }
    ;

functionReturn:
    IESI expression
    { $$ = ast_createFunctionReturn($2); }
  | IESI
    { $$ = ast_createFunctionReturn(ast_createNuiLiteral()); }
    ;

literal:
    NUI
    { $$ = ast_createNuiLiteral(); }
  | NUMBERLIT
    { $$ = ast_createNumLiteral($1); }
  | STRINGLIT
    { $$ = ast_createStrLiteral($1); }
  | IDENTIFIER
    { $$ = ast_createIdentifierLiteral($1); }
    ;

 /* the precedence of these is declared above, in the %left statements */
expression:
    expression EGAL expression
    { $$ = ast_createBinaryOp(OP_EGAL, $1, $3); }
  | expression NUEGAL expression
    { $$ = ast_createBinaryOp(OP_NUEGAL, $1, $3); }
  | expression PLUS expression
    { $$ = ast_createBinaryOp(OP_PLUS, $1, $3); }
  | expression MINUS expression
    { $$ = ast_createBinaryOp(OP_MINUS, $1, $3); }
  | expression ORI literal
    { $$ = ast_createBinaryOp(OP_ORI, $1, $3); }
  | expression IMPARTITLA literal
    { $$ = ast_createBinaryOp(OP_IMPARTIT_LA, $1, $3); }
  | MINUS expression %prec UNARY_MINUS
    { $$ = ast_createUnaryOp(OP_MINUS, $2); }
  | literal
    { $$ = $1; }
  | functionCall
    { $$ = $1; }
    ;

%%

int yyerror(char* err) {
    fprintf(stderr, "at %d: %s near `%s`\n", yyline, err, yytext);
    exit(1);
}