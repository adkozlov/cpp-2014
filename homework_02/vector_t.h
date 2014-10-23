#pragma once

#include <iostream>
#include <stdexcept>

class vector_t
{
public:
    vector_t();

    vector_t(long long);

    vector_t(const vector_t &);

    ~vector_t();

    vector_t &operator=(const vector_t &);

    size_t size() const;

    void push_back(long long);

    void pop_back();

    void swap(vector_t &);

    void clear();

    void resize(size_t, long long);

    void insert(int, int, long long);

    long long &operator[](size_t);

    long long operator[](size_t) const;

private:
    void spread(bool force = false);

    void shrink();

    union
    {
        long long *digits_;
        long long digit_;
    } container_;

    size_t size_;
    size_t capacity_;

    static const size_t RESIZE_COEFFICIENT = 2;
};
