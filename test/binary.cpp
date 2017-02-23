/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// binary.cpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

#include "io_fixture.hpp"
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>
#include <cstdlib> // for rand(), NULL, size_t

class binary_class
{
    friend class boost::serialization::access;
    friend std::ostream& operator<<(std::ostream&, const binary_class&);
    char                 data[150];
    // note: from an aesthetic perspective, I would much prefer to have this
    // defined out of line.  Unfortunately, this trips a bug in the VC 6.0
    // compiler. So hold our nose and put it her to permit running of tests.
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& boost::serialization::make_nvp(
            "data",
            boost::serialization::make_binary_object(data, sizeof(data)));
    }

  public:
    binary_class();
    bool operator==(const binary_class& rhs) const;
};

binary_class::binary_class()
{
    int i = sizeof(data);
    while (i-- > 0) data[i] = static_cast<char>(0xff & std::rand());
}

bool binary_class::operator==(const binary_class& rhs) const
{
    int i = sizeof(data);
    while (i-- > 0)
        if (data[i] != rhs.data[i]) return false;
    return true;
}

std::ostream& operator<<(std::ostream& os, const binary_class& b)
{
    os << "[ ";
    for (const auto c : b.data) os << c << ' ';
    return os << ']';
}

BOOST_FIXTURE_TEST_CASE(binary, io_fixture)
{
    const binary_class a;
    binary_class       a1;
    char               s1[] = "a";
    char               s2[] = "ab";
    char               s3[] = "abc";
    char               s4[] = "abcd";
    const int          i = 12345;
    char               s1_1[10];
    char               s1_2[10];
    char               s1_3[10];
    char               s1_4[10];
    int                i1 = 34790;

    std::memset(s1_1, '\0', sizeof(s1_1));
    std::memset(s1_2, '\0', sizeof(s1_2));
    std::memset(s1_3, '\0', sizeof(s1_3));
    std::memset(s1_4, '\0', sizeof(s1_4));

    using boost::serialization::make_binary_object;
    using boost::serialization::make_nvp;

    {
        output() << make_nvp("s1", make_binary_object(s1, sizeof(s1)))
                 << make_nvp("s2", make_binary_object(s2, sizeof(s2)))
                 << make_nvp("s3", make_binary_object(s3, sizeof(s3)))
                 << make_nvp("s4", make_binary_object(s4, sizeof(s4)))
                 << BOOST_SERIALIZATION_NVP(a)
                 // note: add a little bit on the end of the archive to detect
                 // failure of text mode binary.
                 << BOOST_SERIALIZATION_NVP(i);
    }
    {
        input() >> make_nvp("s1", make_binary_object(s1_1, sizeof(s1))) >>
            make_nvp("s2", make_binary_object(s1_2, sizeof(s2))) >>
            make_nvp("s3", make_binary_object(s1_3, sizeof(s3))) >>
            make_nvp("s4", make_binary_object(s1_4, sizeof(s4))) >>
            make_nvp("a", a1)
            // note: add a little bit on the end of the archive to detect
            // failure of text mode binary.
            >> make_nvp("i", i1);
    }

    BOOST_CHECK_EQUAL(0, std::strcmp(s1, s1_1));
    BOOST_CHECK_EQUAL(0, std::strcmp(s2, s1_2));
    BOOST_CHECK_EQUAL(0, std::strcmp(s3, s1_3));
    BOOST_CHECK_EQUAL(0, std::strcmp(s4, s1_4));
    BOOST_CHECK_EQUAL(a, a1);
    BOOST_CHECK_EQUAL(i, i1);
}
