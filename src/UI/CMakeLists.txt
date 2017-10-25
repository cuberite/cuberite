project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	SlotArea.cpp
	Window.cpp
	AnvilWindow.cpp
	BeaconWindow.cpp
	BrewingstandWindow.cpp
	ChestWindow.cpp
	CraftingWindow.cpp
	DropSpenserWindow.cpp
	EnchantingWindow.cpp
	EnderChestWindow.cpp
	FurnaceWindow.cpp
	HopperWindow.cpp
	HorseWindow.cpp
	InventoryWindow.cpp)

SET (HDRS
	SlotArea.h
	Window.h
	AnvilWindow.h
	BeaconWindow.h
	BrewingstandWindow.h
	ChestWindow.h
	CraftingWindow.h
	DropSpenserWindow.h
	EnchantingWindow.h
	EnderChestWindow.h
	FurnaceWindow.h
	HopperWindow.h
	HorseWindow.h
	InventoryWindow.h
	MinecartWithChestWindow.h
	WindowOwner.h)

if(NOT MSVC)
	add_library(UI ${SRCS} ${HDRS})
	target_link_libraries(UI fmt::fmt)
endif()
