#include <cstring>
#include <algorithm>
#include <iostream>

class string_wrapper {
public:
    string_wrapper(const char* buffer)
        : size_(std::strlen(buffer) + 1)
        , buffer_(new char[size_]) {
        std::strcpy(buffer_, buffer);
    }

    string_wrapper(string_wrapper const& other)
        : size_(other.size_)
        , buffer_(new char[size_]) {
        std::strcpy(buffer_, other.buffer_);
    }

    string_wrapper& operator=(string_wrapper other) {
        std::swap(*this, other);
        return *this;
    }

    ~string_wrapper() {
        delete[] buffer_;
        size_ = 0;
    }

    string_wrapper& concat(const string_wrapper& other) {
        size_t size = size_ + other.size_ - 1;

        char* buffer = new char[size];
        std::strcat(buffer, buffer_);
        std::strcat(buffer, other.buffer_);

        buffer[size - 1] = 0;

        size_ = size;
        buffer_ = buffer;

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const string_wrapper& wrapper) {
        out << "size: " << wrapper.size_ << std::endl;
        out << "data: " << wrapper.buffer_ << std::endl;

        return out;
    }

private:
    size_t size_;
    char* buffer_;
};

int main() {
    const char* hello = "hello";

    string_wrapper wrapper1(hello);
    string_wrapper wrapper2(wrapper1);

    std::cout << wrapper1.concat(wrapper2);

    return 0;
}
