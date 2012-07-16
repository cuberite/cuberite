#include "Globals.h"
#include "Item.h"
#include "../cItem.h"
#include "../cWorld.h"
#include "../cPlayer.h"

//Handler
#include "ItemCloth.h"
#include "ItemHoe.h"
#include "ItemSlab.h"
#include "ItemWood.h"
#include "ItemShears.h"
#include "ItemLeaves.h"
#include "ItemSapling.h"
#include "ItemBucket.h"
#include "ItemLighter.h"
#include "ItemRedstoneDust.h"
#include "ItemRedstoneRepeater.h"
#include "ItemSeeds.h"
#include "ItemDye.h"
#include "ItemSugarcane.h"
#include "ItemPickaxe.h"
#include "ItemShovel.h"
#include "ItemSword.h"
#include "ItemDoor.h"

#include "../blocks/Block.h"

bool cItemHandler::m_HandlerInitialized = false;
cItemHandler *cItemHandler::m_ItemHandler[2266];

cItemHandler *cItemHandler::GetItemHandler(int a_ItemID)
{
	if(a_ItemID < 0) a_ItemID = 0;

	if(!m_HandlerInitialized)
	{	//We have to initialize
		memset(m_ItemHandler, 0, sizeof(m_ItemHandler));
		m_HandlerInitialized = true;
	}
	if(m_ItemHandler[a_ItemID])
		return m_ItemHandler[a_ItemID];
	m_ItemHandler[a_ItemID] = CreateItemHandler(a_ItemID);
	return m_ItemHandler[a_ItemID];
}

cItemHandler *cItemHandler::CreateItemHandler(int a_ItemID)
{
	switch(a_ItemID)
	{
	case E_ITEM_WOODEN_HOE:
	case E_ITEM_STONE_HOE:
	case E_ITEM_IRON_HOE:
	case E_ITEM_GOLD_HOE:
	case E_ITEM_DIAMOND_HOE:
		return new cItemHoeHandler(a_ItemID);
	case E_ITEM_WHITE_CLOTH:
		return new cItemClothHandler(a_ItemID);
	case E_ITEM_STONE_SLAB:
	case E_ITEM_WOODEN_SLAB:
		return new cItemSlabHandler(a_ItemID);
	case E_ITEM_LOG:
	case E_ITEM_WOOD:
		return new cItemWoodHandler(a_ItemID);
	case E_ITEM_SHEARS:
		return new cItemShearsHandler(a_ItemID);
	case E_ITEM_LEAVES:
		return new cItemLeavesHandler(a_ItemID);
	case E_ITEM_SAPLING:
		return new cItemSaplingHandler(a_ItemID);
	case E_ITEM_REDSTONE_DUST:
		return new cItemRedstoneDustHandler(a_ItemID);
	case E_ITEM_REDSTONE_REPEATER:
		return new cItemRedstoneRepeaterHandler(a_ItemID);
	case E_ITEM_BUCKET:
	case E_ITEM_WATER_BUCKET:
	case E_ITEM_LAVA_BUCKET:
		return new cItemBucketHandler(a_ItemID);
	case E_ITEM_FLINT_AND_STEEL:
		return new cItemLighterHandler(a_ItemID);
	case E_ITEM_PUMPKIN_SEEDS:
	case E_ITEM_MELON_SEEDS:
	case E_ITEM_SEEDS:
		return new cItemSeedsHandler(a_ItemID);
	case E_ITEM_DYE:
		return new cItemDyeHandler(a_ItemID);
	case E_ITEM_SUGARCANE:
		return new cItemSugarcaneHandler(a_ItemID);
	case E_ITEM_WOODEN_PICKAXE:
	case E_ITEM_STONE_PICKAXE:
	case E_ITEM_IRON_PICKAXE:
	case E_ITEM_GOLD_PICKAXE:
	case E_ITEM_DIAMOND_PICKAXE:
		return new cItemPickaxeHandler(a_ItemID);
	case E_ITEM_WOODEN_SHOVEL:
	case E_ITEM_STONE_SHOVEL:
	case E_ITEM_IRON_SHOVEL:
	case E_ITEM_GOLD_SHOVEL:
	case E_ITEM_DIAMOND_SHOVEL:
		return new cItemShovelHandler(a_ItemID);
	case E_ITEM_WOODEN_SWORD:
	case E_ITEM_STONE_SWORD:
	case E_ITEM_IRON_SWORD:
	case E_ITEM_GOLD_SWORD:
	case E_ITEM_DIAMOND_SWORD:
		return new cItemSwordHandler(a_ItemID);

	case E_ITEM_IRON_DOOR:
	case E_ITEM_WOODEN_DOOR:
		return new cItemDoorHandler(a_ItemID);

	default:		
		return new cItemHandler(a_ItemID);
		break;
	}
}

