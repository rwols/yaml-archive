/** @file
 *
 * @brief Declares narrow concrete polymorphic output archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_POLYMORPHIC_YAML_OARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_YAML_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/archive/detail/polymorphic_oarchive_route.hpp>
#include <boost/archive/yaml_oarchive.hpp>
#include <boost/config.hpp>

namespace boost {
namespace archive {

/**
 * @brief A YAML narrow output archive suitable for DLL boundaries.
 */
typedef detail::polymorphic_oarchive_route<yaml_oarchive_impl<yaml_oarchive>>
    polymorphic_yaml_oarchive;

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::polymorphic_yaml_oarchive)

#endif // BOOST_ARCHIVE_POLYMORPHIC_YAML_OARCHIVE_HPP
