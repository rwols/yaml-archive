/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_class_info_load.cpp: test implementation level trait

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// test implementation level "object_class_info"
// should pass compilation and execution

#include <fstream>
#include <string>

#include "test_tools.hpp"
#include <boost/archive/tmpdir.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <boost/serialization/level.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/version.hpp>
#include <boost/static_assert.hpp>

class without_version_number
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /*ar*/, const unsigned int file_version)
    {
        // class that don't save class info always have a version number of 0
        BOOST_CHECK(file_version == 0);
        BOOST_STATIC_ASSERT(
            0 ==
            ::boost::serialization::version<without_version_number>::value);
        ++count;
    }

  public:
    unsigned int count;
    without_version_number() : count(0) {}
};

BOOST_CLASS_IMPLEMENTATION(without_version_number,
                           ::boost::serialization::object_serializable)
BOOST_CLASS_TRACKING(without_version_number,
                     ::boost::serialization::track_never)

// second case : serialize WITH class information
class with_version_number
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /*ar*/, const unsigned int file_version)
    {
        // verify at execution that the version number corresponds to the saved
        // one
        BOOST_CHECK(file_version == 4);
        ++count;
    }

  public:
    unsigned int count;
    with_version_number() : count(0) {}
};

BOOST_CLASS_IMPLEMENTATION(with_version_number,
                           ::boost::serialization::object_class_info)
BOOST_CLASS_TRACKING(with_version_number, ::boost::serialization::track_never)
BOOST_CLASS_VERSION(with_version_number, 4)

void in(const char* testfile, A& a, B& b)
{
    test_istream  is(testfile, TEST_STREAM_FLAGS);
    test_iarchive ia(is, TEST_ARCHIVE_FLAGS);
    ia >> BOOST_SERIALIZATION_NVP(a);
    ia >> BOOST_SERIALIZATION_NVP(a);
    BOOST_CHECK(a.count == 2); // no tracking => redundant loads
    ia >> BOOST_SERIALIZATION_NVP(b);
    ia >> BOOST_SERIALIZATION_NVP(b);
    // note: archive was saved with tracking so that is what determines
    // whether tracking is perform on load - regardless of the latest
    // tracking setting.
    BOOST_CHECK(b.count == 1);
}

int test_main(int /* argc */, char* /* argv */ [])
{
    A           a;
    B           b;
    std::string filename;
    // filename += boost::archive::tmpdir();
    // filename += '/';
    filename += BOOST_PP_STRINGIZE(testfile_);
    filename += BOOST_PP_STRINGIZE(BOOST_ARCHIVE_TEST);
    in(filename.c_str(), a, b);
    return EXIT_SUCCESS;
}

// EOF
