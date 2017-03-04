/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_set.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// (C) Copyright 2014 Jim Bell
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::size_t;
}
#endif

#include <boost/detail/workaround.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::remove;
}
#endif

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <algorithm> // std::copy
#include <boost/archive/archive_exception.hpp>
#if BOOST_VERSION > 105900
#include <boost/serialization/boost_unordered_set.hpp>
#endif
#include <boost/test/unit_test.hpp>
#include <functional> // requires changeset [69520]; Ticket #5254
#include <vector>

namespace boost {
template <> struct hash<A>
{
    std::size_t operator()(const A& a) const
    {
        return static_cast<std::size_t>(a);
    }
};
} // namespace boost

BOOST_FIXTURE_TEST_CASE(boost_unordered_set, io_fixture)
{
#if BOOST_VERSION > 105900
    boost::unordered_set<A> anunordered_set, anunordered_set1;
    anunordered_set.insert(A());
    anunordered_set.insert(A());
    {
        output() << boost::serialization::make_nvp("anunordered_set",
                                                   anunordered_set);
    }
    {
        input() >>
            boost::serialization::make_nvp("anunordered_set", anunordered_set1);
    }
    std::vector<A> tvec, tvec1;
    tvec.clear();
    tvec1.clear();
    std::copy(anunordered_set.begin(), anunordered_set.end(),
              std::back_inserter(tvec));
    std::sort(tvec.begin(), tvec.end());
    std::copy(anunordered_set1.begin(), anunordered_set1.end(),
              std::back_inserter(tvec1));
    std::sort(tvec1.begin(), tvec1.end());
    BOOST_CHECK(tvec == tvec1);
#else
    BOOST_TEST_MESSAGE("serialization of boost::unordered_set requires at "
                       "least boost version 1.60!");
#endif
}

BOOST_FIXTURE_TEST_CASE(boost_unordered_multiset, io_fixture)
{
#if BOOST_VERSION > 105900
    boost::unordered_multiset<A> anunordered_multiset, anunordered_multiset1;
    anunordered_multiset.insert(A());
    anunordered_multiset.insert(A());
    {
        output() << boost::serialization::make_nvp("anunordered_multiset",
                                                   anunordered_multiset);
    }
    {
        input() >> boost::serialization::make_nvp("anunordered_multiset",
                                                  anunordered_multiset1);
    }

    std::vector<A> tvec, tvec1;
    tvec.clear();
    tvec1.clear();
    std::copy(anunordered_multiset.begin(), anunordered_multiset.end(),
              std::back_inserter(tvec));
    std::sort(tvec.begin(), tvec.end());
    std::copy(anunordered_multiset1.begin(), anunordered_multiset1.end(),
              std::back_inserter(tvec1));
    std::sort(tvec1.begin(), tvec1.end());
    BOOST_CHECK(tvec == tvec1);
#else
    BOOST_TEST_MESSAGE("serialization of boost::unordered_multiset requires at "
                       "least boost version 1.60!");
#endif
}
