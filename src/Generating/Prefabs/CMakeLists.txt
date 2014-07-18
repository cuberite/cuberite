
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../../")

SET (SRCS
	AlchemistVillagePrefabs.cpp
	JapaneseVillagePrefabs.cpp
	NetherFortPrefabs.cpp
	PlainsVillagePrefabs.cpp
	RainbowRoadPrefabs.cpp
	SandFlatRoofVillagePrefabs.cpp
	SandVillagePrefabs.cpp
	TestRailsPrefabs.cpp
	UnderwaterBasePrefabs.cpp)

SET (HDRS
	AlchemistVillagePrefabs.h
	JapaneseVillagePrefabs.h
	NetherFortPrefabs.h
	PlainsVillagePrefabs.h
	RainbowRoadPrefabs.h
	SandFlatRoofVillagePrefabs.h
	SandVillagePrefabs.h
	TestRailsPrefabs.h
	UnderwaterBasePrefabs.h)

if(NOT MSVC)
	add_library(Generating_Prefabs ${SRCS} ${HDRS})

	target_link_libraries(Generating_Prefabs OSSupport iniFile Blocks)
endif()
