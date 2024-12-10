/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#ifndef _TABLE_SYMBOL_H_
#define _TABLE_SYMBOL_H_

#include "errors.h"
#include <string.h>
#include <stdlib.h>
#include "iloc.h"
#include <stdio.h>
#define ARQUIVO_SAIDA "saida.dot"

typedef enum symbol_kind {
  FUNCTION,
  VARIABLE
} symbol_kind;

typedef enum symbol_type {
  NULL_TYPE,
  FLOAT,
  INT
} symbol_type;

typedef struct row_symbol {
  int line;
  symbol_kind kind;
  symbol_type type;
  char* value; 
  char *temp; 
  char *shift; 
  struct row_symbol *next_row;
  struct row_symbol *previous_row;
} row_symbol;

typedef struct table_symbol {
  row_symbol* first_row;
  row_symbol* last_row;
  int shift; 
  struct table_symbol* next_table;
  struct table_symbol* previous_table;
} table_symbol;


/*
 * Preenche as linhas da tabela com tipo nulo com o tipo informado
 */
void table_fill_type(table_symbol * table, symbol_type type);

/*
 * Pega a primeira tabela da pilha
 */
table_symbol *get_first_table(table_symbol * table);

/*
 * Função new_row, cria uma nova linha
 */
row_symbol *new_row(int line, symbol_type type, symbol_kind kind, char *value);

/*
 * Função table_new, cria uma nova tabela
 */
table_symbol *table_new();

/*
 * Função table_free, libera a tabela atual. Dá erro se ela tiver filho. Retorna a tabela anterior a tabela que foi liberada.
 */
table_symbol * table_free(table_symbol *row);

/*
 * Função table_add_row, adiciona uma row na tabela
 */
void table_add_row(table_symbol *table, row_symbol *next_row);

/*
 * Função table_add_table, adiciona uma tabela na pilha, retorna a ultima tabela
 */
table_symbol* table_add_table(table_symbol *table, table_symbol *next_table);

/*
 * Função table_print, imprime recursivamente a tabela.
 */
void table_print(const table_symbol *tree);

/*
 * Checa se o identificador já está definido no escopo
 */
int is_identifier_declared(const table_symbol * table, const char * identifier);

/*
 * Checa se o identificador já está definido na pilha de escopos
 */
int does_identifier_exist(const table_symbol * current_table, const char * identifier);

/*
 * Retorna a row  associada ao identificador passado no escopo atual, retorna NULL caso não exista
 */
row_symbol * get_row_from_scope(const table_symbol * table, const char * identifier);

/*
 * Retorna a row  associada ao identificador passado em todos os escopos, retorna NULL caso não exista
 */
row_symbol * get_row_from_stack(const table_symbol * table, const char * identifier);

/*
 * Transforma enum de symbol_kind em uma string
 */
const char* get_str_symbol_kind(symbol_kind kind);


/*
 * Verifica se o identificador existe e está com o tipo correto
 */
void verify_identifier(const table_symbol * current_table, const char * current_identifier, symbol_kind desired_kind, int yylineno);

/*
 * Verifica se a declaração do identificador está correta
 */
void verify_declaration_identifier(const table_symbol * current_table, const char * current_identifier, int yylineno);


#endif