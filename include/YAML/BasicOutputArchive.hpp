/**
 * @author Raoul Wols
 *
 * @file   Defines the BasicOutputArchive template class.
 */

#pragma once

#include <YAML/BasicArchive.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <forward_list>
#include <list>
#include <queue>

#include <YAML/nsbegin.hpp>

/**
 * @brief      Writes YAML archives. Don't use this class directly, instead use
 *             YAML::OutputArchive.
 *
 * @tparam     Archive  The derived class.
 */
template <class Archive>
class BasicOutputArchive
    : public boost::archive::detail::common_oarchive<Archive>
{
  protected:
    BasicOutputArchive(std::ostream& output, const ArchiveFlag flags,
                       const char* tag)
        : boost::archive::detail::common_oarchive<Archive>(
              static_cast<unsigned>(flags)),
          m_emit(output)
    {
        if (!check_flag(ArchiveFlag::NoHeader))
        {
            output << "\%YAML 1.2\n\%TAG ! tag:" << tag << ","
                   << boost::archive::BOOST_ARCHIVE_VERSION() << ":\n";
        }
        if (!check_flag(ArchiveFlag::NoDocumentDelimiters))
        {
            m_emit << BeginDoc;
        }
        if (check_flag(ArchiveFlag::RootIsSequence))
        {
            m_emit << BeginSeq;
        }
        else
        {
            m_emit << BeginMap;
        }
        if (!check_flag(ArchiveFlag::NoTags))
        {
            m_tag.reserve(16);
        }
    }

    ~BasicOutputArchive()
    {
        if (std::uncaught_exception()) return;
        while (!m_deferred.empty())
        {
            m_emit << m_deferred.front();
            m_deferred.pop();
        }
        m_emit << EndMap;
        if (!check_flag(ArchiveFlag::NoDocumentDelimiters))
        {
            m_emit << EndDoc;
        }
    }

    friend class boost::archive::detail::interface_oarchive<Archive>;

    template <bool B, class T = void> using EnableIf = std::enable_if_t<B, T>;

    using detail_common_oarchive =
        boost::archive::detail::common_oarchive<Archive>;

    Emitter m_emit;
    std::string m_tag;
    std::queue<EMITTER_MANIP> m_deferred;

    inline bool check_flag(const ArchiveFlag f)
    {
        return 1 == (static_cast<unsigned>(f) & this->get_flags());
    }

    void check_tag()
    {
        if (!m_tag.empty())
        {
            m_emit << LocalTag(m_tag);
            m_tag.clear();
        }
        while (!m_deferred.empty())
        {
            m_emit << m_deferred.front();
            m_deferred.pop();
        }
    }

    // void save_start(const char* key)
    // {
    //     if (!key) return;
    //     check_tag();
    //     m_emit << Key << t.name();
    //     m_deferred.push(Value);
    // }
    // void save_end(const char* key)
    // {

    // }

    // Anything not an attribute and not a name-value pair is an
    // error and should be trapped here.
    template <class T> void save_override(const T& t)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        static_assert(
            std::is_base_of<
                boost::mpl::true_,
                typename boost::serialization::is_wrapper<T>::type>::value,
            "Wrap your variable to be serialized inside the "
            "BOOST_SERIALIZATION_NVP macro or use the function "
            "boost::serialization::make_nvp. To use the macro or the function,"
            "include the header boost/serialization/nvp.hpp");
        this->detail_common_oarchive::save_override(t);
    }

    // If T is a YAML type (i.e. "primitive" type), then the Emitter already
    // knows how to deal with T. This includes vectors, lists and maps of
    // primitive types. So for instance, an
    // std::vector<std::map<std::string, int>> should end up here and be
    // serialized immediately by the emitter.
    template <class T>
    EnableIf<IsPrimitive<T>> save_override(const KeyValue<T>& t)
    {
        if (t.name())
        {
            check_tag();
            m_emit << Key << t.name() << Value << t.const_value();
        }
        else // we're in the process of serializing a pointer
        {
            m_emit << t.const_value();
        }
    }

    // If T is not a YAML type, invoke boost serialization machinery.
    template <class T>
    EnableIf<!IsPrimitive<T>> save_override(const KeyValue<T>& t)
    {
        using namespace boost::serialization;
        if (t.name())
        {
            check_tag();
            m_emit << Key << t.name();
            m_deferred.push(Value);
            m_deferred.push(BeginMap);
            this->detail_common_oarchive::save_override(t.const_value());
            m_emit << EndMap;
        }
        else // we're in the process of serializing a pointer
        {
            this->detail_common_oarchive::save_override(t.const_value());
        }
    }

    // If we happen to deal with a sequence which holds non-YAML types T, then
    // we can compactify this with a BeginSeq and EndSeq, and invoke the boost
    // serialization machinery for each item of non-YAML type T.
    template <class Sequence> void save_sequence(const KeyValue<Sequence>& t)
    {
        check_tag();
        m_emit << Key << t.name();
        if (t.const_value().empty())
        {
            m_emit << Value << BeginSeq << EndSeq;
        }
        else
        {
            m_deferred.push(Value);
            m_deferred.push(BeginSeq);
            for (const auto& item : t.const_value())
            {
                m_deferred.push(Value);
                m_deferred.push(BeginMap);
                this->detail_common_oarchive::save_override(item);
                m_emit << EndMap;
            }
            m_emit << EndSeq;
        }
    }
    template <class T, class Alloc>
    EnableIf<!IsPrimitive<T>>
    save_override(const KeyValue<std::vector<T, Alloc>>& t)
    {
        save_sequence(t);
    }
    template <class T, class Alloc>
    EnableIf<!IsPrimitive<T>>
    save_override(const KeyValue<std::list<T, Alloc>>& t)
    {
        save_sequence(t);
    }
    template <class T, class Alloc>
    EnableIf<!IsPrimitive<T>>
    save_override(const KeyValue<std::forward_list<T, Alloc>>& t)
    {
        save_sequence(t);
    }

    // Same reasoning for a map (see above comment).
    template <class TMap> void save_map(const KeyValue<TMap>& t)
    {
        check_tag();
        m_emit << Key << t.name();
        if (t.const_value().empty())
        {
            m_emit << Value << BeginMap << EndMap;
        }
        else
        {
            m_deferred.push(Value);
            m_deferred.push(BeginMap);
            for (const auto& kv : t.const_value())
            {
                this->detail_common_oarchive::save_override(
                    boost::serialization::make_nvp(kv.first, kv.second));
            }
            m_emit << EndMap;
        }
    }
    template <class TKey, class T, class Compare, class Alloc>
    EnableIf<!IsPrimitive<T>>
    save_override(const KeyValue<std::map<TKey, T, Compare, Alloc>>& t)
    {
        save_map(t);
    }

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    void save_override(const boost::archive::class_id_type& t)
    {
        if (check_flag(ArchiveFlag::NoTags)) return;
        m_tag.append("c").append(std::to_string(static_cast<int>(t)));
    }
    void save_override(const boost::archive::class_id_optional_type& t)
    {
        if (check_flag(ArchiveFlag::NoTags)) return;
        save_override(boost::archive::class_id_type(static_cast<int>(t)));
    }
    void save_override(const boost::archive::class_id_reference_type& t)
    {
        if (check_flag(ArchiveFlag::NoTags)) return;
        m_tag.append("r").append(std::to_string(static_cast<int>(t)));
    }
    void save_override(const boost::archive::object_id_type& t)
    {
        m_emit << Anchor(std::to_string(static_cast<unsigned>(t)));
    }
    void save_override(const boost::archive::object_reference_type& t)
    {
        while (!m_deferred.empty())
        {
            m_emit << m_deferred.front();
            m_deferred.pop();
        }
        m_tag.clear();
        m_emit << Alias(std::to_string(static_cast<unsigned>(t)));
    }
    void save_override(const boost::archive::version_type& t)
    {
        if (check_flag(ArchiveFlag::NoTags)) return;
        m_tag.append("v").append(std::to_string(static_cast<unsigned>(t)));
    }
    void save_override(const boost::archive::class_name_type& t)
    {
        // Do nothing.
    }
    void save_override(const boost::archive::tracking_type& t)
    {
        if (!check_flag(ArchiveFlag::NoTags) && t) m_tag.append("t");
    }
};

#include <YAML/nsend.hpp>
