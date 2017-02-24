#ifndef YAML_ARCHIVE_DETAIL_DECL_HPP
#define YAML_ARCHIVE_DETAIL_DECL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
#pragma once
#endif

// this is a port of 1.63 boost/archive/detail/decl.hpp

//----------------------------------------------------------------------------//

// This header implements separate compilation features as described in
// http://www.boost.org/more/separate_compilation.html

#include <boost/config.hpp>

#if (defined(BOOST_ALL_DYN_LINK) || defined(BOOST_SERIALIZATION_DYN_LINK))
#if defined(BOOST_ARCHIVE_SOURCE)
#define YAML_ARCHIVE_DECL BOOST_SYMBOL_EXPORT
#else
#define YAML_ARCHIVE_DECL BOOST_SYMBOL_IMPORT
#endif

#if defined(BOOST_WARCHIVE_SOURCE)
#define BOOST_WARCHIVE_DECL BOOST_SYMBOL_EXPORT
#else
#define BOOST_WARCHIVE_DECL BOOST_SYMBOL_IMPORT
#endif

#if defined(BOOST_WARCHIVE_SOURCE) || defined(BOOST_ARCHIVE_SOURCE)
#define BOOST_ARCHIVE_OR_WARCHIVE_DECL BOOST_SYMBOL_EXPORT
#else
#define BOOST_ARCHIVE_OR_WARCHIVE_DECL BOOST_SYMBOL_IMPORT
#endif

#endif

#if !defined(YAML_ARCHIVE_DECL)
#define YAML_ARCHIVE_DECL
#endif
#if !defined(BOOST_WARCHIVE_DECL)
#define BOOST_WARCHIVE_DECL
#endif
#if !defined(BOOST_ARCHIVE_OR_WARCHIVE_DECL)
#define BOOST_ARCHIVE_OR_WARCHIVE_DECL
#endif

#endif // YAML_ARCHIVE_DETAIL_DECL_HPP
