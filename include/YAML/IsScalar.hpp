/**
 * @author Raoul Wols
 *
 * @file   Defines which types are "YAML scalars".
 */

#pragma once

#include <string>

#include <YAML/nsbegin.hpp>

template <class T> struct IsScalar_
{
    static constexpr bool value = false;
};

template <class T> constexpr bool IsScalar = IsScalar_<T>::value;

template <> struct IsScalar_<int>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<short>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<long>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<long long>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<unsigned>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<unsigned short>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<unsigned long>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<unsigned long long>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<char>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<signed char>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<unsigned char>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<float>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<double>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<long double>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<bool>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<std::string>
{
    static constexpr bool value = true;
};
template <> struct IsScalar_<char*>
{
    static constexpr bool value = true;
};

#include <YAML/nsend.hpp>
