/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#ifndef _ARVORE_H_
#define _ARVORE_H_

#include "iloc.h"
#include "table_sym.h"
#include <stdbool.h>


enum type {
  IDENTIFICADOR = 1,
  LITERAL = 2
};

struct valor_token {
  int line; 
  enum type type;
  char* valor;
};

typedef struct asd_tree {
  char *label;
  int number_of_children;
  struct asd_tree **children;
  struct asd_tree *last_node;
  struct valor_token * token;
  symbol_type type;
  iloc_op_list  *code; 
  char *temp;
} asd_tree_t;


/*
 * Função asd_new, cria um nó sem filhos com o label informado.
 */
asd_tree_t *asd_new(const char *label);

/*
 * Função asd_new, cria um nó sem filhos com o label informado e o valor_token armazenado no nodo.
 */
asd_tree_t *asd_new_token(const char *label, struct valor_token * token);

/*
 * Função asd_tree, libera recursivamente o nó e seus filhos.
 */
void asd_free(asd_tree_t *tree);

/*
 * Função asd_add_child, adiciona child como filho de tree.
 */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/*
 * Função asd_print, imprime recursivamente a árvore.
 */
void asd_print(asd_tree_t *tree);

/*
 * Função asd_print_graphviz, idem, em formato DOT
 */
void asd_print_graphviz (asd_tree_t *tree);

/*
 * Função asd_print_export, imprime recursivamente a árvore no formato especificado. 
 */
void asd_print_export(asd_tree_t *tree);

/*
 * Função que decide qual tipo deve ser retornado dado os 2 tipos dos operandos e as regras de inferência 
 */
symbol_type infer_type(symbol_type type1, symbol_type type2); 


void generate_expression_code(asd_tree_t * operator, char * op1_temp, char * op2_temp, bool is_binary); 

#endif //_ARVORE_H_
