
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	SlotArea.cpp
	Window.cpp
	AnvilWindow.cpp
	BeaconWindow.cpp
	ChestWindow.cpp
	CraftingWindow.cpp
	DropSpenserWindow.cpp
	EnchantingWindow.cpp
	EnderChestWindow.cpp
	FurnaceWindow.cpp
	HopperWindow.cpp
	InventoryWindow.cpp)

SET (HDRS
	SlotArea.h
	Window.h
	AnvilWindow.h
	BeaconWindow.h
	ChestWindow.h
	CraftingWindow.h
	DropSpenserWindow.h
	EnchantingWindow.h
	EnderChestWindow.h
	FurnaceWindow.h
	HopperWindow.h
	InventoryWindow.h
	MinecartWithChestWindow.h
	WindowOwner.h)

if(NOT MSVC)
	add_library(UI ${SRCS} ${HDRS})
endif()
