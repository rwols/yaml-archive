/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_vector.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <cstddef> // NULL
#include <fstream>

#include <boost/config.hpp>
#include <cstdio> // remove
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::remove;
}
#endif
#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/serialization/yaml/vector.hpp>
#include <boost/static_assert.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(std_vector_int, io_fixture)
{
    std::vector<int> v;
    v.push_back(42);
    v.push_back(100);
    check_roundtrip_collection(v);
}

BOOST_FIXTURE_TEST_CASE(std_vector_bool, io_fixture)
{
    std::vector<bool> v;
    v.push_back(false);
    v.push_back(true);
    v.push_back(false);
    v.push_back(false);
    v.push_back(true);
    check_roundtrip_collection(v);
}

BOOST_FIXTURE_TEST_CASE(std_vector_A, io_fixture)
{
    std::vector<A> v;
    v.push_back(A());
    v.push_back(A());
    check_roundtrip_collection(v);
}

// class without default constructor
struct X
{
    // BOOST_DELETED_FUNCTION(X());
  public:
    int m_i;
    X(const X& x) : m_i(x.m_i) {}
    X(const int& i) : m_i(i) {}
    bool operator==(const X& rhs) const { return m_i == rhs.m_i; }
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar& BOOST_SERIALIZATION_NVP(m_i);
    }
};

template <class Archive>
inline void save_construct_data(Archive& ar, const X* x,
                                const unsigned int /* file_version */
                                )
{
    // variable used for construction
    ar << boost::serialization::make_nvp("i", x->m_i);
}

template <class Archive>
inline void load_construct_data(Archive& ar, X* x,
                                const unsigned int /* file_version */
                                )
{
    int i;
    ar >> boost::serialization::make_nvp("i", i);
    ::new (x) X(i);
}

std::ostream& operator<<(std::ostream& os, const X& x)
{
    return os << "<X, m_i: " << x.m_i << ">";
}

BOOST_FIXTURE_TEST_CASE(std_vector_non_default_ctor, io_fixture)
{
    std::vector<X> v;
    v.push_back(X(123));
    v.push_back(X(456));
    check_roundtrip_collection(v);
}
