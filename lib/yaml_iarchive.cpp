#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/yaml_iarchive.hpp>
#include <boost/locale/encoding_utf.hpp>

#ifdef YAML_ARCHIVE_DEBUG_STACK
#include <iostream>
#endif

using namespace boost::archive;

using boost::locale::conv::utf_to_utf;

yaml_iarchive::yaml_iarchive(std::istream& is, const unsigned flags)
    : detail::common_iarchive<yaml_iarchive>(flags)
{
    m_stack.push(YAML::Load(is));
#ifdef YAML_ARCHIVE_DEBUG_STACK
    std::cout << "The tag is: " << m_stack.top().Tag() << '\n';
    debug_print_stack();
#endif
}

void yaml_iarchive::load_binary(void* address, std::size_t count)
{
    const auto x = YAML::DecodeBase64(m_stack.top().Scalar());
    std::memcpy(address, x.data(), x.size());
}

#ifdef YAML_ARCHIVE_DEBUG_STACK
void yaml_iarchive::debug_print_stack()
{
    auto temp = m_stack;
    std::cout << "stack:  [ ";
    while (temp.empty() == false)
    {
        switch (temp.top().Type())
        {
        case YAML::NodeType::Undefined:
            std::cout << "<undefined> ";
            break;
        case YAML::NodeType::Null:
            std::cout << "<null> ";
            break;
        case YAML::NodeType::Scalar:
            std::cout << temp.top().Scalar() << ' ';
            break;
        case YAML::NodeType::Sequence:
            std::cout << "<sequence> ";
            break;
        case YAML::NodeType::Map:
            std::cout << "<map> ";
            break;
        }
        temp.pop();
    }
    std::cout << "]\n";
}
#endif

#include <iostream>

void yaml_iarchive::load(wchar_t& t)
{
    std::string utf8string;
    load(utf8string);
    const auto wstr = utf_to_utf<std::wstring::value_type>(utf8string);
    std::wcout << L"Loaded wchar_t: " << wstr << L"\n";
    if (wstr.size() != 1)
    {
        boost::serialization::throw_exception(
            archive_exception(archive_exception::input_stream_error));
    }
    t = wstr[0];
}

#include <iostream>

void yaml_iarchive::load(std::wstring& t)
{
    std::string utf8string;
    load(utf8string);
    t = utf_to_utf<std::wstring::value_type>(utf8string);
}

void yaml_iarchive::load(char& t)
{
    std::string str;
    load(str);
    if (str.size() != 1)
    {
        boost::serialization::throw_exception(
            archive_exception(archive_exception::input_stream_error));
    }
    t = str[0];
}

void yaml_iarchive::load(signed char& t)
{
    int x;
    load(x);
    t = static_cast<signed char>(x);
}

void yaml_iarchive::load(unsigned char& t)
{
    unsigned int x;
    load(x);
    t = static_cast<unsigned char>(x);
}

void yaml_iarchive::load(boost::serialization::item_version_type& t)
{
    unsigned x;
    load(x);
    t = boost::serialization::item_version_type(x);
}

void yaml_iarchive::load(boost::serialization::collection_size_type& t)
{
    std::size_t x;
    load(x);
    t = boost::serialization::collection_size_type(x);
}

template <class ArithmeticType, class BoostType>
bool load_tag(const std::string& tag, const char identifier, BoostType& t)
{
    auto pos = tag.rfind('/');
    if (pos == std::string::npos)
        pos = 0;
    else
        ++pos;
    pos = tag.find(identifier, pos);
    if (pos == std::string::npos)
        return false;
    else
        ++pos;
    const char*    c = tag.data() + pos;
    ArithmeticType x = 0;
    while (std::isdigit(*c))
    {
        x *= 10;
        x += *c - '0';
        ++c;
    }
    t = BoostType(x);
    return true;
}

void yaml_iarchive::load_override(class_id_type& t)
{
    load_tag<int>(m_stack.top().Tag(), 'c', t);
}

void yaml_iarchive::load_override(class_id_optional_type& t)
{
    class_id_type x;
    load_override(x);
    t = class_id_optional_type(x);
}

void yaml_iarchive::load_override(class_id_reference_type& t)
{
    class_id_type x;
    load_override(x);
    t = class_id_reference_type(x);
}

void yaml_iarchive::load_override(object_id_type& t)
{
    load_tag<unsigned>(m_stack.top().Tag(), 'o', t);
}

void yaml_iarchive::load_override(object_reference_type& t)
{
    object_id_type r;
    load_tag<unsigned>(m_stack.top().Tag(), 'r', r);
    t = object_reference_type(r);
}

void yaml_iarchive::load_override(version_type& t)
{
    load_tag<unsigned>(m_stack.top().Tag(), 'v', t);
}

void yaml_iarchive::load_override(class_name_type& t)
{
    const auto cls = m_stack.top()["__class__"];
    if (cls)
    {
        const auto& s = cls.Scalar();
        if (s.size() > BOOST_SERIALIZATION_MAX_KEY_SIZE - 1)
            boost::serialization::throw_exception(
                archive_exception(archive_exception::invalid_class_name));
        char* tptr = t;
        std::memcpy(tptr, s.data(), s.size());
        tptr[s.size()] = '\0';
    }
    else
    {
        boost::serialization::throw_exception(
            archive_exception(archive_exception::unregistered_class));
    }
}

void yaml_iarchive::load_override(tracking_type& t)
{
    auto pos = m_stack.top().Tag().rfind('/');
    if (pos == std::string::npos) pos = 0;
    pos = m_stack.top().Tag().find('t', pos);
    t = tracking_type(pos != std::string::npos);
}

#include <boost/archive/impl/archive_serializer_map.ipp>
template class detail::archive_serializer_map<yaml_iarchive>;
