/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_yaml_iarchive.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <algorithm>
#include <boost/assert.hpp>
#include <cstddef> // NULL

#include <boost/archive/basic_yaml_iarchive.hpp>
#include <boost/archive/yaml_archive_exception.hpp>
#include <boost/serialization/throw_exception.hpp>
#include <boost/serialization/tracking.hpp>

// remove me
#include <iostream>

namespace boost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implementation of yaml_text_archive

template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_iarchive<Archive>::load_start(const char* name)
{
    // name == nullptr happens when we're in the process of serializing a
    // pointer or enum and we are now serializing the "dereferenced" object.
    // In the case of an enum that is just an integer.
    if (nullptr == name) return;
    if (!this->This()->gimpl->parse_start_tag(depth, this->This()->get_is()))
    {
        boost::serialization::throw_exception(
            archive_exception(archive_exception::input_stream_error));
    }
    ++depth;
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
basic_yaml_iarchive<Archive>::load_end(const char* name)
{
    // name == nullptr happens when we're in the process of serializing a
    // pointer or enum and we are now serializing the "dereferenced" object.
    // In the case of an enum that is just an integer.
    if (nullptr == name) return;
    --depth;
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
#if BOOST_VERSION > 105800
basic_yaml_iarchive<Archive>::load_override(object_id_type& t)
#else // BOOST_VERSION <= 105800
basic_yaml_iarchive<Archive>::load_override(object_id_type& t, int)
#endif
{
    t = object_id_type(this->This()->gimpl->rv.object_id);
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
#if BOOST_VERSION > 105800
basic_yaml_iarchive<Archive>::load_override(version_type& t)
#else // BOOST_VERSION <= 105800
basic_yaml_iarchive<Archive>::load_override(version_type& t, int)
#endif
{
    t = version_type(this->This()->gimpl->rv.version);
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
#if BOOST_VERSION > 105800
basic_yaml_iarchive<Archive>::load_override(class_id_type& t)
#else // BOOST_VERSION <= 105800
basic_yaml_iarchive<Archive>::load_override(class_id_type& t, int)
#endif
{
    t = class_id_type(this->This()->gimpl->rv.class_id);
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
#if BOOST_VERSION > 105800
basic_yaml_iarchive<Archive>::load_override(tracking_type& t)
#else // BOOST_VERSION <= 105800
basic_yaml_iarchive<Archive>::load_override(tracking_type& t, int)
#endif
{
    t = this->This()->gimpl->rv.tracking_level;
}

template <class Archive>
BOOST_SYMBOL_VISIBLE
basic_yaml_iarchive<Archive>::basic_yaml_iarchive(unsigned int flags)
    : detail::common_iarchive<Archive>(flags), depth(0)
{
}
template <class Archive>
BOOST_SYMBOL_VISIBLE basic_yaml_iarchive<Archive>::~basic_yaml_iarchive()
{
}

} // namespace archive
} // namespace boost
