/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_unique_ptr.cpp

// (C) Copyright 2002-14 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "A.hpp"
#include "io_fixture.hpp" // includes boost/version.hpp
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/config.hpp>

#if BOOST_VERSION >= 105600
#include <boost/serialization/unique_ptr.hpp>
#endif

BOOST_FIXTURE_TEST_CASE(std_unique_ptr_1, io_fixture)
{
#if BOOST_VERSION >= 105600
    // create  a new auto pointer to ta new object of type A
    std::unique_ptr<A> spa(new A);
    {
        output() << BOOST_SERIALIZATION_NVP(spa);
    }
    // Reset the unique_ptr to null, thereby destroying the object of type A.
    // Note that the reset automagically maintains the reference count.
    spa.reset();
    {
        input() >> BOOST_SERIALIZATION_NVP(spa);
    }
#else
    BOOST_TEST_MESSAGE(
        "no support for std::unique_ptr on this version of boost");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_unique_ptr_2, io_fixture)
{
#if BOOST_VERSION >= 105600
    // create  a new auto pointer to ta new object of type A
    std::unique_ptr<A> spa(new A);
    std::unique_ptr<A> spa1;
    {
        output() << BOOST_SERIALIZATION_NVP(spa);
    }
    {
        input() >> boost::serialization::make_nvp("spa", spa1);
    }
    BOOST_CHECK(*spa == *spa1);
#else
    BOOST_TEST_MESSAGE(
        "no support for std::unique_ptr on this version of boost");
#endif
}
