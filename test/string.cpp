#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>
#include <string>

BOOST_FIXTURE_TEST_CASE(string, io_fixture)
{
    for (std::size_t i = 0; i < get_string_sample_count(); ++i)
    {
        std::string x{get_string_sample(i)};
        check_roundtrip(x);
    }
}

BOOST_FIXTURE_TEST_CASE(wstring, io_fixture)
{
    for (std::size_t i = 0; i < get_wstring_sample_count(); ++i)
    {
        std::wstring x{get_wstring_sample(i)};
        std::wstring another_x;
        {
            output() << BOOST_SERIALIZATION_NVP(x);
        }
        {
            input() >> boost::serialization::make_nvp("x", another_x);
        }
        BOOST_CHECK(x == another_x);
    }
}
