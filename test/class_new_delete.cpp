/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_new_operator.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/serialization/access.hpp>
#include <boost/test/unit_test.hpp>
#include <new>

class ANew : public A
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned /*file_version*/)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(A);
    }

  public:
    static unsigned int m_new_calls;
    static unsigned int m_delete_calls;
    // implement class specific new/delete in terms standard
    // implementation - we're testing serialization
    // not "new" here.
    static void* operator new(size_t s)
    {
        ++m_new_calls;
        return ::operator new(s);
    }
    static void operator delete(void* p, std::size_t)
    {
        ++m_delete_calls;
        ::operator delete(p);
    }
};
unsigned int ANew::m_new_calls = 0;
unsigned int ANew::m_delete_calls = 0;

class ANew1 : public A
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned /*file_version*/)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(A);
    }

  public:
    static unsigned int m_new_calls;
    static unsigned int m_delete_calls;
    // implement class specific new/delete in terms standard
    // implementation - we're testing serialization
    // not "new" here.
    static void* operator new(size_t s)
    {
        ++m_new_calls;
        return ::operator new(s);
    }
    static void operator delete(void* p)
    {
        ++m_delete_calls;
        ::operator delete(p);
    }
};
unsigned int ANew1::m_new_calls = 0;
unsigned int ANew1::m_delete_calls = 0;

class ANew2 : public A
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned /*file_version*/)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(A);
    }

  public:
    static unsigned int m_new_calls;
    static unsigned int m_delete_calls;
    // implement class specific new/delete in terms standard
    // implementation - we're testing serialization
    // not "new" here.
    static void* operator new(size_t s)
    {
        ++m_new_calls;
        return ::operator new(s);
    }
};
unsigned int ANew2::m_new_calls = 0;
unsigned int ANew2::m_delete_calls = 0;

BOOST_FIXTURE_TEST_CASE(class_new_delete_0, io_fixture)
{
    ANew* ta = new ANew();

    BOOST_CHECK(1 == ANew::m_new_calls);
    BOOST_CHECK(0 == ANew::m_delete_calls);

    ANew* ta1 = nullptr;
    {
        output() << BOOST_SERIALIZATION_NVP(ta);
    }
    {
        input() >> boost::serialization::make_nvp("ta", ta1);
    }
    BOOST_CHECK(ta != ta1);
    BOOST_CHECK(*ta == *ta1);

    BOOST_CHECK(2 == ANew::m_new_calls);
    BOOST_CHECK(0 == ANew::m_delete_calls);

    delete ta;
    delete ta1;

    BOOST_CHECK(2 == ANew::m_new_calls);
    BOOST_CHECK(2 == ANew::m_delete_calls);
}

BOOST_FIXTURE_TEST_CASE(class_new_delete_1, io_fixture)
{
    ANew1* ta = new ANew1();

    BOOST_CHECK(1 == ANew1::m_new_calls);
    BOOST_CHECK(0 == ANew1::m_delete_calls);

    ANew1* ta1 = nullptr;
    {
        output() << BOOST_SERIALIZATION_NVP(ta);
    }
    {
        input() >> boost::serialization::make_nvp("ta", ta1);
    }
    BOOST_CHECK(ta != ta1);
    BOOST_CHECK(*ta == *ta1);

    BOOST_CHECK(2 == ANew1::m_new_calls);
    BOOST_CHECK(0 == ANew1::m_delete_calls);

    delete ta;
    delete ta1;

    BOOST_CHECK(2 == ANew1::m_new_calls);
    BOOST_CHECK(2 == ANew1::m_delete_calls);
}

// Note: Testing ANew2 FAILS. To see why this is, look into the file
// iserializer line # 247. Feel free to send a patch to detect the absense
// of a class specific delete.

// EOF
