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

# if (BUILD_SHARED_LIBS)
#     set(Boost_USE_STATIC_LIBS OFF)
# else()
#     set(Boost_USE_STATIC_LIBS ON)
# endif()
# find_package(Boost 1.54 COMPONENTS system serialization REQUIRED)

# add_library(yaml-archive
#     lib/yaml_iarchive.cpp
#     lib/yaml_oarchive.cpp)

# set_target_properties(yaml-archive
#     PROPERTIES
#         CXX_STANDARD 
#             11
#         PUBLIC_HEADERS 
#             include/boost/archive/detail/is_yaml_primitive.hpp
#             include/boost/archive/yaml_iarchive.hpp
#             include/boost/archive/yaml_oarchive.hpp
#             include/YAML/extra_converters.hpp
#             include/YAML/extra_emitter_overloads.hpp
#     )

# target_include_directories(yaml-archive PUBLIC  
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  
#     $<INSTALL_INTERFACE:include>
# )

# target_include_directories(yaml-archive PUBLIC 
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ext/yaml-cpp/include> 
#     # No way to get at the install include dir of yaml-cpp right now,
#     # so just "guess" the yaml-cpp include directory.
#     $<INSTALL_INTERFACE:include>)

# target_include_directories(yaml-archive PUBLIC ${Boost_INCLUDE_DIR})

# include(GenerateExportHeader)
# generate_export_header(yaml-archive)

# install(TARGETS yaml-archive
#     EXPORT yaml-archive-targets
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
#     PUBLIC_HEADER DESTINATION ${yaml-archive_INCLUDE_DESTINATION})

# target_link_libraries(yaml-archive PUBLIC yaml-cpp)
# target_link_libraries(yaml-archive PUBLIC ${Boost_LIBRARIES})

# include(CMakePackageConfigHelpers)
# write_basic_package_version_file(
#     "${CMAKE_CURRENT_BINARY_DIR}/yaml-archive-config-version.cmake"
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY AnyNewerVersion
# )

# export(EXPORT yaml-archive-targets
#   FILE "${CMAKE_CURRENT_BINARY_DIR}/yaml-archive-config-version.cmake"
# )
# configure_file(yaml-archive-config.cmake
#   "${CMAKE_CURRENT_BINARY_DIR}/yaml-archive-config.cmake"
#   COPYONLY
# )

# set(ConfigPackageLocation lib/cmake/yaml-archive)
# install(EXPORT yaml-archive-targets
#     FILE yaml-archive-targets.cmake
#     DESTINATION ${ConfigPackageLocation}
# )
# install(
#     FILES
#         yaml-archive-config.cmake
#         "${CMAKE_CURRENT_BINARY_DIR}/yaml-archive-config-version.cmake"
#     DESTINATION ${ConfigPackageLocation}
#     COMPONENT develop
# )

# install(
#     DIRECTORY include
#     DESTINATION ${yaml-archive_INCLUDE_DESTINATION}
#     FILES_MATCHING PATTERN "*.hpp"
# )

# add_subdirectory(ext)
# add_subdirectory(include)
# add_subdirectory(lib)
add_subdirectory(src)

if (yaml-archive_BUILD_UNIT_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if (yaml-archive_BUILD_DOCUMENTATION)
    # ...
endif()
