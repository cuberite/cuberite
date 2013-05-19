
#include "Globals.h"
#include "ItemHandler.h"
#include "../Item.h"
#include "../World.h"
#include "../Player.h"

// Handlers:
#include "ItemBed.h"
#include "ItemBrewingStand.h"
#include "ItemBucket.h"
#include "ItemCauldron.h"
#include "ItemCloth.h"
#include "ItemDoor.h"
#include "ItemDye.h"
#include "ItemFlowerPot.h"
#include "ItemFood.h"
#include "ItemHoe.h"
#include "ItemLeaves.h"
#include "ItemLighter.h"
#include "ItemMinecart.h"
#include "ItemPickaxe.h"
#include "ItemRedstoneDust.h"
#include "ItemRedstoneRepeater.h"
#include "ItemSapling.h"
#include "ItemSeeds.h"
#include "ItemShears.h"
#include "ItemShovel.h"
#include "ItemSign.h"
#include "ItemSlab.h"
#include "ItemSpawnEgg.h"
#include "ItemSugarcane.h"
#include "ItemSword.h"
#include "ItemWood.h"

#include "../Blocks/BlockHandler.h"





bool cItemHandler::m_HandlerInitialized = false;
cItemHandler * cItemHandler::m_ItemHandler[2268];





cItemHandler *cItemHandler::GetItemHandler(int a_ItemType)
{
	if(a_ItemType < 0) a_ItemType = 0;

	if(!m_HandlerInitialized)
	{	//We have to initialize
		memset(m_ItemHandler, 0, sizeof(m_ItemHandler));
		m_HandlerInitialized = true;
	}
	if(m_ItemHandler[a_ItemType])
		return m_ItemHandler[a_ItemType];
	m_ItemHandler[a_ItemType] = CreateItemHandler(a_ItemType);
	return m_ItemHandler[a_ItemType];
}





cItemHandler *cItemHandler::CreateItemHandler(int a_ItemType)
{
	switch(a_ItemType)
	{
		default:                       return new cItemHandler(a_ItemType);
		
		// Single item per handler, alphabetically sorted:
		case E_BLOCK_LEAVES:           return new cItemLeavesHandler(a_ItemType);
		case E_BLOCK_SAPLING:          return new cItemSaplingHandler(a_ItemType);
		case E_BLOCK_WOOL:             return new cItemClothHandler(a_ItemType);
		case E_ITEM_BED:               return new cItemBedHandler(a_ItemType);
		case E_ITEM_BREWING_STAND:     return new cItemBrewingStandHandler(a_ItemType);
		case E_ITEM_CAULDRON:          return new cItemCauldronHandler(a_ItemType);
		case E_ITEM_DYE:               return new cItemDyeHandler(a_ItemType);
		case E_ITEM_FLINT_AND_STEEL:   return new cItemLighterHandler(a_ItemType);
		case E_ITEM_FLOWER_POT:        return new cItemFlowerPotHandler(a_ItemType);
		case E_ITEM_REDSTONE_DUST:     return new cItemRedstoneDustHandler(a_ItemType);
		case E_ITEM_REDSTONE_REPEATER: return new cItemRedstoneRepeaterHandler(a_ItemType);
		case E_ITEM_SHEARS:            return new cItemShearsHandler(a_ItemType);
		case E_ITEM_SIGN:              return new cItemSignHandler(a_ItemType);
		case E_ITEM_SPAWN_EGG:         return new cItemSpawnEggHandler(a_ItemType);
		case E_ITEM_SUGARCANE:         return new cItemSugarcaneHandler(a_ItemType);
		
		case E_ITEM_WOODEN_HOE:
		case E_ITEM_STONE_HOE:
		case E_ITEM_IRON_HOE:
		case E_ITEM_GOLD_HOE:
		case E_ITEM_DIAMOND_HOE:
		{
			return new cItemHoeHandler(a_ItemType);
		}
		
		case E_ITEM_WOODEN_PICKAXE:
		case E_ITEM_STONE_PICKAXE:
		case E_ITEM_IRON_PICKAXE:
		case E_ITEM_GOLD_PICKAXE:
		case E_ITEM_DIAMOND_PICKAXE:
		{
			return new cItemPickaxeHandler(a_ItemType);
		}
		
		case E_ITEM_WOODEN_SHOVEL:
		case E_ITEM_STONE_SHOVEL:
		case E_ITEM_IRON_SHOVEL:
		case E_ITEM_GOLD_SHOVEL:
		case E_ITEM_DIAMOND_SHOVEL:
		{
			return new cItemShovelHandler(a_ItemType);
		}
		
		case E_ITEM_WOODEN_SWORD:
		case E_ITEM_STONE_SWORD:
		case E_ITEM_IRON_SWORD:
		case E_ITEM_GOLD_SWORD:
		case E_ITEM_DIAMOND_SWORD:
		{
			return new cItemSwordHandler(a_ItemType);
		}
		
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_WOODEN_SLAB:
		{
			return new cItemSlabHandler(a_ItemType);
		}
		
		case E_BLOCK_LOG:
		case E_BLOCK_PLANKS:
		{
			return new cItemWoodHandler(a_ItemType);
		}
		
		case E_ITEM_BUCKET:
		case E_ITEM_WATER_BUCKET:
		case E_ITEM_LAVA_BUCKET:
		{
			return new cItemBucketHandler(a_ItemType);
		}
		
		case E_ITEM_CARROT:
		case E_ITEM_MELON_SEEDS:
		case E_ITEM_POTATO:
		case E_ITEM_PUMPKIN_SEEDS:
		case E_ITEM_SEEDS:
		{
			return new cItemSeedsHandler(a_ItemType);
		}
		
		case E_ITEM_IRON_DOOR:
		case E_ITEM_WOODEN_DOOR:
		{
			return new cItemDoorHandler(a_ItemType);
		}
		
		case E_ITEM_MINECART:
		case E_ITEM_CHEST_MINECART:
		case E_ITEM_FURNACE_MINECART:
		{
			return new cItemMinecartHandler(a_ItemType);
		}
		
		// Food:
		case E_ITEM_BREAD:
		case E_ITEM_COOKIE:
		case E_ITEM_MELON_SLICE:
		case E_ITEM_RAW_CHICKEN:
		case E_ITEM_COOKED_CHICKEN:
		case E_ITEM_RAW_BEEF:
		case E_ITEM_RAW_PORKCHOP:
		case E_ITEM_STEAK:
		case E_ITEM_COOKED_PORKCHOP:
		case E_ITEM_RAW_FISH:
		case E_ITEM_COOKED_FISH:
		case E_ITEM_RED_APPLE:
		case E_ITEM_GOLDEN_APPLE:
		case E_ITEM_ROTTEN_FLESH:
		case E_ITEM_SPIDER_EYE:
		{
			return new cItemFoodHandler(a_ItemType);
		}
	}
}





