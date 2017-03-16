#include "io_count_fixture.hpp"
#include <boost/serialization/export.hpp>
#include <boost/serialization/map.hpp>
#include <boost/test/unit_test.hpp>

#define NVP(name) BOOST_SERIALIZATION_NVP(name)
using boost::serialization::make_nvp;

namespace {
// used to detect when base_diamond class is saved multiple times
int diamond_save_count = 0;
// used to detect when base_diamond class is loaded multiple times
int diamond_load_count = 0;
}

class base_diamond
{
  public:
    base_diamond() : i(0) {}
    base_diamond(int i) : i(i) { m[i] = "text"; }

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving base_diamond");
        ar << NVP(i);
        ar << NVP(m);
        ++diamond_save_count;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring base_diamond");
        ar >> NVP(i);
        ar >> NVP(m);
        ++diamond_load_count;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    bool operator==(const base_diamond& another) const
    {
        return i == another.i && m == another.m;
    }
    // make polymorphic by marking at least one function virtual
    virtual ~base_diamond(){};

  private:
    int i;
    std::map<int, std::string> m;
};

// note: the default is for object tracking to be performed if and only
// if and object of the corresponding class is anywhere serialized
// through a pointer.  In this example, that doesn't occur so
// by default, the shared base object wouldn't normally be tracked.
// This would leave to multiple save/load operation of the data in
// this shared base class.  This wouldn't cause an error, but it would
// be a waste of time.  So set the tracking behavior trait of the base
// class to always track serialized objects of that class.  This permits
// the system to detect and elminate redundent save/load operations.
// (It is concievable that this might someday be detected automatically
// but for now, this is not done so we have to rely on the programmer
// to specify this trait)
BOOST_CLASS_TRACKING(base_diamond, track_always)

class derived1_diamond : virtual public base_diamond
{
  public:
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving derived1_diamond");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_diamond);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring derived1_diamond");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_diamond);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class derived2_diamond : virtual public base_diamond
{
  public:
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving derived2_diamond");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_diamond);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring derived2_diamond");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(base_diamond);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class final_diamond : public derived1_diamond, public derived2_diamond
{
  public:
    final_diamond() {}
    final_diamond(int i) : base_diamond(i) {}

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving final_diamond");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(derived1_diamond);
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(derived2_diamond);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring final_diamond");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(derived1_diamond);
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(derived2_diamond);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_EXPORT(final_diamond)

BOOST_FIXTURE_TEST_CASE(diamond1, io_count_fixture)
{
    diamond_save_count = 0;
    diamond_load_count = 0;
    const final_diamond b(3);
    {
        output() << NVP(b);
    }
    final_diamond b2;
    {
        input() >> make_nvp("b", b2);
    }
    BOOST_CHECK(1 == diamond_save_count);
    BOOST_CHECK(1 == diamond_load_count);
    BOOST_CHECK(b2 == b);
}

BOOST_FIXTURE_TEST_CASE(diamond2, io_count_fixture)
{
    // do the same test with pointers (same as diamond1 test)
    diamond_save_count = 0;
    diamond_load_count = 0;
    const base_diamond* bp = new final_diamond(3);
    {
        output() << NVP(bp);
    }
    base_diamond* bp2;
    {
        input() >> make_nvp("bp", bp2);
    }
    BOOST_CHECK(1 == diamond_save_count);
    BOOST_CHECK(1 == diamond_load_count);
    BOOST_CHECK(*bp2 == *bp);
}
