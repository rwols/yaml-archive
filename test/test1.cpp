#define BOOST_TEST_MODULE test1

#include <YAML/InputArchive.hpp>
#include <YAML/OutputArchive.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

struct Basic
{
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

struct Y
{
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
    return os << "<Y obj with name: " << y.name << ", temp: " << y.temperature
              << '>';
}

struct X
{
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

std::ostream& operator<<(std::ostream& os, const X& x)
{
    return os << "<X obj with data: " << x.data << ", y: " << x.y
              << " and more...>";
}

BOOST_AUTO_TEST_CASE(basic_struct_input_output)
{
    Basic b;
    b.data = 5;
    b.more_data = 3.141592f;
    b.some_name = "Hello, world!";
    {
        std::ofstream output("basic_struct_input_output.yml");
        YAML::OutputArchive archive(output);
        archive << BOOST_SERIALIZATION_NVP(b);
        YAML::OutputArchive couttest(std::cout);
        couttest << BOOST_SERIALIZATION_NVP(b);
    }
    Basic another_b;
    {
        std::ifstream input("basic_struct_input_output.yml");
        YAML::InputArchive archive(input);
        try
        {
            archive >> boost::serialization::make_nvp("b", another_b);
        }
        catch (const YAML::InputArchive::KeyNotPresentError& e)
        {
            std::cerr << "Key not present: " << e.key() << '\n';
            BOOST_CHECK(false);
        }
    }
    BOOST_CHECK_EQUAL(b, another_b);
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
        YAML::OutputArchive archive(output);
        archive << BOOST_SERIALIZATION_NVP(x);
        YAML::OutputArchive couttest(std::cout);
        couttest << BOOST_SERIALIZATION_NVP(x);
    }
    X another_x;
    {
        std::ifstream input("value_types_and_vectors_and_maps.yml");
        YAML::InputArchive archive(input);
        archive >> boost::serialization::make_nvp("x", another_x);
    }
    BOOST_CHECK_EQUAL(x, another_x);
}

BOOST_AUTO_TEST_CASE(binary_blobs)
{
    X x;
    {
        YAML::OutputArchive archive(std::cout);
        archive.save_binary(&x, sizeof(x));
    }
}

BOOST_AUTO_TEST_CASE(naked_pointer)
{
    auto x = new X();
    x->data = 42;
    x->y = Y();
    x->y.name = "John Doe";
    x->y.temperature = 100;
    std::stringstream ss;
    std::string copy;
    {
        YAML::OutputArchive archive(ss);
        archive << BOOST_SERIALIZATION_NVP(x);
        copy = ss.str();
    }
    delete x;
    x = nullptr;
    x = new X();
    {
        YAML::InputArchive archive(ss);
        archive >> BOOST_SERIALIZATION_NVP(x);
    }
    std::cout << copy << '\n';
    delete x;
}

BOOST_AUTO_TEST_CASE(travis_yaml_file)
{
    std::ifstream input("basic.yml");
    BOOST_CHECK(static_cast<bool>(input));
    std::string yaml(std::istreambuf_iterator<char>{input},
                     std::istreambuf_iterator<char>{});
    std::cout << yaml << '\n';
    auto doc = YAML::Load(yaml);
    BOOST_CHECK(doc.IsMap());
    std::stack<YAML::Node> nodes;
    nodes.push(std::move(doc));

    if (nodes.top()["b"])
    {
        nodes.push(nodes.top()["b"]);
        if (nodes.top()["__class_id__"])
        {
            nodes.push(nodes.top()["__class_id__"]);
            std::cout << "__class_id__ is " << nodes.top().as<unsigned>()
                      << std::endl;
            nodes.pop();
        }
        if (nodes.top()["__tracking__"])
        {
            nodes.push(nodes.top()["__tracking__"]);
            std::cout << "__tracking__ is " << nodes.top().as<bool>()
                      << std::endl;
            nodes.pop();
        }
        if (nodes.top()["data"])
        {
            nodes.push(nodes.top()["data"]);
            std::cout << "data is " << nodes.top().as<int>() << std::endl;
            nodes.pop();
        }
        if (nodes.top()["some_name"])
        {
            nodes.push(nodes.top()["some_name"]);
            std::cout << "some_name is " << nodes.top().as<std::string>()
                      << std::endl;
            nodes.pop();
        }
    }
}
