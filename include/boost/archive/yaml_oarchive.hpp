/**
 * @author Raoul Wols
 * @file Declares the yaml_oarchive class.
 */

#pragma once

#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/archive/detail/decl.hpp>
#include <boost/archive/detail/is_yaml_primitive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#if BOOST_VERSION < 105600
#include <boost/archive/shared_ptr_helper.hpp>
#endif
#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/string.hpp>
#include <boost/version.hpp>
#include <queue>
#include <stack>
#include <yaml-cpp/yaml.h>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost {
namespace archive {

/**
 * @brief      Archive for YAML format
 */
class BOOST_SYMBOL_VISIBLE yaml_oarchive
    : public detail::common_oarchive<yaml_oarchive>
#if BOOST_VERSION < 105600
      ,
      public detail::shared_ptr_helper
#endif
{
  public:
    BOOST_SYMBOL_VISIBLE yaml_oarchive(std::ostream&  os,
                                       const unsigned flags = 0);

    BOOST_SYMBOL_VISIBLE ~yaml_oarchive() override;

    BOOST_SYMBOL_VISIBLE
    void save_binary(const void* address, std::size_t count);

  private:
    friend class detail::interface_oarchive<yaml_oarchive>;
    friend class save_access;

    YAML::Emitter                   m_emit;
    std::queue<YAML::EMITTER_MANIP> m_deferred;
    std::string                     m_tag;
    unsigned                        m_anchor = (unsigned)-1;
    const char*                     m_pending_class = nullptr;
    bool                            m_is_alias = false;
    std::stack<bool>                m_saving_dereffed_ptr;

    BOOST_SYMBOL_VISIBLE void end_preamble();

    using base = detail::common_oarchive<yaml_oarchive>;
    template <class T> using nvp = boost::serialization::nvp<T>;

#if BOOST_VERSION > 105800

    // Anything not an attribute and not a name-value pair is an
    // error and should be trapped here.
    template <class T> void save_override(T& t)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        static_assert(serialization::is_wrapper<T>::type::value,
                      "wrap your values to be serialized in the "
                      "BOOST_SERIALIZATION_NVP wrapper, or use the function "
                      "boost::serialization::make_nvp");
        base::save_override(t);
    }

    template <class T> void save_override(const nvp<T>& t)
    {
        if (t.name() != nullptr)
        {
            end_preamble();
            m_emit << YAML::Key << t.name() << YAML::Value;
            m_saving_dereffed_ptr.push(false);
        }
        else
        {
            m_saving_dereffed_ptr.push(true);
        }
        save_value(t.const_value());
        m_saving_dereffed_ptr.pop();
    }

    // Otherwise, invoke boost::serialization machinery.
    template <class T>
    typename std::enable_if<!detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    save_value(const T& t)
    {
        if (!m_saving_dereffed_ptr.top() && !boost::is_enum<T>::value &&
            !detail::is_yaml_primitive<typename std::remove_pointer<
                typename std::decay<T>::type>::type>::value)
        {
            m_deferred.push(YAML::BeginMap);
        }

        base::save_override(t);

        if (m_is_alias)
        {
            // don't emit EndMap
            m_is_alias = false;
        }
        else if (!m_saving_dereffed_ptr.top() && !boost::is_enum<T>::value &&
                 !detail::is_yaml_primitive<typename std::remove_pointer<
                     typename std::decay<T>::type>::type>::value)
        {
            end_preamble();
            m_emit << YAML::EndMap;
        }
    }

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    BOOST_SYMBOL_VISIBLE void save_override(const class_id_type& t);
    BOOST_SYMBOL_VISIBLE void save_override(const class_id_optional_type& t);
    BOOST_SYMBOL_VISIBLE void save_override(const class_id_reference_type& t);
    BOOST_SYMBOL_VISIBLE void save_override(const object_id_type& t);
    BOOST_SYMBOL_VISIBLE void save_override(const object_reference_type& t);
    BOOST_SYMBOL_VISIBLE void save_override(const version_type& t);
    BOOST_SYMBOL_VISIBLE void save_override(const class_name_type& t);
    BOOST_SYMBOL_VISIBLE void save_override(const tracking_type& t);

#else // BOOST_VERSION <= 105800

