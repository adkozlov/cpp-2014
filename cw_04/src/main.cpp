#include <iostream>

#include "maybe.h"

using utils::maybe;
using utils::empty;

int main()
{
    maybe<int> monad(5);
    maybe<int> monad2(monad);

    maybe<int> monad3 = monad;

    std::cout << *monad3 << std::endl;
    monad3.reset();

    maybe<int> monad4(empty);
    std::cout << (bool) monad4 << " " << !monad3 << std::endl;

    std::cout << monad2.value() << std::endl;

    return 0;
}
