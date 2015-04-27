#pragma once

#include <memory>

namespace std_utils
{
    using std::shared_ptr;

    template <typename T, typename Traits>
    class lazy_basic_string;

    namespace details
    {
        class itraits : public std::char_traits<char>
        {
        public:
            static bool eq(char a, char b)
            {
                return std::tolower(a) == std::tolower(b);
            }

            static bool lt(char a, char b)
            {
                return std::tolower(a) < std::tolower(b);
            }

            static int compare(char const* lhs, char const* rhs, size_t count)
            {
                for (size_t i = count; i > 0; --i)
                {
                    char a = lhs[i - 1];
                    char b = rhs[i - 1];
                    if (!eq(a, b))
                    {
                        return lt(a, b) ? -1 : 1;
                    }
                }
                return 0;
            }
        };

        template <typename T>
        class buffer_t
        {
        public:
            buffer_t(size_t size)
                : buffer_(new T[size + 1])
                , size_(size + 1)
            {
            }

            ~buffer_t()
            {
                delete[] buffer_;
            }

            T const& operator[](size_t position) const
            {
                return buffer_[position];
            }

            T& operator[](size_t position)
            {
                return buffer_[position];
            }

            size_t size() const
            {
                return size_;
            }

        private:
            T* buffer_;
            size_t size_;
        };

        template <typename T, typename Traits>
        buffer_t<T>* copy(size_t size, T const* buffer)
        {
            buffer_t<T>* new_buffer = new buffer_t<T>(size);
            T* raw_buffer = &(*new_buffer)[0];
            Traits::copy(raw_buffer, buffer, size + 1);
            return new_buffer;
        }

        template <typename T>
        shared_ptr<buffer_t<T>> share_allocated_buffer(buffer_t<T>* buffer)
        {
            return shared_ptr<buffer_t<T>>(buffer, [](buffer_t<T>* b){ delete b; });
        }

        template <typename T, typename Traits>
        shared_ptr<buffer_t<T>> share_allocated_buffer_by_symbol(size_t size=0, T const& symbol=T())
        {
            buffer_t<T>* buffer = new buffer_t<T>(size);
            T* raw_buffer = &(*buffer)[0];
            for (size_t i = 0; i < size; ++i)
            {
                Traits::assign(raw_buffer[i], symbol);
            }
            Traits::assign(raw_buffer[size], T());
            return share_allocated_buffer<T>(buffer);
        }

        template <typename String>
        class proxy
        {
        public:
            typedef typename String::value_type T;
            typedef typename String::traits_type Traits;

            proxy(String* string, size_t position)
                : string_(string)
                , position_(position)
            {
            }

            T& operator=(T const& new_symbol)
            {
                if (string_->buffer_.unique())
                {
                    proxied() = new_symbol;
                }
                else
                {
                    buffer_t<T>* new_buffer = copy<T, Traits>(string_->size(), string_->c_str());
                    Traits::assign((&(*new_buffer)[0])[position_], new_symbol);
                    string_->buffer_ = share_allocated_buffer<T>(new_buffer);
                }

                return proxied();
            }

            operator T()
            {
                return proxied();
            }

        private:
            String* string_;
            size_t position_;

            T& proxied()
            {
                return const_cast<T*>(string_->c_str())[position_];
            }
        };
    } // lazy string utils

    template <typename T, typename Traits = std::char_traits<T>>
    class lazy_basic_string
    {
    public:
        typedef T value_type;
        typedef Traits traits_type;
        typedef size_t size_type;

        typedef T& reference;
        typedef T const& const_reference;
        typedef T* pointer;
        typedef T const* const_pointer;
        typedef T difference_type;

        lazy_basic_string();
        lazy_basic_string(T const&);
        lazy_basic_string(lazy_basic_string<T, Traits> const&);
        lazy_basic_string(lazy_basic_string<T, Traits>&&);
        lazy_basic_string(size_t, T const&);
        lazy_basic_string(T const*);

        ~lazy_basic_string();

        lazy_basic_string<T, Traits>& operator=(lazy_basic_string<T, Traits> const&);
        lazy_basic_string<T, Traits>& operator=(lazy_basic_string<T, Traits>&&);

        lazy_basic_string<T, Traits>& operator+=(lazy_basic_string<T, Traits> const&);

        T const& operator[](size_t) const;
        details::proxy<lazy_basic_string<T, Traits>> operator[](size_t);

        bool operator==(lazy_basic_string<T, Traits> const&) const;
        bool operator!=(lazy_basic_string<T, Traits> const&) const;
        bool operator< (lazy_basic_string<T, Traits> const&) const;
        bool operator<=(lazy_basic_string<T, Traits> const&) const;
        bool operator> (lazy_basic_string<T, Traits> const&) const;
        bool operator>=(lazy_basic_string<T, Traits> const&) const;

        void swap(lazy_basic_string<T, Traits>&);
        void clear();
        size_t size() const;
        bool empty() const;
        T const* c_str() const;

    private:
        typedef details::buffer_t<T> TBuffer;
        shared_ptr<TBuffer> buffer_;

        friend class details::proxy<lazy_basic_string>;
    };

