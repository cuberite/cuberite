include_directories(${PROJECT_SOURCE_DIR}/src/)

add_library(ChunkBuffer ${PROJECT_SOURCE_DIR}/src/ChunkData.cpp ${PROJECT_SOURCE_DIR}/src/StringUtils.cpp)

target_link_libraries(ChunkBuffer PUBLIC fmt::fmt)

add_executable(creatable-test Creatable.cpp)
target_link_libraries(creatable-test ChunkBuffer)
add_test_executable(creatable-test)

add_executable(coordinates-test Coordinates.cpp)
target_link_libraries(coordinates-test ChunkBuffer)
add_test_executable(coordinates-test)

add_executable(copies-test Copies.cpp)
target_link_libraries(copies-test ChunkBuffer)
add_test_executable(copies-test)

add_executable(arraystocoords-test ArraytoCoord.cpp)
target_link_libraries(arraystocoords-test ChunkBuffer)
add_test_executable(arraystocoords-test)

# Put all test projects into a separate folder:
set_target_properties(
	arraystocoords-test
	coordinates-test
	copies-test
	creatable-test
	PROPERTIES FOLDER Tests/ChunkData
)
set_target_properties(
	ChunkBuffer
	PROPERTIES FOLDER Tests/Libraries
)
