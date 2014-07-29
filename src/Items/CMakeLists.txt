
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	ItemHandler.cpp)

SET (HDRS
	ItemArmor.h
	ItemBed.h
	ItemBoat.h
	ItemBow.h
	ItemBrewingStand.h
	ItemBucket.h
	ItemCake.h
	ItemCauldron.h
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
	ItemRedstoneDust.h
	ItemRedstoneRepeater.h
	ItemSapling.h
	ItemSeeds.h
	ItemShears.h
	ItemShovel.h
	ItemSign.h
	ItemSpawnEgg.h
	ItemString.h
	ItemSugarcane.h
	ItemSword.h
	ItemThrowable.h)

if(NOT MSVC)
	add_library(Items ${SRCS} ${HDRS})
endif()
