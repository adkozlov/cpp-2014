CPP=g++-4.8
CPP_FLAGS=-Wall -Werror -std=c++11

all: main

main: bin/imp_double.o bin/main.o
	$(CPP) $(CPP_FLAGS) bin/imp_double.o bin/main.o -o bin/main

bin/imp_double.o: src/imp_double.h src/imp_double.cpp
	$(CPP) $(CPP_FLAGS) -c src/imp_double.cpp -o bin/imp_double.o

bin/main.o: src/imp_double.h src/main.cpp
	$(CPP) $(CPP_FLAGS) -c src/main.cpp -o bin/main.o

clean:
	rm -f bin/*

.PHONY: clean
