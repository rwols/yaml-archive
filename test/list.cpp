/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_list.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/list.hpp>
#include <boost/test/unit_test.hpp>
#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
#include <boost/serialization/forward_list.hpp>
#endif

#define NVP(name) BOOST_SERIALIZATION_NVP(name)
using boost::serialization::make_nvp;

BOOST_FIXTURE_TEST_CASE(std_list, io_fixture)
{
    std::list<A> alist;
    alist.push_back(A());
    alist.push_back(A());
    std::list<A> alist1;
    {
        output() << BOOST_SERIALIZATION_NVP(alist);
    }
    {
        input() >> boost::serialization::make_nvp("alist", alist1);
    }
    BOOST_CHECK(alist == alist1);
}

BOOST_FIXTURE_TEST_CASE(std_forward_list2, io_fixture)
{
#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
    std::forward_list<A> alist;
    alist.push_front(A());
    alist.push_front(A());
    std::forward_list<A> alist1;
    {
        output() << BOOST_SERIALIZATION_NVP(alist);
    }
    {
        input() >> boost::serialization::make_nvp("alist", alist1);
    }
    BOOST_CHECK(alist == alist1);
#else
    BOOST_WARN_MESSAGE(false, "std::forward_list header not present.");
#endif
}

// EOF
