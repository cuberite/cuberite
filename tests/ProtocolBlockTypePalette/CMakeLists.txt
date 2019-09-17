cmake_minimum_required(VERSION 3.0.2)
enable_testing()
add_definitions(-DTEST_GLOBALS=1)

include_directories(SYSTEM "../../lib/jsoncpp/include")
include_directories(${CMAKE_SOURCE_DIR}/src/)


add_definitions(-DTEST_GLOBALS=1)

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/Protocol/ProtocolBlockTypePalette.cpp
	${CMAKE_SOURCE_DIR}/src/BlockState.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
)

set (SHARED_HDRS
	../TestHelpers.h
	${CMAKE_SOURCE_DIR}/src/Protocol/ProtocolBlockTypePalette.h
	${CMAKE_SOURCE_DIR}/src/BlockState.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
)

set (SRCS
	ProtocolBlockTypePaletteTest.cpp
)

file (COPY
	test.btp.json
	../../Server/Protocol/1.13/base.btp.json
	DESTINATION ./)

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(ProtocolBlockTypePaletteTest-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(ProtocolBlockTypePaletteTest-exe fmt::fmt jsoncpp)
if (WIN32)
	target_link_libraries(ProtocolBlockTypePaletteTest-exe ws2_32)
endif()
add_test(NAME ProtocolBlockTypePaletteTest-test COMMAND ProtocolBlockTypePaletteTest-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	ProtocolBlockTypePaletteTest-exe
	PROPERTIES FOLDER Tests
)
