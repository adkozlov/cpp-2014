#include <iostream>
#include "singleton.h"

int main()
{
    std::cout << singleton::get_instance() << std::endl;
    std::cout << singleton::get_instance()->get_data() << std::endl;
}
