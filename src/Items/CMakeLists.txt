
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	ItemHandler.cpp)

SET (HDRS
	ItemArmor.h
	ItemBed.h
	ItemBigFlower.h
	ItemBoat.h
	ItemBow.h
	ItemBrewingStand.h
	ItemBucket.h
	ItemCake.h
	ItemCauldron.h
	ItemChest.h
	ItemCloth.h
	ItemComparator.h
	ItemDoor.h
	ItemDye.h
	ItemEmptyMap.h
	ItemFishingRod.h
	ItemFlowerPot.h
	ItemFood.h
	ItemHandler.h
	ItemHoe.h
	ItemItemFrame.h
	ItemLeaves.h
	ItemLighter.h
	ItemLilypad.h
	ItemMap.h
	ItemMilk.h
	ItemMinecart.h
	ItemMobHead.h
	ItemNetherWart.h
	ItemPainting.h
	ItemPickaxe.h
	ItemPotion.h
	ItemPumpkin.h
	ItemRedstoneDust.h
	ItemRedstoneRepeater.h
	ItemSapling.h
	ItemSeeds.h
	ItemShears.h
	ItemShovel.h
	ItemSlab.h
	ItemSign.h
	ItemSpawnEgg.h
	ItemString.h
	ItemSugarcane.h
	ItemSword.h
	ItemThrowable.h
)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(ItemHandler.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=sign-conversion -Wno-error=conversion -Wno-error=switch-enum")
endif()

if(NOT MSVC)
	add_library(Items ${SRCS} ${HDRS})
endif()
