#define BOOST_TEST_MODULE test1

#include <boost/archive/yaml_iarchive.hpp>
#include <boost/archive/yaml_oarchive.hpp>

#include <boost/serialization/map.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

struct Basic {
    int data;
    float more_data;
    std::string some_name;
    template <class Archive>
    void serialize(Archive& archive, const unsigned version)
    {
        archive& BOOST_SERIALIZATION_NVP(data) &
            BOOST_SERIALIZATION_NVP(more_data) &
            BOOST_SERIALIZATION_NVP(some_name);
    }

    bool operator==(const Basic& other) const noexcept
    {
        return data == other.data && more_data == other.more_data &&
               some_name == other.some_name;
    }
    bool operator!=(const Basic& other) const noexcept
    {
        return !operator==(other);
    }
};

std::ostream& operator<<(std::ostream& os, const Basic& b)
{
    return os << "<Basic obj with data: " << b.data
              << ", more_data: " << b.more_data << ", name: " << b.some_name
              << '>';
}

struct Y {
    std::string name;
    float temperature;

    template <class Archive>
    void serialize(Archive& archive, const unsigned version)
    {
        archive& BOOST_SERIALIZATION_NVP(name) &
            BOOST_SERIALIZATION_NVP(temperature);
    }

    bool operator==(const Y& other) const noexcept
    {
        return name == other.name && temperature == other.temperature;
    }
    bool operator!=(const Y& other) const noexcept
    {
        return !operator==(other);
    }
};

std::ostream& operator<<(std::ostream& os, const Y& y)
{
    return os << "<Y name: " << y.name << ", temp: " << y.temperature << '>';
}

struct X {
    int data;
    Y y;
    std::map<std::string, int> mymap;
    std::vector<float> myfloats;
    std::vector<Y> mywhys;

    template <class Archive>
    void serialize(Archive& archive, const unsigned version)
    {
        archive& BOOST_SERIALIZATION_NVP(data) & BOOST_SERIALIZATION_NVP(y) &
            BOOST_SERIALIZATION_NVP(mymap) & BOOST_SERIALIZATION_NVP(myfloats) &
            BOOST_SERIALIZATION_NVP(mywhys);
    }

    bool operator==(const X& other) const noexcept
    {
        return data == other.data && y == other.y && mymap == other.mymap &&
               myfloats == other.myfloats && mywhys == other.mywhys;
    }

    bool operator!=(const X& other) const noexcept
    {
        return !operator==(other);
    }
};

template <class First, class Second>
std::ostream& operator<<(std::ostream& os, const std::pair<First, Second>& x)
{
    os << "<std::pair first: " << x.first << ", second: " << x.second << '>';
    return os;
}

template <class T, class Alloc>
std::ostream& operator<<(std::ostream& os, const std::vector<T, Alloc>& x)
{
    os << "<std::vector values:";
    for (const auto& item : x) os << ' ' << item;
    return os << '>';
}

template <class Key, class T, class Compare, class Alloc>
std::ostream& operator<<(std::ostream& os,
                         const std::map<Key, T, Compare, Alloc>& x)
{
    os << "<std::map values:";
    for (const auto& kv : x) os << ' ' << kv;
    return os << '>';
}

std::ostream& operator<<(std::ostream& os, const X& x)
{
    os << "<X data: " << x.data;
    os << ", y: " << x.y;
    os << ", mymap: " << x.mymap;
    os << ", myfloats: " << x.myfloats;
    os << ", mywhys: " << x.mywhys;
    return os << '>';
}

