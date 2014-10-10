CPP=g++-4.8
CPP_FLAGS=-std=c++0x -Wall -Werror

EXECUTABLE=huffman
SRC_DIR=src
BIN_DIR=bin

objects=$(BIN_DIR)/common_definitions.o $(BIN_DIR)/huffman_code.o $(BIN_DIR)/codec.o $(BIN_DIR)/binary_reader.o $(BIN_DIR)/binary_writer.o $(BIN_DIR)/arguments_parsing_utils.o $(BIN_DIR)/main.o

all: $(EXECUTABLE)

$(BIN_DIR)/common_definitions.o: $(SRC_DIR)/common_definitions.h $(SRC_DIR)/common_definitions.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/common_definitions.cpp -o $(BIN_DIR)/common_definitions.o

$(BIN_DIR)/huffman_code.o: $(SRC_DIR)/common_definitions.h $(SRC_DIR)/huffman_code.h $(SRC_DIR)/huffman_code.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/huffman_code.cpp -o $(BIN_DIR)/huffman_code.o

$(BIN_DIR)/codec.o: $(SRC_DIR)/huffman_code.h $(SRC_DIR)/codec.h $(SRC_DIR)/codec.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/codec.cpp -o $(BIN_DIR)/codec.o

$(BIN_DIR)/binary_reader.o: $(SRC_DIR)/common_definitions.h $(SRC_DIR)/binary_reader.h $(SRC_DIR)/binary_reader.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/binary_reader.cpp -o $(BIN_DIR)/binary_reader.o

$(BIN_DIR)/binary_writer.o: $(SRC_DIR)/common_definitions.h $(SRC_DIR)/binary_writer.h $(SRC_DIR)/binary_writer.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/binary_writer.cpp -o $(BIN_DIR)/binary_writer.o

$(BIN_DIR)/arguments_parsing_utils.o: $(SRC_DIR)/arguments_parsing_utils.h $(SRC_DIR)/arguments_parsing_utils.cpp
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/arguments_parsing_utils.cpp -o $(BIN_DIR)/arguments_parsing_utils.o

$(BIN_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/arguments_parsing_utils.h $(SRC_DIR)/codec.h $(SRC_DIR)/binary_reader.h $(SRC_DIR)/binary_writer.h
	$(CPP) $(CPP_FLAGS) -c $(SRC_DIR)/main.cpp -o $(BIN_DIR)/main.o

$(EXECUTABLE): $(objects)
	$(CPP) $(CPP_FLAGS) -o $(BIN_DIR)/$(EXECUTABLE) $(objects)

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: clean