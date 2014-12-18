#include "binary_writer.h"

const size_t binary_writer::BUFFER_SIZE = 256;

void binary_writer::init_buffer()
{
    buffer.assign(BUFFER_SIZE, 0);
    byte_position = 0;
}

void binary_writer::flush_buffer()
{
    char* buffer_begin = &buffer[0];

    output.write(buffer_begin, byte_position);
    output.flush();

    init_buffer();
}

binary_writer::binary_writer(const std::string& file_name)
    : bit_position(BYTE_SIZE - 1)
{
    output.open(file_name, std::ios::out | std::ios::binary);

    init_buffer();
}

binary_writer::~binary_writer()
{
    if (is_open())
    {
        output.close();
    }
}

bool binary_writer::is_open() const
{
    return output.is_open();
}

void binary_writer::write_bytes(char_t* source, const size_t size)
{
    if (size > 0 && bit_position < BYTE_SIZE - 1)
    {
        byte_position++;
    }

    int bytes_left = size - (buffer.size() - byte_position);

    size_t write_bytes = std::min(size, buffer.size() - byte_position);
    std::copy(source, source + write_bytes, buffer.begin() + byte_position);

    byte_position += write_bytes;
    source += write_bytes;

    while (bytes_left > 0)
    {
        flush_buffer();

        write_bytes = std::min(bytes_left, (int) buffer.size());
        bytes_left -= write_bytes;
        std::copy(source, source + write_bytes, buffer.begin() + byte_position);

        byte_position += write_bytes;
        source += write_bytes;
    }

    if (byte_position >= buffer.size())
    {
        flush_buffer();
    }
}

void binary_writer::write_bit(const char_t bit)
{
    if (is_open())
    {
        if (bit == 1)
        {
            buffer[byte_position] |= (1 << bit_position);
        }
        else
        {
            buffer[byte_position] &= ~(1 << bit_position);
        }

        bit_position = (bit_position == 0 ? BYTE_SIZE : bit_position) - 1;
        if (bit_position == BYTE_SIZE - 1)
        {
            byte_position++;
        }

        if (byte_position >= buffer.size())
        {
            flush_buffer();
        }
    }
}

void binary_writer::flush()
{
    if (is_open())
    {
        if (bit_position < BYTE_SIZE - 1)
        {
            byte_position++;
        }

        flush_buffer();
    }
}
