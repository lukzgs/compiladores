#define LITERAL 0
#define IDENTIFICADOR 1
#define EXPRESSIAO_OPERADOR 2
#define LANGUAGEM_OPERATOR 3
#define CONTROLE 4
#define TIPO 5
#define TOKEN_SINTATICO 6
#define CHAMADA_FUNCAO 7

#include <string.h>
#include <stdlib.h>

<<<<<<< HEAD
typedef struct ValorLexico {
  int linha_token;
  char *valor_token;
  int tipo_token;
} ValorLexico;
=======
/* Estrutura para representar um valor_lexico. */
typedef struct valor_lexico {
	int numero_linha;
	int tipo_token;
	char *valor_token;
} valor_lexico;
>>>>>>> 8e84f91 (asome functions added and changes in parser. not working.)

/* Estrutura para representar um nodo da árvore. */
typedef struct Nodo {
  struct valor_lexico *info;
  struct Nodo** filho;
  int numeroFilhos;
} Nodo ;

/* Função para criar um novo nó da árvore. */
Nodo* criaNodo(valor_lexico* info);

/* Função para adicionar um filho a um nó. */
void adicionaNodo(Nodo* pai, Nodo* filho);

/* Função para remover um nó e todos os seus descendentes. */
void removeNodo(Nodo* node);

/* Função para imprimir a árvore usando um percurso em profundidade (DFS) */
void impressaoDFS(Nodo* raiz);

char* concatChamada(char* s1);

void concatList(Nodo* list1, Nodo* list2);
