/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#ifndef _ARVORE_H_
#define _ARVORE_H_

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
} asd_tree_t;


/* Função asd_new, cria um nó sem filhos com o label informado.  */
asd_tree_t *asd_new(const char *label);

/* Função asd_new, cria um nó sem filhos com o label informado e o valor_token armazenado no nodo. */
asd_tree_t *asd_new_token(const char *label, struct valor_token * token);

/* Função asd_tree, libera recursivamente o nó e seus filhos. */
void asd_free(asd_tree_t *tree);

/* Função asd_add_child, adiciona child como filho de tree. */
void asd_add_child(asd_tree_t *tree, asd_tree_t *child);

/* Função asd_print, imprime recursivamente a árvore. */
void asd_print(asd_tree_t *tree);

/* Função asd_print_graphviz, idem, em formato DOT  */
void asd_print_graphviz (asd_tree_t *tree);

/* Função asd_print_export, imprime recursivamente a árvore no formato especificado.  */
void asd_print_export(asd_tree_t *tree);


#endif //_ARVORE_H_
