#ifndef BOOST_ARCHIVE_YAML_IARCHIVE_HPP
#define BOOST_ARCHIVE_YAML_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// yaml_iarchive.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <istream>

#include <boost/archive/basic_text_iprimitive.hpp>
#include <boost/archive/basic_yaml_iarchive.hpp>
#include <boost/archive/detail/auto_link_archive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/serialization/item_version_type.hpp>
#if BOOST_VERSION < 105600
#include <boost/archive/shared_ptr_helper.hpp>
#endif

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4511 4512)
#endif

namespace boost {
namespace archive {

namespace detail {
template <class Archive> class interface_iarchive;
} // namespace detail

template <class CharType> class basic_yaml_grammar;
typedef basic_yaml_grammar<char> yaml_grammar;

template <class Archive>
class BOOST_SYMBOL_VISIBLE yaml_iarchive_impl
    : public basic_text_iprimitive<std::istream>,
      public basic_yaml_iarchive<Archive>
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
  public:
#else
  protected:
    friend class detail::interface_iarchive<Archive>;
    friend class basic_yaml_iarchive<Archive>;
    friend class load_access;
#endif
    // use boost:scoped_ptr to implement automatic deletion;
    boost::scoped_ptr<yaml_grammar> gimpl;

    std::istream&           get_is() { return is; }
    template <class T> void load(T& t)
    {
        basic_text_iprimitive<std::istream>::load(t);
    }
    void load(version_type& t)
    {
        unsigned int v;
        load(v);
        t = version_type(v);
    }
    void load(boost::serialization::item_version_type& t)
    {
        unsigned int v;
        load(v);
        t = boost::serialization::item_version_type(v);
    }
    YAML_ARCHIVE_API void load(char* t);
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
    YAML_ARCHIVE_API void load(wchar_t* t);
#endif // BOOST_NO_INTRINSIC_WCHAR_T
    YAML_ARCHIVE_API void load(std::string& s);
#ifndef BOOST_NO_STD_WSTRING
    YAML_ARCHIVE_API void load(std::wstring& ws);
#endif // BOOST_NO_STD_WSTRING
#if BOOST_VERSION > 105800
    template <class T> void load_override(T& t)
    {
        basic_yaml_iarchive<Archive>::load_override(t);
    }
    YAML_ARCHIVE_API void load_override(class_name_type& t);
#else  // BOOST_VERSION <= 105800
    template <class T> void load_override(T& t, BOOST_PFTO int)
    {
        basic_yaml_iarchive<Archive>::load_override(t, 0);
    }
    YAML_ARCHIVE_API void load_override(class_name_type& t, int);
#endif // BOOST_VERSION
    YAML_ARCHIVE_API void init();
    YAML_ARCHIVE_API yaml_iarchive_impl(std::istream& is, unsigned int flags);
    YAML_ARCHIVE_API ~yaml_iarchive_impl();

  public:
    YAML_ARCHIVE_API void load_binary(void* address, std::size_t count);
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas
#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4511 4512)
#endif

namespace boost {
namespace archive {

#if BOOST_VERSION < 105600
class BOOST_SYMBOL_VISIBLE yaml_iarchive
    : public yaml_iarchive_impl<yaml_iarchive>,
      public detail::shared_ptr_helper
{
#else
class BOOST_SYMBOL_VISIBLE yaml_iarchive
    : public yaml_iarchive_impl<yaml_iarchive>
{
#endif
  public:
    yaml_iarchive(std::istream& is, unsigned int flags = 0)
        : yaml_iarchive_impl<yaml_iarchive>(is, flags)
    {
    }
    ~yaml_iarchive(){};
};

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::yaml_iarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // BOOST_ARCHIVE_YAML_IARCHIVE_HPP
