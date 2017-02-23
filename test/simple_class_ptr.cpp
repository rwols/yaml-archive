/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_simple_class_ptr.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <boost/math/special_functions/next.hpp>
#include <boost/test/unit_test.hpp>
#include <cmath>
#include <cstddef> // NULL

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"

BOOST_FIXTURE_TEST_CASE(simple_class_ptr, io_fixture)
{
    const A* ta = new A();
    A*       ta1 = nullptr;

    {
        output() << BOOST_SERIALIZATION_NVP(ta);
    }
    {
        input() >> boost::serialization::make_nvp("ta", ta1);
    }
    BOOST_CHECK(ta != ta1);
    BOOST_CHECK(*ta == *ta1);
    delete ta;
    delete ta1;
}

// EOF