    // Anything not an attribute and not a name-value pair is an
    // error and should be trapped here.
    template <class T> void save_override(T& t, BOOST_PFTO int)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        static_assert(serialization::is_wrapper<T>::type::value,
                      "wrap your values to be serialized in the "
                      "BOOST_SERIALIZATION_NVP wrapper, or use the function "
                      "boost::serialization::make_nvp");
        base::save_override(t, 0);
    }

    template <class T> void save_override(const nvp<T>& t, int)
    {
        if (t.name() != nullptr)
        {
            end_preamble();
            m_emit << YAML::Key << t.name() << YAML::Value;
            m_saving_dereffed_ptr.push(false);
        }
        else
        {
            m_saving_dereffed_ptr.push(true);
        }
        save_value(t.const_value());
        m_saving_dereffed_ptr.pop();
    }

    // Otherwise, invoke boost::serialization machinery.
    template <class T>
    typename std::enable_if<!detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    save_value(const T& t)
    {
        if (!m_saving_dereffed_ptr.top() && !boost::is_enum<T>::value &&
            !detail::is_yaml_primitive<typename std::remove_pointer<
                typename std::decay<T>::type>::type>::value)
        {
            m_deferred.push(YAML::BeginMap);
        }

        base::save_override(t, 0);

        if (m_is_alias)
        {
            // don't emit EndMap
            m_is_alias = false;
        }
        else if (!m_saving_dereffed_ptr.top() && !boost::is_enum<T>::value &&
                 !detail::is_yaml_primitive<typename std::remove_pointer<
                     typename std::decay<T>::type>::type>::value)
        {
            end_preamble();
            m_emit << YAML::EndMap;
        }
    }

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    BOOST_SYMBOL_VISIBLE void save_override(const class_id_type& t, int);
    BOOST_SYMBOL_VISIBLE void save_override(const class_id_optional_type& t,
                                            int);
    BOOST_SYMBOL_VISIBLE void save_override(const class_id_reference_type& t,
                                            int);
    BOOST_SYMBOL_VISIBLE void save_override(const object_id_type& t, int);
    BOOST_SYMBOL_VISIBLE void save_override(const object_reference_type& t,
                                            int);
    BOOST_SYMBOL_VISIBLE void save_override(const version_type& t, int);
    BOOST_SYMBOL_VISIBLE void save_override(const class_name_type& t, int);
    BOOST_SYMBOL_VISIBLE void save_override(const tracking_type& t, int);

#endif // BOOST_VERSION

    // If YAML::Emitter understands T, then emit it immediately.
    template <class T>
    typename std::enable_if<detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    save_value(const T& t)
    {
        save(t);
    }

    template <class T, std::size_t N> void save_value(const T (&t)[N])
    {
        save_fixed_size_array<N>(t);
    }
    template <class T, std::size_t N> void save_value(const std::array<T, N>& t)
    {
        save_fixed_size_array<N>(t);
    }
    template <class T, std::size_t N>
    void save_value(const boost::array<T, N>& t)
    {
        save_fixed_size_array<N>(t);
    }

    template <class T, class Alloc>
    typename std::enable_if<!detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    save_value(const std::vector<T, Alloc>& t)
    {
        save_sequence(t);
    }
    template <class Alloc> void save_value(const std::vector<bool, Alloc>& t)
    {
        std::string bits;
        bits.reserve(t.size());
        for (const auto b : t) bits.push_back(b ? '1' : '0');
        save(bits);
    }

    template <class T, class Alloc>
    typename std::enable_if<!detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    save_value(const std::deque<T, Alloc>& t)
    {
        save_sequence(t);
    }

    template <class T, class Alloc>
    typename std::enable_if<!detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    save_value(const std::list<T, Alloc>& t)
    {
        save_sequence(t);
    }

#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
    template <class T, class Alloc>
    typename std::enable_if<!detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    save_value(const std::forward_list<T, Alloc>& t)
    {
        save_sequence(t);
    }
#endif

    template <class Key, class T, class Compare, class Alloc>
    void save_value(const std::map<Key, T, Compare, Alloc>& t)
    {
        save_map(t);
    }

#ifndef BOOST_NO_CXX11_HDR_UNORDERED_MAP
    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void save_value(const std::unordered_map<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        save_map(t);
    }
#endif

    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void
    save_value(const boost::unordered_map<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        save_map(t);
    }

    template <class Key, class T, class Compare, class Alloc>
    void save_value(const std::multimap<Key, T, Compare, Alloc>& t)
    {
        save_multimap(t);
    }

#ifndef BOOST_NO_CXX11_HDR_UNORDERED_MAP
    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void
    save_value(const std::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        save_multimap(t);
    }
#endif

    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void save_value(
        const boost::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        save_multimap(t);
    }

    template <class Key, class Compare, class Alloc>
    void save_value(const std::set<Key, Compare, Alloc>& t)
    {
        save_sequence(t);
    }

    template <class Key, class Compare, class Alloc>
    void save_value(const std::multiset<Key, Compare, Alloc>& t)
    {
        save_sequence(t);
    }