void cItemHandler::Deinit()
{
	for(int i = 0; i < 2266; i++)
	{
		delete m_ItemHandler[i];
	}
}

cItemHandler::cItemHandler(int a_ItemID)
{
	m_ItemID = a_ItemID;
}

bool cItemHandler::OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir)
{
	return false;
}

bool cItemHandler::OnDiggingBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir)
{
	return false;
}

void cItemHandler::OnBlockDestroyed(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z)
{
	char Block = a_World->GetBlock(a_X, a_Y, a_Z);
	cBlockHandler *Handler = cBlockHandler::GetBlockHandler(Block);

	if(a_Player->GetGameMode() == eGameMode_Survival)
	{
		if(!BlockRequiresSpecialTool(Block) || CanHarvestBlock(Block))
		{
			Handler->DropBlock(a_World, a_X, a_Y, a_Z);
		}
	}
	
	a_Player->UseEquippedItem();
}

void cItemHandler::OnFoodEaten(cWorld *a_World, cPlayer *a_Player, cItem *a_Item)
{

}

int cItemHandler::GetMaxStackSize()
{
	return 64;
}

int cItemHandler::GetMaxDamage()
{
	return 0;
}

bool cItemHandler::IsTool()
{
	return 
		   (m_ItemID >= 256 && m_ItemID <= 259)
		|| (m_ItemID == 261)
		|| (m_ItemID >= 267 && m_ItemID <= 279)
		|| (m_ItemID >= 283 && m_ItemID <= 286)
		|| (m_ItemID >= 290 && m_ItemID <= 294)
		|| (m_ItemID >= 256 && m_ItemID <= 259)
		|| (m_ItemID == 325)
		|| (m_ItemID == 346);
}

bool cItemHandler::IsFood()
{
	return 
		   (m_ItemID == 260)
		|| (m_ItemID == 282)
		|| (m_ItemID == 297)
		|| (m_ItemID >= 319 && m_ItemID <= 320)
		|| (m_ItemID == 335)
		|| (m_ItemID >= 349 && m_ItemID <= 350)
		|| (m_ItemID == 357)
		|| (m_ItemID == 360)
		|| (m_ItemID >= 363 && m_ItemID <= 366);
}

bool cItemHandler::IsPlaceable()
{
	return m_ItemID >= 1 && m_ItemID <= 136;
}


bool cItemHandler::CanHarvestBlock(BLOCKTYPE a_BlockID)
{
	return false;
}

BLOCKTYPE cItemHandler::GetBlockType()
{
	return m_ItemID;
}

char cItemHandler::GetBlockMeta(char a_ItemMeta)
{
	return a_ItemMeta; //This keeps most textures. The few other items have to override this
}

void cItemHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir)
{
	BLOCKTYPE Block = GetBlockType();
	cBlockHandler *Handler = cBlockHandler::GetBlockHandler(Block);
	Handler->PlaceBlock(a_World, a_Player, GetBlockMeta(a_Item->m_ItemHealth), a_X, a_Y, a_Z, a_Dir);
	if(a_Player->GetGameMode() == eGameMode_Survival)
		a_Player->GetInventory().RemoveItem(cItem(a_Item->m_ItemID, 1));
}