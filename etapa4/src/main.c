/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#include <stdio.h>
#include "asd.h"
#include "table_sym.h"
#include <stdlib.h>

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
table_symbol* current_table = NULL; 
void exporta (void *arvore){
  // asd_print_graphviz(arvore); 
  asd_print_export(arvore); 
}

int main (int argc, char **argv)
{
  current_table = table_new(); 
  int ret = yyparse(); 
  // exporta (arvore);
  yylex_destroy();
  asd_free(arvore);
  table_print(current_table); 
  table_free(current_table); 
  return ret;
}
