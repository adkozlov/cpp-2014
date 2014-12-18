#include <memory>

class singleton
{
public:
    static std::shared_ptr<singleton> get_instance();
    
    ~singleton()
    {
    }

    size_t get_data() const
    {
        return data_;
    }

private:
    static std::weak_ptr<singleton> instance_;

    const size_t data_ = 42;

    singleton()
    {
    }
};