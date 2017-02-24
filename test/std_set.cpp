/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// std_set.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// (C) Copyright 2014 Jim Bell
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <algorithm> // std::copy
#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/set.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(std_set, io_fixture)
{
    std::set<A> aset;
    aset.insert(A());
    aset.insert(A());
    {
        output() << boost::serialization::make_nvp("aset", aset);
    }
    std::set<A> aset1;
    {
        input() >> boost::serialization::make_nvp("aset", aset1);
    }
    BOOST_CHECK(aset == aset1);
}

BOOST_FIXTURE_TEST_CASE(std_multiset, io_fixture)
{
    std::multiset<A> amultiset;
    amultiset.insert(A());
    amultiset.insert(A());
    {
        output() << boost::serialization::make_nvp("amultiset", amultiset);
    }
    std::multiset<A> amultiset1;
    {
        input() >> boost::serialization::make_nvp("amultiset", amultiset1);
    }
    BOOST_CHECK(amultiset == amultiset1);
}
