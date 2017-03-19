# This is the main CMake file for the yaml-archive project.
#
# It sets some options and adds the src directory as a cmake subdirectory. The
# CMake file in the src directory is responsible for defining the yaml-archive
# target.
#
# Optionally, it will add the test subdirectory. The CMake file in that
# directory will define test targets to be run by CTest.

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

option(yaml-archive_BUILD_DOCUMENTATION 
    "Build the documentation?" 
    ${yaml-archive_WE_ARE_ROOT})

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
    # ...
endif()
