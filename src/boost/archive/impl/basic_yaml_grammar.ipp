/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_yaml_grammar.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#if (defined _MSC_VER) && (_MSC_VER == 1200)
#pragma warning(disable : 4786) // too long name, harmless warning
#endif

#include <algorithm>
#include <boost/config.hpp> // typename
#include <istream>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4244 4511 4512)
#endif

// spirit stuff
#include <boost/spirit/include/classic_actions.hpp>
#include <boost/spirit/include/classic_numerics.hpp>
#include <boost/spirit/include/classic_operators.hpp>

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/archive/impl/basic_yaml_grammar.hpp>
#include <boost/archive/iterators/yaml_unescape.hpp>
#include <boost/archive/yaml_archive_exception.hpp>
#include <boost/function.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/serialization/throw_exception.hpp>

using namespace boost::spirit::classic;

namespace boost {
namespace archive {

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// template code for basic_yaml_grammar of both wchar_t and char types

namespace yaml { // anonymous

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4511 4512)
#endif

template <class T> struct assign_impl
{
    T&   t;
    void operator()(const T t_) const { t = t_; }
    assign_impl(T& t_) : t(t_) {}
};

template <> struct assign_impl<std::string>
{
    std::string& t;
    void operator()(std::string::const_iterator b,
                    std::string::const_iterator e) const
    {
        t.resize(0);
        while (b != e)
        {
            t += *b;
            ++b;
        }
    }
    assign_impl<std::string>& operator=(assign_impl<std::string>& rhs);
    assign_impl(std::string& t_) : t(t_) {}
};

#ifndef BOOST_NO_STD_WSTRING
template <> struct assign_impl<std::wstring>
{
    std::wstring& t;
    void operator()(std::wstring::const_iterator b,
                    std::wstring::const_iterator e) const
    {
        t.resize(0);
        while (b != e)
        {
            t += *b;
            ++b;
        }
    }
    assign_impl(std::wstring& t_) : t(t_) {}
};
#endif

template <class T> assign_impl<T> assign_object(T& t)
{
    return assign_impl<T>(t);
}

struct assign_level
{
    tracking_type& tracking_level;
    void operator()(const unsigned int tracking_level_) const
    {
        tracking_level = (0 == tracking_level_) ? false : true;
    }
    assign_level(tracking_type& tracking_level_)
        : tracking_level(tracking_level_)
    {
    }
};

struct set_tracking
{
    tracking_type& tracking_level;

    template <class X, class Y>
    void operator()(const X& /*x*/, const Y& /*y*/) const
    {
        tracking_level = true;
    }

    set_tracking(tracking_type& tracking_level_)
        : tracking_level(tracking_level_)
    {
    }
};

struct set_null_pointer
{
    int_least16_t& class_id;

    template <class X, class Y>
    void operator()(const X& /*x*/, const Y& /*y*/) const
    {
        class_id = NULL_POINTER_TAG;
    }

