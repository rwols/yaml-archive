/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_array.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "io_fixture.hpp" // includes boost/version.hpp
#include <boost/archive/archive_exception.hpp>
#if BOOST_VERSION < 1005600
#include <boost/detail/no_exceptions_support.hpp>
#else
#include <boost/core/no_exceptions_support.hpp>
#endif
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/test/unit_test.hpp>

struct test_dummy_out
{
    template <class Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
        throw boost::archive::archive_exception(
            boost::archive::archive_exception::other_exception);
    }
    template <class Archive> void load(Archive& ar, const unsigned int version)
    {
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    test_dummy_out() {}
};

struct test_dummy_in
{
    template <class Archive>
    void save(Archive& ar, const unsigned int /*version*/) const
    {
    }
    template <class Archive>
    void load(Archive& ar, const unsigned int /*version*/)
    {
        throw boost::archive::archive_exception(
            boost::archive::archive_exception::other_exception);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    test_dummy_in() {}
};

BOOST_FIXTURE_TEST_CASE(interrupts1, io_fixture)
{
    const test_dummy_out t;
    {
        test_ostream os(get_filename().c_str(), STREAM_FLAGS);
        {
            BOOST_TRY
            {
                test_oarchive oa(os, ARCHIVE_FLAGS);
                bool          exception_invoked = false;
                BOOST_TRY { oa << BOOST_SERIALIZATION_NVP(t); }
                BOOST_CATCH(boost::archive::archive_exception ae)
                {
                    BOOST_CHECK(
                        boost::archive::archive_exception::other_exception ==
                        ae.code);
                    exception_invoked = true;
                }
                BOOST_CATCH_END
                BOOST_CHECK(exception_invoked);
            }
            BOOST_CATCH(boost::archive::archive_exception ae) {}
            BOOST_CATCH_END
        }
    }
}

BOOST_FIXTURE_TEST_CASE(interrupts2, io_fixture)
{
    const test_dummy_in t;
    {
        test_ostream os(get_filename().c_str(), STREAM_FLAGS);
        {
            test_oarchive oa(os, ARCHIVE_FLAGS);
            oa << BOOST_SERIALIZATION_NVP(t);
        }
        os.close();
    }
    {
        test_istream is(get_filename().c_str(), STREAM_FLAGS);
        {
            test_dummy_in t1;
            BOOST_TRY
            {
                test_iarchive ia(is, ARCHIVE_FLAGS);
                bool          exception_invoked = false;
                BOOST_TRY { ia >> boost::serialization::make_nvp("t", t1); }
                BOOST_CATCH(boost::archive::archive_exception ae)
                {
                    BOOST_CHECK(
                        boost::archive::archive_exception::other_exception ==
                        ae.code);
                    exception_invoked = true;
                }
                BOOST_CATCH_END
                BOOST_CHECK(exception_invoked);
            }
            BOOST_CATCH(boost::archive::archive_exception ae) {}
            BOOST_CATCH_END
        }
    }
}

// EOF
