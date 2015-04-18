#pragma once

#include <queue>
#include <algorithm>
#include <iostream>

template <typename P=size_t, typename C=std::less<P>>
struct task_queue
{
public:
    task_queue(C const& comparator=C());
    task_queue(task_queue<P, C>&& o);

    void push(std::function<void()> const& function, P const& priority=P());

    size_t run_one();
    size_t run();

    bool empty() const;

private:
    typedef std::pair<std::function<void()>, P> priority_pair;
    class priority_pair_comparator
    {
    public:
        priority_pair_comparator(C const& comparator)
            : comparator_(comparator)
        {
        }

        bool operator()(priority_pair const& lhs, priority_pair const& rhs)
        {
            return comparator_(lhs.second, rhs.second);
        }
    private:
        C comparator_;
    };

    std::priority_queue<priority_pair, std::vector<priority_pair>, priority_pair_comparator> queue_;

    task_queue(task_queue<P, C> const&);
    task_queue& operator=(task_queue<P, C> const&);
};

template <typename P, typename C>
task_queue<P, C>::task_queue(C const& comparator)
    : queue_(std::priority_queue<priority_pair, std::vector<priority_pair>, priority_pair_comparator>(priority_pair_comparator(comparator)))
{
}

template <typename P, typename C>
task_queue<P, C>::task_queue(task_queue<P, C>&& o)
{
    std::swap(queue_, o.queue_);
}

template <typename P, typename C>
void task_queue<P, C>::push(std::function<void()> const& function, P const& priority)
{
    queue_.push(std::make_pair(function, priority));
}

template <typename P, typename C>
size_t task_queue<P, C>::run_one()
{
    if (empty())
    {
        return 0;
    }
    queue_.top().first();
    queue_.pop();
    return 1;
}

template <typename P, typename C>
size_t task_queue<P, C>::run()
{
    if (empty())
    {
        return 0;
    }

    size_t result = queue_.size();
    while (!queue_.empty())
    {
        run_one();
    }
    return result;
}

template <typename P, typename C>
bool task_queue<P, C>::empty() const
{
    return queue_.empty();
}

template<class forward_iterator>
void sleep_sort(forward_iterator begin, forward_iterator end)
{
    task_queue<typename forward_iterator::value_type> queue;
    for (auto it = begin; it != end; ++it)
    {
        queue.push([&it]{std::cout << *it << std::endl;}, *it);
    }
    queue.run();
}
