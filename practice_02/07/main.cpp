#include <iostream>
#include "a.h"

static int a = 42;

namespace {
    int b() {
        return 42;
    }
}

int main() {
    std::cout << global_a() << std::endl;
    std::cout << a << " " << b() << std::endl;

    return 0;
}
