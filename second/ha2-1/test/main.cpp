#include <iostream>

#include "../src/linked_ptr.h"

struct base
{
    virtual void echo()
    {
        std::cout << "base" << std::endl;
    }

    virtual ~base()
    {}
};

struct derived : base
{
    virtual void echo()
    {
        std::cout << "derived" << std::endl;
    }
};

int main()
{
    derived* d = new derived();
    smart_ptr::linked_ptr<derived> derived_ptr(d);
    if (derived_ptr)
        std::cout << "ok1" << std::endl;

    smart_ptr::linked_ptr<base> base_ptr;
    base_ptr = derived_ptr;
    if (base_ptr)
        std::cout << "ok2 " << base_ptr.unique() << std::endl;

    return 0;
}