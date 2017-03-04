#include "A.hpp"
#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>
#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
#include <forward_list>
#endif

BOOST_FIXTURE_TEST_CASE(std_forward_list, io_fixture)
{
#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
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
    BOOST_TEST_MESSAGE("std::forward_list not supported!");
#endif
}
