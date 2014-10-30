#include "lint.h"

using apa::lint;

lint::lint()
    : lint(0)
{
}

lint::lint(int digit)
    : lint((long_t) digit)
{
}

lint::lint(double value)
    : shift_(0)
    , is_negative_(value < 0)
{
    value = std::abs(value);

    if (value >= 1)
    {
        while (value >= 1)
        {
            digits_.push_back(remainder(value, BASE));
            value /= BASE;
        }
    }
    else
    {
        digits_.push_back(0);
    }

    cut_zeroes();
}

lint::lint(long_t digit)
    : shift_(0)
    , is_negative_(digit < 0)
{
    digits_.push_back(std::abs(digit));
}

lint::lint(const lint& value)
    : digits_(value.digits_)
    , shift_(value.shift_)
    , is_negative_(value.is_negative_)
{
}

lint::lint(std::string const& str)
    : shift_(0)
    , is_negative_(false)
{
    int begin = 0;

    is_negative_ = (str[0] == '-');
    if (str[0] == '-' || str[0] == '+')
    {
        begin = 1;
    }

    size_t counter = 0;
    long_t temp = 0;

    digits_.clear();

    for (int i = str.length() - 1; i >= begin; i--)
    {
        if (counter == lint::BASE_LENGTH)
        {
            digits_.push_back(temp);
            temp = 0;
            counter = 0;
        }

        int d = (str[i] - '0');
        for (size_t i = 0; i < counter; i++)
        {
            d *= 10;
        }
        temp += d;
        counter++;
    }

    digits_.push_back(temp);

    cut_zeroes();
}

std::string lint::to_string() const
{
    std::ostringstream sstream;
    if (is_negative_)
    {
        sstream << "-";
    }

    int size = digits_.size();
    for (int i = size - 1; i >= 0; i--)
    {
        std::ostringstream out;
        out << digits_[i];
        std::string str = out.str();

        if (i != size - 1)
        {
            int count = lint::BASE_LENGTH - str.length();
            for (int i = 0; i < count; i++)
            {
                sstream << "0";
            }
        }

        sstream << str;
    }

    std::string zero(BASE_LENGTH, '0');
    for (int i = 0; i < shift_; i++)
    {
        sstream << zero;
    }

    return sstream.str();
}

lint::operator int() const
{
    return digits_[digits_.size() - 1];
}

lint::operator bool() const
{
    return *this != lint(0);
}

lint& lint::operator=(lint const& value)
{
    if (this != &value)
    {
        is_negative_ = value.is_negative_;
        shift_ = value.shift_;
        digits_ = value.digits_;
    }

    return *this;
}

lint lint::abs() const
{
    return (is_negative_ ? -*this : *this);
}


lint lint::pow(long_t power) const
{
    if (power < 0)
    {
        return lint(0);
    }
    else if (power == 1)
    {
        return lint(1);
    }

    lint result = 1;
    lint temp = *this;

    while (power)
    {
        if (power & 1)
        {
            result *= temp;
        }

        temp *= temp;
        power >>= 1;
    }


    return result;
}

