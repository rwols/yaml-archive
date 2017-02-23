#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

/**
 * @brief      Class serialized without a version number
 */
class without_version_number_
{
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& /*ar*/, const unsigned int file_version)
    {
        // class that don't save class info always have a version number of 0
        BOOST_CHECK_EQUAL(file_version, 0);
        BOOST_STATIC_ASSERT(
            0 ==
            ::boost::serialization::version<without_version_number_>::value);
        ++count;
    }

  public:
    unsigned int count;
    without_version_number_() : count(0) {}
};

BOOST_CLASS_IMPLEMENTATION(without_version_number_,
                           ::boost::serialization::object_serializable)
BOOST_CLASS_TRACKING(without_version_number_,
                     ::boost::serialization::track_never)

#define NVP(name) BOOST_SERIALIZATION_NVP(name)

BOOST_FIXTURE_TEST_CASE(without_version_number, io_fixture)
{
    without_version_number_ a;
    {
        output() << NVP(a) << NVP(a);
    }
    {
        input() >> NVP(a) >> NVP(a);
    }
    BOOST_CHECK_EQUAL(a.count, 4); // no tracking => redundant loads
}
