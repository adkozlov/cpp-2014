#include "vector_t.h"

vector_t::vector_t() : size_(0), capacity_(1)
{
}

vector_t::vector_t(long long digit) : size_(1), capacity_(1)
{
    container_.digit_ = digit;
}

vector_t::vector_t(const vector_t &vector) : size_(vector.size_), capacity_(vector.capacity_)
{
    if (capacity_ == 1)
    {
        container_.digit_ = vector.container_.digit_;
    }
    else
    {
        container_.digits_ = new long long[capacity_];
        for (size_t i = 0; i < size_; i++)
        {
            container_.digits_[i] = vector.container_.digits_[i];
        }
    }
}

vector_t::~vector_t()
{
    if (capacity_ > 1)
    {
        delete[] container_.digits_;
    }
}

vector_t &vector_t::operator=(const vector_t &vector)
{
    if (this == &vector)
    {
        return *this;
    }

    if (capacity_ > 1)
    {
        delete[] container_.digits_;
    }

    size_ = vector.size_;
    capacity_ = vector.capacity_;
    if (capacity_ > 1)
    {
        container_.digits_ = new long long[capacity_];
    }

    for (size_t i = 0; i < vector.size(); i++)
    {
        (*this)[i] = vector[i];
    }

    return *this;
}

size_t vector_t::size() const
{
    return size_;
}

void vector_t::push_back(long long digit)
{
    spread();
    size_++;
    (*this)[size_ - 1] = digit;
}

void vector_t::pop_back()
{
    if (size_ == 0)
    {
        throw std::runtime_error("error: vector is empty\n");
    }

    size_--;
    shrink();
}

void vector_t::swap(vector_t &vector)
{
    std::swap(container_, vector.container_);
    std::swap(size_, vector.size_);
    std::swap(capacity_, vector.capacity_);
}

void vector_t::clear()
{
    if (capacity_ > 1)
    {
        delete[] container_.digits_;
        capacity_ = 1;
    }
    size_ = 0;
}

void vector_t::resize(size_t size, long long digit)
{
    while (capacity_ < size)
    {
        spread(true);
    }

    size_t tmp = size_;
    size_ = size;

    for (size_t i = tmp; i < size_; i++)
    {
        (*this)[i] = digit;
    }

    shrink();
}

void vector_t::insert(int position, int count, long long digit)
{
    size_t tmp = size_;
    resize(size_ + count, 0);
    for (int i = tmp - 1; i >= position; i--)
    {
        (*this)[i + count] = (*this)[i];
    }
    for (int i = position; i < count; i++)
    {
        (*this)[i] = digit;
    }
}

long long &vector_t::operator[](size_t i)
{
    if (i >= size_)
    {
        throw std::runtime_error("error: index is out of bounds!");
    }

    if (capacity_ == 1)
    {
        return container_.digit_;
    }

    return container_.digits_[i];
}

long long vector_t::operator[](size_t i) const
{
    if (i >= size_)
    {
        throw std::runtime_error("error: index is out of bounds!");
    }

    if (capacity_ == 1)
    {
        return container_.digit_;
    }

    return container_.digits_[i];
}

void vector_t::spread(bool force)
{
    if (!force && size_ < capacity_)
    {
        return;
    }

    long long *new_digits = new long long[RESIZE_COEFFICIENT * capacity_];
    if (capacity_ == 1)
    {
        new_digits[0] = container_.digit_;
    }
    else
    {
        for (size_t i = 0; i < size_; i++)
        {
            new_digits[i] = container_.digits_[i];
        }

        delete[] container_.digits_;
    }

    container_.digits_ = new_digits;
    capacity_ *= RESIZE_COEFFICIENT;
}

void vector_t::shrink()
{
    if (capacity_ == 1 || size_ * RESIZE_COEFFICIENT * RESIZE_COEFFICIENT >= capacity_)
    {
        return;
    }

    if (capacity_ / RESIZE_COEFFICIENT == 1)
    {
        long long new_digit = container_.digits_[0];
        delete[] container_.digits_;
        container_.digit_ = new_digit;
    }
    else
    {
        long long *new_digits = new long long[capacity_ / RESIZE_COEFFICIENT];
        for (size_t i = 0; i < size_; i++)
        {
            new_digits[i] = container_.digits_[i];
        }
        delete[] container_.digits_;
        container_.digits_ = new_digits;
    }

    capacity_ /= RESIZE_COEFFICIENT;
}