include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/lib/jsoncpp/include)

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/CompositeChat.cpp
	${PROJECT_SOURCE_DIR}/src/JsonUtils.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
)

set (SHARED_HDRS
	../TestHelpers.h
	${PROJECT_SOURCE_DIR}/src/CompositeChat.h
	${PROJECT_SOURCE_DIR}/src/JsonUtils.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
)

set (SRCS
	CompositeChatTest.cpp
	ClientHandle.cpp
)


source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(CompositeChat-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(CompositeChat-exe jsoncpp_lib fmt::fmt)
add_test(NAME CompositeChat-test COMMAND CompositeChat-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	CompositeChat-exe
	PROPERTIES FOLDER Tests
)
