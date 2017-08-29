
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
    Behavior.cpp
    BehaviorAggressive.cpp
    BehaviorBreeder.cpp
    BehaviorChaser.cpp
    BehaviorDoNothing.cpp
    BehaviorDayLightBurner.cpp
    BehaviorCoward.cpp
    BehaviorItemDropper.cpp
    BehaviorItemFollower.cpp
    BehaviorItemReplacer.cpp
    BehaviorStriker.cpp
    BehaviorWanderer.cpp
)

SET (HDRS
    Behavior.h
    BehaviorAggressive.h
    BehaviorBreeder.h
    BehaviorChaser.h
    BehaviorDoNothing.h
    BehaviorDayLightBurner.h
    BehaviorCoward.h
    BehaviorItemDropper.h
    BehaviorItemFollower.h
    BehaviorItemReplacer.h
    BehaviorStriker.h
    BehaviorWanderer.h
)

if(NOT MSVC)
	add_library(Behaviors ${SRCS} ${HDRS})
endif()
