#include <stdexcept>
#include <string>
#include <new>
#include <algorithm>

namespace utils
{
    class empty_t
    {
    };

    const empty_t empty = empty_t();

    class bad_maybe_value : public std::runtime_error
    {
    public:
        bad_maybe_value()
                : std::runtime_error::runtime_error("not initialized value")
        {
        }
    };

    template <typename T>
    class maybe
    {
    public:
        maybe()
        {
            reset();
        }

        ~maybe()
        {
        }

        maybe(const T& data)
        {
            new (data_) T(data);
            is_empty_ = false;
        }

        maybe(const empty_t& data)
            : maybe()
        {
        }

        maybe(const maybe& data)
        {
            if (data.is_initialized())
            {
                new (data_) T(data.value());
                is_empty_ = false;
            }
            else
            {
                is_empty_ = true;
            }
        }

        bool is_initialized() const
        {
            return !is_empty_;
        }

        explicit operator bool() const
        {
            return is_initialized();
        }

        bool operator!() const
        {
            return !is_initialized();
        }

        const maybe& operator=(const T& data)
        {
            new (data_) T(data);
            is_empty_ = false;

            return *this;
        }

        const maybe& operator=(const empty_t& data)
        {
            reset();

            return *this;
        }

        const T* get_pointer() const
        {
            return reinterpret_cast<const T*>(data_);
        }

        T* get_pointer()
        {
            return reinterpret_cast<T*>(data_);
        }

        const T& get() const
        {
            return *get_pointer();
        }

        T& get()
        {
            return *get_pointer();
        }

        const T* operator->() const
        {
            return get_pointer();
        }

        T* operator->()
        {
            return get_pointer();
        }

        const T& operator*() const
        {
            return get();
        }

        T& operator*()
        {
            return get();
        }

        void reset()
        {
            for (size_t i = 0; i < sizeof(T); i++)
            {
                data_[i] = 0;
            }

            is_empty_ = true;
        }

        const T& value() const
        {
            if (is_initialized())
            {
                return get();
            }
            else
            {
                throw bad_maybe_value();
            }
        }

        T& value()
        {
            if (is_initialized())
            {
                return get();
            }
            else
            {
                throw bad_maybe_value();
            }
        }

        void swap(maybe<T>& data)
        {
            std::swap(*this, data);
        }

    private:
        char data_[sizeof(T)];
        bool is_empty_;
    };

    template <typename T>
    void swap(maybe<T>& first, maybe<T>& second)
    {
        first.swap(second);
    }
}