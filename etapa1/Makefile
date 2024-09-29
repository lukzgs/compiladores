ETAPA := etapa1

# Gera o scanner com o Flex
scanner: flex/scanner.l
	flex -o build/lex.yy.c flex/scanner.l

# Compila os arquivos gerados pelo Flex
compile: scanner
	gcc -c build/lex.yy.c -o build/lex.yy.o

# Linka os arquivos compilados e gera o executável
link: compile
	gcc -o $(ETAPA) src/main.c build/lex.yy.o  -lfl -Wall
	
# Limpa os arquivos gerados
clean:
	rm build/* $(ETAPA)

# Padrão do Makefile, executa o alvo "run" por padrão
.DEFAULT_GOAL := link
