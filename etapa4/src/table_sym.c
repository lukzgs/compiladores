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
  ret = malloc(sizeof(row_symbol));
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

void table_fill_type(table_symbol * table, symbol_type type){
  row_symbol *row = table->first_row;
  while (row != NULL){
    if (row->type == NULL_TYPE) {
      row->type = type;
    } 
    row = row->next_row; 
  }
}

table_symbol *table_new(){
  table_symbol *ret = NULL;
  ret = calloc(1, sizeof(table_symbol));
  if (ret != NULL){
      ret->first_row = NULL;
      ret->last_row = NULL;
      ret->next_table = NULL;
      ret->previous_table = NULL;
  }
  return ret; 
}

table_symbol *table_free(table_symbol *table){
  if (table != NULL){
    if (table->next_table == NULL){
      table_symbol *previous_table = table->previous_table; 
      row_symbol *row = table->first_row; 
      while (row != NULL){
        row_symbol* next = row->next_row; 
        free(row->value); 
        free(row); 
        row = next; 
      }
      free(table); 
      if (previous_table != NULL) previous_table->next_table = NULL; 
      return previous_table; 
    } else {
      printf("Erro: %s recebeu parâmetro table que possui outra table como filho = %p / %p.\n", __FUNCTION__, table, table->next_table);
      exit(1); 
    }
    return NULL; 
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

table_symbol *get_first_table(table_symbol * table){
  while (table->previous_table != NULL){
    table = table->previous_table; 
  }
  return table; 
}

table_symbol* table_add_table(table_symbol *table, table_symbol *next_table){
  if (table != NULL) {
    while (table->next_table != NULL){
      table = table->next_table; 
    }
    table->next_table = next_table; 
    next_table->previous_table = table; 
    return next_table; 
  } else {
    printf("Erro: %s recebeu parâmetro table = %p.\n", __FUNCTION__, table);
    return NULL; 
  }
}

void table_print(table_symbol *table)
{
  printf("line | kind | type | value\n");
  if (table != NULL) {
    row_symbol* row = table->first_row; 
    while (row != NULL){
      printf("%04d | %04d | %04d | %s\n", row->line, row->kind, row->type, row->value); 
      row = row->next_row; 
    }
  }else{
    printf("Erro: %s recebeu parâmetro table = %p.\n", __FUNCTION__, table);
  }
}

int is_identifier_declared(table_symbol * table, char * identifier){
  row_symbol * row = table->first_row; 
  while (row != NULL){
    if (!strcmp(row->value, identifier)){
      return 1; 
    }
    row = row->next_row; 
  }
  return 0; 
}

int does_identifier_exist(table_symbol * current_table, char * identifier){
  while (current_table != NULL){
    if (is_identifier_declared(current_table, identifier)){
      return 1; 
    }
    current_table = current_table->previous_table; 
  }
  return 0; 
}

row_symbol *  get_row_from_stack(table_symbol * current_table, char * identifier){
  while (current_table != NULL){
    row_symbol * row = get_row_from_scope(current_table, identifier); 
    if (row != NULL){
      return row; 
    }
    current_table = current_table->previous_table; 
  }
  return NULL; 
}


row_symbol * get_row_from_scope(table_symbol * table, char * identifier){
    row_symbol * row = table->first_row; 
  while (row != NULL){
    if (!strcmp(row->value, identifier)){
      return row; 
    }
    row = row->next_row; 
  }
  return NULL; 
}

const char* get_str_symbol_kind(symbol_kind kind){
  if (kind == VARIABLE){
    return "VARIABLE"; 
  } else if (kind == FUNCTION){
    return "FUNCTION"; 
  } else {
    fprintf(stderr, "Kind unidentified %d\n", kind);
    exit(1); 
  }
}

