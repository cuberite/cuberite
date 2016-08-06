
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
    BehaviorBreeder.cpp
    BehaviorCoward.cpp
    BehaviorItemFollower.cpp
    BehaviorWanderer.cpp
)

SET (HDRS
    BehaviorBreeder.h
    BehaviorCoward.h
    BehaviorItemFollower.h
    BehaviorWanderer.h
)

if(NOT MSVC)
	add_library(Behaviors ${SRCS} ${HDRS})
endif()
