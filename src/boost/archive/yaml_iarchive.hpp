/** @file
 *
 * @brief Defines narrow concrete input archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_YAML_IARCHIVE_HPP
#define BOOST_ARCHIVE_YAML_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <istream>

#include <boost/archive/basic_text_iprimitive.hpp>
#include <boost/archive/basic_yaml_iarchive.hpp>
#include <boost/archive/detail/auto_link_archive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/serialization/item_version_type.hpp>
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
typedef basic_yaml_grammar<char> yaml_grammar;

/**
 * @brief      YAML input archive for a narrow character stream.
 *
 * Don't use this class directly. Instead, use boost::archive::yaml_iarchive.
 *
 * @tparam     Archive  The derived archive class.
 */
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
    /**
     * @brief      Load bytes from the YAML archive.
     *
     * The binary data is assumed to be an unqouted base64 encoded string.
     *
     * @param[out] address  The address
     * @param[in]  count    The number of bytes
     *
     * @warning You should **never** use this method. Instead, use
     * boost::serialization::make_binary_object. See the code example below.
     *
     * @code
     * #include <boost/archive/yaml_iarchive.hpp>
     * #include <boost/serialization/binary_object.hpp>
     * #include <boost/serialization/nvp.hpp>
     * #include <iostream>
     * int main()
     * {
     *     char data[150];
     *     boost::archive::yaml_iarchive yaml(std::cin);
     *     using boost::serialization::make_nvp;
     *     using boost::serialization::make_binary_object;
     *     yaml >> make_nvp("data", make_binary_object(data, sizeof(data)));
     *     return 0;
     * }
     * @endcode
     */
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

/**
 * @brief      Concrete YAML input archive for a narrow character stream.
 *
 * Use this class to load your classes from a YAML archive.
 * Do not derive from this class.  If you want to extend this functionality via
 * inheritance, derive from yaml_iarchive_impl instead. This will preserve
 * correct static polymorphism.
 *
 * @see boost::archive::yaml_oarchive for its output counter-part.
 *
 * @code
 * #include <boost/archive/yaml_iarchive.hpp>
 * #include <boost/serialization/nvp.hpp>
 * #include <iostream>
 * #include <string>
 * int main()
 * {
 *     std::string world;
 *     boost::archive::yaml_iarchive yaml(std::cin);
 *     yaml >> boost::serialization::make_nvp("hello", world);
 *     return 0;
 * }
 * @endcode
 */
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
    /**
     * @brief      Constructor.
     *
     * @param      is     The input stream
     * @param[in]  flags  Modifier flags
     *
     * @see http://www.boost.org/doc/libs/1_63_0/boost/archive/basic_archive.hpp
     * for the available flags.
     */
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
