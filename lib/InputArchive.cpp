#include <YAML/InputArchive.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <climits>
#include <functional>
#include <string>
#include <yaml-cpp/node/parse.h>

using namespace YAML;

InputArchive::InputArchive(std::istream& input, const unsigned flags)
    : BasicInputArchive<InputArchive>(input, flags)
{
    /* Empty */
}

void InputArchive::load_binary(void* address, std::size_t count) {}

#include <boost/archive/impl/archive_serializer_map.ipp>

template class boost::archive::detail::archive_serializer_map<InputArchive>;
