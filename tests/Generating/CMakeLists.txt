include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/lib/)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/lib/mbedtls/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/BiomeDef.cpp
	${PROJECT_SOURCE_DIR}/src/BlockArea.cpp
	${PROJECT_SOURCE_DIR}/src/BlockInfo.cpp
	${PROJECT_SOURCE_DIR}/src/BlockType.cpp
	${PROJECT_SOURCE_DIR}/src/Cuboid.cpp
	${PROJECT_SOURCE_DIR}/src/ChunkData.cpp
	${PROJECT_SOURCE_DIR}/src/Defines.cpp
	${PROJECT_SOURCE_DIR}/src/Enchantments.cpp
	${PROJECT_SOURCE_DIR}/src/FastRandom.cpp
	${PROJECT_SOURCE_DIR}/src/IniFile.cpp
	${PROJECT_SOURCE_DIR}/src/ProbabDistrib.cpp
	${PROJECT_SOURCE_DIR}/src/StringCompression.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
	${PROJECT_SOURCE_DIR}/src/VoronoiMap.cpp

	${PROJECT_SOURCE_DIR}/src/Bindings/LuaState.cpp  # Needed for PrefabPiecePool loading

	${PROJECT_SOURCE_DIR}/src/Noise/Noise.cpp

	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp  # Needed for LuaState
	${PROJECT_SOURCE_DIR}/src/OSSupport/File.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/GZipFile.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp

	${PROJECT_SOURCE_DIR}/src/WorldStorage/FastNBT.cpp
	${PROJECT_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.cpp
)

set (GENERATING_SRCS
	${PROJECT_SOURCE_DIR}/src/Generating/BioGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/Caves.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/ChunkDesc.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/ChunkGenerator.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/CompoGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/CompoGenBiomal.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/ComposableGenerator.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/DistortedHeightmap.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/DungeonRoomsFinisher.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/EndGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/FinishGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/GridStructGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/HeiGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/MineShafts.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/Noise3DGenerator.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PieceGeneratorBFSTree.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PiecePool.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PieceStructuresGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/Prefab.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabPiecePool.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabStructure.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/Ravines.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/RoughRavines.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/SinglePieceStructuresGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/StructGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/Trees.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/TwoHeights.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalLimit.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalStrategy.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/VillageGen.cpp
)

