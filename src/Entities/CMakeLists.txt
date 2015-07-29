
cmake_minimum_required (VERSION 2.6)
project (MCServer)

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

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(Entity.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion -Wno-error=global-constructors -Wno-error=switch-enum ")
	set_source_files_properties(EntityEffect.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum ")
	set_source_files_properties(Floater.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion ")
	set_source_files_properties(Player.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion -Wno-error=switch-enum -Wno-error=conversion ")
endif()

if(NOT MSVC)
	add_library(Entities ${SRCS} ${HDRS})
	target_link_libraries(Entities WorldStorage)
endif()
