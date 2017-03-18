include(CMakeParseArguments)

# Find Boost.Test.
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.54 REQUIRED COMPONENTS unit_test_framework)

function(add_archive_test name exe ext)
  add_test(NAME ${name}_${ext} COMMAND ${exe} --run_test=${name})
endfunction(add_archive_test)

function(target_create_boost_archive_test_suite)
  set(functionArguments 
    LIBRARY_TARGET
    INPUT_ARCHIVE_HEADER 
    INPUT_ARCHIVE_FULLY_QUALIFIED_NAME 
    OUTPUT_ARCHIVE_HEADER 
    OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME
    TEST_ISTREAM
    TEST_OSTREAM
    ARCHIVE_FILENAME_EXTENSION
    ARCHIVE_FLAGS
    STREAM_FLAGS)
  cmake_parse_arguments(args "" "${functionArguments}" "" ${ARGN})
  set(ext ${args_ARCHIVE_FILENAME_EXTENSION})
  configure_file(../config.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/config.hpp")
  set(exe test_${args_LIBRARY_TARGET}_${ext})
  add_executable(${exe}
    
    # file that defines the entry point for the unit test framework
    ../main.cpp

    # auxiliary files
    ../A.cpp
    ../B.cpp
    ../polymorphic_base.cpp
    ../polymorphic_derived1.cpp
    ../polymorphic_derived2.cpp
    ../polymorphic_A.cpp
    
    # unit test fixtures
    ../io_fixture.cpp

    # unit tests -- basics
    ../empty.cpp
    ../char_.cpp
    ../signed_char.cpp
    ../unsigned_char.cpp
    ../short_.cpp
    ../unsigned_short.cpp
    ../int_.cpp
    ../unsigned_int.cpp
    ../long_.cpp
    ../unsigned_long.cpp
    ../long_long.cpp
    ../unsigned_long_long.cpp
    ../float_.cpp
    ../double_.cpp
    ../wchar_t.cpp
    ../string.cpp
    ../binary.cpp
    ../object_serializable.cpp
    ../simple_class.cpp
    ../contained_class.cpp
    ../value_semantics.cpp

    # unit tests -- more advanced stuff
    ../without_version_number.cpp
    ../with_version_number.cpp
    ../simple_class_ptr.cpp
    ../interrupts.cpp
    ../nvp.cpp
    ../null_ptr_restore.cpp
    ../cyclic_ptrs.cpp
    ../delete_pointer.cpp
    ../derived.cpp
    ../multiple_inheritance.cpp
    ../diamond.cpp
    ../diamond_complex.cpp
    ../exported.cpp
    ../no_rtti.cpp
    ../class_new_delete.cpp
    ../non_intrusive.cpp
    ../private_base.cpp
    ../recursion.cpp
    ../registered.cpp
    ../unregistered.cpp
    ../split.cpp
    ../tracking.cpp
    ../non_default_ctor.cpp
    ../non_default_ctor2.cpp

    # unit tests -- collections
    ../arrays.cpp
    ../std_deque.cpp
    ../std_forward_list.cpp
    ../list_pointers.cpp
    ../list.cpp
    ../std_map.cpp
    ../std_unordered_map.cpp
    ../boost_unordered_map.cpp
    ../std_set.cpp
    ../std_unordered_set.cpp
    ../boost_unordered_set.cpp
    ../std_vector.cpp
    ../std_priority_queue.cpp

    # unit tests -- misc
    ../helper_support.cpp
    ../optional.cpp
    ../unique_ptr.cpp
    ../shared_ptr.cpp
    ../shared_ptr_multi_base.cpp
    ../std_complex.cpp
    ../std_bitset.cpp
    ../variant.cpp
    )

  # ALWAYS dynamically link with Boost.Test.
  target_compile_definitions(${exe} PRIVATE BOOST_TEST_DYN_LINK)
  target_compile_definitions(${exe} PRIVATE BOOST_TEST_NO_LIB)

  # For the config.hpp file.
  target_include_directories(${exe} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

  # Link with the archive that we're going to run unit tests on.
  # This should bring in Boost.Serialization too.
  target_link_libraries(${exe} PRIVATE ${args_LIBRARY_TARGET})

  # Link with Boost.Test
  target_link_libraries(${exe} PRIVATE ${Boost_LIBRARIES})
  
  # We want C++11.
  set_property(TARGET ${exe} PROPERTY CXX_STANDARD 11)

  if (MSVC)
    # Enable multithreaded compilation. This speeds things up.
    target_compile_definitions(${exe} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(${exe} PRIVATE /MP)
  endif()

  # unit tests -- basics
  add_archive_test(empty ${exe} ${ext})
  add_archive_test(char_ ${exe} ${ext})
  add_archive_test(signed_char ${exe} ${ext})
  add_archive_test(unsigned_char ${exe} ${ext})
  add_archive_test(short_ ${exe} ${ext})
  add_archive_test(unsigned_short ${exe} ${ext})
  add_archive_test(int_ ${exe} ${ext})
  add_archive_test(unsigned_int ${exe} ${ext})
  add_archive_test(long_ ${exe} ${ext})
  add_archive_test(unsigned_long ${exe} ${ext})
  add_archive_test(long_long ${exe} ${ext})
  add_archive_test(unsigned_long_long ${exe} ${ext})
  add_archive_test(float_ ${exe} ${ext})
  add_archive_test(double_ ${exe} ${ext})
  add_archive_test(string ${exe} ${ext})
  add_archive_test(wchar_t_ ${exe} ${ext})
  add_archive_test(wstring ${exe} ${ext})
  add_archive_test(binary ${exe} ${ext})
  add_archive_test(object_serializable ${exe} ${ext})
  add_archive_test(simple_class ${exe} ${ext})
  add_archive_test(value_semantics ${exe} ${ext})

  # unit tests -- more advanced stuff
  add_archive_test(contained_class ${exe} ${ext})
  add_archive_test(without_version_number ${exe} ${ext})
  add_archive_test(with_version_number ${exe} ${ext}) 
  add_archive_test(simple_class_ptr ${exe} ${ext})
  add_archive_test(interrupts1 ${exe} ${ext})
  add_archive_test(interrupts2 ${exe} ${ext})
  add_archive_test(nvp ${exe} ${ext})
  add_archive_test(nvp_pointer ${exe} ${ext})
  add_archive_test(null_ptr_restore ${exe} ${ext})
  add_archive_test(cyclic_pointers_1 ${exe} ${ext})
  add_archive_test(cyclic_pointers_2 ${exe} ${ext})
  add_archive_test(cyclic_pointers_3 ${exe} ${ext})
  add_archive_test(delete_pointer ${exe} ${ext})
  add_archive_test(derived ${exe} ${ext})
  add_archive_test(derived_class ${exe} ${ext})
  add_archive_test(derived_class_ptr ${exe} ${ext})
  add_archive_test(multiple_inheritance ${exe} ${ext})
  add_archive_test(diamond1 ${exe} ${ext})
  add_archive_test(diamond2 ${exe} ${ext})
  add_archive_test(diamond_complex1 ${exe} ${ext})
  add_archive_test(diamond_complex2 ${exe} ${ext})
  add_archive_test(diamond_complex3 ${exe} ${ext})
  add_archive_test(diamond_complex4 ${exe} ${ext})
  add_archive_test(exported ${exe} ${ext})
  add_archive_test(no_rtti ${exe} ${ext})
  add_archive_test(class_new_delete_0 ${exe} ${ext})
  add_archive_test(class_new_delete_1 ${exe} ${ext})
  add_archive_test(private_base1 ${exe} ${ext})
  add_archive_test(private_base2 ${exe} ${ext})
  add_archive_test(recursion ${exe} ${ext})
  add_archive_test(registered1 ${exe} ${ext})
  add_archive_test(registered2 ${exe} ${ext})
  add_archive_test(unregistered1 ${exe} ${ext})
  add_archive_test(unregistered2 ${exe} ${ext})
  add_archive_test(split ${exe} ${ext})
  add_archive_test(tracking1 ${exe} ${ext})
  add_archive_test(tracking2 ${exe} ${ext})
  add_archive_test(non_default_ctor ${exe} ${ext})
  add_archive_test(non_default_ctor2 ${exe} ${ext})

  # unit tests -- collections
  add_archive_test(native_array_1d_A ${exe} ${ext})
  add_archive_test(native_array_1d_A_wrong_size ${exe} ${ext})
  add_archive_test(native_array_2d_A ${exe} ${ext})
  add_archive_test(native_array_2d_A_wrong_size ${exe} ${ext})
  add_archive_test(native_array_1d_int ${exe} ${ext})
  add_archive_test(native_array_2d_int ${exe} ${ext})
  add_archive_test(boost_array_A ${exe} ${ext})
  add_archive_test(boost_array_A_wrong_size ${exe} ${ext})
  add_archive_test(std_array_A ${exe} ${ext})
  add_archive_test(std_array_A_wrong_size ${exe} ${ext})
  add_archive_test(std_deque ${exe} ${ext})
  add_archive_test(std_forward_list ${exe} ${ext})
  add_archive_test(std_list_pointers ${exe} ${ext})
  add_archive_test(std_forward_list_ptrs ${exe} ${ext})
  add_archive_test(std_list ${exe} ${ext})
  add_archive_test(std_forward_list2 ${exe} ${ext})
  add_archive_test(std_map ${exe} ${ext})
  add_archive_test(std_map_int_ptr ${exe} ${ext})
  add_archive_test(std_multimap ${exe} ${ext})
  add_archive_test(std_unordered_map ${exe} ${ext})
  add_archive_test(std_unordered_multimap ${exe} ${ext})
  add_archive_test(boost_unordered_map ${exe} ${ext})
  add_archive_test(boost_unordered_multimap ${exe} ${ext})
  add_archive_test(std_set ${exe} ${ext})
  add_archive_test(std_multiset ${exe} ${ext})
  add_archive_test(std_unordered_set ${exe} ${ext})
  add_archive_test(std_unordered_multiset ${exe} ${ext})
  add_archive_test(boost_unordered_set ${exe} ${ext})
  add_archive_test(boost_unordered_multiset ${exe} ${ext})
  add_archive_test(std_vector_int ${exe} ${ext})
  add_archive_test(std_vector_bool ${exe} ${ext})
  add_archive_test(std_vector_A ${exe} ${ext})
  add_archive_test(std_vector_non_default_ctor ${exe} ${ext})
  # FIXME!!!
  # add_archive_test(std_priority_queue ${exe} ${ext})
  # add_archive_test(std_queue ${exe} ${ext})
  # add_archive_test(std_stack ${exe} ${ext})
  # add_archive_test(std_valarray ${exe} ${ext})

  # unit tests -- misc
  add_archive_test(helper_support ${exe} ${ext})
  add_archive_test(boost_optional ${exe} ${ext})
  add_archive_test(std_unique_ptr_1 ${exe} ${ext})
  add_archive_test(std_unique_ptr_2 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_1 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_2 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_3 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_4 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_5 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_6 ${exe} ${ext})
  add_archive_test(std_shared_ptr_1 ${exe} ${ext})
  add_archive_test(std_shared_ptr_2 ${exe} ${ext})
  add_archive_test(std_shared_ptr_3 ${exe} ${ext})
  add_archive_test(std_shared_ptr_4 ${exe} ${ext})
  add_archive_test(std_shared_ptr_5 ${exe} ${ext})
  add_archive_test(std_shared_ptr_6 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_multi_base_1 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_multi_base_2 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_multi_base_3 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_multi_base_4 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_multi_base_5 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_multi_base_6 ${exe} ${ext})
  add_archive_test(boost_shared_ptr_multi_base_7 ${exe} ${ext})
  add_archive_test(std_shared_ptr_multi_base_1 ${exe} ${ext})
  add_archive_test(std_shared_ptr_multi_base_2 ${exe} ${ext})
  add_archive_test(std_shared_ptr_multi_base_3 ${exe} ${ext})
  add_archive_test(std_shared_ptr_multi_base_4 ${exe} ${ext})
  add_archive_test(std_shared_ptr_multi_base_5 ${exe} ${ext})
  add_archive_test(std_shared_ptr_multi_base_6 ${exe} ${ext})
  add_archive_test(std_shared_ptr_multi_base_7 ${exe} ${ext})
  add_archive_test(std_complex ${exe} ${ext})
  add_archive_test(std_bitset ${exe} ${ext})
  add_archive_test(boost_variant_1 ${exe} ${ext})
  add_archive_test(boost_variant_2 ${exe} ${ext})
  add_archive_test(boost_variant_3 ${exe} ${ext})
  add_archive_test(boost_variant_4 ${exe} ${ext})
  add_archive_test(boost_variant_5 ${exe} ${ext})
  add_archive_test(boost_variant_6 ${exe} ${ext})
  add_archive_test(boost_variant_7 ${exe} ${ext})

endfunction(target_create_boost_archive_test_suite)

function(target_create_boost_polymorphic_archive_test_suite)
  set(functionArguments 
    LIBRARY_TARGET
    INPUT_ARCHIVE_HEADER 
    INPUT_ARCHIVE_FULLY_QUALIFIED_NAME 
    OUTPUT_ARCHIVE_HEADER 
    OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME
    TEST_ISTREAM
    TEST_OSTREAM
    ARCHIVE_FILENAME_EXTENSION
    ARCHIVE_FLAGS
    STREAM_FLAGS)
  cmake_parse_arguments(args "" "${functionArguments}" "" ${ARGN})
  configure_file(config.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/polymorphic_config.hpp")

  set(exe test_polymorphic_${args_LIBRARY_TARGET})
  add_executable(${exe}
    polymorphic_main.cpp

    # auxiliary files
    A.cpp
    polymorphic_A.cpp
    polymorphic_io_fixture.cpp

    # test files
    polymorphic.cpp
    )

  if (BUILD_SHARED_LIBS)
      set(Boost_USE_STATIC_LIBS OFF)
  else()
      set(Boost_USE_STATIC_LIBS ON)
  endif()
  find_package(Boost COMPONENTS unit_test_framework REQUIRED)

  target_include_directories(${exe} PRIVATE ${Boost_INCLUDE_DIRS})
  target_include_directories(${exe} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

  target_link_libraries(${exe} PRIVATE ${args_LIBRARY_TARGET})
  target_link_libraries(${exe} PRIVATE ${Boost_LIBRARIES})
  
  if (BUILD_SHARED_LIBS)
      target_compile_definitions(${exe} PUBLIC BOOST_TEST_DYN_LINK)
  endif()
  target_compile_definitions(${exe} PUBLIC BOOST_ALL_NO_LIB)
  
  set_property(TARGET ${exe} PROPERTY CXX_STANDARD 11)

  # FIXME!!!
  # add_test(NAME polymorphic_1 COMMAND ${exe} --run_test=polymorphic_1)
  # add_test(NAME polymorphic_2 COMMAND ${exe} --run_test=polymorphic_2)
  # add_test(NAME polymorphic_3 COMMAND ${exe} --run_test=polymorphic_3)

endfunction(target_create_boost_polymorphic_archive_test_suite)

################################################################################
# FUNCTION target_add_polymorphic_archive_test_suite                           #
#                                                                              #
# PARAMETERS:                                                                  #
# - target         The cmake target that defines the archive                   #
# - config         The header that makes typedefs for the particular archive   #
################################################################################
# function(target_add_polymorphic_archive_test_suite target config)
#   add_archive_test(NAME test_polymorphic        ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS test_polymorphic_A A)
#   add_archive_test(NAME test_polymorphic2       ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS test_polymorphic2imp)
#   add_archive_test(NAME test_polymorphic_helper ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
# endfunction(target_add_polymorphic_archive_test_suite)

# add_executable(test1 test1.cpp)
# set_property(TARGET test1 PROPERTY CXX_STANDARD 11)
# target_link_libraries(test1 PRIVATE serialization-test-interface)
# target_link_libraries(test1 PRIVATE yaml-archive)
# target_compile_options(test1 PRIVATE -ftemplate-backtrace-limit=0)

add_executable(demo_yaml demo_yaml.cpp)
set_property(TARGET demo_yaml PROPERTY CXX_STANDARD 11)
target_link_libraries(demo_yaml PRIVATE yaml-archive)
target_compile_options(demo_yaml PRIVATE -ftemplate-backtrace-limit=0)

# target_create_boost_archive_test_suite(
#   LIBRARY_TARGET yaml-archive
#   INPUT_ARCHIVE_HEADER boost/archive/yaml_iarchive.hpp 
#   OUTPUT_ARCHIVE_HEADER boost/archive/yaml_oarchive.hpp
#   INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_iarchive 
#   OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_oarchive 
#   TEST_ISTREAM std::ifstream 
#   TEST_OSTREAM std::ofstream
#   ARCHIVE_FILENAME_EXTENSION yml
#   ARCHIVE_FLAGS "0"
#   STREAM_FLAGS "(std::ios_base::openmode)0"
#   )

# target_create_boost_polymorphic_archive_test_suite(
#   LIBRARY_TARGET yaml-archive
#   INPUT_ARCHIVE_HEADER boost/archive/polymorphic_yaml_iarchive.hpp 
#   OUTPUT_ARCHIVE_HEADER boost/archive/polymorphic_yaml_oarchive.hpp
#   INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::polymorphic_yaml_iarchive 
#   OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::polymorphic_yaml_oarchive 
#   TEST_ISTREAM std::ifstream 
#   TEST_OSTREAM std::ofstream
#   ARCHIVE_FILENAME_EXTENSION yml
#   ARCHIVE_FLAGS "0"
#   STREAM_FLAGS "(std::ios_base::openmode)0"
#   )

# target_create_boost_archive_test_suite(
#   LIBRARY_TARGET yaml-archive
#   INPUT_ARCHIVE_HEADER boost/archive/yaml_iarchive.hpp 
#   OUTPUT_ARCHIVE_HEADER boost/archive/yaml_oarchive.hpp
#   INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_iarchive 
#   OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_oarchive 
#   TEST_ISTREAM std::ifstream 
#   TEST_OSTREAM std::ofstream
#   ARCHIVE_FILENAME_EXTENSION yml
#   ARCHIVE_FLAGS "0"
#   STREAM_FLAGS "(std::ios_base::openmode)0"
#   )

# target_create_boost_archive_test_suite(
#   LIBRARY_TARGET yaml-archive
#   INPUT_ARCHIVE_HEADER boost/archive/yaml_wiarchive.hpp 
#   OUTPUT_ARCHIVE_HEADER boost/archive/yaml_woarchive.hpp
#   INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_wiarchive 
#   OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_woarchive 
#   TEST_ISTREAM std::wifstream 
#   TEST_OSTREAM std::wofstream
#   ARCHIVE_FILENAME_EXTENSION wide.yml
#   ARCHIVE_FLAGS "0"
#   STREAM_FLAGS "(std::ios_base::openmode)0"
#   )

# target_create_boost_polymorphic_archive_test_suite(
#   LIBRARY_TARGET yaml-archive
#   INPUT_ARCHIVE_HEADER boost/archive/polymorphic_yaml_iarchive.hpp 
#   OUTPUT_ARCHIVE_HEADER boost/archive/polymorphic_yaml_oarchive.hpp
#   INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::polymorphic_yaml_iarchive 
#   OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::polymorphic_yaml_oarchive 
#   TEST_ISTREAM std::ifstream 
#   TEST_OSTREAM std::ofstream
#   ARCHIVE_FILENAME_EXTENSION yml
#   ARCHIVE_FLAGS "0"
#   STREAM_FLAGS "(std::ios_base::openmode)0"
#   )

add_subdirectory(narrow)

if (NOT WIN32 AND Boost_VERSION VERSION_LESS 105600)
  message(WARNING "Boost version is too old. Skipping wide char unit tests.")
else()
  add_subdirectory(wide)
endif()

