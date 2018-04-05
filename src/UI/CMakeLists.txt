project (Cuberite)

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

add_library(UI ${SRCS} ${HDRS})
target_include_directories(UI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" PRIVATE ..)
target_link_libraries(UI PRIVATE fmt::fmt jsoncpp_lib_static mbedTLS++ SQLiteCpp)
