#include <cstring>
#include <algorithm>
#include <iostream>

namespace lazy_string {

    class string_wrapper {
    public:

        class node_t {
        public:
            node_t(const char* buffer)
                : count_(1)
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

            char* buffer() const {
                return buffer_;
            }

            size_t size() const {
                return size_;
            }

            char operator[](size_t i) const {
                return get_at(i);
            }

            char get_at(size_t i) const {
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
                out << node.buffer_;

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

        char* c_str() const {
            return node->buffer();
        }

        char operator[](size_t i) const {
            return get_at(i);
        }

        char get_at(size_t i) const {
            return (*node)[i];
        }

        void set_at(size_t i, char c) {

        }

        size_t size() const {
            return node->size();
        }

        string_wrapper operator+=(string_wrapper const& other) const {
            char* s = new char[size() + other.size() - 1];
            strcat(s, c_str());
            strcat(s, other.c_str());

            string_wrapper result = string_wrapper(s);
            delete[] s;

            return result;
        }

        string_wrapper operator+(string_wrapper const& other) {
            return *this += other;
        }

        ~string_wrapper() {
            --(*node);

            if (node->count() == 0) {
                delete node;
            }
        }

        friend std::ostream& operator<<(std::ostream& out, const string_wrapper& wrapper) {
            out << *(wrapper.node);

            return out;
        }

        bool operator<(const string_wrapper& other) const {
            return strcmp(c_str(), other.c_str()) < 0;
        }
        
        bool operator>(const string_wrapper& other) const {
            return other < *this;
        }

        bool operator<=(const string_wrapper& other) const {
            return !(*this > other);
        }

        bool operator>=(const string_wrapper& other) const {
            return !(*this < other);
        }

        bool operator==(const string_wrapper& other) const {
            return !(*this < other && other < *this);
        }

        bool operator!=(const string_wrapper& other) const {
            return !(*this == other);
        }

    private:
        node_t* node;
    };

    int find(const string_wrapper& string, char c) {
        for (size_t i = 0; i < string.size(); ++i) {
            if (string[i] == c) {
                return i;
            }
        }

        return -1;
    }    
} // lazy string implementation

int main() {
    const char* hello = "hello";

    using lazy_string::string_wrapper;

    string_wrapper wrapper1(hello);
    string_wrapper wrapper2("hell");
    
    std::cout << (wrapper1 + wrapper2) << std::endl;

    return 0;
}
