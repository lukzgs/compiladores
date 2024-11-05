/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"

int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;

%}

%{
    extern int yylineno;
%}

%union {
  ValorLexico *valor_lexico;
  Nodo *nodo;
}

%define parse.error verbose


%token<valor_lexico> TK_PR_INT
%token<valor_lexico> TK_PR_FLOAT
%token<valor_lexico> TK_PR_IF
%token<valor_lexico> TK_PR_ELSE
%token<valor_lexico> TK_PR_WHILE
%token<valor_lexico> TK_PR_RETURN
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_OC_AND
%token<valor_lexico> TK_OC_OR
%token<valor_lexico> TK_IDENTIFICADOR
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT

%token<valor_lexico> '='
%token<valor_lexico> '<'
%token<valor_lexico> '>'
%token<valor_lexico> '+'
%token<valor_lexico> '*'
%token<valor_lexico> '/'
%token<valor_lexico> '%'
%token<valor_lexico> '!'
%token<valor_lexico> '-'

%type<nodo> programa
%type<nodo> lista_funcao
%type<nodo> funcao
%type<nodo> lista_parametros
%type<nodo> cabecalho
%type<nodo> corpo
%type<nodo> tipo

%type<nodo> else_opcional
%type<nodo> declaracao_variavel
%type<nodo> lista_parametros_ou_vazio


// até aqui tá garantido

%type<nodo> lista_comandos
%type<nodo> comando_simples
%type<nodo> declaracao_local
%type<nodo> lista_identificadores
%type<nodo> identificador_local
%type<nodo> bloco_comandos
%type<nodo> atribuicao
%type<nodo> chamada_funcao
%type<nodo> lista_expressoes
%type<nodo> retorno
%type<nodo> clausula_if_com_else_opcional
%type<nodo> iterativo
%type<nodo> expressao
%type<nodo> expressao2
%type<nodo> expressao3
%type<nodo> expressao4
%type<nodo> expressao5
%type<nodo> expressao6
%type<nodo> expressao7


%type<valor_lexico> literal

%token TK_ERRO

%start programa;

%%

/* programa */
programa: 
	lista_funcao { 
		$$ = $1; 
		arvore = $$;
	}; |
	/* vazio */ { $$ = NULL; }; 

lista_funcao: 
	funcao lista_funcao | 
	funcao { $$ = $1; } ;


/* funcao */
funcao: cabecalho corpo ; 

cabecalho: TK_IDENTIFICADOR '=' lista_parametros_ou_vazio '>' tipo ; 
corpo: bloco_comandos ; 

tipo: TK_PR_INT | TK_PR_FLOAT ;	
lista_parametros_ou_vazio: 
	lista_parametros | 
	/* vazio */ { $$ = NULL; } ; 
lista_parametros: parametro TK_OC_OR lista_parametros | parametro ; 
parametro: TK_IDENTIFICADOR '<' '-' tipo ; 

bloco_comandos:	'{' lista_comandos '}' { $$ = $2; };

lista_comandos:	
	comando_simples lista_comandos |
	/* vazio */ { $$ = NULL; } ;


/* comandos */
comando_simples:
	declaracao_variavel ';' { $$ = $1; } |
	atribuicao ';' { $$ = $1; } |
	fluxo_controle ';' { $$ = $1; } |
	retorno ';' { $$ = $1; } |
	bloco_comandos ';' { $$ = $1; } |
	chamada_funcao ';' { $$ = $1; } ;


/* declaração de var */
declaracao_variavel:	tipo lista_identificadores { $$ = $2; } ; 
lista_identificadores: variavel | variavel ',' lista_identificadores ; 
variavel: TK_IDENTIFICADOR | TK_IDENTIFICADOR TK_OC_LE literal ; 

literal: 
	TK_LIT_INT { $$ = $1; } |
	TK_LIT_FLOAT { $$ = $1; } ; 


/* atribuição */
atribuicao:	TK_IDENTIFICADOR '=' expressao ; 


/* chamada de função */
chamada_funcao: TK_IDENTIFICADOR '(' lista_expressoes ')'; 
lista_expressoes:	expressao | expressao ',' lista_expressoes ;


/* controle de fluxo */
fluxo_controle: condicional_if | iterativo ; 


/* retorno */
retorno:	TK_PR_RETURN expressao ; 


/* condicional */
condicional_if: TK_PR_IF '(' expressao ')' bloco_comandos else_opcional ; 
else_opcional: 
	TK_PR_ELSE bloco_comandos |
	/* vazio */  { $$ = NULL; } ; 


/* iteração */
iterativo: TK_PR_WHILE '(' expressao ')' bloco_comandos ;


/* expressões */
expressao: expressao_or
expressao_or:	expressao_and | expressao_or TK_OC_OR expressao_and;
expressao_and:	expressao_igualdade | expressao_and TK_OC_AND expressao_igualdade;

operadores_igualdade: TK_OC_EQ | TK_OC_NE; 
expressao_igualdade:	expressao_comparacao | expressao_igualdade operadores_igualdade expressao_comparacao;

operadores_comparacao: '>' | '<' |  TK_OC_LE | TK_OC_GE; 
expressao_comparacao:	expressao_soma | expressao_comparacao operadores_comparacao expressao_soma;

operadores_soma: '+' | '-';
expressao_soma:	expressao_multiplicacao | expressao_soma operadores_soma expressao_multiplicacao;

operadores_multiplicacao: '*' | '/' | '%'; 
expressao_multiplicacao:	expressao_unarias | expressao_multiplicacao operadores_multiplicacao expressao_unarias;

operadores_unarios: '!' | '-';
expressao_unarias:  expressao_paranteses | operadores | operadores_unarios expressao_multiplicacao;

expressao_paranteses: '(' expressao ')'; 
operadores:	TK_IDENTIFICADOR | literal | chamada_funcao ;


%%

void yyerror(char const *s) {
	extern int yylineno;
	printf("ERRO - LINHA %d - %s\n", yylineno, s);	
}
