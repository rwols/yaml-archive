/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_object.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// test implementation level "object_serializable"
// should pass compilation and execution

#include "io_fixture.hpp"
#include <boost/serialization/level.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

class obj_ser
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /* ar */, const unsigned int /* file_version */)
    {
    }
};

BOOST_CLASS_IMPLEMENTATION(obj_ser, boost::serialization::object_serializable)

// note: version can be assigned only to objects whose implementation
// level is object_class_info.  So, doing the following will result in
// a static assertion
// BOOST_CLASS_VERSION(A, 2);

BOOST_FIXTURE_TEST_CASE(object_serializable, io_fixture)
{
    obj_ser a;
    {
        output() << BOOST_SERIALIZATION_NVP(a);
    }
    {
        input() >> BOOST_SERIALIZATION_NVP(a);
    }
}

// EOF
