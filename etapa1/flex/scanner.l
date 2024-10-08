
/* Lucas Guaitanelli da Silveira - 208695 
   Vithor Barros Pileco - 326674 */

%{
#include "tokens.h"
%}

%option yylineno
%option noyywrap

%%

\/\/.* { }
[[:space:]]+   { }

"int" {return TK_PR_INT; }
"float" { return TK_PR_FLOAT; }
"if" { return TK_PR_IF; }
"else" { return TK_PR_ELSE; }
"while" { return TK_PR_WHILE; }
"return" { return TK_PR_RETURN; }

"<=" { return TK_OC_LE; }
">=" { return TK_OC_GE; }
"==" { return TK_OC_EQ; }
"!=" { return TK_OC_NE; }
"&" { return TK_OC_AND; }
"|" { return TK_OC_OR; }



"!"|"*"|"/"|"%"|"+"|"-"|"<"|">"|"{"|"}"|"("|")"|"="|","|";" { return yytext[0]; }

([a-z]|_)([a-z]|[0-9]|_)* 	{ return TK_IDENTIFICADOR; }

[0-9]+ { return TK_LIT_INT; }
[0-9]*\.[0-9]+ { return TK_LIT_FLOAT; }


. { return TK_ERRO; }

%%

int get_line_number(void) {
    return yylineno;
}
