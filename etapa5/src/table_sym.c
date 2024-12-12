/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#include "table_sym.h"

row_symbol *new_row(int line, symbol_type type, symbol_kind kind, char *value) {
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

void table_fill_type(table_symbol * table, symbol_type type) {
  row_symbol *row = table->first_row;
  while (row != NULL){
    if (row->type == NULL_TYPE) {
      row->type = type;
    } 
    row = row->next_row;
  }
}

table_symbol *table_new() {
  table_symbol *ret = NULL;
  ret = calloc(1, sizeof(table_symbol));
  if (ret != NULL){
      ret->first_row = NULL;
      ret->last_row = NULL;
      ret->next_table = NULL;
      ret->previous_table = NULL;
      ret->shift = 0;
  }
  return ret; 
}

table_symbol *table_free(table_symbol *table) {
  if (table != NULL) {
    if (table->next_table == NULL) {
      table_symbol *previous_table = table->previous_table;
      row_symbol *row = table->first_row;
      while (row != NULL) {
        row_symbol* next = row->next_row;
        free(row->value); 
        free(row->temp);
        free(row->shift); 
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

void table_add_row(table_symbol *table, row_symbol *next_row) {
  if (table != NULL && next_row != NULL) {
    if (table->last_row != NULL) {
      table->last_row->next_row = next_row;
      next_row->previous_row = table->last_row;
      table->last_row = next_row;
    } else {
      table->last_row = next_row;
      table->first_row = next_row;
    }
    int length = snprintf( NULL, 0, "%d", table->shift );
    next_row->shift = malloc( length + 1 );
    snprintf(next_row->shift, length + 1, "%d", table->shift);
    table->shift += sizeof(int);
    next_row->temp = generate_temp(); 
  } else {
    printf("Erro: %s recebeu parâmetro table = %p / %p.\n", __FUNCTION__, table, next_row);
  }
}

table_symbol *get_first_table(table_symbol * table)  {
  while (table->previous_table != NULL){
    table = table->previous_table;
  }
  return table;
}

table_symbol* table_add_table(table_symbol *table, table_symbol *next_table) {
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

void table_print(const table_symbol *table) {
  printf("line | kind | type | value\n");
  if (table != NULL) {
    row_symbol* row = table->first_row;
    while (row != NULL) {
      printf("%04d | %04d | %04d | %s\n", row->line, row->kind, row->type, row->value);
      row = row->next_row;
    }
  } else {
    printf("Erro: %s recebeu parâmetro table = %p.\n", __FUNCTION__, table);
  }
}

int is_identifier_declared(const table_symbol * table, const char * identifier) {
  row_symbol * row = table->first_row;
  while (row != NULL){
    if (!strcmp(row->value, identifier)) {
      return 1;
    }
    row = row->next_row;
  }
  return 0;
}

int does_identifier_exist(const table_symbol * current_table, const char * identifier) {
  while (current_table != NULL) {
    if (is_identifier_declared(current_table, identifier)) {
      return 1;
    }
    current_table = current_table->previous_table;
  }
  return 0;
}

row_symbol *  get_row_from_stack(const table_symbol * current_table, const char * identifier) {
  while (current_table != NULL) {
    row_symbol * row = get_row_from_scope(current_table, identifier);
    if (row != NULL) {
      return row;
    }
    current_table = current_table->previous_table;
  }
  return NULL; 
}


row_symbol * get_row_from_scope(const table_symbol * table, const char * identifier){
    row_symbol * row = table->first_row;
  while (row != NULL) {
    if (!strcmp(row->value, identifier)) {
      return row;
    }
    row = row->next_row;
  }
  return NULL;
}

const char* get_str_symbol_kind(symbol_kind kind) {
  if (kind == VARIABLE) {
    return "VARIABLE";
  } else if (kind == FUNCTION) {
    return "FUNCTION";
  } else {
    fprintf(stderr, "Kind unidentified %d\n", kind);
    exit(1); 
  }
}

void verify_identifier(const table_symbol * current_table, const char * current_identifier, symbol_kind desired_kind, int yylineno){
  if (!does_identifier_exist(current_table, current_identifier)) {
    fprintf(stderr, "Declaração  do identificador [%s] usado na linha [%d] não encontrada\n", current_identifier, yylineno);
    exit(ERR_UNDECLARED);
  }
  row_symbol * row = get_row_from_stack(current_table, current_identifier);

  if (row->kind != desired_kind) {
    fprintf(stderr, "Identificador [%s] usado na linha [%d] declarado como [%s] mas usado como [%s]\n", current_identifier, yylineno, get_str_symbol_kind(row->kind), get_str_symbol_kind(desired_kind));

    if (row->kind == VARIABLE) {
      exit(ERR_VARIABLE);
    } else if (row->kind == FUNCTION) {
      exit(ERR_FUNCTION);
    }
  }
}

void verify_declaration_identifier(const table_symbol * current_table, const char * current_identifier, int yylineno){
  if (is_identifier_declared(current_table, current_identifier)) {
    fprintf(stderr, "Redeclaração  do identificador [%s] detectada na linha [%d], declaração prévia na linha [%d]\n", current_identifier, yylineno, get_row_from_scope(current_table, current_identifier)->line);
    exit(ERR_DECLARED);
  }
}