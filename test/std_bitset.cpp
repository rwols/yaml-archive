//  (C) Copyright 2009 Brian Ravnsgaard and Kenneth Riddile
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for most recent version including documentation.

// Test that serialization of std::bitset works.
// Should pass compilation and execution
// 16.09.2004, updated 04.03.2009

#include <cstddef> // NULL
#include <cstdio>  // remove
#include <fstream>

#include <boost/config.hpp>

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::remove;
}
#endif

#include "io_fixture.hpp"
#include <boost/serialization/bitset.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

template <std::size_t N>
std::ostream& operator<<(std::ostream& os, const std::bitset<N>& bits)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        os << (bits[i] ? '1' : '0');
    }
    return os;
}

BOOST_FIXTURE_TEST_CASE(std_bitset, io_fixture)
{
    std::bitset<8> bitsetA;
    bitsetA.set(0, false);
    bitsetA.set(1, true);
    bitsetA.set(2, false);
    bitsetA.set(3, true);
    bitsetA.set(4, false);
    bitsetA.set(5, false);
    bitsetA.set(6, true);
    bitsetA.set(7, true);
    {
        output() << boost::serialization::make_nvp("bitset", bitsetA);
    }
    std::bitset<8> bitsetB;
    {
        input() >> boost::serialization::make_nvp("bitset", bitsetB);
    }
    BOOST_CHECK_EQUAL(bitsetA, bitsetB);
}

// EOF
