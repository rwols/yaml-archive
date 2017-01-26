/**
 * @author Raoul Wols
 *
 * @file   Creates a simple typedef for boost::serialization::nvp.
 */

#pragma once

#include <boost/serialization/nvp.hpp>

#include <YAML/nsbegin.hpp>

template <class T> using KeyValue = boost::serialization::nvp<T>;

#include <YAML/nsend.hpp>
