CPP=g++-4.8
CPP_FLAGS=-std=c++11 -Wall -Werror

SRC_DIR=src
BIN_DIR=bin

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(SRC_DIR)/vector_t.h $(SRC_DIR)/vector_t.cpp $(SRC_DIR)/lint.h $(SRC_DIR)/lint.cpp $(SRC_DIR)/main.cpp
	$(CPP) $(CPP_FLAGS) $(SRC_DIR)/*.cpp -o $(BIN_DIR)/main

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: clean
