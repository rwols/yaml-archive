#define BOOST_ARCHIVE_SOURCE
#include <YAML/InputArchive.hpp>

using namespace YAML;

InputArchive::InputArchive(std::istream& input, const unsigned flags)
    : BasicInputArchive<InputArchive>(input, flags)
{
    /* Empty */
}

void InputArchive::load_binary(void* address, std::size_t count) {}

#include <boost/archive/impl/archive_serializer_map.ipp>

template class boost::archive::detail::archive_serializer_map<InputArchive>;
