/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* os argumentos das operações podem ser nomes de temporários, valores constantes, ou nomes de rótulos. Não esquecer de liberar as structs */

// Uma instrução ILOC possui um mnemônico e até 3 argumentos
// mnemonico e arg1 => obrigatórios
// arg2 e arg3 => opcionais

// Estrutura para representar uma operação ILOC
typedef struct Iloc {
  char* mnemonico;
  char* arg1;
  char* arg2;
  char* arg3;
} Iloc;

// Estrutura para manter uma lista de operações ILOC
typedef struct IlocOperationList {
    Iloc* operation;
    struct IlocOperationList* next_operation;
} IlocOperationList;



// [SEGUIR DAQUI]
// next episode => operations 