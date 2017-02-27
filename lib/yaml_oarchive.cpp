#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/iterators/mb_from_wchar.hpp>
#include <boost/archive/yaml_oarchive.hpp>

#ifdef DEBUG_YAML_OARCHIVE
#include <iostream>
#endif

using namespace boost::archive;

yaml_oarchive::yaml_oarchive(std::ostream& os, const unsigned flags)
    : detail::common_oarchive<yaml_oarchive>(flags), m_emit(os)
{
    os << "%YAML 1.2\n%TAG ! boost/serialization/"
       << boost::archive::BOOST_ARCHIVE_VERSION() << "/\n";
#ifdef DEBUG_YAML_OARCHIVE
    print(YAML::BeginDoc);
    print(YAML::BeginMap);
#endif
    m_emit << YAML::BeginDoc << YAML::BeginMap;
}

yaml_oarchive::~yaml_oarchive()
{
    if (std::uncaught_exception()) return;
    while (!m_deferred.empty())
    {
        if (m_is_alias)
        {
            // don't emit EndMap
            m_is_alias = false;
        }
        else
        {
#ifdef DEBUG_YAML_OARCHIVE
            print(m_deferred.front());
#endif
            m_emit << m_deferred.front();
        }
        m_deferred.pop();
    }
    m_emit << YAML::EndMap << YAML::EndDoc;
}

void yaml_oarchive::save(const wchar_t t)
{
    static_assert(sizeof(wchar_t) <= sizeof(int),
                  "wchar_t must fit inside an int.");
    const auto x = static_cast<int>(t);
    save(x);
}

void yaml_oarchive::save(const std::wstring& t)
{
    std::string bytes;
    bytes.reserve(2 * t.length());
    typedef boost::archive::iterators::mb_from_wchar<
        std::wstring::const_iterator>
        translator;
    std::copy(translator(t.begin()), translator(t.end()),
              std::back_inserter(bytes));
    save(bytes);
}

void yaml_oarchive::save(char t) { save(static_cast<int>(t)); }

void yaml_oarchive::save(signed char t) { save(static_cast<int>(t)); }

void yaml_oarchive::save(unsigned char t)
{
    save(static_cast<unsigned int>(t));
}

void yaml_oarchive::save(const boost::serialization::item_version_type& t)
{
    const auto x = static_cast<unsigned>(t);
    save(x);
}

void yaml_oarchive::save(const boost::serialization::collection_size_type& t)
{
    const auto x = static_cast<std::size_t>(t);
    save(x);
}

#ifdef DEBUG_YAML_OARCHIVE
void yaml_oarchive::print(const YAML::EMITTER_MANIP x)
{
    switch (x)
    {
    // general manipulators
    case YAML::Auto:
        std::cout << "DEBUG: Auto\n";
        break;
    case YAML::TagByKind:
        std::cout << "DEBUG: TagByKind\n";
        break;
    case YAML::Newline:
        std::cout << "DEBUG: Newline\n";
        break;
    case YAML::EmitNonAscii:
        std::cout << "DEBUG: EmitNonAscii\n";
        break;
    case YAML::EscapeNonAscii:
        std::cout << "DEBUG: EscapeNonAscii\n";
        break;
    case YAML::SingleQuoted:
        std::cout << "DEBUG: SingleQuoted\n";
        break;
    case YAML::DoubleQuoted:
        std::cout << "DEBUG: DoubleQuoted\n";
        break;
    case YAML::Literal:
        std::cout << "DEBUG: Literal\n";
        break;
    case YAML::YesNoBool:
        std::cout << "DEBUG: YesNoBool\n";
        break;
    case YAML::TrueFalseBool:
        std::cout << "DEBUG: TrueFalseBool\n";
        break;
    case YAML::OnOffBool:
        std::cout << "DEBUG: OnOffBool\n";
        break;
    case YAML::UpperCase:
        std::cout << "DEBUG: UpperCase\n";
        break;
    case YAML::LowerCase:
        std::cout << "DEBUG: LowerCase\n";
        break;
    case YAML::CamelCase:
        std::cout << "DEBUG: CamelCase\n";
        break;
    case YAML::LongBool:
        std::cout << "DEBUG: LongBool\n";
        break;
    case YAML::ShortBool:
        std::cout << "DEBUG: ShortBool\n";
        break;
    case YAML::Dec:
        std::cout << "DEBUG: Dec\n";
        break;
    case YAML::Hex:
        std::cout << "DEBUG: Hex\n";
        break;
    case YAML::Oct:
        std::cout << "DEBUG: Oct\n";
        break;
    case YAML::BeginDoc:
        std::cout << "DEBUG: BeginDoc\n";
        break;
    case YAML::EndDoc:
        std::cout << "DEBUG: EndDoc\n";
        break;
    case YAML::BeginSeq:
        std::cout << "DEBUG: BeginSeq\n";
        ++m_debug_seq_count;
        break;
    case YAML::EndSeq:
        assert(m_debug_seq_count > 0 && "incorrect EndSeq called.");
        --m_debug_seq_count;
        std::cout << "DEBUG: EndSeq(" << m_debug_seq_count << ")\n";
        break;
    case YAML::Flow:
        std::cout << "DEBUG: Flow\n";
        break;
    case YAML::Block:
        std::cout << "DEBUG: Block\n";
        break;
    case YAML::BeginMap:
        std::cout << "DEBUG: BeginMap(" << m_debug_map_count << ")\n";
        ++m_debug_map_count;
        break;
    case YAML::EndMap:
        assert(m_debug_map_count > 0 && "incorrect EndMap called.");
        --m_debug_map_count;
        std::cout << "DEBUG: EndMap(" << m_debug_map_count << ")\n";
        break;
    case YAML::Key:
        std::cout << "DEBUG: Key\n";
        break;
    case YAML::Value:
        std::cout << "DEBUG: Value\n";
        break;
    case YAML::LongKey:
        std::cout << "DEBUG: LongKey\n";
        break;
    }
}
#endif

