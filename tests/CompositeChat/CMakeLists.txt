cmake_minimum_required(VERSION 3.0.2)
enable_testing()
add_definitions(-DTEST_GLOBALS=1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/jsoncpp/include)

add_definitions(-DTEST_GLOBALS=1)

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/CompositeChat.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/CompositeChat.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
)

set (SRCS
	CompositeChatTest.cpp
	ClientHandle.cpp
)


source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(CompositeChat-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(CompositeChat-exe jsoncpp_lib_static fmt::fmt)
add_test(NAME CompositeChat-test COMMAND CompositeChat-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	CompositeChat-exe
	PROPERTIES FOLDER Tests
)
