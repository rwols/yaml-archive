/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// non_intrusive.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

// this tests the non-intrusive method of implementing serialization

#include "io_fixture.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/limits.hpp>
#include <boost/math/special_functions/next.hpp>
#include <boost/test/unit_test.hpp>

///////////////////////////////////////////////////////
// simple class test - using non-intrusive syntax
// illustrates the usage of the non-intrusve syntax
class As
{
  public:
    signed char   s;
    unsigned char t;
    signed int    u;
    unsigned int  v;
    float         w;
    double        x;
    As();
    bool operator==(const As& rhs) const;
    bool operator<(const As& rhs) const;
};

As::As()
    : s(static_cast<signed char>(0xff & std::rand())),
      t(static_cast<signed char>(0xff & std::rand())), u(std::rand()),
      v(std::rand()), w((float)std::rand() / std::rand()),
      x((double)std::rand() / std::rand())
{
}

bool As::operator==(const As& rhs) const
{
    // FIXME!!!
    return s == rhs.s && t == rhs.t && u == rhs.u && v == rhs.v &&
           std::abs(boost::math::float_distance(w, rhs.w)) < 8 &&
           std::abs(boost::math::float_distance(x, rhs.x)) < 8;
}

bool As::operator<(const As& rhs) const
{
    if (!(s == rhs.s)) return s < rhs.s;
    if (!(t == rhs.t)) return t < rhs.t;
    if (!(u == rhs.u)) return t < rhs.u;
    if (!(v == rhs.v)) return t < rhs.v;
    if (std::abs(boost::math::float_distance(w, rhs.w)) > 1) return false;
    if (std::abs(boost::math::float_distance(x, rhs.x)) > 1) return false;
    return false;
}

// note the following:

// function specializations must be defined in the appropriate
// namespace - boost::serialization
namespace boost {
namespace serialization {

// This first set of overrides should work with all compilers.

// The last argument is int while the default versions
// defined in serialization.hpp have long as the last argument.
// This is part of the work around for compilers that don't
// support correct function template ordering.  These functions
// are always called with 0 (i.e. an int) as the last argument.
// Our specialized versions also have int as the last argument
// while the default versions have a long as the last argument.
// This makes our specialized versions a better match than the
// default ones as no argument conversion is required to make a match
template <class Archive>
void serialize(Archive& ar, As& a, const unsigned int /* file_version */
               )
{
    ar& boost::serialization::make_nvp("s", a.s);
    ar& boost::serialization::make_nvp("t", a.t);
    ar& boost::serialization::make_nvp("u", a.u);
    ar& boost::serialization::make_nvp("v", a.v);
    ar& boost::serialization::make_nvp("w", a.w);
    ar& boost::serialization::make_nvp("x", a.x);
}

} // serialization
} // namespace boost

BOOST_FIXTURE_TEST_CASE(non_intrusive, io_fixture)
{
    As  a;
    As  b;
    As* pa1 = &a;
    As* pa2 = new As();
    {
        output() << BOOST_SERIALIZATION_NVP(a) << BOOST_SERIALIZATION_NVP(pa1)
                 << BOOST_SERIALIZATION_NVP(pa2);
    }
    delete pa2;
    pa1 = nullptr;
    pa2 = nullptr;
    {
        input() >> boost::serialization::make_nvp("a", b) >>
            BOOST_SERIALIZATION_NVP(pa1) >> BOOST_SERIALIZATION_NVP(pa2);
    }
    BOOST_CHECK(a == b);
    BOOST_CHECK_MESSAGE(pa1 == &a, "Copy of pointer not correctly restored");
    BOOST_CHECK_MESSAGE(pa2 != &a, "Pointer not correctly restored");
}

// EOF
