/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// yaml_archive_exception.cpp:

// (C) Copyright 2009 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#if (defined _MSC_VER) && (_MSC_VER == 1200)
#pragma warning(disable : 4786) // too long name, harmless warning
#endif

#include <exception>
#include <string>

#include <boost/assert.hpp>

#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/yaml_archive_exception.hpp>
#include <boost/serialization/config.hpp>

namespace boost {
namespace archive {

BOOST_ARCHIVE_DECL
yaml_archive_exception::yaml_archive_exception(exception_code c, const char* e1,
                                               const char* e2)
    : archive_exception(other_exception, e1, e2)
{
    switch (c)
    {
    case yaml_archive_parsing_error:
        archive_exception::append(0, "unrecognized YAML syntax");
        break;
    case yaml_archive_tag_mismatch:
    {
        unsigned int l;
        l = archive_exception::append(0, "YAML start/end tag mismatch");
        if (NULL != e1)
        {
            l = archive_exception::append(l, " - ");
            archive_exception::append(l, e1);
        }
        break;
    }
    case yaml_archive_tag_name_error:
        archive_exception::append(0, "Invalid YAML tag name");
        break;
    default:
        BOOST_ASSERT(false);
        archive_exception::append(0, "programming error");
        break;
    }
}

BOOST_ARCHIVE_DECL
yaml_archive_exception::yaml_archive_exception(
    yaml_archive_exception const& oth)
    : archive_exception(oth)
{
}

BOOST_ARCHIVE_DECL
yaml_archive_exception::~yaml_archive_exception() BOOST_NOEXCEPT_OR_NOTHROW {}

} // archive
} // boost
