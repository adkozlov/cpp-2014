#include <iostream>
#include <cstdint>

template<size_t bits_number>
struct uint_aligned
{
    static_assert(bits_number <= 32, "bits number is greater than 32");
    typedef typename uint_aligned<bits_number + 1>::type type;
};

template<>
struct uint_aligned<8>
{
    typedef uint8_t type;
};

template<>
struct uint_aligned<16>
{
    typedef uint16_t type;
};

template<>
struct uint_aligned<32>
{
    typedef uint32_t type;
};

int main()
{
    const size_t header_size = 6;
    const size_t data_size = 32;

    std::cout << sizeof(uint_aligned<header_size>::type) << std::endl;
    std::cout << sizeof(uint_aligned<data_size>::type) << std::endl;

    return 0;
}
