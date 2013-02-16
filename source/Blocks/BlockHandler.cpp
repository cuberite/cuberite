
#include "Globals.h"
#include "BlockHandler.h"
#include "../Item.h"
#include "../World.h"
#include "../Root.h"
#include "../PluginManager.h"
#include "BlockSand.h"
#include "BlockGravel.h"
#include "BlockDoor.h"
#include "BlockFire.h"
#include "BlockRedstone.h"
#include "BlockRedstoneTorch.h"
#include "BlockRedstoneRepeater.h"
#include "BlockPiston.h"
#include "BlockWorkbench.h"
#include "BlockEntity.h"
#include "BlockVine.h"
#include "BlockTallGrass.h"
#include "BlockSnow.h"
#include "BlockCloth.h"
#include "BlockSlab.h"
#include "BlockDirt.h"
#include "BlockTorch.h"
#include "BlockWood.h"
#include "BlockLeaves.h"
#include "BlockSapling.h"
#include "BlockFluid.h"
#include "BlockChest.h"
#include "BlockFurnace.h"
#include "BlockDispenser.h"
#include "BlockStairs.h"
#include "BlockLadder.h"
#include "BlockLever.h"
#include "BlockSign.h"
#include "BlockCrops.h"
#include "BlockSugarcane.h"
#include "BlockFlower.h"
#include "BlockMushroom.h"
#include "BlockCactus.h"
#include "BlockStems.h"
#include "BlockGlowstone.h"
#include "BlockStone.h"
#include "BlockMelon.h"
#include "BlockIce.h"
#include "BlockOre.h"
#include "BlockNote.h"
#include "BlockBed.h"
#include "BlockFarmland.h"
#include "BlockMycelium.h"
#include "BlockRail.h"
#include "BlockGlass.h"
#include "BlockEnderchest.h"
#include "BlockFenceGate.h"
#include "BlockFlowerPot.h"
#include "BlockCauldron.h"
#include "BlockBrewingStand.h"





bool            cBlockHandler::m_HandlerInitialized = false;
cBlockHandler * cBlockHandler::m_BlockHandler[256];





cBlockHandler * cBlockHandler::GetBlockHandler(BLOCKTYPE a_BlockType)
{
	if (!m_HandlerInitialized)
	{
		// We have to initialize
		memset(m_BlockHandler, 0, sizeof(m_BlockHandler));
		m_HandlerInitialized = true;
	}
	if (m_BlockHandler[a_BlockType] != NULL)
	{
		return m_BlockHandler[a_BlockType];
	}

	return m_BlockHandler[a_BlockType] = CreateBlockHandler(a_BlockType);
}





