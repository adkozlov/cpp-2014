#include "huffman_code.h"

huffman_code::huffman_code()
    : bitset_size(0)
{
}

huffman_code::huffman_code(const size_t size, const size_t value)
    : bitset_size(size)
{
    if (bitset_size > 0)
    {
        data = bitset(value);
    }
}

size_t huffman_code::size() const
{
    return bitset_size;
}

char_t huffman_code::operator[](const size_t position) const
{
    return data[bitset_size - position - 1];
}

void huffman_code::push(const char_t bit)
{
    data = bitset(data.to_ulong() * 2);
    data.set(0, bit ? true : false);

    bitset_size++;
}

long_t huffman_code::long_value() const
{
    return data.to_ulong();
}

huffman_code& huffman_code::operator++()
{
    bitset temp = bitset(data.to_ulong() + 1);
    if (temp[bitset_size] && !data[bitset_size])
    {
        bitset_size++;
    }

    data = temp;

    return *this;
}

void huffman_code::shift_right()
{
    data = bitset(data.to_ulong() / 2);
    bitset_size--;
}

void huffman_code::clear()
{
    bitset_size = 0;
    data.reset();
}