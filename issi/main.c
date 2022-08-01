#include "issi.h"
#include "iakab.tab.h"
#include "tokens.tab.h"

int main(int argc, char** argv) {
    astInit();
    symtableInit();

    if (argc == 2 && strcmp(argv[1], "tok") == 0) {
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