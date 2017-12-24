
#include "Globals.h"
#include "BlockHandler.h"
#include "../Item.h"
#include "../World.h"
#include "../Chunk.h"
#include "BlockPluginInterface.h"
#include "BlockAnvil.h"
#include "BlockBed.h"
#include "BlockBigFlower.h"
#include "BlockBrewingStand.h"
#include "BlockButton.h"
#include "BlockCactus.h"
#include "BlockCake.h"
#include "BlockCarpet.h"
#include "BlockCauldron.h"
#include "BlockChest.h"
#include "BlockCloth.h"
#include "BlockCobWeb.h"
#include "BlockCocoaPod.h"
#include "BlockCommandBlock.h"
#include "BlockComparator.h"
#include "BlockConcretePowder.h"
#include "BlockCrops.h"
#include "BlockDeadBush.h"
#include "BlockDirt.h"
#include "BlockDoor.h"
#include "BlockDropSpenser.h"
#include "BlockEnchantmentTable.h"
#include "BlockEnderchest.h"
#include "BlockEntity.h"
#include "BlockFarmland.h"
#include "BlockFence.h"
#include "BlockFenceGate.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFlowerPot.h"
#include "BlockFluid.h"
#include "BlockFurnace.h"
#include "BlockGlass.h"
#include "BlockGlowstone.h"
#include "BlockGravel.h"
#include "BlockMobHead.h"
#include "BlockHopper.h"
#include "BlockIce.h"
#include "BlockLadder.h"
#include "BlockLeaves.h"
#include "BlockLilypad.h"
#include "BlockLever.h"
#include "BlockMelon.h"
#include "BlockMobSpawner.h"
#include "BlockMushroom.h"
#include "BlockMycelium.h"
#include "BlockNetherrack.h"
#include "BlockNetherWart.h"
#include "BlockOre.h"
#include "BlockPiston.h"
#include "BlockPlanks.h"
#include "BlockPortal.h"
#include "BlockPumpkin.h"
#include "BlockPressurePlate.h"
#include "BlockQuartz.h"
#include "BlockRail.h"
#include "BlockRedstone.h"
#include "BlockRedstoneLamp.h"
#include "BlockRedstoneRepeater.h"
#include "BlockRedstoneTorch.h"
#include "BlockTNT.h"
#include "BlockTripwire.h"
#include "BlockTripwireHook.h"
#include "BlockSand.h"
#include "BlockSapling.h"
#include "BlockSeaLantern.h"
#include "BlockSideways.h"
#include "BlockSignPost.h"
#include "BlockSlab.h"
#include "BlockSlime.h"
#include "BlockSnow.h"
#include "BlockStairs.h"
#include "BlockStems.h"
#include "BlockStone.h"
#include "BlockSugarcane.h"
#include "BlockTallGrass.h"
#include "BlockTorch.h"
#include "BlockTrapdoor.h"
#include "BlockVine.h"
#include "BlockWallSign.h"
#include "BlockWorkbench.h"






