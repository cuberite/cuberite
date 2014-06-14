cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)
add_library(ChunkBuffer ${CMAKE_SOURCE_DIR}/src/ChunkData.cpp ${CMAKE_SOURCE_DIR}/src/StringUtils.cpp)


add_executable(creatable-exe creatable.cpp)
target_link_libraries(creatable-exe ChunkBuffer)
add_test(NAME creatable-test COMMAND creatable-exe)

add_executable(coordinates-exe Coordinates.cpp)
target_link_libraries(coordinates-exe ChunkBuffer)
add_test(NAME coordinates-test COMMAND coordinates-exe)

add_executable(copies-exe Copies.cpp)
target_link_libraries(copies-exe ChunkBuffer)
add_test(NAME copies-test COMMAND copies-exe)

add_executable(arraystocoords-exe ArraytoCoord.cpp)
target_link_libraries(arraystocoords-exe ChunkBuffer)
add_test(NAME arraystocoords-test COMMAND arraystocoords-exe)

add_executable(copyblocks-exe CopyBlocks.cpp)
target_link_libraries(copyblocks-exe ChunkBuffer)
add_test(NAME copyblocks-test COMMAND copyblocks-exe)
