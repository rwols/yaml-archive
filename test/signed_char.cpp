#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(signed_char, io_fixture)
{
    signed char x;
    check_roundtrip(x);
}
