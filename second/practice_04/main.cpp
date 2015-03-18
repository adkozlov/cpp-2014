#include <iostream>
#include <cstdlib>

#include "vector.h"

int main()
{
    safe_vector<size_t> vector;
    for (size_t i = 0; i < 10; ++i)
    {
        size_t value = (size_t) rand() % 1000;
        std::cout << value << " ";
        vector.push_back(value);
    }
    std::cout << std::endl;

    while (!vector.empty())
    {
        std::cout << vector.top() << " ";
        vector.pop();
    }
    std::cout << std::endl;

    return 0;
}