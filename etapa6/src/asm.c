/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

#include "asm.h"
#include <stdbool.h>

char * concat_str(char * str, char * i){
    int length =  strlen(str) + strlen(i) + 1; 
    char *new_str = malloc(sizeof(char) * (length)); 
    *new_str = '\0'; 
    strcat(new_str, str);
    strcat(new_str, i); 
    return new_str; 
}

char *get_mem_str(char * shift){
    char *str_2 = concat_str("-", shift);
    char *str_3 = concat_str(str_2, "(%rbp)"); 
    free(str_2); 
    return str_3; 
}

char * get_asm_op_code(char * mnemonico){
    if (!strcmp(mnemonico, "add")){
        return "addl"; 
    }
    if (!strcmp(mnemonico, "sub")){
        return "subl"; 
    }
    if (!strcmp(mnemonico, "mult")){
        return "imul"; 
    }
    if (!strcmp(mnemonico, "cmp_LE")){
        return "setle";
    }
    if (!strcmp(mnemonico, "cmp_LT")){
        return "setl";
    }
    if (!strcmp(mnemonico, "cmp_GT")){
        return "setg";
    }
    if (!strcmp(mnemonico, "cmp_GE")){
        return "setge";
    }
    if (!strcmp(mnemonico, "cmp_EQ")){
        return "sete";
    }
    if (!strcmp(mnemonico, "cmp_NE")){
        return "setne";
    }
    fprintf(stderr, "Failed to find asm code for iloc instruction %s", mnemonico);
    exit(1); 
}

char * get_literal_0_or_1(int is_zero){
    if (is_zero){
        return "$0";
    } else {
        return "$1";
    }
}

