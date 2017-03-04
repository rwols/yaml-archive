/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_shared_ptr_multi_base.cpp

// (C) Copyright 2002 Robert Ramey- http://www.rrsd.com and Takatoshi Kondo.
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

struct Base1
{
    Base1() {}
    Base1(int x) : m_x(1 + x) {}
    virtual ~Base1() {}
    int m_x;
    // serialize
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(m_x);
    }
};

struct Base2
{
    Base2() {}
    Base2(int x) : m_x(2 + x) {}
    int       m_x;
    virtual ~Base2() {}
    // serialize
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(m_x);
    }
};

struct Base3
{
    Base3() {}
    Base3(int x) : m_x(3 + x) {}
    virtual ~Base3() {}
    int m_x;
    // serialize
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(m_x);
    }
};

// SubMulti is a subclass of Base1, Base1 and Base3.
struct SubMulti : public Base1, public Base2, public Base3
{
    static int count;
    SubMulti() { ++count; }
    SubMulti(int x) : Base1(x), Base2(x), m_x(x) { ++count; }
    SubMulti(const SubMulti& rhs) : m_x(rhs.m_x) { ++count; }
    virtual ~SubMulti()
    {
        assert(0 < count);
        --count;
    }
    int m_x;
    // serialize
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base1);
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base2);
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base3);
        ar& BOOST_SERIALIZATION_NVP(m_x);
    }
};

// SubMulti needs to be exported because its serialized via a base class pointer
BOOST_CLASS_EXPORT(SubMulti)
BOOST_SERIALIZATION_SHARED_PTR(SubMulti)

int SubMulti::count = 0;

template <class T, class U>
boost::shared_ptr<T>
dynamic_pointer_cast(boost::shared_ptr<U> const& u) BOOST_NOEXCEPT
{
    return boost::dynamic_pointer_cast<T>(u);
}

#ifndef BOOST_NO_CXX11_SMART_PTR
template <class T, class U>
std::shared_ptr<T>
dynamic_pointer_cast(std::shared_ptr<U> const& u) BOOST_NOEXCEPT
{
    return std::dynamic_pointer_cast<T>(u);
}
#endif

class shared_ptr_multi_base_fixture : public io_fixture
{
  protected:
    template <class A, class B, template <class> class SharedPtr,
              template <class> class WeakPtr>
    void do_test1(SharedPtr<A> ptr, WeakPtr<B> weak)
    {
        BOOST_REQUIRE(!weak.expired());
        int firstm = ptr->m_x;
        int secondm = weak.lock()->m_x;
        {
            output() << BOOST_SERIALIZATION_NVP(ptr)
                     << BOOST_SERIALIZATION_NVP(weak);
        }
        // Clear the pointers, thereby destroying the objects they contain
        ptr.reset();
        weak.reset();
        {
            input() >> BOOST_SERIALIZATION_NVP(ptr) >>
                BOOST_SERIALIZATION_NVP(weak);
        }
        // Check data member
        BOOST_CHECK(firstm == ptr->m_x);
        BOOST_CHECK(secondm == weak.lock()->m_x);
        // Check pointer to vtable
        BOOST_CHECK(::dynamic_pointer_cast<SubMulti>(ptr));
        BOOST_CHECK(::dynamic_pointer_cast<SubMulti>(weak.lock()));
    }
    template <class A, class B, template <class> class SharedPtr,
              template <class> class WeakPtr>
    void do_test2(SharedPtr<A> ptr, WeakPtr<B> weak)
    {
        BOOST_REQUIRE(!weak.expired());
        int firstm = ptr->m_x;
        int secondm = weak.lock()->m_x;
        {
            output() << BOOST_SERIALIZATION_NVP(weak)
                     << BOOST_SERIALIZATION_NVP(ptr);
        }
        // Clear the pointers, thereby destroying the objects they contain
        ptr.reset();
        weak.reset();
        {
            input() >> BOOST_SERIALIZATION_NVP(weak) >>
                BOOST_SERIALIZATION_NVP(ptr);
        }
        // Check data member
        BOOST_CHECK(firstm == ptr->m_x);
        BOOST_CHECK(secondm == weak.lock()->m_x);
        // Check pointer to vtable
        BOOST_CHECK(::dynamic_pointer_cast<SubMulti>(ptr));
        BOOST_CHECK(::dynamic_pointer_cast<SubMulti>(weak.lock()));
    }
};

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_multi_base_1,
                        shared_ptr_multi_base_fixture)
{
    boost::shared_ptr<SubMulti> ptr(new SubMulti(10));
    boost::weak_ptr<SubMulti>   weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_multi_base_2,
                        shared_ptr_multi_base_fixture)
{
    boost::shared_ptr<SubMulti> ptr(new SubMulti(10));
    boost::weak_ptr<Base1>      weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_multi_base_3,
                        shared_ptr_multi_base_fixture)
{
    boost::shared_ptr<SubMulti> ptr(new SubMulti(10));
    boost::weak_ptr<Base2>      weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_multi_base_4,
                        shared_ptr_multi_base_fixture)
{
    boost::shared_ptr<SubMulti> ptr(new SubMulti(10));
    boost::weak_ptr<Base3>      weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_multi_base_5,
                        shared_ptr_multi_base_fixture)
{
    boost::shared_ptr<SubMulti> ptr_tmp(new SubMulti(10));
    boost::shared_ptr<Base1>    ptr(ptr_tmp);
    boost::weak_ptr<Base2>      weak(ptr_tmp);
    ptr_tmp.reset();
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_multi_base_6,
                        shared_ptr_multi_base_fixture)
{
    boost::shared_ptr<SubMulti> ptr_tmp(new SubMulti(10));
    boost::shared_ptr<Base2>    ptr(ptr_tmp);
    boost::weak_ptr<Base3>      weak(ptr_tmp);
    ptr_tmp.reset();
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
}

