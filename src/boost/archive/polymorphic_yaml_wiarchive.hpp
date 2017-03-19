/** @file
 *
 * @brief Declares wide concrete polymorphic input archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_POLYMORPHIC_YAML_WIARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_YAML_WIARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <boost/archive/detail/polymorphic_iarchive_route.hpp>
#include <boost/archive/yaml_wiarchive.hpp>

namespace boost {
namespace archive {

/**
 * @brief      A YAML wide input archive suitable for DLL boundaries.
 */
class polymorphic_yaml_wiarchive
    : public detail::polymorphic_iarchive_route<yaml_wiarchive>
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
    polymorphic_yaml_wiarchive(std::wistream& is, unsigned int flags = 0)
        : detail::polymorphic_iarchive_route<yaml_wiarchive>(is, flags)
    {
    }
    ~polymorphic_yaml_wiarchive() {}
};

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::polymorphic_yaml_wiarchive)

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_POLYMORPHIC_YAML_WIARCHIVE_HPP
