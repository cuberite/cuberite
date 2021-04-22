include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/lib/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/BiomeDef.cpp
	${PROJECT_SOURCE_DIR}/src/BlockArea.cpp
	${PROJECT_SOURCE_DIR}/src/Cuboid.cpp
	${PROJECT_SOURCE_DIR}/src/ChunkData.cpp
	${PROJECT_SOURCE_DIR}/src/StringCompression.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp

	${PROJECT_SOURCE_DIR}/src/Bindings/LuaState.cpp

	${PROJECT_SOURCE_DIR}/src/Generating/ChunkDesc.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PiecePool.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/Prefab.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabPiecePool.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalLimit.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalStrategy.cpp

	${PROJECT_SOURCE_DIR}/src/Noise/Noise.cpp

	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/File.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/GZipFile.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.cpp

	${PROJECT_SOURCE_DIR}/src/WorldStorage/FastNBT.cpp
	${PROJECT_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.cpp
)

set (SHARED_HDRS
	${PROJECT_SOURCE_DIR}/src/BiomeDef.h
	${PROJECT_SOURCE_DIR}/src/BlockArea.h
	${PROJECT_SOURCE_DIR}/src/Cuboid.h
	${PROJECT_SOURCE_DIR}/src/ChunkData.h
	${PROJECT_SOURCE_DIR}/src/Globals.h
	${PROJECT_SOURCE_DIR}/src/StringCompression.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h

	${PROJECT_SOURCE_DIR}/src/Bindings/LuaState.h

	${PROJECT_SOURCE_DIR}/src/Generating/ChunkDesc.h
	${PROJECT_SOURCE_DIR}/src/Generating/PiecePool.h
	${PROJECT_SOURCE_DIR}/src/Generating/Prefab.h
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabPiecePool.h
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalLimit.h
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalStrategy.h

	${PROJECT_SOURCE_DIR}/src/Noise/Noise.h

	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/File.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/GZipFile.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.h

	${PROJECT_SOURCE_DIR}/src/WorldStorage/FastNBT.h
	${PROJECT_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.h
)

set (SRCS
	LoadablePieces.cpp
	Stubs.cpp
	LuaState_Typedefs.inc
	LuaState_Declaration.inc
	Bindings.h
)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options("-Wno-error=global-constructors")
	add_compile_options("-Wno-error=switch-enum")
endif()


if (MSVC)
	# Add the MSVC-specific LeakFinder sources:
	list (APPEND SHARED_SRCS ${PROJECT_SOURCE_DIR}/src/LeakFinder.cpp ${PROJECT_SOURCE_DIR}/src/StackWalker.cpp)
	list (APPEND SHARED_HDRS ${PROJECT_SOURCE_DIR}/src/LeakFinder.h   ${PROJECT_SOURCE_DIR}/src/StackWalker.h)
endif()

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
source_group("Data files" FILES Test.cubeset Test1.schematic)
add_executable(LoadablePieces ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS} Test.cubeset Test1.schematic)
target_link_libraries(LoadablePieces tolualib zlib)
add_test(NAME LoadablePieces-test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND LoadablePieces)




# Put the projects into solution folders (MSVC):
set_target_properties(
	LoadablePieces
	PROPERTIES FOLDER Tests
)
