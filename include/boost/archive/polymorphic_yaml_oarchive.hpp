#pragma once

#include <boost/archive/detail/polymorphic_oarchive_route.hpp>
#include <boost/archive/yaml_oarchive.hpp>
#include <boost/config.hpp>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4511 4512)
#endif

namespace boost {
namespace archive {

class polymorphic_yaml_oarchive
    : public detail::polymorphic_oarchive_route<yaml_oarchive>
{
  public:
    polymorphic_yaml_oarchive(std::ostream& os, unsigned int flags = 0)
        : detail::polymorphic_oarchive_route<yaml_oarchive>(os, flags)
    {
    }
    ~polymorphic_yaml_oarchive() override {}

    // special treatment for name-value pairs.
    template <class T> void save_override(const serialization::nvp<T>& t)
    {
        yaml_oarchive::save_override(t);
    }
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::polymorphic_yaml_oarchive);
