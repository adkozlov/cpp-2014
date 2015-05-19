#include <iostream>
#include "../src/fn.h"

using namespace fn;

void println(double d)
{
    std::cout << d << std::endl;
}

double divide(double x, double y)
{
    return x / y;
}

int main()
{
    auto println_42 = fn::bind(&println, 42);
    println_42();

    auto divide5_2 = fn::bind(&divide, 5, 2);
    println(divide5_2());

    auto divide5 = fn::bind(&divide, 5);
    println(divide5(3));

    auto divide_2 = fn::bind(&divide, _1, 2.0);
    println(divide_2(5.0));

    return 0;
}