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
         * @brief      The key that was not present during deserialization.
         *
         * @return     The key that was not present during deserialization.
         */
        const char* key() const noexcept { return m_key; }
      private:
        friend class BasicInputArchive;
        const char* m_key;
        KeyNotPresentError(const char* whichKey) noexcept
            : boost::archive::archive_exception(
                  boost::archive::archive_exception::other_exception),
              m_key(whichKey)
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

    void load_start(const char* key)
    {
        if (top()[key])
        {
            m_stack.push(top()[key]);
        }
        else
        {
            throw KeyNotPresentError(key);
        }
    }

    void load_end() noexcept { m_stack.pop(); }

    // If T is a YAML type (i.e. "primitive" type), then we know what to do with
    // it.
    template <class T>
    EnableIf<IsPrimitive<T>> load_override(const KeyValue<T>& t)
    {
        t.value() = top()[t.name()].template as<T>();
    }

    template <class T> EnableIf<IsPrimitive<T>> load(T& t)
    {
        t = top().template as<T>();
    }

    template <class T> EnableIf<IsPrimitive<T>> load_override(T& t)
    {
        t = top().template as<T>();
    }

    std::size_t m_seq_index = -1;

    // If T is not a YAML type, we invoke the serialization machinery of boost.
    template <class T>
    EnableIf<!IsPrimitive<T> && !IsMap<T> && !IsSequence<T>>
    load_override(const KeyValue<T>& t)
    {
        load_start(t.name());
        this->detail_common_iarchive::load_override(t.value());
        load_end();
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
        const Node tempcopy = top();
        for (const auto& kv : tempcopy)
        {
            m_stack.push(kv.second);
            typename MapLike::mapped_type x;
            this->detail_common_iarchive::load_override(x);
            t.value().emplace(kv.first.as<std::string>(), std::move(x));
            m_stack.pop();
        }
    }
    template <class MapLike>
    EnableIf<
        !std::is_default_constructible<typename MapLike::mapped_type>::value>
    load_map(const KeyValue<MapLike>& t)
    {
        const Node tempcopy = top();
        for (const auto& kv : tempcopy)
        {
            m_stack.push(kv.second);
            // taken from: boost/serialization/collections_load_imp.hpp
            boost::serialization::detail::stack_construct<
                Archive, typename MapLike::mapped_type>
                u(*this->This(), 0);
            this->detail_common_iarchive::load_override(u.reference());
            t.value().emplace(kv.second.as<std::string>(), u.reference());
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

    template <class ArithmeticType>
    ArithmeticType load_from_tag(const char identifier)
    {
        static_assert(std::is_arithmetic<ArithmeticType>::value,
                      "ArithmeticType must be arithmetic.");
        const auto pos = top().Tag().find(identifier);
        if (pos == std::string::npos)
        {
            throw std::runtime_error("failed to load meta.");
        }
        const char* iter = top().Tag().data() + pos + 1;
        ArithmeticType x = 0;
        while (isdigit(*iter))
        {
            x *= 10;
            x += *iter - '0';
            ++iter;
        }
        return x;
    }

    // special handling for bool
    bool load_from_tag(const char identifier)
    {
        return top().Tag().find(identifier) != std::string::npos;
    }

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    void load_override(boost::archive::class_id_type& t)
    {
        // const auto x = top()["__class_id__"].template as<int>();
        t = boost::archive::class_id_type(load_from_tag<int>('c'));
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
        t = boost::archive::class_id_reference_type(load_from_tag<int>('r'));
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
        t = boost::archive::version_type(load_from_tag<unsigned>('v'));
    }
    void load_override(boost::archive::tracking_type& t)
    {
        // const auto x = top()["__tracking__"].template as<bool>();
        t = boost::archive::tracking_type(load_from_tag<bool>('t'));
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
