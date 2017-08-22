
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
    BehaviorAggressive.cpp
    BehaviorChaser.cpp
    BehaviorBreeder.cpp
    BehaviorDayLightBurner.cpp
    BehaviorCoward.cpp
    BehaviorItemDropper.cpp
    BehaviorItemFollower.cpp
    BehaviorStriker.cpp
    BehaviorWanderer.cpp
)

SET (HDRS
    BehaviorAggressive.h
    BehaviorChaser.h
    BehaviorBreeder.h
    BehaviorDayLightBurner.h
    BehaviorCoward.h
    BehaviorItemDropper.h
    BehaviorItemFollower.h
    BehaviorStriker.h
    BehaviorWanderer.h
)

if(NOT MSVC)
	add_library(Behaviors ${SRCS} ${HDRS})
endif()
