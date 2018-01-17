cmake_minimum_required(VERSION 3.0.2)
enable_testing()
add_definitions(-DTEST_GLOBALS=1)

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/FastRandom.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/FastRandom.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.h
)

set (SRCS
	FastRandomTest.cpp
)


source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(FastRandom-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(FastRandom-exe fmt::fmt)
if (WIN32)
	target_link_libraries(FastRandom-exe ws2_32)
endif()
add_test(NAME FastRandom-test COMMAND FastRandom-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	FastRandom-exe
	PROPERTIES FOLDER Tests
)