void  convert_iloc_to_asm(iloc_op * op, asm_op_list * list, table_symbol * table){
    if (!strcmp(op->mnemonico, "loadI")){
        row_symbol * row = get_or_create_row_from_scope(table, op->arg2); 
        char * str = concat_str("$", op->arg1); 
        char * str_2 = get_mem_str(row->shift); 
        add_iloc_operation(list, new_iloc_operation("movl", str, str_2, NULL)); 
        free(str); 
        free(str_2);
    }   else if (!strcmp(op->mnemonico, "loadAO")){
        row_symbol * row = get_or_create_row_from_scope(table, op->arg1); 
        char * str_2 = get_mem_str(row->shift);
        row_symbol * row_3 = get_or_create_row_from_scope(table, op->arg3); 
        char * str_3 = get_mem_str(row_3->shift);
        add_iloc_operation(list, new_iloc_operation("movslq",  str_2, "%rbx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movq", "%rbp", "%rdx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("subq",  "%rbx", "%rdx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movl", "(%rdx)", "%ebx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movl", "%ebx", str_3, NULL)); 
        free(str_2);
        free(str_3); 
    } else if (!strcmp(op->mnemonico, "storeAO")){
        row_symbol * row = get_or_create_row_from_scope(table, op->arg1); 
        char * str_1 = get_mem_str(row->shift);
        row_symbol * row_2 = get_or_create_row_from_scope(table, op->arg2); 
        char * str_2 = get_mem_str(row_2->shift);
        add_iloc_operation(list, new_iloc_operation("movslq",  str_2, "%rbx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movq", "%rbp", "%rdx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("subq",  "%rbx", "%rdx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movl", str_1, "%eax", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movl", "%eax", "(%rdx)", NULL)); 
        free(str_2);
        free(str_1); 
    } else if (!strcmp(op->mnemonico, "return")){
        row_symbol * row = get_row_from_scope_or_throw(table, op->arg1); 
        char *str = get_mem_str(row->shift); 
        add_iloc_operation(list, new_iloc_operation("movl", str, "%eax", NULL)); 
        free(str);
    }  else  if (!strcmp(op->mnemonico, "add") || !strcmp(op->mnemonico, "sub") || !strcmp(op->mnemonico, "mult")){
        row_symbol * row = get_row_from_scope_or_throw(table, op->arg1); 
        char *str = get_mem_str(row->shift); 
        row_symbol * row_2 = get_row_from_scope_or_throw(table, op->arg2); 
        char *str_2 = get_mem_str(row_2->shift); 
        row_symbol * row_3 = get_or_create_row_from_scope(table, op->arg3); 
        char *str_3 = get_mem_str(row_3->shift); 
        add_iloc_operation(list, new_iloc_operation("movl", str,  "%ebx", NULL)); 
        add_iloc_operation(list, new_iloc_operation(get_asm_op_code(op->mnemonico), str_2, "%ebx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movl", "%ebx",  str_3, NULL));
        free(str);
        free(str_2);
        free(str_3);
    }  else if (!strcmp(op->mnemonico, "div")){
        row_symbol * row = get_row_from_scope_or_throw(table, op->arg1); 
        char *str = get_mem_str(row->shift); 
        row_symbol * row_2 = get_row_from_scope_or_throw(table, op->arg2); 
        char *str_2 = get_mem_str(row_2->shift); 
        row_symbol * row_3 = get_or_create_row_from_scope(table, op->arg3); 
        char *str_3 = get_mem_str(row_3->shift); 
        add_iloc_operation(list, new_iloc_operation("movl", str,  "%eax", NULL));
        add_iloc_operation(list, new_iloc_operation("cltd", NULL, NULL, NULL)); 
        add_iloc_operation(list, new_iloc_operation("idivl", str_2, NULL, NULL)); 
        add_iloc_operation(list, new_iloc_operation("movl", "%eax",  str_3, NULL));
        free(str);
        free(str_2);
        free(str_3);
    }  else if (!strcmp(op->mnemonico, "cmp_GE") || !strcmp(op->mnemonico, "cmp_LE") || !strcmp(op->mnemonico, "cmp_GT") || !strcmp(op->mnemonico, "cmp_LT")
        || !strcmp(op->mnemonico, "cmp_NE") || !strcmp(op->mnemonico, "cmp_EQ")){
        row_symbol * row = get_row_from_scope_or_throw(table, op->arg1); 
        char *str = get_mem_str(row->shift); 
        row_symbol * row_2 = get_row_from_scope_or_throw(table, op->arg2); 
        char *str_2 = get_mem_str(row_2->shift); 
        row_symbol * row_3 = get_or_create_row_from_scope(table, op->arg3); 
        char *str_3 = get_mem_str(row_3->shift); 
        add_iloc_operation(list, new_iloc_operation("movl", str,  "%ebx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("cmpl", str_2, "%ebx", NULL)); 
        add_iloc_operation(list, new_iloc_operation(get_asm_op_code(op->mnemonico), "%al",  NULL, NULL));
        add_iloc_operation(list, new_iloc_operation("movzbl", "%al",  "%ebx", NULL));
        add_iloc_operation(list, new_iloc_operation("movl", "%ebx",  str_3, NULL));
        free(str);
        free(str_2);
        free(str_3);
    } else if (!strcmp(op->mnemonico, "and") || !strcmp(op->mnemonico, "or")){
        row_symbol * row = get_row_from_scope_or_throw(table, op->arg1); 
        char *str = get_mem_str(row->shift); 
        row_symbol * row_2 = get_row_from_scope_or_throw(table, op->arg2); 
        char *str_2 = get_mem_str(row_2->shift); 
        row_symbol * row_3 = get_or_create_row_from_scope(table, op->arg3); 
        char *str_3 = get_mem_str(row_3->shift); 
        char * label_1 = generate_label();
        char * label_2 = generate_label(); 
        add_iloc_operation(list, new_iloc_operation("cmpl", get_literal_0_or_1(!strcmp(op->mnemonico, "and")), str, NULL)); 
        add_iloc_operation(list, new_iloc_operation("je", label_1, "%ebx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("cmpl", get_literal_0_or_1(!strcmp(op->mnemonico, "and")), str_2, NULL)); 
        add_iloc_operation(list, new_iloc_operation("je", label_1, "%ebx", NULL)); 
        add_iloc_operation(list, new_iloc_operation("movl", get_literal_0_or_1(strcmp(op->mnemonico, "and")),  str_3, NULL));
        add_iloc_operation(list, new_iloc_operation("jmp", label_2,  NULL, NULL));
        add_iloc_operation(list, new_iloc_operation("label", label_1,  NULL, NULL));
        add_iloc_operation(list, new_iloc_operation("movl", get_literal_0_or_1(!strcmp(op->mnemonico, "and")),  str_3, NULL));
        add_iloc_operation(list, new_iloc_operation("label", label_2,  NULL, NULL));

        free(str);
        free(str_2);
        free(str_3);
        free(label_1);
        free(label_2);
    } else if (!strcmp(op->mnemonico, "jumpI")){
        add_iloc_operation(list, new_iloc_operation("jmp", op->arg1,  NULL, NULL));
    } else if (!strcmp(op->mnemonico, "label")){
        add_iloc_operation(list, new_iloc_operation("label", op->arg1,  NULL, NULL));
    } else if (!strcmp(op->mnemonico, "cbr")){
        row_symbol * row = get_row_from_scope_or_throw(table, op->arg1); 
        char *str = get_mem_str(row->shift);
        add_iloc_operation(list, new_iloc_operation("cmpl", "$0", str, NULL)); 
        add_iloc_operation(list, new_iloc_operation("je", op->arg3, NULL, NULL)); 
        add_iloc_operation(list, new_iloc_operation("jmp", op->arg2, NULL, NULL)); 
        free(str); 
    }
}

asm_op_list * generate_asm(iloc_op_list * list, table_symbol * table){
    asm_op_list * list_asm = create_iloc_list(); 
    while (list != NULL){
        convert_iloc_to_asm(list->operation,list_asm, table);
        list = list->next_operation; 
    }
    return list_asm; 
}

void print_asm_op(asm_op * op){
    if (!strcmp(op->mnemonico, "label")){
        printf("%s:\n", op->arg1);
        return; 
    }
    printf("\t"); 
    if (!strcmp(op->mnemonico, "cltd")){
        printf("%s", op->mnemonico);
    }
    if (!strcmp(op->mnemonico, "idivl") || !strcmp(op->mnemonico, "setle") || !strcmp(op->mnemonico, "setge") || !strcmp(op->mnemonico, "setg")
     || !strcmp(op->mnemonico, "setl") || !strcmp(op->mnemonico, "sete") || !strcmp(op->mnemonico, "setne") || !strcmp(op->mnemonico, "je") 
     || !strcmp(op->mnemonico, "jmp") ){
        printf("%s %s", op->mnemonico,  op->arg1);
    }
    if (!strcmp(op->mnemonico, "movl") || !strcmp(op->mnemonico, "addl") || !strcmp(op->mnemonico, "subl") || !strcmp(op->mnemonico, "imul")
    || !strcmp(op->mnemonico, "cmpl") || !strcmp(op->mnemonico, "movzbl") || !strcmp(op->mnemonico, "movslq")  || !strcmp(op->mnemonico, "addq") 
    || !strcmp(op->mnemonico, "subq") || !strcmp(op->mnemonico, "movq")){
        printf("%s %s, %s", op->mnemonico,  op->arg1, op->arg2);
    }
    printf("\n");
}

void print_asm_list(asm_op_list * list){
    printf(".globl	main\n.type	main, @function\nmain:\n.LFB0:\n\tpushq	%%rbp\n\tmovq	%%rsp, %%rbp\n"); 
    while (list != NULL){
        print_asm_op(list->operation);
        list = list->next_operation; 
    }
    printf("\tpopq	%%rbp\n\tret\n"); 
    

}
