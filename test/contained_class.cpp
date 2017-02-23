#include "B.hpp"
#include "io_fixture.hpp"
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/test/unit_test.hpp>

/**
 * @brief      Class with another class as a member variable
 */
class contained
{
  private:
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(b);
    }
    B b;

  public:
    bool operator==(const contained& rhs) const { return b == rhs.b; }
    friend std::ostream& operator<<(std::ostream&, const contained&);
    contained() {}
};

std::ostream& operator<<(std::ostream& os, const contained& c)
{
    return os << c.b;
}

BOOST_FIXTURE_TEST_CASE(contained_class, io_fixture)
{
    const contained c;
    check_roundtrip(c);
}
