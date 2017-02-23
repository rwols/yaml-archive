/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// test_non_default_ctor2.cpp

// (C) Copyright 2002 Martin Ecker.
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// should pass compilation and execution

// this tests complex usage of non-default constructor.  Specifically
// the case where a constructor serializes a pointer member.

#include "io_fixture.hpp"
#include <boost/config.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

class IntValueHolder
{
  public:
    IntValueHolder() : value(0) {}

    IntValueHolder(int newvalue) : value(newvalue) {}

    int GetValue() const { return value; }

    inline bool operator==(const IntValueHolder& other) const
    {
        return value == other.value;
    }
    inline bool operator!=(const IntValueHolder& other) const
    {
        return !operator==(other);
    }

  private:
    int value;

    friend std::ostream& operator<<(std::ostream&, const IntValueHolder&);
    friend class boost::serialization::access;

    template <class ArchiveT>
    void serialize(ArchiveT& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(value);
    }
};

class FloatValueHolder
{
  public:
    FloatValueHolder() : value(0) {}

    FloatValueHolder(float newvalue) : value(newvalue) {}

    float GetValue() const { return value; }

    inline bool operator==(const FloatValueHolder& other) const
    {
        return value == other.value;
    }

  private:
    float value;

    friend std::ostream& operator<<(std::ostream&, const FloatValueHolder&);
    friend class boost::serialization::access;

    template <class ArchiveT>
    void serialize(ArchiveT& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(value);
    }
};

class SomeClass
{
  public:
    SomeClass(const IntValueHolder& initialValue)
        : value(initialValue), floatValue(new FloatValueHolder(10.0f))
    {
    }

    ~SomeClass() { delete floatValue; }

    IntValueHolder    value;
    FloatValueHolder* floatValue;

    inline bool operator==(const SomeClass& other) const
    {
        if (value != other.value) return false;
        if (floatValue == nullptr && other.floatValue != nullptr) return false;
        if (floatValue != nullptr && other.floatValue == nullptr) return false;
        if (floatValue == nullptr && other.floatValue == nullptr) return true;
        return *floatValue == *other.floatValue;
    }

  private:
    friend std::ostream& operator<<(std::ostream&, const SomeClass&);
    friend class boost::serialization::access;

    template <class ArchiveT>
    void serialize(ArchiveT& ar, const unsigned int /* file_version */)
    {
        ar& BOOST_SERIALIZATION_NVP(floatValue);
    }
};

std::ostream& operator<<(std::ostream& os, const IntValueHolder& holder)
{
    return os << "<IntValueHolder, value: " << holder.value << ">";
}

std::ostream& operator<<(std::ostream& os, const FloatValueHolder& holder)
{
    return os << "<FloatValueHolder, value: " << holder.value << ">";
}

std::ostream& operator<<(std::ostream& os, const SomeClass& c)
{
    os << "<SomeClass, value: " << c.value << ", floatValue: ";
    if (c.floatValue == nullptr)
    {
        os << "nullptr";
    }
    else
    {
        os << *c.floatValue;
    }
    return os << ">";
}

namespace boost {
namespace serialization {

template <class ArchiveT>
void save_construct_data(ArchiveT& archive, const SomeClass* p,
                         const unsigned int /*version*/
                         )
{
    archive& boost::serialization::make_nvp("initialValue", p->value);
}

template <class ArchiveT>
void load_construct_data(ArchiveT& archive, SomeClass* p,
                         const unsigned int /*version*/
                         )
{
    IntValueHolder initialValue;
    archive&       boost::serialization::make_nvp("initialValue", initialValue);

    ::new (p) SomeClass(initialValue);
}

} // serialization
} // namespace boost

BOOST_FIXTURE_TEST_CASE(non_default_ctor2, io_fixture)
{
    SomeClass* a = new SomeClass(5);
    {
        output() << BOOST_SERIALIZATION_NVP(a);
    }
    SomeClass* a_new;
    {
        input() >> boost::serialization::make_nvp("a", a_new);
    }
    BOOST_CHECK_EQUAL(*a, *a_new);
    // ???
    // BOOST_CHECK_EQUAL(a, a_new);
    delete a;
    delete a_new;
}
