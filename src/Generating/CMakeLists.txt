
cmake_minimum_required (VERSION 2.6)
project (MCServer)

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
	NetherFortGen.cpp
	Noise3DGenerator.cpp
	POCPieceGenerator.cpp
	PieceGenerator.cpp
	Prefab.cpp
	PrefabPiecePool.cpp
	RainbowRoadsGen.cpp
	Ravines.cpp
	RoughRavines.cpp
	StructGen.cpp
	TestRailsGen.cpp
	Trees.cpp
	TwoHeights.cpp
	UnderwaterBaseGen.cpp
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
	NetherFortGen.h
	Noise3DGenerator.h
	POCPieceGenerator.h
	PieceGenerator.h
	Prefab.h
	PrefabPiecePool.h
	ProtIntGen.h
	RainbowRoadsGen.h
	Ravines.h
	RoughRavines.h
	ShapeGen.cpp
	StructGen.h
	TestRailsGen.h
	Trees.h
	TwoHeights.h
	UnderwaterBaseGen.h
	VillageGen.h
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(BioGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum")
	set_source_files_properties(CompoGenBiomal.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors ")
	set_source_files_properties(ComposableGenerator.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum -Wno-error=old-style-cast")
	set_source_files_properties(DistortedHeightmap.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=old-style-cast")
	set_source_files_properties(EndGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=old-style-cast")
	set_source_files_properties(FinishGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum -Wno-error=switch")
	set_source_files_properties(HeiGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=old-style-cast")
	set_source_files_properties(NetherFortGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(Noise3DGenerator.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=old-style-cast")
	set_source_files_properties(PieceGenerator.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(Prefab.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(RainbowRoadsGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(Ravines.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=old-style-cast")
	set_source_files_properties(RoughRavines.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=float-equal -Wno-error=old-style-cast")
	set_source_files_properties(StructGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum -Wno-error=switch -Wno-error=old-style-cast")
	set_source_files_properties(TestRailsGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(UnderwaterBaseGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors -Wno-error=switch-enum")
	set_source_files_properties(VillageGen.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors -Wno-error=switch-enum")
endif()

if(NOT MSVC)
	add_library(Generating ${SRCS} ${HDRS})
	target_link_libraries(Generating OSSupport Blocks Bindings)
endif()
