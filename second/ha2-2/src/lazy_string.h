#pragma once

#include <memory>

namespace std_utils
{
    using std::shared_ptr;

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
            buffer_t(shared_ptr<T> buffer, size_t size)
                : buffer_(buffer)
                , size_(size)
            {
            }

            ~buffer_t()
            {
            }

            T const& operator[](size_t position) const
            {
                return buffer_.get()[position];
            }

            T& operator[](size_t position)
            {
                return buffer_.get()[position];
            }

            shared_ptr<T> const buffer() const
            {
                return buffer_;
            }

            size_t size() const
            {
                return size_;
            }

        private:
            shared_ptr<T> buffer_;
            size_t size_;
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
        lazy_basic_string(lazy_basic_string<T, Traits> const&);
        lazy_basic_string(lazy_basic_string<T, Traits>&&);
        lazy_basic_string(size_t, T const&);
        lazy_basic_string(T const*);

        ~lazy_basic_string();

        lazy_basic_string<T, Traits>& operator=(lazy_basic_string<T, Traits> const&);
        lazy_basic_string<T, Traits>& operator=(lazy_basic_string<T, Traits>&&);

        lazy_basic_string<T, Traits>& operator+=(lazy_basic_string<T, Traits> const&);

        T const& operator[](size_t) const;
        T& operator[](size_t); // TODO not const version

        bool operator==(lazy_basic_string<T, Traits> const&) const;
        bool operator< (lazy_basic_string<T, Traits> const&) const;

        void swap(lazy_basic_string<T, Traits>&);
        void clear();
        size_t size() const;
        bool empty() const;
        T const* c_str() const;

    private:
        shared_ptr<details::buffer_t<T>> buffer_;

        static shared_ptr<T> allocate_buffer_if_needed(size_t size=0, T* ptr=nullptr);
        static shared_ptr<details::buffer_t<T>> share_allocated_buffer(size_t size=0, shared_ptr<T> ptr=nullptr);
        static shared_ptr<details::buffer_t<T>> init(size_t size=0, T const& symbol=T());
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
    lazy_basic_string<T, Traits>::lazy_basic_string(lazy_basic_string<T, Traits> const& other)
        : buffer_(shared_ptr<details::buffer_t<T>>(other.buffer_))
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(lazy_basic_string<T, Traits>&& other)
        : buffer_(other.buffer_)
    {
        other.clear();
    }

    template <typename T, typename Traits>
    shared_ptr<T> lazy_basic_string<T, Traits>::allocate_buffer_if_needed(size_t size, T* ptr)
    {
        if (ptr == nullptr)
        {
            ptr = new T[size + 1];
        }
        return shared_ptr<T>(ptr, [](T* p){ delete[] p; });
    }

    template <typename T, typename Traits>
    shared_ptr<details::buffer_t<T>> lazy_basic_string<T, Traits>::share_allocated_buffer(size_t size, shared_ptr<T> ptr)
    {
        return std::make_shared<details::buffer_t<T>>(ptr, size + 1);
    }

