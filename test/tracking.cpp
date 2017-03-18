/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_tracking_save.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "io_fixture.hpp"
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/test/unit_test.hpp>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4308)
#endif

#define TEST_CLASS(N, TRACKING)                                                \
    class N                                                                    \
    {                                                                          \
        friend class boost::serialization::access;                             \
        template <class Archive>                                               \
        void serialize(Archive& /* ar */,                                      \
                       const unsigned int /* file_version */)                  \
        {                                                                      \
            ++count;                                                           \
        }                                                                      \
                                                                               \
      public:                                                                  \
        static unsigned int count;                                             \
    };                                                                         \
    unsigned int N::count = 0;                                                 \
    BOOST_CLASS_TRACKING(N, TRACKING)

TEST_CLASS(AN, ::boost::serialization::track_never)
TEST_CLASS(AS, ::boost::serialization::track_selectively)
TEST_CLASS(AA, ::boost::serialization::track_always)

// test pointers
TEST_CLASS(PAN, ::boost::serialization::track_never)
TEST_CLASS(PAS, ::boost::serialization::track_selectively)
TEST_CLASS(PAA, ::boost::serialization::track_always)

#define NVP(name) BOOST_SERIALIZATION_NVP(name)

BOOST_FIXTURE_TEST_CASE(tracking1, io_fixture)
{
    // write object twice to check tracking
    AN an;
    AS as;
    AA aa;
    using boost::serialization::make_nvp;
    {
        output() << make_nvp("an1", an) << make_nvp("an2", an)
                 << make_nvp("as1", as) << make_nvp("as2", as)
                 << make_nvp("aa1", aa) << make_nvp("aa2", aa);
    }
    BOOST_CHECK_EQUAL(an.count, 2);
    BOOST_CHECK_EQUAL(as.count, 2);
    BOOST_CHECK_EQUAL(aa.count, 1);
    AN::count = 0;
    AS::count = 0;
    AA::count = 0;
    {
        input() >> make_nvp("an1", an) >> make_nvp("an2", an) >>
            make_nvp("as1", as) >> make_nvp("as2", as) >> make_nvp("aa1", aa) >>
            make_nvp("aa2", aa);
    }
    BOOST_CHECK_EQUAL(an.count, 2);
    BOOST_CHECK_EQUAL(as.count, 2);
    BOOST_CHECK_EQUAL(aa.count, 1);
}

BOOST_FIXTURE_TEST_CASE(tracking2, io_fixture)
{
    PAN* pan = new PAN;
    PAS* pas = new PAS;
    PAA* paa = new PAA;
    using boost::serialization::make_nvp;
    {
        output() << make_nvp("pan1", pan) << make_nvp("pan2", pan)
                 << make_nvp("pas1", pas) << make_nvp("pas2", pas)
                 << make_nvp("paa1", paa) << make_nvp("paa2", paa);
    }
    BOOST_CHECK_EQUAL(pan->count, 2);
    BOOST_CHECK_EQUAL(pas->count, 1);
    BOOST_CHECK_EQUAL(paa->count, 1);
    delete pan;
    delete pas;
    delete paa;
    pan = nullptr;
    pas = nullptr;
    paa = nullptr;
    PAN::count = 0;
    PAS::count = 0;
    PAA::count = 0;
    {
        input() >> make_nvp("pan1", pan) >> make_nvp("pan2", pan) >>
            make_nvp("pas1", pas) >> make_nvp("pas2", pas) >>
            make_nvp("paa1", paa) >> make_nvp("paa2", paa);
    }
    BOOST_CHECK_EQUAL(pan->count, 2);
    BOOST_CHECK_EQUAL(pas->count, 1);
    BOOST_CHECK_EQUAL(paa->count, 1);
}

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif
