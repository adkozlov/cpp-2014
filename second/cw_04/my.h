#pragma once

namespace my
{
    template <typename T>
    class vector
    {
    public:
        explicit vector(size_t capacity);
        vector();
        vector(const vector<T>& other);
        vector(vector<T>&& other);

        vector<T>& operator=(vector<T> other);
        void swap(vector<T>& other);

        ~vector();

        T& operator[](size_t position);
        const T& operator[](size_t position) const;

        size_t size() const;
        size_t capacity() const;

        void reserve(size_t new_capacity);

        bool empty() const;

        void push_back(const T& element);
        void push_back(T&& element);

        template <typename... Args>
        void emplace_back(Args&&... args);

    private:
        size_t capacity_;
        size_t size_;
        T* buffer_;

        void clear();
        T* new_copy(const T* src, size_t src_size, size_t dst_size, bool move);
    };

    template <typename T>
    vector<T>::vector(size_t capacity)
        : capacity_(capacity)
        , size_(0)
        , buffer_(static_cast<T*>(operator new[] (capacity_ * sizeof(T))))
    {
    }

    template <typename T>
    vector<T>::vector()
        : vector(10)
    {
    }

    template <typename T>
    vector<T>::vector(const vector<T>& other)
        : capacity_(other.capacity_)
        , size_(other.size_)
        , buffer_(new_copy(other.buffer_, size_, other.capacity_, false))
    {
    }

    template <typename T>
    vector<T>::vector(vector<T>&& other)
        : capacity_(other.capacity_)
        , size_(other.capacity_)
        , buffer_(other.buffer_)
    {
        other.capacity_ = 0;
        other.size_ = 0;
        other.buffer_ = nullptr;
    }

    template <typename T>
    vector<T>& vector<T>::operator=(vector<T> other)
    {
        if (this != &other)
        {
            swap(other);
        }
        return *this;
    }

    template <typename T>
    void vector<T>::swap(vector<T>& other)
    {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(buffer_, other.buffer_);
    }

    template <typename T>
    vector<T>::~vector()
    {
        clear();
    }

    template <typename T>
    T& vector<T>::operator[](size_t position)
    {
        return buffer_[position];
    }

    template <typename T>
    const T& vector<T>::operator[](size_t position) const
    {
        return buffer_[position];
    }

    template <typename T>
    size_t vector<T>::size() const
    {
        return size_;
    }

    template <typename T>
    size_t vector<T>::capacity() const
    {
        return capacity_;
    }

    template <typename T>
    void vector<T>::reserve(size_t new_capacity)
    {
        T* new_buffer = new_copy(buffer_, size_, new_capacity, true);
        clear();
        buffer_ = new_buffer;
        capacity_ = new_capacity;
    }

    template <typename T>
    bool vector<T>::empty() const
    {
        return size_ == 0;
    }

    template <typename T>
    void vector<T>::push_back(const T& element)
    {
        if (size_ == capacity_)
        {
            reserve(2 * capacity_);
        }
        new(buffer_ + size_) T(element);
        ++size_;
    }

    template <typename T>
    void vector<T>::push_back(T&& element)
    {
        if (size_ == capacity_)
        {
            reserve(2 * capacity_);
        }
        new(buffer_ + size_) T(std::forward<T>(element));
        ++size_;
    }

    template <typename T>
    template <typename... Args>
    void vector<T>::emplace_back(Args&& ... args)
    {
        if (size_ == capacity_)
        {
            reserve(2 * size_);
        }
        new(buffer_ + size_) T(std::forward<Args>(args)...);
        ++size_;
    }

    template <typename T>
    void vector<T>::clear()
    {
        for (size_t i = 0; i < size_; ++i)
        {
            (buffer_ + i)->~T();
        }
        delete[] buffer_;
    }

    template <typename T>
    T* vector<T>::new_copy(const T* src, size_t src_size, size_t dst_size, bool move)
    {
        T* dst = static_cast<T*>(operator new[] (dst_size * sizeof(T)));
        try
        {
            for (size_t i = 0; i < src_size; ++i)
            {
                new(dst + i) T(move ? std::move_if_noexcept(src[i]) : src[i]);
            }
        }
        catch (...)
        {
            delete[] dst;
            throw;
        }
        return dst;
    }
} // vector
