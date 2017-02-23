#include "A.hpp"
#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>
#include <deque>

BOOST_FIXTURE_TEST_CASE(std_deque, io_fixture)
{
    std::deque<A> adeque, adeque1;
    adeque.push_front(A());
    adeque.push_front(A());
    {
        output() << BOOST_SERIALIZATION_NVP(adeque);
    }
    {
        input() >> boost::serialization::make_nvp("adeque", adeque1);
    }
    BOOST_CHECK(adeque == adeque1);
}