#ifndef BOOST_NO_CXX11_HDR_UNORDERED_SET
    template <class Key, class Hash, class KeyEqual, class Alloc>
    void save_value(const std::unordered_set<Key, Hash, KeyEqual, Alloc>& t)
    {
        save_sequence(t);
    }
#endif

    template <class Key, class Hash, class KeyEqual, class Alloc>
    void save_value(const boost::unordered_set<Key, Hash, KeyEqual, Alloc>& t)
    {
        save_sequence(t);
    }

#ifndef BOOST_NO_CXX11_HDR_UNORDERED_SET
    template <class Key, class Hash, class KeyEqual, class Alloc>
    void
    save_value(const std::unordered_multiset<Key, Hash, KeyEqual, Alloc>& t)
    {
        save_sequence(t);
    }
#endif

    template <class Key, class Hash, class KeyEqual, class Alloc>
    void
    save_value(const boost::unordered_multiset<Key, Hash, KeyEqual, Alloc>& t)
    {
        save_sequence(t);
    }

    template <class First, class Second>
    // void
    typename std::enable_if<!detail::is_yaml_primitive<
                                typename std::remove_cv<First>::type>::value ||
                            !detail::is_yaml_primitive<typename std::remove_cv<
                                Second>::type>::value>::type
    save_value(const std::pair<First, Second>& t)
    {
        m_deferred.push(YAML::BeginSeq);
        save_value(t.first);
        save_value(t.second);
        m_emit << YAML::EndSeq;
    }

  public:
    template <std::size_t N, class T>
    typename std::enable_if<!std::is_default_constructible<T>::value>::type
    save_fixed_size_array(const T& t)
    {
        end_preamble();
        m_emit << YAML::BeginSeq;
        for (const auto& item : t)
        {
            m_emit << YAML::BeginMap;
            m_emit << YAML::Key << "construct_data" << YAML::Value
                   << YAML::BeginMap;
            serialization::save_construct_data_adl(
                *this, &item, serialization::version<T>::value);
            end_preamble();
            m_emit << YAML::EndMap << YAML::Key << "object" << YAML::Value;
            save_value(item);
            m_emit << YAML::EndMap;
        }
        m_emit << YAML::EndSeq;
    }

    template <std::size_t N, class T>
    typename std::enable_if<std::is_default_constructible<T>::value>::type
    save_fixed_size_array(const T& t)
    {
        end_preamble();
        m_emit << YAML::BeginSeq;
        for (const auto& item : t)
        {
            save_value(item);
        }
        m_emit << YAML::EndSeq;
    }

    template <class T>
    typename std::enable_if<
        !std::is_default_constructible<typename T::value_type>::value>::type
    save_sequence(const T& t)
    {
        end_preamble();
        m_emit << YAML::BeginSeq;
        for (const auto& item : t)
        {
            m_emit << YAML::BeginMap;
            m_emit << YAML::Key << "construct_data" << YAML::Value
                   << YAML::BeginMap;
            serialization::save_construct_data_adl(
                *this, &item,
                serialization::version<typename T::value_type>::value);
            end_preamble();
            m_emit << YAML::EndMap << YAML::Key << "object" << YAML::Value;
            save_value(item);
            m_emit << YAML::EndMap;
        }
        m_emit << YAML::EndSeq;
    }

    template <class T>
    typename std::enable_if<
        std::is_default_constructible<typename T::value_type>::value>::type
    save_sequence(const T& t)
    {
        end_preamble();
        m_emit << YAML::BeginSeq;
        for (const auto& item : t)
        {
            save_value(item);
        }
        m_emit << YAML::EndSeq;
    }

    template <class T> void save_map(const T& t)
    {
        end_preamble();
        m_deferred.push(YAML::BeginMap);
        for (const auto& kv : t)
        {
            m_deferred.push(YAML::Key);
            save_value(kv.first);
            m_deferred.push(YAML::Value);
            save_value(kv.second);
        }
        m_emit << YAML::EndMap;
    }
    template <class T> void save_multimap(const T& t)
    {
        m_deferred.push(YAML::BeginSeq);
        for (const auto& kv : t)
        {
            save_value(kv); // save the pair
        }

        end_preamble();
        m_emit << YAML::EndSeq;
    }

  protected:
    template <class T> void save(const T& t)
    {
        end_preamble();
        m_emit << t;
    }
    void save(char t);
    void save(signed char t);
    void save(unsigned char t);
    void save(const wchar_t t);
    void save(const std::wstring& t);
    void save(const boost::serialization::item_version_type& t);
    void save(const boost::serialization::collection_size_type& t);
};

} // namespace archive
} // namespace boost

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::yaml_oarchive)
