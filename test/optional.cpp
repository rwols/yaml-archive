/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_optional.cpp

// (C) Copyright 2004 Pavel Vozenilek
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(boost_optional, io_fixture)
{
    const boost::optional<int> aoptional1;
    const boost::optional<int> aoptional2(123);
    check_roundtrip(aoptional1);
    check_roundtrip(aoptional2);
}

// std::optional coming soon?
