
#include "Globals.h"
#include "BlockHandler.h"
#include "../Item.h"
#include "../World.h"
#include "../Root.h"
#include "../Bindings/PluginManager.h"
#include "../Chunk.h"
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
#include "BlockCommandBlock.h"
#include "BlockComparator.h"
#include "BlockCrops.h"
#include "BlockDeadBush.h"
#include "BlockDirt.h"
#include "BlockDoor.h"
#include "BlockDropSpenser.h"
#include "BlockEnchantmentTable.h"
#include "BlockEnderchest.h"
#include "BlockEntity.h"
#include "BlockFarmland.h"
#include "BlockFenceGate.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFlowerPot.h"
#include "BlockFluid.h"
#include "BlockFurnace.h"
#include "BlockGlass.h"
#include "BlockGlowstone.h"
#include "BlockGravel.h"
#include "BlockHayBale.h"
#include "BlockMobHead.h"
#include "BlockHopper.h"
#include "BlockIce.h"
#include "BlockLadder.h"
#include "BlockLeaves.h"
#include "BlockLilypad.h"
#include "BlockNewLeaves.h"
#include "BlockLever.h"
#include "BlockMelon.h"
#include "BlockMushroom.h"
#include "BlockMycelium.h"
#include "BlockNetherWart.h"
#include "BlockNote.h"
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
#include "BlockSideways.h"
#include "BlockSignPost.h"
#include "BlockSlab.h"
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
			if (Handler == NULL)
			{
				printf("NULL handler for block type %d!\n", Type);
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
		case E_BLOCK_ACACIA_WOOD_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_ACTIVATOR_RAIL:        return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_ANVIL:                 return new cBlockAnvilHandler           (a_BlockType);
		case E_BLOCK_BED:                   return new cBlockBedHandler             (a_BlockType);
		case E_BLOCK_BIG_FLOWER:            return new cBlockBigFlowerHandler       (a_BlockType);
		case E_BLOCK_BIRCH_WOOD_STAIRS:     return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_BREWING_STAND:         return new cBlockBrewingStandHandler    (a_BlockType);
		case E_BLOCK_BRICK_STAIRS:          return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_BROWN_MUSHROOM:        return new cBlockMushroomHandler        (a_BlockType);
		case E_BLOCK_CACTUS:                return new cBlockCactusHandler          (a_BlockType);
		case E_BLOCK_CAKE:                  return new cBlockCakeHandler            (a_BlockType);
		case E_BLOCK_CARROTS:               return new cBlockCropsHandler           (a_BlockType);
		case E_BLOCK_CARPET:                return new cBlockCarpetHandler          (a_BlockType);
		case E_BLOCK_CAULDRON:              return new cBlockCauldronHandler        (a_BlockType);
		case E_BLOCK_CHEST:                 return new cBlockChestHandler           (a_BlockType);
		case E_BLOCK_COAL_ORE:              return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_COMMAND_BLOCK:         return new cBlockCommandBlockHandler    (a_BlockType);
		case E_BLOCK_ACTIVE_COMPARATOR:     return new cBlockComparatorHandler      (a_BlockType);
		case E_BLOCK_COBBLESTONE:           return new cBlockStoneHandler           (a_BlockType);
		case E_BLOCK_COBBLESTONE_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_COBWEB:                return new cBlockCobWebHandler          (a_BlockType);
		case E_BLOCK_CROPS:                 return new cBlockCropsHandler           (a_BlockType);
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:  return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_DEAD_BUSH:             return new cBlockDeadBushHandler        (a_BlockType);
		case E_BLOCK_DETECTOR_RAIL:         return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_DIAMOND_ORE:           return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_DIRT:                  return new cBlockDirtHandler            (a_BlockType);
		case E_BLOCK_DISPENSER:             return new cBlockDropSpenserHandler     (a_BlockType);
		case E_BLOCK_DOUBLE_STONE_SLAB:     return new cBlockDoubleSlabHandler      (a_BlockType);
		case E_BLOCK_DOUBLE_WOODEN_SLAB:    return new cBlockDoubleSlabHandler      (a_BlockType);
		case E_BLOCK_DROPPER:               return new cBlockDropSpenserHandler     (a_BlockType);
		case E_BLOCK_EMERALD_ORE:           return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_ENCHANTMENT_TABLE:     return new cBlockEnchantmentTableHandler(a_BlockType);
		case E_BLOCK_ENDER_CHEST:           return new cBlockEnderchestHandler      (a_BlockType);
		case E_BLOCK_FARMLAND:              return new cBlockFarmlandHandler        (a_BlockType);
		case E_BLOCK_FENCE_GATE:            return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_FIRE:                  return new cBlockFireHandler            (a_BlockType);
		case E_BLOCK_FLOWER_POT:            return new cBlockFlowerPotHandler       (a_BlockType);
		case E_BLOCK_FURNACE:               return new cBlockFurnaceHandler         (a_BlockType);
		case E_BLOCK_GLOWSTONE:             return new cBlockGlowstoneHandler       (a_BlockType);
		case E_BLOCK_GOLD_ORE:              return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_GLASS:                 return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_GLASS_PANE:            return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_GRASS:                 return new cBlockDirtHandler            (a_BlockType);
		case E_BLOCK_GRAVEL:                return new cBlockGravelHandler          (a_BlockType);
		case E_BLOCK_HAY_BALE:              return new cBlockHayBaleHandler         (a_BlockType);
		case E_BLOCK_HEAD:                  return new cBlockMobHeadHandler         (a_BlockType);
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return new cBlockPressurePlateHandler(a_BlockType);
		case E_BLOCK_HOPPER:                return new cBlockHopperHandler          (a_BlockType);
		case E_BLOCK_ICE:                   return new cBlockIceHandler             (a_BlockType);
		case E_BLOCK_INACTIVE_COMPARATOR:   return new cBlockComparatorHandler      (a_BlockType);
		case E_BLOCK_IRON_DOOR:             return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_IRON_ORE:              return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_JACK_O_LANTERN:        return new cBlockPumpkinHandler         (a_BlockType);
		case E_BLOCK_JUKEBOX:               return new cBlockEntityHandler          (a_BlockType);
		case E_BLOCK_JUNGLE_WOOD_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_LADDER:                return new cBlockLadderHandler          (a_BlockType);
		case E_BLOCK_LEVER:                 return new cBlockLeverHandler           (a_BlockType);
		case E_BLOCK_LAPIS_ORE:             return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_LAVA:                  return new cBlockLavaHandler            (a_BlockType);
		case E_BLOCK_LEAVES:                return new cBlockLeavesHandler          (a_BlockType);
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return new cBlockPressurePlateHandler(a_BlockType);
		case E_BLOCK_LILY_PAD:              return new cBlockLilypadHandler         (a_BlockType);
		case E_BLOCK_LIT_FURNACE:           return new cBlockFurnaceHandler         (a_BlockType);
		case E_BLOCK_LOG:                   return new cBlockSidewaysHandler        (a_BlockType);
		case E_BLOCK_MELON:                 return new cBlockMelonHandler           (a_BlockType);
		case E_BLOCK_MELON_STEM:            return new cBlockStemsHandler           (a_BlockType);
		case E_BLOCK_MYCELIUM:              return new cBlockMyceliumHandler        (a_BlockType);
		case E_BLOCK_NETHER_BRICK_STAIRS:   return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_NETHER_PORTAL:         return new cBlockPortalHandler          (a_BlockType);
		case E_BLOCK_NETHER_WART:           return new cBlockNetherWartHandler      (a_BlockType);
		case E_BLOCK_NETHER_QUARTZ_ORE:     return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_NEW_LEAVES:            return new cBlockNewLeavesHandler       (a_BlockType);
		case E_BLOCK_NEW_LOG:               return new cBlockSidewaysHandler        (a_BlockType);
		case E_BLOCK_NOTE_BLOCK:            return new cBlockNoteHandler            (a_BlockType);
		case E_BLOCK_PISTON:                return new cBlockPistonHandler          (a_BlockType);
		case E_BLOCK_PISTON_EXTENSION:      return new cBlockPistonHeadHandler;
		case E_BLOCK_PLANKS:                return new cBlockPlanksHandler          (a_BlockType);
		case E_BLOCK_POTATOES:              return new cBlockCropsHandler           (a_BlockType);
		case E_BLOCK_POWERED_RAIL:          return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_PUMPKIN:               return new cBlockPumpkinHandler         (a_BlockType);
		case E_BLOCK_PUMPKIN_STEM:          return new cBlockStemsHandler           (a_BlockType);
		case E_BLOCK_QUARTZ_BLOCK:          return new cBlockQuartzHandler          (a_BlockType);
		case E_BLOCK_QUARTZ_STAIRS:         return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_RAIL:                  return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_REDSTONE_LAMP_ON:      return new cBlockRedstoneLampHandler    (a_BlockType);
		case E_BLOCK_REDSTONE_ORE:          return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_REDSTONE_ORE_GLOWING:  return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_REDSTONE_REPEATER_OFF: return new cBlockRedstoneRepeaterHandler(a_BlockType);
		case E_BLOCK_REDSTONE_REPEATER_ON:  return new cBlockRedstoneRepeaterHandler(a_BlockType);
		case E_BLOCK_REDSTONE_TORCH_OFF:    return new cBlockRedstoneTorchHandler   (a_BlockType);
		case E_BLOCK_REDSTONE_TORCH_ON:     return new cBlockRedstoneTorchHandler   (a_BlockType);
		case E_BLOCK_REDSTONE_WIRE:         return new cBlockRedstoneHandler        (a_BlockType);
		case E_BLOCK_RED_MUSHROOM:          return new cBlockMushroomHandler        (a_BlockType);
		case E_BLOCK_RED_ROSE:              return new cBlockFlowerHandler          (a_BlockType);
		case E_BLOCK_SAND:                  return new cBlockSandHandler            (a_BlockType);
		case E_BLOCK_SANDSTONE_STAIRS:      return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_SAPLING:               return new cBlockSaplingHandler         (a_BlockType);
		case E_BLOCK_SIGN_POST:             return new cBlockSignPostHandler        (a_BlockType);
		case E_BLOCK_SNOW:                  return new cBlockSnowHandler            (a_BlockType);
		case E_BLOCK_SPRUCE_WOOD_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_STAINED_GLASS:         return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_STAINED_GLASS_PANE:    return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_STATIONARY_LAVA:       return new cBlockLavaHandler            (a_BlockType);
		case E_BLOCK_STATIONARY_WATER:      return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_STICKY_PISTON:         return new cBlockPistonHandler          (a_BlockType);
		case E_BLOCK_STONE:                 return new cBlockStoneHandler           (a_BlockType);
		case E_BLOCK_STONE_BRICK_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_STONE_BUTTON:          return new cBlockButtonHandler          (a_BlockType);
		case E_BLOCK_STONE_PRESSURE_PLATE:  return new cBlockPressurePlateHandler   (a_BlockType);
		case E_BLOCK_STONE_SLAB:            return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_SUGARCANE:             return new cBlockSugarcaneHandler       (a_BlockType);
		case E_BLOCK_TALL_GRASS:            return new cBlockTallGrassHandler       (a_BlockType);
		case E_BLOCK_TORCH:                 return new cBlockTorchHandler           (a_BlockType);
		case E_BLOCK_TRAPDOOR:              return new cBlockTrapdoorHandler        (a_BlockType);
		case E_BLOCK_TNT:                   return new cBlockTNTHandler             (a_BlockType);
		case E_BLOCK_TRAPPED_CHEST:         return new cBlockChestHandler           (a_BlockType);
		case E_BLOCK_TRIPWIRE:              return new cBlockTripwireHandler        (a_BlockType);
		case E_BLOCK_TRIPWIRE_HOOK:         return new cBlockTripwireHookHandler    (a_BlockType);
		case E_BLOCK_VINES:                 return new cBlockVineHandler            (a_BlockType);
		case E_BLOCK_WALLSIGN:              return new cBlockWallSignHandler        (a_BlockType);
		case E_BLOCK_WATER:                 return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_WOODEN_BUTTON:         return new cBlockButtonHandler          (a_BlockType);
		case E_BLOCK_WOODEN_DOOR:           return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_WOODEN_PRESSURE_PLATE: return new cBlockPressurePlateHandler   (a_BlockType);
		case E_BLOCK_WOODEN_SLAB:           return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_WOODEN_STAIRS:         return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_WOOL:                  return new cBlockClothHandler           (a_BlockType);
		case E_BLOCK_WORKBENCH:             return new cBlockWorkbenchHandler       (a_BlockType);
		case E_BLOCK_YELLOW_FLOWER:         return new cBlockFlowerHandler          (a_BlockType);
			
		default: return new cBlockHandler(a_BlockType);
	}
}





