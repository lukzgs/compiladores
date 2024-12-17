/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table_sym.h"

int yylex(void);
void yyerror (char const *mensagem);
extern void *arvore;
extern table_symbol *current_table; 
%}

%{
  extern int yylineno;
%}

%code requires{
  #include "table_sym.h"
  #include "asd.h"
  #include "errors.h"
}

%union {
  struct valor_token * valor_lexico;
  asd_tree_t *nodo;
  symbol_type info_type; 
}

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
%token TK_ERRO
%token<valor_lexico> TK_IDENTIFICADOR
%token<valor_lexico> TK_LIT_INT
%token<valor_lexico> TK_LIT_FLOAT

%type<nodo> programa
%type<nodo> lista_funcao
%type<nodo> funcao
%type<nodo> cabecalho
%type<nodo> corpo
%type<nodo> bloco_comandos
%type<nodo> bloco_comandos_funcao
%type<nodo> lista_comandos
%type<nodo> comando_simples
%type<nodo> comandos_unica_linha
%type<nodo> declaracao_variavel
%type<nodo> lista_identificadores
%type<nodo> variavel
%type<nodo> literal
%type<info_type> tipo
%type<nodo> atribuicao
%type<nodo> chamada_funcao
%type<nodo> lista_expressoes
%type<nodo> fluxo_controle
%type<nodo> retorno
%type<nodo> condicional_if
%type<nodo> else_opcional
%type<nodo> iterativo
%type<nodo> expressao
%type<nodo> expressao_or
%type<nodo> expressao_and
%type<nodo> expressao_igualdade
%type<nodo> expressao_comparacao
%type<nodo> expressao_soma 
%type<nodo> expressao_multiplicacao
%type<nodo> expressao_unarias
%type<nodo> expressao_paranteses
%type<nodo> operandos
%type<nodo> operadores_unarios
%type<nodo> operadores_multiplicacao
%type<nodo> operadores_soma
%type<nodo> operadores_comparacao
%type<nodo> operadores_igualdade
%type<nodo> identificador


%start programa;

%%

/* programa */
programa:
  lista_funcao { $$ = $1; arvore = $$; } |
  /* vazio */ { $$ = NULL; arvore=$$; };

lista_funcao:
  funcao lista_funcao {
    $$ = $1;
    asd_add_child($$, $2);
  } |
  funcao { $$ = $1; };


/* funcao */
funcao: cabecalho corpo {
  if ($2 != NULL)
    asd_add_child($1, $2);
};

cabecalho: 
  identificador '=' empilha_tabela lista_parametros_ou_vazio '>' tipo { 
    verify_declaration_identifier(current_table, $1->token->valor, yylineno);
    $$ = $1;
    table_add_row(
      get_first_table(current_table), new_row($1->token->line, $6 , FUNCTION, $1->label)
    );
   };
corpo:
  bloco_comandos_funcao desempilha_tabela { $$ = $1; };

tipo:
  TK_PR_INT  { $$ = INT; } |
  TK_PR_FLOAT { $$ = FLOAT; };
lista_parametros_ou_vazio:
  lista_parametros |
  /* vazio */;
lista_parametros:
  parametro TK_OC_OR lista_parametros |
  parametro;
parametro:
  identificador '<' '-' tipo {
    verify_declaration_identifier(current_table, $1->token->valor, yylineno);
    table_add_row(
      current_table, new_row($1->token->line, $4, VARIABLE, $1->label)
    );
  };

bloco_comandos_funcao:
  '{' lista_comandos '}' { $$ = $2; };

bloco_comandos:
  empilha_tabela '{' lista_comandos '}' desempilha_tabela { $$ = $3; };

empilha_tabela: {
  current_table = table_add_table(current_table, table_new());
}; 
desempilha_tabela: {
  current_table = table_free(current_table);
};


lista_comandos:
  comando_simples lista_comandos {
    if ($1 != NULL) {
      $$ = $1; 
      if ($2 != NULL)
        asd_add_child($$, $2);
    } 
    else $$ = $2;
  }; |
  comandos_unica_linha ';' lista_comandos  {
    if ($1 != NULL) {
      $$ = $1;
      if ($3 != NULL)
        asd_add_child($$->last_node, $3); 
    } else {
      $$ = $3;
    }
  }; |
  /* vazio */ { $$ = NULL; };



/* comandos */
comando_simples:
  atribuicao ';' { $$ = $1; } |
  fluxo_controle ';' { $$ = $1; } |
  retorno ';' { $$ = $1; } |
  bloco_comandos ';' { $$ = $1; } |
  chamada_funcao ';' { $$ = $1; } ;