BOOST_FIXTURE_TEST_CASE(boost_shared_ptr_multi_base_7,
                        shared_ptr_multi_base_fixture)
{
    boost::shared_ptr<SubMulti> ptr_tmp(new SubMulti(10));
    boost::shared_ptr<Base3>    ptr(ptr_tmp);
    boost::weak_ptr<Base1>      weak(ptr_tmp);
    ptr_tmp.reset();
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_multi_base_1,
                        shared_ptr_multi_base_fixture)
{
#if BOOST_VERSION >= 105600
    std::shared_ptr<SubMulti> ptr(new SubMulti(10));
    std::weak_ptr<SubMulti>   weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
#else
    BOOST_TEST_MESSAGE(
        "No std::shared_ptr support; test will automatically pass.");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_multi_base_2,
                        shared_ptr_multi_base_fixture)
{
#if BOOST_VERSION >= 105600
    std::shared_ptr<SubMulti> ptr(new SubMulti(10));
    std::weak_ptr<Base1>      weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
#else
    BOOST_TEST_MESSAGE(
        "No std::shared_ptr support; test will automatically pass.");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_multi_base_3,
                        shared_ptr_multi_base_fixture)
{
#if BOOST_VERSION >= 105600
    std::shared_ptr<SubMulti> ptr(new SubMulti(10));
    std::weak_ptr<Base2>      weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
#else
    BOOST_TEST_MESSAGE(
        "No std::shared_ptr support; test will automatically pass.");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_multi_base_4,
                        shared_ptr_multi_base_fixture)
{
#if BOOST_VERSION >= 105600
    std::shared_ptr<SubMulti> ptr(new SubMulti(10));
    std::weak_ptr<Base3>      weak(ptr);
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
#else
    BOOST_TEST_MESSAGE(
        "No std::shared_ptr support; test will automatically pass.");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_multi_base_5,
                        shared_ptr_multi_base_fixture)
{
#if BOOST_VERSION >= 105600
    std::shared_ptr<SubMulti> ptr_tmp(new SubMulti(10));
    std::shared_ptr<Base1>    ptr(ptr_tmp);
    std::weak_ptr<Base2>      weak(ptr_tmp);
    ptr_tmp.reset();
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
#else
    BOOST_TEST_MESSAGE(
        "No std::shared_ptr support; test will automatically pass.");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_multi_base_6,
                        shared_ptr_multi_base_fixture)
{
#if BOOST_VERSION >= 105600
    std::shared_ptr<SubMulti> ptr_tmp(new SubMulti(10));
    std::shared_ptr<Base2>    ptr(ptr_tmp);
    std::weak_ptr<Base3>      weak(ptr_tmp);
    ptr_tmp.reset();
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
#else
    BOOST_TEST_MESSAGE(
        "No std::shared_ptr support; test will automatically pass.");
#endif
}

BOOST_FIXTURE_TEST_CASE(std_shared_ptr_multi_base_7,
                        shared_ptr_multi_base_fixture)
{
#if BOOST_VERSION >= 105600
    std::shared_ptr<SubMulti> ptr_tmp(new SubMulti(10));
    std::shared_ptr<Base3>    ptr(ptr_tmp);
    std::weak_ptr<Base1>      weak(ptr_tmp);
    ptr_tmp.reset();
    do_test1(ptr, weak);
    do_test2(ptr, weak);
    ptr.reset();
    BOOST_CHECK(0 == SubMulti::count);
#else
    BOOST_TEST_MESSAGE(
        "No std::shared_ptr support; test will automatically pass.");
#endif
}
