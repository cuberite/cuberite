
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
    TraitBreeder.cpp
    TraitCoward.cpp
    TraitItemFollower.cpp
    TraitWanderer.cpp
)

SET (HDRS
    TraitBreeder.h
    TraitCoward.h
    TraitItemFollower.h
    TraitWanderer.h
)

if(NOT MSVC)
	add_library(Traits ${SRCS} ${HDRS})
endif()
