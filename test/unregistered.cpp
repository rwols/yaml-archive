/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_unregistered.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "io_fixture.hpp"
#include <boost/archive/archive_exception.hpp>
#if BOOST_VERSION < 1005600
#include <boost/detail/no_exceptions_support.hpp>
#else
#include <boost/core/no_exceptions_support.hpp>
#endif
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/test/unit_test.hpp>

class unreg_base
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /* ar */, const unsigned int /* file_version */)
    {
    }

  public:
    virtual ~unreg_base(){};
};

class unreg_derived1 : public unreg_base
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(unreg_base);
    }

  public:
};

class unreg_derived2 : public unreg_base
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */);
};

BOOST_FIXTURE_TEST_CASE(unregistered1, io_fixture)
{

    unreg_base* rb1 = new unreg_derived1;

    // registration IS necessary when serializing a polymorphic class
    // through pointer to the base class
    bool except = false;
    BOOST_TRY
    {
        output() << BOOST_SERIALIZATION_NVP(rb1);
        // test_ostream  os(get_filename().c_str(), STREAM_FLAGS);
        // test_oarchive oa(os, ARCHIVE_FLAGS);
        // oa << BOOST_SERIALIZATION_NVP(rb1);
    }
    BOOST_CATCH(boost::archive::archive_exception aex) { except = true; }
    BOOST_CATCH_END
    BOOST_CHECK_MESSAGE(except, "lack of registration not detected !");

    delete rb1;

// note: the corresponding save function above will not result in
// valid archive - hence, the following code which attempts to load
// and archive will fail.  Leave this as a reminder not to do this
#if 0
    test_istream  is(get_filename().c_str(), STREAM_FLAGS);
    test_iarchive ia(is, ARCHIVE_FLAGS);
    rb1 = nullptr;

    // registration IS necessary when serializing a polymorphic class
    // through pointer to the base class
    bool except = false;
    BOOST_TRY {
        ia >> BOOST_SERIALIZATION_NVP(rb1);
    }
    BOOST_CATCH(boost::archive::archive_exception aex){
        except = true;
        BOOST_CHECK_MESSAGE(
            NULL == rb1, 
            "failed load resulted in a non-null pointer"
        );
    }
    BOOST_CATCH_END
    BOOST_CHECK_MESSAGE(except, "lack of registration not detected !");

    delete rb1;
#endif
}

BOOST_FIXTURE_TEST_CASE(unregistered2, io_fixture)
{
    // save unregistered polymorphic classes

    unreg_derived1* rd1 = new unreg_derived1;

    // registration is NOT necessary when serializing a polymorphic class
    // through pointer to a derived class
    bool except = false;
    BOOST_TRY
    {
        output() << BOOST_SERIALIZATION_NVP(rd1);
        // test_ostream  os(get_filename().c_str(), STREAM_FLAGS);
        // test_oarchive oa(os, ARCHIVE_FLAGS);
        // oa << BOOST_SERIALIZATION_NVP(rd1);
    }
    BOOST_CATCH(boost::archive::archive_exception aex) { except = true; }
    BOOST_CATCH_END
    BOOST_CHECK_MESSAGE(!except, "registration not detected !");

    delete rd1;

    // note: the corresponding save function above will not result in
    // valid archive - hence, the following code which attempts to load
    // and archive will fail.  Leave this as a reminder not to do this
    // load unregistered polymorphic classes
    rd1 = nullptr;

    // registration is NOT necessary when serializing a polymorphic class
    // or through pointer to a derived class
    except = false;
    BOOST_TRY
    {
        input() >> BOOST_SERIALIZATION_NVP(rd1);
        // test_istream  is(get_filename().c_str(), STREAM_FLAGS);
        // test_iarchive ia(is, ARCHIVE_FLAGS);
        // ia >> BOOST_SERIALIZATION_NVP(rd1);
    }
    BOOST_CATCH(boost::archive::archive_exception aex)
    {
        except = true;
        BOOST_CHECK_MESSAGE(nullptr == rd1,
                            "failed load resulted in a non-null pointer");
    }
    BOOST_CATCH_END
    BOOST_CHECK_MESSAGE(!except, "registration not detected !");

    delete rd1;
}

// EOF
