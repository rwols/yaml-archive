if (BUILD_SHARED_LIBS)
    set(Boost_USE_STATIC_LIBS OFF)
else()
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost COMPONENTS system serialization REQUIRED)

add_library(yaml-archive 
    # basic_text_iprimitive.cpp
    # basic_text_oprimitive.cpp
    # basic_text_wiprimitive.cpp
    # basic_text_woprimitive.cpp
    # xml_archive_exception.cpp
    # xml_grammar.cpp
    # xml_iarchive.cpp
    # xml_oarchive.cpp
    # xml_wgrammar.cpp
    # xml_wiarchive.cpp
    # xml_woarchive.cpp
    yaml_oarchive.cpp
    yaml_iarchive.cpp
    )

set_property(TARGET yaml-archive PROPERTY CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
    target_compile_options(yaml-archive PRIVATE -ftemplate-backtrace-limit=0)
endif()
target_link_libraries(yaml-archive PUBLIC yaml-archive-headers)
target_link_libraries(yaml-archive PUBLIC yaml-cpp)
target_include_directories(yaml-archive PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(yaml-archive PUBLIC ${Boost_LIBRARIES})