/*
// Tests the meta rotation and mirroring.
// Note that the cMetaRotator needs to have its assert paths disabled for this test to work!
static class cBlockHandlerRotationTester
{
public:
	cBlockHandlerRotationTester(void)
	{
		printf("Performing block handlers test...\n");
		for (BLOCKTYPE Type = 0; Type < E_BLOCK_MAX_TYPE_ID; Type++)
		{
			cBlockHandler * Handler = cBlockInfo::GetHandler(Type);
			if (Handler == nullptr)
			{
				printf("nullptr handler for block type %d!\n", Type);
				continue;
			}
			AString BlockName = ItemTypeToString(Type);
			for (NIBBLETYPE Meta = 0; Meta < 16; Meta++)
			{
				// Test the CW / CCW rotations:
				NIBBLETYPE TestMeta;
				TestMeta = Handler->MetaRotateCW(Handler->MetaRotateCW(Handler->MetaRotateCW(Handler->MetaRotateCW(Meta))));
				if (TestMeta != Meta)
				{
					// 4 CW rotations should produce no change in the meta
					printf("Handler for blocktype %d (%s) fails CW 4-rotation test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}
				TestMeta = Handler->MetaRotateCCW(Handler->MetaRotateCCW(Handler->MetaRotateCCW(Handler->MetaRotateCCW(Meta))));
				if (TestMeta != Meta)
				{
					// 4 CCW rotations should produce no change in the meta
					printf("Handler for blocktype %d (%s) fails CCW 4-rotation test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}
				TestMeta = Handler->MetaRotateCCW(Handler->MetaRotateCW(Meta));
				if (TestMeta != Meta)
				{
					// CCW rotation of a CW rotation should produce no change in the meta
					printf("Handler for blocktype %d (%s) fails CCW(CW) rotation test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}
				TestMeta = Handler->MetaRotateCW(Handler->MetaRotateCCW(Meta));
				if (TestMeta != Meta)
				{
					// CW rotation of a CCW rotation should produce no change in the meta
					printf("Handler for blocktype %d (%s) fails CW(CCW) rotation test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}

				// Test the mirroring:
				TestMeta = Handler->MetaMirrorXY(Handler->MetaMirrorXY(Meta));
				if (TestMeta != Meta)
				{
					// Double-mirroring should produce the same meta:
					printf("Handler for blocktype %d (%s) fails XY mirror test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}
				TestMeta = Handler->MetaMirrorXZ(Handler->MetaMirrorXZ(Meta));
				if (TestMeta != Meta)
				{
					// Double-mirroring should produce the same meta:
					printf("Handler for blocktype %d (%s) fails XZ mirror test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}
				TestMeta = Handler->MetaMirrorYZ(Handler->MetaMirrorYZ(Meta));
				if (TestMeta != Meta)
				{
					// Double-mirroring should produce the same meta:
					printf("Handler for blocktype %d (%s) fails YZ mirror test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}

				// Test mirror-rotating:
				TestMeta = Handler->MetaRotateCW(Handler->MetaRotateCW(Handler->MetaMirrorXY(Handler->MetaMirrorYZ(Meta))));
				if (TestMeta != Meta)
				{
					// 2 CW rotations should be the same as XY, YZ mirroring:
					printf("Handler for blocktype %d (%s) fails rotation-mirror test for meta %d: got back %d\n", Type, BlockName.c_str(), Meta, TestMeta);
				}
			}
		}  // for Type
		printf("Block handlers test complete.\n");
	}
} g_BlockHandlerRotationTester;
//*/





