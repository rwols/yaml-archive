#include <boost/archive/detail/archive_serializer_map.hpp>
#include <boost/archive/yaml_oarchive.hpp>
#include <boost/locale/encoding_utf.hpp>

#ifdef DEBUG_YAML_OARCHIVE
#include <iostream>
#endif

using namespace boost::archive;

yaml_oarchive::yaml_oarchive(std::ostream& os, const unsigned flags)
    : detail::common_oarchive<yaml_oarchive>(flags), m_emit(os)
{
    os << "%YAML 1.2\n%TAG ! boost/serialization/"
       << boost::archive::BOOST_ARCHIVE_VERSION() << "/\n";
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
            m_emit << m_deferred.front();
        }
        m_deferred.pop();
    }
    m_emit << YAML::EndMap << YAML::EndDoc;
}

#include <iostream>

void yaml_oarchive::save(const wchar_t t)
{
    const std::wstring wstr(1, t);
    save(boost::locale::conv::utf_to_utf<std::string::value_type>(wstr));
}

void yaml_oarchive::save(const std::wstring& t)
{
    save(boost::locale::conv::utf_to_utf<std::string::value_type>(t));
}

void yaml_oarchive::save(char t) { save(std::string(1, t)); }

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
        m_emit << m_deferred.front();
        m_deferred.pop();
    }
    if (m_pending_class != nullptr)
    {
        m_emit << YAML::Key << "__class__" << YAML::Value << m_pending_class;
        m_pending_class = nullptr;
    }
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const class_id_type& t)
#else
void yaml_oarchive::save_override(const class_id_type& t, int)
#endif
{
    if (t == NULL_POINTER_TAG)
    {
        assert(m_deferred.size() > 0);
        while (m_deferred.size() > 1)
        {
            m_emit << m_deferred.front();
            m_deferred.pop();
        }
        // pop the BeginMap that was pushed most recently
        m_deferred.pop();
        m_emit << YAML::Null;
        m_is_alias = true;
        return;
    }
    m_tag.append("c").append(std::to_string(static_cast<int>(t)));
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const class_id_optional_type& t)
#else
void yaml_oarchive::save_override(const class_id_optional_type& t, int)
#endif
{
    m_tag.append("c").append(std::to_string(static_cast<int>(t)));
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const class_id_reference_type& t)
#else
void yaml_oarchive::save_override(const class_id_reference_type& t, int)
#endif
{
    m_tag.append("r").append(std::to_string(static_cast<int>(t)));
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const object_id_type& t)
#else
void yaml_oarchive::save_override(const object_id_type& t, int)
#endif
{
    m_anchor = static_cast<unsigned>(t);
    m_tag.append("o").append(std::to_string(static_cast<unsigned>(t)));
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const object_reference_type& t)
#else
void yaml_oarchive::save_override(const object_reference_type& t, int)
#endif
{
    m_tag.clear();
    assert(m_deferred.size() > 0);
    while (m_deferred.size() > 1)
    {
        m_emit << m_deferred.front();
        m_deferred.pop();
    }
    // pop the BeginMap that was pushed most recently
    m_deferred.pop();
    m_emit << YAML::Alias(std::to_string(static_cast<unsigned>(t)));
    m_is_alias = true;
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const version_type& t)
#else
void yaml_oarchive::save_override(const version_type& t, int)
#endif
{
    m_tag.append("v").append(std::to_string(static_cast<unsigned>(t)));
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const class_name_type& t)
#else
void yaml_oarchive::save_override(const class_name_type& t, int)
#endif
{
    assert(m_pending_class == nullptr);
    m_pending_class = static_cast<const char*>(t);
}

#if BOOST_VERSION > 104800
void yaml_oarchive::save_override(const tracking_type& t)
#else
void yaml_oarchive::save_override(const tracking_type& t, int)
#endif
{
    if (static_cast<bool>(t))
    {
        m_tag.append("t");
    }
}

#include <boost/archive/impl/archive_serializer_map.ipp>
template class detail::archive_serializer_map<yaml_oarchive>;
