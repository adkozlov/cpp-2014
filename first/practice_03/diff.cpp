#include <iostream>

void print(int *x, int *y) {
    std::cout << x << " " << y << " diff=" << (x - y) << std::endl;
}

int main() {
    int x = 42;
    int y = 566;

    print(&x, &y);

    print(new int, new int);

    int *p = new int;
    int *q = new int;
    print(p, q);

    return 0;
}
