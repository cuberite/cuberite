
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../../")

SET (SRCS
	NetherFortPrefabs.cpp
	RainbowRoadPrefabs.cpp
	TestRailsPrefabs.cpp
	UnderwaterBasePrefabs.cpp
)

SET (HDRS
	NetherFortPrefabs.h
	RainbowRoadPrefabs.h
	TestRailsPrefabs.h
	UnderwaterBasePrefabs.h
)

if(NOT MSVC)
	add_library(Generating_Prefabs ${SRCS} ${HDRS})

	target_link_libraries(Generating_Prefabs OSSupport Blocks)
endif()