comandos_unica_linha:
  declaracao_variavel
  /* Para expansão futura */
  
/* declaração de var */
declaracao_variavel:
  tipo lista_identificadores {
    $$ = $2; 
    table_fill_type(current_table, $1);
  };
lista_identificadores:
  variavel {
    $$ = $1;
    if ($1 != NULL)
      $1->last_node = $1;
  } |
  variavel ',' lista_identificadores { 
    if($1 != NULL) {
      $$ = $1;
      if ($3 != NULL) {
        asd_add_child($$, $3);
        $1->last_node = $3->last_node;
      } else { 
        $1->last_node = $1;
      }
    }
    else $$ = $3;
  };
variavel: 
  identificador {
    verify_declaration_identifier(current_table, $1->token->valor, yylineno);
    $$ = NULL; 
    table_add_row(
      current_table,
      new_row($1->token->line, NULL_TYPE, VARIABLE, $1->label)
    ); 
  } |
  identificador TK_OC_LE literal {
    verify_declaration_identifier(current_table, $1->token->valor, yylineno);
    $$ = asd_new("<="); 
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    table_add_row(
      current_table,
      new_row($1->token->line, NULL_TYPE, VARIABLE, $1->label)
    );
  };


/* atribuição */
atribuicao:
  identificador '=' expressao {
    verify_identifier(current_table, $1->token->valor, VARIABLE, yylineno); 
    $$ = asd_new("=");
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    row_symbol * row = get_row_from_stack(current_table, $1->token->valor);
    $$->type = row->type; 
    $$->code = add_iloc_operation($$->code, new_iloc_operation("loadI", row->shift, row->temp, NULL));
    $$->code = add_iloc_operation($$->code, new_iloc_operation("storeAO", $3->temp, row->temp, "rfp")); 
  };


/* chamada de função */
chamada_funcao:
  identificador '(' lista_expressoes ')' {
    verify_identifier(current_table, $1->token->valor, FUNCTION, yylineno); 
    int len = strlen($1->label);
    char name[len + 5];
    strcpy(name, "call");
    strcat(name, $1->label);
    $$ = asd_new(name);
    asd_add_child($$, $3);
  };
lista_expressoes:
  expressao  { $$ = $1; } |
  expressao ',' lista_expressoes { 
    $$ = $1;
    asd_add_child($$, $3);
  };


/* controle de fluxo */
fluxo_controle:
  condicional_if { $$ = $1; } |
  iterativo { $$ = $1; };


/* retorno */
retorno:
  TK_PR_RETURN expressao {
    $$ = asd_new("return");
    asd_add_child($$, $2);
  };


/* condicional */
condicional_if:
  TK_PR_IF '(' expressao ')' bloco_comandos else_opcional {
    $$ = asd_new("if");
    asd_add_child($$, $3);
    char *l1 = generate_label(), *l2 = generate_label(), *l3 = generate_label();
    add_iloc_operation($$->code, new_iloc_operation("cbr", $3->temp, l1, l2)); 
    add_iloc_operation($$->code, new_iloc_operation("label", l1, NULL, NULL)); 
    if($5 != NULL){
      asd_add_child($$, $5); 
    }
    add_iloc_operation($$->code, new_iloc_operation("jumpI", l3, NULL, NULL)); 
    add_iloc_operation($$->code, new_iloc_operation("label", l2, NULL, NULL)); 
      if($6 != NULL){
        asd_add_child($$, $6);
      }
    add_iloc_operation($$->code, new_iloc_operation("label", l3, NULL, NULL)); 


  }; 
else_opcional: 
  TK_PR_ELSE bloco_comandos { $$ = $2; } |
  /* vazio */ { $$ = NULL; };


/* iteração */
iterativo:
  TK_PR_WHILE '(' expressao ')' bloco_comandos {
    $$ = asd_new("while");
    char *l1 = generate_label(), *l2 = generate_label(), *l3 = generate_label();
    $$->code = add_iloc_operation(create_iloc_list(), new_iloc_operation("label", l3, NULL, NULL)); 
    asd_add_child($$, $3);
    add_iloc_operation($$->code, new_iloc_operation("cbr", $3->temp, l1, l2)); 
    add_iloc_operation($$->code, new_iloc_operation("label", l1, NULL, NULL)); 
    if($5 != NULL) {
      asd_add_child($$, $5);
    }
    add_iloc_operation($$->code, new_iloc_operation("jumpI", l3, NULL, NULL)); 
    add_iloc_operation($$->code, new_iloc_operation("label", l2, NULL, NULL)); 
  };


