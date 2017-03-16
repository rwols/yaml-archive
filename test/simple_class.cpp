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

BOOST_FIXTURE_TEST_CASE(simple_class, io_fixture)
{
    for (std::size_t i = 0; i < get_string_sample_count(); ++i)
    {
        for (std::size_t j = 0; j < get_wstring_sample_count(); ++j)
        {
            A a, another_a;
            a.y = get_string_sample(i);
#ifndef BOOST_NO_STD_WSTRING
            a.z = get_wstring_sample(j);
#endif
            const auto filename = get_filename() + "_" + std::to_string(i) +
                                  "_" + std::to_string(j) +
                                  ARCHIVE_FILENAME_EXTENSION;
            {
                test_ostream  os(filename.c_str(), STREAM_FLAGS);
                test_oarchive oa(os, ARCHIVE_FLAGS);
                oa << BOOST_SERIALIZATION_NVP(a);
            }
            {
                test_istream  is(filename.c_str(), STREAM_FLAGS);
                test_iarchive ia(is, ARCHIVE_FLAGS);
                ia >> boost::serialization::make_nvp("a", another_a);
            }
            BOOST_CHECK_EQUAL(a, another_a);
        }
    }
}
