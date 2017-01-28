/**
 * @author Raoul Wols
 *
 * @file   Defines the BasicInputArchive template class.
 */

#pragma once

#include <YAML/BasicArchive.hpp>
#include <boost/archive/archive_exception.hpp>
#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/serialization/detail/stack_constructor.hpp>
#include <istream>
#include <stack>

#include <YAML/nsbegin.hpp>

/**
 * @brief      Reads YAML archives. Don't use this class directly, instead use
 *             YAML::InputArchive.
 *
 * @tparam     Archive  The derived class.
 */
template <class Archive>
class BasicInputArchive
    : public boost::archive::detail::common_iarchive<Archive>
{

  public:
    /**
     * @brief      Gets thrown during deserialization when a key is not found.
     */
    class KeyNotPresentError : public boost::archive::archive_exception
    {
      public:
        ~KeyNotPresentError() noexcept override = default;

        /**
         * @brief      A chain of keys in which the last key was not present.
         *
         * @return     A chain of keys in which the last key was not present.
         */
        const std::vector<std::string>& keys() const noexcept { return m_keys; }
      private:
        friend class BasicInputArchive;
        std::vector<std::string> m_keys;
        KeyNotPresentError(std::vector<std::string>&& keys) noexcept
            : boost::archive::archive_exception(
                  boost::archive::archive_exception::other_exception),
              m_keys(std::move(keys))
        {
        }
    };

  protected:
    BasicInputArchive(std::istream& input, const unsigned flags = 0)
        : boost::archive::detail::common_iarchive<Archive>(flags)
    {
        m_stack.push(Load(input));
        if (!m_stack.top().IsMap())
        {
            throw std::runtime_error("expected a YAML map...");
        }
    }

    ~BasicInputArchive() noexcept = default;

    friend class boost::archive::detail::interface_iarchive<Archive>;

    template <bool B, class T = void> using EnableIf = std::enable_if_t<B, T>;

    using detail_common_iarchive =
        boost::archive::detail::common_iarchive<Archive>;

    std::stack<Node> m_stack;

    Node& top() { return m_stack.top(); }

    void throw_exception(const char* badkey)
    {
        std::vector<std::string> keys;
        // while (!m_stack.empty())
        // {
        //     m_stack.pop();
        //     keys.push_back(m_stack.top().template as<std::string>());
        // }
        keys.push_back(badkey);
        std::reverse(keys.begin(), keys.end());
        throw KeyNotPresentError(std::move(keys));
    }

    void load_start(const char* key)
    {
        auto node = top()[key];
        if (node)
        {
            m_stack.push(std::move(node));
        }
        else
        {
            throw_exception(key);
        }
    }

    void load_end() noexcept { m_stack.pop(); }

    // If T is a YAML type (i.e. "primitive" type), then we know what to do with
    // it.
    template <class T>
    EnableIf<IsPrimitive<T>> load_override(const KeyValue<T>& t)
    {
        const auto node = top()[t.name()];
        if (node)
        {
            t.value() = node.template as<T>();
        }
        else
        {
            throw_exception(t.name());
        }
    }

    template <class T> EnableIf<IsPrimitive<T>> load(T& t)
    {
        t = top().template as<T>();
    }

    template <class T> EnableIf<IsPrimitive<T>> load_override(T& t)
    {
        t = top().template as<T>();
    }

    // If T is not a YAML type, we invoke the serialization machinery of boost.
    template <class T>
    EnableIf<!IsPrimitive<T> && !IsMap<T> && !IsSequence<T>>
    load_override(const KeyValue<T>& t)
    {
        if (t.name())
        {
            load_start(t.name());
            this->detail_common_iarchive::load_override(t.value());
            load_end();
        }
        else
        {
            this->detail_common_iarchive::load_override(t.value());
        }
    }

    template <class Sequence>
    EnableIf<
        std::is_default_constructible<typename Sequence::value_type>::value>
    load_sequence(const KeyValue<Sequence>& t)
    {
        const auto count = top().size();
        for (std::size_t i = 0; i < count; ++i)
        {
            m_stack.push(top()[i]);
            typename Sequence::value_type x;
            this->detail_common_iarchive::load_override(x);
            t.value().push_back(std::move(x));
            m_stack.pop();
        }
    }
    template <class Sequence>
    EnableIf<
        !std::is_default_constructible<typename Sequence::value_type>::value>
    load_sequence(const KeyValue<Sequence>& t)
    {
        const auto count = top().size();
        for (std::size_t i = 0; i < count; ++i)
        {
            m_stack.push(top()[i]);
            // taken from: boost/serialization/collections_load_imp.hpp
            boost::serialization::detail::stack_construct<
                Archive, typename Sequence::value_type>
                u(*this->This(), 0);
            this->detail_common_iarchive::load_override(u.reference());
            t.value().push_back(u.reference());
            this->This()->reset_object_address(&t.value().back(),
                                               &u.reference());
            m_stack.pop();
        }
    }
    template <class T>
    EnableIf<IsSequence<T> && !IsPrimitive<typename T::value_type>>
    load_override(const KeyValue<T>& t)
    {
        load_start(t.name());
        load_sequence(t);
        load_end();
    }
    template <class MapLike>
    EnableIf<
        std::is_default_constructible<typename MapLike::mapped_type>::value>
    load_map(const KeyValue<MapLike>& t)
    {
        using KeyType = typename MapLike::key_type;
        using ValueType = typename MapLike::mapped_type;
        const Node tempcopy = top();
        for (const auto& kv : tempcopy)
        {
            m_stack.push(kv.second);
            ValueType x;
            this->detail_common_iarchive::load_override(x);
            t.value().emplace(kv.first.as<KeyType>(), std::move(x));
            m_stack.pop();
        }
    }
    template <class MapLike>
    EnableIf<
        !std::is_default_constructible<typename MapLike::mapped_type>::value>
    load_map(const KeyValue<MapLike>& t)
    {
        using KeyType = typename MapLike::key_type;
        using ValueType = typename MapLike::mapped_type;
        const Node tempcopy = top();
        for (const auto& kv : tempcopy)
        {
            m_stack.push(kv.second);
            // taken from: boost/serialization/collections_load_imp.hpp
            boost::serialization::detail::stack_construct<Archive, ValueType> u(
                *this->This(), 0);
            this->detail_common_iarchive::load_override(u.reference());
            t.value().emplace(kv.second.as<KeyType>(), u.reference());
            this->This()->reset_object_address(&t.value().back(),
                                               &u.reference());
            m_stack.pop();
        }
    }
    template <class T>
    EnableIf<IsMap<T> && !IsPrimitive<typename T::mapped_type>>
    load_override(const KeyValue<T>& t)
    {
        load_start(t.name());
        load_map(t);
        load_end();
    }

    template <class ArithmeticType, class BoostType>
    bool load_from_tag(const char identifier, BoostType& boost_type)
    {
        static_assert(std::is_arithmetic<ArithmeticType>::value,
                      "ArithmeticType must be arithmetic.");
        const auto pos = top().Tag().find(identifier);
        if (pos == std::string::npos) return false;
        const char* iter = top().Tag().data() + pos + 1;
        ArithmeticType x = 0;
        while (isdigit(*iter))
        {
            x *= 10;
            x += *iter - '0';
            ++iter;
        }
        boost_type = BoostType(x);
        return true;
    }

    // special handling for bool
    template <class BoostType>
    bool load_from_tag(const char identifier, BoostType& boost_type)
    {
        boost_type =
            BoostType(top().Tag().find(identifier) != std::string::npos);
        return static_cast<bool>(boost_type);
    }

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    void load_override(boost::archive::class_id_type& t)
    {
        // const auto x = top()["__class_id__"].template as<int>();
        load_from_tag<int>('c', t);
    }
    void load_override(boost::archive::class_id_optional_type& t)
    {
        boost::archive::class_id_type x;
        this->This()->load_override(x);
        t = boost::archive::class_id_optional_type(x);
    }
    void load_override(boost::archive::class_id_reference_type& t)
    {
        // const auto x = top()["__class_id_ref__"].template as<int>();
        if (!load_from_tag<int>('r', t))
        {
            assert(load_from_tag<int>('c', t));
        }
        // t = boost::archive::class_id_reference_type(load_from_tag<int>('r'));
    }
    void load_override(boost::archive::object_id_type& t)
    {
        // TODO!!!
        // const auto x = top()["__object_id__"].template as<unsigned>();
        // t = boost::archive::object_id_type(x);
    }
    void load_override(boost::archive::object_reference_type& t)
    {
        // TODO!!!
        // const auto x = top()["__object_id_ref__"].template as<unsigned>();
        // t = boost::archive::object_reference_type(x);
    }
    void load_override(boost::archive::version_type& t)
    {
        // const auto x = top()["__version__"].template as<unsigned>();
        // t = boost::archive::version_type(load_from_tag<unsigned>('v'));
        load_from_tag<unsigned>('v', t);
    }
    void load_override(boost::archive::tracking_type& t)
    {
        // const auto x = top()["__tracking__"].template as<bool>();
        // t = boost::archive::tracking_type(load_from_tag<bool>('t'));
        load_from_tag<bool>('t', t);
    }
    void load_override(boost::archive::class_name_type& t)
    {
        // Inspiration from from boost/archive/basic_text_iarchive.hpp
        // const auto cn = top()["__class_name__"].template as<std::string>();
        // if (cn.size() > (BOOST_SERIALIZATION_MAX_KEY_SIZE - 1))
        //     boost::serialization::throw_exception(
        //         boost::archive::archive_exception(
        //             boost::archive::archive_exception::invalid_class_name));
        // std::memcpy(t, cn.data(), cn.size());
    }
};

#include <YAML/nsend.hpp>
