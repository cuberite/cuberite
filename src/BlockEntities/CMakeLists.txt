
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	BeaconEntity.cpp
	BlockEntity.cpp
	ChestEntity.cpp
	CommandBlockEntity.cpp
	DispenserEntity.cpp
	DropSpenserEntity.cpp
	DropperEntity.cpp
	EnderChestEntity.cpp
	FlowerPotEntity.cpp
	FurnaceEntity.cpp
	HopperEntity.cpp
	JukeboxEntity.cpp
	MobHeadEntity.cpp
	NoteEntity.cpp
	SignEntity.cpp)

SET (HDRS
	BeaconEntity.h
	BlockEntity.h
	BlockEntityWithItems.h
	ChestEntity.h
	CommandBlockEntity.h
	DispenserEntity.h
	DropSpenserEntity.h
	DropperEntity.h
	EnderChestEntity.h
	FlowerPotEntity.h
	FurnaceEntity.h
	HopperEntity.h
	JukeboxEntity.h
	MobHeadEntity.h
	NoteEntity.h
	SignEntity.h)

if(NOT MSVC)
	add_library(BlockEntities ${SRCS} ${HDRS})
endif()
