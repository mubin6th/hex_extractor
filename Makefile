COMPILER=gcc
SRC=main.c libbmp.c
EXE=main
LIBS=-lm

all:
	$(COMPILER) $(SRC) -o $(EXE) $(LIBS)
