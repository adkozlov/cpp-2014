#include <functional>
#include <vector>
#include <iostream>

void print(const int t)
{
    std::cout << t << std::endl;
}

template<class T>
void for_each(const std::vector<T>& container, std::function<void(const T&)> v)
{
    for (size_t i = 0; i < container.size(); i++)
    {
        v(container[i]);
    }
}

int main()
{
    std::vector<int> v(5);
    for_each<int>(v, [](const int x) { std::cout << x << std::endl; });

    return 0;
}
