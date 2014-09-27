template<class T> class scoped_ptr {
//    scoped_ptr(const scoped_ptr& p);
//    const scoped_ptr& operator=(const scoped_ptr& p);

    T *t;
public:
    explicit scoped_ptr(T *p = 0);
    ~scoped_ptr();

    T& operator*() const;
    T* operator->() const;
    T* get() const;

    bool is_null() const;

//    void swap(scoped_ptr &p);
};

//template<class T> void swap(scoped_ptr<T> &p, scoped_ptr<T> &q);
