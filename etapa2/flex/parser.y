/* Lucas Guaitanelli da Silveira - 208695 */
/* Vithor Barros Pileco - 326674 */

%{
int yylex(void);
void yyerror (char const *mensagem);
%}

%define parse.error verbose
%token TK_PR_INT
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_ELSE
%token TK_PR_WHILE
%token TK_PR_RETURN
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_OC_AND
%token TK_OC_OR
%token TK_IDENTIFICADOR
%token TK_LIT_INT
%token TK_LIT_FLOAT
%token TK_ERRO

%%

programa: func_list

%%