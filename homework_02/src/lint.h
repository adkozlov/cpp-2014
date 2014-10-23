#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>

#include "vector_t.h"

typedef long long long_t;

namespace apa
{
    class lint
    {
    public:
        lint();

        lint(int);
        explicit lint(long_t);
        explicit lint(std::string const&);

        std::string to_string() const;

        lint(lint const&);
        lint &operator=(lint const&);

	    explicit operator int() const;
        explicit operator bool() const;

        lint abs() const;
        lint operator+() const;
        lint operator-() const;

        lint pow(long_t) const;

        bool operator<(lint const&) const;
        bool operator>(lint const&) const;
        bool operator<=(lint const&) const;
        bool operator>=(lint const&) const;
        bool operator==(lint const&) const;
        bool operator!=(lint const&) const;

        lint& operator+=(lint const&);
        lint& operator-=(lint const&);
        lint& operator*=(long_t);
        lint& operator*=(lint const&);
        lint& operator/=(lint const&);
        lint& operator%=(lint const&);

        lint operator+(lint const&) const;
        lint operator-(lint const&) const;
        lint operator*(long_t) const;
        lint operator*(lint const&) const;
        lint operator/(lint const&) const;
        lint operator%(lint const&) const;

        lint& operator++();
        lint operator++(int);

        lint& operator--();
        lint operator--(int);

        friend std::istream& operator>>(std::istream &, lint&);
        friend std::ostream& operator<<(std::ostream &, lint const&);

    private:
        vector_t digits_;
        int shift_;
        bool is_negative_;

        static const long_t BASE = 1000000000;
        static const size_t BASE_LENGTH = 9;

        int compare_to(lint const&) const;

        void normalize();
        void cut_zeroes();

        long_t find_divisor(lint const&) const;
        std::pair<lint, lint> quotient_and_remainder(lint const&) const;

        lint& operator<<=(int);
        lint& operator>>=(int);

        lint operator<<(int) const;
        lint operator>>(int) const;
    };

    lint abs(lint const&);
    lint pow(lint const&, long_t);

    bool operator<(int, lint const&);
    bool operator>(int, lint const&);
    bool operator<=(int, lint const&);
    bool operator>=(int, lint const&);
    bool operator==(int, lint const&);
    bool operator!=(int, lint const&);

    lint operator+(int, lint const&);
    lint operator-(int, lint const&);
    lint operator*(int, lint const&);
    lint operator/(int, lint const&);
    lint operator%(int, lint const&);
} // arbitrary-precision arithmetic