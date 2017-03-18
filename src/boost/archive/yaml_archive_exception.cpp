/** @file
 *
 * @brief Defines an exception class for yaml archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#if (defined _MSC_VER) && (_MSC_VER == 1200)
#pragma warning(disable : 4786) // too long name, harmless warning
#endif

#include <exception>
#include <string>

#include <boost/assert.hpp>

#define BOOST_ARCHIVE_SOURCE
#define BOOST_WARCHIVE_SOURCE

#include <boost/archive/yaml_archive_exception.hpp>
#include <boost/serialization/config.hpp>

namespace boost {
namespace archive {

YAML_AWARCHIVE_API
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

YAML_AWARCHIVE_API
yaml_archive_exception::yaml_archive_exception(
    yaml_archive_exception const& oth)
    : archive_exception(oth)
{
}

YAML_AWARCHIVE_API
yaml_archive_exception::~yaml_archive_exception() BOOST_NOEXCEPT_OR_NOTHROW {}

} // archive
} // boost
