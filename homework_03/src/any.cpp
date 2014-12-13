#include "any.h"

using namespace utils;

any::placeholder::~placeholder()
{
}

any::any()
        : placeholder_(nullptr)
{
}

any::~any()
{
    if (!empty())
    {
        delete placeholder_;
    }
}

any::any(any const& rhs)
{
    if (!rhs.empty())
    {
        placeholder_ = rhs.placeholder_->clone();
    }
    else
    {
        placeholder_ = nullptr;
    }
}

any& any::operator=(any& rhs)
{
    any(rhs).swap(*this);
    return *this;
}

bool any::empty() const
{
    return placeholder_ == nullptr;
}

std::type_info const& any::type() const
{
    return !empty() ? placeholder_->type() : typeid(void);
}

bad_any_cast::bad_any_cast(std::string const& name)
    : name_(name)
{
}

char const* bad_any_cast::what() const throw()
{
    return (std::string("bad cast to \'") + name_ + std::string("\'")).c_str();
}

any& any::swap(any& rhs)
{
    std::swap(placeholder_, rhs.placeholder_);
    return *this;
}

void utils::swap(any& lhs, any& rhs)
{
    lhs.swap(rhs);
}