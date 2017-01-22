
find_package(Boost COMPONENTS system serialization REQUIRED)
add_library(yaml-archive main.cpp)
target_include_directories(yaml-archive PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(yaml-archive PUBLIC yaml-archive-headers)
target_link_libraries(yaml-archive PUBLIC ${Boost_LIBRARIES})
target_link_libraries(yaml-archive PRIVATE yaml-cpp)
