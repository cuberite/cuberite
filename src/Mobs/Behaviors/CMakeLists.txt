
cmake_minimum_required (VERSION 2.6)
project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
    Behavior.cpp
    BehaviorAggressive.cpp
    BehaviorAttacker.cpp
    BehaviorBrave.cpp
    BehaviorBreeder.cpp
    BehaviorDoNothing.cpp
    BehaviorDayLightBurner.cpp
    BehaviorCoward.cpp
    BehaviorItemDropper.cpp
    BehaviorItemFollower.cpp
    BehaviorItemReplacer.cpp
    BehaviorAttackerMelee.cpp
    BehaviorAttackerRanged.cpp
    BehaviorAttackerSuicideBomber.cpp
    BehaviorWanderer.cpp
    BehaviorPet.cpp
    BehaviorPetTamableWithItem.cpp
    BehaviorPetTamableWithItemShy.cpp
)

SET (HDRS
    Behavior.h
    BehaviorAggressive.h
    BehaviorAttacker.h
    BehaviorBrave.h
    BehaviorBreeder.h
    BehaviorDoNothing.h
    BehaviorDayLightBurner.h
    BehaviorCoward.h
    BehaviorItemDropper.h
    BehaviorItemFollower.h
    BehaviorItemReplacer.h
    BehaviorAttackerMelee.h
    BehaviorAttackerRanged.h
    BehaviorAttackerSuicideBomber.h
    BehaviorWanderer.h
    BehaviorPet.h
    BehaviorPetTamableWithItem.h
    BehaviorPetTamableWithItemShy.h
)

if(NOT MSVC)
	add_library(Behaviors ${SRCS} ${HDRS})
endif()
