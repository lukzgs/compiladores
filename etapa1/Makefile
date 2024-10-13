ETAPA=etapa1

IDIR =include
CC=gcc
CFLAGS=-I$(IDIR)
ODIR=build
CDIR=src

_DEPS = tokens.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_FLEX_FILE = flex/scanner.l
_FLEX_SRC = build/lex.yy.c

_OBJ = main.o lex.yy.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(_FLEX_SRC): $(_FLEX_FILE)
	flex -o $(_FLEX_SRC) $(_FLEX_FILE)

$(ODIR)/%.o: $(CDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ETAPA): $(OBJ)
	$(CC) -o $(ETAPA) $^ $(CFLAGS)

.PHONY: clean
.DEFAULT_GOAL := $(ETAPA)

clean:
	rm -f $(ODIR)/* *~ $(ETAPA) 

