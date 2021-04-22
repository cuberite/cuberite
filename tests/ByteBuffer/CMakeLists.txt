include_directories(${PROJECT_SOURCE_DIR}/src/)

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/ByteBuffer.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
)

set (SHARED_HDRS
	../TestHelpers.h
	${PROJECT_SOURCE_DIR}/src/ByteBuffer.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
)

set (SRCS
	ByteBufferTest.cpp
	Stubs.cpp
)

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(ByteBuffer-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(ByteBuffer-exe fmt::fmt)
if (WIN32)
	target_link_libraries(ByteBuffer-exe ws2_32)
endif()
add_test(NAME ByteBuffer-test COMMAND ByteBuffer-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	ByteBuffer-exe
	PROPERTIES FOLDER Tests
)
