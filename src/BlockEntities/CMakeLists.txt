project (Cuberite)

SET (SRCS
	BeaconEntity.cpp
	BedEntity.cpp
	BlockEntity.cpp
	BlockEntityWithItems.cpp
	BrewingstandEntity.cpp
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
	MobSpawnerEntity.cpp
	NoteEntity.cpp
	SignEntity.cpp
)

SET (HDRS
	BeaconEntity.h
	BedEntity.h
	BlockEntity.h
	BlockEntityWithItems.h
	BrewingstandEntity.h
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
	MobSpawnerEntity.h
	NoteEntity.h
	SignEntity.h
)

add_library(BlockEntities ${SRCS} ${HDRS})
target_include_directories(BlockEntities PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" PRIVATE ..)
target_link_libraries(BlockEntities PRIVATE fmt::fmt mbedTLS++ jsoncpp_lib_static SQLiteCpp)
