/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_null_ptr.cpp: test implementation level trait

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "io_fixture.hpp"
#include "polymorphic_derived1.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(null_ptr_restore, io_fixture)
{
    polymorphic_base*     rb1 = nullptr;
    polymorphic_derived1* rd1 = nullptr;
    polymorphic_derived1  dummy;
    {
        output() << BOOST_SERIALIZATION_NVP(rb1)
                 << BOOST_SERIALIZATION_NVP(rd1);
    }
    rb1 = &dummy;
    rd1 = &dummy;
    {
        input() >> BOOST_SERIALIZATION_NVP(rb1) >> BOOST_SERIALIZATION_NVP(rd1);
    }
    BOOST_CHECK_MESSAGE(nullptr == rb1, "Null pointer not restored");
    BOOST_CHECK_MESSAGE(nullptr == rd1, "Null pointer not restored");
    delete rb1;
    delete rd1;
}

// EOF