    template <typename T, typename Traits>
    shared_ptr<details::buffer_t<T>> lazy_basic_string<T, Traits>::init(size_t size, T const& symbol)
    {
        auto ptr = allocate_buffer_if_needed(size);
        for (size_t i = 0; i < size; ++i)
        {
            Traits::assign(ptr.get()[i], symbol);
        }
        Traits::assign(ptr.get()[size], T());
        return share_allocated_buffer(size, ptr);
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(size_t size, T const& symbol)
        : buffer_(lazy_basic_string::init(size, symbol))
    {
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>::lazy_basic_string(T const* other)
    {
        size_t size = Traits::length(other);
        auto ptr = allocate_buffer_if_needed(size);
        Traits::copy(ptr.get(), other, size + 1);
        buffer_ = share_allocated_buffer(size, ptr);
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
        buffer_ = shared_ptr<details::buffer_t<T>>(other.buffer_);
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
        T* buffer = new T[new_size + 1];
        auto ptr = allocate_buffer_if_needed(new_size, buffer);

        Traits::copy(buffer, buffer_->buffer().get(), this_size);
        Traits::copy(buffer + this_size * sizeof(T), other.buffer_->buffer().get(), other_size + 1);
        buffer_ = share_allocated_buffer(new_size, ptr);

        return *this;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>& operator+=(lazy_basic_string<T, Traits>& lhs, T const* rhs)
    {
        lhs += lazy_basic_string<T, Traits>(rhs);
        return lhs;
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits>& operator+=(lazy_basic_string<T, Traits>& lhs, T const& rhs)
    {
        lhs += lazy_basic_string<T, Traits>(1, rhs);
        return lhs;
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
        return lhs + lazy_basic_string<T, Traits>(1, rhs);
    }

    template <typename T, typename Traits>
    lazy_basic_string<T, Traits> operator+(T const& lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return lazy_basic_string<T, Traits>(1, lhs) + rhs;
    }

    template <typename T, typename Traits>
    T const& lazy_basic_string<T, Traits>::operator[](size_t position) const
    {
        return (*buffer_)[position];
    }

    template <typename T, typename Traits>
    T& lazy_basic_string<T, Traits>::operator[](size_t position)
    {
        return (*buffer_)[position];
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator==(lazy_basic_string<T, Traits> const& other) const
    {
        size_t size = (*buffer_).size();
        return (size == (*other.buffer_).size())
               ? Traits::compare((*buffer_).buffer().get(), (*other.buffer_).buffer().get(), size) == 0
               : false;
    }

    template <typename T, typename Traits>
    bool operator!=(lazy_basic_string<T, Traits> const& lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T, typename Traits>
    bool lazy_basic_string<T, Traits>::operator< (lazy_basic_string<T, Traits> const& other) const
    {
        size_t size = (*buffer_).size();
        size_t other_size = (*other.buffer_).size();
        int compared = Traits::compare((*buffer_).buffer().get(), (*other.buffer_).buffer().get(), std::min(size, other_size));
        return (compared < 0)
               ? true
               : (compared == 0) ? size < other_size : false;
    }

    template <typename T, typename Traits>
    bool operator<=(lazy_basic_string<T, Traits> const& lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return (lhs < rhs) || (lhs == rhs);
    }

    template <typename T, typename Traits>
    bool operator> (lazy_basic_string<T, Traits> const& lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs < lhs;
    }

    template <typename T, typename Traits>
    bool operator>=(lazy_basic_string<T, Traits> const& lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs <= lhs;
    }

    template <typename T, typename Traits>
    bool operator==(lazy_basic_string<T, Traits> const& lhs, T const* rhs)
    {
        return lhs == lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    bool operator==(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs == lhs;
    }

    template <typename T, typename Traits>
    bool operator!=(lazy_basic_string<T, Traits> const& lhs, T const* rhs)
    {
        return lhs != lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    bool operator!=(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs != lhs;
    }

    template <typename T, typename Traits>
    bool operator< (lazy_basic_string<T, Traits> const& lhs, T const* rhs)
    {
        return lhs < lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    bool operator< (T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs > lhs;
    }

    template <typename T, typename Traits>
    bool operator<=(lazy_basic_string<T, Traits> const& lhs, T const* rhs)
    {
        return lhs <= lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    bool operator<=(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs >= lhs;
    }

    template <typename T, typename Traits>
    bool operator> (lazy_basic_string<T, Traits> const& lhs, T const* rhs)
    {
        return lhs > lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    bool operator> (T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs < lhs;
    }

    template <typename T, typename Traits>
    bool operator>=(lazy_basic_string<T, Traits> const& lhs, T const* rhs)
    {
        return lhs >= lazy_basic_string<T, Traits>(rhs);
    }

    template <typename T, typename Traits>
    bool operator>=(T const* lhs, lazy_basic_string<T, Traits> const& rhs)
    {
        return rhs <= lhs;
    }

    template <typename T, typename Traits>
    void lazy_basic_string<T, Traits>::swap(lazy_basic_string<T, Traits>& other)
    {
        shared_ptr<details::buffer_t<T>> ptr(buffer_);
        buffer_ = other.buffer_;
        other.buffer_ = ptr;
    }

    template <typename T, typename Traits>
    void swap(lazy_basic_string<T, Traits>& lhs, lazy_basic_string<T, Traits>& rhs)
    {
        lhs.swap(rhs);
    }

    template <typename T, typename Traits>
    void lazy_basic_string<T, Traits>::clear()
    {
        buffer_ = lazy_basic_string::init();
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
        return buffer_->buffer().get();
    }
} // lazy string implementation
