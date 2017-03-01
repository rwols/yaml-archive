/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_variant.cpp
// test of non-intrusive serialization of variant types
//
// copyright (c) 2005
// troy d. straszheim <troy@resophonic.com>
// http://www.resophonic.com
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
//
// thanks to Robert Ramey and Peter Dimov.
//

#include <boost/config.hpp>
#include <boost/math/special_functions/next.hpp>
#include <cstddef> // NULL
#include <cstdio>  // remove
#include <fstream>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::remove;
}
#endif

#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/serialization/throw_exception.hpp>
#include <boost/type_traits/is_same.hpp>

#if defined(_MSC_VER) && (_MSC_VER <= 1020)
#pragma warning(disable : 4786) // too long name, harmless warning
#endif

#include "A.hpp"
#include "A.ipp"
#include "io_fixture.hpp"
#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/test/unit_test.hpp>

class are_equal : public boost::static_visitor<bool>
{
  public:
    // note extra rigamorole for compilers which don't support
    // partial function template ordering - specfically msvc 6.x
    struct same
    {
        template <class T, class U> static bool invoke(const T& t, const U& u)
        {
            return t == u;
        }
    };

    struct not_same
    {
        template <class T, class U> static bool invoke(const T&, const U&)
        {
            return false;
        }
    };

    template <class T, class U> bool operator()(const T& t, const U& u) const
    {
        typedef typename boost::mpl::eval_if<
            boost::is_same<T, U>, boost::mpl::identity<same>,
            boost::mpl::identity<not_same>>::type type;
        return type::invoke(t, u);
    }

    bool operator()(const float& lhs, const float& rhs) const
    {
        // FIXME!!!
        return std::abs(boost::math::float_distance(lhs, rhs)) < 8;
    }
    bool operator()(const double& lhs, const double& rhs) const
    {
        // FIXME!!!
        return std::abs(boost::math::float_distance(lhs, rhs)) < 8;
    }
};

BOOST_FIXTURE_TEST_CASE(boost_variant_1, io_fixture)
{
    boost::variant<bool, int, float, double, A, std::string> v, v1;
    v = false;
    {
        output() << BOOST_SERIALIZATION_NVP(v);
    }
    {
        input() >> boost::serialization::make_nvp("v", v1);
    }
    BOOST_CHECK(boost::apply_visitor(are_equal(), v, v1));
}

BOOST_FIXTURE_TEST_CASE(boost_variant_2, io_fixture)
{
    boost::variant<bool, int, float, double, A, std::string> v, v1;
    v = 1;
    {
        output() << BOOST_SERIALIZATION_NVP(v);
    }
    {
        input() >> boost::serialization::make_nvp("v", v1);
    }
    BOOST_CHECK(boost::apply_visitor(are_equal(), v, v1));
}

BOOST_FIXTURE_TEST_CASE(boost_variant_3, io_fixture)
{
    boost::variant<bool, int, float, double, A, std::string> v, v1;
    v = 2.3f;
    {
        output() << BOOST_SERIALIZATION_NVP(v);
    }
    {
        input() >> boost::serialization::make_nvp("v", v1);
    }
    BOOST_CHECK(boost::apply_visitor(are_equal(), v, v1));
}

BOOST_FIXTURE_TEST_CASE(boost_variant_4, io_fixture)
{
    boost::variant<bool, int, float, double, A, std::string> v, v1;
    v = 6.4;
    {
        output() << BOOST_SERIALIZATION_NVP(v);
    }
    {
        input() >> boost::serialization::make_nvp("v", v1);
    }
    BOOST_CHECK(boost::apply_visitor(are_equal(), v, v1));
}

BOOST_FIXTURE_TEST_CASE(boost_variant_5, io_fixture)
{
    boost::variant<bool, int, float, double, A, std::string> v, v1;
    v = std::string("we can't stop here, this is Bat Country");
    {
        output() << BOOST_SERIALIZATION_NVP(v);
    }
    {
        input() >> boost::serialization::make_nvp("v", v1);
    }
    BOOST_CHECK(boost::apply_visitor(are_equal(), v, v1));
}

BOOST_FIXTURE_TEST_CASE(boost_variant_6, io_fixture)
{
    boost::variant<bool, int, float, double, A, std::string> v, v1;
    v = A();
    {
        output() << BOOST_SERIALIZATION_NVP(v);
    }
    {
        input() >> boost::serialization::make_nvp("v", v1);
    }
    BOOST_CHECK(boost::apply_visitor(are_equal(), v, v1));
}

BOOST_FIXTURE_TEST_CASE(boost_variant_7, io_fixture)
{
// Compiling this test invokes an ICE on msvc 6
// So, we'll just to skip it for this compiler
#if defined(_MSC_VER) && (_MSC_VER <= 1020)
    bool exception_invoked = false;
    boost::variant<bool, float, int, std::string> v;
    boost::variant<bool, float, int> v1; // note we don't have std::string
    v = std::string("adrenochrome");
    {
        output() << BOOST_SERIALIZATION_NVP(v);
    }
    BOOST_TRY { input() >> boost::serialization::make_nvp("v", v1); }
    BOOST_CATCH(boost::archive::archive_exception e)
    {
        BOOST_CHECK(boost::archive::archive_exception::unsupported_version ==
                    e.code);
        exception_invoked = true;
    }
    BOOST_CHECK(exception_invoked);
#endif
}

// EOF
