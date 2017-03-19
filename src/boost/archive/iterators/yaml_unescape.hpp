/** @file
 *
 * @brief Unescape iterators
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_ITERATORS_YAML_UNESCAPE_HPP
#define BOOST_ARCHIVE_ITERATORS_YAML_UNESCAPE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/assert.hpp>

#include <boost/serialization/throw_exception.hpp>

#include <boost/archive/iterators/dataflow_exception.hpp>
#include <boost/archive/iterators/unescape.hpp>

namespace boost {
namespace archive {
namespace iterators {

template <class Base>
class yaml_unescape : public unescape<yaml_unescape<Base>, Base>
{
    friend class boost::iterator_core_access;
    typedef yaml_unescape<Base> this_t;
    typedef unescape<this_t, Base> super_t;
    typedef typename boost::iterator_reference<this_t> reference_type;

    reference_type dereference() const
    {
        return unescape<yaml_unescape<Base>, Base>::dereference();
    }

  public:
// workaround msvc 7.1 ICU crash
#if defined(BOOST_MSVC)
    typedef int value_type;
#else
    typedef typename this_t::value_type value_type;
#endif

    void drain_residue(const char* literal);
    value_type drain();

    template <class T>
    yaml_unescape(T start) : super_t(Base(static_cast<T>(start)))
    {
    }
    // intel 7.1 doesn't like default copy constructor
    yaml_unescape(const yaml_unescape& rhs) : super_t(rhs.base_reference()) {}
};

template <class Base>
void yaml_unescape<Base>::drain_residue(const char* literal)
{
    do
    {
        if (*literal != *++(this->base_reference()))
            boost::serialization::throw_exception(dataflow_exception(
                dataflow_exception::invalid_xml_escape_sequence));
    } while ('\0' != *++literal);
}

// note key constraint on this function is that can't "look ahead" any
// more than necessary into base iterator.  Doing so would alter the base
// iterator refenence which would make subsequent iterator comparisons
// incorrect and thereby break the composiblity of iterators.
template <class Base>
typename yaml_unescape<Base>::value_type
    // int
    yaml_unescape<Base>::drain()
{
    value_type retval = *this->base_reference();
    if ('&' != retval)
    {
        return retval;
    }
    retval = *++(this->base_reference());
    switch (retval)
    {
    case 'l': // &lt;
        drain_residue("t;");
        retval = '<';
        break;
    case 'g': // &gt;
        drain_residue("t;");
        retval = '>';
        break;
    case 'a':
        retval = *++(this->base_reference());
        switch (retval)
        {
        case 'p': // &apos;
            drain_residue("os;");
            retval = '\'';
            break;
        case 'm': // &amp;
            drain_residue("p;");
            retval = '&';
            break;
        }
        break;
    case 'q':
        drain_residue("uot;");
        retval = '"';
        break;
    }
    return retval;
}

} // namespace iterators
} // namespace archive
} // namespace boost

#endif // BOOST_ARCHIVE_ITERATORS_YAML_UNESCAPE_HPP
