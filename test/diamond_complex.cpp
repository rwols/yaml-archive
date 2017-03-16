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

class EX1Level1
{
  public:
    EX1Level1() : i(0) {}
    EX1Level1(int i) : i(i) { m[i] = "text"; }

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX1Level1");
        ar << BOOST_SERIALIZATION_NVP(i);
        ar << BOOST_SERIALIZATION_NVP(m);
        ++diamond_save_count;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX1Level1");
        ar >> BOOST_SERIALIZATION_NVP(i);
        ar >> BOOST_SERIALIZATION_NVP(m);
        ++diamond_load_count;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    bool operator==(const EX1Level1& another) const
    {
        return i == another.i && m == another.m;
    }
    // make polymorphic by marking at least one function virtual
    virtual ~EX1Level1(){};

  private:
    int i;
    std::map<int, std::string> m;
};

// note: the default is for object tracking to be performed if and only
// if and object of the corresponding class is anywhere serialized
// through a pointer.  In this example, that doesn't occur so
// by default, the shared EX1Level1 object wouldn't normally be tracked.
// This would leave to multiple save/load operation of the data in
// this shared EX1Level1 class.  This wouldn't cause an error, but it would
// be a waste of time.  So set the tracking behavior trait of the EX1Level1
// class to always track serialized objects of that class.  This permits
// the system to detect and elminate redundent save/load operations.
// (It is concievable that this might someday be detected automatically
// but for now, this is not done so we have to rely on the programmer
// to specify this trait)
BOOST_CLASS_TRACKING(EX1Level1, track_always)

class EX1Level2_A : virtual public EX1Level1
{
  public:
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX1Level2_A");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level1);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX1Level2_A");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level1);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX1Level2_B : virtual public EX1Level1
{
  public:
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX1Level2_B");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level1);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX1Level2_B");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level1);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX1Level3_A : public EX1Level2_A, public EX1Level2_B
{
  public:
    EX1Level3_A() {}
    EX1Level3_A(int i) : EX1Level1(i) {}

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX1Level3_A");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_A);
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_B);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX1Level3_A");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_A);
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_B);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX1Level3_B : public EX1Level2_A, public EX1Level2_B
{
  public:
    EX1Level3_B() {}
    EX1Level3_B(int) {}

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX1Level3_B");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_A);
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_B);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX1Level3_B");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_A);
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level2_B);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX1Level4 : public EX1Level3_B
{
  public:
    EX1Level4() {}
    EX1Level4(int i) : EX1Level1(i) {}

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX1Level4");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level3_B);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX1Level4");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX1Level3_B);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX2Level1
{
  public:
    EX2Level1() : i(0) {}
    EX2Level1(int i) : i(i) { m[i] = "text"; }

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX2Level1");
        ar << BOOST_SERIALIZATION_NVP(i);
        ar << BOOST_SERIALIZATION_NVP(m);
        ++diamond_save_count;
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX2Level1");
        ar >> BOOST_SERIALIZATION_NVP(i);
        ar >> BOOST_SERIALIZATION_NVP(m);
        ++diamond_load_count;
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    bool operator==(const EX2Level1& another) const
    {
        return i == another.i && m == another.m;
    }
    // make polymorphic by marking at least one function virtual
    virtual ~EX2Level1(){};

  private:
    int i;
    std::map<int, std::string> m;
};

// note: the default is for object tracking to be performed if and only
// if and object of the corresponding class is anywhere serialized
// through a pointer.  In this example, that doesn't occur so
// by default, the shared EX2Level1 object wouldn't normally be tracked.
// This would leave to multiple save/load operation of the data in
// this shared EX2Level1 class.  This wouldn't cause an error, but it would
// be a waste of time.  So set the tracking behavior trait of the EX2Level1
// class to always track serialized objects of that class.  This permits
// the system to detect and elminate redundent save/load operations.
// (It is concievable that this might someday be detected automatically
// but for now, this is not done so we have to rely on the programmer
// to specify this trait)
BOOST_CLASS_TRACKING(EX2Level1, track_always)

