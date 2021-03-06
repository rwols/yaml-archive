/** @file
 *
 * @brief Common base class for yaml_oarchive and yaml_woarchive.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_BASIC_YAML_OARCHIVE_HPP
#define BOOST_ARCHIVE_BASIC_YAML_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/yaml_decl.hpp>
#include <boost/config.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/serialization/collection_size_type.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

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
 * @brief      Write serialized objects to a YAML output stream.
 *
 * @warning    **Everything** that you want to stream to the archive must be
 *             wrapped inside boost::serialization::make_nvp or the
 *             BOOST_SERIALIZATION_NVP macro. Include the file
 *             boost/serialization/nvp.hpp for that.
 *
 * @tparam     Archive  The derived archive class
 */
template <class Archive>
class BOOST_SYMBOL_VISIBLE basic_yaml_oarchive
    : public detail::common_oarchive<Archive>
{
    // special stuff for yaml output
    unsigned int depth;
    const char*  m_pending_class_name = nullptr;
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
  public:
#else
  protected:
    friend class detail::interface_oarchive<Archive>;
#endif
    YAML_AWARCHIVE_API void indent();
    YAML_AWARCHIVE_API void init();
    YAML_AWARCHIVE_API void windup();
    // helpers used below
    YAML_AWARCHIVE_API void save_start(const char* name);
    YAML_AWARCHIVE_API void save_end(const char* name);
    YAML_AWARCHIVE_API void end_preamble();

#if BOOST_VERSION > 105800

    // Anything not an attribute and not a name-value pair is an
    // error and should be trapped here.
    template <class T> void save_override(T& t)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        BOOST_MPL_ASSERT((serialization::is_wrapper<T>));
        this->detail_common_oarchive::save_override(t);
    }

    // special treatment for name-value pairs.
    typedef detail::common_oarchive<Archive> detail_common_oarchive;
    template <class T> void save_override(const serialization::nvp<T>& t)
    {
        this->This()->save_start(t.name());
        this->detail_common_oarchive::save_override(t.const_value());
        this->This()->save_end(t.name());
    }

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    YAML_AWARCHIVE_API void save_override(const class_id_type& t);
    YAML_AWARCHIVE_API void save_override(const class_id_optional_type& t);
    YAML_AWARCHIVE_API void save_override(const class_id_reference_type& t);
    YAML_AWARCHIVE_API void save_override(const object_id_type& t);
    YAML_AWARCHIVE_API void save_override(const object_reference_type& t);
    YAML_AWARCHIVE_API void save_override(const version_type& t);
    YAML_AWARCHIVE_API void save_override(const class_name_type& t);
    YAML_AWARCHIVE_API void save_override(const tracking_type& t);

#else // BOOST_VERSION <= 105800

#ifdef _MSC_VER
#pragma message("Boost version is " BOOST_LIB_VERSION ". Consider upgrading!")
#endif

    // These overloads are required for versions of boost <= 1.58.

    template <class T> void save_override(T& t, BOOST_PFTO int)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        BOOST_MPL_ASSERT((serialization::is_wrapper<T>));
        this->detail_common_oarchive::save_override(t, 0);
    }

    typedef detail::common_oarchive<Archive> detail_common_oarchive;
    template <class T> void save_override(const serialization::nvp<T>& t, int)
    {
        this->This()->save_start(t.name());
        this->detail_common_oarchive::save_override(t.const_value(), 0);
        this->This()->save_end(t.name());
    }

    YAML_AWARCHIVE_API
    void save_override(const class_id_type& t, int);

    YAML_AWARCHIVE_API
    void save_override(const class_id_optional_type& t, int);

    YAML_AWARCHIVE_API
    void save_override(const class_id_reference_type& t, int);

    YAML_AWARCHIVE_API
    void save_override(const object_id_type& t, int);

    YAML_AWARCHIVE_API
    void save_override(const object_reference_type& t, int);

    YAML_AWARCHIVE_API
    void save_override(const version_type& t, int);

    YAML_AWARCHIVE_API
    void save_override(const class_name_type& t, int);

    YAML_AWARCHIVE_API
    void save_override(const tracking_type& t, int);

#endif // BOOST_VERSION

    YAML_AWARCHIVE_API basic_yaml_oarchive(unsigned int flags);
    YAML_AWARCHIVE_API ~basic_yaml_oarchive();
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // BOOST_ARCHIVE_BASIC_YAML_OARCHIVE_HPP