cBlockHandler * cBlockHandler::CreateBlockHandler(BLOCKTYPE a_BlockType)
{
	switch(a_BlockType)
	{
		// Block handlers, alphabetically sorted:
		case E_BLOCK_BED:                   return new cBlockBedHandler             (a_BlockType);
		case E_BLOCK_BIRCH_WOOD_STAIRS:     return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_BREWING_STAND:         return new cBlockBrewingStandHandler    (a_BlockType);
		case E_BLOCK_BRICK_STAIRS:          return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_BROWN_MUSHROOM:        return new cBlockMushroomHandler        (a_BlockType);
		case E_BLOCK_CACTUS:                return new cBlockCactusHandler          (a_BlockType);
		case E_BLOCK_CAULDRON:              return new cBlockCauldronHandler        (a_BlockType);
		case E_BLOCK_CHEST:                 return new cBlockChestHandler           (a_BlockType);
		case E_BLOCK_COAL_ORE:              return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_COBBLESTONE:           return new cBlockStoneHandler           (a_BlockType);
		case E_BLOCK_COBBLESTONE_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_CROPS:                 return new cBlockCropsHandler           (a_BlockType);
		case E_BLOCK_DIAMOND_ORE:           return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_DIRT:                  return new cBlockDirtHandler            (a_BlockType);
		case E_BLOCK_DISPENSER:             return new cBlockDispenserHandler       (a_BlockType);
		case E_BLOCK_DOUBLE_STONE_SLAB:     return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_DOUBLE_WOODEN_SLAB:    return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_EMERALD_ORE:           return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_ENDER_CHEST:           return new cBlockEnderchestHandler      (a_BlockType);
		case E_BLOCK_FARMLAND:              return new cBlockFarmlandHandler;
		case E_BLOCK_FENCE_GATE:            return new cBlockFenceGateHandler       (a_BlockType);
		case E_BLOCK_FIRE:                  return new cBlockFireHandler            (a_BlockType);
		case E_BLOCK_FLOWER_POT:            return new cBlockFlowerPotHandler       (a_BlockType);
		case E_BLOCK_FURNACE:               return new cBlockFurnaceHandler         (a_BlockType);
		case E_BLOCK_GLOWSTONE:             return new cBlockGlowstoneHandler       (a_BlockType);
		case E_BLOCK_GOLD_ORE:              return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_GLASS:                 return new cBlockGlassHandler           (a_BlockType);
		case E_BLOCK_GRASS:                 return new cBlockDirtHandler            (a_BlockType);
		case E_BLOCK_GRAVEL:                return new cBlockGravelHandler          (a_BlockType);
		case E_BLOCK_ICE:                   return new cBlockIceHandler             (a_BlockType);
		case E_BLOCK_IRON_DOOR:             return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_IRON_ORE:              return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_JUKEBOX:               return new cBlockEntityHandler          (a_BlockType);
		case E_BLOCK_JUNGLE_WOOD_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_LADDER:                return new cBlockLadderHandler          (a_BlockType);
		case E_BLOCK_LEVER:                 return new cBlockLeverHandler           (a_BlockType);
		case E_BLOCK_LAPIS_ORE:             return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_LAVA:                  return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_LEAVES:                return new cBlockLeavesHandler          (a_BlockType);
		case E_BLOCK_LIT_FURNACE:           return new cBlockFurnaceHandler         (a_BlockType);
		case E_BLOCK_LOG:                   return new cBlockWoodHandler            (a_BlockType);
		case E_BLOCK_MELON:                 return new cBlockMelonHandler           (a_BlockType);
		case E_BLOCK_MELON_STEM:            return new cBlockStemsHandler           (a_BlockType);
		case E_BLOCK_MYCELIUM:              return new cBlockMyceliumHandler        (a_BlockType);
		case E_BLOCK_NETHER_BRICK_STAIRS:   return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_NOTE_BLOCK:            return new cBlockNoteHandler            (a_BlockType);
		case E_BLOCK_PISTON:                return new cBlockPistonHandler          (a_BlockType);
		case E_BLOCK_PLANKS:                return new cBlockWoodHandler            (a_BlockType);
		case E_BLOCK_PUMPKIN_STEM:          return new cBlockStemsHandler           (a_BlockType);
		case E_BLOCK_RAIL:                  return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_POWERED_RAIL:          return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_DETECTOR_RAIL:         return new cBlockRailHandler            (a_BlockType);
		case E_BLOCK_REDSTONE_ORE:          return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_REDSTONE_ORE_GLOWING:  return new cBlockOreHandler             (a_BlockType);
		case E_BLOCK_REDSTONE_REPEATER_OFF: return new cBlockRedstoneRepeaterHandler(a_BlockType);
		case E_BLOCK_REDSTONE_REPEATER_ON:  return new cBlockRedstoneRepeaterHandler(a_BlockType);
		case E_BLOCK_REDSTONE_TORCH_OFF:    return new cBlockRedstoneTorchHandler   (a_BlockType);
		case E_BLOCK_REDSTONE_TORCH_ON:     return new cBlockRedstoneTorchHandler   (a_BlockType);
		case E_BLOCK_REDSTONE_WIRE:	        return new cBlockRedstoneHandler        (a_BlockType);
		case E_BLOCK_RED_MUSHROOM:          return new cBlockMushroomHandler        (a_BlockType);
		case E_BLOCK_RED_ROSE:              return new cBlockFlowerHandler          (a_BlockType);
		case E_BLOCK_SAND:                  return new cBlockSandHandler            (a_BlockType);
		case E_BLOCK_SANDSTONE_STAIRS:      return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_SAPLING:               return new cBlockSaplingHandler         (a_BlockType);
		case E_BLOCK_SIGN_POST:             return new cBlockSignHandler            (a_BlockType);
		case E_BLOCK_SNOW:                  return new cBlockSnowHandler            (a_BlockType);
		case E_BLOCK_SPRUCE_WOOD_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_STATIONARY_LAVA:       return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_STATIONARY_WATER:      return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_STICKY_PISTON:         return new cBlockPistonHandler          (a_BlockType);
		case E_BLOCK_STONE:                 return new cBlockStoneHandler           (a_BlockType);
		case E_BLOCK_STONE_BRICK_STAIRS:    return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_STONE_SLAB:            return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_SUGARCANE:             return new cBlockSugarcaneHandler       (a_BlockType);
		case E_BLOCK_TALL_GRASS:            return new cBlockTallGrassHandler       (a_BlockType);
		case E_BLOCK_TORCH:                 return new cBlockTorchHandler           (a_BlockType);
		case E_BLOCK_VINES:                 return new cBlockVineHandler            (a_BlockType);
		case E_BLOCK_WALLSIGN:              return new cBlockSignHandler            (a_BlockType);
		case E_BLOCK_WATER:                 return new cBlockFluidHandler           (a_BlockType);
		case E_BLOCK_WOODEN_DOOR:           return new cBlockDoorHandler            (a_BlockType);
		case E_BLOCK_WOODEN_SLAB:           return new cBlockSlabHandler            (a_BlockType);
		case E_BLOCK_WOODEN_STAIRS:         return new cBlockStairsHandler          (a_BlockType);
		case E_BLOCK_WOOL:                  return new cBlockClothHandler           (a_BlockType);
		case E_BLOCK_WORKBENCH:             return new cBlockWorkbenchHandler       (a_BlockType);
		case E_BLOCK_YELLOW_FLOWER:         return new cBlockFlowerHandler          (a_BlockType);
			
		default: return new cBlockHandler(a_BlockType);
	}
}





