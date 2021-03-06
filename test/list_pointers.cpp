/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_list.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include <boost/config.hpp>

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp" // includes boost/version.hpp
#include <boost/archive/archive_exception.hpp>
#include <boost/checked_delete.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/static_assert.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/type_traits/is_pointer.hpp>
#if BOOST_VERSION > 105500
#include <boost/serialization/forward_list.hpp>
#endif

using boost::serialization::make_nvp;

template <class T> struct ptr_equal_to : public std::binary_function<T, T, bool>
{
    BOOST_STATIC_ASSERT(::boost::is_pointer<T>::value);
    bool operator()(T const _Left, T const _Right) const
    {
        if (nullptr == _Left && nullptr == _Right) return true;
        if (typeid(*_Left) != typeid(*_Right)) return false;
        return *_Left == *_Right;
    }
};

BOOST_FIXTURE_TEST_CASE(std_list_pointers, io_fixture)
{
    std::list<A*> alist;
    A*            free_a_ptr = new A;
    alist.push_back(free_a_ptr);
    alist.push_back(new A);
    // verify that first element is the same as the free pointer
    BOOST_CHECK((*alist.begin()) == free_a_ptr);
    {
        output() << make_nvp("alist", alist)
                 << make_nvp("free_a_ptr", free_a_ptr);
    }
    std::list<A*> alist1;
    A*            free_a_ptr1;
    {
        input() >> make_nvp("alist", alist1) >>
            make_nvp("free_a_ptr", free_a_ptr1);
    }

    BOOST_CHECK(alist.size() == alist1.size() &&
                std::equal(alist.begin(), alist.end(), alist1.begin(),
                           ptr_equal_to<A*>()));
    // verify that first element is the same as the free pointer
    BOOST_CHECK((*alist1.begin()) == free_a_ptr1);

    std::for_each(alist.begin(), alist.end(), boost::checked_deleter<A>());
    std::for_each(alist1.begin(), alist1.end(), boost::checked_deleter<A>());
}

BOOST_FIXTURE_TEST_CASE(std_forward_list_ptrs, io_fixture)
{
#if BOOST_VERSION > 105500
    std::forward_list<A*> aslist;
    aslist.push_front(new A);
    aslist.push_front(new A);
    {
        output() << make_nvp("aslist", aslist);
    }
    std::forward_list<A*> aslist1;
    {
        input() >> make_nvp("aslist", aslist1);
    }
    BOOST_CHECK(std::equal(aslist.begin(), aslist.end(), aslist1.begin(),
                           ptr_equal_to<A*>()));
    std::for_each(aslist.begin(), aslist.end(), boost::checked_deleter<A>());
    std::for_each(aslist1.begin(), aslist1.end(), boost::checked_deleter<A>());
#else
    BOOST_TEST_MESSAGE("serialization of std::forward_list requires at least "
                       "boost version 1.56!");
#endif
}

// EOF
