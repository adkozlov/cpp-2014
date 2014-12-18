#pragma once

#include <vector>
#include <fstream>

#include "common_definitions.h"

class binary_writer
{
public:
    binary_writer(const std::string& file_name);
    ~binary_writer();

    bool is_open() const;

    void write_bytes(char_t* source, const size_t size);
    void write_bit(const char_t bit);

    void flush();

private:
    static const size_t BUFFER_SIZE;

    std::ofstream output;
    std::vector<char> buffer;

    size_t bit_position;
    size_t byte_position;

    void init_buffer();
    void flush_buffer();
};