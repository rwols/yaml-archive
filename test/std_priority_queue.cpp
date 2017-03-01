/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_priority_queue.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

// must come BEFORE boost/serialization/priority_queue.hpp
#include <boost/serialization/vector.hpp>

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/serialization/priority_queue.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(std_priority_queue, io_fixture)
{
    std::priority_queue<A, std::vector<A>> a_priority_queue, a_priority_queue1;
    a_priority_queue.push(A());
    a_priority_queue.push(A());
    a_priority_queue.push(A());
    a_priority_queue.push(A());
    {
        output() << BOOST_SERIALIZATION_NVP(a_priority_queue);
    }
    {
        input() >> boost::serialization::make_nvp("a_priority_queue",
                                                  a_priority_queue1);
    }
    for (int i = a_priority_queue.size(); i-- > 0;)
    {
        const A& a1 = a_priority_queue.top();
        const A& a2 = a_priority_queue1.top();
        BOOST_CHECK_EQUAL(a1, a2);
        a_priority_queue.pop();
        a_priority_queue1.pop();
    }
}

// EOF
