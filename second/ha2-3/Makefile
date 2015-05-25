CPP=g++-4.8
CPP_FLAGS=-Wall -Werror -std=c++11

SRC = src
TEST = test
BIN = bin

all: $(BIN)/main $(BIN)/test

$(BIN)/main: $(TEST)/main.cpp $(SRC)/fn.h
	$(CPP) $(CPP_FLAGS) $(TEST)/main.cpp -o $(BIN)/main

$(BIN)/test: $(TEST)/test.cpp $(SRC)/fn.h
	$(CPP) $(CPP_FLAGS) $(TEST)/test.cpp -o $(BIN)/test

clean:
	rm -f bin/*

.PHONY: clean
