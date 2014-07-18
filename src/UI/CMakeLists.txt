
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	SlotArea.cpp
	Window.cpp)

SET (HDRS
	SlotArea.h
	Window.h
	WindowOwner.h)

if(NOT MSVC)
	add_library(UI ${SRCS} ${HDRS})
endif()