    typedef lazy_basic_string<char> lazy_string;
    typedef lazy_basic_string<wchar_t> lazy_wstring;
    typedef lazy_basic_string<char, details::itraits> lazy_istring;

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string()
        : lazy_basic_string(0, T())
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(T const& symbol)
            : lazy_basic_string(1, symbol)
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(lazy_basic_string<T, Traits> const& other)
        : buffer_(shared_ptr<TBuffer>(other.buffer_))
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(lazy_basic_string<T, Traits>&& other)
        : buffer_(other.buffer_)
    {
        other.clear();
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(size_t size, T const& symbol)
        : buffer_(details::share_allocated_buffer_by_symbol<T, Traits>(size, symbol))
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(T const* other)
    {
        TBuffer* buffer = details::copy<T, Traits>(Traits::length(other), other);
        buffer_ = details::share_allocated_buffer<T>(buffer);
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::~lazy_basic_string()
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>& lazy_basic_string<T, Traits>::operator=(lazy_basic_string<T, Traits> const& other)
    {
        if (this == &other)
        {
            return *this;
        }
        lazy_basic_string<T, Traits> string;
        swap(string);
        buffer_ = shared_ptr<TBuffer>(other.buffer_);
        return *this;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>& lazy_basic_string<T, Traits>::operator=(lazy_basic_string<T, Traits>&& other)
    {
        lazy_basic_string<T, Traits> string;
        swap(string);
        buffer_ = other.buffer_;
        other.clear();
        return *this;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>& lazy_basic_string<T, Traits>::operator+=(lazy_basic_string<T, Traits> const& other)
    {
        size_t this_size = size();
        size_t other_size = other.size();
        size_t new_size = this_size + other_size;
        TBuffer* buffer = new TBuffer(new_size);
        T* raw_buffer = &(*buffer)[0];

        Traits::copy(raw_buffer, c_str(), this_size);
        Traits::copy(raw_buffer + this_size * sizeof(T), other.c_str(), other_size + 1);
        buffer_ = details::share_allocated_buffer<T>(buffer);

        return *this;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits> operator+(lazy_basic_string<T, Traits> const& lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        auto result = lhs;
        result += rhs;
        return result;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits> operator+(lazy_basic_string<T, Traits> const& lhs, T const* rhs)
    {
        return lhs + lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits> operator+(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return lazy_basic_string<T, Traits>(lhs) + rhs;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits> operator+(lazy_basic_string<T, Traits> const& lhs, T const& rhs)
    {
        return lhs + lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits> operator+(T const& lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return lazy_basic_string<T, Traits>(lhs) + rhs;
    }

    template <typename T, typename Traits>
    T const& lazy_basic_string<T, Traits>::operator[](size_t position) const
    {
        return (*buffer_)[position];
    }

    template <typename T, typename Traits>
    details::proxy<lazy_basic_string<T, Traits>> lazy_basic_string<T, Traits>::operator[](size_t position)
    {
        details::proxy<lazy_basic_string<T, Traits>> proxy(this, position);
        return proxy;
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator==(lazy_basic_string<T, Traits> const& other) const
    {
        size_t this_size = size();
        return (this_size == other.size())
               ? Traits::compare(c_str(), other.c_str(), this_size + 1) == 0
               : false;
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator!=(lazy_basic_string<T, Traits> const& other) const
    {
        return !(*this == other);
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator< (lazy_basic_string<T, Traits> const& other) const
    {
        size_t this_size = size();
        size_t other_size = other.size();
        int compared = Traits::compare(c_str(), other.c_str(), std::min(this_size, other_size) + 1);
        return (compared < 0)
               ? true
               : (compared == 0) ? this_size < other_size : false;
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator<=(lazy_basic_string<T, Traits> const& other) const
    {
        return (*this < other) || (*this == other);
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator> (lazy_basic_string<T, Traits> const& other) const
    {
        return other < *this;
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator>=(lazy_basic_string<T, Traits> const& other) const
    {
        return other <= *this;
    }

    template <typename T, typename Traits>
    bool operator==(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs == lhs;
    }

    template <typename T, typename Traits>
    bool operator!=(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs != lhs;
    }

    template <typename T, typename Traits>
    bool operator< (T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs > lhs;
    }

    template <typename T, typename Traits>
    bool operator<=(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs >= lhs;
    }

    template <typename T, typename Traits>
    bool operator> (T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs < lhs;
    }

    template <typename T, typename Traits>
    bool operator>=(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs <= lhs;
    }

    template <typename T, typename Traits>
    void lazy_basic_string<T, Traits>::swap(lazy_basic_string<T, Traits>& other)
    {
        shared_ptr<TBuffer> buffer(buffer_);
        buffer_ = other.buffer_;
        other.buffer_ = buffer;
    }

    template <typename T, typename Traits>
    void swap(lazy_basic_string<T, Traits>& lhs, lazy_basic_string<T, Traits>& rhs)
    {
        lhs.swap(rhs);
    }

    template <typename T, typename Traits>
    void lazy_basic_string<T, Traits>::clear()
    {
        buffer_ = details::share_allocated_buffer_by_symbol<T, Traits>();
    }

    template <typename T, typename Traits>
    size_t lazy_basic_string<T, Traits>::size() const
    {
        return buffer_->size() - 1;
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::empty() const
    {
        return buffer_->size() == 1;
    }

    template <typename T, typename Traits>
    T const* lazy_basic_string<T, Traits>::c_str() const
    {
        return &(*buffer_)[0];
    }
} // lazy string implementation
