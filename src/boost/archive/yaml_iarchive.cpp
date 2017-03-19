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

#if (defined _MSC_VER) && (_MSC_VER == 1200)
#pragma warning(disable : 4786) // too long name, harmless warning
#endif

#define BOOST_ARCHIVE_SOURCE
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/yaml_iarchive.hpp>
#include <boost/serialization/config.hpp>

// explicitly instantiate for this type of yaml stream
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/impl/basic_yaml_iarchive.ipp>
#include <boost/archive/impl/yaml_iarchive_impl.ipp>

namespace boost {
namespace archive {

template class detail::archive_serializer_map<yaml_iarchive>;
template class basic_yaml_iarchive<yaml_iarchive>;
template class yaml_iarchive_impl<yaml_iarchive>;

} // namespace archive
} // namespace boost
