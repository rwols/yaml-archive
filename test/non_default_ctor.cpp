/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_non_default_ctor.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

// this tests:
// a) non-intrusive method of implementing serialization
// b) usage of a non-default constructor

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/limits.hpp>
#include <boost/math/special_functions/next.hpp>
#include <cstddef> // NULL
#include <cstdio>  // remove()
#include <cstdlib> // for rand()
#include <fstream>

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::rand;
using ::remove;
#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400) && !defined(UNDER_CE)
using ::numeric_limits;
#endif
}
#endif

#include "io_fixture.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/test/unit_test.hpp>

///////////////////////////////////////////////////////
// simple class test - using non-intrusive syntax
// illustrates the usage of the non-intrusve syntax
class non_default
{
    friend class boost::serialization::access;

    // note const values can only be initialized with a non
    // non-default constructor
    const int i;

    signed char   s;
    unsigned char t;
    signed int    u;
    unsigned int  v;
    float         w;
    double        x;

  public:
    bool operator==(const non_default& rhs) const;
    bool operator<(const non_default& rhs) const;

  private:
    friend std::ostream& operator<<(std::ostream&, const non_default&);
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(s);
        ar& BOOST_SERIALIZATION_NVP(t);
        ar& BOOST_SERIALIZATION_NVP(u);
        ar& BOOST_SERIALIZATION_NVP(v);
        ar& BOOST_SERIALIZATION_NVP(w);
        ar& BOOST_SERIALIZATION_NVP(x);
    }
    non_default(const non_default& rhs);
    non_default& operator=(const non_default& rhs);

  public:
    static int count;
    const int& get_i() const { return i; }
    non_default(int i_);
    ~non_default();
};

int non_default::count = 0;

non_default::non_default(int i_)
    : i(i_), s(static_cast<signed char>(0xff & std::rand())),
      t(static_cast<signed char>(0xff & std::rand())), u(std::rand()),
      v(std::rand()), w((float)std::rand() / std::rand()),
      x((double)std::rand() / std::rand())
{
    ++count;
}

non_default::~non_default() { --count; }

bool non_default::operator==(const non_default& rhs) const
{
    return s == rhs.s && t == rhs.t && u == rhs.u && v == rhs.v &&
           std::abs(boost::math::float_distance(w, rhs.w)) < 8 &&
           std::abs(boost::math::float_distance(x, rhs.x)) < 8;
}

bool non_default::operator<(const non_default& rhs) const
{
    if (!(s == rhs.s)) return s < rhs.s;
    if (!(t == rhs.t)) return t < rhs.t;
    if (!(u == rhs.u)) return t < rhs.u;
    if (!(v == rhs.v)) return t < rhs.v;
    if (std::abs(boost::math::float_distance(w, rhs.w)) > 1) return false;
    if (std::abs(boost::math::float_distance(x, rhs.x)) > 1) return false;
    return false;
}

std::ostream& operator<<(std::ostream& os, const non_default& nd)
{
    os << "<non_default, i: " << nd.i << ", s: " << static_cast<int>(nd.s)
       << ", t: " << static_cast<unsigned int>(nd.t) << ", u: " << nd.u
       << ", v: " << nd.v << ", w: " << nd.w << ", x: " << nd.x << ">";
    return os;
}

namespace boost {
namespace serialization {

template <class Archive>
inline void save_construct_data(Archive& ar, const non_default* a,
                                const unsigned int /* file_version */
                                )
{
    // variable used for construction
    ar << boost::serialization::make_nvp("i", a->get_i());
}

template <class Archive>
inline void load_construct_data(Archive& ar, non_default* a,
                                const unsigned int /* file_version */
                                )
{
    int i;
    ar >> boost::serialization::make_nvp("i", i);
    ::new (a) non_default(i);
}

} // serialization
} // namespace boost

BOOST_FIXTURE_TEST_CASE(non_default_ctor, io_fixture)
{
    {
        non_default  a(2), a1(2);
        non_default* pa1 = &a;
        non_default* pa2 = new non_default(4);

        {
            output() << BOOST_SERIALIZATION_NVP(a)
                     << BOOST_SERIALIZATION_NVP(pa1)
                     << BOOST_SERIALIZATION_NVP(pa2);
        }

        delete pa2;
        pa1 = nullptr;
        pa2 = nullptr;

        {
            input() >> boost::serialization::make_nvp("a", a1) >>
                BOOST_SERIALIZATION_NVP(pa1) >> BOOST_SERIALIZATION_NVP(pa2);
        }

        BOOST_CHECK_EQUAL(a, a1);
        BOOST_CHECK_EQUAL(a, *pa1);
        // ???
        // BOOST_CHECK_MESSAGE(pa1 == &a, "Copy of pointer not correctly
        // restored");
        // BOOST_CHECK_MESSAGE(pa2 != &a, "Pointer not correctly restored");
        delete pa2;
        pa1 = nullptr;
        pa2 = nullptr;
    }
    BOOST_CHECK_EQUAL(0, non_default::count);
}

// EOF
