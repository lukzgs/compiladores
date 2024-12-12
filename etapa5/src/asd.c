/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "asd.h"
#include "iloc.h"
#define ARQUIVO_SAIDA "saida.dot"

asd_tree_t *asd_new(const char *label) {
  asd_tree_t *ret = NULL;
  ret = calloc(1, sizeof(asd_tree_t));
  if (ret != NULL) {
    ret->label = strdup(label);
    ret->number_of_children = 0;
    ret->children = NULL;
    ret->token = NULL;
    ret->code = NULL;
    ret->temp = NULL; 
  }
  return ret;
}

asd_tree_t *asd_new_token(const char *label, struct valor_token * token) {
  asd_tree_t *ret = NULL;
  ret = calloc(1, sizeof(asd_tree_t));
  if (ret != NULL) {
    ret->label = strdup(label);
    ret->number_of_children = 0;
    ret->children = NULL;
    ret->token = token; 
  }
  return ret;
}

void asd_free(asd_tree_t *tree) {
  if (tree != NULL) {
    int i;
    for (i = 0; i < tree->number_of_children; i++) {
      asd_free(tree->children[i]);
    }
    free(tree->children);
    free(tree->label);
    free(tree->temp);
    free_iloc_op_list(tree->code);
    free(tree->token); 
    free(tree);
  }
}

void asd_add_child(asd_tree_t *tree, asd_tree_t *child) {
  if (tree != NULL && child != NULL) {
    tree->number_of_children++;
    tree->children = realloc(tree->children, tree->number_of_children * sizeof(asd_tree_t*));
    tree->children[tree->number_of_children-1] = child;
    if (child->code != NULL){
      if (tree->code == NULL){
        tree->code = create_iloc_list(); 
      }
      copy_list(tree->code, child->code); 
    }
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p / %p.\n", __FUNCTION__, tree, child);
  }
}

static void _asd_print (FILE *foutput, asd_tree_t *tree, int profundidade) {
  int i;
  if (tree != NULL) {
    fprintf(foutput, "%d%*s: Nó '%s' tem %d filhos:\n", profundidade, profundidade*2, "", tree->label, tree->number_of_children);
    for (i = 0; i < tree->number_of_children; i++) {
      _asd_print(foutput, tree->children[i], profundidade+1);
    }
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_print(asd_tree_t *tree) {
  FILE *foutput = stderr;
  if (tree != NULL) {
    _asd_print(foutput, tree, 0);
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

static void _asd_print_graphviz (FILE *foutput, asd_tree_t *tree) {
  int i;
  if (tree != NULL) {
    fprintf(foutput, "  %ld [ label=\"%s\" ];\n", (long)tree, tree->label);
    for (i = 0; i < tree->number_of_children; i++) {
      fprintf(foutput, "  %ld -> %ld;\n", (long)tree, (long)tree->children[i]);
      _asd_print_graphviz(foutput, tree->children[i]);
    }
  } else {
    printf("Erro: %s recebeu parâmetro tree = %p.\n", __FUNCTION__, tree);
  }
}

void asd_print_graphviz(asd_tree_t *tree) {
  FILE *foutput = fopen(ARQUIVO_SAIDA, "w+");
  if(foutput == NULL) {
    printf("Erro: %s não pude abrir o arquivo [%s] para escrita.\n", __FUNCTION__, ARQUIVO_SAIDA);
  }
  if (tree != NULL) {
    fprintf(foutput, "digraph grafo {\n");
    _asd_print_graphviz(foutput, tree);
    fprintf(foutput, "}\n");
    fclose(foutput);
  }
}


static void _asd_print_export (asd_tree_t *tree) {
  int i;
  if (tree != NULL) {
    printf("  %p [label=\"%s\"];\n", tree, tree->label);
    for (i = 0; i < tree->number_of_children; i++) {
      printf("  %p, %p\n", tree, tree->children[i]);
      _asd_print_export(tree->children[i]);
    }
  }
}

void asd_print_export(asd_tree_t *tree) {
  if (tree != NULL) {
    _asd_print_export(tree);
  }
}



void generate_expression_code(asd_tree_t * operator, char * op1_temp, char * op2_temp, bool is_binary){
    operator->temp = generate_temp();
    char *instruction = is_binary ? select_binary_instruction(operator->label) : select_unary_instruction(operator->label); 
    add_iloc_operation(operator->code, new_iloc_operation(instruction, op1_temp, op2_temp, operator->temp));
}



symbol_type infer_type(symbol_type type1, symbol_type type2) {
  if (type1 == NULL_TYPE || type2 == NULL_TYPE) {
    return NULL_TYPE; 
  }
  if (type1 == FLOAT || type2 == FLOAT)
    return FLOAT; 
  return INT; 
}

