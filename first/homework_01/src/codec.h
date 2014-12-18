#pragma once

#include <cstddef>
#include <climits>
#include <vector>
#include <map>

#include "huffman_code.h"

typedef std::map<long_t, std::pair<huffman_code, char_t>> decoding_map;

class codec
{
public:
    static const size_t TABLE_SIZE = UCHAR_MAX + 1;
    static const size_t UINT32_SIZE = sizeof(uint32_t);

    static codec build_by_frequencies(const std::vector<size_t>& frequencies);
    static codec build_by_code_lengths(const std::vector<char_t>& lengths);

    static void encode_uint32(const uint32_t n, char_t buffer[UINT32_SIZE]);
    static uint32_t decode_uint32(const char_t buffer[UINT32_SIZE]);

    size_t get_min_code_size() const;

    bool encode(const char_t c, huffman_code* code);
    bool decode(const huffman_code& code, char_t* c);

    void copy_sizes(char_t lengths[TABLE_SIZE]);

private:
    std::vector<huffman_code> codes;
    std::vector<decoding_map> decoding_maps;
    size_t min_code_size;

    codec(const size_t size, const size_t decoding_maps_size, const size_t min_code_size);
};