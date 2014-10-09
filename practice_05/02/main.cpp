#include <functional>
#include <vector>
#include <iostream>

int square(const int t)
{
    std::cout << t << std::endl;

    return t * t;
}

long sum(const std::vector<int>& container, std::function<int(const int&)> function)
{
    long result = 0;

    for (size_t i = 0; i < container.size(); i++)
    {
        result += function(container[i]);
    }
    
    return result;
}

int main()
{
    std::vector<int> v(5);
    for (size_t i = 0; i < v.size(); i++)
    {
        v[i] = i;
    }

    std::cout << sum(v, square) << std::endl;

    return 0;
}
