#include "polymorphic_io_fixture.hpp"
#include <boost/test/unit_test.hpp>

polymorphic_io_fixture::polymorphic_io_fixture()
    : m_filename(boost::unit_test::framework::current_test_case().p_name)
{
    m_filename += ".yml";
}

polymorphic_io_fixture::outputter::outputter(const char* filename)
    : m_os(filename, STREAM_FLAGS), m_oa(m_os, ARCHIVE_FLAGS)
{
}

polymorphic_io_fixture::inputter::inputter(const char* filename)
    : m_is(filename, STREAM_FLAGS), m_ia(m_is, ARCHIVE_FLAGS)
{
}

polymorphic_io_fixture::outputter polymorphic_io_fixture::output()
{
    return {m_filename.c_str()};
}

polymorphic_io_fixture::inputter polymorphic_io_fixture::input()
{
    return {m_filename.c_str()};
}
