/** @file
 *
 * @brief Declares wide concrete output archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_YAML_WOARCHIVE_HPP
#define BOOST_ARCHIVE_YAML_WOARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else
#include <cstddef> // size_t
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std {
using ::size_t;
} // namespace std
#endif

#include <ostream>

#include <boost/archive/basic_text_oprimitive.hpp>
#include <boost/archive/basic_yaml_oarchive.hpp>
#include <boost/archive/detail/auto_link_warchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
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
template <class Archive> class interface_oarchive;
} // namespace detail

/**
 * @brief      YAML output archive for a wide character stream.
 *
 * Don't use this class directly. Instead, use boost::archive::yaml_woarchive.
 *
 * @tparam     Archive  The derived archive class.
 */
template <class Archive>
class BOOST_SYMBOL_VISIBLE yaml_woarchive_impl
    : public basic_text_oprimitive<std::wostream>,
      public basic_yaml_oarchive<Archive>
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
  public:
#else
  protected:
    friend class detail::interface_oarchive<Archive>;
    friend class basic_yaml_oarchive<Archive>;
    friend class save_access;
#endif
    template <class T> void save(const T& t)
    {
        basic_text_oprimitive<std::wostream>::save(t);
    }
    void save(const version_type& t)
    {
        save(static_cast<const unsigned int>(t));
    }
    void save(const boost::serialization::item_version_type& t)
    {
        save(static_cast<const unsigned int>(t));
    }
    YAML_WARCHIVE_API void save(const char* t);
#ifndef BOOST_NO_INTRINSIC_WCHAR_T
    YAML_WARCHIVE_API void save(const wchar_t* t);
#endif
    YAML_WARCHIVE_API void save(const std::string& s);
#ifndef BOOST_NO_STD_WSTRING
    YAML_WARCHIVE_API void save(const std::wstring& ws);
#endif
    YAML_WARCHIVE_API
    yaml_woarchive_impl(std::wostream& os, unsigned int flags);
    YAML_WARCHIVE_API
    ~yaml_woarchive_impl();

  public:
    /**
     * @brief      Save bytes to the YAML archive.
     *
     * The binary data is realized as an unqouted base64 encoded string.
     *
     * @param[in]  address  The address
     * @param[in]  count    The number of bytes
     *
     * @warning You should **never** use this method. Instead, use
     * boost::serialization::make_binary_object. See the code example below.
     *
     * @code
     * #include <boost/archive/yaml_woarchive.hpp>
     * #include <boost/serialization/binary_object.hpp>
     * #include <boost/serialization/nvp.hpp>
     * #include <iostream>
     * int main()
     * {
     *     char data[150];
     *     boost::archive::yaml_woarchive yaml(std::wcout);
     *     using boost::serialization::make_nvp;
     *     using boost::serialization::make_binary_object;
     *     yaml << make_nvp("data", make_binary_object(data, sizeof(data)));
     *     return 0;
     * }
     * @endcode
     */
    YAML_WARCHIVE_API void save_binary(const void* address, std::size_t count);
};

/**
 * @brief      Concrete YAML output archive for a wide character stream.
 *
 * Use this class to output your classes to a YAML archive.
 * Do not derive from this class.  If you want to extend this functionality via
 * inheritance, derive from yaml_woarchive_impl instead. This will preserve
 * correct static polymorphism.
 *
 * @see boost::archive::yaml_wiarchive for its input counter-part.
 *
 * @code
 * #include <boost/archive/yaml_woarchive.hpp>
 * #include <boost/serialization/nvp.hpp>
 * #include <iostream>
 * #include <string>
 * int main()
 * {
 *     std::wstring world("world");
 *     boost::archive::yaml_woarchive yaml(std::wcout);
 *     yaml << boost::serialization::make_nvp("hello", world);
 *     return 0;
 * }
 * @endcode
 */
#if BOOST_VERSION < 105600
class BOOST_SYMBOL_VISIBLE yaml_woarchive
    : public yaml_woarchive_impl<yaml_woarchive>,
      public detail::shared_ptr_helper
{
#else
class BOOST_SYMBOL_VISIBLE yaml_woarchive
    : public yaml_woarchive_impl<yaml_woarchive>
{
#endif
  public:
    /**
     * @brief      Constructor.
     *
     * @param      os     The output stream
     * @param[in]  flags  Modifier flags
     *
     * @see http://www.boost.org/doc/libs/1_63_0/boost/archive/basic_archive.hpp
     * for the available flags.
     */
    yaml_woarchive(std::wostream& os, unsigned int flags = 0)
        : yaml_woarchive_impl<yaml_woarchive>(os, flags)
    {
    }
    ~yaml_woarchive() {}
};

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::yaml_woarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_YAML_OARCHIVE_HPP
