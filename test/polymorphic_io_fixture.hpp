#pragma once

#pragma once

#include <boost/test/test_tools.hpp>
#include <boost/version.hpp>

#include "polymorphic_config.hpp" // generated by cmake, see config.hpp.in

class polymorphic_io_fixture
{
  protected:
    polymorphic_io_fixture();

    class outputter
    {
      public:
        template <class T> outputter& operator<<(const T& t)
        {
            m_oa << t;
            return *this;
        }

      private:
        outputter(const char* filename);
        friend class polymorphic_io_fixture;

        test_ostream  m_os;
        test_oarchive m_oa;
    };

    class inputter
    {
      public:
        template <class T> inputter& operator>>(T& t)
        {
            m_ia >> t;
            return *this;
        }

      private:
        inputter(const char* filename);
        friend class polymorphic_io_fixture;

        test_istream  m_is;
        test_iarchive m_ia;
    };

    // Note: you have to chain all of the "<<" operators because otherwise
    // the test_oarchive and test_ostream destructors get called, in which case
    // all the serialized contents is flushed to the stream.
    outputter output();

    // Note: you have to chain all of the ">>" operators because otherwise
    // the test_iarchive and test_istream destructors get called.
    inputter input();

    const std::string& get_filename() { return m_filename; }

  private:
    std::string m_filename;
};
