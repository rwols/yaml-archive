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
    {
        output() << NVP(an) << NVP(an) << NVP(as) << NVP(as) << NVP(aa)
                 << NVP(aa);
    }
    BOOST_CHECK(an.count == 2);
    BOOST_CHECK(as.count == 2);
    BOOST_CHECK(aa.count == 1);
    AN::count = 0;
    AS::count = 0;
    AA::count = 0;
    {
        input() >> NVP(an) >> NVP(an) >> NVP(as) >> NVP(as) >> NVP(aa) >>
            NVP(aa);
    }
    BOOST_CHECK(an.count == 2);
    BOOST_CHECK(as.count == 2);
    BOOST_CHECK(aa.count == 1);
}

BOOST_FIXTURE_TEST_CASE(tracking2, io_fixture)
{
    PAN* pan = new PAN;
    PAS* pas = new PAS;
    PAA* paa = new PAA;
    {
        output() << NVP(pan) << NVP(pan) << NVP(pas) << NVP(pas) << NVP(paa)
                 << NVP(paa);
    }
    BOOST_CHECK(pan->count == 2);
    BOOST_CHECK(pas->count == 1);
    BOOST_CHECK(paa->count == 1);
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
        input() >> NVP(pan) >> NVP(pan) >> NVP(pas) >> NVP(pas) >> NVP(paa) >>
            NVP(paa);
    }
    BOOST_CHECK(pan->count == 2);
    BOOST_CHECK(pas->count == 1);
    BOOST_CHECK(paa->count == 1);
}
