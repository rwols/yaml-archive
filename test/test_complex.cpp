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
#include <cstddef> // NULL
#include <cstdio>  // remove
#include <cstdlib> // rand

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

#include "test_tools.hpp"

#include <boost/preprocessor/stringize.hpp>
#include BOOST_PP_STRINGIZE(BOOST_ARCHIVE_TEST)

#include <boost/serialization/complex.hpp>

#include <iostream>

template <class T>
std::ostream& operator<<(std::ostream& os, const std::complex<T>& z)
{
    os << std::setprecision(std::numeric_limits<T>::digits10 + 1);
    return os << z.real() << " + " << z.imag() << 'i';
}

int test_main(int /* argc */, char* /* argv */ [])
{
    // const char * testfile = boost::archive::tmpnam(NULL);
    const char* testfile = "test_complex.yml";
    BOOST_REQUIRE(NULL != testfile);

    // test array of objects
    std::complex<float> a(
        static_cast<float>(std::rand()) / static_cast<float>(std::rand()),
        static_cast<float>(std::rand()) / static_cast<float>(std::rand()));
    std::complex<double> b(
        static_cast<double>(std::rand()) / static_cast<double>(std::rand()),
        static_cast<double>(std::rand()) / static_cast<double>(std::rand()));
    {
        std::ofstream os(testfile, TEST_STREAM_FLAGS);
        test_oarchive oa(os);
        test_oarchive testcout(std::cout, TEST_STREAM_FLAGS);
        oa << boost::serialization::make_nvp("afloatcomplex", a);
        oa << boost::serialization::make_nvp("adoublecomplex", b);
        testcout << boost::serialization::make_nvp("afloatcomplex", a);
        testcout << boost::serialization::make_nvp("adoublecomplex", b);
    }
    std::complex<float> a1;
    std::complex<double> b1;
    {
        test_istream is(testfile, TEST_STREAM_FLAGS);
        test_iarchive ia(is);
        ia >> boost::serialization::make_nvp("afloatcomplex", a1);
        ia >> boost::serialization::make_nvp("adoublecomplex", b1);
    }

    std::cerr << "a  = " << a << '\n';
    std::cerr << "a1 = " << a1 << '\n';
    std::cerr << "b  = " << b << '\n';
    std::cerr << "b1 = " << b1 << '\n';

    std::cerr << "a.real()-a1.real() distance = "
              << std::abs(boost::math::float_distance(a.real(), a1.real()))
              << std::endl;
    BOOST_CHECK(std::abs(boost::math::float_distance(a.real(), a1.real())) < 3);
    std::cerr << "a.imag() - a1.imag() distance = "
              << std::abs(boost::math::float_distance(a.imag(), a1.imag()))
              << std::endl;
    BOOST_CHECK(std::abs(boost::math::float_distance(a.imag(), a1.imag())) < 3);
    std::cerr << "b.real() - b1.real() distance = "
              << std::abs(boost::math::float_distance(b.real(), b1.real()))
              << std::endl;
    BOOST_CHECK(std::abs(boost::math::float_distance(b.real(), b1.real())) < 3);
    std::cerr << "b.imag() - b1.imag() distance = "
              << std::abs(boost::math::float_distance(b.imag(), b1.imag()))
              << std::endl;
    BOOST_CHECK(std::abs(boost::math::float_distance(b.imag(), b1.imag())) < 3);

    // std::remove(testfile);
    return EXIT_SUCCESS;
}

// EOF
