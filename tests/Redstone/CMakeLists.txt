cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)

add_executable(Redstone-creatable-exe creatable.cpp ../../src/BoundingBox.cpp)
add_test(NAME creatable-test COMMAND Redstone-creatable-exe)
