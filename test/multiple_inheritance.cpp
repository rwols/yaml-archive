/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_multiple_inheritance.cpp

// (C) Copyright 2009 Robert Ramey.
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// test of serialization library for multiple inheritence situations

#include "io_fixture.hpp"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

struct Base1
{
    int m_x;
    Base1() {}
    Base1(int x) : m_x(1 + x) {}
    virtual ~Base1() {}
    bool operator==(Base1& rhs) const { return m_x == rhs.m_x; }
    // serialize
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(m_x);
    }
};

// BOOST_CLASS_EXPORT(Base1)

struct Base2
{
    int m_x;
    Base2() {}
    Base2(int x) : m_x(2 + x) {}
    virtual ~Base2() {}
    bool operator==(Base2& rhs) const { return m_x == rhs.m_x; }
    // serialize
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(m_x);
    }
};

// BOOST_CLASS_EXPORT(Base2)

struct Sub : public Base1, public Base2
{
    int m_x;
    Sub() {}
    Sub(int x) : Base1(x), Base2(x), m_x(x) {}
    bool operator==(Sub& rhs) const
    {
        if (!Base2::operator==(rhs)) return false;
        if (!Base1::operator==(rhs)) return false;
        return m_x == rhs.m_x;
    }
    virtual ~Sub() {}

    // serialize
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base1);
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base2);
        ar& BOOST_SERIALIZATION_NVP(m_x);
    }
};

BOOST_CLASS_EXPORT(Sub)

BOOST_FIXTURE_TEST_CASE(multiple_inheritance, io_fixture)
{
    Base2* pb2;
    {
        // serialize
        pb2 = new Sub(2);
        output() << boost::serialization::make_nvp("Base2", pb2);
    }
    Base2* pb2_1;
    {
        // de-serialize
        input() >> boost::serialization::make_nvp("Base2", pb2_1);
    }
    Sub* s1 = dynamic_cast<Sub*>(pb2);
    BOOST_CHECK(0 != s1);
    Sub* s2 = dynamic_cast<Sub*>(pb2_1);
    BOOST_CHECK(0 != s2);
    BOOST_CHECK(*s1 == *s2);
}
