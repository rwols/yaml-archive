/** @file
 *
 * @brief Defines narrow concrete output archives.
 *
 * @author    Raoul Wols
 *
 * @date      2017
 *
 * @copyright See LICENSE.md
 *
 */

#if (defined _MSC_VER) && (_MSC_VER == 1200)
#pragma warning(disable : 4786) // too long name, harmless warning
#endif

#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/yaml_oarchive.hpp>
#include <boost/serialization/config.hpp>

// explicitly instantiate for this type of yaml stream
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/impl/basic_yaml_oarchive.ipp>
#include <boost/archive/impl/yaml_oarchive_impl.ipp>

namespace boost {
namespace archive {

template class detail::archive_serializer_map<yaml_oarchive>;
template class basic_yaml_oarchive<yaml_oarchive>;
template class yaml_oarchive_impl<yaml_oarchive>;

} // namespace archive
} // namespace boost
