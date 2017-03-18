
add_library(yaml-archive
    boost/archive/impl/yaml_grammar.cpp
    boost/archive/impl/yaml_wgrammar.cpp
    boost/archive/yaml_archive_exception.cpp
    boost/archive/yaml_iarchive.cpp
    boost/archive/yaml_oarchive.cpp
    boost/archive/yaml_wiarchive.cpp
    boost/archive/yaml_woarchive.cpp
    )

# set_target_properties(yaml-archive PROPERTIES CXX_STANDARD 11)

set(public_headers
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/iterators/yaml_escape.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/iterators/yaml_unescape_exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/yaml_iarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/basic_yaml_iarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/polymorphic_yaml_iarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/yaml_oarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/basic_yaml_oarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/polymorphic_yaml_oarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/yaml_wiarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/basic_yaml_grammar.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/polymorphic_yaml_wiarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/yaml_woarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/iterators/yaml_escape.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/polymorphic_yaml_woarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/iterators/yaml_unescape.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/yaml_archive_exception.hpp")

set(private_headers
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/basic_yaml_grammar.ipp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/basic_yaml_iarchive.ipp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/basic_yaml_oarchive.ipp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/yaml_iarchive_impl.ipp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/yaml_oarchive_impl.ipp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/yaml_wiarchive_impl.ipp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/impl/yaml_woarchive_impl.ipp")

set_target_properties(yaml-archive
    PROPERTIES
        CXX_STANDARD 
            11
        PUBLIC_HEADER
            "${public_headers}"
    )

target_include_directories(yaml-archive PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:include>
)

target_sources(yaml-archive PUBLIC "${public_headers}" "${private_headers}")

# Link with boost serialization
if (BUILD_SHARED_LIBS)
    set(Boost_USE_STATIC_LIBS OFF)
    # If we are a shared library, make sure we link with the shared library
    # variant of Boost.Serialization. Users don't have to repeat this because
    # we make this definition public.
    target_compile_definitions(yaml-archive PUBLIC BOOST_SERIALIZATION_DYN_LINK)
else()
    set(Boost_USE_STATIC_LIBS ON)
endif()
target_compile_definitions(yaml-archive PUBLIC BOOST_SERIALIZATION_NO_LIB)

# We support Boost versions >= 1.54.
find_package(Boost 1.54 COMPONENTS serialization wserialization REQUIRED)

# Link with Boost and add its include path to the search paths.
target_include_directories(yaml-archive PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(yaml-archive PUBLIC ${Boost_LIBRARIES})

if (MSVC)
    target_compile_definitions(yaml-archive PRIVATE _SCL_SECURE_NO_WARNINGS)
    target_compile_definitions(yaml-archive PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(yaml-archive PRIVATE /MP)
endif()

install(TARGETS yaml-archive
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include)
