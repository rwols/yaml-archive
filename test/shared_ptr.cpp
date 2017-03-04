/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_shared_ptr.cpp

// (C) Copyright 2002 Robert Ramey- http://www.rrsd.com - David Tonge  .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org for updates, documentation, and revision history.

#include "io_fixture.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>
#include <boost/test/unit_test.hpp>

// This is a simple class.  It contains a counter of the number
// of objects of this class which have been instantiated.
class Acount
{
  private:
    friend class boost::serialization::access;
    int x;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(x);
    }
    Acount(Acount const& rhs);
    Acount& operator=(Acount const& rhs);

  public:
    static int count;
    bool operator==(const Acount& rhs) const { return x == rhs.x; }
    Acount() { ++count; }          // default constructor
    virtual ~Acount() { --count; } // default destructor
};

BOOST_SERIALIZATION_SHARED_PTR(Acount)

int Acount::count = 0;

// Bcount is a subclass of Acount
class Bcount : public Acount
{
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Acount);
    }

  public:
    static int count;
    Bcount() : Acount(){};
    virtual ~Bcount(){};
};

int Bcount::count = 0;

// Bcount needs to be exported because its serialized via a base class pointer
BOOST_CLASS_EXPORT(Bcount)
BOOST_SERIALIZATION_SHARED_PTR(Bcount)

// test a non-polymorphic class
class Ccount
{
  private:
    friend class boost::serialization::access;
    int z;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(z);
    }

  public:
    static int count;
    bool operator==(const Ccount& rhs) const { return z == rhs.z; }
    Ccount() : z(++count) // default constructor
    {
    }
    virtual ~Ccount() { --count; } // default destructor
};

int Ccount::count = 0;

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_1, io_fixture)
{
    {
        boost::shared_ptr<Acount> ptr;
        check_roundtrip(ptr);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_2, io_fixture)
{
    {
        boost::shared_ptr<Acount> ptr(new Acount());
        boost::shared_ptr<Acount> ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr);
        }
        {
            input() >> boost::serialization::make_nvp("ptr", ptr1);
        }
        BOOST_CHECK(*ptr == *ptr1);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_3, io_fixture)
{
    {
        boost::shared_ptr<Acount> ptr1(new Acount());
        boost::shared_ptr<Acount> ptr2 = ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr1)
                     << BOOST_SERIALIZATION_NVP(ptr2);
        }
        ptr1.reset();
        ptr2.reset();
        {
            input() >> BOOST_SERIALIZATION_NVP(ptr1) >>
                BOOST_SERIALIZATION_NVP(ptr2);
        }
        BOOST_CHECK(ptr1 == ptr2);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_4, io_fixture)
{
    {
        boost::shared_ptr<Acount> ptr1(new Bcount());
        boost::shared_ptr<Acount> ptr2 = ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr1)
                     << BOOST_SERIALIZATION_NVP(ptr2);
        }
        ptr1.reset();
        ptr2.reset();
        {
            input() >> BOOST_SERIALIZATION_NVP(ptr1) >>
                BOOST_SERIALIZATION_NVP(ptr2);
        }
        BOOST_CHECK(ptr1 == ptr2);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
    BOOST_CHECK_EQUAL(0, Bcount::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_5, io_fixture)
{
    {
        boost::shared_ptr<Acount> ptr1(new Acount());
        boost::shared_ptr<Acount> ptr2 = ptr1;
        boost::weak_ptr<Acount>   weak = ptr1;
        {
            // note that we deserialize the weak pointer first in order to test
            // that a temporarily solitary weak pointer is correctly restored.
            output() << BOOST_SERIALIZATION_NVP(weak)
                     << BOOST_SERIALIZATION_NVP(ptr1)
                     << BOOST_SERIALIZATION_NVP(ptr2);
        }
        ptr1.reset();
        ptr2.reset();
        weak.reset();
        {
            // note that we deserialize the weak pointer first in order to test
            // that a temporarily solitary weak pointer is correctly restored.
            input() >> BOOST_SERIALIZATION_NVP(weak) >>
                BOOST_SERIALIZATION_NVP(ptr1) >> BOOST_SERIALIZATION_NVP(ptr2);
        }
        BOOST_CHECK(ptr1 == ptr2);
        BOOST_CHECK(ptr1 == weak.lock());
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_6, io_fixture)
{
    {
        boost::shared_ptr<Ccount> ptr(new Ccount());
        boost::shared_ptr<Ccount> ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr);
        }
        {
            input() >> boost::serialization::make_nvp("ptr", ptr1);
        }
        BOOST_CHECK(*ptr == *ptr1);
    }
    BOOST_CHECK_EQUAL(0, Ccount::count);
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_1, io_fixture)
{
#if BOOST_VERSION >= 105600
    {
        std::shared_ptr<Acount> ptr;
        check_roundtrip(ptr);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
#else
    BOOST_TEST_MESSAGE(
        "serialization of std::shared_ptr requires boost version >= 1.56");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_2, io_fixture)
{
#if BOOST_VERSION >= 105600
    {
        std::shared_ptr<Acount> ptr(new Acount());
        std::shared_ptr<Acount> ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr);
        }
        {
            input() >> boost::serialization::make_nvp("ptr", ptr1);
        }
        BOOST_CHECK(*ptr == *ptr1);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
#else
    BOOST_TEST_MESSAGE(
        "serialization of std::shared_ptr requires boost version >= 1.56");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_3, io_fixture)
{
#if BOOST_VERSION >= 105600
    {
        std::shared_ptr<Acount> ptr1(new Acount());
        std::shared_ptr<Acount> ptr2 = ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr1)
                     << BOOST_SERIALIZATION_NVP(ptr2);
        }
        ptr1.reset();
        ptr2.reset();
        {
            input() >> BOOST_SERIALIZATION_NVP(ptr1) >>
                BOOST_SERIALIZATION_NVP(ptr2);
        }
        BOOST_CHECK(ptr1 == ptr2);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
