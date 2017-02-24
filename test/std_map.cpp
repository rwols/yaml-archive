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
#include <boost/serialization/map.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

///////////////////////////////////////////////////////
// a key value initialized with a random value for use
// in testing STL map serialization
struct random_key
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
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

BOOST_FIXTURE_TEST_CASE(std_map, io_fixture)
{
    std::map<random_key, A> amap;

    amap.insert(std::make_pair(random_key(), A()));
    amap.insert(std::make_pair(random_key(), A()));
    {
        output() << BOOST_SERIALIZATION_NVP(amap);
    }
    std::map<random_key, A> amap1;
    {
        input() >> boost::serialization::make_nvp("amap", amap1);
    }
    BOOST_CHECK(amap == amap1);
}

BOOST_FIXTURE_TEST_CASE(std_map_int_ptr, io_fixture)
{
    using T = std::map<int, int>;
    std::pair<int, int> a(11, 22);
    T b;

    b[0] = 0;
    b[-1] = -1;
    b[1] = 1;
    std::pair<int, int>* pa = &a;
    std::map<int, int>*  pb = &b;
    {
        output() << BOOST_SERIALIZATION_NVP(pb) << BOOST_SERIALIZATION_NVP(pa);
    }
    pa = nullptr;
    pb = nullptr;
    {
        input() >> BOOST_SERIALIZATION_NVP(pb) >> BOOST_SERIALIZATION_NVP(pa);
    }
    BOOST_CHECK(*pa == a);
    BOOST_CHECK(*pb == b);
    delete pa;
    delete pb;
}

BOOST_FIXTURE_TEST_CASE(std_multimap, io_fixture)
{
    std::multimap<random_key, A> amultimap;
    amultimap.insert(std::make_pair(random_key(), A()));
    amultimap.insert(std::make_pair(random_key(), A()));
    {
        output() << BOOST_SERIALIZATION_NVP(amultimap);
    }
    std::multimap<random_key, A> amultimap1;
    {
        input() >> boost::serialization::make_nvp("amultimap", amultimap1);
    }
    BOOST_CHECK(amultimap == amultimap1);
}
