#include "A.hpp"
#include "io_fixture.hpp"
#include <array>
#include <boost/array.hpp>
#include <boost/test/unit_test.hpp>

#define NVP(name) BOOST_SERIALIZATION_NVP(name)
using boost::serialization::make_nvp;

BOOST_FIXTURE_TEST_CASE(native_array_1d_A, io_fixture)
{
    const A array[10] = {A(), A(), A(), A(), A(), A(), A(), A(), A(), A()};
    A       array1[10];
    {
        output() << NVP(array);
    }
    {
        input() >> make_nvp("array", array1);
    }
    for (int i = 0; i < 10; ++i)
    {
        BOOST_CHECK_EQUAL(array[i], array1[i]);
    }
}

BOOST_FIXTURE_TEST_CASE(native_array_1d_A_wrong_size, io_fixture)
{
    const A array[2] = {A(), A()};
    A       array1[1];
    {
        output() << NVP(array);
    }
    {
        BOOST_CHECK_THROW((input() >> make_nvp("array", array1)),
                          boost::archive::archive_exception);
    }
}

BOOST_FIXTURE_TEST_CASE(native_array_2d_A, io_fixture)
{
    const A array[2][3] = {{A(), A(), A()}, {A(), A(), A()}};
    A       array1[2][3];
    {
        output() << NVP(array);
    }
    {
        input() >> make_nvp("array", array1);
    }
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            BOOST_CHECK_EQUAL(array[i][j], array1[i][j]);
        }
    }
}

BOOST_FIXTURE_TEST_CASE(native_array_2d_A_wrong_size, io_fixture)
{
    const A array[2][3] = {{A(), A(), A()}, {A(), A(), A()}};
    A       array1[2][2];
    A       array2[1][3];
    {
        output() << NVP(array);
    }
    {
        BOOST_CHECK_THROW((input() >> make_nvp("array", array1)),
                          boost::archive::archive_exception);
        BOOST_CHECK_THROW((input() >> make_nvp("array", array2)),
                          boost::archive::archive_exception);
    }
}

BOOST_FIXTURE_TEST_CASE(native_array_1d_int, io_fixture)
{
    const int array[10] = {int(), int(), int(), int(), int(),
                           int(), int(), int(), int(), int()};
    int array1[10];
    {
        output() << NVP(array);
    }
    {
        input() >> make_nvp("array", array1);
    }
    for (int i = 0; i < 10; ++i)
    {
        BOOST_CHECK_EQUAL(array[i], array1[i]);
    }
}

BOOST_FIXTURE_TEST_CASE(native_array_2d_int, io_fixture)
{
    const int array[2][3] = {{int(), int(), int()}, {int(), int(), int()}};
    int       array1[2][3];
    {
        output() << NVP(array);
    }
    {
        input() >> make_nvp("array", array1);
    }
    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            BOOST_CHECK_EQUAL(array[i][j], array1[i][j]);
        }
    }
}

BOOST_FIXTURE_TEST_CASE(boost_array_A, io_fixture)
{
    const boost::array<A, 10> array = {
        {A(), A(), A(), A(), A(), A(), A(), A(), A(), A()}};
    boost::array<A, 10> array1;
    {
        output() << NVP(array);
    }
    {
        input() >> make_nvp("array", array1);
    }
    for (int i = 0; i < 10; ++i)
    {
        BOOST_CHECK_EQUAL(array[i], array1[i]);
    }
}

BOOST_FIXTURE_TEST_CASE(boost_array_A_wrong_size, io_fixture)
{
    const boost::array<A, 2> array = {{A(), A()}};
    boost::array<A, 1>       array1;
    {
        output() << NVP(array);
    }
    {
        BOOST_CHECK_THROW((input() >> make_nvp("array", array1)),
                          boost::archive::archive_exception);
    }
}

BOOST_FIXTURE_TEST_CASE(std_array_A, io_fixture)
{
#if BOOST_VERSION >= 105600
    const std::array<A, 10> array = {
        {A(), A(), A(), A(), A(), A(), A(), A(), A(), A()}};
    std::array<A, 10> array1;
    {
        output() << NVP(array);
    }
    {
        input() >> make_nvp("array", array1);
    }
    for (int i = 0; i < 10; ++i)
    {
        BOOST_CHECK_EQUAL(array[i], array1[i]);
    }
#endif
}

BOOST_FIXTURE_TEST_CASE(std_array_A_wrong_size, io_fixture)
{
#if BOOST_VERSION >= 105600
    const std::array<A, 2> array = {{A(), A()}};
    std::array<A, 1>       array1;
    {
        output() << NVP(array);
    }
    {
        BOOST_CHECK_THROW((input() >> make_nvp("array", array1)),
                          boost::archive::archive_exception);
    }
#endif
}
