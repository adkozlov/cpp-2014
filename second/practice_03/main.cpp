#include <iostream>
#include <vector>
#include <algorithm>

template <typename container_t>
void set_add(container_t& container, typename container_t::value_type value)
{
    auto it = container.begin();
    while (it != container.end() && *it < value)
    {
        ++it;
    }

    if (*it != value)
    {
        container.insert(it, value);
    }
}

template <typename iterator_t>
size_t count_of_equals(iterator_t const& begin,
        iterator_t const& end,
        typename iterator_t::value_type elem)
{
    auto range = equal_range(begin, end, elem);
    return range.second - range.first;
}

template <typename iterator_t>
void replace_if(iterator_t& begin, iterator_t& end, )

template <typename iterator_t>
void print(iterator_t const& begin, iterator_t const& end)
{
    for (auto it = begin; it != end; ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> set({10});
    set_add(set, 16);
    set_add(set, 16);
    set_add(set, 10);
    set_add(set, 5);
    set_add(set, 5);

    print(set.begin(), set.end());

    std::cout << count_of_equals(set.begin(), set.end(), 16) << std::endl;

    return 0;
}