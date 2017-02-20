add_library(yaml-archive-headers INTERFACE)
target_include_directories(yaml-archive-headers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(yaml-archive-headers INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(yaml-archive-headers INTERFACE yaml-cpp-headers)

# This makes the header files show up in IDEs like Visual Studio and Xcode
target_sources(yaml-archive-headers INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/detail/is_yaml_primitive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/yaml_iarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/boost/archive/yaml_oarchive.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/YAML/extra_converters.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/YAML/extra_emitter_overloads.hpp")
