/** @file
 *
 * @brief Declares wide concrete input archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_YAML_WIARCHIVE_HPP
#define BOOST_ARCHIVE_YAML_WIARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <istream>

#include <boost/archive/basic_text_iprimitive.hpp>
#include <boost/archive/basic_yaml_iarchive.hpp>
#include <boost/archive/detail/auto_link_warchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/version.hpp>
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
typedef basic_yaml_grammar<wchar_t> yaml_wgrammar;

/**
 * @brief      YAML input archive for a wide character stream.
 *
 * Don't use this class directly. Instead, use boost::archive::yaml_wiarchive.
 *
 * @tparam     Archive  The derived archive class.
 */
template <class Archive>
class BOOST_SYMBOL_VISIBLE yaml_wiarchive_impl
    : public basic_text_iprimitive<std::wistream>,
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
    boost::scoped_ptr<yaml_wgrammar> gimpl;
    std::wistream&                   get_is() { return is; }
    template <class T> void load(T& t)
    {
        basic_text_iprimitive<std::wistream>::load(t);
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
    YAML_WARCHIVE_API void load(char* t);
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
    YAML_WARCHIVE_API void load(wchar_t* t);
#endif
    YAML_WARCHIVE_API void load(std::string& s);
#ifndef BOOST_NO_STD_WSTRING
    YAML_WARCHIVE_API void load(std::wstring& ws);
#endif
#if BOOST_VERSION > 105800
    template <class T> void load_override(T& t)
    {
        basic_yaml_iarchive<Archive>::load_override(t);
    }
    YAML_WARCHIVE_API void load_override(class_name_type& t);
#else  // BOOST_VERSION <= 105800
    template <class T> void load_override(T& t, BOOST_PFTO int)
    {
        basic_yaml_iarchive<Archive>::load_override(t, 0);
    }
    YAML_WARCHIVE_API void load_override(class_name_type& t, int);
#endif // BOOST_VERSION
    YAML_WARCHIVE_API void init();
    YAML_WARCHIVE_API yaml_wiarchive_impl(std::wistream& is,
                                          unsigned int   flags);
    YAML_WARCHIVE_API ~yaml_wiarchive_impl();
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

/**
 * @brief      Concrete YAML input archive for a wide character stream.
 *
 * Use this class to load your classes from a YAML archive.
 * Do not derive from this class.  If you want to extend this functionality via
 * inheritance, derive from yaml_wiarchive_impl instead. This will preserve
 * correct static polymorphism.
 *
 * @see boost::archive::yaml_woarchive for its output counter-part.
 *
 * @code
 * #include <boost/archive/yaml_wiarchive.hpp>
 * #include <boost/serialization/nvp.hpp>
 * #include <iostream>
 * #include <string>
 * int main()
 * {
 *     std::wstring world;
 *     boost::archive::yaml_wiarchive yaml(std::wcin);
 *     yaml >> boost::serialization::make_nvp("hello", world);
 *     return 0;
 * }
 * @endcode
 */
#if BOOST_VERSION < 105600
class BOOST_SYMBOL_VISIBLE yaml_wiarchive
    : public yaml_wiarchive_impl<yaml_wiarchive>,
      public detail::shared_ptr_helper
{
#else
class BOOST_SYMBOL_VISIBLE yaml_wiarchive
    : public yaml_wiarchive_impl<yaml_wiarchive>
{
#endif
  public:
    /**
     * @brief      Constructor.
     *
     * @param      is     The input stream
     * @param[in]  flags  Modifier flags
     *
     * @see http://www.boost.org/doc/libs/1_63_0/boost/archive/basic_archive.hpp
     * for the available flags.
     */
    yaml_wiarchive(std::wistream& is, unsigned int flags = 0)
        : yaml_wiarchive_impl<yaml_wiarchive>(is, flags)
    {
    }
    ~yaml_wiarchive() {}
};

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::yaml_wiarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_YAML_WIARCHIVE_HPP
