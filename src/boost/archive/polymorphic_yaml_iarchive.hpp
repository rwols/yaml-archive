/** @file
 *
 * @brief Declares narrow concrete polymorphic input archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_POLYMORPHIC_YAML_IARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_YAML_IARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/archive/detail/polymorphic_iarchive_route.hpp>
#include <boost/archive/yaml_iarchive.hpp>
#include <boost/config.hpp>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4511 4512)
#endif

namespace boost {
namespace archive {

/**
 * @brief      A YAML narrow input archive suitable for DLL boundaries.
 */
class polymorphic_yaml_iarchive
    : public detail::polymorphic_iarchive_route<yaml_iarchive>
{
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
    polymorphic_yaml_iarchive(std::istream& is, unsigned int flags = 0)
        : detail::polymorphic_iarchive_route<yaml_iarchive>(is, flags)
    {
    }
    ~polymorphic_yaml_iarchive() {}
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::polymorphic_yaml_iarchive)

#endif // BOOST_ARCHIVE_POLYMORPHIC_YAML_IARCHIVE_HPP
