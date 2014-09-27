#include <iostream>
#include <fstream>

int fib(const int n)
{
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    return fib(n - 1) + fib(n - 2);
}

int main()
{
    int n = 0;

    std::ifstream infile("input.txt");
    if (infile >> n)
    {
        std::cout << fib(n) << std::endl;
    } else {
        std::cout << "no number" << std::endl;
    }

    return 0;
}
