#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

/**
 * @brief      Class serialized with a version number
 */
class with_version_number_
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /*ar*/, const unsigned int file_version)
    {
        // verify at execution that the version number corresponds to the saved
        // one
        BOOST_CHECK_EQUAL(file_version, 4);
        ++count;
    }

  public:
    unsigned int count;
    with_version_number_() : count(0) {}
};

BOOST_CLASS_IMPLEMENTATION(with_version_number_,
                           ::boost::serialization::object_class_info)

BOOST_CLASS_VERSION(with_version_number_, 4)

#define NVP(name) BOOST_SERIALIZATION_NVP(name)

BOOST_FIXTURE_TEST_CASE(with_version_number, io_fixture)
{
    with_version_number_  a;
    with_version_number_* ptr = &a;
    {
        output() << NVP(ptr) << NVP(ptr);
    }
    {
        input() >> NVP(ptr) >> NVP(ptr);
    }
    BOOST_CHECK_EQUAL(a.count, 1);
}
