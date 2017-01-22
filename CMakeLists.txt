cmake_minimum_required(VERSION 3.0)
project(yaml-archive VERSION 0.1.0 LANGUAGES CXX)
set(yaml-archive_WE_ARE_ROOT OFF CACHE INTERNAL "")
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(yaml-archive_WE_ARE_ROOT ON CACHE INTERNAL "")
endif()
option(yaml-archive_USE_STATIC_LIBS "Build static libraries instead of dynamic ones." ${BUILD_STATIC_LIBS})
option(yaml-archive_BUILD_UNIT_TESTS "Build unit tests." ${yaml-archive_WE_ARE_ROOT})
option(yaml-archive_BUILD_DOCUMENTATION "Build documentation." ${yaml-archive_WE_ARE_ROOT})
add_subdirectory(ext)
add_subdirectory(include)
add_subdirectory(lib)

if (yaml-archive_BUILD_UNIT_TESTS)
    enable_testing()
    # ...
endif()

if (yaml-archive_BUILD_DOCUMENTATION)
    # ...
endif()
