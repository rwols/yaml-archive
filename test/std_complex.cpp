/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_complex.cpp

// (C) Copyright 2005 Matthias Troyer .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <fstream>

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/math/special_functions/next.hpp>

#if defined(BOOST_NO_STDC_NAMESPACE)
#include <boost/limits.hpp>
namespace std {
using ::rand;
using ::remove;
#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400) && !defined(UNDER_CE)
using ::numeric_limits;
#endif
}
#endif

#include "io_fixture.hpp"
#include <boost/preprocessor/stringize.hpp>
#include <boost/serialization/complex.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

template <class T>
std::ostream& operator<<(std::ostream& os, const std::complex<T>& z)
{
    os << std::setprecision(std::numeric_limits<T>::digits10 + 1);
    return os << z.real() << " + " << z.imag() << 'i';
}

BOOST_FIXTURE_TEST_CASE(std_complex, io_fixture)
{
    using boost::serialization::make_nvp;
    std::complex<float> a(
        static_cast<float>(std::rand()) / static_cast<float>(std::rand()),
        static_cast<float>(std::rand()) / static_cast<float>(std::rand()));
    std::complex<double> b(
        static_cast<double>(std::rand()) / static_cast<double>(std::rand()),
        static_cast<double>(std::rand()) / static_cast<double>(std::rand()));
    {
        output() << make_nvp("afloatcomplex", a)
                 << make_nvp("adoublecomplex", b);
    }
    std::complex<float>  a1;
    std::complex<double> b1;
    {
        input() >> make_nvp("afloatcomplex", a1) >>
            make_nvp("adoublecomplex", b1);
    }

    // FIXME!!! FLOAT PRECISION!!!
    using boost::math::float_distance;
    BOOST_CHECK_LT(std::abs(float_distance(a.real(), a1.real())), 8);
    BOOST_CHECK_LT(std::abs(float_distance(a.imag(), a1.imag())), 8);
    BOOST_CHECK_LT(std::abs(float_distance(b.real(), b1.real())), 8);
    BOOST_CHECK_LT(std::abs(float_distance(b.imag(), b1.imag())), 8);
}

// EOF
