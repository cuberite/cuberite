include_directories(${PROJECT_SOURCE_DIR}/src/)

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/ByteBuffer.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
)

set (SHARED_HDRS
	${PROJECT_SOURCE_DIR}/src/ByteBuffer.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
)

set (SRCS
	ByteBufferTest.cpp
	Stubs.cpp
)

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(ByteBufferTest ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(ByteBufferTest fmt::fmt gtest gmock gtest_main)
if (WIN32)
	target_link_libraries(ByteBufferTest ws2_32)
endif()

add_test_executable(ByteBufferTest)





# Put the projects into solution folders (MSVC):
set_target_properties(
	ByteBufferTest
	PROPERTIES FOLDER Tests
)
