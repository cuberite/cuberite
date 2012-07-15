#include "Globals.h"
#include "Block.h"
#include "../cItem.h"
#include "../cWorld.h"
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
#include "BlockSign.h"
#include "BlockCrops.h"
#include "BlockSugarcane.h"
#include "BlockFlower.h"
#include "BlockMushroom.h"
#include "BlockCactus.h"
#include "BlockStems.h"
#include "BlockGlowstone.h"
#include "BlockRedstoneOre.h"
#include "BlockStone.h"
#include "BlockMelon.h"
#include "BlockIce.h"

bool cBlockHandler::m_HandlerInitialized = false;
cBlockHandler *cBlockHandler::m_BlockHandler[256];

cBlockHandler *cBlockHandler::GetBlockHandler(char a_BlockID)
{
	if(!m_HandlerInitialized)
	{	//We have to initialize
		memset(m_BlockHandler, 0, sizeof(m_BlockHandler));
		m_HandlerInitialized = true;
	}
	if(m_BlockHandler[a_BlockID])
		return m_BlockHandler[a_BlockID];

	return m_BlockHandler[a_BlockID] = CreateBlockHandler(a_BlockID);
}

cBlockHandler *cBlockHandler::CreateBlockHandler(BLOCKTYPE a_BlockID)
{
	switch(a_BlockID)
	{
	case E_BLOCK_WOODEN_DOOR:
	case E_BLOCK_IRON_DOOR:
		return new cBlockDoorHandler(a_BlockID);
	case E_BLOCK_FIRE:
		return new cBlockFireHandler(a_BlockID);
	case E_BLOCK_REDSTONE_TORCH_ON:
	case E_BLOCK_REDSTONE_TORCH_OFF:
		return new cBlockRedstoneTorchHandler(a_BlockID);
	case E_BLOCK_REDSTONE_WIRE:	
		return new cBlockRedstoneHandler(a_BlockID);
	case  E_BLOCK_PISTON:
	case E_BLOCK_STICKY_PISTON:
		return new cBlockPistonHandler(a_BlockID);
	case E_BLOCK_REDSTONE_REPEATER_ON:
	case E_BLOCK_REDSTONE_REPEATER_OFF:
		return new cBlockRedstoneRepeaterHandler(a_BlockID);
	case E_BLOCK_WORKBENCH:
		return new cBlockWorkbenchHandler(a_BlockID);
	case E_BLOCK_SNOW:
		return new cBlockSnowHandler(a_BlockID);
	case E_BLOCK_TALL_GRASS:
		return new cBlockTallGrassHandler(a_BlockID);
	case E_BLOCK_VINES:
		return new cBlockVineHandler(a_BlockID);
	case ::E_BLOCK_WOOL:
		return new cBlockClothHandler(a_BlockID);
	case E_BLOCK_WOODEN_SLAB:
	case E_BLOCK_STONE_SLAB:
	case E_BLOCK_DOUBLE_WOODEN_SLAB:
	case E_BLOCK_DOUBLE_STONE_SLAB:
		return new cBlockSlabHandler(a_BlockID);
	case E_BLOCK_LOG:
	case E_BLOCK_PLANKS:
		return new cBlockWoodHandler(a_BlockID);
	case E_BLOCK_TORCH:
		return new cBlockTorchHandler(a_BlockID);
	case E_BLOCK_DIRT:
	case E_BLOCK_GRASS:
		return new cBlockDirtHandler(a_BlockID);
	case E_BLOCK_LEAVES:
		return new cBlockLeavesHandler(a_BlockID);
	case E_BLOCK_SAPLING:
		return new cBlockSaplingHandler(a_BlockID);
	case E_BLOCK_WATER:
	case E_BLOCK_STATIONARY_WATER:
	case E_BLOCK_STATIONARY_LAVA:
	case E_BLOCK_LAVA:
		return new cBlockFluidHandler(a_BlockID);
	case E_BLOCK_DISPENSER:
		return new cBlockDispenserHandler(a_BlockID);
	case E_BLOCK_FURNACE:
	case E_BLOCK_LIT_FURNACE:
		return new cBlockFurnaceHandler(a_BlockID);
	case E_BLOCK_CHEST:
		return new cBlockChestHandler(a_BlockID);
	case E_BLOCK_ICE:
		return new cBlockIceHandler(a_BlockID);
	case E_BLOCK_LADDER:
		return new cBlockLadderHandler(a_BlockID);
	case E_BLOCK_COBBLESTONE_STAIRS:
	case E_BLOCK_BRICK_STAIRS:
	case E_BLOCK_STONE_BRICK_STAIRS:
	case E_BLOCK_NETHER_BRICK_STAIRS:
	case E_BLOCK_WOODEN_STAIRS:
		return new cBlockStairsHandler(a_BlockID);
	case E_BLOCK_SIGN_POST:
	case E_BLOCK_WALLSIGN:
		return new cBlockSignHandler(a_BlockID);
	case E_BLOCK_CROPS:
		return new cBlockCropsHandler(a_BlockID);
	case E_BLOCK_SUGARCANE:
		return new cBlockSugarcaneHandler(a_BlockID);
	case E_BLOCK_YELLOW_FLOWER:
	case E_BLOCK_RED_ROSE:
		return new cBlockFlowerHandler(a_BlockID);
	case E_BLOCK_BROWN_MUSHROOM:
	case E_BLOCK_RED_MUSHROOM:
		return new cBlockMushroomHandler(a_BlockID);
	case E_BLOCK_CACTUS:
		return new cBlockCactusHandler(a_BlockID);
	case E_BLOCK_MELON_STEM:
	case E_BLOCK_PUMPKIN_STEM:
		return new cBlockStemsHandler(a_BlockID);
	case E_BLOCK_GLOWSTONE:
		return new cBlockGlowstoneHandler(a_BlockID);
	case E_BLOCK_REDSTONE_ORE:
	case E_BLOCK_REDSTONE_ORE_GLOWING:
		return new cBlockRedstoneOreHandler(a_BlockID);
	case E_BLOCK_STONE:
	case E_BLOCK_COBBLESTONE:
		return new cBlockStoneHandler(a_BlockID);
	case E_BLOCK_MELON:
		return new cBlockMelonHandler(a_BlockID);
	default:		
		return new cBlockHandler(a_BlockID);
		break;
	}
}

