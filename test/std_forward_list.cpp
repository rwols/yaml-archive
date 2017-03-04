#include "A.hpp"
#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>
#if BOOST_VERSION > 105500
#include <boost/serialization/forward_list.hpp>
#endif

BOOST_FIXTURE_TEST_CASE(std_forward_list, io_fixture)
{
#if BOOST_VERSION > 105500
    std::forward_list<A> alist, alist1;
    alist.push_front(A());
    alist.push_front(A());
    {
        output() << BOOST_SERIALIZATION_NVP(alist);
    }
    {
        input() >> boost::serialization::make_nvp("alist", alist1);
    }
    BOOST_CHECK(alist == alist1);
#else
    BOOST_TEST_MESSAGE("serialization of std::forward_list requires at least "
                       "boost version 1.56!");
#endif
}