cBlockHandler * cBlockHandler::CreateBlockHandler(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		// Block handlers, alphabetically sorted:
		case E_BLOCK_ACACIA_DOOR:                   return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_ACACIA_FENCE:                  return new cBlockFenceHandler           (a_BlockType);
		case E_BLOCK_ACACIA_FENCE_GATE:             return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_ACACIA_WOOD_STAIRS:            return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_ACTIVATOR_RAIL:                return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_ANVIL:                         return new cBlockAnvilHandler           (a_BlockType);
		case E_BLOCK_BEACON:                        return new cBlockEntityHandler          (a_BlockType);
		case E_BLOCK_BED:                           return new cBlockBedHandler             (a_BlockType);
		case E_BLOCK_BEETROOTS:                     return new cBlockCropsHandler<3>        (a_BlockType);  // 4 stages of growth
		case E_BLOCK_BIG_FLOWER:                    return new cBlockBigFlowerHandler       (a_BlockType);
		case E_BLOCK_BIRCH_DOOR:                    return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_BIRCH_FENCE:                   return new cBlockFenceHandler           (a_BlockType);
		case E_BLOCK_BIRCH_FENCE_GATE:              return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_BIRCH_WOOD_STAIRS:             return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_BREWING_STAND:                 return new cBlockBrewingStandHandler    (a_BlockType);
		case E_BLOCK_BRICK_STAIRS:                  return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_BROWN_MUSHROOM:                return new cBlockMushroomHandler        (a_BlockType);
		case E_BLOCK_CACTUS:                        return new cBlockCactusHandler          (a_BlockType);
		case E_BLOCK_CAKE:                          return new cBlockCakeHandler            (a_BlockType);
		case E_BLOCK_CARROTS:                       return new cBlockCropsHandler<7>        (a_BlockType);  // 8 stages of growth
		case E_BLOCK_CARPET:                        return new cBlockCarpetHandler          (a_BlockType);
		case E_BLOCK_CAULDRON:                      return new cBlockCauldronHandler        (a_BlockType);
		case E_BLOCK_CHAIN_COMMAND_BLOCK:           return new cBlockCommandBlockHandler    (a_BlockType);
		case E_BLOCK_CHEST:                         return new cBlockChestHandler           (a_BlockType);
		case E_BLOCK_CLAY:                          return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_COAL_ORE:                      return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_COCOA_POD:                     return new cBlockCocoaPodHandler        (a_BlockType);
		case E_BLOCK_COMMAND_BLOCK:                 return new cBlockCommandBlockHandler    (a_BlockType);
		case E_BLOCK_ACTIVE_COMPARATOR:             return new cBlockComparatorHandler      (a_BlockType);
		case E_BLOCK_CONCRETE_POWDER:               return new cBlockConcretePowderHandler  (a_BlockType);
		case E_BLOCK_COBBLESTONE:                   return new cBlockStoneHandler           (a_BlockType);
		case E_BLOCK_COBBLESTONE_STAIRS:            return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_COBWEB:                        return new cBlockCobWebHandler          (a_BlockType);
		case E_BLOCK_CROPS:                         return new cBlockCropsHandler<7>        (a_BlockType);  // 8 stages of growth
		case E_BLOCK_DARK_OAK_DOOR:                 return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_DARK_OAK_FENCE:                return new cBlockFenceHandler           (a_BlockType);
		case E_BLOCK_DARK_OAK_FENCE_GATE:           return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:          return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_DEAD_BUSH:                     return new cBlockDeadBushHandler        (a_BlockType);
		case E_BLOCK_DETECTOR_RAIL:                 return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_DIAMOND_ORE:                   return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_DIRT:                          return new cBlockDirtHandler            (a_BlockType);
		case E_BLOCK_DISPENSER:                     return new cBlockDropSpenserHandler     (a_BlockType);
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:     return new cBlockDoubleSlabHandler      (a_BlockType);
		case E_BLOCK_DOUBLE_STONE_SLAB:             return new cBlockDoubleSlabHandler      (a_BlockType);
		case E_BLOCK_DOUBLE_WOODEN_SLAB:            return new cBlockDoubleSlabHandler      (a_BlockType);
		case E_BLOCK_DROPPER:                       return new cBlockDropSpenserHandler     (a_BlockType);
		case E_BLOCK_EMERALD_ORE:                   return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_ENCHANTMENT_TABLE:             return new cBlockEnchantmentTableHandler(a_BlockType);
		case E_BLOCK_ENDER_CHEST:                   return new cBlockEnderchestHandler      (a_BlockType);
		case E_BLOCK_FARMLAND:                      return new cBlockFarmlandHandler        (a_BlockType);
		case E_BLOCK_FENCE:                         return new cBlockFenceHandler           (a_BlockType);
		case E_BLOCK_FROSTED_ICE:                   return new cBlockIceHandler             (a_BlockType);
		case E_BLOCK_FIRE:                          return new cBlockFireHandler            (a_BlockType);
		case E_BLOCK_FLOWER_POT:                    return new cBlockFlowerPotHandler       (a_BlockType);
		case E_BLOCK_FURNACE:                       return new cBlockFurnaceHandler         (a_BlockType);
		case E_BLOCK_GLOWSTONE:                     return new cBlockGlowstoneHandler       (a_BlockType);
		case E_BLOCK_GOLD_ORE:                      return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_GLASS:                         return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_GLASS_PANE:                    return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_GRASS:                         return new cBlockDirtHandler            (a_BlockType);
		case E_BLOCK_GRAVEL:                        return new cBlockGravelHandler          (a_BlockType);
		case E_BLOCK_HAY_BALE:                      return new cBlockSidewaysHandler        (a_BlockType);
		case E_BLOCK_HEAD:                          return new cBlockMobHeadHandler         (a_BlockType);
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return new cBlockPressurePlateHandler   (a_BlockType);
		case E_BLOCK_HOPPER:                        return new cBlockHopperHandler          (a_BlockType);
		case E_BLOCK_ICE:                           return new cBlockIceHandler             (a_BlockType);
		case E_BLOCK_INACTIVE_COMPARATOR:           return new cBlockComparatorHandler      (a_BlockType);
		case E_BLOCK_IRON_DOOR:                     return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_IRON_ORE:                      return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_IRON_TRAPDOOR:                 return new cBlockTrapdoorHandler        (a_BlockType);
		case E_BLOCK_JACK_O_LANTERN:                return new cBlockPumpkinHandler         (a_BlockType);
		case E_BLOCK_JUKEBOX:                       return new cBlockEntityHandler          (a_BlockType);
		case E_BLOCK_JUNGLE_DOOR:                   return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_JUNGLE_FENCE:                  return new cBlockFenceHandler           (a_BlockType);
		case E_BLOCK_JUNGLE_FENCE_GATE:             return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_JUNGLE_WOOD_STAIRS:            return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_LADDER:                        return new cBlockLadderHandler          (a_BlockType);
		case E_BLOCK_LEVER:                         return new cBlockLeverHandler           (a_BlockType);
		case E_BLOCK_LAPIS_ORE:                     return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_LAVA:                          return new cBlockLavaHandler            (a_BlockType);
		case E_BLOCK_LEAVES:                        return new cBlockLeavesHandler          (a_BlockType);
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return new cBlockPressurePlateHandler   (a_BlockType);
		case E_BLOCK_LILY_PAD:                      return new cBlockLilypadHandler         (a_BlockType);
		case E_BLOCK_LIT_FURNACE:                   return new cBlockFurnaceHandler         (a_BlockType);
		case E_BLOCK_LOG:                           return new cBlockSidewaysHandler        (a_BlockType);
		case E_BLOCK_MELON:                         return new cBlockMelonHandler           (a_BlockType);
		case E_BLOCK_MELON_STEM:                    return new cBlockStemsHandler           (a_BlockType);
		case E_BLOCK_MOB_SPAWNER:                   return new cBlockMobSpawnerHandler      (a_BlockType);
		case E_BLOCK_MYCELIUM:                      return new cBlockMyceliumHandler        (a_BlockType);
		case E_BLOCK_NETHER_BRICK_FENCE:            return new cBlockFenceHandler           (a_BlockType);
		case E_BLOCK_NETHER_BRICK_STAIRS:           return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_NETHER_PORTAL:                 return new cBlockPortalHandler          (a_BlockType);
		case E_BLOCK_NETHER_WART:                   return new cBlockNetherWartHandler      (a_BlockType);
		case E_BLOCK_NETHERRACK:                    return new cBlockNetherrack             (a_BlockType);
		case E_BLOCK_NETHER_QUARTZ_ORE:             return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_NEW_LEAVES:                    return new cBlockLeavesHandler          (a_BlockType);
		case E_BLOCK_NEW_LOG:                       return new cBlockSidewaysHandler        (a_BlockType);
		case E_BLOCK_RED_SANDSTONE_SLAB:            return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_NOTE_BLOCK:                    return new cBlockEntityHandler          (a_BlockType);
		case E_BLOCK_OAK_FENCE_GATE:                return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_PACKED_ICE:                    return new cBlockIceHandler             (a_BlockType);
		case E_BLOCK_PISTON:                        return new cBlockPistonHandler          (a_BlockType);
		case E_BLOCK_PISTON_EXTENSION:              return new cBlockPistonHeadHandler;
		case E_BLOCK_PLANKS:                        return new cBlockPlanksHandler          (a_BlockType);
		case E_BLOCK_POTATOES:                      return new cBlockCropsHandler<7>        (a_BlockType);  // 8 stages of growth
		case E_BLOCK_POWERED_RAIL:                  return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_PUMPKIN:                       return new cBlockPumpkinHandler         (a_BlockType);
		case E_BLOCK_PUMPKIN_STEM:                  return new cBlockStemsHandler           (a_BlockType);
		case E_BLOCK_PURPUR_DOUBLE_SLAB:            return new cBlockDoubleSlabHandler      (a_BlockType);
		case E_BLOCK_PURPUR_SLAB:                   return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_PURPUR_STAIRS:                 return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_QUARTZ_BLOCK:                  return new cBlockQuartzHandler          (a_BlockType);
		case E_BLOCK_QUARTZ_STAIRS:                 return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_RAIL:                          return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_RED_SANDSTONE_STAIRS:          return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_REDSTONE_LAMP_ON:              return new cBlockRedstoneLampHandler    (a_BlockType);
		case E_BLOCK_REDSTONE_ORE:                  return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_REDSTONE_ORE_GLOWING:          return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_REDSTONE_REPEATER_OFF:         return new cBlockRedstoneRepeaterHandler(a_BlockType);
		case E_BLOCK_REDSTONE_REPEATER_ON:          return new cBlockRedstoneRepeaterHandler(a_BlockType);
		case E_BLOCK_REDSTONE_TORCH_OFF:            return new cBlockRedstoneTorchHandler   (a_BlockType);
		case E_BLOCK_REDSTONE_TORCH_ON:             return new cBlockRedstoneTorchHandler   (a_BlockType);
		case E_BLOCK_REDSTONE_WIRE:                 return new cBlockRedstoneHandler        (a_BlockType);
		case E_BLOCK_RED_MUSHROOM:                  return new cBlockMushroomHandler        (a_BlockType);
		case E_BLOCK_RED_ROSE:                      return new cBlockFlowerHandler          (a_BlockType);
		case E_BLOCK_REPEATING_COMMAND_BLOCK:       return new cBlockCommandBlockHandler    (a_BlockType);
		case E_BLOCK_SAND:                          return new cBlockSandHandler            (a_BlockType);
		case E_BLOCK_SANDSTONE_STAIRS:              return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_SAPLING:                       return new cBlockSaplingHandler         (a_BlockType);
		case E_BLOCK_SEA_LANTERN:                   return new cBlockSeaLanternHandler      (a_BlockType);
		case E_BLOCK_SIGN_POST:                     return new cBlockSignPostHandler        (a_BlockType);
		case E_BLOCK_SNOW:                          return new cBlockSnowHandler            (a_BlockType);
		case E_BLOCK_SLIME_BLOCK:                   return new cBlockSlimeHandler           (a_BlockType);
		case E_BLOCK_SPRUCE_DOOR:                   return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_SPRUCE_FENCE:                  return new cBlockFenceHandler           (a_BlockType);
		case E_BLOCK_SPRUCE_FENCE_GATE:             return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_SPRUCE_WOOD_STAIRS:            return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_STAINED_GLASS:                 return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_STAINED_GLASS_PANE:            return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_STATIONARY_LAVA:               return new cBlockLavaHandler            (a_BlockType);
		case E_BLOCK_STATIONARY_WATER:              return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_STICKY_PISTON:                 return new cBlockPistonHandler          (a_BlockType);
		case E_BLOCK_STONE:                         return new cBlockStoneHandler           (a_BlockType);
		case E_BLOCK_STONE_BRICK_STAIRS:            return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_STONE_BUTTON:                  return new cBlockButtonHandler          (a_BlockType);
		case E_BLOCK_STONE_PRESSURE_PLATE:          return new cBlockPressurePlateHandler   (a_BlockType);
		case E_BLOCK_STONE_SLAB:                    return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_SUGARCANE:                     return new cBlockSugarcaneHandler       (a_BlockType);
		case E_BLOCK_TALL_GRASS:                    return new cBlockTallGrassHandler       (a_BlockType);
		case E_BLOCK_TORCH:                         return new cBlockTorchHandler           (a_BlockType);
		case E_BLOCK_TRAPDOOR:                      return new cBlockTrapdoorHandler        (a_BlockType);
		case E_BLOCK_TNT:                           return new cBlockTNTHandler             (a_BlockType);
		case E_BLOCK_TRAPPED_CHEST:                 return new cBlockChestHandler           (a_BlockType);
		case E_BLOCK_TRIPWIRE:                      return new cBlockTripwireHandler        (a_BlockType);
		case E_BLOCK_TRIPWIRE_HOOK:                 return new cBlockTripwireHookHandler    (a_BlockType);
		case E_BLOCK_VINES:                         return new cBlockVineHandler            (a_BlockType);
		case E_BLOCK_WALLSIGN:                      return new cBlockWallSignHandler        (a_BlockType);
		case E_BLOCK_WATER:                         return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_WOODEN_BUTTON:                 return new cBlockButtonHandler          (a_BlockType);
		case E_BLOCK_OAK_DOOR:                      return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_WOODEN_PRESSURE_PLATE:         return new cBlockPressurePlateHandler   (a_BlockType);
		case E_BLOCK_WOODEN_SLAB:                   return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_OAK_WOOD_STAIRS:               return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_WOOL:                          return new cBlockClothHandler           (a_BlockType);
		case E_BLOCK_WORKBENCH:                     return new cBlockWorkbenchHandler       (a_BlockType);
		case E_BLOCK_YELLOW_FLOWER:                 return new cBlockFlowerHandler          (a_BlockType);

		default: return new cBlockHandler(a_BlockType);
	}
}





