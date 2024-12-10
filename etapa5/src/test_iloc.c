#include <stdio.h>
#include <stdlib.h>
#include "iloc.h"
#include "test_iloc.h"

void test_new_iloc_operation() {
  iloc_op* op = new_iloc_operation("addI", "r1", "r2", "r3");
  if (op == NULL) {
    printf("Erro: new_iloc_operation retornou NULL\n");
    return;
  }
  printf("new_iloc_operation passou\n");
}

void test_add_iloc_operation() {
  iloc_op* op = new_iloc_operation("addI", "r1", "r2", "r3");
  iloc_op_list* list = add_iloc_operation(NULL, op);
  if (list == NULL || list->operation != op) {
    printf("Erro: add_iloc_operation falhou\n");
    return;
  }
  printf("add_iloc_operation passou\n");
}

void test_print_iloc_op() {
  iloc_op* op = new_iloc_operation("addI", "r1", "r2", "r3");
  printf("Saída esperada: addI r1, r2 => r3\n");
  printf("Saída real: ");
  print_iloc_op(op);
}

void test_print_iloc_op_list() {
  iloc_op* op1 = new_iloc_operation("addI", "r1", "r2", "r3");
  iloc_op* op2 = new_iloc_operation("subI", "r4", "r5", "r6");
  iloc_op_list * list = add_iloc_operation(NULL, op1);
  add_iloc_operation(list, op2);
  printf("Saída esperada:\naddI r1, r2 => r3\nsubI r4, r5 => r6\n");
  printf("Saída real:\n");
  print_iloc_op_list(list);
}

int main() {
  test_new_iloc_operation();
  test_add_iloc_operation();
  test_print_iloc_op();
  test_print_iloc_op_list();
  return 0;
}