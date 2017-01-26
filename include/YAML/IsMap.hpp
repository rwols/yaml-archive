/**
 * @author Raoul Wols
 *
 * @file   Defines which types are "YAML map like".
 */

#pragma once

#include <map>
#include <unordered_map>

#include <YAML/nsbegin.hpp>

template <class T> struct IsMap_
{
    static constexpr bool value = false;
};

template <class T> static constexpr bool IsMap = IsMap_<T>::value;

template <class Key, class T, class Compare, class Alloc>
struct IsMap_<std::map<Key, T, Compare, Alloc>>
{
    static constexpr bool value = true;
};

template <class Key, class T, class Hash, class KeyEqual, class Alloc>
struct IsMap_<std::unordered_map<Key, T, Hash, KeyEqual, Alloc>>
{
    static constexpr bool value = true;
};

#include <YAML/nsend.hpp>
