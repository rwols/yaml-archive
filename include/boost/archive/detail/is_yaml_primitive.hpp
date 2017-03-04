#pragma once

#include <YAML/extra_converters.hpp>
#include <YAML/extra_emitter_overloads.hpp>
#include <boost/config.hpp>
#include <type_traits>

namespace boost {
namespace archive {
namespace detail {

/**
 * @brief      Metafuction to decree a type T as a yaml primitive. By default,
 *             nothing is a yaml primitive.
 *
 * @tparam     T     The type to decree a yaml primitive type. Note that
 *                   YAML::Emitter must support this type. That is, if `t` is of
 *                   type `T` and `emitter` is of type `YAML::Emitter`, then the
 *                   expression `emitter << t;` must be valid.
 */
template <class T> struct is_yaml_primitive : public std::false_type
{
};

/**
 * @brief      Helper macro for quickly defining simple yaml primitives.
 *
 * @param      TYPE  The type to decree yaml primitive.
 */
#define YAML_PRIM(TYPE)                                                        \
    template <> struct is_yaml_primitive<TYPE> : public std::true_type         \
    {                                                                          \
    }
YAML_PRIM(bool);
YAML_PRIM(short);
YAML_PRIM(int);
YAML_PRIM(long);
YAML_PRIM(long long);
YAML_PRIM(unsigned);
YAML_PRIM(unsigned short);
YAML_PRIM(unsigned long);
YAML_PRIM(unsigned long long);
YAML_PRIM(char);
YAML_PRIM(wchar_t);
YAML_PRIM(signed char);
YAML_PRIM(unsigned char);
YAML_PRIM(float);
YAML_PRIM(double);
YAML_PRIM(long double);
YAML_PRIM(std::string);
YAML_PRIM(std::wstring);
#undef YAML_PRIM

/**
 * @brief      Helper macro for defining yaml sequence primitives.
 *
 * @param      SEQ   The sequence's C++ type
 */
#define YAML_SEQ_PRIM(SEQ)                                                     \
    template <class T, class Alloc>                                            \
    struct is_yaml_primitive<std::SEQ<T, Alloc>>                               \
    {                                                                          \
        static constexpr bool value =                                          \
            is_yaml_primitive<typename std::remove_cv<T>::type>::value;        \
    }
YAML_SEQ_PRIM(vector);
YAML_SEQ_PRIM(list);
YAML_SEQ_PRIM(deque);
#ifndef BOOST_NO_CXX11_HDR_FORWARD_LIST
YAML_SEQ_PRIM(forward_list);
#endif
#undef YAML_SEQ_PRIM

template <class T, std::size_t N> struct is_yaml_primitive<std::array<T, N>>
{
    static constexpr bool value = is_yaml_primitive<T>::value;
};

// template <class T, std::size_t N> struct is_yaml_primitive<boost::array<T,
// N>>
// {
//     static constexpr bool value = is_yaml_primitive<T>::value;
// };

template <class First, class Second>
struct is_yaml_primitive<std::pair<First, Second>>
{
    static constexpr bool value =
        is_yaml_primitive<typename std::remove_cv<First>::type>::value &&
        is_yaml_primitive<typename std::remove_cv<Second>::type>::value;
};

template <class Key, class T, class Compare, class Alloc>
struct is_yaml_primitive<std::map<Key, T, Compare, Alloc>>
{
    static constexpr bool value =
        is_yaml_primitive<Key>::value && is_yaml_primitive<T>::value;
};

template <class Key, class T, class Compare, class Alloc>
struct is_yaml_primitive<std::multimap<Key, T, Compare, Alloc>>
{
    static constexpr bool value =
        is_yaml_primitive<Key>::value && is_yaml_primitive<T>::value;
};

template <class Key, class Compare, class Alloc>
struct is_yaml_primitive<std::set<Key, Compare, Alloc>>
{
    static constexpr bool value = is_yaml_primitive<Key>::value;
};

} // namespace detail
} // namespace archive
} // namespace boost
