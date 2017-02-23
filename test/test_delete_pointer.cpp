/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_delete_pointer.cpp

// (C) Copyright 2002 Vahan Margaryan.
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/core/no_exceptions_support.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/throw_exception.hpp>
#include <boost/test/unit_test.hpp>

// A holds a pointer to another A, but doesn't own the pointer.
// objCount
class non_owning
{
    friend class boost::serialization::access;
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        ar << BOOST_SERIALIZATION_NVP(next_);
    }
    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        ar >> BOOST_SERIALIZATION_NVP(next_);
        ++loadcount;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  public:
    non_owning()
    {
        if (test && objcount == 3)
            boost::serialization::throw_exception(
                boost::archive::archive_exception(
                    boost::archive::archive_exception::no_exception));
        next_ = 0;
        ++objcount;
    }
    ~non_owning()
    {
        delete next_;
        --objcount;
    }
    non_owning* next_;
    static int  objcount;
    static bool test;
    static int  loadcount;
};

int  non_owning::objcount = 0;
int  non_owning::loadcount = 0;
bool non_owning::test = false;

BOOST_FIXTURE_TEST_CASE(delete_pointer, io_fixture)
{

    // fill the vector with chained A's. The vector is assumed
    // to own the objects - we will destroy the objects through this vector.

    non_owning*  head = new non_owning;
    non_owning*  last = head;
    unsigned int i;
    for (i = 1; i < 9; ++i)
    {
        non_owning* a = new non_owning;
        last->next_ = a;
        last = a;
    }
    {
        output() << BOOST_SERIALIZATION_NVP(head);
    }
    delete head;
    BOOST_CHECK(non_owning::objcount == 0);

    head = nullptr;
    non_owning::test = true;
    test_istream  is(get_filename().c_str(), TEST_STREAM_FLAGS);
    test_iarchive ia(is, TEST_ARCHIVE_FLAGS);

    try
    {
        ia >> BOOST_SERIALIZATION_NVP(head);
    }
    catch (...)
    {
        ia.delete_created_pointers();
    }

    // identify the leaks
    BOOST_CHECK(non_owning::loadcount == 0);
}
