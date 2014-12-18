#pragma once

#include <iostream>
#include <stdexcept>

typedef long long long_t;

namespace apa
{
    class vector_t
    {
    public:
        vector_t();
        explicit vector_t(long_t);

        ~vector_t();

        vector_t(vector_t const&);
        vector_t &operator=(vector_t const&);

        size_t size() const;

        void push_back(long_t);
        void pop_back();

        void clear();
        void resize(size_t, long_t digit = 0);

        void insert(size_t, size_t, long_t);

        long long &operator[](size_t);
        long long operator[](size_t) const;

    private:
        void spread(bool force = false);
        void shrink();

        union
        {
            long_t *digits_;
            long_t digit_;
        } container_;

        size_t size_;
        size_t capacity_;

        static const size_t RESIZE_COEFFICIENT = 2;
    };
} // vector
