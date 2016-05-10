CFLAGS = -Wall -Werror -ansi -pedantic
CC = g++
EXEC = rshell
SRC_DIR = src
BIN_DIR = bin
OBJ = ../$(BIN_DIR)/main.o ../$(BIN_DIR)/Processes.o ../$(BIN_DIR)/Base.o ../$(BIN_DIR)/Command.o ../$(BIN_DIR)/Andand.o ../$(BIN_DIR)/Oror.o

all: rshell

../$(BIN_DIR)/%.o: $(SRC_DIR)%.cpp $(SRC_DIR)%.h
    $(CC) $(CFLAGS) -c $(SRC_DIR)%.cpp -o $@

$(EXEC): $(OBJ)
    @mkdir -p bin
    $(CC) $(CFLAGS) $(BIN_DIR)/$(OBJ) -o $(BIN_DIR)/$(EXEC)

.PHONY : clean

clean:
    -rm -rf $(BIN_DIR)