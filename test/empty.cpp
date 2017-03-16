#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(empty, io_fixture)
{
    {
        output();
    }
    {
        input();
    }
}
