#include "A.ipp"
#include "B.hpp"
#include "io_fixture.hpp"
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(nvp, io_fixture)
{
    B b, b1;
    {
        output() << BOOST_SERIALIZATION_NVP(b);
    }
    {
        input() >> boost::serialization::make_nvp("b", b1);
    }
    BOOST_CHECK(b == b1);
}

BOOST_FIXTURE_TEST_CASE(nvp_pointer, io_fixture)
{
    B  b;
    B* bptr = &b;
    B* bptr1;
    {
        output() << BOOST_SERIALIZATION_NVP(bptr);
    }
    {
        input() >> boost::serialization::make_nvp("bptr", bptr1);
    }
    BOOST_CHECK(b == *bptr1);
}
