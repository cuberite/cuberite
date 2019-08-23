cmake_minimum_required(VERSION 3.0.2)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)





# Define individual test executables:

# BlockStateTest: Verify that the BlockState class works as intended:
add_executable(BlockStateTest
	BlockStateTest.cpp
	../TestHelpers.h
	${CMAKE_SOURCE_DIR}/src/BlockState.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
)
target_link_libraries(BlockStateTest fmt::fmt)

# BlockTypeRegistryTest: Verify that the BlockTypeRegistry class works as intended:
add_executable(BlockTypeRegistryTest
	BlockTypeRegistryTest.cpp
	../TestHelpers.h
	${CMAKE_SOURCE_DIR}/src/BlockTypeRegistry.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
)
target_link_libraries(BlockTypeRegistryTest fmt::fmt)






# Define individual tests:

add_test(NAME BlockStateTest COMMAND BlockStateTest)
add_test(NAME BlockTypeRegistryTest COMMAND BlockTypeRegistryTest)





# Put all the tests into a solution folder (MSVC):
set_target_properties(
	BlockStateTest
	BlockTypeRegistryTest
	PROPERTIES FOLDER Tests/BlockTypeRegistry
)
