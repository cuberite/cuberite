
cmake_minimum_required (VERSION 2.6)
project (MCServer)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	BlockBed.cpp
	BlockDoor.cpp
	BlockHandler.cpp
	BlockPiston.cpp
	ChunkInterface.cpp)

SET (HDRS
	BlockAnvil.h
	BlockBed.h
	BlockBigFlower.h
	BlockBrewingStand.h
	BlockButton.h
	BlockCactus.h
	BlockCake.h
	BlockCarpet.h
	BlockCauldron.h
	BlockChest.h
	BlockCloth.h
	BlockCobWeb.h
	BlockCommandBlock.h
	BlockComparator.h
	BlockCrops.h
	BlockDeadBush.h
	BlockDirt.h
	BlockDoor.h
	BlockDropSpenser.h
	BlockEnchantmentTable.h
	BlockEnderchest.h
	BlockEntity.h
	BlockFarmland.h
	BlockFenceGate.h
	BlockFire.h
	BlockFlower.h
	BlockFlowerPot.h
	BlockFluid.h
	BlockFurnace.h
	BlockGlass.h
	BlockGlowstone.h
	BlockGravel.h
	BlockHandler.h
	BlockHayBale.h
	BlockHopper.h
	BlockIce.h
	BlockLadder.h
	BlockLeaves.h
	BlockLever.h
	BlockLilypad.h
	BlockMelon.h
	BlockMobHead.h
	BlockMushroom.h
	BlockMycelium.h
	BlockNetherWart.h
	BlockNewLeaves.h
	BlockNote.h
	BlockOre.h
	BlockPiston.h
	BlockPlanks.h
	BlockPluginInterface.h
	BlockPortal.h
	BlockPressurePlate.h
	BlockPumpkin.h
	BlockQuartz.h
	BlockRail.h
	BlockRedstone.h
	BlockRedstoneLamp.h
	BlockRedstoneRepeater.h
	BlockRedstoneTorch.h
	BlockSand.h
	BlockSapling.h
	BlockSideways.h
	BlockSignPost.h
	BlockSlab.h
	BlockSnow.h
	BlockStairs.h
	BlockStems.h
	BlockStone.h
	BlockSugarcane.h
	BlockTNT.h
	BlockTallGrass.h
	BlockTorch.h
	BlockTrapdoor.h
	BlockTripwire.h
	BlockTripwireHook.h
	BlockVine.h
	BlockWallSign.h
	BlockWorkbench.h
	BroadcastInterface.h
	ChunkInterface.h
	ClearMetaOnDrop.h
	MetaRotator.h
	WorldInterface.h)

if(NOT MSVC)
	add_library(Blocks ${SRCS} ${HDRS})
endif()
