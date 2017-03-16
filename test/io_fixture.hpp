#pragma once

#include "config.hpp"
#include <boost/test/test_tools.hpp>
#include <boost/version.hpp>

class io_fixture
{
  protected:
    io_fixture();

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
        friend class io_fixture;

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
        friend class io_fixture;

        test_istream  m_is;
        test_iarchive m_ia;
    };

    outputter output();

    inputter input();

    const wchar_t* get_wstring_sample(const std::size_t index) const noexcept;

    std::size_t get_wstring_sample_count() const noexcept;

    const wchar_t* get_random_wstring_sample() const noexcept;

    const char* get_string_sample(const std::size_t index) const noexcept;

    std::size_t get_string_sample_count() const noexcept;

    const char* get_random_string_sample() const noexcept;

    template <class T> void check_roundtrip(const T& t1)
    {
        using boost::serialization::make_nvp;
        T t2;
        {
            output() << make_nvp("t", t1);
        }
        {
            input() >> make_nvp("t", t2);
        }
        BOOST_CHECK_EQUAL(t1, t2);
    }

    template <class T> void check_roundtrip_collection(const T& t1)
    {
        using boost::serialization::make_nvp;
        T t2;
        {
            output() << make_nvp("t", t1);
        }
        {
            input() >> make_nvp("t", t2);
        }
#if BOOST_VERSION > 105800
        BOOST_TEST(t1 == t2, boost::test_tools::per_element());
#else
        BOOST_CHECK_EQUAL(t1.size(), t2.size());
        auto i1 = t1.begin();
        auto i2 = t2.begin();
        while (i1 != t1.end())
        {
            BOOST_CHECK_EQUAL(*i1, *i2);
            ++i1;
            ++i2;
        }
#endif
    }

    const std::string& get_filename() { return m_filename; }

  private:
    std::string m_filename;
};
