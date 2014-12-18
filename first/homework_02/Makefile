CPP=g++-4.8
CPP_FLAGS=-std=c++11 -Wall -Werror

SRC_DIR=src
BIN_DIR=bin
TEST_DIR=test

all: $(BIN_DIR)/main

$(BIN_DIR)/vector_t.o: $(SRC_DIR)/vector_t.h $(SRC_DIR)/vector_t.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/vector_t.cpp -o $(BIN_DIR)/vector_t.o

$(BIN_DIR)/lint.o: $(SRC_DIR)/vector_t.h $(SRC_DIR)/lint.h $(SRC_DIR)/lint.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/lint.cpp -o $(BIN_DIR)/lint.o

$(BIN_DIR)/main: $(BIN_DIR)/vector_t.o $(BIN_DIR)/lint.o
	@echo "compiled successfully"

$(BIN_DIR)/test.o: $(SRC_DIR)/vector_t.h $(SRC_DIR)/lint.h $(TEST_DIR)/main.cpp
	$(CPP) $(CPP_FLAGS) -c $(TEST_DIR)/main.cpp -o $(BIN_DIR)/test.o

$(BIN_DIR)/test: $(BIN_DIR)/vector_t.o $(BIN_DIR)/lint.o $(BIN_DIR)/test.o
	$(CPP) $(BIN_DIR)/vector_t.o $(BIN_DIR)/lint.o $(BIN_DIR)/test.o -o $(BIN_DIR)/test

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: clean
