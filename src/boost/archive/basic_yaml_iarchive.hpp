/** @file
 *
 * @brief Common base class for yaml_iarchive and yaml_wiarchive.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_BASIC_YAML_IARCHIVE_HPP
#define BOOST_ARCHIVE_BASIC_YAML_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/yaml_decl.hpp>
#include <boost/config.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/version.hpp>

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

/**
 * @brief      Read serialized objects from an input YAML stream.
 *
 * @warning    **Everything** that you want to read from the archive must be
 *             wrapped inside boost::serialization::make_nvp or the
 *             BOOST_SERIALIZATION_NVP macro. Include the file
 *             boost/serialization/nvp.hpp for that.
 *
 * @tparam     Archive  The derived archive class
 */
template <class Archive>
class BOOST_SYMBOL_VISIBLE basic_yaml_iarchive
    : public detail::common_iarchive<Archive>
{

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
  public:
#else // compiler is not totally broken
  protected:
#endif
    unsigned int depth;
    friend class detail::interface_iarchive<Archive>;

    YAML_AWARCHIVE_API void load_start(const char* name);
    YAML_AWARCHIVE_API void load_end(const char* name);

#if BOOST_VERSION >= 105800

    // Anything not an attribute and not a name-value pair is an
    // should be trapped here.
    template <class T> void load_override(T& t)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        BOOST_MPL_ASSERT((serialization::is_wrapper<T>));
        this->detail_common_iarchive::load_override(t);
    }

    // Anything not an attribute - see below - should be a name value
    // pair and be processed here
    typedef detail::common_iarchive<Archive> detail_common_iarchive;
    template <class T> void load_override(const boost::serialization::nvp<T>& t)
    {
        this->This()->load_start(t.name());
        this->detail_common_iarchive::load_override(t.value());
        this->This()->load_end(t.name());
    }

    // specific overrides for attributes - handle as
    // primitives. These are not name-value pairs
    // so they have to be intercepted here and passed on to load.
    // although the class_id is included in the yaml text file in order
    // to make the file self describing, it isn't used when loading
    // an yaml archive.  So we can skip it here.  Note: we MUST override
    // it otherwise it will be loaded as a normal primitive w/o tag and
    // leaving the archive in an undetermined state
    YAML_AWARCHIVE_API void load_override(class_id_type& t);
    YAML_AWARCHIVE_API void load_override(class_id_optional_type& /* t */) {}
    YAML_AWARCHIVE_API void load_override(object_id_type& t);
    YAML_AWARCHIVE_API void load_override(version_type& t);
    YAML_AWARCHIVE_API void load_override(tracking_type& t);
// class_name_type can't be handled here as it depends upon the
// char type used by the stream.  So require the derived implementation
// handle this.

#else // BOOST_VERSION < 105800

    // These overloads are required for versions of boost < 1.59.

    template <class T> void load_override(T& t, BOOST_PFTO int)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        BOOST_MPL_ASSERT((serialization::is_wrapper<T>));
        this->detail_common_iarchive::load_override(t, 0);
    }
    typedef detail::common_iarchive<Archive> detail_common_iarchive;
    template <class T>
    void load_override(const boost::serialization::nvp<T>& t, int)
    {
        this->This()->load_start(t.name());
        this->detail_common_iarchive::load_override(t.value(), 0);
        this->This()->load_end(t.name());
    }
    YAML_AWARCHIVE_API void load_override(class_id_type& t, int);
    YAML_AWARCHIVE_API void load_override(class_id_optional_type&, int) {}
    YAML_AWARCHIVE_API void load_override(object_id_type& t, int);
    YAML_AWARCHIVE_API void load_override(version_type& t, int);
    YAML_AWARCHIVE_API void load_override(tracking_type& t, int);

#endif // BOOST_VERSION

    YAML_AWARCHIVE_API
    basic_yaml_iarchive(unsigned int flags);
    YAML_AWARCHIVE_API
    ~basic_yaml_iarchive();
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // BOOST_ARCHIVE_BASIC_YAML_IARCHIVE_HPP
