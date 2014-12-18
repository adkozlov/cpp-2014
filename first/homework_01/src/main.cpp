#include <iostream>
#include <fstream>

#include "arguments_parsing_utils.h"
#include "codec.h"
#include "binary_writer.h"
#include "binary_reader.h"

using std::string;
using std::vector;

size_t size_of_file(const string& file_name)
{
    std::ifstream input(file_name, std::ios::binary);
    if (input)
    {
        input.seekg(0, input.end);
        return input.tellg();
    }

    return -1;
}

void encode_file(const string& input_file_name, const string& output_file_name)
{
    std::fstream input(input_file_name, std::ios::in | std::ios::binary);

    vector<size_t> frequencies(codec::TABLE_SIZE);
    size_t char_count = 0;

    while (input.good())
    {
        char_t c = input.get();

        if (!input.eof())
        {
            frequencies[c]++;
            char_count++;
        }
    }

    input.clear();
    input.seekg(0, input.beg);

    codec codec = codec::build_by_frequencies(frequencies);
    binary_writer writer(output_file_name);

    char_t count_buffer[codec::UINT32_SIZE];
    codec::encode_uint32(char_count, count_buffer);
    writer.write_bytes(count_buffer, codec::UINT32_SIZE);

    char_t table[codec::TABLE_SIZE] = {0};
    codec.copy_sizes(table);
    writer.write_bytes(table, codec::TABLE_SIZE);

    while (input.good())
    {
        char_t c = input.get();

        if (!input.eof())
        {
            huffman_code code;

            if (codec.encode(c, &code))
            {
                for (size_t i = 0; i < code.size(); i++)
                {
                    writer.write_bit(code[i]);
                }
            }
        }
    }

    writer.flush();
    input.close();
}

void decode_file(const string& input_file_name, const string& output_file_name)
{
    binary_reader reader(input_file_name);

    char_t count_buffer[codec::UINT32_SIZE];
    reader.read_bytes(count_buffer, codec::UINT32_SIZE);
    uint32_t count = codec::decode_uint32(count_buffer);

    char_t table[codec::TABLE_SIZE] = {0};
    reader.read_bytes(table, codec::TABLE_SIZE);

    std::vector<char_t> lengths_vector(codec::TABLE_SIZE);
    for (size_t i = 0; i < codec::TABLE_SIZE; i++)
    {
        lengths_vector[i] = table[i];
    }

    codec codec = codec::build_by_code_lengths(lengths_vector);
    std::fstream output(output_file_name, std::ios::out | std::ios::binary);

    huffman_code code;
    while (!reader.eof() && count > 0)
    {
        char_t bit = reader.read_bit();
        if (bit == binary_reader::EOF_BYTE)
        {
            break;
        }

        code.push(bit);

        char_t symbol = 0;
        if (code.size() >= codec.get_min_code_size() && codec.decode(code, &symbol))
        {
            output.put((char) symbol);

            code.clear();
            count--;
        }
    }

    output.close();
}

int main(int argc, char* argv[])
{
    if (argc < 6)
    {
        std::cout << "Usage: " << argv[0] << " [-c | -u] [-f | --file] <file_name> [-o | --output] <file_name>" << std::endl;

        return 0;
    }

    bool encode = false;
    bool decode = false;

    char *input_file_name;
    char *output_file_name;

    for (int i = 1; i < argc; i++)
    {
        char const *argument = argv[i];

        if (argument_equals(argument, "-c"))
        {
            encode = true;
        }
        else if (argument_equals(argument, "-u"))
        {
            decode = true;
        }
        else if (is_enough_arguments_for_key(argument, "-f", "--file", i, argc))
        {
            i++;
            input_file_name = argv[i];
        }
        else if (is_enough_arguments_for_key(argument, "-o", "--output", i, argc))
        {
            i++;
            output_file_name = argv[i];
        }
        else if (is_not_enough_arguments_for_key(argument, "-f", "--file", i, argc) ||
                is_not_enough_arguments_for_key(argument, "-o", "--output", i, argc))
        {
            std::cout << "Not enough arguments for option: " << argv[i];

            return 1;
        }
        else
        {
            std::cout << "Unknown argument: " << argument << std::endl;

            return 1;
        }
    }

    if (encode == decode)
    {
        std::cout << "Incompatible arguments: -c -u" << std::endl;

        return 1;
    }

    string input_file_name_string = string(input_file_name);
    string output_file_name_string = string(output_file_name);

    encode ? encode_file(input_file_name_string, output_file_name_string) : decode_file(input_file_name_string, output_file_name_string);

    std::cout << size_of_file(input_file_name) << std::endl;
    std::cout << size_of_file(output_file_name) << std::endl;
    std::cout << codec::UINT32_SIZE + codec::TABLE_SIZE << std::endl;

    return 0;
}