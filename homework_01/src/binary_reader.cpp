#include "binary_reader.h"

const size_t binary_reader::BUFFER_SIZE = 1024;

binary_reader::binary_reader(const std::string& file_name)
    : file_size(0)
    , byte_position(0)
    , bit_position(BYTE_SIZE - 1)
{
    input.open(file_name, std::ios::in | std::ios::binary);
    input.seekg(0, std::ios::end);

    int file_size = input.tellg();
    if (file_size != -1)
    {
        this->file_size = file_size;
    }

    input.seekg(0, std::ios::beg);
    size_t size = std::min(this->file_size, BUFFER_SIZE);
    buffer.resize(size);

    char* bytes = &buffer[0];
    input.read(bytes, size);

    read = 1;
}

binary_reader::~binary_reader()
{
    if (is_open())
    {
        input.close();
    }
}

bool binary_reader::is_open() const
{
    return input.is_open();
}

bool binary_reader::eof() const
{
    return ((read - 1) * BUFFER_SIZE + byte_position) >= file_size;
}

void binary_reader::read_bytes(char_t* destination, const size_t size)
{
    int bytes_left = size - (buffer.size() - byte_position);

    size_t read_bytes = std::min(size, buffer.size() - byte_position);
    std::copy(buffer.begin() + byte_position, buffer.begin() + byte_position + read_bytes, destination);

    byte_position += read_bytes;
    destination += read_bytes;

    while (bytes_left > 0 && update_buffer())
    {
        read_bytes = std::min(bytes_left, (int) buffer.size());
        bytes_left -= read_bytes;

        std::copy(buffer.begin() + byte_position, buffer.begin() + byte_position + read_bytes, destination);

        byte_position += read_bytes;
        destination += read_bytes;
    }

    if (byte_position >= buffer.size())
    {
        update_buffer();
    }
}

char_t binary_reader::read_bit()
{
    if (!is_open() || eof() || input.fail())
    {
        return EOF_BYTE;
    }

    char_t result = (buffer[byte_position] & (1 << bit_position));
    result >>= bit_position;

    bit_position = (bit_position == 0 ? BYTE_SIZE : bit_position) - 1;
    byte_position += (bit_position == BYTE_SIZE - 1) ? 1 : 0;

    if (byte_position >= buffer.size())
    {
        update_buffer();
    }

    return result;
}

bool binary_reader::update_buffer()
{
    size_t read_bytes = (read - 1) * BUFFER_SIZE + buffer.size();
    if (read_bytes >= file_size)
    {
        return false;
    }

    size_t size = std::min(file_size - read_bytes, BUFFER_SIZE);
    buffer.resize(size);

    char* bytes = &buffer[0];
    input.read(bytes, size);

    read++;
    byte_position = 0;

    return true;
}
