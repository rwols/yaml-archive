#include "io_fixture.hpp"
#include "polymorphic_base.hpp"
#include "polymorphic_derived1.hpp"
#include "polymorphic_derived2.hpp"
#include <boost/serialization/export.hpp>
#include <boost/test/unit_test.hpp>

#define NVP(name) BOOST_SERIALIZATION_NVP(name)
using boost::serialization::make_nvp;

BOOST_FIXTURE_TEST_CASE(exported, io_fixture)
{
    // save exported polymorphic class
    polymorphic_base* rb1 = new polymorphic_derived1;
    polymorphic_base* rb2 = new polymorphic_derived2;
    {
        output() << NVP(rb1) << NVP(rb2);
    }
    delete rb1;
    delete rb2;
    rb1 = nullptr;
    rb2 = nullptr;
    {
        input() >> NVP(rb1) >> NVP(rb2);
    }

    BOOST_CHECK_MESSAGE(boost::serialization::type_info_implementation<
                            polymorphic_derived1>::type::get_const_instance() ==
                            *boost::serialization::type_info_implementation<
                                 polymorphic_base>::type::get_const_instance()
                                 .get_derived_extended_type_info(*rb1),
                        "restored pointer b1 not of correct type");

    BOOST_CHECK_MESSAGE(boost::serialization::type_info_implementation<
                            polymorphic_derived2>::type::get_const_instance() ==
                            *boost::serialization::type_info_implementation<
                                 polymorphic_base>::type::get_const_instance()
                                 .get_derived_extended_type_info(*rb2),
                        "restored pointer b2 not of correct type");

    delete rb1;
    delete rb2;
}
