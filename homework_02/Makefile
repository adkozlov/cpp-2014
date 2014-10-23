CPP=g++-4.8
CPP_FLAGS=-std=c++11 -Wall -Werror

SRC_DIR=src
BIN_DIR=bin

all: $(BIN_DIR)/main

$(BIN_DIR)/main: vector_t.h vector_t.cpp lint.h lint.cpp main.cpp
	$(CPP) $(CPP_FLAGS) vector_t.cpp lint.cpp main.cpp -o $(BIN_DIR)/main

clean:
	rm -rf $(BIN_DIR)/

.PHONY: clean
