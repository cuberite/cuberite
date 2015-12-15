cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)


add_definitions(-DTEST_GLOBALS=1)
add_library(TestFastNBT ${CMAKE_SOURCE_DIR}/src/WorldStorage/FastNBT.cpp ${CMAKE_SOURCE_DIR}/src/StringUtils.cpp)

add_executable(fastnbt-creatable-exe creatable.cpp)
target_link_libraries(fastnbt-creatable-exe TestFastNBT)
add_test(NAME fastnbt-creatable-test COMMAND fastnbt-creatable-exe)


add_executable(fastnbt-parse-exe Parse.cpp)
target_link_libraries(fastnbt-parse-exe TestFastNBT)
add_test(NAME fastnbt-parse-test COMMAND fastnbt-parse-exe)
