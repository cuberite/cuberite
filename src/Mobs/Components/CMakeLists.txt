cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	AIComponent.cpp
	AIAggressiveComponent.cpp
	AttackComponent.cpp
	BurningComponent.cpp
	EnvironmentComponent.cpp
	MovementComponent.cpp)

SET (HDRS
	AIComponent.h
	AIAggressiveComponent.h
	AttackComponent.h
	BurningComponent.inc
	BurningComponent.h
	EnvironmentComponent.h
	MovementComponent.h)

if(NOT MSVC)
	add_library(Components ${SRCS} ${HDRS})
endif()
