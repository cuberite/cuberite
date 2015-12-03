cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/)
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
	${CMAKE_SOURCE_DIR}/src/Generating/PieceGenerator.cpp
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
	${CMAKE_SOURCE_DIR}/src/Generating/PieceGenerator.h
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

	${CMAKE_SOURCE_DIR}/src/WorldStorage/FastNBT.h
	${CMAKE_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.h
)

set (SRCS
	LoadablePieces.cpp
	Stubs.cpp
	LuaState_Typedefs.inc
	LuaState_Declaration.inc
	Bindings.h
)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_flags_cxx("-Wno-error=conversion -Wno-error=old-style-cast")
	add_flags_cxx("-Wno-error=global-constructors")
	add_flags_cxx("-Wno-error=switch-enum")
endif()


if (MSVC)
	# Add the MSVC-specific LeakFinder sources:
	list (APPEND SHARED_SRCS ${CMAKE_SOURCE_DIR}/src/LeakFinder.cpp ${CMAKE_SOURCE_DIR}/src/StackWalker.cpp)
	list (APPEND SHARED_HDRS ${CMAKE_SOURCE_DIR}/src/LeakFinder.h   ${CMAKE_SOURCE_DIR}/src/StackWalker.h)
endif()

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(LoadablePieces ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(LoadablePieces tolualib zlib)
add_test(NAME LoadablePieces-test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND LoadablePieces)




