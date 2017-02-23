#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(unsigned_short, io_fixture)
{
    unsigned short x = static_cast<unsigned short>(rand());
    check_roundtrip(x);
}
