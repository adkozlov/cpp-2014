#pragma once
#include "b.h"

extern const int a1;
const int a2 = 20;

inline int& global_a() {
#ifdef HAPPY_DEBUG
#define a2 21
#endif
    static int a = add(a1, a2);
    return a;
}
