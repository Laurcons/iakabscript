#include "issi.h"
#include "iakab.tab.h"
#include "tokens.tab.h"

void _visitAst(ast_node);

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
    if (argc == 2 && strcmp(argv[1], "--debug") == 0) {
        yydebug = 1;
    }

    yyparse();
    _visitAst(rootNode);
}