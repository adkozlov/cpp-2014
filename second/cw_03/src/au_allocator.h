#pragma once

class au_allocator
{
public:
    explicit au_allocator(size_t max_order=7)
        : max_order_(max_order)
        , buffers_size_(std::max((size_t) 1 << max_order_, (size_t) 4096))
    {
        for (size_t order = 0; order <= max_order_; ++order) {
            buffers_.push_back(new char[buffers_size_]);
            free_.push_back(std::vector<bool>(buffers_size_ / (1 << order), true));
        }
    }

    virtual ~au_allocator()
    {
        for (auto it = buffers_.begin(); it != buffers_.end(); ++it)
        {
            delete[] (*it);
        }
    }

    void* allocate(size_t size)
    {
        size_t size_ceil = nearest_power_of_2(size);
        if (size_ceil > (size_t) 1 << max_order_)
        {
            return new char[size];
        }

        size_t real_size = log(size_ceil);
        size_t block_size = (size_t) (1 << real_size);
        size_t blocks_count = buffers_size_ / block_size;

        for (size_t i = 0; i < blocks_count; ++i)
        {
            if (free_[real_size][i]) {
                free_[real_size][i] = false;
                return buffers_[real_size] + block_size * i;
            }
        }
        return nullptr;
    }

    template <typename T, class... CONSTR_PARAMS>
    T* allocate(CONSTR_PARAMS... constr_params)
    {
        static_assert(sizeof(T) > 0, "T size > 0");
        char* ptr = static_cast<char*>(allocate(sizeof(T)));
        return new (ptr) T(std::forward<CONSTR_PARAMS>(constr_params)...);
    }

    void deallocate(void* ptr, size_t size)
    {
        size_t size_ceil = nearest_power_of_2(size);
        if (size_ceil > (size_t) 1 << max_order_)
        {
            delete[] static_cast<char*>(ptr);
        }

        size_t real_size = log(size_ceil);
        size_t block_size = (size_t) (1 << real_size);
        size_t blocks_count = buffers_size_ / block_size;

        for (size_t i = 0; i < blocks_count; ++i)
        {
            if (ptr == buffers_[real_size] + block_size * i)
            {
                free_[real_size][i] = true;
                return;
            }
        }
    }

    template <typename T>
    void deallocate(T* const ptr)
    {
        ptr->~T();
        deallocate((void*) ptr, sizeof(T));
    }
private:
    size_t max_order_;
    size_t buffers_size_;
    std::vector<char*> buffers_;
    std::vector<std::vector<bool>> free_;

    static size_t log(size_t size)
    {
        size_t result = 0;
        while ((size = size >> 1))
        {
            ++result;
        }
        return result;
    }

    static size_t nearest_power_of_2(size_t size)
    {
        size_t result = 1;
        while (result < size)
        {
            result <<= 1;
        }
        return result;
    }
};