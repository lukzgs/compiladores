ETAPA=etapa6
IDIR=include
CC=gcc
CFLAGS=-I$(IDIR)
ODIR=build
CDIR=src

_DEPS=tokens.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_FLEX_FILE = flex/scanner.l
_FLEX_SRC = build/lex.yy.c

_BISON_SRC=build/parser.tab.c 
_BISON_HEADER=include/parser.tab.h
_BISON_FILE=bison/parser.y

_OBJ= lex.yy.o parser.tab.o main.o asd.o table_sym.o iloc.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(_FLEX_SRC): $(_FLEX_FILE) $(_BISON_HEADER)
	flex -o $(_FLEX_SRC) $(_FLEX_FILE)
	
$(_BISON_SRC) $(_BISON_HEADER): $(_BISON_FILE)
	bison -o $(_BISON_SRC) --header=$(_BISON_HEADER) $(_BISON_FILE)

$(ODIR)/%.o: $(CDIR)/%.c  # $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

$(ETAPA): $(OBJ)
	$(CC) -o $(ETAPA) $^ $(CFLAGS)

.PHONY: clean
.DEFAULT_GOAL := $(ETAPA)

clean:
	rm -f $(ODIR)/* *~ $(ETAPA) $(_BISON_HEADER)

