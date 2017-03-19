/** @file
 *
 * @brief Declares wide concrete polymorphic output archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#ifndef BOOST_ARCHIVE_POLYMORPHIC_YAML_WOARCHIVE_HPP
#define BOOST_ARCHIVE_POLYMORPHIC_YAML_WOARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/config.hpp>
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <boost/archive/detail/polymorphic_oarchive_route.hpp>
#include <boost/archive/yaml_woarchive.hpp>

namespace boost {
namespace archive {

/**
 * @brief A YAML wide output archive suitable for DLL boundaries.
 */
typedef detail::polymorphic_oarchive_route<yaml_woarchive_impl<yaml_woarchive>>
    polymorphic_yaml_woarchive;

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::polymorphic_yaml_woarchive)

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // BOOST_ARCHIVE_POLYMORPHIC_YAML_WOARCHIVE_HPP
