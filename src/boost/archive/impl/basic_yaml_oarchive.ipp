/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_yaml_oarchive.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <algorithm>
#include <cstddef> // NULL
#include <cstring>
#if defined(BOOST_NO_STDC_NAMESPACE) && !defined(__LIBCOMO__)
namespace std {
using ::strlen;
} // namespace std
#endif
#include <boost/archive/basic_yaml_oarchive.hpp>
#include <boost/archive/yaml_archive_exception.hpp>
#include <boost/core/no_exceptions_support.hpp>

namespace boost {
namespace archive {

namespace detail {
template <class CharType> struct YAML_name
{
    void operator()(CharType t) const
    {
        const unsigned char lookup_table[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, // -.
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, // 0-9
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // A-
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, // -Z _
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a-
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, // -z
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        if ((unsigned)t > 127) return;
        if (0 == lookup_table[(unsigned)t])
            boost::serialization::throw_exception(yaml_archive_exception(
                yaml_archive_exception::yaml_archive_tag_name_error));
    }
};

} // namespace detail

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implemenations of functions common to both types of yaml output

template <class Archive>
BOOST_SYMBOL_VISIBLE void basic_yaml_oarchive<Archive>::indent()
{
    for (int i = 0; i != 2 * depth; ++i) this->This()->put(' ');
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_start(const char* name)
{
    if (nullptr == name) return;

    // be sure name has no invalid characters
    std::for_each(name, name + std::strlen(name),
                  detail::YAML_name<const char>());

    end_preamble();
    if (depth > 0)
    {
        this->This()->put('\n');
        indent();
    }
    ++depth;
    this->This()->put(name);
    this->This()->put(": ");
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_end(const char* name)
{
    if (nullptr == name) return;
    --depth;
    if (0 == depth) this->This()->put('\n');
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void basic_yaml_oarchive<Archive>::end_preamble()
{
    if (m_pending_class_name != nullptr)
    {
        this->This()->put('/');
        this->This()->put(m_pending_class_name);
        m_pending_class_name = nullptr;
    }
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const object_id_type& t)
{
    if (m_pending_class_name != nullptr)
    {
        this->This()->put('/');
        this->This()->put(m_pending_class_name);
        m_pending_class_name = nullptr;
    }
    this->This()->put(" &");
    this->This()->save(static_cast<int>(t));
}
template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const object_reference_type& t)
{
    this->This()->put(" *");
    this->This()->save(static_cast<int>(t));
}
template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const version_type& t)
{
    this->This()->put('v');
    this->This()->save(static_cast<int>(t));
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const class_id_type& t)
{
    if (t == NULL_POINTER_TAG)
    {
        this->This()->put('~');
    }
    else
    {
        this->This()->put("!!c");
        this->This()->save(static_cast<int>(t));
    }
}
template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const class_id_reference_type& t)
{
    this->This()->put("!!r");
    this->This()->save(static_cast<int>(t));
}
template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const class_id_optional_type& t)
{
    this->This()->put("!!c");
    this->This()->save(static_cast<int>(t));
}
template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const class_name_type& t)
{
    m_pending_class_name = t;
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_oarchive<Archive>::save_override(const tracking_type& t)
{
    if (t.t) this->This()->put('t');
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void basic_yaml_oarchive<Archive>::init()
{
    this->This()->put("%YAML 1.2\n");
    this->This()->put("%TAG ! boost/archive/v");
    this->This()->save(BOOST_ARCHIVE_VERSION());
    this->This()->put("/\n");
    this->This()->put("---\n");
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void basic_yaml_oarchive<Archive>::windup()
{
    this->This()->put("...");
}

template <class Archive>
BOOST_SYMBOL_VISIBLE
basic_yaml_oarchive<Archive>::basic_yaml_oarchive(unsigned int flags)
    : detail::common_oarchive<Archive>(flags), depth(0)
{
}

template <class Archive>
BOOST_SYMBOL_VISIBLE basic_yaml_oarchive<Archive>::~basic_yaml_oarchive()
{
}

} // namespace archive
} // namespace boost
