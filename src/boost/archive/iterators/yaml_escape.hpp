/** @file
 *
 * @brief Escape iterators
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_ITERATORS_YAML_ESCAPE_HPP
#define BOOST_ARCHIVE_ITERATORS_YAML_ESCAPE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/archive/iterators/escape.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace archive {
namespace iterators {

template <class Base> class yaml_escape : public escape<yaml_escape<Base>, Base>
{
    friend class boost::iterator_core_access;

    typedef escape<yaml_escape<Base>, Base> super_t;

  public:
    char fill(const char*& bstart, const char*& bend);
    wchar_t fill(const wchar_t*& bstart, const wchar_t*& bend);

    template <class T>
    yaml_escape(T start) : super_t(Base(static_cast<T>(start)))
    {
    }
    // intel 7.1 doesn't like default copy constructor
    yaml_escape(const yaml_escape& rhs) : super_t(rhs.base_reference()) {}
};

template <class Base>
char yaml_escape<Base>::fill(const char*& bstart, const char*& bend)
{
    char current_value = *this->base_reference();
    switch (current_value)
    {
    case '"':
        bstart = "\\\"";
        bend = bstart + 2;
        break;
    default:
        return current_value;
    }
    return *bstart;
}

template <class Base>
wchar_t yaml_escape<Base>::fill(const wchar_t*& bstart, const wchar_t*& bend)
{
    wchar_t current_value = *this->base_reference();
    switch (current_value)
    {
    case '"':
        bstart = L"\\\"";
        bend = bstart + 2;
        break;
    default:
        return current_value;
    }
    return *bstart;
}

} // namespace iterators
} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_ITERATORS_YAML_ESCAPE_HPP
