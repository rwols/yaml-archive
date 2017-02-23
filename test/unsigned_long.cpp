#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(unsigned_long, io_fixture)
{
    unsigned long x = static_cast<unsigned long>(rand());
    check_roundtrip(x);
}