cBlockHandler::cBlockHandler(BLOCKTYPE a_BlockType)
{
	m_BlockType = a_BlockType;
}





bool cBlockHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	// By default, all blocks can be placed and the meta is copied over from the item's damage value:
	a_BlockType = m_BlockType;
	a_BlockMeta = (NIBBLETYPE)(a_Player->GetEquippedItem().m_ItemDamage & 0x0f);
	return true;
}





void cBlockHandler::OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlacedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
}





void cBlockHandler::OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlaced(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	// Notify the neighbors
	NeighborChanged(a_ChunkInterface, a_BlockX - 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX + 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY - 1, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY + 1, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ - 1);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ + 1);
}





void cBlockHandler::OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Notify the neighbors
	NeighborChanged(a_ChunkInterface, a_BlockX - 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX + 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY - 1, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY + 1, a_BlockZ);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ - 1);
	NeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ + 1);
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if ((a_BlockY >= 0) && (a_BlockY < cChunkDef::Height))
	{
		cBlockInfo::GetHandler(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ))->OnNeighborChanged(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
	}
}




void cBlockHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
	// Setting the meta to a_BlockMeta keeps most textures. The few other blocks have to override this.
	a_Pickups.push_back(cItem(m_BlockType, 1, a_BlockMeta));
}





