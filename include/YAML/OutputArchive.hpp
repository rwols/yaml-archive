#pragma once

#include <YAML/BasicOutputArchive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <ostream>

#include <YAML/nsbegin.hpp>

/**
 * @brief      Serializes to YAML streams with the `<<` operator.
 *
 * @detail     Be sure to wrap all of your variables to be serialized into the
 *             BOOST_SERIALIZATION_NVP macro, or use the function
 *             boost::serialization::make_nvp.
 */
class OutputArchive : public BasicOutputArchive<OutputArchive>
{
  public:
    /**
     * @brief      Construct an OutputArchive ready for use.
     *
     * @param[out] output  The stream to write YAML to.
     * @param[in]  flags   The flags.
     * @param[in]  tag     The tag that will be displayed in the `%TAG`
     *                     directive at the "top" of the `output` stream. If
     *                     `ArchiveFlag::NoHeader` is present in the `flags`
     *                     variable, then this information is suppressed.
     */
    OutputArchive(std::ostream& output,
                  const ArchiveFlag flags = ArchiveFlag::Null,
                  const char* tag = "boost_serialization");
    ~OutputArchive() noexcept = default;

    /**
     * @brief      Writes binary data to the stream (in a YAML format).
     *
     * @detail     The binary data is saved in base64 format. The key shall be
     *             of the form __binaryX__, where X is the number of times that
     *             this method is called. This key-value pair shall always be
     *             at the root of the YAML document.
     *
     * @param      address  The memory address read the bytes from.
     * @param[in]  count    The number of bytes.
     */
    void save_binary(void* address, std::size_t count);

    /**
     * @detail     The binary data is saved in base64 format. The key shall be
     *             equal to `key`. This key-value pair shall always be at the
     *             root of the YAML document.
     *
     * @param[in]  key      The key of the key-value pair.
     * @param      address  The memory address read the bytes from.
     * @param[in]  count    The number of bytes.
     */
    void save_binary(const char* key, void* address, std::size_t count);

  private:
    friend class boost::archive::save_access;
    std::size_t m_binary_count = 0;
};

#include <YAML/nsend.hpp>

BOOST_SERIALIZATION_REGISTER_ARCHIVE(YAML::OutputArchive);
