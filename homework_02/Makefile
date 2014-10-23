CPP=g++-4.8
CPP_FLAGS=-std=c++11

all: main

main: vector_t.o lint.o main.o
	$(CPP) $(CPP_FLAGS) vector_t.o lint.o main.o -o main

vector_t.o: vector_t.h vector_t.cpp
	$(CPP) $(CPP_FLAGS) -c vector_t.cpp -o vector_t.o

lint.o: lint.h lint.cpp vector_t.h
	$(CPP) $(CPP_FLAGS) -c lint.cpp -o lint.o

main.o: main.cpp lint.h
	$(CPP) $(CPP_FLAGS) -c main.cpp -o main.o

clean:
	rm -f *.o main

.PHONY: clean