void cBlockHandler::Deinit()
{
	for (int i = 0; i < 256; i++)
	{
		delete m_BlockHandler[i];
	}
	memset(m_BlockHandler, 0, sizeof(m_BlockHandler));  // Don't leave any dangling pointers around, just in case
	m_HandlerInitialized = false;
}





cBlockHandler::cBlockHandler(BLOCKTYPE a_BlockType)
{
	m_BlockType = a_BlockType;
}





bool cBlockHandler::GetPlacementBlockTypeMeta(
	cWorld * a_World, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	// By default, all blocks can be placed and the meta is copied over from the item's damage value:
	a_BlockType = m_BlockType;
	a_BlockMeta = (NIBBLETYPE)(a_Player->GetEquippedItem().m_ItemDamage & 0x0f);
	return true;
}





void cBlockHandler::OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlacedByPlayer(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
}





void cBlockHandler::OnDestroyedByPlayer(cWorld *a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnPlaced(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	// Notify the neighbors
	NeighborChanged(a_World, a_BlockX - 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_World, a_BlockX + 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_World, a_BlockX, a_BlockY - 1, a_BlockZ);
	NeighborChanged(a_World, a_BlockX, a_BlockY + 1, a_BlockZ);
	NeighborChanged(a_World, a_BlockX, a_BlockY, a_BlockZ - 1);
	NeighborChanged(a_World, a_BlockX, a_BlockY, a_BlockZ + 1);
}





void cBlockHandler::OnDestroyed(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	// Notify the neighbors
	NeighborChanged(a_World, a_BlockX - 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_World, a_BlockX + 1, a_BlockY, a_BlockZ);
	NeighborChanged(a_World, a_BlockX, a_BlockY - 1, a_BlockZ);
	NeighborChanged(a_World, a_BlockX, a_BlockY + 1, a_BlockZ);
	NeighborChanged(a_World, a_BlockX, a_BlockY, a_BlockZ - 1);
	NeighborChanged(a_World, a_BlockX, a_BlockY, a_BlockZ + 1);
}





void cBlockHandler::NeighborChanged(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	GetBlockHandler(a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ))->OnNeighborChanged(a_World, a_BlockX, a_BlockY, a_BlockZ);
}





void cBlockHandler::OnNeighborChanged(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnDigging(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
{
}





void cBlockHandler::OnUse(cWorld *a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
}





void cBlockHandler::ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta)
{
	// Setting the meta to a_BlockMeta keeps most textures. The few other blocks have to override this.
	a_Pickups.push_back(cItem(m_BlockType, 1, a_BlockMeta));
}





void cBlockHandler::DropBlock(cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cItems Pickups;
	NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	ConvertToPickups(Pickups, Meta);
	
	// Allow plugins to modify the pickups:
	cRoot::Get()->GetPluginManager()->CallHookBlockToPickups(a_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, m_BlockType, Meta, Pickups);
	
	if (!Pickups.empty())
	{
		a_World->SpawnItemPickups(Pickups, a_BlockX, a_BlockY, a_BlockZ);
	}
}





const char * cBlockHandler::GetStepSound()
{
	return "step.stone";
}





bool cBlockHandler::CanBePlacedAt(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	return CanBeAt(a_World, a_BlockX, a_BlockY, a_BlockZ);
}





bool cBlockHandler::CanBeAt(cWorld *a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
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





bool cBlockHandler::DoesIgnoreBuildCollision(void)
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesAllowBlockOnTop(void)
{
	return true;
}





bool cBlockHandler::CanBePlacedOnSide(void)
{
	return true;
}





bool cBlockHandler::DoesDropOnUnsuitable(void)
{
	return true;
}





void cBlockHandler::Check(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	if (!CanBeAt(a_World, a_BlockX, a_BlockY, a_BlockZ))
	{
		if (DoesDropOnUnsuitable())
		{
			DropBlock(a_World, NULL, a_BlockX, a_BlockY, a_BlockZ);
		}
		
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
	}
	else
	{
		// Wake up the simulators:
		a_World->GetSimulatorManager()->WakeUp(a_BlockX, a_BlockY, a_BlockZ);
	}
}




