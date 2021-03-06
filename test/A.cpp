﻿/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// A.cpp    simple class test

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/detail/workaround.hpp>
#include <boost/math/special_functions/next.hpp>
#include <cassert>
#include <cstddef> // size_t
#include <cstdlib> // rand()
#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
#include <boost/archive/dinkumware.hpp>
#endif
#include <boost/locale/encoding_utf.hpp>

#include "A.hpp"

template <class T> void accumulate(std::size_t& s, const T& t)
{
    const char*  tptr = (const char*)(&t);
    unsigned int count = sizeof(t);
    while (count-- > 0)
    {
        s += *tptr++;
    }
}

A::operator std::size_t() const
{
    std::size_t retval = 0;
    accumulate(retval, b);
#ifndef BOOST_NO_INT64_T
    accumulate(retval, f);
    accumulate(retval, g);
#endif
    accumulate(retval, l);
    accumulate(retval, m);
    accumulate(retval, n);
    accumulate(retval, o);
    accumulate(retval, p);
    accumulate(retval, q);
#ifndef BOOST_NO_CWCHAR
    accumulate(retval, r);
#endif
    accumulate(retval, c);
    accumulate(retval, s);
    accumulate(retval, t);
    accumulate(retval, u);
    accumulate(retval, v);
    return retval;
}

#if defined(_MSC_VER)
#pragma warning(push)           // Save warning settings.
#pragma warning(disable : 4244) // Disable possible loss of data warning

#endif
A::A()
    : b(true),
#ifndef BOOST_NO_INT64_T
      f(std::rand() * std::rand()), g(std::rand() * std::rand()),
#endif
      l(static_cast<enum h>(std::rand() % 3)), m(std::rand()), n(std::rand()),
      o(std::rand()), p(std::rand()), q(std::rand()),
#ifndef BOOST_NO_CWCHAR
      r(L'€'),
#endif
      c('x'), s(0xff & std::rand()), t(0xff & std::rand()), u(std::rand()),
      v(std::rand()), w((float)std::rand()), x((double)std::rand())
{
}

#if defined(_MSC_VER)
#pragma warning(pop) // Restore warnings to previous state.
#endif

bool A::operator==(const A& rhs) const
{
    return b == rhs.b && l == rhs.l
#ifndef BOOST_NO_INT64_T
           && f == rhs.f && g == rhs.g
#endif
           && m == rhs.m && n == rhs.n && o == rhs.o && p == rhs.p && q == rhs.q
#ifndef BOOST_NO_CWCHAR
           && r == rhs.r
#endif
           && c == rhs.c && s == rhs.s && t == rhs.t && u == rhs.u &&
           v == rhs.v &&
           std::abs(boost::math::float_distance(x, rhs.x)) <= 8 // FIXME!!!
           && y == rhs.y
#ifndef BOOST_NO_STD_WSTRING
           && z == rhs.z
#endif
        ;
}

bool A::operator!=(const A& rhs) const { return !(*this == rhs); }

bool A::operator<(const A& rhs) const
{
    if (b != rhs.b) return b < rhs.b;
#ifndef BOOST_NO_INT64_T
    if (f != rhs.f) return f < rhs.f;
    if (g != rhs.g) return g < rhs.g;
#endif
    if (l != rhs.l) return l < rhs.l;
    if (m != rhs.m) return m < rhs.m;
    if (n != rhs.n) return n < rhs.n;
    if (o != rhs.o) return o < rhs.o;
    if (p != rhs.p) return p < rhs.p;
    if (q != rhs.q) return q < rhs.q;
#ifndef BOOST_NO_CWCHAR
    if (r != rhs.r) return r < rhs.r;
#endif
    if (c != rhs.c) return c < rhs.c;
    if (s != rhs.s) return s < rhs.s;
    if (t != rhs.t) return t < rhs.t;
    if (u != rhs.u) return u < rhs.u;
    if (v != rhs.v) return v < rhs.v;
    if (w != rhs.w) return w < rhs.w;
    if (x != rhs.x) return x < rhs.x;
    int i = y.compare(rhs.y);
    if (i != 0) return i < 0;
#ifndef BOOST_NO_STD_WSTRING
    int j = z.compare(rhs.z);
    if (j != 0) return j < 0;
#endif
    return false;
}

std::ostream& operator<<(std::ostream& os, const A& a)
{
    os << "<A";
    os << ' ' << a.b;
#ifndef BOOST_NO_INT64_T
    os << ' ' << a.f;
    os << ' ' << a.g;
#endif
    os << ' ' << a.l;
    os << ' ' << a.m;
    os << ' ' << a.n;
    os << ' ' << a.o;
    os << ' ' << a.p;
    os << ' ' << a.q;
#ifndef BOOST_NO_CWCHAR
    os << ' ' << a.r;
#endif
    os << ' ' << a.c;
    os << ' ' << a.s;
    os << ' ' << a.t;
    os << ' ' << a.u;
    os << ' ' << a.v;
    os << ' ' << a.w;
    os << ' ' << a.x;
    os << ' ' << a.y;
#ifndef BOOST_NO_STD_WSTRING
    using boost::locale::conv::utf_to_utf;
    os << ' ' << utf_to_utf<std::string::value_type>(a.z);
#endif
    return os << '>';
}