void cBlockHandler::Deinit()
{
	for(int i = 0; i < 256; i++)
	{
		delete m_BlockHandler[i];
	}
}

cBlockHandler::cBlockHandler(BLOCKTYPE a_BlockID)
{
	m_BlockID = a_BlockID;
}

void cBlockHandler::OnUpdate(cWorld *a_World, int a_X, int a_Y, int a_Z)
{

}

void cBlockHandler::OnPlacedByPlayer(cWorld *a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z, int a_Dir)
{
}

void cBlockHandler::OnDestroyedByPlayer(cWorld *a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z)
{
}

void cBlockHandler::OnPlaced(cWorld *a_World, int a_X, int a_Y, int a_Z, int a_Dir)
{
	//Notify the neighbors
	NeighborChanged(a_World, a_X - 1, a_Y, a_Z);
	NeighborChanged(a_World, a_X + 1, a_Y, a_Z);
	NeighborChanged(a_World, a_X, a_Y - 1, a_Z);
	NeighborChanged(a_World, a_X, a_Y + 1, a_Z);
	NeighborChanged(a_World, a_X, a_Y, a_Z - 1);
	NeighborChanged(a_World, a_X, a_Y, a_Z + 1);
}

void cBlockHandler::OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	//Notify the neighbors
	NeighborChanged(a_World, a_X - 1, a_Y, a_Z);
	NeighborChanged(a_World, a_X + 1, a_Y, a_Z);
	NeighborChanged(a_World, a_X, a_Y - 1, a_Z);
	NeighborChanged(a_World, a_X, a_Y + 1, a_Z);
	NeighborChanged(a_World, a_X, a_Y, a_Z - 1);
	NeighborChanged(a_World, a_X, a_Y, a_Z + 1);
}

void cBlockHandler::NeighborChanged(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	GetBlockHandler(a_World->GetBlock(a_X, a_Y, a_Z))->OnNeighborChanged(a_World, a_X, a_Y, a_Z);
}

void cBlockHandler::OnNeighborChanged(cWorld *a_World, int a_X, int a_Y, int a_Z)
{

}

void cBlockHandler::OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z)
{

}

void cBlockHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir)
{
	a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, a_BlockMeta);
	OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
}


int cBlockHandler::GetTickRate()
{
	return 10;
}

char cBlockHandler::GetDropCount()
{
	return 1;
}

int cBlockHandler::GetDropID()
{
	return m_BlockID;
}

char cBlockHandler::GetDropMeta(char a_BlockMeta)
{
	return a_BlockMeta;		//This keeps most textures. The few other blocks have to override this
}

void cBlockHandler::DropBlock(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	cItems Drops;
	char Meta = a_World->GetBlockMeta(a_X, a_Y, a_Z);
	char DropCount = GetDropCount();
	int DropItem = GetDropID();
	if(DropCount > 0 && DropItem != E_ITEM_EMPTY)
	{
		Drops.push_back(cItem((ENUM_ITEM_ID)DropItem, DropCount, GetDropMeta(Meta)));
		a_World->SpawnItemPickups(Drops, a_X, a_Y, a_Z);
	}
}

bool cBlockHandler::CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z)
{
	return true;
}

bool cBlockHandler::IsUseable()
{
	return false;
}

bool cBlockHandler::IsClickedThrough()
{
	return false;
}

bool cBlockHandler::IgnoreBuildCollision()
{
	return m_BlockID == E_BLOCK_AIR;
}

bool cBlockHandler::NeedsRandomTicks()
{
	return false;
}

bool cBlockHandler::AllowBlockOnTop()
{
	return true;
}

bool cBlockHandler::CanBePlacedOnSide()
{
	return true;
}

bool cBlockHandler::DropOnUnsuitable()
{
	return true;
}