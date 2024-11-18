/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "table_sym.h"
#define ARQUIVO_SAIDA "saida.dot"

row_symbol *new_row(int line, symbol_type type, symbol_kind kind, char *value)
{
  row_symbol *ret = NULL;
  ret = calloc(1, sizeof(row_symbol));
  if (ret != NULL){
    ret->value = strdup(value);
    ret->line = line;
    ret->next_row = NULL;
    ret->previous_row = NULL;
    ret->type = type;
    ret->kind = kind; 
  }
  return ret;
}

table_symbol *new_table(){
  table_symbol *ret = NULL;
  ret = calloc(1, sizeof(table_symbol));
  if (ret != NULL){
      ret->first_row = NULL;
      ret->last_row = NULL;
      ret->next_table = NULL;
  }
  return ret; 
}

void table_free(table_symbol *table){
  if (table != NULL){
    table_free(table->next_table); 
    row_symbol *row = table->first_row; 
    while (row != NULL){
      row_symbol* next = row->next_row; 
      free(row->value); 
      free(row); 
      row = next; 
    }
    free(table); 
  }
}

void table_add_row(table_symbol *table, row_symbol *next_row)
{
  if (table != NULL && next_row != NULL){
    if (table->last_row != NULL) {
      table->last_row->next_row = next_row;
      next_row->previous_row = table->last_row; 
      table->last_row = next_row; 
    } else {
      table->last_row = next_row; 
      table->first_row = next_row; 
    }
  }else{
    printf("Erro: %s recebeu parâmetro table = %p / %p.\n", __FUNCTION__, table, next_row);
  }
}



void table_add_table(table_symbol *table, table_symbol *next_table){
  if (table != NULL) {
    while (table->next_table != NULL){
      table = table->next_table; 
    }
    table->next_table = next_table; 
  } else {
    printf("Erro: %s recebeu parâmetro table = %p.\n", __FUNCTION__, table);
  }
}

void table_print(table_symbol *table)
{
  printf("line | kind | type | value\n");
  if (table != NULL) {
    row_symbol* row = table->first_row; 
    while (row != NULL){
      printf("%04d | %04d | %04d | %s", row->line, row->kind, row->type, row->value); 
      row = row->next_row; 
    }
  }else{
    printf("Erro: %s recebeu parâmetro table = %p.\n", __FUNCTION__, table);
  }
}

