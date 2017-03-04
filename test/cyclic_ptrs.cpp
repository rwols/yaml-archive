/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_cyclic_ptrs.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/version.hpp>
#include <boost/test/unit_test.hpp>
#if BOOST_VERSION < 1005600
#include <boost/detail/no_exceptions_support.hpp>
#else
#include <boost/core/no_exceptions_support.hpp>
#endif

#define NVP(name) BOOST_SERIALIZATION_NVP(name)
using boost::serialization::make_nvp;

/**
 * @brief      Class with a member that refers to itself
 */
class J1 : public A
{
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(A);
        ar& NVP(j);
    }

  public:
    bool operator==(const J1& rhs) const;
    J1* j;
    J1(J1* _j) : j(_j) {}
    J1() : j(NULL) {}
};

std::ostream& operator<<(std::ostream& os, const J1& j)
{
    return os << "<J1 " << static_cast<const A&>(j) << ", j: " << j.j << ">";
}

BOOST_CLASS_VERSION(J1, 6)

bool J1::operator==(const J1& rhs) const
{
    return static_cast<const A&>(*this) == static_cast<const A&>(rhs);
}

BOOST_FIXTURE_TEST_CASE(cyclic_pointers_1, io_fixture)
{
    J1 j;
    check_roundtrip(j);
}

BOOST_FIXTURE_TEST_CASE(cyclic_pointers_2, io_fixture)
{
    J1* j1 = new J1;
    j1->j = j1;
    J1* j2 = reinterpret_cast<J1*>(0xBAADF00D);
    {
        output() << NVP(j1);
    }
    {
        input() >> make_nvp("j1", j2);
    }
    BOOST_CHECK_EQUAL(*j1, *j2);
    BOOST_CHECK_EQUAL(j2, j2->j);
}

/**
 * @brief      Class with members that refer to each other
 *
 * @details    this is an example of a class that, as written, cannot be
 *             serialized with this system. The problem is that the
 *             serialization of the first member - j1, provokes serialization
 *             of those objects which it points to either directly or
 *             indirectly. When those objects are subsequently serialized, it is
 *             discovered that have already been serialized through pointers.
 *             This is detected by the system and an exception
 *             - pointer_conflict - is thrown. Permiting this to go undetected
 *             would result in the creation of multiple equal objects rather
 *             than the original structure.
 */
class K
{
    J1 j1;
    J1 j2;
    J1 j3;
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& NVP(j1);
        ar& NVP(j2);
        ar& NVP(j3);
    }

  public:
    bool operator==(const K& rhs) const;
    K();
};

K::K() : j1(&j2), j2(&j3), j3(&j1) {}

bool K::operator==(const K& rhs) const
{
    return j1.j == &j2 && j2.j == &j3 && j3.j == &j1 && j1 == rhs.j1 &&
           j2 == rhs.j2 && j3 == rhs.j3;
}

BOOST_FIXTURE_TEST_CASE(cyclic_pointers_3, io_fixture)
{
    K k;
    BOOST_REQUIRE_THROW(output() << NVP(k), boost::archive::archive_exception);
    try
    {
        output() << NVP(k);
    }
    catch (const boost::archive::archive_exception& e)
    {
        BOOST_REQUIRE_EQUAL(
            e.code, boost::archive::archive_exception::pointer_conflict);
    }
}

// EOF
