#include "A.hpp"
#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>
#include <forward_list>

BOOST_FIXTURE_TEST_CASE(std_forward_list, io_fixture)
{
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
}
