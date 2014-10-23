#include <iostream>
#include <sstream>
#include <assert.h>

#include "lint.h"

using apa::lint;

void test_construction()
{
    lint x;
    const lint y(x);

    x = y;
    x = x;
}

void test_conversions()
{
    int ordinary = 42;
    lint long_int(ordinary);
    long_int = ordinary;

    ordinary = static_cast<int>(long_int); // ok

    std::string s("-15");
    lint z(s);

    z.to_string() == s;
}

void failed_conversions_test()
{
    int ordinary = 42;
    std::string str = "-42";

    lint x = 5;

    //ordinary = x; // must be compilation error!
    //x = str; // must be compilation error!
    //str = x; // must be compilation error!

    std::cout << ordinary << std::endl;
}

void test_comparison()
{
    lint long_int1 = 2;
    lint long_int2 = 3;;

    // all comparisons
    bool check = false;

    check = 3.14 < long_int1;
    check = long_int1 != long_int2;
    check = 5 > long_int1;
    check = long_int1 == long_int2;
    check = 5 <= long_int2;
    check = long_int1 >= 6;

    std::cout << check << std::endl;
}

void test_check()
{
    lint value = 0;
    if (value)
        std::cout << "not zero value" << std::endl;
}

void test_inc_n_dec()
{
    lint x = 42;

    lint y = ++x;
    assert(y == 42 && x == 43);

    y = x--;
    assert(y == 43 && x == 42);
}

void test_op_n_eq()
{
    lint a = 123456;
    const lint b = 789012;
    lint c = a * b;

    a *= b;
    a += b;
    c /= a;
    c -= c;
    a /= c; // division by zero
}

void test_bin_op()
{
    lint a = 123456789;
    const lint b = 987654321;

    lint c = a * b;
    lint d = 3.14 + c;

    c = a * b;
    c = a + b;
    c = c / a;
    c = c + (-c);
    c = a / c; // division by zero

    c = +c;
}

void test_free_func()
{
    apa::lint a = -42;
    bool check = (-a == abs(a));

    std::cout << check << std::endl;

    apa::lint c = pow(a, 0);
}

void test_io()
{
    std::stringstream s;

    const lint a = -123454321;
    s << a;

    lint b;
    s >> b;

    bool check = a == b;

    std::cout << check << std::endl;
}

int main()
{
    return 0;
}
