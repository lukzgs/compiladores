/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */
#ifndef _ILOC_H_
#define _ILOC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* os argumentos das operações podem ser nomes de temporários, valores constantes, ou nomes de rótulos. Não esquecer de liberar as structs */

// Uma instrução ILOC possui um mnemônico e até 3 argumentos
// mnemonico e arg1 => obrigatórios
// arg2 e arg3 => opcionais

// Estrutura para representar uma operação ILOC
typedef struct iloc_op {
  char* mnemonico;
  char* arg1;
  char* arg2;
  char* arg3;
} iloc_op;

// Estrutura para manter uma lista de operações ILOC
typedef struct iloc_op_list {
	iloc_op* operation;
	struct iloc_op_list* next_operation;
} iloc_op_list;

// Cria uma nova operação ILOC
iloc_op* new_iloc_operation(char* mnemonico, char* arg1, char* arg2, char* arg3);

// Adiciona uma operação à lista de operações ILOC
iloc_op_list* add_iloc_operation(iloc_op* new_iloc_op);

// Imprime uma operação ILOC
void print_iloc_op(iloc_op* operation);

// Imprime a lista de operações ILOC da lista global
void print_iloc_op_list();

#endif