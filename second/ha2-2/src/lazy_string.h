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

        template <typename T, typename Traits>
        class buffer_t
        {
        public:
            buffer_t(size_t size=0)
                : buffer_(new T[size + 1])
                , size_(size + 1)
            {
                Traits::assign(buffer_[size_], T());
            }

            buffer_t(T const* other)
                : buffer_t(Traits::length(other))
            {
                Traits::copy(buffer_, other, size_);
            }

            buffer_t(T const* first, T const* second)
                : buffer_t(Traits::length(first) + Traits::length(second))
            {
                size_t first_size = Traits::length(first);
                Traits::copy(buffer_, first, first_size);
                Traits::copy(buffer_ + first_size * sizeof(T), second, Traits::length(second));
            }

            buffer_t(size_t size, T const& symbol)
                : buffer_t(size)
            {
                for (size_t i = 0; i < size; ++i)
                {
                    replace(i, symbol);
                }
            }

            buffer_t(buffer_t const& other)
                : buffer_t(other.buffer_)
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
                return size_ - 1;
            }

            void replace(size_t position, T const& symbol)
            {
                Traits::assign(buffer_[position], symbol);
            }

        private:
            T* buffer_;
            size_t size_;
        };

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

            T& operator=(T const& symbol)
            {
                string_->replace(position_, symbol);
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

        lazy_basic_string<T, Traits>& operator=(lazy_basic_string<T, Traits>);

        lazy_basic_string<T, Traits>& operator+=(lazy_basic_string<T, Traits> const&);

        T const& operator[](size_t) const;
        details::proxy<lazy_basic_string<T, Traits>> operator[](size_t);
        lazy_basic_string<T, Traits>& replace(size_t, T const&);

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
        typedef details::buffer_t<T, Traits> TBuffer;

        shared_ptr<TBuffer> buffer_;
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
        : buffer_(shared_ptr<TBuffer>(new TBuffer(size, symbol)))
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(T const* other)
        : buffer_(shared_ptr<TBuffer>(new TBuffer(other)))
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>& lazy_basic_string<T, Traits>::operator=(lazy_basic_string<T, Traits> other)
    {
        if (this != &other)
        {
            swap(other);
        }
        return *this;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>& lazy_basic_string<T, Traits>::operator+=(lazy_basic_string<T, Traits> const& other)
    {
        buffer_ = shared_ptr<TBuffer>(new TBuffer(c_str(), other.c_str()));
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
    lazy_basic_string<T, Traits>& lazy_basic_string<T, Traits>::replace(size_t position, T const& symbol)
    {
        if (buffer_.unique())
        {
            buffer_->replace(position, symbol);
        }
        else
        {
            TBuffer* buffer = new TBuffer(*buffer_);
            buffer->replace(position, symbol);
            buffer_ = shared_ptr<TBuffer>(buffer);
        }
        return *this;
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
        std::swap(buffer_, other.buffer_);
    }

    template <typename T, typename Traits>
    void swap(lazy_basic_string<T, Traits>& lhs, lazy_basic_string<T, Traits>& rhs)
    {
        lhs.swap(rhs);
    }

    template <typename T, typename Traits>
    void lazy_basic_string<T, Traits>::clear()
    {
        buffer_ = shared_ptr<TBuffer>(new TBuffer());
    }

    template <typename T, typename Traits>
    size_t lazy_basic_string<T, Traits>::size() const
    {
        return buffer_->size();
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::empty() const
    {
        return buffer_->size() == 0;
    }

    template <typename T, typename Traits>
    T const* lazy_basic_string<T, Traits>::c_str() const
    {
        return &(*buffer_)[0];
    }
} // lazy string implementation
