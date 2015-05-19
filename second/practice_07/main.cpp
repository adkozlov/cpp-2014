#include <iostream>
#include <mutex>
#include <vector>

template <typename T>
class data_monitor
{
public:
    data_monitor(T& object, std::recursive_mutex& mutex)
        : object_(object)
        , lock_(mutex)
    {
        std::cout << "locked" << std::endl;
    }

    data_monitor(data_monitor<T>&& other)
        : object_(std::move(other.object_))
        , lock_(std::move(other.lock_))
    {}

    data_monitor(data_monitor<T> const&) = delete;

    ~data_monitor()
    {
        std::cout << "unlocked" << std::endl;
    }

    data_monitor<T> operator=(data_monitor<T> other)
    {
        swap(other);
        return *this;
    }

    data_monitor<T>& operator=(data_monitor<T> const&) = delete;

    T& operator*() const
    {
        return object_;
    }

    T* operator->() const
    {
        return &object_;
    }

private:
    T& object_;
    std::unique_lock<std::recursive_mutex> lock_;

    void swap(data_monitor<T>& other)
    {
        std::swap(object_, other.object_);
        std::swap(lock_, other.lock_);
    }
};

template <typename T>
class thread_safe_obj
{
public:
    thread_safe_obj(T const& object)
            : object_(object)
    {}

    thread_safe_obj(T&& object)
            : object_(std::forward<T>(object))
    {}

    data_monitor<T> monitor()
    {
        return data_monitor<T>(object_, mutex_);
    }

    data_monitor<const T> const_monitor()
    {
        return data_monitor<const T>(object_, mutex_);
    }

private:
    T& object_;
    std::recursive_mutex mutex_;
};

int main()
{
    thread_safe_obj<std::string> o(std::string("abc"));
    auto x = o.monitor();
    std::cout  << x->size() << " " << (*x).size() << std::endl;

    return 0;
}