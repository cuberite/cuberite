enable_testing()
add_definitions(-DTEST_GLOBALS=1)

include_directories(${CMAKE_SOURCE_DIR}/src/)

add_definitions(-DTEST_GLOBALS=1)

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/FastRandom.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/FastRandom.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
)

set (SRCS
	FastRandomTest.cpp
)

if (MSVC)
	# Add the MSVC-specific LeakFinder / StackTracer sources:
	list (APPEND SHARED_SRCS ${CMAKE_SOURCE_DIR}/src/LeakFinder.cpp ${CMAKE_SOURCE_DIR}/src/StackWalker.cpp)
	list (APPEND SHARED_HDRS ${CMAKE_SOURCE_DIR}/src/LeakFinder.h   ${CMAKE_SOURCE_DIR}/src/StackWalker.h)
endif()

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(FastRandom-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
if (WIN32)
	target_link_libraries(FastRandom-exe ws2_32)
endif()
add_test(NAME FastRandom-test COMMAND FastRandom-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	FastRandom-exe
	PROPERTIES FOLDER Tests
)
