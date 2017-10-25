project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	BioGen.cpp
	Caves.cpp
	ChunkDesc.cpp
	ChunkGenerator.cpp
	CompoGen.cpp
	CompoGenBiomal.cpp
	ComposableGenerator.cpp
	DistortedHeightmap.cpp
	DungeonRoomsFinisher.cpp
	EndGen.cpp
	FinishGen.cpp
	GridStructGen.cpp
	HeiGen.cpp
	MineShafts.cpp
	Noise3DGenerator.cpp
	PieceGeneratorBFSTree.cpp
	PiecePool.cpp
	PieceStructuresGen.cpp
	Prefab.cpp
	PrefabPiecePool.cpp
	PrefabStructure.cpp
	Ravines.cpp
	RoughRavines.cpp
	StructGen.cpp
	Trees.cpp
	TwoHeights.cpp
	VerticalLimit.cpp
	VerticalStrategy.cpp
	VillageGen.cpp
)

SET (HDRS
	BioGen.h
	Caves.h
	ChunkDesc.h
	ChunkGenerator.h
	CompoGen.h
	CompoGenBiomal.h
	ComposableGenerator.h
	CompositedHeiGen.h
	DistortedHeightmap.h
	DungeonRoomsFinisher.h
	EndGen.h
	FinishGen.h
	GridStructGen.h
	HeiGen.h
	IntGen.h
	MineShafts.h
	Noise3DGenerator.h
	PieceGeneratorBFSTree.h
	PiecePool.h
	PieceStructuresGen.h
	Prefab.h
	PrefabPiecePool.h
	PrefabStructure.h
	ProtIntGen.h
	Ravines.h
	RoughRavines.h
	ShapeGen.cpp
	StructGen.h
	Trees.h
	TwoHeights.h
	VerticalLimit.h
	VerticalStrategy.h
	VillageGen.h
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(CompoGenBiomal.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
endif()

if(NOT MSVC)
	add_library(Generating ${SRCS} ${HDRS})
	target_link_libraries(Generating fmt::fmt OSSupport Blocks Bindings)
endif()
