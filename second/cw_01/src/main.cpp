#include <iostream>
#include <cassert>

#include "task_queue.h"

void test_empty()
{
    task_queue<> queue{};
    assert(queue.empty());
    assert(queue.run_one() == 0);
    assert(queue.run() == 0);
}

void test_one_element()
{
    task_queue<> queue{};

    bool executed = false;
    queue.push([&executed]{ executed = true; });
    assert(!queue.empty());
    assert(queue.run_one() == 1);
    assert(executed);
    assert(queue.empty());
    assert(queue.run() == 0);

    executed = false;
    queue.push([&executed]{ executed = true; });
    assert(queue.run() == 1);
    assert(executed);
    assert(queue.empty());
}

void test_many_elements()
{
    task_queue<> queue{};
    int result_loop = 0;
    int result_queue = 0;
    auto iterations = 100;
    for (int i = 0; i < iterations; ++i)
    {
        if (i % 2 == 0)
        {
            queue.push([&result_queue, i] {result_queue *= i; });
            result_loop *= i;
        } else
        {
            queue.push([&result_queue, i] {result_queue += i; });
            result_loop += i;
        }
    }
    assert(iterations == queue.run());
    assert(queue.empty());
    assert(result_loop = result_queue);
}

void test_priority_custom_struct()
{
    struct foo
    {
        int a;
    };

    struct foo_compare
    {
        bool operator()(const foo& lhs, const foo& rhs) const {
            return lhs.a < rhs.a;
        }
    };

    task_queue<foo, foo_compare> queue{foo_compare()};
    int initial = 1;
    int result = 0;
    queue.push([&result, initial]{ result = initial; }, foo{ 3 });
    queue.push([&result]{ result += 2; }, foo{ 2 });
    queue.push([&result]{ result *= 3; }, foo{ 1 });
    assert(!queue.empty());
    queue.run();
    assert(result == (initial + 2) * 3);

    assert(queue.empty());
    queue.push([&result, initial]{ result = initial; }, foo{ 3 });
    queue.push([&result]{ result += 2; }, foo{ 1 });
    queue.push([&result]{ result *= 3; }, foo{ 2 });
    queue.run();
    assert(result == (initial * 3) + 2);
}

void test_priority_many_elements()
{
    std::vector<int> data(100);
    srand(42);
    generate(data.begin(), data.end(), rand);
    task_queue<int, std::greater<int>> queue;
    int result = 0;
    for (auto num : data)
    {
        queue.push([&result, num]{result = num; }, num);
    }
    sort(data.begin(), data.end());
    for (int i = 0; i < data.size(); ++i)
    {
        queue.run_one();
        assert(result == data[i]);
    }
    assert(queue.empty());
}

template<class fwd_it>
void sort_and_compare(fwd_it beg, fwd_it end)
{
    std::vector<typename fwd_it::value_type> reference(beg, end);
    std::sort(reference.begin(), reference.end());
    sleep_sort(beg, end);
    assert(std::equal(reference.begin(), reference.end(), beg));
}

template<class T>
void sort_and_compare(std::vector<T> vector)
{
    sort_and_compare(vector.begin(), vector.end());
}

void test_sleep_sorting()
{
    sort_and_compare(std::vector<int>());

    std::vector<int> data = { 1 };
    sort_and_compare(data);

    data = { 1, 1 };
    sort_and_compare(data);

    data = { 1, 2, 3, 4, 5 };
    sort_and_compare(data);

    data = { 5, 4, 3, 2, 1 };
    sort_and_compare(data);

    srand(42);
    data.resize(100);
    generate(data.begin(), data.end(), rand);
    sort_and_compare(data);
}

int main()
{
    test_empty();
    test_one_element();
    test_many_elements();
    test_priority_custom_struct();
    test_priority_many_elements();
    test_sleep_sorting();

    return 0;
}