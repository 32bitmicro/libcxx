//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <deque>

// template <class InputIterator>
//   iterator insert (const_iterator p, InputIterator f, InputIterator l);

#include <deque>
#include <cassert>

#include "test_iterators.h"
#include "../../../MoveOnly.h"
#include "../../../stack_allocator.h"

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

void
test(int P, std::deque<int>& c1, const std::deque<int>& c2)
{
    typedef std::deque<int> C;
    typedef C::iterator I;
    typedef C::const_iterator CI;
    typedef bidirectional_iterator<CI> BCI;
    std::size_t c1_osize = c1.size();
    CI i = c1.insert(c1.begin() + P, BCI(c2.begin()), BCI(c2.end()));
    assert(i == c1.begin() + P);
    assert(c1.size() == c1_osize + c2.size());
    assert(distance(c1.begin(), c1.end()) == c1.size());
    i = c1.begin();
    for (int j = 0; j < P; ++j, ++i)
        assert(*i == j);
    for (int j = 0; j < c2.size(); ++j, ++i)
        assert(*i == j);
    for (int j = P; j < c1_osize; ++j, ++i)
        assert(*i == j);
}

void
testN(int start, int N, int M)
{
    typedef std::deque<int> C;
    typedef C::iterator I;
    typedef C::const_iterator CI;
    for (int i = 0; i <= 3; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            test(i, c1, c2);
        }
    }
    for (int i = M-1; i <= M+1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            test(i, c1, c2);
        }
    }
    for (int i = N/2-1; i <= N/2+1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            test(i, c1, c2);
        }
    }
    for (int i = N - M - 1; i <= N - M + 1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            test(i, c1, c2);
        }
    }
    for (int i = N - M - 1; i <= N - M + 1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            test(i, c1, c2);
        }
    }
    for (int i = N - 3; i <= N; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            test(i, c1, c2);
        }
    }
}

void
testI(int P, std::deque<int>& c1, const std::deque<int>& c2)
{
    typedef std::deque<int> C;
    typedef C::iterator I;
    typedef C::const_iterator CI;
    typedef input_iterator<CI> ICI;
    std::size_t c1_osize = c1.size();
    CI i = c1.insert(c1.begin() + P, ICI(c2.begin()), ICI(c2.end()));
    assert(i == c1.begin() + P);
    assert(c1.size() == c1_osize + c2.size());
    assert(distance(c1.begin(), c1.end()) == c1.size());
    i = c1.begin();
    for (int j = 0; j < P; ++j, ++i)
        assert(*i == j);
    for (int j = 0; j < c2.size(); ++j, ++i)
        assert(*i == j);
    for (int j = P; j < c1_osize; ++j, ++i)
        assert(*i == j);
}

void
testNI(int start, int N, int M)
{
    typedef std::deque<int> C;
    typedef C::iterator I;
    typedef C::const_iterator CI;
    for (int i = 0; i <= 3; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            testI(i, c1, c2);
        }
    }
    for (int i = M-1; i <= M+1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            testI(i, c1, c2);
        }
    }
    for (int i = N/2-1; i <= N/2+1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            testI(i, c1, c2);
        }
    }
    for (int i = N - M - 1; i <= N - M + 1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            testI(i, c1, c2);
        }
    }
    for (int i = N - 3; i <= N; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make(N, start);
            C c2 = make(M);
            testI(i, c1, c2);
        }
    }
}

void
test_move()
{
#ifndef _LIBCPP_HAS_NO_RVALUE_REFERENCES
    std::deque<MoveOnly, stack_allocator<MoveOnly, 2000> > c;
    typedef std::deque<MoveOnly>::const_iterator CI;
    {
        MoveOnly mo(0);
        typedef MoveOnly* I;
        c.insert(c.end(), std::move_iterator<I>(&mo), std::move_iterator<I>(&mo+1));
    }
    int j = 0;
    for (CI i = c.begin(); i != c.end(); ++i, ++j)
        assert(*i == MoveOnly(j));
    {
        MoveOnly mo(1);
        typedef input_iterator<MoveOnly*> I;
        c.insert(c.end(), std::move_iterator<I>(I(&mo)), std::move_iterator<I>(I(&mo+1)));
    }
    j = 0;
    for (CI i = c.begin(); i != c.end(); ++i, ++j)
        assert(*i == MoveOnly(j));
#endif  // _LIBCPP_HAS_NO_RVALUE_REFERENCES
}

int main()
{
    int rng[] = {0, 1, 2, 3, 1023, 1024, 1025, 2047, 2048, 2049};
    const int N = sizeof(rng)/sizeof(rng[0]);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                testN(rng[i], rng[j], rng[k]);
    testNI(1500, 2000, 1000);
    test_move();
}
