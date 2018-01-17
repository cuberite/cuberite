cmake_minimum_required(VERSION 3.0.2)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/lib/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DTEST_GLOBALS=1)

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/BiomeDef.cpp
	${CMAKE_SOURCE_DIR}/src/BlockArea.cpp
	${CMAKE_SOURCE_DIR}/src/Cuboid.cpp
	${CMAKE_SOURCE_DIR}/src/ChunkData.cpp
	${CMAKE_SOURCE_DIR}/src/StringCompression.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp

	${CMAKE_SOURCE_DIR}/src/Bindings/LuaState.cpp

	${CMAKE_SOURCE_DIR}/src/Generating/ChunkDesc.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/PiecePool.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/Prefab.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/PrefabPiecePool.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalLimit.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalStrategy.cpp

	${CMAKE_SOURCE_DIR}/src/Noise/Noise.cpp

	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/File.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/GZipFile.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp

	${CMAKE_SOURCE_DIR}/src/WorldStorage/FastNBT.cpp
	${CMAKE_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/BiomeDef.h
	${CMAKE_SOURCE_DIR}/src/BlockArea.h
	${CMAKE_SOURCE_DIR}/src/Cuboid.h
	${CMAKE_SOURCE_DIR}/src/ChunkData.h
	${CMAKE_SOURCE_DIR}/src/Globals.h
	${CMAKE_SOURCE_DIR}/src/StringCompression.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h

	${CMAKE_SOURCE_DIR}/src/Bindings/LuaState.h

	${CMAKE_SOURCE_DIR}/src/Generating/ChunkDesc.h
	${CMAKE_SOURCE_DIR}/src/Generating/PiecePool.h
	${CMAKE_SOURCE_DIR}/src/Generating/Prefab.h
	${CMAKE_SOURCE_DIR}/src/Generating/PrefabPiecePool.h
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalLimit.h
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalStrategy.h

	${CMAKE_SOURCE_DIR}/src/Noise/Noise.h

	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/File.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/GZipFile.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.h

	${CMAKE_SOURCE_DIR}/src/WorldStorage/FastNBT.h
	${CMAKE_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.h
)

set (SRCS
	LuaThreadStress.cpp
	Stubs.cpp
	LuaState_Typedefs.inc
	LuaState_Declaration.inc
	Bindings.h
)



source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
source_group("Lua files" FILES Test.lua)
add_executable(LuaThreadStress ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS} Test.lua)
target_link_libraries(LuaThreadStress tolualib zlib fmt::fmt)
add_test(NAME LuaThreadStress-test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND LuaThreadStress)




# Put the projects into solution folders (MSVC):
set_target_properties(
	LuaThreadStress
	PROPERTIES FOLDER Tests
)
