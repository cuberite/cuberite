include_directories(${PROJECT_SOURCE_DIR}/src/)

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/FastRandom.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
)

set (SHARED_HDRS
	../TestHelpers.h
	${PROJECT_SOURCE_DIR}/src/FastRandom.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
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