    set_null_pointer(int_least16_t& class_id_) : class_id(class_id_) {}
};

template <class String, class Iterator> struct append_string
{
    String& contents;
    void operator()(Iterator start, Iterator end) const
    {
#if 0
        typedef boost::archive::iterators::yaml_unescape<Iterator> translator;
        contents.append(
            translator(BOOST_MAKE_PFTO_WRAPPER(start)), 
            translator(BOOST_MAKE_PFTO_WRAPPER(end))
        );
#endif
        contents.append(start, end);
    }
    append_string(String& contents_) : contents(contents_) {}
};

template <class String> struct append_char
{
    String& contents;
    void operator()(const unsigned int char_value) const
    {
        const typename String::value_type z = char_value;
        contents += z;
    }
    append_char(String& contents_) : contents(contents_) {}
};

template <class String, unsigned int c> struct append_lit
{
    String& contents;
    template <class X, class Y>
    void operator()(const X& /*x*/, const Y& /*y*/) const
    {
        const typename String::value_type z = c;
        contents += z;
    }
    append_lit(String& contents_) : contents(contents_) {}
};

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

} // namespace anonymous

template <class CharType>
bool basic_yaml_grammar<CharType>::parse(
    unsigned int depth, typename basic_yaml_grammar<CharType>::IStream& is,
    const rule_t& rule_, CharType delimiter) const
{
    std::basic_string<CharType> arg;
    CharType                    val;
    if (is.fail())
    {
        return false;
    }
    else if (is.peek() == '\n')
    {
        is.get();
    }
    do
    {
        val = static_cast<CharType>(is.get());
        if (is.fail()) return false;
        arg += val;
    } while (val != '\n');
    parse_info<typename std::basic_string<CharType>::iterator> parseresult =
        boost::spirit::classic::parse(arg.begin(), arg.end(), rule_);
    return parseresult.hit;
}

template <class CharType>
bool basic_yaml_grammar<CharType>::parse_start_tag(unsigned int depth,
                                                   IStream&     is)
{
    rv.class_name.clear();
    rv.tracking_level = tracking_type(false);
    if (is.fail())
    {
        return false;
    }
    else if (is.peek() == '\n')
    {
        is.get();
    }

    boost::io::ios_flags_saver ifs(is);
    is >> std::noskipws;
    depth *= 2;
    while (depth != 0)
    {
        if (!std::isspace(is.get()) || is.fail())
        {
            return false;
        }
        --depth;
    }
    std::basic_string<CharType> arg;
    CharType                    val;
    bool                        extracting_meta = false;
    bool                        after_colon = false;
    do
    {
        val = static_cast<CharType>(is.get());
        if (is.fail()) return false;
        if (after_colon)
        {
            if (val == '!' || val == '&' || val == '*' || val == '~')
            {
                extracting_meta = true;
                arg += val;
                break;
            }
            else if (!std::isspace(val))
            {
                is.unget();
                break;
            }
        }
        else if (val == ':')
        {
            after_colon = true;
        }
        arg += val;
    } while (val != '\n');
    if (extracting_meta)
    {
        do
        {
            val = static_cast<CharType>(is.get());
            if (is.fail()) return false;
            arg += val;
        } while (val != '\n');
    }
    if (!arg.empty() && arg.back() == '\n') arg.pop_back();
    parse_info<typename std::basic_string<CharType>::iterator> parseresult =
        boost::spirit::classic::parse(arg.begin(), arg.end(), StartTag);
    return parseresult.hit;
}

template <class CharType>
bool basic_yaml_grammar<CharType>::parse_end_tag(unsigned int depth,
                                                 IStream&     is) const
{
    return true;
}

template <class CharType>
bool basic_yaml_grammar<CharType>::parse_string(unsigned int depth, IStream& is,
                                                StringType& s)
{
    if (is.fail())
    {
        return false;
    }
    boost::io::ios_flags_saver ifs(is);
    is >> std::noskipws;
    std::basic_string<CharType> arg;
    CharType                    val;
    typename IStream::int_type  result;
    do
    {
        result = is.get();
        if (is.fail())
        {
            return false;
        }
        val = static_cast<CharType>(result);
        arg += val;
    } while (val != '\n');
    if (!arg.empty() && arg.back() == '\n') arg.pop_back();
    rv.contents.clear();
    parse_info<typename std::basic_string<CharType>::iterator> parseresult =
        boost::spirit::classic::parse(arg.begin(), arg.end(), content);
    if (parseresult.hit) s = std::move(rv.contents);
    return parseresult.hit;
}

template <class CharType> basic_yaml_grammar<CharType>::basic_yaml_grammar()
{
    init_chset();

    S = +(Sch);

    // refactoring to workaround template depth on darwin
    NameHead = (Letter | '_');
    NameTail = *(NameChar | '_');
    Name = NameHead >> NameTail;

    NullPointer = str_p("~")[yaml::set_null_pointer(rv.class_id)];

    AttributeList =
        (str_p("!!") >> ClassIDAttribute >> !TrackingAttribute >>
         !VersionAttribute >> !ClassNameAttribute >> !S >> !ObjectIDAttribute) |
        ObjectIDAttribute;

    StartTag = Name[yaml::assign_object(rv.object_name)] >> L':' >> S >>
               !(NullPointer | AttributeList);

    // refactoring to workaround template depth on darwin
    CharDataChars = +(anychar_p - chset_p(L"\\\""));
    CharData = CharDataChars[yaml::append_string<
        StringType, typename std::basic_string<CharType>::const_iterator>(
        rv.contents)];

    QuoteRef = str_p(L"\\\"")[yaml::append_lit<StringType, L'"'>(rv.contents)];

    content = L'"' >> *(QuoteRef | CharData) >> L'"';

    ClassIDAttribute =
        (str_p(L"r") | str_p(L"c")) >> int_p[yaml::assign_object(rv.class_id)];

    ObjectIDAttribute = (str_p(L"&") | str_p(L"*")) >>
                        uint_p[yaml::assign_object(rv.object_id)];

    ClassNameChar =
        (anychar_p -
         chset_p(L"\" "))[yaml::append_char<StringType>(rv.class_name)];

    ClassName = +ClassNameChar;

    ClassNameAttribute = str_p(L"/") >> ClassName;

    TrackingAttribute = str_p(L"t")[yaml::set_tracking(rv.tracking_level)];

    VersionAttribute = str_p(L"v") >> uint_p[yaml::assign_object(rv.version)];

    YAMLDirective = str_p(L"%YAML 1.2");
    TagDirective = str_p(L"%TAG ! boost/archive/") >> VersionAttribute >> '/';

    StartDoc = str_p(L"---");
    EndDoc = str_p(L"...");

#ifdef BOOST_SPIRIT_DEBUG
    BOOST_SPIRIT_DEBUG_NODE(YAMLDirective);
    BOOST_SPIRIT_DEBUG_NODE(TagDirective);
    BOOST_SPIRIT_DEBUG_NODE(StartDoc);
    BOOST_SPIRIT_DEBUG_NODE(EndDoc);
    BOOST_SPIRIT_DEBUG_NODE(StartTag);
    BOOST_SPIRIT_DEBUG_NODE(ClassIDAttribute);
    BOOST_SPIRIT_DEBUG_NODE(ObjectIDAttribute);
    BOOST_SPIRIT_DEBUG_NODE(VersionAttribute);
    BOOST_SPIRIT_DEBUG_NODE(ClassNameAttribute);
    BOOST_SPIRIT_DEBUG_NODE(TrackingAttribute);
    BOOST_SPIRIT_DEBUG_NODE(AttributeList);
    BOOST_SPIRIT_DEBUG_NODE(S);
    BOOST_SPIRIT_DEBUG_NODE(CharData);
    BOOST_SPIRIT_DEBUG_NODE(CharDataChars);
    BOOST_SPIRIT_DEBUG_NODE(Name);
    BOOST_SPIRIT_DEBUG_NODE(NameHead);
    BOOST_SPIRIT_DEBUG_NODE(NameTail);
    BOOST_SPIRIT_DEBUG_NODE(Letter);
    BOOST_SPIRIT_DEBUG_NODE(NameChar);
    BOOST_SPIRIT_DEBUG_NODE(content);
    BOOST_SPIRIT_DEBUG_NODE(Sch);
    BOOST_SPIRIT_DEBUG_NODE(NullPointer);
#endif
}

template <class CharType> void basic_yaml_grammar<CharType>::init(IStream& is)
{
    init_chset();
    if (!parse(0, is, YAMLDirective))
    {
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    }
    if (!parse(0, is, TagDirective))
    {
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    }
    if (!parse(0, is, StartDoc))
    {
        serialization::throw_exception(yaml_archive_exception(
            yaml_archive_exception::yaml_archive_parsing_error));
    }
}

template <class CharType> bool basic_yaml_grammar<CharType>::windup(IStream& is)
{
    return parse(0, is, EndDoc);
}

} // namespace archive
} // namespace boost
