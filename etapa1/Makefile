# ARRUMAR
# Makefile

# Gera o parser com o Bison
parser: parser.y
	bison -d parser.y

# Gera o scanner com o Flex
scanner: scanner.l
	flex scanner.l

# Compila os arquivos gerados pelo Bison e pelo Flex
compile: parser scanner
	gcc -c lex.yy.c parser.tab.c utils.c

# Linka os arquivos compilados e gerar o executável
link: compile
	gcc -o etapa4 main.c parser.tab.o lex.yy.o utils.o -lfl -Wall

# Limpa os arquivos gerados
clean: rm -f lex.yy.c lex.yy.o parser.tab.c parser.tab.o parser.tab.h parser.output utils.o

# Padrão do Makefile, executa o alvo "run" por padrão
.DEFAULT_GOAL := link