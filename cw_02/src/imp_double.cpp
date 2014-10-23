#include "imp_double.h"

using math::imp_double;

imp_double::imp_double()
	: value_(0)
	, precision_(imp_double::eps)
{
}

imp_double::imp_double(const double value)
	: value_(value)
	, precision_(imp_double::eps)
{
}

imp_double::imp_double(const double value, const double precision)
	: value_(value)
	, precision_(precision)
{
}

imp_double::imp_double(const imp_double& other)
    : value_(other.value_)
    , precision_(other.precision_)
{
}

imp_double& imp_double::operator=(const imp_double& other)
{
    if (&other != this) {
        value_ = other.value_;
        precision_ = other.precision_;
    }

    return *this;
}

imp_double::~imp_double()
{
}

bool imp_double::operator<(const imp_double& other) const
{
    return value_ + precision_ < other.value_ + other.precision_;
}

bool imp_double::operator>(const imp_double& other) const
{
    return other < *this;
}

bool imp_double::operator<=(const imp_double& other) const
{
    return !(*this > other);
}

bool imp_double::operator>=(const imp_double& other) const
{
    return !(*this < other);
}

bool imp_double::imp_double::operator==(const imp_double& other) const
{
    return !(*this < other && other < *this);
}

bool imp_double::operator!=(const imp_double& other) const
{
    return !(*this == other);
}

imp_double imp_double::operator-() const
{
	return imp_double(-value_, precision_);
}

imp_double& imp_double::operator+=(const imp_double& other)
{
	value_ += other.value_;
	precision_ += other.precision_;

	return *this;
}

imp_double& imp_double::operator-=(const imp_double& other)
{
	value_ -= other.value_;
	precision_ += other.precision_;
	
	return *this;
}

imp_double& imp_double::operator*=(const imp_double& other)
{
	double v = value_ * other.value_;
	double p = precision_ * value_ + other.precision_ * other.value_;

	value_ = v;
	precision_ = p;

	return *this;
}

imp_double& imp_double::operator/=(const imp_double& other)
{
	double v = value_ / other.value_;
	double p = precision_ * value_ + other.precision_ * other.value_;

	value_ = v;
	precision_ = p;
	
	return *this;
}

imp_double imp_double::operator+(const imp_double& other) const
{
	return imp_double(value_ + other.value_, precision_ + other.precision_);
}

imp_double imp_double::operator-(const imp_double& other) const
{
	return imp_double(value_ - other.value_, precision_ + other.precision_);
}

imp_double imp_double::operator*(const imp_double& other) const
{
	double v = value_ * other.value_;
	double p = precision_ * value_ + other.precision_ * other.value_;

	return imp_double(v, v * p);
}

imp_double imp_double::operator/(const imp_double& other) const
{
	double v = value_ / other.value_;
	double p = precision_ * value_ + other.precision_ * other.value_;

	return imp_double(v, v * p);
}

double imp_double::value() const
{
	return value_;
}

double imp_double::precision() const
{
	return precision_;
}

std::istream& operator>>(std::istream& in, imp_double& d) {
	double value;
	double precision;

    in >> value >> precision;
    d = imp_double(value, precision);

    return in;
}

std::ostream& operator<<(std::ostream& out, const imp_double& d) {
    out << d.value() << "±" << d.precision() << std::endl;

    return out;
}