void yaml_oarchive::save_binary(const void* address, std::size_t count)
{
    end_preamble();
    m_emit << YAML::Binary(static_cast<const unsigned char*>(address), count);
}

// end_preamble gets called when all the meta-info has been serialized like
// class_id_type and so forth, and we're about to serialize a value or enum
// or pointer.
void yaml_oarchive::end_preamble()
{
    if (!m_tag.empty())
    {
        m_emit << YAML::LocalTag(m_tag);
        m_tag.clear();
    }
    if (m_anchor != (unsigned)-1)
    {
        m_emit << YAML::Anchor(std::to_string(m_anchor));
        m_anchor = (unsigned)-1;
    }
    while (!m_deferred.empty())
    {
// emit all the YAML::BeginMap and YAML::BeginSeq's now
#ifdef DEBUG_YAML_OARCHIVE
        print(m_deferred.front());
#endif
        m_emit << m_deferred.front();
        m_deferred.pop();
    }
    if (m_pending_class != nullptr)
    {
        m_emit << YAML::Key << "__class__" << YAML::Value << m_pending_class;
        m_pending_class = nullptr;
    }
}

void yaml_oarchive::save_override(const class_id_type& t)
{
    if (t == NULL_POINTER_TAG)
    {
        assert(m_deferred.size() > 0);
        while (m_deferred.size() > 1)
        {
#ifdef DEBUG_YAML_OARCHIVE
            print(m_deferred.front());
#endif
            m_emit << m_deferred.front();
            m_deferred.pop();
        }
        // pop the BeginMap that was pushed most recently
        m_deferred.pop();
#ifdef DEBUG_YAML_OARCHIVE
        print(YAML::Oct);
#endif
        m_emit << YAML::Null;
        m_is_alias = true;
        return;
    }
    m_tag.append("c").append(std::to_string(static_cast<int>(t)));
}

void yaml_oarchive::save_override(const class_id_optional_type& t)
{
    m_tag.append("c").append(std::to_string(static_cast<int>(t)));
}

void yaml_oarchive::save_override(const class_id_reference_type& t)
{
    m_tag.append("r").append(std::to_string(static_cast<int>(t)));
}

void yaml_oarchive::save_override(const object_id_type& t)
{
    m_anchor = static_cast<unsigned>(t);
    m_tag.append("o").append(std::to_string(static_cast<unsigned>(t)));
}

void yaml_oarchive::save_override(const object_reference_type& t)
{
    m_tag.clear();
    assert(m_deferred.size() > 0);
    while (m_deferred.size() > 1)
    {
#ifdef DEBUG_YAML_OARCHIVE
        print(m_deferred.front());
#endif
        m_emit << m_deferred.front();
        m_deferred.pop();
    }
    // pop the BeginMap that was pushed most recently
    m_deferred.pop();
#ifdef DEBUG_YAML_OARCHIVE
    print(YAML::Auto);
#endif
    m_emit << YAML::Alias(std::to_string(static_cast<unsigned>(t)));
    m_is_alias = true;
}

void yaml_oarchive::save_override(const version_type& t)
{
    m_tag.append("v").append(std::to_string(static_cast<unsigned>(t)));
}

void yaml_oarchive::save_override(const class_name_type& t)
{
    assert(m_pending_class == nullptr);
    m_pending_class = static_cast<const char*>(t);
}

void yaml_oarchive::save_override(const tracking_type& t)
{
    if (static_cast<bool>(t))
    {
        m_tag.append("t");
    }
}

#include <boost/archive/impl/archive_serializer_map.ipp>
template class detail::archive_serializer_map<yaml_oarchive>;