cBlockHandler::cBlockHandler(BLOCKTYPE a_BlockType)
{
	m_BlockType = a_BlockType;
}





bool cBlockHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	// By default, all blocks can be placed and the meta is copied over from the item's damage value:
	a_BlockType = m_BlockType;
	a_BlockMeta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage & 0x0f);
	return true;
}





void cBlockHandler::OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
	OnPlaced(a_ChunkInterface, a_WorldInterface, a_BlockChange.GetX(), a_BlockChange.GetY(), a_BlockChange.GetZ(), a_BlockChange.m_BlockType, a_BlockChange.m_BlockMeta);
}





void cBlockHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	// Notify the neighbors
	NeighborChanged(a_ChunkInterface, a_BlockX - 1, a_BlockY, a_BlockZ, BLOCK_FACE_XP);
	NeighborChanged(a_ChunkInterface, a_BlockX + 1, a_BlockY, a_BlockZ, BLOCK_FACE_XM);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY - 1, a_BlockZ, BLOCK_FACE_YP);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_YM);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ - 1, BLOCK_FACE_ZP);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ + 1, BLOCK_FACE_ZM);
}





void cBlockHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Notify the neighbors
	NeighborChanged(a_ChunkInterface, a_BlockX - 1, a_BlockY, a_BlockZ, BLOCK_FACE_XP);
	NeighborChanged(a_ChunkInterface, a_BlockX + 1, a_BlockY, a_BlockZ, BLOCK_FACE_XM);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY - 1, a_BlockZ, BLOCK_FACE_YP);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY + 1, a_BlockZ, BLOCK_FACE_YM);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ - 1, BLOCK_FACE_ZP);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ + 1, BLOCK_FACE_ZM);
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, int a_NeighborX, int a_NeighborY, int a_NeighborZ, eBlockFace a_WhichNeighbor)
{
	if ((a_NeighborY >= 0) && (a_NeighborY < cChunkDef::Height))
	{
		cBlockInfo::GetHandler(a_ChunkInterface.GetBlock({a_NeighborX, a_NeighborY, a_NeighborZ}))->OnNeighborChanged(a_ChunkInterface, a_NeighborX, a_NeighborY, a_NeighborZ, a_WhichNeighbor);
	}
}





void cBlockHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
	// Setting the meta to a_BlockMeta keeps most textures. The few other blocks have to override this.
	a_Pickups.push_back(cItem(m_BlockType, 1, a_BlockMeta));
}





void cBlockHandler::DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_CanDrop)
{
	cItems Pickups;
	NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta({a_BlockX, a_BlockY, a_BlockZ});

	if (a_CanDrop)
	{
		if ((a_Digger != nullptr) && (a_Digger->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchSilkTouch) > 0))
		{
			switch (m_BlockType)
			{
				case E_BLOCK_ACACIA_DOOR:
				case E_BLOCK_ACTIVE_COMPARATOR:
				case E_BLOCK_BEETROOTS:
				case E_BLOCK_BIRCH_DOOR:
				case E_BLOCK_BREWING_STAND:
				case E_BLOCK_CAKE:
				case E_BLOCK_CARROTS:
				case E_BLOCK_CAULDRON:
				case E_BLOCK_COCOA_POD:
				case E_BLOCK_CROPS:
				case E_BLOCK_DARK_OAK_DOOR:
				case E_BLOCK_DEAD_BUSH:
				case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
				case E_BLOCK_DOUBLE_STONE_SLAB:
				case E_BLOCK_DOUBLE_WOODEN_SLAB:
				case E_BLOCK_FIRE:
				case E_BLOCK_FARMLAND:
				case E_BLOCK_FLOWER_POT:
				case E_BLOCK_HEAD:
				case E_BLOCK_INACTIVE_COMPARATOR:
				case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
				case E_BLOCK_IRON_DOOR:
				case E_BLOCK_JUNGLE_DOOR:
				case E_BLOCK_MELON_STEM:
				case E_BLOCK_MOB_SPAWNER:
				case E_BLOCK_NETHER_WART:
				case E_BLOCK_OAK_DOOR:
				case E_BLOCK_PISTON_EXTENSION:
				case E_BLOCK_POTATOES:
				case E_BLOCK_PUMPKIN_STEM:
				case E_BLOCK_PURPUR_DOUBLE_SLAB:
				case E_BLOCK_REDSTONE_ORE_GLOWING:
				case E_BLOCK_REDSTONE_REPEATER_OFF:
				case E_BLOCK_REDSTONE_REPEATER_ON:
				case E_BLOCK_REDSTONE_TORCH_OFF:
				case E_BLOCK_REDSTONE_WIRE:
				case E_BLOCK_SIGN_POST:
				case E_BLOCK_SNOW:
				case E_BLOCK_SPRUCE_DOOR:
				case E_BLOCK_STANDING_BANNER:
				case E_BLOCK_SUGARCANE:
				case E_BLOCK_TALL_GRASS:
				case E_BLOCK_TRIPWIRE:
				case E_BLOCK_WALL_BANNER:
				case E_BLOCK_WALLSIGN:
				{
					// Silktouch can't be used for these blocks
					ConvertToPickups(Pickups, Meta);
					break;
				}
				case E_BLOCK_BED:
				{
					// Need to access the bed entity to get the color for the item damage
					ConvertToPickups(a_WorldInterface, Pickups, Meta, a_BlockX, a_BlockY, a_BlockZ);
				}
				case E_BLOCK_ENDER_CHEST:
				{
					// Reset meta to 0
					Pickups.Add(m_BlockType, 1, 0);
					break;
				}
				case E_BLOCK_LEAVES:
				case E_BLOCK_NEW_LEAVES:
				{
					Pickups.Add(m_BlockType, 1, Meta & 0x03);
					break;
				}
				default: Pickups.Add(m_BlockType, 1, Meta); break;
			}
		}
		else if (m_BlockType == E_BLOCK_BED)
		{
			// Need to access the bed entity to get the color for the item damage
			ConvertToPickups(a_WorldInterface, Pickups, Meta, a_BlockX, a_BlockY, a_BlockZ);
		}
		else
		{
			ConvertToPickups(Pickups, Meta);
		}
	}

	// Allow plugins to modify the pickups:
	a_BlockPluginInterface.CallHookBlockToPickups(a_Digger, a_BlockX, a_BlockY, a_BlockZ, m_BlockType, Meta, Pickups);

	if (!Pickups.empty())
	{
		auto & r1 = GetRandomProvider();

		// Mid-block position first
		double MicroX, MicroY, MicroZ;
		MicroX = a_BlockX + 0.5;
		MicroY = a_BlockY + 0.5;
		MicroZ = a_BlockZ + 0.5;

		// Add random offset second
		MicroX += r1.RandReal<double>(-0.5, 0.5);
		MicroZ += r1.RandReal<double>(-0.5, 0.5);

		a_WorldInterface.SpawnItemPickups(Pickups, MicroX, MicroY, MicroZ);
	}
}




