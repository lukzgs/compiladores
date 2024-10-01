ETAPA := etapa1

# Gera o scanner com o Flex
build/lex.yy.c: flex/scanner.l
	flex -o build/lex.yy.c flex/scanner.l

# Compila os arquivos gerados pelo Flex
build/lex.yy.o: build/lex.yy.c
	gcc -c build/lex.yy.c -o build/lex.yy.o

# Linka os arquivos compilados e gera o executável
$(ETAPA): build/lex.yy.o
	gcc -o $(ETAPA) src/main.c build/lex.yy.o  -lfl -Wall
	
# Limpa os arquivos gerados
clean:
	rm -f build/* $(ETAPA)

# Padrão do Makefile, executa o alvo "run" por padrão
.DEFAULT_GOAL := $(ETAPA)
