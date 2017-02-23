#define BOOST_TEST_MODULE boost archive
#include <boost/core/no_exceptions_support.hpp>
#include <boost/math/special_functions/next.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/test/unit_test.hpp>

#include "A.hpp"
#include "A.ipp"
#include "B.hpp"
#include "basics_fixture.hpp"

namespace utf = boost::unit_test;

using boost::serialization::make_nvp;
#define NVP(var) BOOST_SERIALIZATION_NVP(var)

#define TEST(name) BOOST_FIXTURE_TEST_CASE(name, basics_fixture)

bool A::check_equal(const A& rhs) const
{
    BOOST_CHECK_EQUAL(b, rhs.b);
    BOOST_CHECK_EQUAL(l, rhs.l);
#ifndef BOOST_NO_INT64_T
    BOOST_CHECK_EQUAL(f, rhs.f);
    BOOST_CHECK_EQUAL(g, rhs.g);
#endif
    BOOST_CHECK_EQUAL(m, rhs.m);
    BOOST_CHECK_EQUAL(n, rhs.n);
    BOOST_CHECK_EQUAL(o, rhs.o);
    BOOST_CHECK_EQUAL(p, rhs.p);
    BOOST_CHECK_EQUAL(q, rhs.q);
#ifndef BOOST_NO_CWCHAR
    BOOST_CHECK_EQUAL(r, rhs.r);
#endif
    BOOST_CHECK_EQUAL(c, rhs.c);
    BOOST_CHECK_EQUAL(s, rhs.s);
    BOOST_CHECK_EQUAL(t, rhs.t);
    BOOST_CHECK_EQUAL(u, rhs.u);
    BOOST_CHECK_EQUAL(v, rhs.v);
    BOOST_CHECK_EQUAL(l, rhs.l);
    BOOST_CHECK(std::abs(boost::math::float_distance(w, rhs.w)) < 4);
    BOOST_CHECK(std::abs(boost::math::float_distance(x, rhs.x)) < 4);
    BOOST_CHECK(!(0 != y.compare(rhs.y)));
#ifndef BOOST_NO_STD_WSTRING
    BOOST_CHECK(!(0 != z.compare(rhs.z)));
#endif
    return true;
}

TEST(simple_class)
{
    A a, a1;
    {
        output() << NVP(a);
    }
    {
        input() >> make_nvp("a", a1);
    }
    a.check_equal(a1);
}

/**
 * @brief      Class with another class as a member variable
 */
class contained
{
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(b);
    }
    B b;

  public:
    bool operator==(const contained& rhs) const { return b == rhs.b; }
    friend std::ostream& operator<<(std::ostream&, const contained&);
    contained() {}
};

std::ostream& operator<<(std::ostream& os, const contained& c)
{
    return os << c.b;
}

TEST(contained_class)
{
    const contained c;
    check_roundtrip(c);
}

/**
 * @brief      Class serialized without a version number
 */
class without_version_number_
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /*ar*/, const unsigned int file_version)
    {
        // class that don't save class info always have a version number of 0
        BOOST_CHECK_EQUAL(file_version, 0);
        BOOST_STATIC_ASSERT(
            0 ==
            ::boost::serialization::version<without_version_number_>::value);
        ++count;
    }

  public:
    unsigned int count;
    without_version_number_() : count(0) {}
};

BOOST_CLASS_IMPLEMENTATION(without_version_number_,
                           ::boost::serialization::object_serializable)
BOOST_CLASS_TRACKING(without_version_number_,
                     ::boost::serialization::track_never)

TEST(without_version_number)
{
    without_version_number_ a;
    {
        output() << NVP(a) << NVP(a);
    }
    {
        input() >> NVP(a) >> NVP(a);
    }
    BOOST_CHECK_EQUAL(a.count, 4); // no tracking => redundant loads
}

/**
 * @brief      Class serialized with a version number
 */
class with_version_number_
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /*ar*/, const unsigned int file_version)
    {
        // verify at execution that the version number corresponds to the saved
        // one
        BOOST_CHECK_EQUAL(file_version, 4);
        ++count;
    }

  public:
    unsigned int count;
    with_version_number_() : count(0) {}
};

BOOST_CLASS_IMPLEMENTATION(with_version_number_,
                           ::boost::serialization::object_class_info)

BOOST_CLASS_VERSION(with_version_number_, 4)

TEST(with_version_number)
{
    with_version_number_  a;
    with_version_number_* ptr = &a;
    {
        output() << NVP(ptr) << NVP(ptr);
    }
    {
        input() >> NVP(ptr) >> NVP(ptr);
    }
    BOOST_CHECK_EQUAL(a.count, 1);
}