void cItemHandler::Deinit()
{
	for(int i = 0; i < 2267; i++)
	{
		delete m_ItemHandler[i];
	}
	memset(m_ItemHandler, 0, sizeof(m_ItemHandler));  // Don't leave any dangling pointers around, just in case
	m_HandlerInitialized = false;
}





cItemHandler::cItemHandler(int a_ItemType)
{
	m_ItemType = a_ItemType;
}





bool cItemHandler::OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	return false;
}





bool cItemHandler::OnDiggingBlock(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir)
{
	return false;
}





void cItemHandler::OnBlockDestroyed(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ)
{
	BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	cBlockHandler * Handler = cBlockHandler::GetBlockHandler(Block);

	if (a_Player->GetGameMode() == gmSurvival)
	{
		if (!BlockRequiresSpecialTool(Block) || CanHarvestBlock(Block))
		{
			Handler->DropBlock(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
		}
	}
	
	a_Player->UseEquippedItem();
}





void cItemHandler::OnFoodEaten(cWorld * a_World, cPlayer * a_Player, cItem * a_Item)
{

}





char cItemHandler::GetMaxStackSize(void)
{
	if (m_ItemType < 256)
	{
		// All blocks can stack up to 64
		return 64;
	}
	
	switch (m_ItemType) //sorted by id
	{
		case E_ITEM_ARROW:                return 64;
		case E_ITEM_BLAZE_POWDER:         return 64;
		case E_ITEM_BLAZE_ROD:            return 64;
		case E_ITEM_BONE:                 return 64;
		case E_ITEM_BOOK:                 return 64;
		case E_ITEM_BOTTLE_O_ENCHANTING:  return 64;
		case E_ITEM_BOWL:                 return 64;
		case E_ITEM_BREAD:                return 64;
		case E_ITEM_BREWING_STAND:        return 64;
		case E_ITEM_BUCKET:               return 1;  // TODO: change this to 16 when turning compatibility to 1.3
		case E_ITEM_CLAY:                 return 64;
		case E_ITEM_CLAY_BRICK:           return 64;
		case E_ITEM_CLOCK:                return 64;
		case E_ITEM_COAL:                 return 64;
		case E_ITEM_COMPASS:              return 64;
		case E_ITEM_COOKED_CHICKEN:       return 64;
		case E_ITEM_COOKED_FISH:          return 64;
		case E_ITEM_COOKED_PORKCHOP:      return 64;
		case E_ITEM_COOKIE:               return 64;
		case E_ITEM_DIAMOND:              return 64;
		case E_ITEM_DYE:                  return 64;
		case E_ITEM_EGG:                  return 16;
		case E_ITEM_EMERALD:              return 64;
		case E_ITEM_ENDER_PEARL:          return 16;
		case E_ITEM_EYE_OF_ENDER:         return 64;
		case E_ITEM_FEATHER:              return 64;
		case E_ITEM_FERMENTED_SPIDER_EYE: return 64;
		case E_ITEM_FIRE_CHARGE:          return 64;
		case E_ITEM_FLINT:                return 64;
		case E_ITEM_GHAST_TEAR:           return 64;
		case E_ITEM_GLASS_BOTTLE:         return 64;
		case E_ITEM_GLISTERING_MELON:     return 64;
		case E_ITEM_GLOWSTONE_DUST:       return 64;
		case E_ITEM_GOLD:                 return 64;
		case E_ITEM_GOLDEN_APPLE:         return 64;
		case E_ITEM_GOLD_NUGGET:          return 64;
		case E_ITEM_GUNPOWDER:            return 64;
		case E_ITEM_IRON:                 return 64;
		case E_ITEM_LEATHER:              return 64;
		case E_ITEM_MAGMA_CREAM:          return 64;
		case E_ITEM_MELON_SEEDS:          return 64;
		case E_ITEM_MELON_SLICE:          return 64;
		case E_ITEM_PAINTINGS:            return 64;
		case E_ITEM_PAPER:                return 64;
		case E_ITEM_PUMPKIN_SEEDS:        return 64;
		case E_ITEM_RAW_BEEF:             return 64;
		case E_ITEM_RAW_CHICKEN:          return 64;
		case E_ITEM_RAW_FISH:             return 64;
		case E_ITEM_RAW_PORKCHOP:         return 64;
		case E_ITEM_RED_APPLE:            return 64;
		case E_ITEM_REDSTONE_DUST:        return 64;
		case E_ITEM_REDSTONE_REPEATER:    return 64;
		case E_ITEM_ROTTEN_FLESH:         return 64;
		case E_ITEM_SEEDS:                return 64;
		case E_ITEM_SIGN:                 return 16;
		case E_ITEM_SLIMEBALL:            return 64;
		case E_ITEM_SNOWBALL:             return 16;
		case E_ITEM_SPIDER_EYE:           return 64;
		case E_ITEM_STEAK:                return 64;
		case E_ITEM_STICK:                return 64;
		case E_ITEM_STRING:               return 64;
		case E_ITEM_SUGAR:                return 64;
		case E_ITEM_SUGAR_CANE:           return 64;
		case E_ITEM_WHEAT:                return 64;
	}
	// By default items don't stack:
	return 1;
}





bool cItemHandler::IsTool()
{
	// TODO: Rewrite this to list all tools specifically
	return 
		   (m_ItemType >= 256 && m_ItemType <= 259)
		|| (m_ItemType == 261)
		|| (m_ItemType >= 267 && m_ItemType <= 279)
		|| (m_ItemType >= 283 && m_ItemType <= 286)
		|| (m_ItemType >= 290 && m_ItemType <= 294)
		|| (m_ItemType >= 256 && m_ItemType <= 259)
		|| (m_ItemType == 325)
		|| (m_ItemType == 346);
}





bool cItemHandler::IsFood(void)
{
	switch (m_ItemType)
	{
		case E_ITEM_RED_APPLE:
		case E_ITEM_GOLDEN_APPLE:
		case E_ITEM_MUSHROOM_SOUP:
		case E_ITEM_BREAD:
		case E_ITEM_RAW_PORKCHOP:
		case E_ITEM_COOKED_PORKCHOP:
		case E_ITEM_MILK:
		case E_ITEM_RAW_FISH:
		case E_ITEM_COOKED_FISH:
		case E_ITEM_COOKIE:
		case E_ITEM_MELON_SLICE:
		case E_ITEM_RAW_BEEF:
		case E_ITEM_STEAK:
		case E_ITEM_RAW_CHICKEN:
		case E_ITEM_COOKED_CHICKEN:
		case E_ITEM_ROTTEN_FLESH:
		case E_ITEM_SPIDER_EYE:
		case E_ITEM_CARROT:
		case E_ITEM_POTATO:
		case E_ITEM_BAKED_POTATO:
		case E_ITEM_POISONOUS_POTATO:
		{
			return true;
		}
	}  // switch (m_ItemType)
	return false;
}





bool cItemHandler::IsPlaceable(void)
{
	// We can place any block that has a corresponding E_BLOCK_TYPE:
	return (m_ItemType >= 1) && (m_ItemType <= E_BLOCK_MAX_TYPE_ID);
}





bool cItemHandler::CanHarvestBlock(BLOCKTYPE a_BlockType)
{
	return false;
}





bool cItemHandler::GetPlacementBlockTypeMeta(
	cWorld * a_World, cPlayer * a_Player,
	int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
	int a_CursorX, int a_CursorY, int a_CursorZ,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	ASSERT(m_ItemType < 256);  // Items with IDs above 255 should all be handled by specific handlers
	
	if (m_ItemType > 256)
	{
		LOGERROR("%s: Item %d has no valid block!", __FUNCTION__, m_ItemType);
		return false;
	}
	
	cBlockHandler * BlockH = BlockHandler(m_ItemType);
	return BlockH->GetPlacementBlockTypeMeta(
		a_World, a_Player,
		a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, 
		a_CursorX, a_CursorY, a_CursorZ,
		a_BlockType, a_BlockMeta
	);
}





bool cItemHandler::EatItem(cPlayer * a_Player, cItem * a_Item)
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




