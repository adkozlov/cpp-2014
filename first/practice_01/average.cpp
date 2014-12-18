#include <iostream>

const int n = 5;

int main()
{
    float result = 0;

    for (int i = 0; i < n; ++i)
    {
        float f = 0;
        std::cin >> f;
        result += f;
    }

    std::cout << (result / n) << std::endl;

    return 0;
}
