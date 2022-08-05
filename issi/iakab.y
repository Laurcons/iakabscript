
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
%token EGAL NUEGAL INVERS PLUS MINUS ORI IMPARTIT_LA
%token DACA ATUNCI ALTFEL CAT_TIMP
%token <text> IDENTIFIER STRINGLIT
%token <num> NUMBERLIT

%type <node> assignment literal statement declaration scopedBlock unscopedBlock
%type <node> functionDef functionCall functionReturn
%type <node> flowControl daca catTimp
%type <node> expression expression1 expression2
%type <arr> statements formalParamList actualParamList

%start st

%%

st:
    statements
    { rootNode = createAstBlock($1, BLOCK_NOT_SCOPED); }
    ;

scopedBlock:
  FA PERIOD statements GATA
  { $$ = createAstBlock($3, BLOCK_SCOPED); }
  ;

unscopedBlock:
  FA PERIOD statements GATA
  { $$ = createAstBlock($3, BLOCK_NOT_SCOPED); }
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
    { $$ = createAstEmpty(); }
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
    { $$ = createAstDaca($2, $4, NULL); }
  | DACA expression ATUNCI scopedBlock ALTFEL scopedBlock
    { $$ = createAstDaca($2, $4, $6); }
    ;

catTimp:
    CAT_TIMP expression scopedBlock
    { $$ = createAstCatTimp($2, $3); }
    ;

functionCall:
    HOHOH IDENTIFIER
    { $$ = createAstFunctionCall($2, arr_create()); }
  | HOHO IDENTIFIER actualParamList HOH
    { $$ = createAstFunctionCall($2, $3); }
  | HOHO IDENTIFIER HOH
    { stopHard("hoho..hoh syntax is not allowed when function takes no parameters"); }
    ;

functionDef:
    NU_HOHO_DECI IDENTIFIER IA NIMIC SI unscopedBlock
    { $$ = createAstFunctionDef($2, arr_create(), $6); }
  | NU_HOHO_DECI IDENTIFIER IA formalParamList SI unscopedBlock
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
    IDENTIFIER II expression
    { $$ = createAstAssignment($1, $3); }
    ;

declaration:
    NU_DECI IDENTIFIER II expression
    { $$ = createAstDeclaration($2, $4); }
    ;

functionReturn:
    IESI expression
    { $$ = createAstFunctionReturn($2); }
  | IESI
    { $$ = createAstFunctionReturn(createAstNuiLiteral()); }
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
  | expression EGAL expression1
    { $$ = createAstBinaryOp(OP_EGAL, $1, $3); }
  | expression NUEGAL expression1
    { $$ = createAstBinaryOp(OP_NUEGAL, $1, $3); }
    ;

expression1:
    expression2
    { $$ = $1; }
  | expression1 PLUS expression2
    { $$ = createAstBinaryOp(OP_PLUS, $1, $3); }
  | expression1 MINUS expression2
    { $$ = createAstBinaryOp(OP_MINUS, $1, $3); }
    ;

expression2:
    literal
    { $$ = $1; }
  | functionCall
    { $$ = $1; }
  | expression2 ORI literal
    { $$ = createAstBinaryOp(OP_ORI, $1, $3); }
  | expression2 IMPARTIT_LA literal
    { $$ = createAstBinaryOp(OP_IMPARTIT_LA, $1, $3); }
    ;

%%

int yyerror(char* err) {
    fprintf(stderr, "at %d: %s near `%s`\n", yyline, err, yytext);
    exit(1);
}