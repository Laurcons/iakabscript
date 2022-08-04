#include "issi.h"
#include "iakab.tab.h"
#include "tokens.tab.h"
extern FILE* yyin;

int main(int argc, char** argv) {
    astInit();
    symt_init();

    if (argc < 2)
        stopHard("Required file to run: issi [filename]\n");
    
    yyin = fopen(argv[1], "r");

    if (argc == 3 && strcmp(argv[2], "tok") == 0) {
        int tok;
        while ((tok = yylex()) != 0) {
            printf("%s\n", tokentostr(tok));
        }
        return 0;
    }

    dbgprintf("-- Parsing code\n");
    yyparse();
    dbgprintf("-- Parsing symbols\n");
    initialVisitAst();
    dbgprintf("-- Executing code\n");
    visitAst();
}