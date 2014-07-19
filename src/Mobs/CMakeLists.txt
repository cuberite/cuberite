
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	AggressiveMonster.cpp
	Bat.cpp
	Blaze.cpp
	CaveSpider.cpp
	Chicken.cpp
	Cow.cpp
	Creeper.cpp
	EnderDragon.cpp
	Enderman.cpp
	Ghast.cpp
	Giant.cpp
	Horse.cpp
	IronGolem.cpp
	MagmaCube.cpp
	Monster.cpp
	Mooshroom.cpp
	PassiveAggressiveMonster.cpp
	PassiveMonster.cpp
	Pig.cpp
	Sheep.cpp
	Skeleton.cpp
	Slime.cpp
	SnowGolem.cpp
	Spider.cpp
	Squid.cpp
	Villager.cpp
	Witch.cpp
	Wither.cpp
	Wolf.cpp
	Zombie.cpp
	ZombiePigman.cpp)
	
SET (HDRS
	AggressiveMonster.h
	Bat.h
	Blaze.h
	CaveSpider.h
	Chicken.h
	Cow.h
	Creeper.h
	EnderDragon.h
	Enderman.h
	Ghast.h
	Giant.h
	Horse.h
	IncludeAllMonsters.h
	IronGolem.h
	MagmaCube.h
	Monster.h
	Mooshroom.h
	Ocelot.h
	PassiveAggressiveMonster.h
	PassiveMonster.h
	Pig.h
	Sheep.h
	Silverfish.h
	Skeleton.h
	Slime.h
	SnowGolem.h
	Spider.h
	Squid.h
	Villager.h
	Witch.h
	Wither.h
	Wolf.h
	Zombie.h
	ZombiePigman.h)

if(NOT MSVC)
	add_library(Mobs ${SRCS} ${HDRS})
endif()
