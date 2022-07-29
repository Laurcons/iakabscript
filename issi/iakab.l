
%option caseless

%{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
    #include "issi.h"
    #include "iakab.tab.h"
    int yyparse();

    int yyline = 1;
    int yycol = 0;

    // custom parsing functions
    int parseNBLit();
%}

%%

 /* on every token that consumes a NL, add a yyline++ */
\<3.*\n { /* comment, but treat is as a PERIOD */ yyline++; return PERIOD; }
sta+i(.*)\n { /* 'stai' line break, ignore it and continue sentence parsing */ yyline++; }

[\n\.] { yyline++; return PERIOD; }
[ \t]+ { /* ignore spaces and tabs */ }

 /* expressions and words */
"nu deci" { return NU_DECI; }
"nu hoho deci" { return NU_HOHO_DECI; }
"hohoh" { return HOHOH; }
"hoho" { return HOHO; }
"hoh" { return HOH; }
"ia" { return IA; }
"si" { return SI; }
"nimic" { return NIMIC; }
"ii" { return II; }
"nui" { return NUI; }
"fa" { return FA; }
"gata" { return GATA; }

 /* literals */
g+ { yylval.num = strlen(yytext); return NUMBERLIT; }
ez+ { int x = 1; for (int i = 0; i < strlen(yytext) - 1; i++) x *= 10; yylval.num = x; return NUMBERLIT; }
eez+ { double x = 1; for (int i = 0; i < strlen(yytext) - 2; i++) x /= 10; yylval.num = x; return NUMBERLIT; }
[nb]+ { return parseNBLit(); }
\"[^\"]*\" {
    // copy everything but without the quotes
    char* buf = malloc(sizeof(char) * (strlen(yytext) - 1)); // - 2 + 1
    for (int i = 0; i < strlen(yytext) - 2; i++) {
        buf[i] = yytext[i+1];
    }
    yylval.text = buf;
    return STRINGLIT;
}

 /* identifiers */
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
    return NUMBERLIT;
}