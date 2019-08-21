CC = gcc
FLAGS = -g -W -I $(INC) -O0
INC = ./include
SRC = ./src
BUI = ./build
BIN = ./bin
OBJ = $(BUI)/functions.o $(BUI)/home-project.o
PROG = $(BIN)/photo-editor
all: $(PROG)
$(PROG): $(OBJ)
	$(CC) $(FLAGS) -o $(PROG) $(OBJ) -lm
$(BUI)/functions.o: $(SRC)/functions.c $(INC)/functions.h
	$(CC) $(FLAGS) -c $(SRC)/functions.c -o $(BUI)/functions.o
$(BUI)/home-project.o: $(SRC)/home-project.c $(INC)/functions.h
	$(CC) $(FLAGS) -c $(SRC)/home-project.c -o $(BUI)/home-project.o
run:
	reset
	$(PROG)
mkdir:
	mkdir -p $(BUI)
	mkdir -p $(BIN)
	mkdir -p ./res
clean:
	rm -rf $(BUI)
	rm -rf $(BIN)
	rm -rf ./res
gdb:
	gdb $(BIN)/photo-editor
