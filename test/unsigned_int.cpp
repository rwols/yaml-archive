#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(unsigned_int, io_fixture)
{
    unsigned int x = static_cast<unsigned int>(rand());
    check_roundtrip(x);
}
