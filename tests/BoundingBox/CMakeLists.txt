cmake_minimum_required(VERSION 3.0.2)
enable_testing()
add_definitions(-DTEST_GLOBALS=1)

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/BoundingBox.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/BoundingBox.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.h
)

set (SRCS
	BoundingBoxTest.cpp
)


source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(BoundingBox-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(BoundingBox-exe fmt::fmt)
add_test(NAME BoundingBox-test COMMAND BoundingBox-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	BoundingBox-exe
	PROPERTIES FOLDER Tests
)
