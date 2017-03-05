set(prefix boost/archive)

add_library(yaml-archive-2
  
  ${prefix}/impl/yaml_grammar.cpp
  ${prefix}/impl/yaml_wgrammar.cpp

  ${prefix}/basic_yaml_iarchive.cpp
  ${prefix}/yaml_archive_exception.cpp
  ${prefix}/yaml_iarchive.cpp
  ${prefix}/yaml_oarchive.cpp
  ${prefix}/yaml_wiarchive.cpp
  ${prefix}/yaml_woarchive.cpp
  )

set_target_properties(yaml-archive-2
  PROPERTIES
    CXX_STANDARD 
      11
    PUBLIC_HEADERS 
      ${prefix}/impl/basic_yaml_grammar.hpp

      ${prefix}/iterators/yaml_escape.hpp
      ${prefix}/iterators/yaml_unescape.hpp
      ${prefix}/iterators/yaml_unescape_exception.hpp

      ${prefix}/basic_yaml_archive.hpp
      ${prefix}/basic_yaml_iarchive.hpp
      ${prefix}/basic_yaml_oarchive.hpp
      ${prefix}/polymorphic_yaml_iarchive.hpp
      ${prefix}/polymorphic_yaml_oarchive.hpp
      ${prefix}/polymorphic_yaml_woarchive.hpp
      ${prefix}/polymorphic_yaml_woarchive.hpp
      ${prefix}/yaml_archive_exception.hpp
      ${prefix}/yaml_iarchive.hpp
      ${prefix}/yaml_oarchive.hpp
      ${prefix}/yaml_wiarchive.hpp
      ${prefix}/yaml_woarchive.hpp
  )

target_include_directories(yaml-archive-2 PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>  
    $<INSTALL_INTERFACE:include>
)

# Link with boost serialization
if (BUILD_SHARED_LIBS)
  set(Boost_USE_STATIC_LIBS OFF)
else()
  set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost 1.54 COMPONENTS system serialization REQUIRED)
target_include_directories(yaml-archive-2 PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(yaml-archive-2 PUBLIC ${Boost_LIBRARIES})

install(TARGETS yaml-archive-2
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include)
