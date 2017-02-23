#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(unsigned_char, io_fixture)
{
    unsigned char x = static_cast<unsigned char>(rand());
    check_roundtrip(x);
}
