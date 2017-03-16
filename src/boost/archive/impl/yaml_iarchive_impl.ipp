/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// yaml_iarchive_impl.cpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <boost/config.hpp>
#include <cstddef> // NULL
#include <cstring> // memcpy
#include <exception>

#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::memcpy;
} // namespace std
#endif

#ifndef BOOST_NO_CWCHAR
#include <cwchar> // mbstate_t and mbrtowc
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::mbstate_t;
using ::mbrtowc;
} // namespace std
#endif
#endif // BOOST_NO_CWCHAR

#include <boost/detail/workaround.hpp> // RogueWave and Dinkumware
#if BOOST_WORKAROUND(BOOST_DINKUMWARE_STDLIB, == 1)
#include <boost/archive/dinkumware.hpp>
#endif

#include "basic_yaml_grammar.hpp"
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/dataflow_exception.hpp>
#include <boost/archive/iterators/istream_iterator.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/yaml_archive_exception.hpp>
#include <boost/archive/yaml_iarchive.hpp>
#include <boost/core/no_exceptions_support.hpp>
#include <boost/locale/encoding_utf.hpp>

namespace boost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implemenations of functions specific to char archives

// wide char stuff used by char archives

#ifndef BOOST_NO_CWCHAR
#ifndef BOOST_NO_STD_WSTRING
template <class Archive>
BOOST_ARCHIVE_DECL void yaml_iarchive_impl<Archive>::load(std::wstring& ws)
{
    std::string utf8string;

    bool result = gimpl->parse_string(this->This()->depth, is, utf8string);

    if (!result)
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));

    using locale::conv::utf_to_utf;
    ws = utf_to_utf<std::wstring::value_type>(utf8string);
}
#endif // BOOST_NO_STD_WSTRING

#ifndef BOOST_NO_INTRINSIC_WCHAR_T
template <class Archive>
BOOST_ARCHIVE_DECL void yaml_iarchive_impl<Archive>::load(wchar_t* ws)
{
    std::string s;

    bool result = gimpl->parse_string(this->This()->depth, is, s);

    if (!result)
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));

    std::mbstate_t mbs = std::mbstate_t();
    const char*    start = s.data();
    const char*    end = start + s.size();
    while (start < end)
    {
        wchar_t     wc;
        std::size_t length = std::mbrtowc(&wc, start, end - start, &mbs);
        if (static_cast<std::size_t>(-1) == length)
            serialization::throw_exception(iterators::dataflow_exception(
                iterators::dataflow_exception::invalid_conversion));
        if (static_cast<std::size_t>(-2) == length) continue;

        start += length;
        *ws++ = wc;
    }
    *ws = L'\0';
}
#endif // BOOST_NO_INTRINSIC_WCHAR_T

#endif // BOOST_NO_CWCHAR

template <class Archive>
BOOST_ARCHIVE_DECL void yaml_iarchive_impl<Archive>::load(std::string& s)
{
    bool result = gimpl->parse_string(this->This()->depth, is, s);
    if (!result)
    {
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    }
}

template <class Archive>
BOOST_ARCHIVE_DECL void yaml_iarchive_impl<Archive>::load(char* s)
{
    std::string tstring;
    bool        result = gimpl->parse_string(this->This()->depth, is, tstring);
    if (!result)
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    std::memcpy(s, tstring.data(), tstring.size());
    s[tstring.size()] = 0;
}

template <class Archive>
BOOST_ARCHIVE_DECL void
yaml_iarchive_impl<Archive>::load_override(class_name_type& t)
{
    const std::string& s = gimpl->rv.class_name;
    if (s.size() > BOOST_SERIALIZATION_MAX_KEY_SIZE - 1)
        serialization::throw_exception(
            archive_exception(archive_exception::invalid_class_name));
    char* tptr = t;
    std::memcpy(tptr, s.data(), s.size());
    tptr[s.size()] = '\0';
}

template <class Archive>
BOOST_ARCHIVE_DECL void yaml_iarchive_impl<Archive>::init()
{
    gimpl->init(is);
    this->set_library_version(library_version_type(gimpl->rv.version));
}

template <class Archive>
BOOST_ARCHIVE_DECL
yaml_iarchive_impl<Archive>::yaml_iarchive_impl(std::istream& is_,
                                                unsigned int  flags)
    : basic_text_iprimitive<std::istream>(is_, 0 != (flags & no_codecvt)),
      basic_yaml_iarchive<Archive>(flags), gimpl(new yaml_grammar())
{
    // Read the byte order mark (UTF-8).
    // See: http://yaml.org/spec/1.2/spec.html, section 5.2.
    if (is.get() != 0xEF || is.fail())
    {
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    }
    if (is.get() != 0xBB || is.fail())
    {
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    }
    if (is.get() != 0xBF || is.fail())
    {
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    }
    if (0 == (flags & no_header)) init();
}

template <class Archive>
BOOST_ARCHIVE_DECL yaml_iarchive_impl<Archive>::~yaml_iarchive_impl()
{
    if (std::uncaught_exception()) return;
    if (0 == (this->get_flags() & no_header))
    {
        gimpl->windup(is);
    }
}

template <class Archive>
BOOST_ARCHIVE_DECL void
yaml_iarchive_impl<Archive>::load_binary(void* address, std::size_t count)
{
    typedef typename std::istream::char_type CharType;

    if (0 == count) return;

    BOOST_ASSERT(static_cast<std::size_t>(
                     (std::numeric_limits<std::streamsize>::max)()) >
                 (count + sizeof(CharType) - 1) / sizeof(CharType));

    if (is.fail())
        serialization::throw_exception(
            archive_exception(archive_exception::input_stream_error));
    // convert from base64 to binary
    typedef typename iterators::transform_width<
        iterators::binary_from_base64<
            iterators::remove_whitespace<iterators::istream_iterator<CharType>>,
            typename std::istream::int_type>,
        8, 6, CharType>
        binary;

    binary i = binary(iterators::istream_iterator<CharType>(is));

    char* caddr = static_cast<char*>(address);

    // take care that we don't increment anymore than necessary
    while (count-- > 0)
    {
        *caddr++ = static_cast<char>(*i++);
    }

    // skip over any excess input
    for (;;)
    {
        const typename std::istream::int_type r = is.get();
        if (is.eof() || r == '\n')
        {
            break;
        }
    }
}

} // namespace archive
} // namespace boost
