cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_library(ChunkBuffer ${CMAKE_SOURCE_DIR}/src/ChunkBuffer.cpp)


add_executable(creatable-exe creatable.cpp)
target_link_libraries(creatable-exe ChunkBuffer)
add_test(NAME creatable-test COMMAND creatable-exe)

add_executable(coordinates-exe Coordinates.cpp)
target_link_libraries(coordinates-exe ChunkBuffer)
add_test(NAME coordinates-test COMMAND coordinates-exe)
