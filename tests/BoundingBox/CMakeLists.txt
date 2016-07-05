cmake_minimum_required (VERSION 2.6)

enable_testing()
add_definitions(-DTEST_GLOBALS=1)

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/BoundingBox.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/BoundingBox.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
)

set (SRCS
	BoundingBoxTest.cpp
)

if (MSVC)
	# Add the MSVC-specific LeakFinder / StackTracer sources:
	list (APPEND SHARED_SRCS ${CMAKE_SOURCE_DIR}/src/LeakFinder.cpp ${CMAKE_SOURCE_DIR}/src/StackWalker.cpp)
	list (APPEND SHARED_HDRS ${CMAKE_SOURCE_DIR}/src/LeakFinder.h   ${CMAKE_SOURCE_DIR}/src/StackWalker.h)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_flags_cxx("-Wno-error=float-equal")
endif()

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(BoundingBox-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
add_test(NAME BoundingBox-test COMMAND BoundingBox-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	BoundingBox-exe
	PROPERTIES FOLDER Tests
)