#else
    BOOST_TEST_MESSAGE(
        "serialization of std::shared_ptr requires boost version >= 1.56");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_4, io_fixture)
{
#if BOOST_VERSION >= 105600
    {
        std::shared_ptr<Acount> ptr1(new Bcount());
        std::shared_ptr<Acount> ptr2 = ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr1)
                     << BOOST_SERIALIZATION_NVP(ptr2);
        }
        ptr1.reset();
        ptr2.reset();
        {
            input() >> BOOST_SERIALIZATION_NVP(ptr1) >>
                BOOST_SERIALIZATION_NVP(ptr2);
        }
        BOOST_CHECK(ptr1 == ptr2);
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
    BOOST_CHECK_EQUAL(0, Bcount::count);
#else
    BOOST_TEST_MESSAGE(
        "serialization of std::shared_ptr requires boost version >= 1.56");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_5, io_fixture)
{
#if BOOST_VERSION >= 105600
    {
        std::shared_ptr<Acount> ptr1(new Acount());
        std::shared_ptr<Acount> ptr2 = ptr1;
        std::weak_ptr<Acount>   weak = ptr1;
        {
            // note that we deserialize the weak pointer first in order to test
            // that a temporarily solitary weak pointer is correctly restored.
            output() << BOOST_SERIALIZATION_NVP(weak)
                     << BOOST_SERIALIZATION_NVP(ptr1)
                     << BOOST_SERIALIZATION_NVP(ptr2);
        }
        ptr1.reset();
        ptr2.reset();
        weak.reset();
        {
            // note that we deserialize the weak pointer first in order to test
            // that a temporarily solitary weak pointer is correctly restored.
            input() >> BOOST_SERIALIZATION_NVP(weak) >>
                BOOST_SERIALIZATION_NVP(ptr1) >> BOOST_SERIALIZATION_NVP(ptr2);
        }
        BOOST_CHECK(ptr1 == ptr2);
        BOOST_CHECK(ptr1 == weak.lock());
    }
    BOOST_CHECK_EQUAL(0, Acount::count);
#else
    BOOST_TEST_MESSAGE(
        "serialization of std::shared_ptr requires boost version >= 1.56");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_6, io_fixture)
{
#if BOOST_VERSION >= 105600
    {
        std::shared_ptr<Ccount> ptr(new Ccount());
        std::shared_ptr<Ccount> ptr1;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr);
        }
        {
            input() >> boost::serialization::make_nvp("ptr", ptr1);
        }
        BOOST_CHECK(*ptr == *ptr1);
    }
    BOOST_CHECK_EQUAL(0, Ccount::count);
#else
    BOOST_TEST_MESSAGE(
        "serialization of std::shared_ptr requires boost version >= 1.56");
#endif
}
