#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(long_, io_fixture)
{
    long x = static_cast<long>(rand());
    check_roundtrip(x);
}
