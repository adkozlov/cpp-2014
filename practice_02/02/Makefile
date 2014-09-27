CPP=g++-4.8
CPP_FLAGS=-Werror -Wall

objects = main.o b.o

all: main

b.o: b.h b.cpp
	$(CPP) $(CPP_FLAGS) -c b.cpp

main.o: b.h main.cpp
	$(CPP) $(CPP_FLAGS) -c main.cpp

main: $(objects)
	$(CPP) $(CPP_FLAGS) $(objects) -o main

clean:
	rm -f main $(objects)

.PHONY: clean
