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
	InventoryWindow.h
	MinecartWithChestWindow.h
	VillagerTradeWindow.h
	WindowOwner.h)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(SlotArea.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum ")
	set_source_files_properties(Window.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=switch-enum ")
endif()

if(NOT MSVC)
	add_library(UI ${SRCS} ${HDRS})
endif()
