enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_flags_cxx("-Wno-error=old-style-cast")
	set_source_files_properties(${CMAKE_SOURCE_DIR}/src/ChunkData.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion")
	set_source_files_properties(${CMAKE_SOURCE_DIR}/src/StringUtils.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=conversion")
endif()


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




# Put all test projects into a separate folder:
set_target_properties(
	arraystocoords-exe
	coordinates-exe
	copies-exe
	copyblocks-exe
	creatable-exe
	PROPERTIES FOLDER Tests/ChunkData
)
set_target_properties(
	ChunkBuffer
	PROPERTIES FOLDER Lib
)
