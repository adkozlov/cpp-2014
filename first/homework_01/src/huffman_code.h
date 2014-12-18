#pragma once

#include <bitset>

#include "common_definitions.h"

typedef std::bitset<255> bitset;

class huffman_code
{
public:
    huffman_code();
    huffman_code(const size_t size, const size_t value);
    void clear();

    size_t size() const;
    char_t operator[](const size_t position) const;

    void push(const char_t bit);

    long_t long_value() const;
    huffman_code& operator++();
    void shift_right();

private:
    bitset data;    
    size_t bitset_size;
};