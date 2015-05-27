#include <iostream>
#include <cassert>
#include <vector>
#include <array>
#include <string.h>
#include <algorithm>
#include <list>
#include <iterator>
#include "au_allocator.h"

// part 1
static void test_constructor_forwarding()
{
    struct test_struct {
        test_struct(int a, int b, int c)
                : a_(a)
                , b_(b)
                , c_(c)
        {}
        int a_; int b_; int c_;
    };
    au_allocator alloc;
    test_struct *ptr = alloc.allocate<test_struct>(1, 2, 3);
    assert(ptr->a_ == 1);
    assert(ptr->b_ == 2);
    assert(ptr->c_ == 3);
    alloc.deallocate(ptr);
}

static void test_destructor_call()
{
    struct test_struct {
        test_struct(int *val) : val_(val) {}
        ~test_struct() { *val_ = 0xDEAD; }
        int *val_;
    };
    int val = 0;
    au_allocator alloc;
    test_struct *ptr = alloc.allocate<test_struct>(&val);
    alloc.deallocate(ptr);
    assert(val == 0xDEAD);
}

static void test_stress_validity(bool alloc_can_fail)
{
    typedef size_t alloc1_t;
    typedef std::array<char, 10> alloc2_t;
    typedef std::array<char, 64> alloc3_t;
    typedef std::array<char, 1024> alloc4_t;

    std::vector<alloc1_t*> allocs1;
    std::vector<alloc2_t*> allocs2;
    std::vector<alloc3_t*> allocs3;
    std::vector<alloc4_t*> allocs4;
    au_allocator alloc;
    for(size_t i = 0; i < 10000; ++i)
    {
        alloc1_t *ptr1 = alloc.allocate<alloc1_t>();
        if (ptr1 || !alloc_can_fail)
        {
            allocs1.push_back(ptr1);
            memset(allocs1.back(), 0x11, sizeof(alloc1_t));
        }

        alloc2_t *ptr2 = alloc.allocate<alloc2_t>();
        if (ptr2 || !alloc_can_fail)
        {
            allocs2.push_back(ptr2);
            memset(allocs2.back(), 0x22, sizeof(alloc2_t));
        }

        alloc3_t *ptr3 = alloc.allocate<alloc3_t>();
        if (ptr3 || !alloc_can_fail)
        {
            allocs3.push_back(ptr3);
            memset(allocs3.back(), 0x33, sizeof(alloc3_t));
        }

        alloc4_t *ptr4 = alloc.allocate<alloc4_t>();
        if (ptr4 || !alloc_can_fail)
        {
            allocs4.push_back(ptr4);
            memset(allocs4.back(), 0x44, sizeof(alloc4_t));
        }
    }
    std::for_each(allocs1.begin(), allocs1.end(), [](alloc1_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
            assert(*p == 0x11);
    });
    std::for_each(allocs2.begin(), allocs2.end(), [](alloc2_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
            assert(*p == 0x22);
    });
    std::for_each(allocs3.begin(), allocs3.end(), [](alloc3_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
            assert(*p == 0x33);
    });
    std::for_each(allocs4.begin(), allocs4.end(), [](alloc4_t *ptr) {
        for (char * p = (char*)ptr; p < (char*)(ptr + 1); ++p)
            assert(*p == 0x44);
    });

    assert(!allocs1.empty());
    assert(!allocs2.empty());
    assert(!allocs3.empty());
    assert(!allocs4.empty());

    std::for_each(allocs4.begin(), allocs4.end(), [&alloc](alloc4_t *ptr) {
        alloc.deallocate(ptr);
    });
    std::for_each(allocs3.begin(), allocs3.end(), [&alloc](alloc3_t *ptr) {
        alloc.deallocate(ptr);
    });
    std::for_each(allocs2.begin(), allocs2.end(), [&alloc](alloc2_t *ptr) {
        alloc.deallocate(ptr);
    });
    std::for_each(allocs1.begin(), allocs1.end(), [&alloc](alloc1_t *ptr) {
        alloc.deallocate(ptr);
    });
}

// part 3
//static void test_stl_allocator()
//{
//    au_stl_allocator<int>::value_type v = 0;
//    au_stl_allocator<int>::pointer p = &v;
//    au_stl_allocator<int>::reference r = v;
//    au_stl_allocator<int>::const_pointer cp = &v;
//    au_stl_allocator<int>::const_reference cr = v;
//    au_stl_allocator<int>::size_type s = p - (p + 200);
//    au_stl_allocator<int>::difference_type d = p - (p + 200);
//    (void)d;
//    au_stl_allocator<int>::rebind<float>::other::value_type vf = 20.0f;
//    (void)vf;
//
//    au_allocator alloc;
//    typedef std::array<size_t, 128> list_elem;
//    std::list<list_elem, au_stl_allocator<list_elem>> ll(
//            {
//                    { 1, 2, 3 },
//                    { 4, 5, 6 },
//                    { 7, 8, 9 },
//            },
//            au_stl_allocator<list_elem>(&alloc)
//    );
//    for(size_t i = 0; i < 3; ++i)
//    {
//        for(size_t j = 0; j < 3; ++j)
//        {
//            auto it = ll.begin();
//            std::advance(it, i);
//            assert((*it)[j] == (i * 3 + j + 1));
//        }
//    }
//}

int main()
{
    // part 1
    test_constructor_forwarding();
    test_destructor_call();
    test_stress_validity(true);
    // part 2
//    test_stress_validity(false);
    // part 3
//    test_stl_allocator();
    return 0;
}