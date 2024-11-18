/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#ifndef _TABLE_SYMBOL_H_
#define _TABLE_SYMBOL_H_

typedef enum symbol_kind {
    FUNCTION, 
    IDENTIFIER
} symbol_kind;

typedef enum symbol_type {
    FLOAT, 
    INT 
} symbol_type; 


typedef struct row_symbol {
    int line;
    symbol_kind kind;
    symbol_type type;
    char* value; 
    struct row_symbol *next_row;
    struct row_symbol *previous_row; 
} row_symbol; 


typedef struct table_symbol {
    row_symbol* first_row; 
    row_symbol* last_row; 
    struct table_symbol* next_table;
} table_symbol; 


/*
 * Função new_row, cria uma nova linha 
 */
row_symbol *new_row(int line, symbol_type type, symbol_kind kind, char *value);

/*
 * Função new_table, cria uma nova tabela 
 */
table_symbol *new_table(row_symbol *first_row);

/*
 * Função table_free, libera recursivamente a tabela 
 */
void table_free(table_symbol *row);

/*
 * Função table_add_row, adiciona uma row na tabela
 */
void table_add_row(table_symbol *table, row_symbol *next_row);

/*
 * Função table_add_row, adiciona uma row na tabela
 */
void table_add_table(table_symbol *table, table_symbol *next_table);


/*
 * Função table_print, imprime recursivamente a tabela.
 */
void table_print(table_symbol *tree);

#endif 

