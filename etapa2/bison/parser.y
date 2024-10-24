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
atribuicao:	TK_IDENTIFICADOR '=' expression ; 


/* chamada de função */
chamada_funcao: TK_IDENTIFICADOR '(' lista_expressoes ')'; 
lista_expressoes:	expression | expression ',' lista_expressoes ;


/* controle de fluxo */
fluxo_controle: condicional_if | iterativo ; 


/* retorno */
retorno:	TK_PR_RETURN expression ; 


/* condicional */
condicional_if: TK_PR_IF '(' expression ')' bloco_comandos else_opcional ; 
else_opcional: TK_PR_ELSE bloco_comandos | /* vazio */ ; 


/* iteração */
iterativo: TK_PR_WHILE '(' expression ')' bloco_comandos ;


/* expressões */
expression: expression_or
expression_or:	expression_and | expression_or TK_OC_OR expression_and;
expression_and:	expression_igualdade | expression_and TK_OC_AND expression_igualdade;
operadores_igualdade: TK_OC_EQ | TK_OC_NE; 
expression_igualdade:	expression_comparacao | expression_igualdade operadores_igualdade expression_comparacao;
operadores_comparacao: '>' | '<' |  TK_OC_LE | TK_OC_GE; 
expression_comparacao:	expression_soma | expression_comparacao operadores_comparacao expression_soma;
operadores_soma: '+' | '-';
expression_soma:	expression_multiplicacao | expression_soma operadores_soma expression_multiplicacao;
operadores_multiplicacao: '*' | '/' | '%'; 
expression_multiplicacao:	expression_unarias | expression_multiplicacao operadores_multiplicacao expression_unarias;
operadores_unarios: '!' | '-';
expression_unarias:  expression_paranteses | operadores | operadores_unarios expression_multiplicacao;
expression_paranteses: '(' expression ')'; 
operadores:	TK_IDENTIFICADOR | literal | chamada_funcao ;


%%

void yyerror(char const *s)
{
	extern int yylineno;
	printf("ERRO - LINHA %d - %s\n", yylineno, s);	
}
