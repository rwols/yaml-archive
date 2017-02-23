#include "B.hpp"
#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

#define NVP(var) BOOST_SERIALIZATION_NVP(var)
using boost::serialization::make_nvp;

/**
 * @brief      Simple base class
 */
class base
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /* ar */, const unsigned int /* file_version */)
    {
    }

  public:
    virtual ~base(){};
};

std::ostream& operator<<(std::ostream& os, const base&)
{
    return os << "<base>";
}

/**
 * @brief      Simple derived class 1
 */
class derived1 : public base
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(base);
    }
};

std::ostream& operator<<(std::ostream& os, const derived1&)
{
    return os << "<derived1>";
}

/**
 * @brief      Simple derived class 2
 */
class derived2 : public base
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(base);
    }
};

std::ostream& operator<<(std::ostream& os, const derived2&)
{
    return os << "<derived2>";
}

BOOST_FIXTURE_TEST_CASE(derived, io_fixture)
{
    // registration not necessary when serializing the most derived pointer
    derived1* d1 = new derived1;
    derived2* d2 = new derived2;
    // upcasting non-polymorphic pointers may not lead to the expected
    // result. In the current type id system
    base* b1 = d1;
    base* b2 = d2;
    {
        output() << NVP(d1) << NVP(d2) << NVP(b1) << NVP(b2);
    }

    // Warning, the current type id system does not yield true
    // type id for non-polymorphic types
    const boost::serialization::extended_type_info& this_type =
        boost::serialization::type_info_implementation<
            base>::type::get_const_instance();
    // retrieve the true type of the object pointed to
    const boost::serialization::extended_type_info& true_type =
        *boost::serialization::type_info_implementation<
             base>::type::get_const_instance()
             .get_derived_extended_type_info(*b1);

    BOOST_WARN_MESSAGE(
        !(this_type == true_type),
        "current type id system does not support non-polymorphic types");

    delete d1;
    delete d2;

    // registration not necessary when serializing the most derived pointer
    d1 = nullptr;
    d2 = nullptr;
    // upcasting non-polymorphic pointers may not lead to the expected
    // result. In the current type id system
    b1 = nullptr;
    b2 = nullptr;
    {
        input() >> NVP(d1) >> NVP(d2) >> NVP(b1) >> NVP(b2);
    }

    // Warning, the current type id system does not yield true
    // type id for non-polymorphic types
    const boost::serialization::extended_type_info& this_type2 =
        boost::serialization::type_info_implementation<
            base>::type::get_const_instance();
    // retrieve the true type of the object pointed to
    const boost::serialization::extended_type_info& true_type2 =
        *boost::serialization::type_info_implementation<
             base>::type::get_const_instance()
             .get_derived_extended_type_info(*b1);

    BOOST_WARN_MESSAGE(
        !(this_type2 == true_type2),
        "current type id system does fails for non-polymorphic types");

    BOOST_CHECK_EQUAL(b1, static_cast<base*>(d1));
    BOOST_CHECK_EQUAL(b2, static_cast<base*>(d2));

    delete d1;
    delete d2;
}

BOOST_FIXTURE_TEST_CASE(derived_class, io_fixture)
{
    B b;
    check_roundtrip(b);
}

BOOST_FIXTURE_TEST_CASE(derived_class_ptr, io_fixture)
{
    B* tb = new B;
    B* tb1 = nullptr;
    {
        output() << NVP(tb);
    }
    {
        input() >> make_nvp("tb", tb1);
    }
    BOOST_CHECK(tb != tb1);
    BOOST_CHECK_EQUAL(*tb, *tb1);
    delete tb;
}
