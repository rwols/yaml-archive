/** @file
 *
 * @brief Defines wide concrete output archives.
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

#include <boost/config.hpp>
#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#define BOOST_WARCHIVE_SOURCE
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/yaml_woarchive.hpp>
#include <boost/serialization/config.hpp>

// explicitly instantiate for this type of text stream
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/impl/basic_yaml_oarchive.ipp>
#include <boost/archive/impl/yaml_woarchive_impl.ipp>

namespace boost {
namespace archive {

template class detail::archive_serializer_map<yaml_woarchive>;
template class basic_yaml_oarchive<yaml_woarchive>;
template class yaml_woarchive_impl<yaml_woarchive>;

} // namespace archive
} // namespace boost

#endif // BOOST_NO_STD_WSTREAMBUF
