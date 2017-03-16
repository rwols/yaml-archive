/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// yaml_woarchive_impl.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#ifndef BOOST_NO_STD_WSTREAMBUF

#include <algorithm> // std::copy
#include <exception>
#include <locale>
#include <ostream>
#include <string>

#include <cstdlib> // mbtowc
#include <cstring> // strlen
#include <cwchar>  // wcslen

#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::strlen;
#if !defined(BOOST_NO_INTRINSIC_WCHAR_T)
using ::mbtowc;
using ::wcslen;
#endif
} // namespace std
#endif

#include <boost/archive/detail/utf8_codecvt_facet.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/dataflow_exception.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/wchar_from_mb.hpp>
#include <boost/archive/iterators/yaml_escape.hpp>
#include <boost/archive/yaml_woarchive.hpp>
#include <boost/serialization/throw_exception.hpp>

namespace boost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implemenations of functions specific to wide char archives

// copy chars to output escaping to yaml and widening characters as we go
template <class InputIterator>
void save_iterator(std::wostream& os, InputIterator begin, InputIterator end)
{
    typedef iterators::wchar_from_mb<iterators::yaml_escape<InputIterator>>
        xmbtows;
    std::copy(xmbtows(begin), xmbtows(end),
              boost::archive::iterators::ostream_iterator<wchar_t>(os));
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
yaml_woarchive_impl<Archive>::save(const std::string& s)
{
    // note: we don't use s.begin() and s.end() because dinkumware
    // doesn't have string::value_type defined. So use a wrapper
    // around these values to implement the definitions.
    const char* begin = s.data();
    const char* end = begin + s.size();
    this->This()->put('"');
    save_iterator(os, begin, end);
    this->This()->put('"');
}

#ifndef BOOST_NO_STD_WSTRING
template <class Archive>
BOOST_SYMBOL_VISIBLE void
yaml_woarchive_impl<Archive>::save(const std::wstring& ws)
{
    typedef iterators::yaml_escape<const wchar_t*> xmbtows;
    this->This()->put('"');
    std::copy(xmbtows(ws.data()), xmbtows(ws.data() + ws.size()),
              boost::archive::iterators::ostream_iterator<wchar_t>(os));
    this->This()->put('"');
}
#endif // BOOST_NO_STD_WSTRING

template <class Archive>
BOOST_SYMBOL_VISIBLE void yaml_woarchive_impl<Archive>::save(const char* s)
{
    this->This()->put('"');
    save_iterator(os, s, s + std::strlen(s));
    this->This()->put('"');
}

#ifndef BOOST_NO_INTRINSIC_WCHAR_T
template <class Archive>
BOOST_SYMBOL_VISIBLE void yaml_woarchive_impl<Archive>::save(const wchar_t* ws)
{
    this->This()->put('"');
    typedef iterators::yaml_escape<const wchar_t*> xmbtows;
    std::copy(xmbtows(ws), xmbtows(ws + std::wcslen(ws)),
              boost::archive::iterators::ostream_iterator<wchar_t>(os));
    this->This()->put('"');
}
#endif

template <class Archive>
BOOST_SYMBOL_VISIBLE
yaml_woarchive_impl<Archive>::yaml_woarchive_impl(std::wostream& os_,
                                                  unsigned int   flags)
    : basic_text_oprimitive<std::wostream>(
          os_,
          true // don't change the codecvt - use the one below
          ),
      basic_yaml_oarchive<Archive>(flags)
{
    // This is the UTF-8 BOM:
    // https://en.wikipedia.org/wiki/Byte_order_mark#UTF-8
    //
    // See: http://yaml.org/spec/1.2/spec.html, section 5.2
    // this->This()->put("\xEF\xBB\xBF");
    this->This()->put(0xEF);
    this->This()->put(0xBB);
    this->This()->put(0xBF);
    if (0 == (flags & no_codecvt))
    {
        std::locale l = std::locale(
            os_.getloc(), new boost::archive::detail::utf8_codecvt_facet);
        os_.flush();
        os_.imbue(l);
    }
    if (0 == (flags & no_header)) this->init();
}

template <class Archive>
BOOST_SYMBOL_VISIBLE yaml_woarchive_impl<Archive>::~yaml_woarchive_impl()
{
    if (std::uncaught_exception()) return;
    if (0 == (this->get_flags() & no_header)) this->windup();
}

template <class Archive>
BOOST_SYMBOL_VISIBLE void
yaml_woarchive_impl<Archive>::save_binary(const void* address,
                                          std::size_t count)
{
    this->end_preamble();
    typedef typename std::wostream::char_type CharType;

    if (0 == count) return;

    if (os.fail())
        boost::serialization::throw_exception(
            archive_exception(archive_exception::output_stream_error));

    typedef boost::archive::iterators::base64_from_binary<
        boost::archive::iterators::transform_width<const char*, 6, 8>,
        const char // cwpro8 needs this
        >
        base64_text;

    boost::archive::iterators::ostream_iterator<CharType> oi(os);
    std::copy(base64_text(static_cast<const char*>(address)),
              base64_text(static_cast<const char*>(address) + count), oi);

    std::size_t tail = count % 3;
    if (tail > 0)
    {
        *oi++ = '=';
        if (tail < 2) *oi = '=';
    }
}

} // namespace archive
} // namespace boost

#endif // BOOST_NO_STD_WSTREAMBUF