class EX2Level2_A : virtual public EX2Level1
{
  public:
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX2Level2_A");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level1);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX2Level2_A");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level1);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX2Level2_B : virtual public EX2Level1
{
  public:
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX2Level2_B");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level1);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX2Level2_B");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level1);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX2Level3_A : public EX2Level2_A, public EX2Level2_B
{
  public:
    EX2Level3_A() {}
    EX2Level3_A(int i) : EX2Level1(i) {}

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX2Level3_A");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_A);
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_B);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX2Level3_A");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_A);
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_B);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX2Level3_B : public EX2Level2_A, public EX2Level2_B
{
  public:
    EX2Level3_B() {}
    EX2Level3_B(int i) : EX2Level1(i) {}

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX2Level3_B");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_A);
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_B);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX2Level3_B");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_A);
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level2_B);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

class EX2Level4 : public EX2Level3_B
{
  public:
    EX2Level4() {}
    EX2Level4(int i) : EX2Level1(i) {}

    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        BOOST_TEST_MESSAGE("Saving EX2Level4");
        ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level3_B);
    }

    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        BOOST_TEST_MESSAGE("Restoring EX2Level4");
        ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(EX2Level3_B);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_EXPORT(EX1Level4)
BOOST_CLASS_EXPORT(EX1Level3_A)

BOOST_CLASS_EXPORT(EX2Level3_A)
BOOST_CLASS_EXPORT(EX2Level4)

BOOST_FIXTURE_TEST_CASE(diamond_complex1, io_count_fixture)
{
    diamond_save_count = 0;
    diamond_load_count = 0;

    const EX1Level3_A ex1L3a_save(3);
    const EX1Level1*  ex1L1_save = &ex1L3a_save;
    {
        output() << make_nvp("ex1L1", ex1L1_save);
    }
    EX1Level1* ex1L1_load;
    {
        input() >> make_nvp("ex1L1", ex1L1_load);
    }
    BOOST_CHECK(1 == diamond_save_count);
    BOOST_CHECK(1 == diamond_load_count);
    BOOST_CHECK(*ex1L1_save == *ex1L1_load);
}

BOOST_FIXTURE_TEST_CASE(diamond_complex2, io_count_fixture)
{
    diamond_save_count = 0;
    diamond_load_count = 0;

    const EX1Level4  ex1L4_save(3);
    const EX1Level1* ex1L1_save = &ex1L4_save;
    {
        output() << make_nvp("ex1L1", ex1L1_save);
    }
    EX1Level1* ex1L1_load;
    {
        input() >> make_nvp("ex1L1", ex1L1_load);
    }
    BOOST_CHECK(1 == diamond_save_count);
    BOOST_CHECK(1 == diamond_load_count);
    BOOST_CHECK(*ex1L1_save == *ex1L1_load);
}

BOOST_FIXTURE_TEST_CASE(diamond_complex3, io_count_fixture)
{
    diamond_save_count = 0;
    diamond_load_count = 0;

    const EX2Level3_A ex2L3a_save(3);
    const EX2Level1*  ex2L1_save = &ex2L3a_save;
    {
        output() << make_nvp("ex2L1", ex2L1_save);
    }
    EX2Level1* ex2L1_load;
    {
        input() >> make_nvp("ex2L1", ex2L1_load);
    }
    BOOST_CHECK(1 == diamond_save_count);
    BOOST_CHECK(1 == diamond_load_count);
    BOOST_CHECK(*ex2L1_save == *ex2L1_load);
}

BOOST_FIXTURE_TEST_CASE(diamond_complex4, io_count_fixture)
{
    diamond_save_count = 0;
    diamond_load_count = 0;

    const EX2Level4  ex2L4_save(3);
    const EX2Level1* ex2L1_save = &ex2L4_save;
    {
        output() << make_nvp("ex2L1", ex2L1_save);
    }
    EX2Level1* ex2L1_load;
    {
        input() >> make_nvp("ex2L1", ex2L1_load);
    }
    // {
    //     test_ostream  ofs(testfile, TEST_STREAM_FLAGS);
    //     test_oarchive oa(ofs);
    //     oa << boost::serialization::make_nvp("ex2L1", ex2L1_save);
    // }

    // {
    //     test_istream  ifs(testfile, TEST_STREAM_FLAGS);
    //     test_iarchive ia(ifs);
    //     ia >> boost::serialization::make_nvp("ex2L1", ex2L1_load);
    // }
    BOOST_CHECK(1 == diamond_save_count);
    BOOST_CHECK(1 == diamond_load_count);
    BOOST_CHECK(*ex2L1_save == *ex2L1_load);
}
