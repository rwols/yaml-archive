#ifndef BOOST_ARCHIVE_DETAIL_YAML_DECL_HPP
#define BOOST_ARCHIVE_DETAIL_YAML_DECL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

// I would like to use boost/archive/detail/decl.hpp, but the interface was
// changed in boost >= 1.59.

#include <boost/config.hpp>

#if (defined(BOOST_ALL_DYN_LINK) || defined(BOOST_SERIALIZATION_DYN_LINK))
#if defined(BOOST_ARCHIVE_SOURCE)
#define YAML_ARCHIVE_API BOOST_SYMBOL_EXPORT
#else
#define YAML_ARCHIVE_API BOOST_SYMBOL_IMPORT
#endif
#if defined(BOOST_WARCHIVE_SOURCE)
#define YAML_WARCHIVE_API BOOST_SYMBOL_EXPORT
#else
#define YAML_WARCHIVE_API BOOST_SYMBOL_IMPORT
#endif
#if !defined(BOOST_WARCHIVE_SOURCE) && !defined(BOOST_ARCHIVE_SOURCE)
#define YAML_AWARCHIVE_API BOOST_SYMBOL_IMPORT
#endif

#if defined(BOOST_WARCHIVE_SOURCE) || defined(BOOST_ARCHIVE_SOURCE)
#define YAML_AWARCHIVE_API BOOST_SYMBOL_EXPORT
#else
#define YAML_AWARCHIVE_API BOOST_SYMBOL_IMPORT
#endif

#endif

#if !defined(YAML_ARCHIVE_API)
#define YAML_ARCHIVE_API
#endif
#if !defined(YAML_WARCHIVE_API)
#define YAML_WARCHIVE_API
#endif
#if !defined(YAML_AWARCHIVE_API)
#define YAML_AWARCHIVE_API
#endif

#endif // BOOST_ARCHIVE_DETAIL_YAML_DECL_HPP
