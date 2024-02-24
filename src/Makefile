## Para gerar executáveis para windows a partir do linux, instale o pacote "mingw-w64" (debian/arch) ou equivalente.

## Definindo variáveis específicas para cada sistema
ifeq ($(OS), Windows_NT)
	FixPath = $(subst /,\,$1)
	RM=del /s /F /q
	ZIP=powershell Compress-Archive -DestinationPath
	MKDIR=mkdir
else
   ifeq ($(shell uname), Linux)
	FixPath = $1
	RM=rm -rf
	ZIP=zip
	MKDIR=mkdir -p
   endif
endif

## Variáveis genéricas
CC=gcc
# Flags que devem estar presentes em (quase) todos os comandos de compilação
CFLAGS=-pedantic-errors -Wall -lm -I$(INC)
# ALL_CFLAGS são flags aplicadas em builds padrões (make all)
ALL_CFLAGS=$(CFLAGS) -g
RELEASE_CFLAGS=$(CFLAGS) -O2 -DNDEBUG
DEBUG_CFLAGS=$(CFLAGS) -g -fsanitize=address -fsanitize=undefined -ftrapv
# Diretório do código fonte
SRC=.
# Diretório com arquivos.h
INC=.
# Diretório com arquivos executáveis
BINDIR=bin
# Diretório para armazanar arquivos.o
OBJ=obj
# Nome do arquivo zip do projeto
SUBMITNAME=projeto.zip

## Variáveis definidas automaticamente
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
BIN=$(BINDIR)/main$(SUFFIX)
ifeq ($(OS), Windows_NT)
	BIN:=$(BIN).exe
endif

all: $(BIN)

## Criação de diretórios
$(BINDIR) $(OBJ):
	$(MKDIR) $@

#####

forWindows: CC=x86_64-w64-mingw32-gcc
forWindows: all

release:
	make ALL_CFLAGS="$(RELEASE_CFLAGS)" SUFFIX=-$@ OBJS="$(patsubst %.o,%-$@.o,$(OBJS))"

debug:
	make ALL_CFLAGS="$(DEBUG_CFLAGS)" SUFFIX=-$@ OBJS="$(patsubst %.o,%-$@.o,$(OBJS))"

$(BIN): $(OBJS) | $(BINDIR)
	$(CC) $(ALL_CFLAGS) $^ -o $@

$(OBJ)/%$(SUFFIX).o: $(SRC)/%.c | $(OBJ)
	$(CC) $(ALL_CFLAGS) -c $< -o $@

submit:
	$(RM) $(SUBMITNAME)
	$(ZIP) $(SUBMITNAME) *

run: $(BIN)
	$<

clean:
	$(RM) $(call FixPath,$(BINDIR) $(OBJ) $(SUBMITNAME))

## Apenas no Linux
clangd:
	@printf "%s\n" $(CFLAGS) > compile_flags.txt
