
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	BioGen.cpp
	Caves.cpp
	ChunkDesc.cpp
	ChunkGenerator.cpp
	CompoGen.cpp
	ComposableGenerator.cpp
	DistortedHeightmap.cpp
	EndGen.cpp
	FinishGen.cpp
	GridStructGen.cpp
	HeiGen.cpp
	MineShafts.cpp
	NetherFortGen.cpp
	Noise3DGenerator.cpp
	POCPieceGenerator.cpp
	PieceGenerator.cpp
	Prefab.cpp
	PrefabPiecePool.cpp
	RainbowRoadsGen.cpp
	Ravines.cpp
	StructGen.cpp
	TestRailsGen.cpp
	Trees.cpp
	UnderwaterBaseGen.cpp
	VillageGen.cpp)

SET (HDRS
	BioGen.h
	Caves.h
	ChunkDesc.h
	ChunkGenerator.h
	CompoGen.h
	ComposableGenerator.h
	DistortedHeightmap.h
	EndGen.h
	FinishGen.h
	GridStructGen.h
	HeiGen.h
	MineShafts.h
	NetherFortGen.h
	Noise3DGenerator.h
	POCPieceGenerator.h
	PieceGenerator.h
	Prefab.h
	PrefabPiecePool.h
	RainbowRoadsGen.h
	Ravines.h
	StructGen.h
	TestRailsGen.h
	Trees.h
	UnderwaterBaseGen.h
	VillageGen.h)

if(NOT MSVC)
	add_library(Generating ${SRCS} ${HDRS})

	target_link_libraries(Generating OSSupport iniFile Blocks)
endif()
