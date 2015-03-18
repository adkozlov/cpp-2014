//
// Created by Andrew Kozlov on 18.03.15.
//

#pragma once

#include <utility>
#include <vector>
#include <algorithm>

template <typename T, typename Comparator=std::greater<T>>
class safe_vector
{
public:
    safe_vector(size_t capacity=10, Comparator comparator=Comparator());
    ~safe_vector();

    safe_vector(safe_vector<T> const&);
    safe_vector& operator=(safe_vector<T> const&);

    safe_vector(safe_vector<T> const&&);
    safe_vector&& operator=(safe_vector<T> const&&);

    void push_back(T const&);
    T& top() const;
    void pop();

    T& operator[](size_t);
    T& operator[](size_t) const;
    size_t size() const;
    bool empty() const;

    void make_heap();
    void push_heap();
    void pop_heap();
private:
    size_t capacity_;
    size_t size_;
    T* buffer_;

    Comparator comparator_;

    static T* new_copy(T const*, size_t, size_t);
};

template <typename T, typename Comparator>
safe_vector<T, Comparator>::safe_vector(size_t capacity, Comparator comparator)
    : capacity_(capacity)
    , size_(0)
    , buffer_(new T[capacity])
    , comparator_(comparator)
{
}

template <typename T, typename Comparator>
safe_vector<T, Comparator>::~safe_vector()
{
    delete[] buffer_;
}

template <typename T, typename Comparator>
safe_vector<T, Comparator>::safe_vector(safe_vector<T> const& vector)
    : capacity_(vector.capacity_)
    , size_(vector.size_)
    , buffer_(new_copy(vector.buffer_, vector.capacity_, vector.capacity_))
    , comparator_(vector.comparator_)
{
}

template <typename T, typename Comparator>
safe_vector<T, Comparator>& safe_vector<T, Comparator>::operator=(safe_vector<T> const& vector)
{
    if (this != &vector)
    {
        T* copied = new_copy(vector.buffer_, vector.capacity_, vector.capacity_);
        delete[] buffer_;
        capacity_ = vector.capacity_;
        size_ = vector.size_;
        buffer_ = copied;
        comparator_ = vector.comparator_;
    }
    return *this;
}

template <typename T, typename Comparator>
void safe_vector<T, Comparator>::push_back(T const& value)
{
    if (capacity_ == size_)
    {
        size_t capacity = capacity_ * 2;
        T* copied = new_copy(buffer_, capacity_, capacity);
        capacity_ = capacity;
        buffer_ = copied;
    }
    buffer_[size_] = value;
    ++size_;
    push_heap();
}

template <typename T, typename Comparator>
T& safe_vector<T, Comparator>::top() const
{
    return buffer_[0];
}

template <typename T, typename Comparator>
void safe_vector<T, Comparator>::pop()
{
    pop_heap();
    --size_;
}

template <typename T, typename Comparator>
T& safe_vector<T, Comparator>::operator[](size_t index)
{
    return buffer_[index];
}

template <typename T, typename Comparator>
T& safe_vector<T, Comparator>::operator[](size_t index) const
{
    return buffer_[index];
}

template <typename T, typename Comparator>
size_t safe_vector<T, Comparator>::size() const
{
    return size_;
}

template <typename T, typename Comparator>
bool safe_vector<T, Comparator>::empty() const
{
    return size_ == 0;
}

template <typename T, typename Comparator>
T* safe_vector<T, Comparator>::new_copy(T const* source, size_t source_size, size_t destination_size)
{
    T* destination = new T[destination_size];
    try
    {
        for (size_t index = 0; index < source_size; ++index)
        {
            *(destination + index) = std::move_if_noexcept(*(source + index));
        }
    }
    catch(...)
    {
        delete[] destination;
        throw;
    }

    return destination;
}

template <typename T, typename Comparator>
void safe_vector<T, Comparator>::make_heap()
{
    std::make_heap(buffer_, buffer_ + size_, comparator_);
}

template <typename T, typename Comparator>
void safe_vector<T, Comparator>::push_heap()
{
    std::push_heap(buffer_, buffer_ + size_, comparator_);
}

template <typename T, typename Comparator>
void safe_vector<T, Comparator>::pop_heap()
{
    std::pop_heap(buffer_, buffer_ + size_, comparator_);
}
