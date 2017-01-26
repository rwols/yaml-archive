#define BOOST_ARCHIVE_SOURCE
#include <YAML/OutputArchive.hpp>
#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/serialization/config.hpp>

using namespace YAML;

OutputArchive::OutputArchive(std::ostream& output, const unsigned flags)
    : BasicOutputArchive<OutputArchive>(output, flags)
{
}

void OutputArchive::save_binary(void* address, std::size_t count)
{
    std::string key("__binary");
    key.append(std::to_string(m_binary_count));
    key.append("__");
    ++m_binary_count;
    save_binary(key.c_str(), address, count);
}

void OutputArchive::save_binary(const char* key, void* address,
                                std::size_t count)
{
    m_emit << Key << key << Value
           << Binary(reinterpret_cast<const unsigned char*>(address), count);
}

#include <boost/archive/impl/archive_serializer_map.ipp>

template class boost::archive::detail::archive_serializer_map<OutputArchive>;
