# Don't want tools.
set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")

# Don't want contributions.
set(YAML_CPP_BUILD_CONTRIB OFF CACHE INTERNAL "")

add_subdirectory(yaml-cpp)

# Sigh...
add_library(yaml-cpp-headers INTERFACE)
target_include_directories(yaml-cpp-headers INTERFACE yaml-cpp/include)