set (SHARED_HDRS
	${PROJECT_SOURCE_DIR}/src/BiomeDef.h
	${PROJECT_SOURCE_DIR}/src/BlockArea.h
	${PROJECT_SOURCE_DIR}/src/BlockInfo.h
	${PROJECT_SOURCE_DIR}/src/BlockType.h
	${PROJECT_SOURCE_DIR}/src/Cuboid.h
	${PROJECT_SOURCE_DIR}/src/ChunkData.h
	${PROJECT_SOURCE_DIR}/src/ChunkDef.h
	${PROJECT_SOURCE_DIR}/src/Defines.h
	${PROJECT_SOURCE_DIR}/src/Enchantments.h
	${PROJECT_SOURCE_DIR}/src/FastRandom.h
	${PROJECT_SOURCE_DIR}/src/Globals.h
	${PROJECT_SOURCE_DIR}/src/IniFile.h
	${PROJECT_SOURCE_DIR}/src/ProbabDistrib.h
	${PROJECT_SOURCE_DIR}/src/StringCompression.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
	${PROJECT_SOURCE_DIR}/src/VoronoiMap.h

	${PROJECT_SOURCE_DIR}/src/Bindings/LuaState.h

	${PROJECT_SOURCE_DIR}/src/Noise/Noise.h

	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/File.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/GZipFile.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.h

	${PROJECT_SOURCE_DIR}/src/WorldStorage/FastNBT.h
	${PROJECT_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.h
)

set (GENERATING_HDRS
	${PROJECT_SOURCE_DIR}/src/Generating/BioGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/Caves.h
	${PROJECT_SOURCE_DIR}/src/Generating/ChunkDesc.h
	${PROJECT_SOURCE_DIR}/src/Generating/ChunkGenerator.h
	${PROJECT_SOURCE_DIR}/src/Generating/CompoGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/CompoGenBiomal.h
	${PROJECT_SOURCE_DIR}/src/Generating/ComposableGenerator.h
	${PROJECT_SOURCE_DIR}/src/Generating/CompositedHeiGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/DistortedHeightmap.h
	${PROJECT_SOURCE_DIR}/src/Generating/DungeonRoomsFinisher.h
	${PROJECT_SOURCE_DIR}/src/Generating/EndGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/FinishGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/GridStructGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/HeiGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/IntGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/MineShafts.h
	${PROJECT_SOURCE_DIR}/src/Generating/Noise3DGenerator.h
	${PROJECT_SOURCE_DIR}/src/Generating/PieceGeneratorBFSTree.h
	${PROJECT_SOURCE_DIR}/src/Generating/PiecePool.h
	${PROJECT_SOURCE_DIR}/src/Generating/PieceStructuresGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/Prefab.h
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabPiecePool.h
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabStructure.h
	${PROJECT_SOURCE_DIR}/src/Generating/ProtIntGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/Ravines.h
	${PROJECT_SOURCE_DIR}/src/Generating/RoughRavines.h
	${PROJECT_SOURCE_DIR}/src/Generating/ShapeGen.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/SinglePieceStructuresGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/StructGen.h
	${PROJECT_SOURCE_DIR}/src/Generating/Trees.h
	${PROJECT_SOURCE_DIR}/src/Generating/TwoHeights.h
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalLimit.h
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalStrategy.h
	${PROJECT_SOURCE_DIR}/src/Generating/VillageGen.h
)

set (STUBS
	Stubs.cpp
	LuaState_Typedefs.inc
	LuaState_Declaration.inc
	Bindings.h
)


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options("-Wno-error=global-constructors")
	add_compile_options("-Wno-error=switch-enum")
endif()



add_library(GeneratorTestingSupport STATIC
	${SHARED_SRCS}
	${SHARED_HDRS}
	${GENERATING_SRCS}
	${GENERATING_HDRS}
	${STUBS}
)
target_link_libraries(GeneratorTestingSupport tolualib zlib fmt::fmt jsoncpp_lib)
source_group("Stubs" FILES ${STUBS})
source_group("Generating" FILES ${GENERATING_HDRS} ${GENERATING_SRCS})





# BasicGeneratingTest:
add_executable(BasicGeneratorTest
	BasicGeneratorTest.cpp
	${PROJECT_SOURCE_DIR}/src/IniFile.cpp
	${PROJECT_SOURCE_DIR}/src/mbedTLS++/Sha1Checksum.cpp
)
target_link_libraries(BasicGeneratorTest GeneratorTestingSupport mbedtls)
file(COPY "${PROJECT_SOURCE_DIR}/Server/items.ini" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
add_test(
	NAME BasicGeneratorTest
	COMMAND BasicGeneratorTest
)





# LoadablePieces test:
source_group("Data files" FILES Test.cubeset Test1.schematic)
add_executable(LoadablePieces
	LoadablePieces.cpp
	Test.cubeset
	Test1.schematic
)
target_link_libraries(LoadablePieces GeneratorTestingSupport)
add_test(
	NAME LoadablePieces-test
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND LoadablePieces
)





# PieceRotation test:
add_executable(PieceRotation
	PieceRotationTest.cpp
)
target_link_libraries(PieceRotation GeneratorTestingSupport)
add_test(
	NAME PieceRotation-test
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND PieceRotation
)





# PieceGeneratorBFSTree test:
add_executable(PieceGeneratorBFSTree
	PieceGeneratorBFSTreeTest.cpp
)
target_link_libraries(PieceGeneratorBFSTree GeneratorTestingSupport)
add_test(
	NAME PieceGeneratorBFSTree-test
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Server/Prefabs/PieceStructures
	COMMAND PieceGeneratorBFSTree
)





# Put the projects into solution folders (MSVC):
set_target_properties(
	BasicGeneratorTest
	GeneratorTestingSupport
	LoadablePieces
	PieceGeneratorBFSTree
	PieceRotation
	PROPERTIES FOLDER Tests/Generating
)
