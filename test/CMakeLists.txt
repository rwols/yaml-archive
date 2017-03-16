include(CMakeParseArguments)

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
  configure_file(config.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/config.hpp")

  set(exe test_${args_LIBRARY_TARGET})
  add_executable(${exe}
    
    # file that defines the entry point for the unit test framework
    main.cpp

    # auxiliary files
    A.cpp
    B.cpp
    polymorphic_base.cpp
    polymorphic_derived1.cpp
    polymorphic_derived2.cpp
    polymorphic_A.cpp
    
    # unit test fixtures
    io_fixture.cpp

    # unit tests -- basics
    empty.cpp
    char_.cpp
    signed_char.cpp
    unsigned_char.cpp
    short_.cpp
    unsigned_short.cpp
    int_.cpp
    unsigned_int.cpp
    long_.cpp
    unsigned_long.cpp
    long_long.cpp
    unsigned_long_long.cpp
    float_.cpp
    double_.cpp
    wchar_t.cpp
    string.cpp
    binary.cpp
    object_serializable.cpp
    simple_class.cpp
    contained_class.cpp
    value_semantics.cpp

    # unit tests -- more advanced stuff
    without_version_number.cpp
    with_version_number.cpp
    simple_class_ptr.cpp
    interrupts.cpp
    nvp.cpp
    null_ptr_restore.cpp
    cyclic_ptrs.cpp
    delete_pointer.cpp
    derived.cpp
    multiple_inheritance.cpp
    diamond.cpp
    diamond_complex.cpp
    exported.cpp
    no_rtti.cpp
    class_new_delete.cpp
    non_intrusive.cpp
    private_base.cpp
    recursion.cpp
    registered.cpp
    unregistered.cpp
    split.cpp
    tracking.cpp
    non_default_ctor.cpp
    non_default_ctor2.cpp

    # unit tests -- collections
    arrays.cpp
    std_deque.cpp
    std_forward_list.cpp
    list_pointers.cpp
    list.cpp
    std_map.cpp
    std_unordered_map.cpp
    boost_unordered_map.cpp
    std_set.cpp
    std_unordered_set.cpp
    boost_unordered_set.cpp
    std_vector.cpp
    std_priority_queue.cpp

    # unit tests -- misc
    helper_support.cpp
    optional.cpp
    unique_ptr.cpp
    shared_ptr.cpp
    shared_ptr_multi_base.cpp
    std_complex.cpp
    std_bitset.cpp
    variant.cpp
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
      target_compile_definitions(${exe} PRIVATE BOOST_TEST_DYN_LINK)
  endif()
  target_compile_definitions(${exe} PRIVATE BOOST_ALL_NO_LIB)
  
  set_property(TARGET ${exe} PROPERTY CXX_STANDARD 11)

  # unit tests -- basics
  add_test(NAME empty COMMAND ${exe} --run_test=empty)
  add_test(NAME char COMMAND ${exe} --run_test=char_)
  add_test(NAME signed_char COMMAND ${exe} --run_test=signed_char)
  add_test(NAME unsigned_char COMMAND ${exe} --run_test=unsigned_char)
  add_test(NAME short COMMAND ${exe} --run_test=short_)
  add_test(NAME unsigned_short COMMAND ${exe} --run_test=unsigned_short)
  add_test(NAME int COMMAND ${exe} --run_test=int_)
  add_test(NAME unsigned_int COMMAND ${exe} --run_test=unsigned_int)
  add_test(NAME long COMMAND ${exe} --run_test=long_)
  add_test(NAME unsigned_long COMMAND ${exe} --run_test=long_long)
  add_test(NAME long_long COMMAND ${exe} --run_test=long_long)
  add_test(NAME unsigned_long_long COMMAND ${exe} --run_test=unsigned_long_long)
  add_test(NAME float COMMAND ${exe} --run_test=float_)
  add_test(NAME double COMMAND ${exe} --run_test=double_)
  add_test(NAME string COMMAND ${exe} --run_test=string)
  add_test(NAME wchar_t COMMAND ${exe} --run_test=wchar_t_)
  add_test(NAME wstring COMMAND ${exe} --run_test=wstring)
  add_test(NAME binary COMMAND ${exe} --run_test=binary)
  add_test(NAME object_serializable COMMAND ${exe} --run_test=object_serializable)
  add_test(NAME simple_class COMMAND ${exe} --run_test=simple_class)
  add_test(NAME value_semantics COMMAND ${exe} --run_test=value_semantics)

  # unit tests -- more advanced stuff
  add_test(NAME contained_class COMMAND ${exe} --run_test=contained_class)
  add_test(NAME without_version_number COMMAND ${exe} --run_test=without_version_number)
  add_test(NAME with_version_number COMMAND ${exe} --run_test=with_version_number) 
  add_test(NAME simple_class_ptr COMMAND ${exe} --run_test=simple_class_ptr)
  add_test(NAME interrupts1 COMMAND ${exe} --run_test=interrupts1)
  add_test(NAME interrupts2 COMMAND ${exe} --run_test=interrupts2)
  add_test(NAME nvp COMMAND ${exe} --run_test=nvp)
  add_test(NAME nvp_pointer COMMAND ${exe} --run_test=nvp_pointer)
  add_test(NAME null_ptr_restore COMMAND ${exe} --run_test=null_ptr_restore)
  add_test(NAME cyclic_pointers_1 COMMAND ${exe} --run_test=cyclic_pointers_1)
  add_test(NAME cyclic_pointers_2 COMMAND ${exe} --run_test=cyclic_pointers_2)
  add_test(NAME cyclic_pointers_3 COMMAND ${exe} --run_test=cyclic_pointers_3)
  add_test(NAME delete_pointer COMMAND ${exe} --run_test=delete_pointer)
  add_test(NAME derived COMMAND ${exe} --run_test=derived)
  add_test(NAME derived_class COMMAND ${exe} --run_test=derived_class)
  add_test(NAME derived_class_ptr COMMAND ${exe} --run_test=derived_class_ptr)
  add_test(NAME multiple_inheritance COMMAND ${exe} --run_test=multiple_inheritance)
  add_test(NAME diamond1 COMMAND ${exe} --run_test=diamond1)
  add_test(NAME diamond2 COMMAND ${exe} --run_test=diamond2)
  add_test(NAME diamond_complex1 COMMAND ${exe} --run_test=diamond_complex1)
  add_test(NAME diamond_complex2 COMMAND ${exe} --run_test=diamond_complex2)
  add_test(NAME diamond_complex3 COMMAND ${exe} --run_test=diamond_complex3)
  add_test(NAME diamond_complex4 COMMAND ${exe} --run_test=diamond_complex4)
  add_test(NAME exported COMMAND ${exe} --run_test=exported)
  add_test(NAME no_rtti COMMAND ${exe} --run_test=no_rtti)
  add_test(NAME class_new_delete_0 COMMAND ${exe} --run_test=class_new_delete_0)
  add_test(NAME class_new_delete_1 COMMAND ${exe} --run_test=class_new_delete_1)
  add_test(NAME private_base1 COMMAND ${exe} --run_test=private_base1)
  add_test(NAME private_base2 COMMAND ${exe} --run_test=private_base2)
  add_test(NAME recursion COMMAND ${exe} --run_test=recursion)
  add_test(NAME registered1 COMMAND ${exe} --run_test=registered1)
  add_test(NAME registered2 COMMAND ${exe} --run_test=registered2)
  add_test(NAME unregistered1 COMMAND ${exe} --run_test=unregistered1)
  add_test(NAME unregistered2 COMMAND ${exe} --run_test=unregistered2)
  add_test(NAME split COMMAND ${exe} --run_test=split)
  add_test(NAME tracking1 COMMAND ${exe} --run_test=tracking1)
  add_test(NAME tracking2 COMMAND ${exe} --run_test=tracking2)
  add_test(NAME non_default_ctor COMMAND ${exe} --run_test=non_default_ctor)
  add_test(NAME non_default_ctor2 COMMAND ${exe} --run_test=non_default_ctor2)

  # unit tests -- collections
  add_test(NAME native_array_1d_A COMMAND ${exe} --run_test=native_array_1d_A)
  add_test(NAME native_array_1d_A_wrong_size COMMAND ${exe} --run_test=native_array_1d_A_wrong_size)
  add_test(NAME native_array_2d_A COMMAND ${exe} --run_test=native_array_2d_A)
  add_test(NAME native_array_2d_A_wrong_size COMMAND ${exe} --run_test=native_array_2d_A_wrong_size)
  add_test(NAME native_array_1d_int COMMAND ${exe} --run_test=native_array_1d_int)
  add_test(NAME native_array_2d_int COMMAND ${exe} --run_test=native_array_2d_int)
  add_test(NAME boost_array_A COMMAND ${exe} --run_test=boost_array_A)
  add_test(NAME boost_array_A_wrong_size COMMAND ${exe} --run_test=boost_array_A_wrong_size)
  add_test(NAME std_array_A COMMAND ${exe} --run_test=std_array_A)
  add_test(NAME std_array_A_wrong_size COMMAND ${exe} --run_test=std_array_A_wrong_size)
  add_test(NAME std_deque COMMAND ${exe} --run_test=std_deque)
  add_test(NAME std_forward_list COMMAND ${exe} --run_test=std_forward_list)
  add_test(NAME std_list_pointers COMMAND ${exe} --run_test=std_list_pointers)
  add_test(NAME std_forward_list_ptrs COMMAND ${exe} --run_test=std_forward_list_ptrs)
  add_test(NAME std_list COMMAND ${exe} --run_test=std_list)
  add_test(NAME std_forward_list2 COMMAND ${exe} --run_test=std_forward_list2)
  add_test(NAME std_map COMMAND ${exe} --run_test=std_map)
  add_test(NAME std_map_int_ptr COMMAND ${exe} --run_test=std_map_int_ptr)
  add_test(NAME std_multimap COMMAND ${exe} --run_test=std_multimap)
  add_test(NAME std_unordered_map COMMAND ${exe} --run_test=std_unordered_map)
  add_test(NAME std_unordered_multimap COMMAND ${exe} --run_test=std_unordered_multimap)
  add_test(NAME boost_unordered_map COMMAND ${exe} --run_test=boost_unordered_map)
  add_test(NAME boost_unordered_multimap COMMAND ${exe} --run_test=boost_unordered_multimap)
  add_test(NAME std_set COMMAND ${exe} --run_test=std_set)
  add_test(NAME std_multiset COMMAND ${exe} --run_test=std_multiset)
  add_test(NAME std_unordered_set COMMAND ${exe} --run_test=std_unordered_set)
  add_test(NAME std_unordered_multiset COMMAND ${exe} --run_test=std_unordered_multiset)
  add_test(NAME boost_unordered_set COMMAND ${exe} --run_test=boost_unordered_set)
  add_test(NAME boost_unordered_multiset COMMAND ${exe} --run_test=boost_unordered_multiset)
  add_test(NAME std_vector_int COMMAND ${exe} --run_test=std_vector_int)
  add_test(NAME std_vector_bool COMMAND ${exe} --run_test=std_vector_bool)
  add_test(NAME std_vector_A COMMAND ${exe} --run_test=std_vector_A)
  add_test(NAME std_vector_non_default_ctor COMMAND ${exe} --run_test=std_vector_non_default_ctor)
  # FIXME!!!
  # add_test(NAME std_priority_queue COMMAND ${exe} --run_test=std_priority_queue)
  # add_test(NAME std_queue COMMAND ${exe} --run_test=std_queue)
  # add_test(NAME std_stack COMMAND ${exe} --run_test=std_stack)
  # add_test(NAME std_valarray COMMAND ${exe} --run_test=std_valarrary)

  # unit tests -- misc
  add_test(NAME helper_support COMMAND ${exe} --run_test=helper_support)
  add_test(NAME boost_optional COMMAND ${exe} --run_test=boost_optional)
  add_test(NAME std_unique_ptr_1 COMMAND ${exe} --run_test=std_unique_ptr_1)
  add_test(NAME std_unique_ptr_2 COMMAND ${exe} --run_test=std_unique_ptr_2)
  add_test(NAME boost_shared_ptr_1 COMMAND ${exe} --run_test=boost_shared_ptr_1)
  add_test(NAME boost_shared_ptr_2 COMMAND ${exe} --run_test=boost_shared_ptr_2)
  add_test(NAME boost_shared_ptr_3 COMMAND ${exe} --run_test=boost_shared_ptr_3)
  add_test(NAME boost_shared_ptr_4 COMMAND ${exe} --run_test=boost_shared_ptr_4)
  add_test(NAME boost_shared_ptr_5 COMMAND ${exe} --run_test=boost_shared_ptr_5)
  add_test(NAME boost_shared_ptr_6 COMMAND ${exe} --run_test=boost_shared_ptr_6)
  add_test(NAME std_shared_ptr_1 COMMAND ${exe} --run_test=std_shared_ptr_1)
  add_test(NAME std_shared_ptr_2 COMMAND ${exe} --run_test=std_shared_ptr_2)
  add_test(NAME std_shared_ptr_3 COMMAND ${exe} --run_test=std_shared_ptr_3)
  add_test(NAME std_shared_ptr_4 COMMAND ${exe} --run_test=std_shared_ptr_4)
  add_test(NAME std_shared_ptr_5 COMMAND ${exe} --run_test=std_shared_ptr_5)
  add_test(NAME std_shared_ptr_6 COMMAND ${exe} --run_test=std_shared_ptr_6)
  add_test(NAME boost_shared_ptr_multi_base_1 COMMAND ${exe} --run_test=boost_shared_ptr_multi_base_1)
  add_test(NAME boost_shared_ptr_multi_base_2 COMMAND ${exe} --run_test=boost_shared_ptr_multi_base_2)
  add_test(NAME boost_shared_ptr_multi_base_3 COMMAND ${exe} --run_test=boost_shared_ptr_multi_base_3)
  add_test(NAME boost_shared_ptr_multi_base_4 COMMAND ${exe} --run_test=boost_shared_ptr_multi_base_4)
  add_test(NAME boost_shared_ptr_multi_base_5 COMMAND ${exe} --run_test=boost_shared_ptr_multi_base_5)
  add_test(NAME boost_shared_ptr_multi_base_6 COMMAND ${exe} --run_test=boost_shared_ptr_multi_base_6)
  add_test(NAME boost_shared_ptr_multi_base_7 COMMAND ${exe} --run_test=boost_shared_ptr_multi_base_7)
  add_test(NAME std_shared_ptr_multi_base_1 COMMAND ${exe} --run_test=std_shared_ptr_multi_base_1)
  add_test(NAME std_shared_ptr_multi_base_2 COMMAND ${exe} --run_test=std_shared_ptr_multi_base_2)
  add_test(NAME std_shared_ptr_multi_base_3 COMMAND ${exe} --run_test=std_shared_ptr_multi_base_3)
  add_test(NAME std_shared_ptr_multi_base_4 COMMAND ${exe} --run_test=std_shared_ptr_multi_base_4)
  add_test(NAME std_shared_ptr_multi_base_5 COMMAND ${exe} --run_test=std_shared_ptr_multi_base_5)
  add_test(NAME std_shared_ptr_multi_base_6 COMMAND ${exe} --run_test=std_shared_ptr_multi_base_6)
  add_test(NAME std_shared_ptr_multi_base_7 COMMAND ${exe} --run_test=std_shared_ptr_multi_base_7)
  add_test(NAME std_complex COMMAND ${exe} --run_test=std_complex)
  add_test(NAME std_bitset COMMAND ${exe} --run_test=std_bitset)
  add_test(NAME boost_variant_1 COMMAND ${exe} --run_test=boost_variant_1)
  add_test(NAME boost_variant_2 COMMAND ${exe} --run_test=boost_variant_2)
  add_test(NAME boost_variant_3 COMMAND ${exe} --run_test=boost_variant_3)
  add_test(NAME boost_variant_4 COMMAND ${exe} --run_test=boost_variant_4)
  add_test(NAME boost_variant_5 COMMAND ${exe} --run_test=boost_variant_5)
  add_test(NAME boost_variant_6 COMMAND ${exe} --run_test=boost_variant_6)
  add_test(NAME boost_variant_7 COMMAND ${exe} --run_test=boost_variant_7)

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
      target_compile_definitions(${exe} PRIVATE BOOST_TEST_DYN_LINK)
  endif()
  target_compile_definitions(${exe} PRIVATE BOOST_ALL_NO_LIB)
  
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
target_link_libraries(demo_yaml PRIVATE yaml-archive-better)
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

target_create_boost_archive_test_suite(
  LIBRARY_TARGET yaml-archive-better
  INPUT_ARCHIVE_HEADER boost/archive/yaml_iarchive.hpp 
  OUTPUT_ARCHIVE_HEADER boost/archive/yaml_oarchive.hpp
  INPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_iarchive 
  OUTPUT_ARCHIVE_FULLY_QUALIFIED_NAME boost::archive::yaml_oarchive 
  TEST_ISTREAM std::ifstream 
  TEST_OSTREAM std::ofstream
  ARCHIVE_FILENAME_EXTENSION yml
  ARCHIVE_FLAGS "0"
  STREAM_FLAGS "(std::ios_base::openmode)0"
  )

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


