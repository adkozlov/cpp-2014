template<typename T>
class scoped_ptr
{
    public:

    explicit scoped_ptr(T* data)
        : data_(data)
    {}

    ~scoped_ptr()
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

    private:
    T* data_;

    scoped_ptr& operator=(scoped_ptr const&);
    scoped_ptr(scoped_ptr const&);
};