void cBlockHandler::DropBlock(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_BlockPluginInterface, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, bool a_DropVerbatim)
{
	cItems Pickups;
	NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

	if (!a_DropVerbatim)
	{
		ConvertToPickups(Pickups, Meta);
	}
	else
	{
		// TODO: Add a proper overridable function for this
		Pickups.Add(m_BlockType, 1, Meta);
	}
	
	// Allow plugins to modify the pickups:
	a_BlockPluginInterface.CallHookBlockToPickups(a_Digger, a_BlockX, a_BlockY, a_BlockZ, m_BlockType, Meta, Pickups);
	
	if (!Pickups.empty())
	{
		MTRand r1;

		// Mid-block position first
		double MicroX, MicroY, MicroZ;
		MicroX = a_BlockX + 0.5;
		MicroY = a_BlockY + 0.5;
		MicroZ = a_BlockZ + 0.5;

		// Add random offset second
		MicroX += r1.rand(1) - 0.5;
		MicroZ += r1.rand(1) - 0.5;

		a_WorldInterface.SpawnItemPickups(Pickups, MicroX, MicroY, MicroZ);
	}
}





const char * cBlockHandler::GetStepSound()
{
	return "step.stone";
}





bool cBlockHandler::CanBeAt(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ, const cChunk & a_Chunk)
{
	return true;
}





bool cBlockHandler::CanDirtGrowGrass(NIBBLETYPE a_Meta)
{
	return ((cBlockInfo::IsTransparent(m_BlockType)) || (cBlockInfo::IsOneHitDig(m_BlockType)));
}





bool cBlockHandler::IsUseable()
{
	return false;
}





bool cBlockHandler::IsClickedThrough(void)
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(void)
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesDropOnUnsuitable(void)
{
	return true;
}





void cBlockHandler::Check(cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface, int a_RelX, int a_RelY, int a_RelZ, cChunk & a_Chunk)
{
	if (!CanBeAt(a_ChunkInterface, a_RelX, a_RelY, a_RelZ, a_Chunk))
	{
		if (DoesDropOnUnsuitable())
		{
			int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
			int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
			DropBlock(a_ChunkInterface, *a_Chunk.GetWorld(), a_PluginInterface, NULL, BlockX, a_RelY, BlockZ);
		}
		
		a_Chunk.SetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_AIR, 0);
	}
	else
	{
		// Wake up the simulators for this block:
		int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
		a_Chunk.GetWorld()->GetSimulatorManager()->WakeUp(BlockX, a_RelY, BlockZ, &a_Chunk);
	}
}




