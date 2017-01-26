/**
 * @author Raoul Wols
 *
 * @file   Defines which types are "YAML primitives".
 */

#pragma once

#include <YAML/IsScalar.hpp>
#include <list>
#include <map>
#include <vector>
// TODO: forward_list, unordered_map ?

#include <YAML/nsbegin.hpp>

template <class T> struct IsPrimitive_
{
    static constexpr bool value = IsScalar<T>;
};

template <class T> static constexpr bool IsPrimitive = IsPrimitive_<T>::value;

template <class T, class Alloc> struct IsPrimitive_<std::vector<T, Alloc>>
{
    static constexpr bool value = IsPrimitive_<T>::value;
};

template <class T, class Alloc> struct IsPrimitive_<std::list<T, Alloc>>
{
    static constexpr bool value = IsPrimitive_<T>::value;
};

template <class Key, class T, class Compare, class Alloc>
struct IsPrimitive_<std::map<Key, T, Compare, Alloc>>
{
    static constexpr bool value =
        IsPrimitive_<Key>::value && IsPrimitive_<T>::value;
};

#include <YAML/nsend.hpp>
