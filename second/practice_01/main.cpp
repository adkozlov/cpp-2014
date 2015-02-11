#include <iostream>
#include <set>
#include <vector>
#include <list>
#include <unordered_set>

template <typename forward_iterator, typename comparator = std::less<typename forward_iterator::value_type>>
void tree_sort(forward_iterator begin, forward_iterator end)
{
    std::multiset<typename forward_iterator::value_type, comparator> sorting_set;
    for (auto it = begin; it != end; ++it)
    {
        sorting_set.insert(*it);
    }

    auto result_it = begin;
    for (auto it = sorting_set.begin(); it != sorting_set.end(); ++it)
    {
        *result_it = *it;
        ++result_it;
    }
}

template <typename type, typename predicate = std::equal_to<type>>
void remove_duplicates(std::list<type>& list)
{
    std::unordered_set<type, std::hash<type>, predicate, std::allocator<type>> unique;
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        auto pair = unique.insert(*it);
        if (!pair.second)
        {
            list.erase(it);
        }
    }
}

bool is_palindrome(const std::string& s)
{
    auto fwd_it = s.begin();
    for (auto rvd_it = s.rbegin(); fwd_it != s.end(); ++fwd_it, ++rvd_it)
    {
        if (*fwd_it != *rvd_it)
        {
            return false;
        }
    }

    return true;
}

template <typename iterator_type>
class alpha_iterator : public std::iterator<std::forward_iterator_tag, typename std::iterator_traits<iterator_type>::value_type>
{
public:
    alpha_iterator(iterator_type begin, iterator_type end)
        : begin(begin)
        , end(end)
    {
    }

    alpha_iterator(iterator_type end)
        : alpha_iterator(end, end)
    {
    }

    alpha_iterator& operator++()
    {
        ++begin;
        skip();

        return *this;
    }

    alpha_iterator operator++(int)
    {
        alpha_iterator result(*this);
        operator++();

        return result;
    }

    bool operator==(const alpha_iterator& it)
    {
        return begin == it.begin;
    }

    bool operator!=(const alpha_iterator& it)
    {
        return !operator==(it);
    }

    char& operator*()
    {
        skip();
        return *begin;
    }

    char* operator->()
    {
        return &operator*();
    }

private:
    iterator_type begin;
    iterator_type end;

    void skip()
    {
        while (begin != end && !isalpha(*begin))
        {
            ++begin;
        }
    }
};

int main()
{
    //std::vector<int> elements = {8, 15, 4, 42, 23, 16, 16};
    //tree_sort<std::vector<int>::iterator>(elements.begin(), elements.end());

    std::string s = "1a22ba c aba1";
    for (auto it = alpha_iterator<std::string::iterator>(s.begin(), s.end()); it != alpha_iterator<std::string::iterator>(s.end()); ++it)
    {
        std::cout << *it;
    }
    std::cout << std::endl;

    return 0;
}