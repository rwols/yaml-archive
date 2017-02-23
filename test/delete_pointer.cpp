#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

#define NVP(name) BOOST_SERIALIZATION_NVP(name)

/**
 * @brief      Holds a pointer to another delete_pointer_, but doesn't own the
 *             pointer.
 */
class delete_pointer_
{
    friend class boost::serialization::access;
    template <class Archive>
    void save(Archive& ar, const unsigned int /* file_version */) const
    {
        ar << NVP(next_);
    }
    template <class Archive>
    void load(Archive& ar, const unsigned int /* file_version */)
    {
        ar >> NVP(next_);
        ++loadcount;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
  public:
    delete_pointer_()
    {
        if (test && objcount == 3)
            boost::serialization::throw_exception(
                boost::archive::archive_exception(
                    boost::archive::archive_exception::no_exception));
        next_ = 0;
        ++objcount;
    }
    ~delete_pointer_()
    {
        delete next_;
        --objcount;
    }
    delete_pointer_* next_;
    static int       objcount;
    static bool      test;
    static int       loadcount;
};

int  delete_pointer_::objcount = 0;
int  delete_pointer_::loadcount = 0;
bool delete_pointer_::test = false;

BOOST_FIXTURE_TEST_CASE(delete_pointer, io_fixture)
{
    delete_pointer_* head = new delete_pointer_;
    delete_pointer_* last = head;
    unsigned int     i;

    for (i = 1; i < 9; ++i)
    {
        delete_pointer_* a = new delete_pointer_;
        last->next_ = a;
        last = a;
    }

    // output the list
    {
        output() << NVP(head);
    }

    delete head;
    BOOST_CHECK_EQUAL(0, delete_pointer_::objcount);

    head = NULL;
    delete_pointer_::test = true;
    // read the list back
    test_istream  is(get_filename().c_str(), STREAM_FLAGS);
    test_iarchive ia(is, ARCHIVE_FLAGS);
    try
    {
        ia >> NVP(head);
    }
    catch (...)
    {
        ia.delete_created_pointers();
    }

    // identify the leaks
    BOOST_CHECK_EQUAL(0, delete_pointer_::loadcount);
}
