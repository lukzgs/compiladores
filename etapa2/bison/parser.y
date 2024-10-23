/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yylex(void);
void yyerror (char const *mensagem);

%}

%{
    extern int yylineno;
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

%start programa;

%%

/* programa */
programa: lista_funcao | /* vazio */ ; 

lista_funcao: funcao lista_funcao | funcao ;


/* funcao */
funcao: cabecalho corpo ; 

cabecalho: TK_IDENTIFICADOR '=' lista_parametros_ou_vazio '>' tipo ; 
corpo: bloco_comandos ; 

tipo: TK_PR_INT | TK_PR_FLOAT ;	
lista_parametros_ou_vazio: lista_parametros | /* vazio */ ; 
lista_parametros: parametro TK_OC_OR lista_parametros | parametro ; 
parametro: TK_IDENTIFICADOR '<' '-' tipo ; 

bloco_comandos:	'{' lista_comandos '}' ;

lista_comandos:	comando_simples lista_comandos | /* vazio */ ;


/* comandos */
comando_simples:	declaracao_variavel ';' | atribuicao ';' |  fluxo_controle ';' | retorno ';' | bloco_comandos ';' | chamada_funcao ';' ;


/* declaração de var */
declaracao_variavel:	tipo lista_identificadores ; 
lista_identificadores: variavel | variavel ',' lista_identificadores ; 
variavel: TK_IDENTIFICADOR | TK_IDENTIFICADOR TK_OC_LE literal ; 

literal: TK_LIT_INT | TK_LIT_FLOAT ; 


/* atribuição */
atribuicao:	TK_IDENTIFICADOR '=' exp ; 


/* chamada de função */
chamada_funcao: TK_IDENTIFICADOR '(' lista_expressoes_ou_vazio ')'; 
lista_expressoes_ou_vazio: lista_expressoes | /* vazio */ ; 
lista_expressoes:	exp | exp ',' lista_expressoes ; // comentário feito no forum 


/* controle de fluxo */
fluxo_controle: condicional_if | iterativo ; 


/* retorno */
retorno:	TK_PR_RETURN exp ; 


/* condicional */
condicional_if: TK_PR_IF '(' exp ')' bloco_comandos else_opcional ; 
else_opcional: TK_PR_ELSE bloco_comandos | /* vazio */ ; 


/* iteração */
iterativo: TK_PR_WHILE '(' exp ')' bloco_comandos ;


/* expressões */
exp:	exp2 | exp TK_OC_OR exp2;
exp2:	exp3 | exp2 TK_OC_AND exp3;
exp3:	exp4 | exp3 TK_OC_EQ exp4 | exp3 TK_OC_NE exp4;
exp4:	exp5 | exp4 '<' exp5 | exp4 '>' exp5 | exp4 TK_OC_LE exp5 | exp4 TK_OC_GE exp5;
exp5:	exp6 | exp5 '+' exp6 | exp5 '-' exp6;
exp6:	exp7 | exp6 '*' exp7 | exp6 '/' exp7 | exp6 '%' exp7;
exp7:	'(' exp ')' | '!' exp7 | '-' exp7 | TK_IDENTIFICADOR | literal | chamada_funcao ;


%%

void yyerror(char const *s)
{
	extern int yylineno;
	printf("ERRO - LINHA %d - %s\n", yylineno, s);	
}