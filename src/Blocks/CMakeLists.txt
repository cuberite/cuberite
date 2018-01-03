project (Cuberite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	BlockBed.cpp
	BlockDoor.cpp
	BlockHandler.cpp
	BlockPiston.cpp
	ChunkInterface.cpp
)

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
	BlockCocoaPod.h
	BlockCommandBlock.h
	BlockComparator.h
	BlockConcretePowder.h
	BlockCrops.h
	BlockDeadBush.h
	BlockDirt.h
	BlockDoor.h
	BlockDropSpenser.h
	BlockEnchantmentTable.h
	BlockEnderchest.h
	BlockEntity.h
	BlockFarmland.h
	BlockFence.h
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
	BlockHopper.h
	BlockIce.h
	BlockLadder.h
	BlockLeaves.h
	BlockLever.h
	BlockLilypad.h
	BlockMelon.h
	BlockMobHead.h
	BlockMobSpawner.h
	BlockMushroom.h
	BlockMycelium.h
	BlockNetherrack.h
	BlockNetherWart.h
	BlockOre.h
	BlockPiston.h
	BlockPlanks.h
	BlockPlant.h
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
	BlockSeaLantern.h
	BlockSideways.h
	BlockSignPost.h
	BlockSlab.h
	BlockSnow.h
	BlockStairs.h
	BlockStems.h
	BlockStone.h
	BlockSugarcane.h
	BlockTallGrass.h
	BlockTNT.h
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
	GetHandlerCompileTimeTemplate.h
	MetaRotator.h
	WorldInterface.h
)

if(NOT MSVC)
	add_library(Blocks ${SRCS} ${HDRS})
	target_link_libraries(Blocks fmt::fmt)
endif()
