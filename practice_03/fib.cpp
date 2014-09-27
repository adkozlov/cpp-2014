#include <iostream>

int main() {
    int fib[20] = {1, 1};

    for (int i = 2; i < 20; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    std::cout << fib[19] << std::endl;

    return 0;
}
