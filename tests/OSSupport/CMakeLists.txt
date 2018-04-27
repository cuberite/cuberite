cmake_minimum_required(VERSION 3.0.2)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)

# Define individual tests:

# StressEvent: Stress-test the cEvent implementation:
add_executable(StressEvent-exe StressEvent.cpp)
target_link_libraries(StressEvent-exe OSSupport fmt::fmt)
add_test(NAME StressEvent-test COMMAND StressEvent-exe)



# Put all the tests into a solution folder (MSVC):
set_target_properties(
	StressEvent-exe
	PROPERTIES FOLDER Tests/OSSupport
)
set_target_properties(
	OSSupport
	PROPERTIES FOLDER Lib
)




