//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <deque>

// Optimization for deque::iterators

// template <class InputIterator, class OutputIterator>
//   OutputIterator
//   move_backward(InputIterator first, InputIterator last, OutputIterator result);

#include <deque>
#include <cassert>

#include "test_iterators.h"

std::deque<int>
make(int size, int start = 0 )
{
    const int b = 4096 / sizeof(int);
    int init = 0;
    if (start > 0)
    {
        init = (start+1) / b + ((start+1) % b != 0);
        init *= b;
        --init;
    }
    std::deque<int> c(init, 0);
    for (int i = 0; i < init-start; ++i)
        c.pop_back();
    for (int i = 0; i < size; ++i)
        c.push_back(i);
    for (int i = 0; i < start; ++i)
        c.pop_front();
    return c;
};

void testN(int start, int N)
{
    typedef std::deque<int> C;
    typedef C::iterator I;
    typedef C::const_iterator CI;
    typedef random_access_iterator<I> RAI;
    typedef random_access_iterator<CI> RACI;
    C c1 = make(N, start);
    C c2 = make(N);
    assert(std::move_backward(c1.cbegin(), c1.cend(), c2.end()) == c2.begin());
    assert(c1 == c2);
    assert(std::move_backward(c2.cbegin(), c2.cend(), c1.end()) == c1.begin());
    assert(c1 == c2);
    assert(std::move_backward(c1.cbegin(), c1.cend(), RAI(c2.end())) == RAI(c2.begin()));
    assert(c1 == c2);
    assert(std::move_backward(c2.cbegin(), c2.cend(), RAI(c1.end())) == RAI(c1.begin()));
    assert(c1 == c2);
    assert(std::move_backward(RACI(c1.cbegin()), RACI(c1.cend()), c2.end()) == c2.begin());
    assert(c1 == c2);
    assert(std::move_backward(RACI(c2.cbegin()), RACI(c2.cend()), c1.end()) == c1.begin());
    assert(c1 == c2);
}

int main()
{
    int rng[] = {0, 1, 2, 3, 1023, 1024, 1025, 2047, 2048, 2049};
    const int N = sizeof(rng)/sizeof(rng[0]);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            testN(rng[i], rng[j]);
}