BOOST_AUTO_TEST_CASE(signed_char)
{
    auto node = YAML::Load("-126");
    try {
        const auto y = node.as<signed char>();
        std::cout << "y = " << static_cast<int>(y) << '\n';
    }
    catch (const YAML::TypedBadConversion<signed char>& error) {
        std::cerr << error.what() << '\n';
    }

    signed char x;
    x = -126;
    {
        std::ofstream output("signed_char.yml");
        boost::archive::yaml_oarchive archive(output);
        boost::archive::yaml_oarchive couttest(std::cout);
        archive << BOOST_SERIALIZATION_NVP(x);
        couttest << BOOST_SERIALIZATION_NVP(x);
    }
    {
        std::ifstream input("signed_char.yml");
        auto node = YAML::Load(input);
        std::cout << YAML::Dump(node);
    }
    signed char another_x;
    {
        std::ifstream input("signed_char.yml");
        boost::archive::yaml_iarchive archive(input);
        archive >> boost::serialization::make_nvp("x", another_x);
    }
    BOOST_CHECK_EQUAL(x, another_x);
}

BOOST_AUTO_TEST_CASE(single_Y_struct)
{
    std::pair<Y, Y> p;
    p.first.name = "Katy Perry";
    p.first.temperature = 42.0f;
    p.second.name = "John Mayer";
    p.second.temperature = 123;
    {
        std::ofstream output("single_Y_struct.yml");
        boost::archive::yaml_oarchive archive(output);
        boost::archive::yaml_oarchive couttest(std::cout);
        archive << BOOST_SERIALIZATION_NVP(p);
        couttest << BOOST_SERIALIZATION_NVP(p);
    }
    {
        std::ifstream input("single_Y_struct.yml");
        auto node = YAML::Load(input);
        std::cout << YAML::Dump(node) << '\n';
    }
    std::pair<Y, Y> another_p;
    {
        std::ifstream input("single_Y_struct.yml");
        boost::archive::yaml_iarchive archive(input);
        archive >> boost::serialization::make_nvp("p", another_p);
    }
    BOOST_CHECK_EQUAL(p, another_p);
}

BOOST_AUTO_TEST_CASE(basic_struct_input_output)
{
    Basic b;
    b.data = 5;
    b.more_data = 3.141592f;
    b.some_name = "Hello, world!";
    {
        std::ofstream output("basic_struct_input_output.yml");
        boost::archive::yaml_oarchive archive(output);
        archive << BOOST_SERIALIZATION_NVP(b);
        boost::archive::yaml_oarchive couttest(std::cout);
        couttest << BOOST_SERIALIZATION_NVP(b);
    }
    Basic another_b;
    {
        std::ifstream input("basic_struct_input_output.yml");
        boost::archive::yaml_iarchive archive(input);
        try {
            archive >> boost::serialization::make_nvp("b", another_b);
        }
        catch (const std::exception& e) {

            // std::cerr << "bad key chain: ";
            // for (const auto& key : e.keys())
            // {
            //     std::cerr << key << " ";
            // }
            // std::cerr << "\n";
            BOOST_CHECK(false);
        }
    }
    BOOST_CHECK_EQUAL(b, another_b);
    std::cout << "OK!\n";
}

BOOST_AUTO_TEST_CASE(value_types_and_vectors_and_maps)
{
    X x;
    x.data = 42;
    x.y.name = "John Doe";
    x.y.temperature = 25.4f;
    x.mymap.emplace("one", 1);
    x.mymap.emplace("two", 2);
    x.mymap.emplace("minus one", -1);
    x.mywhys.emplace_back(Y());
    x.mywhys.emplace_back(Y());
    x.mywhys[0].name = "Kate";
    x.mywhys[0].temperature = 0;
    x.mywhys[1].name = "Bob";
    x.mywhys[1].temperature = 1123;

    x.myfloats = {1.0f, 2.0f, -0.3213f, -4329.432f};
    {
        std::ofstream output("value_types_and_vectors_and_maps.yml");
        boost::archive::yaml_oarchive archive(output);
        archive << BOOST_SERIALIZATION_NVP(x);
        boost::archive::yaml_oarchive couttest(std::cout);
        couttest << BOOST_SERIALIZATION_NVP(x);
    }
    X another_x;
    {
        std::ifstream input("value_types_and_vectors_and_maps.yml");
        boost::archive::yaml_iarchive archive(input);
        archive >> boost::serialization::make_nvp("x", another_x);
    }
    BOOST_CHECK_EQUAL(x, another_x);
    std::cout << "OK!\n";
}

