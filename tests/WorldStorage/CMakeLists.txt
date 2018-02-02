cmake_minimum_required(VERSION 3.0.2)
enable_testing()

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/WorldStorage/TreeNBT.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/WorldStorage/TreeNBT.h
	${CMAKE_SOURCE_DIR}/src/WorldStorage/NBTDef.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
)

set (SRCS
	TreeNBTTest.cpp
)

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})

add_executable(TreeNBTTest ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(TreeNBTTest fmt::fmt)
target_compile_definitions(TreeNBTTest PRIVATE TEST_GLOBALS=1)
target_include_directories(TreeNBTTest PRIVATE
	${CMAKE_SOURCE_DIR}/src/
)

add_test(NAME TreeNBT-test COMMAND TreeNBTTest)


# Put the projects into solution folders (MSVC):
set_target_properties(
	TreeNBTTest
	PROPERTIES FOLDER Tests/WorldStorage
)
