cmake_minimum_required (VERSION 2.6) 

enable_testing()

add_library(ChunkBuffer ${CMAKE_SOURCE_DIR}/src/ChunkBuffer.cpp)


add_executable(creatable-exe creatable.cpp)
target_link_libraries(creatable-exe ChunkBuffer)
add_test(NAME creatable-test COMMAND creatable-exe)
