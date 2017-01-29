# CMake build control file for Serialization Library tests

# cmake_minimum_required(VERSION 3.3)

include(CMakeParseArguments)

find_package(Boost 
  COMPONENTS 
    system 
    filesystem 
    serialization 
    wserialization 
  REQUIRED)

# Creates a common "abstract" target that all tests link against.
# Saves some boilerplate code.
add_library(serialization-test-interface INTERFACE)
target_include_directories(serialization-test-interface INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(serialization-test-interface INTERFACE ${Boost_LIBRARIES})
target_compile_definitions(serialization-test-interface INTERFACE BOOST_ALL_NO_LIB)
target_compile_definitions(serialization-test-interface INTERFACE BOOST_LIB_DIAGNOSTICS)
if (BUILD_SHARED_LIBS)
  target_compile_definitions(serialization-test-interface INTERFACE BOOST_ALL_DYN_LINK)
endif()

################################################################################
# FUNCTION add_archive_test                                                    #
#                                                                              #
# NAMED PARAMETERS:                                                            #
# - NAME           The main cpp file of the test, without the .cpp extension   #
# - ARCHIVE_NAME   The cmake target that defines the archive                   #
# - ARCHIVE_CONFIG The header that makes typedefs for the particular archive   #
# - EXTRA_DEPS     Extra cpp files without the .cpp extension that should be   #
#                  compiled along with NAME.                                   #
################################################################################
function(add_archive_test)
  set(options)
  set(oneValueArgs NAME ARCHIVE_NAME ARCHIVE_CONFIG)
  set(multiValueArgs EXTRA_DEPS)
  cmake_parse_arguments(args "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  foreach(dependency ${args_EXTRA_DEPS})
    list(APPEND arglist ${dependency}.cpp)
  endforeach()
  set(amended_test_name ${args_NAME}_${args_ARCHIVE_NAME})
  add_executable(${amended_test_name} ${args_NAME}.cpp ${arglist})
  set_property(TARGET ${amended_test_name} PROPERTY CXX_STANDARD 14)
  target_compile_definitions(${amended_test_name} PRIVATE BOOST_ARCHIVE_TEST=${args_ARCHIVE_CONFIG})
  target_link_libraries(${amended_test_name} PRIVATE serialization-test-interface)
  target_link_libraries(${amended_test_name} PRIVATE ${args_ARCHIVE_NAME})
  add_test(${amended_test_name} ${amended_test_name})
endfunction(add_archive_test)

################################################################################
# FUNCTION target_add_archive_test_suite                                       #
#                                                                              #
# PARAMETERS:                                                                  #
# - target         The cmake target that defines the archive                   #
# - config         The header that makes typedefs for the particular archive   #
################################################################################
function(target_add_archive_test_suite target config)
  add_archive_test(NAME test_binary                    ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_native_array              ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_boost_array               ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  
  # This one requires C++11
  add_archive_test(NAME test_array                     ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  
  add_archive_test(NAME test_bitset                    ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_class_info_save           ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_class_info_load           ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_complex                   ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_contained_class           ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_cyclic_ptrs               ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_delete_pointer            ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_deque                     ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_derived                   ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_derived_class             ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_diamond                   ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_diamond_complex           ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_exported polymorphic_base ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_forward_list              ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_forward_list_ptrs         ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_helper_support            ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_interrupts                ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_list                      ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_list_ptrs                 ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_map                       ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_map_boost_unordered       ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_mi                        ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_multiple_ptrs             ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_multiple_inheritance      ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_no_rtti polymorphic_base  ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS polymorphic_derived1)
  add_archive_test(NAME test_new_operator              ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_non_intrusive             ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_non_default_ctor          ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_non_default_ctor2         ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_null_ptr                  ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_nvp                       ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_object                    ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_optional                  ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_primitive                 ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_priority_queue            ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_private_base              ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_private_base2             ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_queue                     ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_recursion                 ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_registered                ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_shared_ptr                ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_shared_ptr_multi_base     ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_shared_ptr_132            ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_simple_class              ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_simple_class_ptr          ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_stack                     ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_split                     ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_tracking                  ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_unregistered              ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_unique_ptr                ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_valarray                  ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
  add_archive_test(NAME test_variant                   ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_vector                    ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_set                       ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  add_archive_test(NAME test_set_boost_unordered       ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
  
  # This one requires C++11
  add_archive_test(NAME test_set_unordered             ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)

  # This one requires C++11
  add_archive_test(NAME test_map_unordered             ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS A)
endfunction(target_add_archive_test_suite)

################################################################################
# FUNCTION target_add_polymorphic_archive_test_suite                           #
#                                                                              #
# PARAMETERS:                                                                  #
# - target         The cmake target that defines the archive                   #
# - config         The header that makes typedefs for the particular archive   #
################################################################################
function(target_add_polymorphic_archive_test_suite target config)
  add_archive_test(NAME test_polymorphic        ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS test_polymorphic_A A)
  add_archive_test(NAME test_polymorphic2       ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config} EXTRA_DEPS test_polymorphic2imp)
  add_archive_test(NAME test_polymorphic_helper ARCHIVE_NAME ${target} ARCHIVE_CONFIG ${config})
endfunction(target_add_polymorphic_archive_test_suite)

target_add_archive_test_suite(yaml-archive Config.hpp)

# TODO!!
# target_add_polymorphic_archive_test_suite(yaml-archive ConfigPoly.hpp)