BOOST_AUTO_TEST_CASE(binary_blobs)
{
    X x;
    {
        boost::archive::yaml_oarchive archive(std::cout);
        archive.save_binary(&x, sizeof(x));
    }
}

class A {
  public:
    int data = 0;

    A* one = nullptr;
    A* two = nullptr;
    A* three = nullptr;

    template <class Archive>
    void serialize(Archive& archive, const unsigned version)
    {
        archive& BOOST_SERIALIZATION_NVP(data) & BOOST_SERIALIZATION_NVP(one) &
            BOOST_SERIALIZATION_NVP(two) & BOOST_SERIALIZATION_NVP(three);
    }

    bool operator==(const A& other) const noexcept
    {
        return data == other.data && (one && other.one)
                   ? *one == *other.one
                   : true && (two && other.two) ? *two == *other.two
                                                : true && (three && other.three)
                                                      ? *three == *other.three
                                                      : true;
    }
};

std::ostream& operator<<(std::ostream& os, const A& a)
{
    return os << "<A, data: " << a.data << ", one: " << a.one
              << ", two: " << a.two << ", three: " << a.three << '>';
}

A* load_naked_pointer_reference()
{
    A* a0 = new A();
    A* a1 = new A();
    A* a2 = new A();
    a0->data = 42;
    a1->data = 1337;
    a2->data = 2017;
    a0->one = a1;
    a0->two = a2;
    a0->three = a1;
    a1->three = a2;
    return a0;
}

void save_naked_pointer(const char* filename)
{
    auto a = load_naked_pointer_reference();
    {
        std::ofstream output(filename);
        boost::archive::yaml_oarchive archive(output);
        archive << BOOST_SERIALIZATION_NVP(a);
    }
    {
        boost::archive::yaml_oarchive archive(std::cout);
        archive << BOOST_SERIALIZATION_NVP(a);
    }
    delete a;
}

A* load_naked_pointer(const char* filename)
{
    {
        std::ifstream input(filename);
        auto node = YAML::Load(input);
        std::cout << YAML::Dump(node) << '\n';
    }
    A* a = nullptr;
    std::ifstream input(filename);
    boost::archive::yaml_iarchive archive(input);
    archive >> BOOST_SERIALIZATION_NVP(a);
    return a;
}

BOOST_AUTO_TEST_CASE(naked_pointer)
{
    const char* filename = "naked_pointer.yml";
    save_naked_pointer(filename);
    auto a = load_naked_pointer(filename);
    auto b = load_naked_pointer_reference();
    BOOST_CHECK_EQUAL(*a, *b);
    delete a;
    delete b;
}

BOOST_AUTO_TEST_CASE(unique_pointer)
{
    // create  a new auto pointer to ta new object of type A
    std::unique_ptr<Y> spa(new Y);
    spa->name = "John Doe";
    spa->temperature = 123;
    {
        std::ofstream os("unique_pointer.yml");
        boost::archive::yaml_oarchive oa(os);
        boost::archive::yaml_oarchive couttest(std::cout);
        oa << BOOST_SERIALIZATION_NVP(spa);
        couttest << BOOST_SERIALIZATION_NVP(spa);
    }
    {
        std::ifstream input("unique_pointer.yml");
        auto node = YAML::Load(input);
        std::cout << YAML::Dump(node) << '\n';
    }
    {
        // reset the unique_ptr to NULL
        // thereby destroying the object of type A
        // note that the reset automagically maintains the reference count
        spa.reset();
        std::ifstream is("unique_pointer.yml");
        boost::archive::yaml_iarchive ia(is);
        ia >> BOOST_SERIALIZATION_NVP(spa);
    }
}
