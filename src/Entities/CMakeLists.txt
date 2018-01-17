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
	LeashKnot.cpp
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
	LeashKnot.h
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

if(NOT MSVC)
	add_library(Entities ${SRCS} ${HDRS})
	target_link_libraries(Entities fmt::fmt WorldStorage)
endif()
