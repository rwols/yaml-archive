#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(wchar_t_, io_fixture)
{
    wchar_t x = L'😘';
    check_roundtrip(x);
}
