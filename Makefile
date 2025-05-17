COMPILER=gcc
SRC=main.c libbmp.c
EXE=hex_extractor
LIBS=-lm

all:
	$(COMPILER) $(SRC) -o $(EXE) $(LIBS)
