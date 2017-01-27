/**
 * @author Raoul Wols
 *
 * @file   Defines common includes for both BasicInputArchive and
 *         BasicOutputArchive
 */

#pragma once

#include <YAML/IsMap.hpp>
#include <YAML/IsPrimitive.hpp>
#include <YAML/IsSequence.hpp>
#include <YAML/KeyValue.hpp>
#include <yaml-cpp/yaml.h>

#include <YAML/nsbegin.hpp>

enum class ArchiveFlag : unsigned
{
    Null = 0,                      // no flags.
    NoHeader = 1 << 0,             // suppress %YAML and %TAG lines.
    NoDocumentDelimiters = 1 << 1, // suppress "---" and "...".
    NoTags = 1 << 2,               // suppress tag metadata.
    RootIsSequence = 1 << 3        // make the root of the doc a sequence
                                   // instead of a map.
};

inline ArchiveFlag operator|(const ArchiveFlag lhs, const ArchiveFlag rhs)
{
    return static_cast<ArchiveFlag>(static_cast<unsigned>(lhs) |
                                    static_cast<unsigned>(rhs));
}

inline ArchiveFlag& operator|=(ArchiveFlag& lhs, const ArchiveFlag rhs)
{
    return lhs = (lhs | rhs);
}

#include <YAML/nsend.hpp>
