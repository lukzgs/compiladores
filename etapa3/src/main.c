#include <stdio.h>
#include "asd.h"

extern int yyparse(void);
extern int yylex_destroy(void);
void *arvore = NULL;
void exporta (void *arvore){
  // asd_print_graphviz(arvore); 
  asd_print_export(arvore); 
}

int main (int argc, char **argv)
{
  int ret = yyparse(); 
  exporta (arvore);
  yylex_destroy();
  asd_free(arvore);
  return ret;
}
