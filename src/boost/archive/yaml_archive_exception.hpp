/** @file
 *
 * @brief Declares an exception class for yaml archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_YAML_ARCHIVE_EXCEPTION_HPP
#define BOOST_ARCHIVE_YAML_ARCHIVE_EXCEPTION_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/archive/archive_exception.hpp>
#include <boost/archive/detail/yaml_decl.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <exception>

#if BOOST_VERSION <= 105800
#include <boost/serialization/pfto.hpp>
#endif

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost {
namespace archive {

/**
 * @brief      Exception thrown by yaml archives.
 */
class BOOST_SYMBOL_VISIBLE yaml_archive_exception
    : public virtual boost::archive::archive_exception
{
  public:
    typedef enum {
        yaml_archive_parsing_error, // see save_register
        yaml_archive_tag_mismatch,
        yaml_archive_tag_name_error
    } exception_code;
    YAML_AWARCHIVE_API yaml_archive_exception(exception_code c,
                                              const char*    e1 = NULL,
                                              const char*    e2 = NULL);
    YAML_AWARCHIVE_API yaml_archive_exception(yaml_archive_exception const&);
    virtual YAML_AWARCHIVE_API ~yaml_archive_exception()
        BOOST_NOEXCEPT_OR_NOTHROW;
};

} // namespace archive
} // namespace boost

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // BOOST_YAML_ARCHIVE_ARCHIVE_EXCEPTION_HPP
