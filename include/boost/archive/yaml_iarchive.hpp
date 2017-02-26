#pragma once

#include <boost/archive/detail/common_iarchive.hpp>
#include <boost/archive/detail/decl.hpp>
#include <boost/archive/detail/is_yaml_primitive.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <boost/serialization/detail/stack_constructor.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <stack>
#include <yaml-cpp/yaml.h>

#ifdef BOOST_HAS_SLIST
#include BOOST_SLIST_HEADER
#endif

#define YAML_ARCHIVE_DEBUG_STACK

#ifdef YAML_ARCHIVE_DEBUG_STACK
#include <iostream>
#endif

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost {
namespace archive {

class BOOST_SYMBOL_VISIBLE yaml_iarchive
    : public detail::common_iarchive<yaml_iarchive>
{
  public:
    BOOST_SYMBOL_VISIBLE yaml_iarchive(std::istream&  is,
                                       const unsigned flags = 0);

    BOOST_SYMBOL_VISIBLE ~yaml_iarchive() noexcept = default;

    BOOST_SYMBOL_VISIBLE
    void load_binary(void* address, std::size_t count);

  private:
    friend class detail::interface_iarchive<yaml_iarchive>;
    friend class load_access;

    std::stack<YAML::Node> m_stack;

#ifdef YAML_ARCHIVE_DEBUG_STACK
    void debug_print_stack();
#endif

    using base = detail::common_iarchive<yaml_iarchive>;
    template <class T> using nvp = boost::serialization::nvp<T>;

    // Anything not an attribute and not a name-value pair is an
    // error and should be trapped here.
    template <class T> void load_override(T& t)
    {
        // If your program fails to compile here, its most likely due to
        // not specifying an nvp wrapper around the variable to
        // be serialized.
        static_assert(serialization::is_wrapper<T>::type::value,
                      "wrap your values to be serialized in the "
                      "BOOST_SERIALIZATION_NVP wrapper, or use the function "
                      "boost::serialization::make_nvp");
        base::load_override(t);
    }

    // "main" entry point into the archive.
    // except for special attributes, see below.
    template <class T> void load_override(const nvp<T>& t)
    {
        if (t.name() != nullptr)
        {
            m_stack.push(m_stack.top()[t.name()]);
#ifdef YAML_ARCHIVE_DEBUG_STACK
            std::cout << "PUSHED: " << t.name() << '\n';
            debug_print_stack();
#endif
        }
        load_value(t.value());
        if (t.name() != nullptr)
        {
            m_stack.pop();
#ifdef YAML_ARCHIVE_DEBUG_STACK
            std::cout << "POPPED: " << t.name() << '\n';
            debug_print_stack();
#endif
        }
    }

    // If T is a yaml primitive, cast the YAML::Node at the top of the stack to
    // T right away.
    template <class T>
    typename std::enable_if<detail::is_yaml_primitive<
        typename std::remove_cv<T>::type>::value>::type
    load_value(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: YAML::Node::as\n";
#endif
        load(t);
    }

    // Otherwise, invoke the boost::serialization machinery, non-pointer version
    template <class T>
    typename std::enable_if<
        !detail::is_yaml_primitive<typename std::remove_cv<T>::type>::value &&
        !boost::is_pointer<T>::value>::type
    load_value(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout
            << "INVOKE: boost::serialization machinery (non-pointer version)\n";
#endif
        base::load_override(t);
    }

    // Otherwise, invoke the boost::serialization machinery, pointer version
    template <class T>
    typename std::enable_if<
        !detail::is_yaml_primitive<typename std::remove_cv<T>::type>::value &&
        boost::is_pointer<T>::value>::type
    load_value(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout
            << "INVOKE: boost::serialization machinery (pointer version)\n";
#endif
        if (m_stack.top().IsNull())
        {
            t = nullptr;
        }
        else
        {
            base::load_override(t);
        }
    }

    template <std::size_t N, class T> void load_fixed_size_array(T& t)
    {
        auto node = m_stack.top();
        if (node.size() < N)
            throw archive_exception(archive_exception::array_size_too_short);
        for (std::size_t i = 0; i != N; ++i)
        {
            m_stack.push(node[i]);
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
            load_value(t[i]);
            m_stack.pop();
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
        }
    }

    template <class T, std::size_t N> void load_value(T (&t)[N])
    {
        load_fixed_size_array<N>(t);
    }
    template <class T, std::size_t N> void load_value(std::array<T, N>& t)
    {
        load_fixed_size_array<N>(t);
    }
    template <class T, std::size_t N> void load_value(boost::array<T, N>& t)
    {
        load_fixed_size_array<N>(t);
    }

    template <class T, class Alloc>
    void
        // typename std::enable_if<!detail::is_yaml_primitive<T>::value>::type
        load_value(std::vector<T, Alloc>& t)
    {
        load_sequence(t);
    }
    template <class Alloc> void load_value(std::vector<bool, Alloc>& t)
    {

        if (!m_stack.top().IsScalar())
        {
            boost::serialization::throw_exception(
                archive_exception(archive_exception::input_stream_error))
        }
        const auto& bits = m_stack.top().Scalar();
        t.clear();
        t.reserve(bits.size());
        for (const auto b : bits)
        {
            switch (b)
            {
            case '0':
                t.push_back(false);
                break;
            case '1':
                t.push_back(true);
                break;
            default:
                boost::serialization::throw_exception(
                    archive_exception(archive_exception::input_stream_error));
            }
        }
    }
    template <class T, class Alloc>
    void
        // typename std::enable_if<!detail::is_yaml_primitive<T>::value>::type
        load_value(std::deque<T, Alloc>& t)
    {
        load_sequence(t);
    }
    template <class T, class Alloc>
    void
        // typename std::enable_if<!detail::is_yaml_primitive<T>::value>::type
        load_value(std::list<T, Alloc>& t)
    {
        load_sequence(t);
    }
    template <class T, class Alloc>
    void load_value(std::forward_list<T, Alloc>& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load_forward_list\n";
#endif
        const auto& node = m_stack.top();
        for (std::size_t i = node.size() - 1; i != (std::size_t)-1; --i)
        {
            m_stack.push(node[i]);
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
            boost::serialization::detail::stack_construct<yaml_iarchive, T>
                temp(*this, get_library_version());
            load_value(temp.reference());
            t.push_front(temp.reference());
            reset_object_address(&(t.front()), &temp.reference());
            m_stack.pop();
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
        }
    }

    template <class T, class Alloc>
    void load_value(BOOST_STD_EXTENSION_NAMESPACE::slist<T, Alloc>& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load slist\n";
#endif
        const auto& node = m_stack.top();
        for (std::size_t i = node.size() - 1; i != (std::size_t)-1; --i)
        {
            m_stack.push(node[i]);
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
            boost::serialization::detail::stack_construct<yaml_iarchive, T>
                temp(*this, get_library_version());
            load_value(temp.reference());
            t.push_front(temp.reference());
            reset_object_address(&(t.front()), &temp.reference());
            m_stack.pop();
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
        }
    }

    template <class Key, class T, class Compare, class Alloc>
    void load_value(std::map<Key, T, Compare, Alloc>& t)
    {
        load_map(t);
    }

    template <class Key, class T, class Compare, class Alloc>
    void load_value(std::multimap<Key, T, Compare, Alloc>& t)
    {
        load_multimap(t);
    }

    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void load_value(std::unordered_map<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        load_map(t);
    }

    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void load_value(std::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        load_multimap(t);
    }

    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void load_value(boost::unordered_map<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        load_map(t);
    }

    template <class Key, class T, class Hash, class KeyEqual, class Alloc>
    void load_value(boost::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>& t)
    {
        load_multimap(t);
    }

    template <class Key, class Compare, class Alloc>
    void load_value(std::set<Key, Compare, Alloc>& t)
    {
        load_set(t);
    }

    template <class Key, class Compare, class Alloc>
    void load_value(std::multiset<Key, Compare, Alloc>& t)
    {
        load_set(t);
    }

    // note: std::pair requires both of its template parameters to be default
    // constructible, so we don't have to use this elaborate stack_construct
    // object.
    template <class First, class Second>
    void
        // typename std::enable_if<!detail::is_yaml_primitive<First>::value ||
        //                         !detail::is_yaml_primitive<Second>::value>::type
        load_value(std::pair<First, Second>& t)
    {
        typedef typename boost::remove_const<First>::type TypeF;
        m_stack.push(m_stack.top()[0]);
        load_value(const_cast<TypeF&>(t.first));
        m_stack.pop();
        m_stack.push(m_stack.top()[1]);
        load_value(t.second);
        m_stack.pop();
    }

    template <class T>
    typename std::enable_if<
        !std::is_default_constructible<typename T::value_type>::value>::type
    load_sequence(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load_sequence\n";
#endif
        auto node = m_stack.top();
        for (auto item : node)
        {
            using serialization::detail::stack_construct;
            m_stack.push(item);

            m_stack.push(m_stack.top()["construct_data"]);
            stack_construct<yaml_iarchive, typename T::value_type> temp(
                *this, get_library_version());
            m_stack.pop(); // construct_data

            m_stack.push(m_stack.top()["object"]);
            load_value(temp.reference());
            m_stack.pop(); // object

            t.push_back(temp.reference());
            reset_object_address(&(t.back()), &temp.reference());

            m_stack.pop(); // item
        }
    }

    template <class T>
    typename std::enable_if<
        std::is_default_constructible<typename T::value_type>::value>::type
    load_sequence(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load_sequence\n";
#endif
        auto node = m_stack.top();
        for (auto item : node)
        {
            using serialization::detail::stack_construct;
            stack_construct<yaml_iarchive, typename T::value_type> temp(
                *this, get_library_version());
            m_stack.push(item);
            load_value(temp.reference());
            m_stack.pop(); // item
            t.push_back(temp.reference());
            reset_object_address(&(t.back()), &temp.reference());
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
        }
    }

    template <class T> void load_map(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load_map\n";
#endif
        auto node = m_stack.top();
        for (auto kv : node)
        {
            using serialization::detail::stack_construct;
            stack_construct<yaml_iarchive, typename T::key_type> tempkey(
                *this, get_library_version());

            stack_construct<yaml_iarchive, typename T::mapped_type> tempvalue(
                *this, get_library_version());

            m_stack.push(kv.first);
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
            load_value(tempkey.reference());
            m_stack.pop();
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
            m_stack.push(kv.second);
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
            load_value(tempvalue.reference());
            m_stack.pop();
            auto emplace_result =
                t.emplace(tempkey.reference(), tempvalue.reference());
            assert(emplace_result.second);
            reset_object_address(&(emplace_result.first->first),
                                 &tempkey.reference());
            reset_object_address(&(emplace_result.first->second),
                                 &tempvalue.reference());

#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
        }
    }

    template <class T> void load_multimap(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load_multimap\n";
#endif
        auto node = m_stack.top();
        auto hint = t.begin();
        for (auto item : node)
        {
            m_stack.push(item);
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
            typedef typename T::value_type type;
            serialization::detail::stack_construct<yaml_iarchive, type> temp(
                *this, get_library_version());
            load_value(temp.reference());
            auto result = t.emplace_hint(hint, temp.reference());
            m_stack.pop();
            reset_object_address(&(result->second), &temp.reference().second);
            hint = result;
            ++hint;
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
        }
    }

    template <class T>
    typename std::enable_if<
        !std::is_default_constructible<typename T::value_type>::value>::type
    load_set(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load_set\n";
#endif
        auto node = m_stack.top();
        for (auto item : node)
        {
            using serialization::detail::stack_construct;
            m_stack.push(item);

            m_stack.push(m_stack.top()["construct_data"]);
            stack_construct<yaml_iarchive, typename T::value_type> temp(
                *this, get_library_version());
            m_stack.pop(); // construct_data

            m_stack.push(m_stack.top()["object"]);
            load_value(temp.reference());
            m_stack.pop(); // object

            auto iter = t.insert(t.end(), temp.reference());
            reset_object_address(&(*iter), &temp.reference());

            m_stack.pop(); // item
        }
    }

    template <class T>
    typename std::enable_if<
        std::is_default_constructible<typename T::value_type>::value>::type
    load_set(T& t)
    {
#ifdef YAML_ARCHIVE_DEBUG_STACK
        std::cout << "INVOKE: load_set\n";
#endif
        auto node = m_stack.top();
        for (auto item : node)
        {
            using serialization::detail::stack_construct;
            stack_construct<yaml_iarchive, typename T::value_type> temp(
                *this, get_library_version());
            m_stack.push(item);
            load_value(temp.reference());
            m_stack.pop(); // item
            auto iter = t.insert(t.end(), temp.reference());
            reset_object_address(&(*iter), &temp.reference());
#ifdef YAML_ARCHIVE_DEBUG_STACK
            debug_print_stack();
#endif
        }
    }

    template <class T> void load(T& t)
    {
        t = m_stack.top().template as<typename std::remove_cv<T>::type>();
    }

    void load(wchar_t& t);
    void load(std::wstring& t);
    void load(char& t);
    void load(signed char& t);
    void load(unsigned char& t);
    void load(boost::serialization::item_version_type& t);
    void load(boost::serialization::collection_size_type& t);

    // specific overrides for attributes - not name value pairs so we
    // want to trap them before the above "fall through"
    BOOST_SYMBOL_VISIBLE void load_override(class_id_type& t);
    BOOST_SYMBOL_VISIBLE void load_override(class_id_optional_type& t);
    BOOST_SYMBOL_VISIBLE void load_override(class_id_reference_type& t);
    BOOST_SYMBOL_VISIBLE void load_override(object_id_type& t);
    BOOST_SYMBOL_VISIBLE void load_override(object_reference_type& t);
    BOOST_SYMBOL_VISIBLE void load_override(version_type& t);
    BOOST_SYMBOL_VISIBLE void load_override(class_name_type& t);
    BOOST_SYMBOL_VISIBLE void load_override(tracking_type& t);
};

} // namespace archive
} // namespace boost

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(boost::archive::yaml_iarchive)
