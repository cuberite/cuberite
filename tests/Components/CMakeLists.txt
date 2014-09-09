cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)

add_executable(BurningComponent-creatable-exe creatable.cpp)
add_test(NAME creatable-test COMMAND BurningComponent-creatable-exe)

add_executable(BurningComponent-tick-exe Tick.cpp)
add_test(NAME tick-test COMMAND BurningComponent-tick-exe)
