
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

%token PERIOD END
%token NU_DECI II
%token <text> IDENTIFIER
%token <num> NUMBER

%type <node> assignment numberLiteral statement declaration

%start st

%%

st:
    statements
    ;

statements:
  | statements statement
    { addToArray(rootNode->payload, $2); }
    ;

statement:
    assignment
    { $$ = $1; }
  | declaration
    { $$ = $1; }
    ;

assignment:
    IDENTIFIER II numberLiteral PERIOD
    { $$ = createAstAssignment($1, $3); }
    ;

declaration:
    NU_DECI IDENTIFIER II numberLiteral PERIOD
    { $$ = createAstDeclaration($2, $4); }
    ;

numberLiteral:
    NUMBER
    { $$ = createAstNumLiteral($1); }
    ;

%%

int yyerror(char* err) {
    fprintf(stderr, "at %d: %s near `%s`\n", yylineno, err, yytext);
    return 1;
}