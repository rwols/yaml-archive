/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_no_rtti.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// note: this program tests the inter-operability of different
// extended typeinfo systems.  In this example, one class is
// identified using the default RTTI while the other uses a custom
// system based on the export key.
//
// As this program uses RTTI for one of the types, the test will fail
// on a system for which RTTI is not enabled or not existent.

#include "io_fixture.hpp"
#include "polymorphic_base.hpp"
#include "polymorphic_derived1.hpp"
#include "polymorphic_derived2.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/extended_type_info_no_rtti.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/test/unit_test.hpp>

template <class Archive>
void polymorphic_derived2::serialize(Archive& ar,
                                     const unsigned int /* file_version */
                                     )
{
    ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(polymorphic_base);
}

template void polymorphic_derived2::serialize(test_oarchive&     ar,
                                              const unsigned int version);
template void polymorphic_derived2::serialize(test_iarchive&     ar,
                                              const unsigned int version);

#define NVP(name) BOOST_SERIALIZATION_NVP(name)

BOOST_FIXTURE_TEST_CASE(no_rtti, io_fixture)
{
    polymorphic_derived1*   rd1 = new polymorphic_derived1;
    polymorphic_derived2*   rd2 = new polymorphic_derived2;
    const polymorphic_base* rb1 = rd1;
    polymorphic_base*       rb2 = rd2;

    BOOST_TEST_MESSAGE("saving polymorphic_derived1 (no_rtti)");
    BOOST_TEST_MESSAGE("saving polymorphic_derived2");
    BOOST_TEST_MESSAGE("saving polymorphic_derived2 through base");
    BOOST_TEST_MESSAGE(
        "saving polymorphic_derived1 (no_rtti) through base (no_rtti)");
    {
        output() << NVP(rd1) << NVP(rd2) << NVP(rb1) << NVP(rb2);
    }

    delete rd1;
    delete rd2;
    rd1 = nullptr;
    rd2 = nullptr;
    rb1 = nullptr;
    rb2 = nullptr;

    BOOST_TEST_MESSAGE("loading polymorphic_derived1 (no_rtti)");
    BOOST_TEST_MESSAGE(
        "loading polymorphic_derived1 (no_rtti) through base (no_rtti)");
    BOOST_TEST_MESSAGE("loading polymorphic_derived2");
    BOOST_TEST_MESSAGE("loading polymorphic_derived2 through base (no_rtti)");
    {
        input() >> NVP(rd1) >> NVP(rd2) >> NVP(rb1) >> NVP(rb2);
    }

    BOOST_CHECK_MESSAGE(
        boost::serialization::type_info_implementation<
            polymorphic_derived1>::type::get_const_instance() ==
            *boost::serialization::type_info_implementation<
                 polymorphic_derived1>::type::get_const_instance()
                 .get_derived_extended_type_info(*rd1),
        "restored pointer d1 not of correct type");

    BOOST_CHECK_MESSAGE(
        boost::serialization::type_info_implementation<
            polymorphic_derived2>::type::get_const_instance() ==
            *boost::serialization::type_info_implementation<
                 polymorphic_derived2>::type::get_const_instance()
                 .get_derived_extended_type_info(*rd2),
        "restored pointer d2 not of correct type");

    // the above opereration registers the derived classes as a side
    // effect.  Hence, instances can now be correctly serialized through
    // a base class pointer.
    BOOST_CHECK_MESSAGE(rb1 == dynamic_cast<polymorphic_base*>(rd1),
                        "serialized pointers not correctly restored");

    BOOST_CHECK_MESSAGE(boost::serialization::type_info_implementation<
                            polymorphic_derived1>::type::get_const_instance() ==
                            *boost::serialization::type_info_implementation<
                                 polymorphic_base>::type::get_const_instance()
                                 .get_derived_extended_type_info(*rb1),
                        "restored pointer b1 not of correct type");

    BOOST_CHECK_MESSAGE(rb2 == dynamic_cast<polymorphic_base*>(rd2),
                        "serialized pointers not correctly restored");
    BOOST_CHECK_MESSAGE(boost::serialization::type_info_implementation<
                            polymorphic_derived2>::type::get_const_instance() ==
                            *boost::serialization::type_info_implementation<
                                 polymorphic_base>::type::get_const_instance()
                                 .get_derived_extended_type_info(*rb2),
                        "restored pointer b2 not of correct type");
    delete rb1;
    delete rb2;
}

// EOF
