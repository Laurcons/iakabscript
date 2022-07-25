#include "issi.h"
#include "iakab.tab.h"

void _visitAst(ast_node);

int main(int argc, char** argv) {
    astInit();
    symtableInit();

    if (argc == 2 && strcmp(argv[1], "tok") == 0) {
        int tok;
        while ((tok = yylex()) != END) {
            printf("%d\n", tok);
        }
        return 0;
    }

    yyparse();
    _visitAst(rootNode);
}