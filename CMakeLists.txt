# This is the main CMake file for the yaml-archive project.
#
# It sets some options and adds the src directory as a cmake subdirectory. The
# CMake file in the src directory is responsible for defining the yaml-archive
# target.
#
# Optionally, it will add the test subdirectory. The CMake file in that
# directory will define test targets to be run by CTest.
#
# Optionally, it will add the doc subdirectory. The CMake file in that directory
# will define the doc target to build the html documentation. Doxygen is
# required to build the documentation. If doxygen is not found, the doc
# target will not be available.

cmake_minimum_required(VERSION 3.0)
project(yaml-archive VERSION 0.1.0 LANGUAGES CXX)
set(yaml-archive_WE_ARE_ROOT OFF CACHE INTERNAL "")
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(yaml-archive_WE_ARE_ROOT ON CACHE INTERNAL "")
endif()

################################################################################
#                                                                              #
#    Options                                                                   #
#                                                                              #
################################################################################

option(yaml-archive_USE_STATIC_LIBS 
    "Build static libraries instead of dynamic ones." 
    ${BUILD_STATIC_LIBS})

option(yaml-archive_BUILD_UNIT_TESTS 
    "Build the unit tests?" 
    ${yaml-archive_WE_ARE_ROOT})

find_package(Doxygen)
if (${DOXYGEN_FOUND} AND ${yaml-archive_WE_ARE_ROOT})
    option(yaml-archive_BUILD_DOCUMENTATION 
        "Build the documentation?" 
        ON)
else()
    option(yaml-archive_BUILD_DOCUMENTATION 
        "Build the documentation?" 
        OFF)
endif()

set(yaml-archive_LIB_DESTINATION lib
    CACHE PATH
    "Where to install the library")

set(yaml-archive_INCLUDE_DESTINATION .
    CACHE PATH
    "Where to install the headers")

################################################################################
#                                                                              #
#    Target yaml-archive                                                       #
#                                                                              #
################################################################################

add_subdirectory(src)

################################################################################
#                                                                              #
#    Unit tests                                                                #
#                                                                              #
################################################################################

if (yaml-archive_BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

################################################################################
#                                                                              #
#    Documentation (with doxygen)                                              #
#                                                                              #
################################################################################

if (yaml-archive_BUILD_DOCUMENTATION)
    add_subdirectory(doc)
endif()
