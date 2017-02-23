/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_simple_class.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

// invoke header for a custom archive test.

#include <boost/math/special_functions/next.hpp>
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstddef> // NULL

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"

bool A::check_equal(const A& rhs) const
{
    BOOST_CHECK_EQUAL(b, rhs.b);
    BOOST_CHECK_EQUAL(l, rhs.l);
#ifndef BOOST_NO_INT64_T
    BOOST_CHECK_EQUAL(f, rhs.f);
    BOOST_CHECK_EQUAL(g, rhs.g);
#endif
    BOOST_CHECK_EQUAL(m, rhs.m);
    BOOST_CHECK_EQUAL(n, rhs.n);
    BOOST_CHECK_EQUAL(o, rhs.o);
    BOOST_CHECK_EQUAL(p, rhs.p);
    BOOST_CHECK_EQUAL(q, rhs.q);
#ifndef BOOST_NO_CWCHAR
    BOOST_CHECK_EQUAL(r, rhs.r);
#endif
    BOOST_CHECK_EQUAL(c, rhs.c);
    BOOST_CHECK_EQUAL(s, rhs.s);
    BOOST_CHECK_EQUAL(t, rhs.t);
    BOOST_CHECK_EQUAL(u, rhs.u);
    BOOST_CHECK_EQUAL(v, rhs.v);
    BOOST_CHECK_EQUAL(l, rhs.l);
    BOOST_CHECK(std::abs(boost::math::float_distance(w, rhs.w)) < 4);
    BOOST_CHECK(std::abs(boost::math::float_distance(x, rhs.x)) < 4);
    BOOST_CHECK(!(0 != y.compare(rhs.y)));
#ifndef BOOST_NO_STD_WSTRING
    BOOST_CHECK(!(0 != z.compare(rhs.z)));
#endif
    return true;
}

BOOST_FIXTURE_TEST_CASE(simple_class, io_fixture)
{
    A a, a1;
    {
        output() << boost::serialization::make_nvp("a", a);
    }
    {
        input() >> boost::serialization::make_nvp("a", a1);
    }
    a.check_equal(a1);
}
