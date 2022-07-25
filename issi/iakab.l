
%option caseless

%{
    #include "issi.h"
    #include "iakab.tab.h"
    int yyparse();

    int yyline = 1;
    int yycol = 0;

    // custom parsing functions
    int parseNBLit();
%}

%%

\<3.*\n { /* comment */ }

[\n\.] { return PERIOD; }
<<EOF>> { return END; }
[ \t]+ { /* ignore spaces and tabs */ }

"nu deci" { return NU_DECI; }
"ii" { return II; }

g+ { yylval.num = strlen(yytext); return NUMBER; }
ez+ { int x = 1; for (int i = 0; i < strlen(yytext) - 1; i++) x *= 10; yylval.num = x; return NUMBER; }
eez+ { double x = 1; for (int i = 0; i < strlen(yytext) - 2; i++) x /= 10; yylval.num = x; return NUMBER; }
[nb]+ { return parseNBLit(); }
[a-z]+ {
    yylval.text = strdup(yytext);
    for (int i = 0; i < strlen(yytext); i++)
        yytext[i] = tolower(yytext[i]);
    return IDENTIFIER;
}

%%

int parseNBLit() {
    int p = 1, res = 0;
    for (int i = strlen(yytext) - 1; i >= 0; i--) {
        if (yytext[i] == 'n')
            res += p;
        p *= 2;
    }
    yylval.num = res;
    return NUMBER;
}