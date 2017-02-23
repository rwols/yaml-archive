#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(short_, io_fixture)
{
    short x = static_cast<short>(rand());
    check_roundtrip(x);
}
