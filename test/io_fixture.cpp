#include "io_fixture.hpp"
#include <boost/test/unit_test.hpp>

io_fixture::io_fixture()
    : m_filename(boost::unit_test::framework::current_test_case().p_name)
{
    m_filename += ".yml";
}

io_fixture::outputter::outputter(const char* filename)
    : m_os(filename, STREAM_FLAGS), m_oa(m_os, ARCHIVE_FLAGS)
{
}

io_fixture::inputter::inputter(const char* filename)
    : m_is(filename, STREAM_FLAGS), m_ia(m_is, ARCHIVE_FLAGS)
{
}

io_fixture::outputter io_fixture::output() { return {m_filename.c_str()}; }

io_fixture::inputter io_fixture::input() { return {m_filename.c_str()}; }
