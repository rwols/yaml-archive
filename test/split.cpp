/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_split.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "io_fixture.hpp"
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/test/unit_test.hpp>

class A1
{
    friend class boost::serialization::access;
    template <class Archive>
    void save(Archive& /* ar */, const unsigned int /* file_version */
              ) const
    {
        ++(const_cast<A1&>(*this).count);
    }
    template <class Archive>
    void load(Archive& /* ar */, const unsigned int /* file_version */
              )
    {
        --count;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    int count;

  public:
    A1() : count(0) {}
    ~A1() { BOOST_CHECK(0 == count); }
};

class B1
{
    friend class boost::serialization::access;
    template <class Archive>
    void save(Archive& /* ar */, const unsigned int /* file_version */
              ) const
    {
        ++(const_cast<B1&>(*this).count);
    }
    template <class Archive>
    void load(Archive& /* ar */, const unsigned int /* file_version */
              )
    {
        --count;
    }
    int count;

  public:
    B1() : count(0) {}
    ~B1() { BOOST_CHECK(0 == count); }
};

// function specializations must be defined in the appropriate
// namespace - boost::serialization
namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, B1& b, const unsigned int file_version)
{
    boost::serialization::split_member(ar, b, file_version);
}

} // serialization
} // namespace boost

class C1
{
  public:
    int count;
    C1() : count(0) {}
    ~C1() { BOOST_CHECK(0 == count); }
};

namespace boost {
namespace serialization {

template <class Archive>
void save(Archive& /* ar */, const C1& c, const unsigned int /* file_version */
          )
{
    ++(const_cast<C1&>(c).count);
}

template <class Archive>
void load(Archive& /* ar */, C1& c, const unsigned int /* file_version */
          )
{
    --c.count;
}

} // serialization
} // namespace boost

BOOST_SERIALIZATION_SPLIT_FREE(C1);

BOOST_FIXTURE_TEST_CASE(split, io_fixture)
{
    A1 a;
    B1 b;
    C1 c;
    {
        output() << BOOST_SERIALIZATION_NVP(a) << BOOST_SERIALIZATION_NVP(b)
                 << BOOST_SERIALIZATION_NVP(c);
    }
    {
        input() >> BOOST_SERIALIZATION_NVP(a) >> BOOST_SERIALIZATION_NVP(b) >>
            BOOST_SERIALIZATION_NVP(c);
    }
}

// EOF
