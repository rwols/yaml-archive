set(Boost_USE_STATIC_LIBS OFF) # Never want static unit test lib.
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

function(add_yaml_archive_test test_name)
    add_executable(${test_name} ${ARGN})
    set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 14)
    target_compile_definitions(${test_name} PRIVATE BOOST_TEST_DYN_LINK)
    if (CMAKE_CXX_COMPILER_ID STREQUAL Clang)
        target_compile_options(${test_name} PRIVATE -ftemplate-backtrace-limit=0)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL MSVC)
        target_compile_definitions(${test_name} PRIVATE BOOST_ALL_NO_LINK)
    endif()
    target_include_directories(${test_name} PRIVATE ${Boost_INCLUDE_DIR})
    target_link_libraries(${test_name} PRIVATE yaml-archive ${Boost_LIBRARIES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_yaml_archive_test(test1 test1.cpp)

add_executable(demo demo_yaml.cpp)
set_property(TARGET demo PROPERTY CXX_STANDARD 14)
target_link_libraries(demo PRIVATE yaml-archive)
