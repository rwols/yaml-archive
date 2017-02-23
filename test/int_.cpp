#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(int_, io_fixture)
{
    int x = static_cast<int>(rand());
    check_roundtrip(x);
}
