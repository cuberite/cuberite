
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
#include "ItemFood.h"

#include "../blocks/Block.h"





bool cItemHandler::m_HandlerInitialized = false;
cItemHandler * cItemHandler::m_ItemHandler[2266];





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
		default:                       return new cItemHandler(a_ItemID);
		
		// Single item per handler:
		case E_ITEM_SHEARS:            return new cItemShearsHandler(a_ItemID);
		case E_ITEM_LEAVES:            return new cItemLeavesHandler(a_ItemID);
		case E_ITEM_SAPLING:           return new cItemSaplingHandler(a_ItemID);
		case E_ITEM_DYE:               return new cItemDyeHandler(a_ItemID);
		case E_ITEM_SUGARCANE:         return new cItemSugarcaneHandler(a_ItemID);
		case E_ITEM_FLINT_AND_STEEL:   return new cItemLighterHandler(a_ItemID);
		case E_ITEM_REDSTONE_DUST:     return new cItemRedstoneDustHandler(a_ItemID);
		case E_ITEM_REDSTONE_REPEATER: return new cItemRedstoneRepeaterHandler(a_ItemID);
		case E_ITEM_WOOL:              return new cItemClothHandler(a_ItemID);
		
		case E_ITEM_WOODEN_HOE:
		case E_ITEM_STONE_HOE:
		case E_ITEM_IRON_HOE:
		case E_ITEM_GOLD_HOE:
		case E_ITEM_DIAMOND_HOE:
		{
			return new cItemHoeHandler(a_ItemID);
		}
		
		case E_ITEM_WOODEN_PICKAXE:
		case E_ITEM_STONE_PICKAXE:
		case E_ITEM_IRON_PICKAXE:
		case E_ITEM_GOLD_PICKAXE:
		case E_ITEM_DIAMOND_PICKAXE:
		{
			return new cItemPickaxeHandler(a_ItemID);
		}
		
		case E_ITEM_WOODEN_SHOVEL:
		case E_ITEM_STONE_SHOVEL:
		case E_ITEM_IRON_SHOVEL:
		case E_ITEM_GOLD_SHOVEL:
		case E_ITEM_DIAMOND_SHOVEL:
		{
			return new cItemShovelHandler(a_ItemID);
		}
		
		case E_ITEM_WOODEN_SWORD:
		case E_ITEM_STONE_SWORD:
		case E_ITEM_IRON_SWORD:
		case E_ITEM_GOLD_SWORD:
		case E_ITEM_DIAMOND_SWORD:
		{
			return new cItemSwordHandler(a_ItemID);
		}
		
		case E_ITEM_STONE_SLAB:
		case E_ITEM_WOODEN_SLAB:
		{
			return new cItemSlabHandler(a_ItemID);
		}
		
		case E_ITEM_LOG:
		case E_ITEM_PLANKS:
		{
			return new cItemWoodHandler(a_ItemID);
		}
		
		case E_ITEM_BUCKET:
		case E_ITEM_WATER_BUCKET:
		case E_ITEM_LAVA_BUCKET:
		{
			return new cItemBucketHandler(a_ItemID);
		}
		
		case E_ITEM_PUMPKIN_SEEDS:
		case E_ITEM_MELON_SEEDS:
		case E_ITEM_SEEDS:
		{
			return new cItemSeedsHandler(a_ItemID);
		}
		
		case E_ITEM_IRON_DOOR:
		case E_ITEM_WOODEN_DOOR:
		{
			return new cItemDoorHandler(a_ItemID);
		}
		
		// Food:
		case E_ITEM_BREAD:
		case E_ITEM_COOKIE:
		case E_ITEM_MELON_SLICE:
		case E_ITEM_RAW_CHICKEN:
		case E_ITEM_COOKED_CHICKEN:
		case E_ITEM_RAW_BEEF:
		case E_ITEM_RAW_MEAT:
		case E_ITEM_STEAK:
		case E_ITEM_COOKED_MEAT:
		case E_ITEM_RAW_FISH:
		case E_ITEM_COOKED_FISH:
		case E_ITEM_RED_APPLE:
		case E_ITEM_GOLDEN_APPLE:
		case E_ITEM_ROTTEN_FLESH:
		case E_ITEM_SPIDER_EYE:
		{
			return new cItemFoodHandler(a_ItemID);
		}
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





char cItemHandler::GetMaxStackSize(void)
{
	if (m_ItemID < 256)
	{
		// All blocks can stack up to 64
		return 64;
	}
	
	// By default items don't stack:
	return 1;
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
	ASSERT(m_ItemID < 256);  // Items with IDs above 255 should all be handled by specific handlers
	
	#ifdef _DEBUG
	if (m_ItemID > 256)
	{
		LOGERROR("Item %d has no valid block!", m_ItemID);
	}
	#endif  // _DEBUG
	
	return (BLOCKTYPE) m_ItemID;
}





NIBBLETYPE cItemHandler::GetBlockMeta(short a_ItemDamage)
{
	return (NIBBLETYPE)a_ItemDamage & 0x0f;  // This keeps most textures. The few other items have to override this
}





void cItemHandler::PlaceBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir)
{
	BLOCKTYPE Block = GetBlockType();
	cBlockHandler *Handler = cBlockHandler::GetBlockHandler(Block);
	Handler->PlaceBlock(a_World, a_Player, GetBlockMeta(a_Item->m_ItemHealth), a_X, a_Y, a_Z, a_Dir);
	if(a_Player->GetGameMode() == eGameMode_Survival)
	{
		cItem Item(a_Item->m_ItemID, 1);
		a_Player->GetInventory().RemoveItem(Item);
	}
}





bool cItemHandler::EatItem(cPlayer *a_Player, cItem *a_Item)
{
	FoodInfo Info = GetFoodInfo();

	if(Info.FoodLevel > 0 || Info.Saturation > 0.f)
	{
		bool Success = a_Player->Feed(Info.FoodLevel, Info.Saturation);
		if(Success && Info.PoisionChance > 0)
		{
			MTRand r1;
			if((r1.randInt(100) - Info.PoisionChance) <= 0)
			{	//Unlucky guy :D
				//TODO: Make player ill
			}
		}

		return Success;
	}

	return false;
}





cItemHandler::FoodInfo cItemHandler::GetFoodInfo()
{
	return FoodInfo(0, 0.f);
}




