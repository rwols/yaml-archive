/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// yaml_oarchive_impl.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm> // std::copy
#include <exception>
#include <iomanip>
#include <ostream>
#include <string>

#include <boost/config.hpp> // msvc 6.0 needs this to suppress warnings
#include <cstring>          // strlen
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::strlen;
} // namespace std
#endif

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/yaml_escape.hpp>

#ifndef BOOST_NO_CWCHAR
#if BOOST_VERSION < 106100
#include <boost/archive/iterators/yaml_mb_from_wchar.hpp>
#else // BOOST_VERSION >= 106100
#include <boost/archive/iterators/mb_from_wchar.hpp>
#endif // BOOST_VERSION
#include <boost/archive/wcslen.hpp>
#endif // BOOST_NO_CWCHAR

namespace boost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implemenations of functions specific to char archives

// wide char stuff used by char archives
#ifndef BOOST_NO_CWCHAR
// copy chars to output escaping to yaml and translating wide chars to mb chars
template <class InputIterator>
void save_iterator(std::ostream& os, InputIterator begin, InputIterator end)
{
    typedef boost::archive::iterators::mb_from_wchar<
        boost::archive::iterators::yaml_escape<InputIterator>>
        translator;
    std::copy(translator(begin), translator(end),
              boost::archive::iterators::ostream_iterator<char>(os));
}

#ifndef BOOST_NO_STD_WSTRING
template <class Archive>
YAML_ARCHIVE_API void yaml_oarchive_impl<Archive>::save(const std::wstring& ws)
{
    this->This()->put('"');
    save_iterator(os, ws.data(), ws.data() + ws.size());
    this->This()->put('"');
}
#endif

#ifndef BOOST_NO_INTRINSIC_WCHAR_T
template <class Archive>
YAML_ARCHIVE_API void yaml_oarchive_impl<Archive>::save(const wchar_t* ws)
{
    this->This()->put('"');
    save_iterator(os, ws, ws + std::wcslen(ws));
    this->This()->put('"');
}
#endif

#endif // BOOST_NO_CWCHAR

template <class Archive>
YAML_ARCHIVE_API void yaml_oarchive_impl<Archive>::save(const std::string& s)
{
    this->This()->put('"');
    typedef boost::archive::iterators::yaml_escape<const char*>
        yaml_escape_translator;
    std::copy(yaml_escape_translator(s.data()),
              yaml_escape_translator(s.data() + s.size()),
              boost::archive::iterators::ostream_iterator<char>(os));
    this->This()->put('"');
}

template <class Archive>
YAML_ARCHIVE_API void yaml_oarchive_impl<Archive>::save(const char* s)
{
    this->This()->put('"');
    typedef boost::archive::iterators::yaml_escape<const char*>
        yaml_escape_translator;
    std::copy(yaml_escape_translator(s),
              yaml_escape_translator(s + std::strlen(s)),
              boost::archive::iterators::ostream_iterator<char>(os));
    this->This()->put('"');
}

template <class Archive>
YAML_ARCHIVE_API
yaml_oarchive_impl<Archive>::yaml_oarchive_impl(std::ostream& os_,
                                                unsigned int  flags)
    : basic_text_oprimitive<std::ostream>(os_, 0 != (flags & no_codecvt)),
      basic_yaml_oarchive<Archive>(flags)
{
    // This is the UTF-8 BOM:
    // https://en.wikipedia.org/wiki/Byte_order_mark#UTF-8
    //
    // See: http://yaml.org/spec/1.2/spec.html, section 5.2
    this->This()->put("\xEF\xBB\xBF");
    if (0 == (flags & no_header)) this->init();
}

template <class Archive>
YAML_ARCHIVE_API void
yaml_oarchive_impl<Archive>::save_binary(const void* address, std::size_t count)
{
    this->end_preamble();
    typedef typename std::ostream::char_type CharType;

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

template <class Archive>
YAML_ARCHIVE_API yaml_oarchive_impl<Archive>::~yaml_oarchive_impl()
{
    if (std::uncaught_exception()) return;
    if (0 == (this->get_flags() & no_header)) this->windup();
}

} // namespace archive
} // namespace boost
