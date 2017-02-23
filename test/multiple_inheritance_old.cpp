/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_mi.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// test of serialization of base classes with multiple inheritance
// contributed by Peter Dimov

#include "io_fixture.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

class base
{
  private:
    friend class boost::serialization::access;
    int x;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(x);
    }

  public:
    explicit base(int x = 0) : x(x) {}
    virtual ~base(); // = 0;
    int get_x() const { return x; }
};

inline base::~base() {}

class derived1
{
  private:
    int y;
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(y);
    }

  public:
    explicit derived1(int y = 0) : y(y) {}
    virtual ~derived1() {}
    int get_y() const { return y; }
};

BOOST_CLASS_EXPORT_GUID(derived1, "derived1asdf");

class derived2 : public base, public derived1
{
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(base);
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(derived1);
    }

  public:
    derived2() {}
    derived2(int x, int y) : base(x), derived1(y) {}
};

BOOST_CLASS_EXPORT_GUID(derived2, "derived2asdf");

BOOST_FIXTURE_TEST_CASE(multiple_inheritance, io_fixture)
{
    derived2* pc = new derived2(1, 2);
    base*     pa = pc;
    derived1* pb = pc;

    BOOST_CHECK(pa == pc);
    BOOST_CHECK(pb == pc);

    int x = pa->get_x();
    int y = pb->get_y();

    BOOST_TEST_PASSPOINT();

    BOOST_TEST_MESSAGE("pa->get_x(): " << pa->get_x());
    BOOST_TEST_MESSAGE("pb->get_y(): " << pb->get_y());
    {
        output() << BOOST_SERIALIZATION_NVP(pc) << BOOST_SERIALIZATION_NVP(pa)
                 << BOOST_SERIALIZATION_NVP(pb);
    }
    BOOST_TEST_PASSPOINT();

    delete pc;
    pc = nullptr;
    pa = nullptr;
    pb = nullptr;
    {
        input() >> BOOST_SERIALIZATION_NVP(pc) >> BOOST_SERIALIZATION_NVP(pa) >>
            BOOST_SERIALIZATION_NVP(pb);
    }

    BOOST_TEST_PASSPOINT();

    BOOST_CHECK(pa == pc);
    BOOST_CHECK(pb == pc);

    BOOST_TEST_PASSPOINT();

    BOOST_CHECK(x == pa->get_x());
    BOOST_CHECK(y == pb->get_y());

    BOOST_TEST_PASSPOINT();

    BOOST_TEST_MESSAGE("pa->get_x(): " << pa->get_x());
    BOOST_TEST_MESSAGE("pb->get_y(): " << pb->get_y());

    BOOST_TEST_PASSPOINT();

    delete pc;
}