bool cBlockHandler::CanBeAt(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, const cChunk & a_Chunk)
{
	return true;
}





bool cBlockHandler::IsUseable()
{
	return false;
}





bool cBlockHandler::IsClickedThrough(void)
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta)
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesDropOnUnsuitable(void)
{
	return true;
}





/* default functionality: only test for height, since we assume full voxels with varying height */
bool cBlockHandler::IsInsideBlock(Vector3d a_Position, const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta)
{
	return a_Position.y < cBlockInfo::GetBlockHeight(a_BlockType);
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP)
{
	if (!cBlockInfo::IsSolid(m_BlockType))
	{
		// Non-solid things can be placed within entities
		return cBoundingBox(0, 0, 0, 0, 0, 0);
	}
	return cBoundingBox(0, 1, 0, cBlockInfo::GetBlockHeight(m_BlockType), 0, 1);
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk)
{
	if (!CanBeAt(a_ChunkInterface, a_RelX, a_RelY, a_RelZ, a_Chunk))
	{
		if (DoesDropOnUnsuitable())
		{
			int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
			int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
			DropBlock(a_ChunkInterface, *a_Chunk.GetWorld(), a_PluginInterface, nullptr, BlockX, a_RelY, BlockZ);
		}

		a_Chunk.SetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_AIR, 0);
	}
	else
	{
		// Wake up the simulators for this block:
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		a_Chunk.GetWorld()->GetSimulatorManager()->WakeUp({BlockX, a_RelY, BlockZ}, &a_Chunk);
	}
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta)
{
	// Zero for transparent
	return 0;
}




