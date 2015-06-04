#include <iostream>

#include "my.h"

using namespace my;

struct temp_t
{
    temp_t()
        : a_(-1)
    {
    }

    temp_t(int a)
        : a_(a)
    {
    }

    temp_t(const temp_t& other) = default;
    temp_t& operator=(const temp_t& other) = default;

    int a_;

};

int main()
{
    int t = 5;
    vector<int> v1 = vector<int>();
    v1.push_back(t);

    for (size_t i = 0; i < v1.size(); ++i)
    {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl << v1.capacity() << std::endl;

    vector<int> v2 = v1;
    for (size_t i = 0; i < 10; ++i)
    {
        v2.push_back((int) i);
    }
    for (size_t i = 0; i < v2.size(); ++i)
    {
        std::cout << v2[i] << " ";
    }
    std::cout << std::endl << v2.size() << " " << v2.capacity() << std::endl;

    temp_t temp = temp_t(1);
    vector<temp_t> v3 = vector<temp_t>();
    v3.push_back(temp);

    for (size_t i = 0; i < v3.size(); ++i)
    {
        std::cout << v3[i].a_ << " ";
    }
    std::cout << std::endl;

    return 0;
}