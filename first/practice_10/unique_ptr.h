template<typename T>
class unique_ptr
{
    public:

    explicit unique_ptr(T* data)
        : data_(data)
    {}

    ~unique_ptr()
    {
        reset();
    }

    explicit operator bool() const
    {
        return data_ != nullptr;
    }

    T* get() const
    {
        return data_;
    }

    T& operator*() const
    {
        return *data_;
    }

    T* operator->() const
    {
        return get();
    }

    void reset()
    {
        if (*this)
        {
            delete data_;
            data_ = nullptr;
        }
    }

    void reset(T* data)
    {
        reset();
        data_ = data;
    }

    unique_ptr& operator=(unique_ptr& ptr)
    {
        unique_ptr result(ptr.data_);
        ptr.data_ = nullptr;
    }

    unique_ptr(unique_ptr& ptr)
        : data_(ptr.data_)
    {
        ptr.data_ = nullptr;
    }

    private:
    T* data_;
};
