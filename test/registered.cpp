/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_registered.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "io_fixture.hpp"
#include "polymorphic_derived1.hpp"
#include "polymorphic_derived2.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_CASE(registered1, io_fixture)
{
    polymorphic_derived1* rd1 = new polymorphic_derived1;
    polymorphic_derived2* rd2 = new polymorphic_derived2;
    polymorphic_base*     rb1 = rd1;
    polymorphic_base*     rb2 = rd2;

    {
        test_ostream  os(get_filename().c_str(), STREAM_FLAGS);
        test_oarchive oa(os, ARCHIVE_FLAGS);

        // registration IS necessary when serializing pointers of
        // polymorphic classes
        oa.register_type(static_cast<polymorphic_derived1*>(NULL));
        oa.register_type(static_cast<polymorphic_derived2*>(NULL));
        oa << BOOST_SERIALIZATION_NVP(rd1);
        oa << BOOST_SERIALIZATION_NVP(rd2);

        oa << BOOST_SERIALIZATION_NVP(rb1);
        oa << BOOST_SERIALIZATION_NVP(rb2);
    }

    delete rd1;
    delete rd2;
    rd1 = nullptr;
    rd2 = nullptr;
    {
        test_istream  is(get_filename().c_str(), STREAM_FLAGS);
        test_iarchive ia(is, ARCHIVE_FLAGS);
        // registration IS necessary when serializing pointers of
        // polymorphic classes
        ia.register_type(static_cast<polymorphic_derived1*>(nullptr));
        ia.register_type(static_cast<polymorphic_derived2*>(nullptr));

        ia >> BOOST_SERIALIZATION_NVP(rd1);
        ia >> BOOST_SERIALIZATION_NVP(rd2);

        // the above opereration registers the derived classes as a side
        // effect.  Hence, instances can now be correctly serialized through
        // a base class pointer.
        ia >> BOOST_SERIALIZATION_NVP(rb1);
        ia >> BOOST_SERIALIZATION_NVP(rb2);
    }

    const boost::serialization::extended_type_info* p1;
    p1 = &boost::serialization::type_info_implementation<
        polymorphic_derived1>::type::get_const_instance();

    BOOST_CHECK(nullptr != p1);

    const boost::serialization::extended_type_info* p2;
    p2 = boost::serialization::type_info_implementation<
             polymorphic_derived1>::type::get_const_instance()
             .get_derived_extended_type_info(*rd1);

    BOOST_CHECK(nullptr != p2);

    BOOST_CHECK_MESSAGE(p1 == p2, "restored pointer d1 not of correct type");

    BOOST_CHECK_MESSAGE(
        &boost::serialization::type_info_implementation<
            polymorphic_derived2>::type::get_const_instance() ==
            boost::serialization::type_info_implementation<
                polymorphic_derived2>::type::get_const_instance()
                .get_derived_extended_type_info(*rd2),
        "restored pointer d2 not of correct type");

    BOOST_CHECK_MESSAGE(rb1 == dynamic_cast<polymorphic_base*>(rd1),
                        "serialized pointers not correctly restored");

    p1 = &boost::serialization::type_info_implementation<
        polymorphic_derived1>::type::get_const_instance();

    BOOST_CHECK(nullptr != p1);

    p2 = boost::serialization::type_info_implementation<
             polymorphic_base>::type::get_const_instance()
             .get_derived_extended_type_info(*rb1);

    BOOST_CHECK(nullptr != p2);

    BOOST_CHECK_MESSAGE(p1 == p2, "restored pointer b1 not of correct type");

    BOOST_CHECK_MESSAGE(rb2 == dynamic_cast<polymorphic_base*>(rd2),
                        "serialized pointers not correctly restored");

    BOOST_CHECK_MESSAGE(&boost::serialization::type_info_implementation<
                            polymorphic_derived2>::type::get_const_instance() ==
                            boost::serialization::type_info_implementation<
                                polymorphic_base>::type::get_const_instance()
                                .get_derived_extended_type_info(*rb2),
                        "restored pointer b2 not of correct type");

    delete rb1;
    delete rb2;
}

BOOST_FIXTURE_TEST_CASE(registered2, io_fixture)
{

    polymorphic_base* rb1 = new polymorphic_derived1;
    polymorphic_base* rb2 = new polymorphic_derived2;

    {
        test_ostream  os(get_filename().c_str(), STREAM_FLAGS);
        test_oarchive oa(os, ARCHIVE_FLAGS);

        // registration (forward declaration) will permit correct serialization
        // through a pointer to a base class
        oa.register_type(static_cast<polymorphic_derived1*>(nullptr));
        oa.register_type(static_cast<polymorphic_derived2*>(nullptr));
        oa << BOOST_SERIALIZATION_NVP(rb1) << BOOST_SERIALIZATION_NVP(rb2);
    }

    delete rb1;
    delete rb2;

    rb1 = nullptr;
    rb2 = nullptr;

    {
        test_istream  is(get_filename().c_str(), STREAM_FLAGS);
        test_iarchive ia(is, ARCHIVE_FLAGS);

        // registration (forward declaration) will permit correct serialization
        // through a pointer to a base class
        ia.register_type(static_cast<polymorphic_derived1*>(nullptr));
        ia.register_type(static_cast<polymorphic_derived2*>(nullptr));
        ia >> BOOST_SERIALIZATION_NVP(rb1) >> BOOST_SERIALIZATION_NVP(rb2);
    }

    BOOST_CHECK_MESSAGE(&boost::serialization::type_info_implementation<
                            polymorphic_derived1>::type::get_const_instance() ==
                            boost::serialization::type_info_implementation<
                                polymorphic_base>::type::get_const_instance()
                                .get_derived_extended_type_info(*rb1),
                        "restored pointer b1 not of correct type");

    BOOST_CHECK_MESSAGE(&boost::serialization::type_info_implementation<
                            polymorphic_derived2>::type::get_const_instance() ==
                            boost::serialization::type_info_implementation<
                                polymorphic_base>::type::get_const_instance()
                                .get_derived_extended_type_info(*rb2),
                        "restored pointer b2 not of correct type");

    delete rb1;
    delete rb2;
}

// EOF
