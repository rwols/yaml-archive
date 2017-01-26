/**
 * @author Raoul Wols
 *
 * @file   Defines the InputArchive class.
 */

#pragma once

#include <YAML/BasicInputArchive.hpp>
#include <boost/archive/detail/register_archive.hpp>

#include <YAML/nsbegin.hpp>

/**
 * @brief      Deserializes YAML streams with the `>>` operator.
 *
 * @detail     Be sure to wrap all of your variables to be deserialized into the
 *             BOOST_SERIALIZATION_NVP macro, or use the function
 *             boost::serialization::make_nvp.
 */
class InputArchive : public BasicInputArchive<InputArchive>
{
  public:
    /**
     * @brief      Construct an InputArchive ready for use.
     *
     * @param      input  The input stream. Must contain valid YAML serialized
     *                    from an OutputArchive.
     * @param[in]  flags  The flags. TODO!
     */
    InputArchive(std::istream& input, const unsigned flags = 0);

    ~InputArchive() noexcept = default;

    /**
     * @brief      Loads binary data from the stream.
     *
     * @param      address  The memory address to store the contents to.
     * @param[in]  count    The number of bytes.
     */
    void load_binary(void* address, std::size_t count);

  private:
    friend class boost::archive::load_access;
    std::size_t m_binary_count = 0;
};

#include <YAML/nsend.hpp>

BOOST_SERIALIZATION_REGISTER_ARCHIVE(YAML::InputArchive);
