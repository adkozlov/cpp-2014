#include <functional>
#include <vector>
#include <iostream>

template<class T>
T fold(const std::vector<T>& container, std::function<T(const T&, const T&)> function)
{
    T result;

    for (size_t i = 0; i < container.size(); i++)
    {
        result = function(result, container[i]);
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

    auto l = [](const int& x, const int& y) { return x + y; };
    std::cout << fold<int>(v, l) << std::endl;

    return 0;
}
