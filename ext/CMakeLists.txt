# Don't want tools.
set(YAML_CPP_BUILD_TOOLS OFF CACHE INTERNAL "")

# Don't want contributions.
set(YAML_CPP_BUILD_CONTRIB OFF CACHE INTERNAL "")

add_subdirectory(yaml-cpp)