/* expressões */
expressao:
  expressao_or { $$ = $1; };
expressao_or:
  expressao_and { $$ = $1; } |
  expressao_or TK_OC_OR expressao_and {
    $$ = asd_new("|");
    $$->type = infer_type($1->type, $3->type);
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    generate_expression_code($$, $1->temp, $3->temp, true); 
  };

expressao_and:
  expressao_igualdade { $$ = $1; } |
  expressao_and TK_OC_AND expressao_igualdade {
    $$ = asd_new("&");
    $$->type = infer_type($1->type, $3->type);
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    generate_expression_code($$, $1->temp, $3->temp, true); 
  };

operadores_igualdade:
  TK_OC_EQ { $$ = asd_new("=="); } |
  TK_OC_NE { $$ = asd_new("!="); };
expressao_igualdade:
  expressao_comparacao { $$ = $1; } |
  expressao_igualdade operadores_igualdade expressao_comparacao {
    $$ = $2;
    $$->type = infer_type($1->type, $3->type);
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    generate_expression_code($2, $1->temp, $3->temp, true); 
  };

operadores_comparacao:
  '>' { $$ = asd_new(">"); } |
  '<' { $$ = asd_new("<"); } |
  TK_OC_LE { $$ = asd_new("<="); } |
  TK_OC_GE { $$ = asd_new(">="); };
expressao_comparacao:
  expressao_soma {$$ = $1;} |
  expressao_comparacao operadores_comparacao expressao_soma {
    $$ = $2;
    $$->type = infer_type($1->type, $3->type); 
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    generate_expression_code($2, $1->temp, $3->temp, true); 
  };

operadores_soma:
  '+' { $$ = asd_new("+"); } |
  '-' { $$ = asd_new("-"); };
expressao_soma:
  expressao_multiplicacao { $$ = $1; } |
  expressao_soma operadores_soma expressao_multiplicacao {
    $$ = $2;
    $$->type = infer_type($1->type, $3->type);
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    generate_expression_code($2, $1->temp, $3->temp, true); 
  };

operadores_multiplicacao:
  '*' { $$ = asd_new("*"); } |
  '/' { $$ = asd_new("/"); } |
  '%' { $$ = asd_new("%"); }; //  gerar codigo e local nullos
expressao_multiplicacao:
  expressao_unarias { $$ = $1; } |
  expressao_multiplicacao operadores_multiplicacao expressao_unarias {
    $$ = $2;
    $$->type = infer_type($1->type, $3->type);
    asd_add_child($$, $1);
    asd_add_child($$, $3);
    generate_expression_code($2, $1->temp, $3->temp, true); 
  };

operadores_unarios:
  '!' { $$ = asd_new("!"); } |  
  '-' { $$ = asd_new("-"); }; 
expressao_unarias:
  expressao_paranteses { $$ = $1; } |
  operandos { $$ = $1; } |
  operadores_unarios expressao_unarias {
    $$ = $1;
    $$->type = $2->type;
    asd_add_child($$, $2);
    char * temp = generate_temp();
    add_iloc_operation($$->code, new_iloc_operation("loadI", get_unary_constant($1->label), temp, NULL)); 
    generate_expression_code($$, $2->temp, temp, false);
  };

expressao_paranteses:
  '(' expressao ')' { $$ = $2; };
operandos:
  identificador { 
    verify_identifier(current_table, $1->token->valor, VARIABLE, yylineno); 
    $$ = $1;
    row_symbol * row = get_row_from_stack(current_table, $1->token->valor);
    $$->type = row->type;
    $$->temp = generate_temp(); 
    $$->code = add_iloc_operation(create_iloc_list(), new_iloc_operation("loadAO", row->temp, "rfp", $$->temp)); 
  } |
  literal { 
    $$ = $1;
    $$->temp = generate_temp();
    $$->code = add_iloc_operation(create_iloc_list(), new_iloc_operation("loadI", $$->token->valor, $$->temp, NULL)); 
  } |
  chamada_funcao { $$ = $1; };

identificador:
  TK_IDENTIFICADOR {
    $$ = asd_new_token($1->valor, $1);
  };

literal: 
  TK_LIT_INT { 
    $$ = asd_new_token($1->valor, $1);
    $$->type = INT;
  } |
  TK_LIT_FLOAT { 
    $$ = asd_new_token($1->valor, $1);
    $$->type = FLOAT; 
  };

%%

void yyerror(char const *s) {
	extern int yylineno;
	printf("ERRO - LINHA %d - %s\n", yylineno, s);
}