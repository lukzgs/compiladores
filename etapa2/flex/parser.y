/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror (char const *mensagem);

%}

%define parse.error verbose
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_ERRO

%define parse.error detailed;
%start programa;

%%

programa: linguagem ; // ok

linguagem: lista_funcao | /* vazio */ ; // ok
lista_funcao: funcao lista_funcao | /* vazio */ ; // ok

funcao: cabecalho corpo ; // ok

cabecalho: TK_IDENTIFICADOR '=' lista_parametros '>' tipo ; // ok
corpo: '{' bloco_comandos '}' ; // ok

tipo: TK_PR_INT | TK_PR_FLOAT ;	// ok
lista_parametros:	parametro TK_OC_OR lista_parametros | parametro | /* vazio */ ; // ok
parametro: TK_IDENTIFICADOR '<' '-' tipo ; // ok

bloco_comandos:	lista_comandos; // ok

lista_comandos:	comando_simples lista_comandos | /* vazio */ ; // ok

comando_simples:	variavel ';' | atribuicao ';' |  fluxo_controle ';' | retorno ';' | bloco_comandos ';' | chamada_funcao ';' /* | condicional_if condicional_else ';' | iterativo ';' */ ;

variavel:	tipo TK_IDENTIFICADOR | tipo lista_identificadores | tipo TK_IDENTIFICADOR TK_OC_LE literal; // acho que ok
lista_identificadores: TK_IDENTIFICADOR ',' lista_identificadores | TK_IDENTIFICADOR TK_OC_LE literal lista_identificadores | TK_IDENTIFICADOR ; // acho que ok

atribuicao:	TK_IDENTIFICADOR '=' exp; // ok

fluxo_controle : ;
retorno:	TK_PR_RETURN exp; // ok
chamada_funcao:	TK_IDENTIFICADOR '(' lista_expressoes ')' | TK_IDENTIFICADOR '(' ')'; // ok
lista_expressoes:	exp | exp ',' lista_expressoes; // ok


condicional_if:	TK_PR_IF '(' exp ')' bloco_comandos;
condicional_else:	TK_PR_ELSE bloco_comandos | /* vazio */;

iterativo:	TK_PR_WHILE '(' exp ')' bloco_comandos;

exp:	exp2 | exp TK_OC_OR exp2;
exp2:	exp3 | exp2 TK_OC_AND exp3;
exp3:	exp4 | exp3 TK_OC_EQ exp4 | exp3 TK_OC_NE exp4;
exp4:	exp5 | exp4 '<' exp5 | exp4 '>' exp5 | exp4 TK_OC_LE exp5 | exp4 TK_OC_GE exp5;
exp5:	exp6 | exp5 '+' exp6 | exp5 '-' exp6;
exp6:	exp7 | exp6 '*' exp7 | exp6 '/' exp7 | exp6 '%' exp7;
exp7:	'(' exp ')' | '!' exp7 | '-' exp7 | TK_IDENTIFICADOR | literal;

literal:	TK_LIT_INT | TK_LIT_FLOAT ; // ok

%%

void yyerror(char const *s)
{
	extern int yylineno;
	printf("ERRO - LINHA %d - %s\n", yylineno, s);	
}
%%