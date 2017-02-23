/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// recursion.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "A.ipp"
#include "J.hpp"
#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(recursion, io_fixture)
{
    // test recursive structure
    J j, j1;
    j.j = &j;
    {
        output() << BOOST_SERIALIZATION_NVP(j);
    }
    {
        input() >> boost::serialization::make_nvp("j", j1);
    }
    BOOST_CHECK(j == j1);
}

// EOF
