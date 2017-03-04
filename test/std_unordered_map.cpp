/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_map.cpp

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
#include <boost/serialization/unordered_map.hpp>
#include <boost/test/unit_test.hpp>
#include <functional> // requires changeset [69520]; Ticket #5254

///////////////////////////////////////////////////////
// a key value initialized with a random value for use
// in testing STL map serialization
struct random_key
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */
                   )
    {
        ar& boost::serialization::make_nvp("random_key", m_i);
    }
    int m_i;
    random_key() : m_i(std::rand()){};
    bool operator<(const random_key& rhs) const { return m_i < rhs.m_i; }
    bool operator==(const random_key& rhs) const { return m_i == rhs.m_i; }
    operator std::size_t() const
    { // required by hash_map
        return m_i;
    }
};

namespace std {
template <> struct hash<random_key>
{
    std::size_t operator()(const random_key& r) const
    {
        return static_cast<std::size_t>(r);
    }
};
} // namespace std

BOOST_FIXTURE_TEST_CASE(std_unordered_map, io_fixture)
{
#ifndef BOOST_NO_CXX11_UNORDERED_MAP
    std::unordered_map<random_key, A> anunordered_map;
    anunordered_map.insert(std::make_pair(random_key(), A()));
    anunordered_map.insert(std::make_pair(random_key(), A()));
    {
        output() << BOOST_SERIALIZATION_NVP(anunordered_map);
    }
    std::unordered_map<random_key, A> anunordered_map1;
    {
        input() >>
            boost::serialization::make_nvp("anunordered_map", anunordered_map1);
    }

    std::vector<std::pair<random_key, A>> tvec, tvec1;
    std::copy(anunordered_map.begin(), anunordered_map.end(),
              std::back_inserter(tvec));
    std::sort(tvec.begin(), tvec.end());
    std::copy(anunordered_map1.begin(), anunordered_map1.end(),
              std::back_inserter(tvec1));
    std::sort(tvec1.begin(), tvec1.end());
    BOOST_CHECK(tvec == tvec1);
#else
    BOOST_TEST_MESSAGE("std::unordered_map is not supported!");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_unordered_multimap, io_fixture)
{
#ifndef BOOST_NO_CXX11_UNORDERED_MAP
    std::unordered_multimap<random_key, A> anunordered_multimap;
    anunordered_multimap.insert(std::make_pair(random_key(), A()));
    anunordered_multimap.insert(std::make_pair(random_key(), A()));
    {
        output() << BOOST_SERIALIZATION_NVP(anunordered_multimap);
    }
    std::unordered_multimap<random_key, A> anunordered_multimap1;
    {
        input() >> boost::serialization::make_nvp("anunordered_multimap",
                                                  anunordered_multimap1);
    }
    std::vector<std::pair<random_key, A>> tvec, tvec1;
    std::copy(anunordered_multimap.begin(), anunordered_multimap.end(),
              std::back_inserter(tvec));
    std::sort(tvec.begin(), tvec.end());
    std::copy(anunordered_multimap1.begin(), anunordered_multimap1.end(),
              std::back_inserter(tvec1));
    std::sort(tvec1.begin(), tvec1.end());
    BOOST_CHECK(tvec == tvec1);
#else
    BOOST_TEST_MESSAGE("std::unordered_multimap is not supported!");
#endif
}
