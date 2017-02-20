/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_array.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <stdlib.h>

#include <algorithm> // equal
#include <boost/config.hpp>
#include <cstddef>
#include <cstdio> // remove
#include <fstream>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::remove;
}
#endif
#include "test_tools.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/array.hpp>
#include <boost/core/no_exceptions_support.hpp>

#include "A.hpp"
#include "A.ipp"

template <class T> int test_native_array()
{
    const char* testfile = "test_native_array.yml";

    // test array of objects
    const T a_array[10] = {T(), T(), T(), T(), T(), T(), T(), T(), T(), T()};
    const T b_array[2][3] = {{T(), T(), T()}, {T(), T(), T()}};
    {
        test_ostream os(testfile, TEST_STREAM_FLAGS);
        {
            test_oarchive oa(os, TEST_ARCHIVE_FLAGS);
            oa << boost::serialization::make_nvp("a_array", a_array);
            oa << boost::serialization::make_nvp("b_array", b_array);
        }
        os.close();
    }
    {
        test_oarchive oa(std::cout, TEST_ARCHIVE_FLAGS);
        oa << boost::serialization::make_nvp("a_array", a_array);
        oa << boost::serialization::make_nvp("b_array", b_array);
    }
    {
        T            a_array1[10];
        T            b_array1[2][3];
        test_istream is(testfile, TEST_STREAM_FLAGS);
        {
            test_iarchive ia(is, TEST_ARCHIVE_FLAGS);
            ia >> boost::serialization::make_nvp("a_array", a_array1);
            ia >> boost::serialization::make_nvp("b_array", b_array1);
        }
        is.close();
        for (int i = 0; i < 10; ++i)
        {
            std::cout << "a_array[" << i << "]  = " << a_array[i] << '\n';
            std::cout << "a_array1[" << i << "] = " << a_array1[i] << '\n';
            BOOST_CHECK_EQUAL(a_array[i], a_array1[i]);
        }
        // BOOST_CHECK(std::equal(&a_array[0], &a_array[10], &a_array1[0]));
        BOOST_CHECK(b_array[0][0] == b_array1[0][0]);
        BOOST_CHECK(b_array[1][0] == b_array1[1][0]);
    }
    {
        T            a_array1[9];
        T            b_array1[2][3];
        test_istream is(testfile, TEST_STREAM_FLAGS);
        {
            test_iarchive ia(is, TEST_ARCHIVE_FLAGS);
            bool          exception_invoked = false;
            BOOST_TRY
            {
                ia >> boost::serialization::make_nvp("a_array", a_array1);
                ia >> boost::serialization::make_nvp("b_array", b_array1);
            }
            BOOST_CATCH(boost::archive::archive_exception ae)
            {
                BOOST_CHECK(
                    boost::archive::archive_exception::array_size_too_short ==
                    ae.code);
                exception_invoked = true;
            }
            BOOST_CATCH_END
            BOOST_CHECK(exception_invoked);
        }
        is.close();
    }
    return EXIT_SUCCESS;
}

int test_main(int /* argc */, char* /* argv */ [])
{
    int res;

    // native array
    res = test_native_array<A>();
    if (res != EXIT_SUCCESS) return EXIT_FAILURE;
    // test an int array for which optimized versions should be available
    res = test_native_array<int>();
    if (res != EXIT_SUCCESS) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

// EOF
