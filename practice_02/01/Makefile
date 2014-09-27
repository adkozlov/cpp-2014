CPP=g++-4.8
CPP_FLAGS=-Werror -Wall

objects = main.o b.o

all: main

b.o: b.cpp
	$(CPP) $(CPP_FLAGS) -c b.cpp

main.o:
	$(CPP) $(CPP_FLAGS) -c main.cpp

main: $(objects)
	$(CPP) $(CPP_FLAGS) $(objects) -o main

clean:
	rm -f main $(objects)

.PHONY: clean
