/** @file
 *
 * @brief Defines an unescape exception.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_ITERATORS_YAML_UNESCAPE_EXCEPTION_HPP
#define BOOST_ARCHIVE_ITERATORS_YAML_UNESCAPE_EXCEPTION_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>
#ifndef BOOST_NO_EXCEPTIONS
#include <exception>

#include <boost/assert.hpp>

namespace boost {
namespace archive {
namespace iterators {

class yaml_unescape_exception : public std::exception
{
  public:
    yaml_unescape_exception() {}

    virtual const char* what() const throw()
    {
        return "yaml contained un-recognized escape code";
    }
};

} // namespace iterators
} // namespace archive
} // namespace boost

#endif // BOOST_NO_EXCEPTIONS
#endif // BOOST_ARCHIVE_ITERATORS_YAML_UNESCAPE_EXCEPTION_HPP
