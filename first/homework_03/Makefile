CPP=g++
CPP_FLAGS=-Wall -Werror -std=c++11

SRC = src/
BIN = bin/

all: $(BIN)main

$(BIN)main: $(SRC)any.h $(SRC)any.cpp $(SRC)test.cpp
	$(CPP) $(CPP_FLAGS) $(SRC)test.cpp $(SRC)any.cpp -o $(BIN)main

clean:
	rm -f bin/*

.PHONY: clean