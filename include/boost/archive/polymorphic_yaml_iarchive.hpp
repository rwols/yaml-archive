#pragma once

#include <boost/archive/detail/polymorphic_iarchive_route.hpp>
#include <boost/archive/yaml_iarchive.hpp>
#include <boost/config.hpp>

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable : 4511 4512)
#endif

namespace boost {
namespace archive {

class polymorphic_yaml_iarchive
    : public detail::polymorphic_iarchive_route<yaml_iarchive>
{
  public:
    polymorphic_yaml_iarchive(std::istream& is, unsigned int flags = 0)
        : detail::polymorphic_iarchive_route<yaml_iarchive>(is, flags)
    {
    }
    ~polymorphic_yaml_iarchive() override {}
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::polymorphic_yaml_iarchive)
