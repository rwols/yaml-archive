#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(char_, io_fixture)
{
    char x = static_cast<char>(rand());
    check_roundtrip(x);
}
