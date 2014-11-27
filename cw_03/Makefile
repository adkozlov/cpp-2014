CPP=g++-4.8
CPP_FLAGS=-Wall -Werror -std=c++11

all: bin/main

bin/main: src/au_list.h src/main.cpp
	$(CPP) $(CPP_FLAGS) src/main.cpp -o bin/main

clean:
	rm -f bin/*

.PHONY: clean