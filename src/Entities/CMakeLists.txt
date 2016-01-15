
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	ArrowEntity.cpp
	Boat.cpp
	EnderCrystal.cpp
	Entity.cpp
	EntityEffect.cpp
	ExpBottleEntity.cpp
	ExpOrb.cpp
	FallingBlock.cpp
	FireChargeEntity.cpp
	FireworkEntity.cpp
	Floater.cpp
	GhastFireballEntity.cpp
	HangingEntity.cpp
	ItemFrame.cpp
	Minecart.cpp
	Painting.cpp
	Pawn.cpp
	Pickup.cpp
	Player.cpp
	ProjectileEntity.cpp
	SplashPotionEntity.cpp
	TNTEntity.cpp
	ThrownEggEntity.cpp
	ThrownEnderPearlEntity.cpp
	ThrownSnowballEntity.cpp
	WitherSkullEntity.cpp)

SET (HDRS
	ArrowEntity.h
	Boat.h
	EnderCrystal.h
	Entity.h
	EntityEffect.h
	ExpBottleEntity.h
	ExpOrb.h
	FallingBlock.h
	FireChargeEntity.h
	FireworkEntity.h
	Floater.h
	GhastFireballEntity.h
	HangingEntity.h
	ItemFrame.h
	Minecart.h
	Painting.h
	Pawn.h
	Pickup.h
	Player.h
	ProjectileEntity.h
	SplashPotionEntity.h
	TNTEntity.h
	ThrownEggEntity.h
	ThrownEnderPearlEntity.h
	ThrownSnowballEntity.h
	WitherSkullEntity.h)

get_directory_property(BINDING_OUTPUTS DIRECTORY ../ DEFINITION BINDING_OUTPUTS)
set_source_files_properties(${BINDING_OUTPUTS} PROPERTIES GENERATED TRUE)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(Entity.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
endif()

if(NOT MSVC)
	add_library(Entities ${SRCS} ${HDRS})
	add_dependencies(Entities BindingsGeneration)
	target_link_libraries(Entities WorldStorage)
endif()
