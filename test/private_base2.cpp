/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_private_base.cpp

// (C) Copyright 2009 Eric Moyer - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

// invoke header for a custom archive test.

#include "io_fixture.hpp"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/test/unit_test.hpp>

class Base
{
    friend class boost::serialization::access;
    int m_i;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& BOOST_SERIALIZATION_NVP(m_i);
    }

  protected:
    bool equals(const Base& rhs) const { return m_i == rhs.m_i; }
    Base(int i = 0) : m_i(i) {}
    virtual ~Base(){};

  public:
    virtual bool operator==(const Base& rhs) const { return false; } // = 0;
};

class Derived : private Base
{
    friend class boost::serialization::access;

  public:
    virtual bool operator==(const Derived& rhs) const
    {
        return Base::equals(static_cast<const Base&>(rhs));
    }
    Derived(int i = 0) : Base(i) {}
};

// BOOST_CLASS_EXPORT(Derived)

BOOST_FIXTURE_TEST_CASE(private_base2, io_fixture)
{
    Derived  a(1), a1(2);
    Derived* ta = &a;
    Derived* ta1 = nullptr;
    {
        output() << BOOST_SERIALIZATION_NVP(a) << BOOST_SERIALIZATION_NVP(ta);
    }
    {
        input() >> boost::serialization::make_nvp("a", a1) >>
            boost::serialization::make_nvp("ta", ta1);
    }
    BOOST_CHECK_EQUAL(a, a1);
    BOOST_CHECK(ta != ta1);
    BOOST_CHECK(*ta == *ta1);
}