int lint::compare_to(lint const& value) const
{
    if (is_negative_ && !value.is_negative_)
    {
        return -1;
    }
    if (!is_negative_ && value.is_negative_)
    {
        return 1;
    }
    if (is_negative_ && value.is_negative_)
    {
        return -(abs().compare_to(value.abs()));
    }

    if (digits_.size() + shift_ != value.digits_.size() + value.shift_)
    {
        if (digits_.size() + shift_ < value.digits_.size() + value.shift_)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    for (int i = digits_.size() - 1; i >= 0; i--)
    {
        int j = i - value.shift_ + shift_;
        long_t d = (0 <= j && j < (int) value.digits_.size()) ? value.digits_[j] : 0;
        
        if (digits_[i] < d)
        {
            return -1;
        }
        if (digits_[i] > d)
        {
            return 1;
        }
    }
    return 0;
}

bool lint::operator<(lint const& value) const
{
    return compare_to(value) < 0;
}

bool lint::operator>(lint const& value) const
{
    return compare_to(value) > 0;
}

bool lint::operator<=(lint const& value) const
{
    return compare_to(value) <= 0;
}

bool lint::operator>=(lint const& value) const
{
    return compare_to(value) >= 0;
}

bool lint::operator==(lint const& value) const
{
    return compare_to(value) == 0;
}

bool lint::operator!=(lint const& value) const
{
    return compare_to(value) != 0;
}

lint& lint::operator+=(lint const& value)
{
    if (value == lint(0))
    {
        return *this;
    }

    if (!is_negative_ && value.is_negative_)
    {
        return *this -= value.abs();
    }
    if (is_negative_ && !value.is_negative_)
    {
        lint temp = abs();
        *this = value;
        return *this -= temp;
    }
    if (is_negative_ && value.is_negative_)
    {
        *this = abs();
        *this += value.abs();
        *this = -(*this);
        return *this;
    }

    if (digits_.size() + shift_ < value.digits_.size() + value.shift_)
    {
        digits_.resize(value.digits_.size() + value.shift_ - shift_, 0);
    }

    if (shift_ > value.shift_)
    {
        digits_.insert(0, shift_ - value.shift_, 0);
        shift_ = value.shift_;
    }

    for (size_t i = 0; i < value.digits_.size(); i++)
    {
        size_t k = i - shift_ + value.shift_;
        digits_[k] += value.digits_[i];
    }

    normalize();
    cut_zeroes();

    return *this;
}

lint& lint::operator-=(lint const& value)
{
    if (value == lint(0))
    {
        return *this;
    }

    if (!is_negative_ && value.is_negative_)
    {
        return *this += value.abs();
    }
    if (is_negative_ && !value.is_negative_)
    {
        *this = -(abs() + value);
        return *this;
    }
    if (is_negative_ && value.is_negative_)
    {
        *this = value.abs() - abs();
        return *this;
    }

    if (value > *this)
    {
        *this = -(value - *this);
        return *this;
    }

    if (shift_ > value.shift_)
    {
        digits_.insert(0, shift_ - value.shift_, 0);
        shift_ = value.shift_;
    }

    for (size_t i = 0; i < value.digits_.size(); i++)
    {
        size_t k = i - shift_ + value.shift_;
        digits_[k] -= value.digits_[i];
    }

    normalize();
    cut_zeroes();

    return *this;
}

lint& lint::operator*=(long_t digit)
{
    if (digit == 0)
    {
        *this = lint(0);

        return *this;
    }

    for (size_t i = 0; i < digits_.size(); i++)
    {
        digits_[i] *= digit;
    }

    normalize();

    return *this;
}

lint& lint::operator*=(lint const& value)
{
    if (value == lint(0))
    {
        *this = lint(0);

        return *this;
    }

    lint temp;

    for (int i = value.digits_.size() - 1; i >= 0; i--)
    {
        temp *= BASE;
        temp += (*this) * value.digits_[i];
    }

    temp.is_negative_ = (temp != lint(0)) && (is_negative_ != value.is_negative_);

    *this = temp;
    return *this;
}

lint& lint::operator/=(lint const& value)
{
    *this = quotient_and_remainder(value).first;

    return *this;
}

lint& lint::operator%=(lint const& value)
{
    *this = quotient_and_remainder(value).second;

    return *this;
}

lint lint::operator+(lint const& value) const
{
    lint result = *this;
    result += value;

    return result;
}

lint lint::operator-(lint const& value) const
{
    lint result = *this;
    result -= value;

    return result;
}

lint lint::operator*(long_t value) const
{
    lint result = *this;
    result *= value;

    return result;
}

lint lint::operator*(lint const& value) const
{
    lint result = *this;
    result *= value;

    return result;
}

lint lint::operator/(lint const& value) const
{
    lint result = *this;
    result /= value;

    return result;
}

lint lint::operator%(lint const& value) const
{
    lint result = *this;
    result %= value;

    return result;
}

lint lint::operator-() const
{
    lint result = *this;
    result.is_negative_ = (result != lint(0)) && !result.is_negative_;

    return result;
}

lint lint::operator+() const
{
    return *this;
}

lint& lint::operator++()
{
    *this += 1;

    return *this;
}

lint lint::operator++(int)
{
    lint result = *this;
    *this += 1;

    return result;
}

lint& lint::operator--()
{
    *this -= 1;

    return *this;
}

lint lint::operator--(int)
{
    lint result = *this;
    *this -= 1;

    return result;
}

std::pair<lint, lint> lint::quotient_and_remainder(lint const& value) const
{
    if (!value)
    {
        throw std::overflow_error("Division by zero exception");
    }

    lint a = this->abs();
    lint b = value.abs();
    lint result;

    int shift = a.digits_.size() - b.digits_.size();
    b <<= shift;

    while (a >= value.abs())
    {
        long_t k = a.find_divisor(b);
        result <<= 1;
        result += lint(k);
        a -= b * k;
        b >>= 1;
        shift--;
    }

    if (shift + 1 > 0)
    {
        result <<= shift + 1;
    }
    result.cut_zeroes();

    result.is_negative_ = (result != lint(0)) && (is_negative_ != value.is_negative_);
    a.is_negative_ = (a != lint(0)) && is_negative_;

    return std::make_pair(result, a);
}

lint& lint::operator<<=(int shift)
{
    shift_ += shift;

    return *this;
}

lint& lint::operator>>=(int shift)
{
    shift_ -= shift;

    return *this;
}

lint lint::operator<<(int shift) const
{
    return lint(*this) <<= shift;
}

lint lint::operator>>(int shift) const
{
    return lint(*this) >>= shift;
}

namespace apa
{
    std::istream& operator>>(std::istream &is, lint &value)
    {
        while (std::isspace(is.peek()))
        {
            is.get();
        }

        std::ostringstream ss;

        if (is.peek() == '-' || is.peek() == '+')
        {
            char sign;
            is.get(sign);
            ss << sign;
        }

        if (!isdigit(is.peek()))
        {
            is.setstate(std::ios_base::badbit);
            return is;
        }

        while (isdigit(is.peek()))
        {
            char temp;
            is.get(temp);
            ss << temp;
        }

        value = lint(ss.str());

        return is;
    }

    std::ostream& operator<<(std::ostream &os, lint const &value)
    {
        os << value.to_string();

        return os;
    }
}

void lint::normalize()
{
    long_t carry;

    for (size_t i = 0; i < digits_.size() - 1; i++)
    {
        if (digits_[i] >= BASE)
        {
            carry = digits_[i] / BASE;
            digits_[i] -= carry * BASE;
            digits_[i + 1] += carry;
        }
        if (digits_[i] < 0)
        {
            carry = std::abs((digits_[i] - BASE + 1) / BASE);
            digits_[i] += carry * BASE;
            digits_[i + 1] -= carry;
        }
    }

    carry = 0;
    while (true)
    {
        size_t last = digits_.size() - 1;

        if (digits_[last] < 0)
        {
            carry = -std::abs((digits_[last] - BASE + 1) / BASE);
            digits_[last] -= carry * BASE;
        }
        if (digits_[last] >= BASE)
        {
            carry = digits_[last] / BASE;
            digits_[last] -= carry * BASE;
        }

        if (carry == 0)
        {
            break;
        }
        else if (carry == -1)
        {
            *this = -((lint(1) << digits_.size()) - *this);
            break;
        }
        else
        {
            digits_.push_back(carry);
            carry = 0;
        }
    }
}

long_t lint::find_divisor(lint const& value) const
{
    long_t left = 0;
    long_t right = BASE;

    while (left + 1 != right)
    {
        long_t m = (left + right) / 2;
        if (*this < value * m)
        {
            right = m;
        }
        else
        {
            left = m;
        }
    }

    return left;
}

void lint::cut_zeroes()
{
    while (digits_.size() > 1 && digits_[digits_.size() - 1] == 0)
    {
        digits_.pop_back();
    }

    if (digits_.size() == 1 && digits_[0] == 0)
    {
        shift_ = 0;
    }
}

lint apa::abs(lint const& value)
{
    return value.abs();
}

lint apa::pow(lint const& value, long_t power)
{
    return value.pow(power);
}

bool apa::operator<(int value, lint const& other)
{
    return other > value;
}

bool apa::operator>(int value, lint const& other)
{
    return other < value;
}

bool apa::operator<=(int value, lint const& other)
{
    return other >= value;
}

bool apa::operator>=(int value, lint const& other)
{
    return other <= value;
}

bool apa::operator==(int value, lint const& other)
{
    return other == value;
}

bool apa::operator!=(int value, lint const& other)
{
    return other != value;
}

lint apa::operator+(int value, lint const& other)
{
    return lint(value) + other;
}

lint apa::operator-(int value, lint const& other)
{
    return lint(value) - other;
}

lint apa::operator*(int value, lint const& other)
{
    return lint(value) * other;
}

lint apa::operator/(int value, lint const& other)
{
    return lint(value) / other;
}

lint apa::operator%(int value, lint const& other)
{
    return lint(value) % other;
}
