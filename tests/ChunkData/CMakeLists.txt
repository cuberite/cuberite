include_directories(${PROJECT_SOURCE_DIR}/src/)

add_library(ChunkBuffer ${PROJECT_SOURCE_DIR}/src/ChunkData.cpp ${PROJECT_SOURCE_DIR}/src/StringUtils.cpp)

target_link_libraries(ChunkBuffer PUBLIC fmt::fmt gtest gmock gtest_main)

add_executable(TestCreatable Creatable.cpp)
target_link_libraries(TestCreatable ChunkBuffer)
add_test_executable(TestCreatable)

add_executable(TestCoordinates Coordinates.cpp)
target_link_libraries(TestCoordinates ChunkBuffer)
add_test_executable(TestCoordinates)

add_executable(TestCopies Copies.cpp)
target_link_libraries(TestCopies ChunkBuffer)
add_test_executable(TestCopies)

add_executable(TestArrayToCoord ArraytoCoord.cpp)
target_link_libraries(TestArrayToCoord ChunkBuffer)
add_test_executable(TestArrayToCoord)

# Put all test projects into a separate folder:
set_target_properties(
	TestArrayToCoord
	TestCoordinates
	TestCopies
	TestCreatable
	PROPERTIES FOLDER Tests/ChunkData
)
set_target_properties(
	ChunkBuffer
	PROPERTIES FOLDER Tests/Libraries
)
