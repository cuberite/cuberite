include_directories(${PROJECT_SOURCE_DIR}/src/)

add_library(ChunkBuffer ${PROJECT_SOURCE_DIR}/src/ChunkData.cpp ${PROJECT_SOURCE_DIR}/src/StringUtils.cpp)

target_link_libraries(ChunkBuffer PUBLIC fmt::fmt)

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
	PROPERTIES FOLDER Tests/Libraries
)
