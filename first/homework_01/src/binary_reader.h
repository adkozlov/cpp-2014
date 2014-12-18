#pragma once

#include <vector>
#include <fstream>

#include "common_definitions.h"

class binary_reader
{
public:
    static const char_t EOF_BYTE = 0xFE;

    binary_reader(const std::string& file_name);
    ~binary_reader();

    bool is_open() const;

    bool eof() const;

    void read_bytes(char_t *buf, const size_t size);
    char_t read_bit();

private:
    static const size_t BUFFER_SIZE;

    std::ifstream input;

    size_t file_size;
    size_t read;
    size_t byte_position;
    size_t bit_position;

    std::vector<char> buffer;

    bool update_buffer();
};