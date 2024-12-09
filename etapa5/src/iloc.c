/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */
#ifndef _ILOC_C_
#define _ILOC_C_

#include "iloc.h"


// Cria uma nova operação ILOC
iloc_op* new_iloc_operation(char* operation, char* arg1, char* arg2, char* arg3) {

  iloc_op* op = (iloc_op*)malloc(sizeof(iloc_op));
  if (op == NULL) {
    fprintf(stderr, "Erro na funcao new_iloc_operation() \n");
    fprintf(stderr, "Alocacao de memória falhou. \n");
    exit(EXIT_FAILURE);
  }

  op->mnemonico = strdup(operation);

  if (arg1 != NULL) 
  	op->arg1 = strdup(arg1);
  if (arg2 != NULL)
    op->arg2 = strdup(arg2);
  else  
    op->arg2 = NULL;
  if (arg3 != NULL)
    op->arg3 = strdup(arg3);
  else
    op->arg3 = NULL;
  return op;
}

// Adiciona uma operação à lista de operações ILOC
iloc_op_list* add_iloc_operation(iloc_op_list * list, iloc_op* new_iloc_op) {
  iloc_op_list *iloc_node = (iloc_op_list *)malloc(sizeof(iloc_op_list));
  if (iloc_node == NULL) {
    fprintf(stderr, "Erro na funcao add_iloc_operation() \n");
    fprintf(stderr, "Alocacao de memória falhou. \n");
    exit(EXIT_FAILURE);
  }

  iloc_node->operation = new_iloc_op;
  iloc_node->next_operation = NULL;

  if (list == NULL)
    list = iloc_node;
  else {
    iloc_op_list* current = list;
    while (current->next_operation != NULL)
      current = current->next_operation;
    current->next_operation = iloc_node;
  }
  return iloc_node;
}

// Função para imprimir uma operação ILOC
void print_iloc_op(iloc_op* operation) {
  char* mnemonico = operation->mnemonico;
  if (strcmp(mnemonico, "storeAI") == 0) 
    // storeAI r1 => r2, c3 // Memoria(r2 + c3) = r1
    printf("%s %s => %s, %s", operation->mnemonico, operation->arg1, operation->arg2, operation->arg3);
  else if (strcmp(mnemonico, "loadAI") == 0)
    // loadAI r1, c2 => r3 // r3 = Memoria(r1 + c2)
    printf("%s %s, %s => %s", operation->mnemonico, operation->arg2, operation->arg3, operation->arg1);
  else if (strcmp(mnemonico, "loadI") == 0)
    // loadI c1 => r2 // r2 = c1
    printf("%s %s => %s", operation->mnemonico, operation->arg1, operation->arg2);
  else if (strcmp(mnemonico, "label") == 0)
    // label
    printf("%s:", operation->arg3);
  else if (strcmp(mnemonico, "cbr") == 0)
    // cbr r1 -> l2, l3 // PC = endereço(l2) se r1 = true, senão PC = endereço(l3)
    printf("%s %s => %s, %s", operation->mnemonico, operation->arg3, operation->arg1, operation->arg2);
  else if (strcmp(mnemonico, "jumpI") == 0)
    // jumpI -> l1 // PC = endereço(l1)
    printf("%s => %s", operation->mnemonico, operation->arg3);
  else if (
    strcmp(mnemonico, "add") == 0 ||
    strcmp(mnemonico, "sub") == 0 ||
    strcmp(mnemonico, "mult") == 0 ||
    strcmp(mnemonico, "div") == 0 ||
    strcmp(mnemonico, "cmp_EQ") == 0 ||
    strcmp(mnemonico, "cmp_NE") == 0 ||
    strcmp(mnemonico, "cmp_GE") == 0 ||
    strcmp(mnemonico, "cmp_LE") == 0 ||
    strcmp(mnemonico, "cmp_GT") == 0 ||
    strcmp(mnemonico, "cmp_LT") == 0 ||
    strcmp(mnemonico, "and") == 0 ||
    strcmp(mnemonico, "or") == 0
  )
    printf("%s %s, %s => %s", operation->mnemonico, operation->arg1, operation->arg2, operation->arg3);
  printf("\n");
}

void print_iloc_op_list(iloc_op_list * list) {
  iloc_op_list* current =  list;
  while (current != NULL) {
    print_iloc_op(current->operation);
    current = current->next_operation;
  }
}

char *generate_label(){
  static int label_num = 0;
  int length = snprintf( NULL, 0, "L%d", label_num );
  char* str = malloc( length + 1 );
  snprintf( str, length + 1, "L%d", label_num);
  label_num++; 
  return str; 
}

char *generate_temp(){
  static int temp_num = 0; 
  int length = snprintf( NULL, 0, "r%d", temp_num );
  char* str = malloc( length + 1 );
  snprintf( str, length + 1, "r%d", temp_num);
  temp_num++;
  return str; 
}


#endif