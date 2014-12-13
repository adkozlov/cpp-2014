#pragma once

#include <string>
#include <cstddef>
#include <typeinfo>
#include <type_traits>

namespace utils
{
    class any
    {
        class placeholder
        {
        public:
            virtual ~placeholder();

            virtual std::type_info const& type() const = 0;
            virtual placeholder* clone() const = 0;
        };

        template <typename V>
        class template_placeholder : public placeholder
        {
        public:
            template_placeholder(V const& value)
                :   value_(value)
            {
            }

            std::type_info const& type() const
            {
                return typeid(V);
            }

            placeholder* clone() const
            {
                return new template_placeholder(value_);
            }

        private:
            V value_;

            template<typename U>
            friend U* any_cast(any*);
        };
    public:
        any();
        ~any();

        any(any const&);
        any& operator=(any&);

        template <typename V>
        any(V const& value)
            : placeholder_(new template_placeholder<const V>(value))
        {
        }

        template <typename V>
        any& operator=(V const& value)
        {
            any(value).swap(*this);
            return *this;
        }

        bool empty() const;
        std::type_info const& type() const;

        any& swap(any&);

    private:
        placeholder* placeholder_;

        template<typename V>
        friend V* any_cast(any*);
    };

    class bad_any_cast : public std::bad_cast
    {
    public:
        bad_any_cast(std::string const&);

        virtual const char* what() const throw();

    private:
        std::string name_;
    };

    template <typename V>
    V* any_cast(any* rhs)
    {
        if (rhs != nullptr && rhs->type() == typeid(V))
        {
            return &static_cast<any::template_placeholder<V>*>(rhs->placeholder_)->value_;
        }
        else
        {
            return nullptr;
        }
    }

    template<typename V>
    V const* any_cast(any const* rhs)
    {
        return any_cast<V>(const_cast<any*>(rhs));
    }

    template <typename V>
    V& any_cast(any& rhs) throw(bad_any_cast)
    {
        typedef typename std::remove_reference<V>::type nonref;

        nonref* result = any_cast<nonref>(&rhs);
        if (result == nullptr)
        {
            throw bad_any_cast(typeid(V).name());
        }

        return static_cast<V&>(*result);
    }

    template <typename V>
    V const& any_cast(any const& rhs) throw(bad_any_cast)
    {
        return any_cast<typename std::remove_reference<V>::type const&>(const_cast<any&>(rhs));
    }

    void swap(any&, any&);
} // any type storage class