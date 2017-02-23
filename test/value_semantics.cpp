/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_archive_derivation.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(value_semantics, io_fixture)
{
    const A a;
    A       a1;
    {
        output() << BOOST_SERIALIZATION_NVP(a);
    }
    {
        input() >> boost::serialization::make_nvp("a", a1);
    }
    BOOST_CHECK(a == a1);
}

// EOF
