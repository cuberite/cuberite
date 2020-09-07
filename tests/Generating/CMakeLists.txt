include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/lib/)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/lib/mbedtls/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (SHARED_SRCS
	${CMAKE_SOURCE_DIR}/src/BiomeDef.cpp
	${CMAKE_SOURCE_DIR}/src/BlockArea.cpp
	${CMAKE_SOURCE_DIR}/src/BlockInfo.cpp
	${CMAKE_SOURCE_DIR}/src/BlockType.cpp
	${CMAKE_SOURCE_DIR}/src/Cuboid.cpp
	${CMAKE_SOURCE_DIR}/src/ChunkData.cpp
	${CMAKE_SOURCE_DIR}/src/Defines.cpp
	${CMAKE_SOURCE_DIR}/src/Enchantments.cpp
	${CMAKE_SOURCE_DIR}/src/FastRandom.cpp
	${CMAKE_SOURCE_DIR}/src/IniFile.cpp
	${CMAKE_SOURCE_DIR}/src/ProbabDistrib.cpp
	${CMAKE_SOURCE_DIR}/src/StringCompression.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
	${CMAKE_SOURCE_DIR}/src/VoronoiMap.cpp

	${CMAKE_SOURCE_DIR}/src/Bindings/LuaState.cpp  # Needed for PrefabPiecePool loading

	${CMAKE_SOURCE_DIR}/src/Noise/Noise.cpp

	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp  # Needed for LuaState
	${CMAKE_SOURCE_DIR}/src/OSSupport/File.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/GZipFile.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp

	${CMAKE_SOURCE_DIR}/src/WorldStorage/FastNBT.cpp
	${CMAKE_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.cpp
)

set (GENERATING_SRCS
	${CMAKE_SOURCE_DIR}/src/Generating/BioGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/Caves.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/ChunkDesc.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/ChunkGenerator.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/CompoGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/CompoGenBiomal.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/ComposableGenerator.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/DistortedHeightmap.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/DungeonRoomsFinisher.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/EndGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/FinishGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/GridStructGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/HeiGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/MineShafts.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/Noise3DGenerator.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/PieceGeneratorBFSTree.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/PiecePool.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/PieceStructuresGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/Prefab.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/PrefabPiecePool.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/PrefabStructure.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/Ravines.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/RoughRavines.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/SinglePieceStructuresGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/StructGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/Trees.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/TwoHeights.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalLimit.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalStrategy.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/VillageGen.cpp
)

set (SHARED_HDRS
	${CMAKE_SOURCE_DIR}/src/BiomeDef.h
	${CMAKE_SOURCE_DIR}/src/BlockArea.h
	${CMAKE_SOURCE_DIR}/src/BlockInfo.h
	${CMAKE_SOURCE_DIR}/src/BlockType.h
	${CMAKE_SOURCE_DIR}/src/Cuboid.h
	${CMAKE_SOURCE_DIR}/src/ChunkData.h
	${CMAKE_SOURCE_DIR}/src/ChunkDef.h
	${CMAKE_SOURCE_DIR}/src/Defines.h
	${CMAKE_SOURCE_DIR}/src/Enchantments.h
	${CMAKE_SOURCE_DIR}/src/FastRandom.h
	${CMAKE_SOURCE_DIR}/src/Globals.h
	${CMAKE_SOURCE_DIR}/src/IniFile.h
	${CMAKE_SOURCE_DIR}/src/ProbabDistrib.h
	${CMAKE_SOURCE_DIR}/src/StringCompression.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
	${CMAKE_SOURCE_DIR}/src/VoronoiMap.h

	${CMAKE_SOURCE_DIR}/src/Bindings/LuaState.h

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

set (GENERATING_HDRS
	${CMAKE_SOURCE_DIR}/src/Generating/BioGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/Caves.h
	${CMAKE_SOURCE_DIR}/src/Generating/ChunkDesc.h
	${CMAKE_SOURCE_DIR}/src/Generating/ChunkGenerator.h
	${CMAKE_SOURCE_DIR}/src/Generating/CompoGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/CompoGenBiomal.h
	${CMAKE_SOURCE_DIR}/src/Generating/ComposableGenerator.h
	${CMAKE_SOURCE_DIR}/src/Generating/CompositedHeiGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/DistortedHeightmap.h
	${CMAKE_SOURCE_DIR}/src/Generating/DungeonRoomsFinisher.h
	${CMAKE_SOURCE_DIR}/src/Generating/EndGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/FinishGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/GridStructGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/HeiGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/IntGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/MineShafts.h
	${CMAKE_SOURCE_DIR}/src/Generating/Noise3DGenerator.h
	${CMAKE_SOURCE_DIR}/src/Generating/PieceGeneratorBFSTree.h
	${CMAKE_SOURCE_DIR}/src/Generating/PiecePool.h
	${CMAKE_SOURCE_DIR}/src/Generating/PieceStructuresGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/Prefab.h
	${CMAKE_SOURCE_DIR}/src/Generating/PrefabPiecePool.h
	${CMAKE_SOURCE_DIR}/src/Generating/PrefabStructure.h
	${CMAKE_SOURCE_DIR}/src/Generating/ProtIntGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/Ravines.h
	${CMAKE_SOURCE_DIR}/src/Generating/RoughRavines.h
	${CMAKE_SOURCE_DIR}/src/Generating/ShapeGen.cpp
	${CMAKE_SOURCE_DIR}/src/Generating/SinglePieceStructuresGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/StructGen.h
	${CMAKE_SOURCE_DIR}/src/Generating/Trees.h
	${CMAKE_SOURCE_DIR}/src/Generating/TwoHeights.h
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalLimit.h
	${CMAKE_SOURCE_DIR}/src/Generating/VerticalStrategy.h
	${CMAKE_SOURCE_DIR}/src/Generating/VillageGen.h
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
	${CMAKE_SOURCE_DIR}/src/IniFile.cpp
	${CMAKE_SOURCE_DIR}/src/mbedTLS++/Sha1Checksum.cpp
)
target_link_libraries(BasicGeneratorTest GeneratorTestingSupport mbedtls)
file(COPY "${CMAKE_SOURCE_DIR}/Server/items.ini" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
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
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Server/Prefabs/PieceStructures
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
