#pragma once

#include <array>
#include <boost/array.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <deque>
#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
#include <forward_list>
#endif
#include <list>
#include <map>
#include <set>
#ifndef BOOST_NO_CXX11_HDR_TUPLE
#include <tuple>
#endif
#ifndef BOOST_NO_CXX11_HDR_UNORDERED_MAP
#include <unordered_map>
#endif
#ifndef BOOST_NO_CXX11_HDR_UNORDERED_SET
#include <unordered_set>
#endif
#include <vector>
#include <yaml-cpp/node/convert.h>

namespace YAML {
namespace detail {

template <class T> struct convert_sequence
{
    Node encode(const T& t)
    {
        Node node(NodeType::Sequence);
        for (const auto& item : t) node.push_back(item);
        return node;
    }
    bool decode(const Node& node, T& t)
    {
        t.clear();
        switch (node.Type())
        {
        case NodeType::Null:
            // OK: empty sequence
            return true;
        case NodeType::Scalar:
            // OK: sequence with one element
            t.push_back(node.template as<typename T::value_type>());
            return true;
        case NodeType::Sequence:
            for (const auto& item : node)
            {
                t.push_back(item.template as<typename T::value_type>());
            }
            return true;
        default:
            return false;
        }
    }
};

template <class T> struct convert_map
{
    Node encode(const T& t)
    {
        Node node(NodeType::Map);
        for (const auto& kv : t)
        {
            node.force_insert(kv.first, kv.second);
        }
        return node;
    }
    bool decode(const Node& node, T& t)
    {
        t.clear();
        switch (node.Type())
        {
        case NodeType::Null:
            // OK: empty map
            return true;
        case NodeType::Map:
            for (const auto& kv : node)
            {
                t.emplace(kv.first.template as<typename T::key_type>(),
                          kv.second.template as<typename T::mapped_type>());
            }
            return true;
        default:
            return false;
        }
    }
};

template <class T> struct convert_multimap
{
    Node encode(const T& t)
    {
        Node node(NodeType::Sequence);
        for (const auto& kv : t)
        {
            node.push_back(kv);
        }
        return node;
    }
    bool decode(const Node& node, T& t)
    {
        t.clear();
        switch (node.Type())
        {
        case NodeType::Null:
            // OK: empty map
            return true;
        case NodeType::Sequence:
            for (const auto& kv : node)
            {
                t.emplace(kv.template as<std::pair<typename T::key_type,
                                                   typename T::mapped_type>>());
            }
            return true;
        default:
            return false;
        }
    }
};

} // namespace detail

template <class T, class Alloc>
struct convert<std::vector<T, Alloc>>
    : public detail::convert_sequence<std::vector<T, Alloc>>
{
};

template <class T, class Alloc>
struct convert<std::deque<T, Alloc>>
    : public detail::convert_sequence<std::deque<T, Alloc>>
{
};

template <class T, class Alloc>
struct convert<std::list<T, Alloc>>
    : public detail::convert_sequence<std::list<T, Alloc>>
{
};

#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
template <class T, class Alloc> struct convert<std::forward_list<T, Alloc>>
{
    Node encode(const std::forward_list<T, Alloc>& t)
    {
        Node node(NodeType::Sequence);
        for (const auto& item : t) node.push_back(item);
        return node;
    }
    bool decode(const Node& node, std::forward_list<T, Alloc>& t)
    {
        t.clear();
        switch (node.Type())
        {
        case NodeType::Null:
            // OK: empty forward_list
            return true;
        case NodeType::Scalar:
            // OK: forward_list with one element
            t.push_front(node.template as<T>());
            return true;
        case NodeType::Sequence:
            for (std::size_t i = node.size() - 1; i != (std::size_t)-1; --i)
            {
                t.push_front(node[i].template as<T>());
            }
            return true;
        default:
            return false;
        }
    }
};
#endif // BOOST_NO_CXX11_HDR_FORWARD_LIST

template <class T, std::size_t N> struct convert<boost::array<T, N>>
{
    Node encode(const boost::array<T, N>& t)
    {
        Node node(NodeType::Sequence);
        for (const auto& item : t) node.push_back(item);
        return node;
    }

    bool decode(const Node& node, boost::array<T, N>& t)
    {
        if (!node.IsSequence()) return false;
        if (node.size() < N) return false;
        for (std::size_t i = 0; i != N; ++i)
        {
            t[i] = node[i].template as<T>();
        }
    }
};

template <class Key, class T, class Compare, class Alloc>
struct convert<std::map<Key, T, Compare, Alloc>>
    : public detail::convert_map<std::map<Key, T, Compare, Alloc>>
{
};

template <class Key, class T, class Compare, class Alloc>
struct convert<std::multimap<Key, T, Compare, Alloc>>
    : public detail::convert_multimap<std::multimap<Key, T, Compare, Alloc>>
{
};

#ifndef BOOST_NO_CXX11_HDR_UNORDERED_MAP
template <class Key, class T, class Hash, class KeyEqual, class Alloc>
struct convert<std::unordered_map<Key, T, Hash, KeyEqual, Alloc>>
    : public detail::convert_map<
          std::unordered_map<Key, T, Hash, KeyEqual, Alloc>>
{
};

template <class Key, class T, class Hash, class KeyEqual, class Alloc>
struct convert<std::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>>
    : public detail::convert_map<
          std::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>>
{
};
#endif // BOOST_NO_CXX11_HDR_UNORDERED_MAP

template <class Key, class T, class Hash, class KeyEqual, class Alloc>
struct convert<boost::unordered_map<Key, T, Hash, KeyEqual, Alloc>>
    : public detail::convert_map<
          boost::unordered_map<Key, T, Hash, KeyEqual, Alloc>>
{
};

template <class Key, class T, class Hash, class KeyEqual, class Alloc>
struct convert<boost::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>>
    : public detail::convert_multimap<
          boost::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>>
{
};

} // namespace YAML
