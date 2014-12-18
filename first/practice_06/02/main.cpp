#include <cstring>
#include <algorithm>
#include <iostream>

class string_wrapper {
public:

    class node_t {
    public:
        node_t(const char* buffer)
            : count_(0)
            , size_(std::strlen(buffer) + 1)
            , buffer_(new char[size_]) {
            std::strcpy(buffer_, buffer);
        }

        node_t& operator++() {
            count_++;
        }

        node_t& operator--() {
            count_--;
        }

        char operator[](size_t i) const {
            return buffer_[i];
        }

        size_t count() const {
            return count_;
        }

        ~node_t() {
            delete[] buffer_;
            size_ = 0;
            count_ = 0;

            std::cout << "buffer is deleted" << std::endl;
        }

        friend std::ostream& operator<<(std::ostream& out, const node_t& node) {
            out << node.buffer_ << std::endl;

            return out;
        }
    private:
        node_t(node_t const& node) {
        }

        node_t& operator=(node_t const& node) {
        }

    private:
        size_t count_;
        
        size_t size_;
        char* buffer_;
    };

    string_wrapper() : node(nullptr) {
    }

    string_wrapper(const char* buffer)
        : node(new node_t(buffer)) {
    }

    string_wrapper(string_wrapper const& other)
        : node(other.node) {
        ++(*node);
    }

    string_wrapper& operator=(string_wrapper const& other) {
        if (&other != this) {
            node = other.node;
            ++(*node);
        }

        return *this;
    }

    char operator[](size_t i) const {
        return (*node)[i];
    }

    ~string_wrapper() {
        --(*node);

        if (node->count() == 0) {
            delete node;
        }
    }

    string_wrapper& concat(const string_wrapper& other) {
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const string_wrapper& wrapper) {
        out << *(wrapper.node);

        return out;
    }

private:
    node_t* node;
};

int main() {
    const char* hello = "hello";

    string_wrapper wrapper1(hello);
    string_wrapper wrapper2(wrapper1);

    string_wrapper wrapper3;
    wrapper3 = wrapper2;

    std::cout << wrapper3[0] << std::endl;

    return 0;
}
