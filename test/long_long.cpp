#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(long_long, io_fixture)
{
    long long x = static_cast<long long>(rand());
    check_roundtrip(x);
}
