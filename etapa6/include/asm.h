/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#ifndef _ASM_H_
#define _ASM_H_

#include "iloc.h"
#include "table_sym.h"

typedef iloc_op asm_op; 
typedef iloc_op_list asm_op_list; 

asm_op_list * generate_asm(iloc_op_list * list, table_symbol * table); 

void print_asm_list(asm_op_list * list);
#endif 
