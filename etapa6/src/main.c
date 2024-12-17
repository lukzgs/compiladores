/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#include <stdio.h>
#include "asd.h"
#include "table_sym.h"
#include <stdlib.h>
#include "iloc.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
table_symbol* current_table = NULL;
void exporta (void *arvore) {
  asd_print_export(arvore);
}

int main (int argc, char **argv) {
  current_table = table_new();
  int ret = yyparse();
  yylex_destroy();
  if (arvore != NULL) print_iloc_op_list(((asd_tree_t * ) arvore)->code);
  asd_free(arvore);
  table_free(current_table);
  return ret;
}
