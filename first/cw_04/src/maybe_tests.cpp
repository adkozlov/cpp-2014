#include "maybe.h"
#include <string>
#include <iostream>
#include <assert.h>

using namespace utils;

namespace 
{

struct test_obj
{
    test_obj(const char* s) 
        : str(s)
    { 
        ++counter(); 
    }

    test_obj(test_obj const& other)
        : str(other.str)
    {
        ++counter();
    }
    
    ~test_obj() 
    { 
        -- counter(); 
    }

    static void verify_counter()
    {
        std::cout << counter() << std::endl;
        assert(counter() == 0);
    }

    std::string const& to_str() const 
    {
        return str;
    }

private:
    test_obj& operator=(test_obj const&);

    static size_t& counter()
    {
        static size_t counter = 0;
        return counter;
    }

private:
    std::string str;
};



bool operator==(test_obj const& lhs, test_obj const& rhs)
{
    return lhs.to_str() == rhs.to_str();
}

//////////////////////////////////////////////////////////////////////////

void test_simple_construction()
{
    test_obj to("hello");

    maybe<test_obj> x("hello");
    maybe<test_obj> y = to;

    assert(*x == *y);
}

void test_construction()
{
    maybe<test_obj> os_uninit1;
    assert(!os_uninit1);

    maybe<test_obj> os_copy_val1("foobar");
    assert(os_copy_val1);

    maybe<test_obj> os_copy_uninit1(os_uninit1);
    assert(!os_copy_uninit1);
                                     
    maybe<test_obj> os_copy_init1(os_copy_val1);
    assert(os_copy_init1);
    assert(*os_copy_init1 == *os_copy_val1);

}

void test_assignment()
{
    maybe<test_obj> os_uninit1;
    maybe<test_obj> os_uninit2;

    maybe<test_obj> os_copy_val1("foobar");
    assert(!os_uninit2);
    os_uninit2 = os_uninit1;
    assert(!os_uninit2);
    os_uninit2 = os_copy_val1;
    assert(os_uninit2);

    assert(*os_uninit2 == *os_copy_val1);
}

void test_accessors()
{
    maybe<test_obj> os_uninit3;
    assert(!os_uninit3);
    maybe<test_obj> os_val1("111");
    assert(*os_val1 == "111");
    os_val1 = "foobar1";
    assert(*os_val1 == "foobar1");
    assert(os_val1->to_str() == "foobar1");
    os_uninit3 = "foobar2";
    assert(*os_uninit3 == "foobar2");
}

void test_reset()
{
    maybe<test_obj> os_val("123");
    assert(os_val);
    os_val.reset();
    assert(!os_val);
}

void test_swap()
{
    maybe<test_obj> os_val1("123");
    maybe<test_obj> os_val2;
    swap(os_val1, os_val2);
    assert(!os_val1);
    assert(*os_val2 == "123");
}

void test_constness()
{
    const maybe<test_obj> os_init_const("foobar");
    const maybe<test_obj> os_uninit_const;
    assert(os_init_const);
    assert(!os_uninit_const);
    assert(os_init_const->to_str() == "foobar");
}

void test_exception_on_value()
{                   
    maybe<int> x;
    bool caught = false;

    try
    {
        int y = x.value();
        (void)y;
    }
    catch(std::exception const&)
    {
        caught = true;
    }

    assert(caught);
    x = 5;

    int y = x.value();
    (void)y;
}

maybe<double> to_maybe(maybe<double> const& some)
{
    return some;
}

void test_empty_n_initialized()
{
    maybe<double> value = 7;
    value = to_maybe(empty);

    assert(!value.is_initialized());
}

} // 'anonymous'

int main()
{
    try
    {
        test_simple_construction();
        test_construction();
        test_assignment();
        test_accessors();
        test_reset();
        test_swap();
        test_constness();
        test_exception_on_value();
        test_empty_n_initialized();
    }
    catch(...)
    {
        assert(false);
    }

    test_obj::verify_counter();
    return 0;
}
