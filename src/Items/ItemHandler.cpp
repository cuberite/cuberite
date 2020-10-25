
#include "Globals.h"
#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../FastRandom.h"
#include "../BlockInServerPluginInterface.h"
#include "../Chunk.h"

// Handlers:
#include "ItemArmor.h"
#include "ItemAxe.h"
#include "ItemBed.h"
#include "ItemBigFlower.h"
#include "ItemBoat.h"
#include "ItemBottle.h"
#include "ItemBow.h"
#include "ItemBucket.h"
#include "ItemChest.h"
#include "ItemCloth.h"
#include "ItemComparator.h"
#include "ItemCookedFish.h"
#include "ItemDoor.h"
#include "ItemDye.h"
#include "ItemEmptyMap.h"
#include "ItemEnchantingTable.h"
#include "ItemEyeOfEnder.h"
#include "ItemFishingRod.h"
#include "ItemFood.h"
#include "ItemFoodSeeds.h"
#include "ItemGoldenApple.h"
#include "ItemHoe.h"
#include "ItemItemFrame.h"
#include "ItemLeaves.h"
#include "ItemLighter.h"
#include "ItemLilypad.h"
#include "ItemMap.h"
#include "ItemMilk.h"
#include "ItemMinecart.h"
#include "ItemMobHead.h"
#include "ItemNetherWart.h"
#include "ItemPainting.h"
#include "ItemPickaxe.h"
#include "ItemPoisonousPotato.h"
#include "ItemPotion.h"
#include "ItemPumpkin.h"
#include "ItemRawChicken.h"
#include "ItemRawFish.h"
#include "ItemRedstoneDust.h"
#include "ItemRedstoneRepeater.h"
#include "ItemRottenFlesh.h"
#include "ItemSapling.h"
#include "ItemSeeds.h"
#include "ItemShears.h"
#include "ItemShovel.h"
#include "ItemSign.h"
#include "ItemSlab.h"
#include "ItemSoup.h"
#include "ItemSpawnEgg.h"
#include "ItemSpiderEye.h"
#include "ItemSword.h"
#include "ItemThrowable.h"

#include "../Blocks/BlockHandler.h"
#include "SimplePlaceableItemHandler.h"





bool cItemHandler::m_HandlerInitialized = false;
cItemHandler * cItemHandler::m_ItemHandler[2268];





cItemHandler * cItemHandler::GetItemHandler(int a_ItemType)
{
	if ((a_ItemType < 0) || (static_cast<size_t>(a_ItemType) >= ARRAYCOUNT(m_ItemHandler)))
	{
		// Either nothing (-1), or bad value, both cases should return the air handler
		if (a_ItemType < -1)
		{
			ASSERT(!"Bad item type");
		}
		a_ItemType = 0;
	}

	if (!m_HandlerInitialized)
	{
		// We need to initialize
		memset(m_ItemHandler, 0, sizeof(m_ItemHandler));
		m_HandlerInitialized = true;
	}
	if (m_ItemHandler[a_ItemType] == nullptr)
	{
		m_ItemHandler[a_ItemType] = CreateItemHandler(a_ItemType);
	}
	return m_ItemHandler[a_ItemType];
}





cItemHandler * cItemHandler::CreateItemHandler(int a_ItemType)
{
	switch (a_ItemType)
	{
		default:                       return new cItemHandler(a_ItemType);

		// Single item per handler, alphabetically sorted:
		case E_BLOCK_BIG_FLOWER:         return new cItemBigFlowerHandler;
		case E_BLOCK_CHEST:              return new cItemChestHandler(a_ItemType);
		case E_BLOCK_ENCHANTMENT_TABLE:  return new cItemEnchantingTableHandler(a_ItemType);
		case E_BLOCK_LEAVES:             return new cItemLeavesHandler(a_ItemType);
		case E_BLOCK_LILY_PAD:           return new cItemLilypadHandler(a_ItemType);
		case E_BLOCK_HEAD:               return new cItemMobHeadHandler(a_ItemType);
		case E_BLOCK_NEW_LEAVES:         return new cItemLeavesHandler(a_ItemType);
		case E_BLOCK_PUMPKIN:            return new cItemPumpkinHandler;
		case E_BLOCK_PURPUR_SLAB:        return new cItemSlabHandler(E_BLOCK_PURPUR_SLAB,  E_BLOCK_PURPUR_DOUBLE_SLAB);
		case E_BLOCK_RED_SANDSTONE_SLAB: return new cItemSlabHandler(E_BLOCK_RED_SANDSTONE_SLAB,  E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB);
		case E_BLOCK_SAPLING:            return new cItemSaplingHandler(a_ItemType);
		case E_BLOCK_STONE_SLAB:         return new cItemSlabHandler(E_BLOCK_STONE_SLAB,  E_BLOCK_DOUBLE_STONE_SLAB);
		case E_BLOCK_TRAPPED_CHEST:      return new cItemChestHandler(a_ItemType);
		case E_BLOCK_WOODEN_SLAB:        return new cItemSlabHandler(E_BLOCK_WOODEN_SLAB, E_BLOCK_DOUBLE_WOODEN_SLAB);
		case E_BLOCK_WOOL:               return new cItemClothHandler(a_ItemType);
		case E_ITEM_BED:                 return new cItemBedHandler(a_ItemType);
		case E_ITEM_BOTTLE_O_ENCHANTING: return new cItemBottleOEnchantingHandler();
		case E_ITEM_BOW:                 return new cItemBowHandler();
		case E_ITEM_BREWING_STAND:       return new cSimplePlaceableItemHandler(a_ItemType, E_BLOCK_BREWING_STAND);
		case E_ITEM_CAKE:                return new cSimplePlaceableItemHandler(a_ItemType, E_BLOCK_CAKE);
		case E_ITEM_CAULDRON:            return new cSimplePlaceableItemHandler(a_ItemType, E_BLOCK_CAULDRON);
		case E_ITEM_COMPARATOR:          return new cItemComparatorHandler(a_ItemType);
		case E_ITEM_DYE:                 return new cItemDyeHandler(a_ItemType);
		case E_ITEM_EGG:                 return new cItemEggHandler();
		case E_ITEM_EMPTY_MAP:           return new cItemEmptyMapHandler();
		case E_ITEM_ENDER_PEARL:         return new cItemEnderPearlHandler();
		case E_ITEM_EYE_OF_ENDER:        return new cItemEyeOfEnderHandler();
		case E_ITEM_FIRE_CHARGE:         return new cItemLighterHandler(a_ItemType);
		case E_ITEM_FIREWORK_ROCKET:     return new cItemFireworkHandler();
		case E_ITEM_FISHING_ROD:         return new cItemFishingRodHandler(a_ItemType);
		case E_ITEM_FLINT_AND_STEEL:     return new cItemLighterHandler(a_ItemType);
		case E_ITEM_FLOWER_POT:          return new cSimplePlaceableItemHandler(a_ItemType, E_BLOCK_FLOWER_POT);
		case E_ITEM_GLASS_BOTTLE:        return new cItemBottleHandler();
		case E_ITEM_MAP:                 return new cItemMapHandler();
		case E_ITEM_MILK:                return new cItemMilkHandler();
		case E_ITEM_ITEM_FRAME:          return new cItemItemFrameHandler(a_ItemType);
		case E_ITEM_NETHER_WART:         return new cItemNetherWartHandler(a_ItemType);
		case E_ITEM_PAINTING:            return new cItemPaintingHandler(a_ItemType);
		case E_ITEM_POTIONS:             return new cItemPotionHandler();
		case E_ITEM_REDSTONE_DUST:       return new cItemRedstoneDustHandler(a_ItemType);
		case E_ITEM_REDSTONE_REPEATER:   return new cItemRedstoneRepeaterHandler(a_ItemType);
		case E_ITEM_SHEARS:              return new cItemShearsHandler(a_ItemType);
		case E_ITEM_SIGN:                return new cItemSignHandler(a_ItemType);
		case E_ITEM_HEAD:                return new cItemMobHeadHandler(a_ItemType);
		case E_ITEM_SNOWBALL:            return new cItemSnowballHandler();
		case E_ITEM_SPAWN_EGG:           return new cItemSpawnEggHandler(a_ItemType);
		case E_ITEM_STRING:              return new cSimplePlaceableItemHandler(a_ItemType, E_BLOCK_TRIPWIRE);
		case E_ITEM_SUGARCANE:           return new cSimplePlaceableItemHandler(a_ItemType, E_BLOCK_SUGARCANE);

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

		case E_ITEM_WOODEN_AXE:
		case E_ITEM_STONE_AXE:
		case E_ITEM_IRON_AXE:
		case E_ITEM_GOLD_AXE:
		case E_ITEM_DIAMOND_AXE:
		{
			return new cItemAxeHandler(a_ItemType);
		}

		case E_ITEM_WOODEN_SWORD:
		case E_ITEM_STONE_SWORD:
		case E_ITEM_IRON_SWORD:
		case E_ITEM_GOLD_SWORD:
		case E_ITEM_DIAMOND_SWORD:
		{
			return new cItemSwordHandler(a_ItemType);
		}

		case E_ITEM_BUCKET:
		case E_ITEM_WATER_BUCKET:
		case E_ITEM_LAVA_BUCKET:
		{
			return new cItemBucketHandler(a_ItemType);
		}

		case E_ITEM_BEETROOT_SEEDS:
		case E_ITEM_MELON_SEEDS:
		case E_ITEM_PUMPKIN_SEEDS:
		case E_ITEM_SEEDS:
		{
			return new cItemSeedsHandler(a_ItemType);
		}

		case E_ITEM_POTATO: return new cItemFoodSeedsHandler(a_ItemType, FoodInfo(1, 0.6));
		case E_ITEM_CARROT: return new cItemFoodSeedsHandler(a_ItemType, FoodInfo(3, 6.6));

		case E_ITEM_ACACIA_DOOR:
		case E_ITEM_BIRCH_DOOR:
		case E_ITEM_DARK_OAK_DOOR:
		case E_ITEM_JUNGLE_DOOR:
		case E_ITEM_SPRUCE_DOOR:
		case E_ITEM_IRON_DOOR:
		case E_ITEM_WOODEN_DOOR:
		{
			return new cItemDoorHandler(a_ItemType);
		}

		case E_ITEM_MINECART:
		case E_ITEM_CHEST_MINECART:
		case E_ITEM_FURNACE_MINECART:
		case E_ITEM_MINECART_WITH_TNT:
		case E_ITEM_MINECART_WITH_HOPPER:
		{
			return new cItemMinecartHandler(a_ItemType);
		}

		// Food (please keep alpha-sorted):
		case E_ITEM_BAKED_POTATO:     return new cItemFoodHandler(a_ItemType, FoodInfo(5, 6));
		case E_ITEM_BEETROOT:         return new cItemFoodHandler(a_ItemType, FoodInfo(1, 1.2));
		case E_ITEM_BREAD:            return new cItemFoodHandler(a_ItemType, FoodInfo(5, 6));
		case E_ITEM_CHORUS_FRUIT:     return new cItemFoodHandler(a_ItemType, FoodInfo(4, 2.4));
		case E_ITEM_COOKED_CHICKEN:   return new cItemFoodHandler(a_ItemType, FoodInfo(6, 7.2));
		case E_ITEM_COOKED_MUTTON:    return new cItemFoodHandler(a_ItemType, FoodInfo(6, 9.6));
		case E_ITEM_COOKED_PORKCHOP:  return new cItemFoodHandler(a_ItemType, FoodInfo(8, 12.8));
		case E_ITEM_COOKED_RABBIT:    return new cItemFoodHandler(a_ItemType, FoodInfo(5, 6));
		case E_ITEM_COOKIE:           return new cItemFoodHandler(a_ItemType, FoodInfo(2, 0.5));
		case E_ITEM_GOLDEN_CARROT:    return new cItemFoodHandler(a_ItemType, FoodInfo(6, 14.4));
		case E_ITEM_MELON_SLICE:      return new cItemFoodHandler(a_ItemType, FoodInfo(2, 1.2));
		case E_ITEM_PUMPKIN_PIE:      return new cItemFoodHandler(a_ItemType, FoodInfo(8, 4.8));
		case E_ITEM_RED_APPLE:        return new cItemFoodHandler(a_ItemType, FoodInfo(4, 2.4));
		case E_ITEM_RAW_BEEF:         return new cItemFoodHandler(a_ItemType, FoodInfo(3, 1.8));
		case E_ITEM_RAW_MUTTON:       return new cItemFoodHandler(a_ItemType, FoodInfo(2, 1.2));
		case E_ITEM_RAW_PORKCHOP:     return new cItemFoodHandler(a_ItemType, FoodInfo(3, 1.8));
		case E_ITEM_RAW_RABBIT:       return new cItemFoodHandler(a_ItemType, FoodInfo(3, 1.8));
		case E_ITEM_STEAK:            return new cItemFoodHandler(a_ItemType, FoodInfo(8, 12.8));

		// Special-case food with their own handler
		case E_ITEM_COOKED_FISH: return new cItemCookedFishHandler();
		case E_ITEM_GOLDEN_APPLE:        return new cItemGoldenAppleHandler();
		case E_ITEM_POISONOUS_POTATO: return new cItemPoisonousPotatoHandler();
		case E_ITEM_RAW_CHICKEN: return new cItemRawChickenHandler();
		case E_ITEM_RAW_FISH: return new cItemRawFishHandler();
		case E_ITEM_ROTTEN_FLESH: return new cItemRottenFleshHandler();
		case E_ITEM_SPIDER_EYE: return new cItemSpiderEyeHandler();

		// Soup:
		case E_ITEM_BEETROOT_SOUP: return new cItemSoupHandler(a_ItemType, FoodInfo(6, 7.2));
		case E_ITEM_MUSHROOM_SOUP: return new cItemSoupHandler(a_ItemType, FoodInfo(6, 7.2));
		case E_ITEM_RABBIT_STEW: return new cItemSoupHandler(a_ItemType, FoodInfo(10, 12));

		// Armor:
		case E_ITEM_LEATHER_CAP:
		case E_ITEM_GOLD_HELMET:
		case E_ITEM_CHAIN_HELMET:
		case E_ITEM_IRON_HELMET:
		case E_ITEM_DIAMOND_HELMET:
		case E_ITEM_LEATHER_TUNIC:
		case E_ITEM_GOLD_CHESTPLATE:
		case E_ITEM_CHAIN_CHESTPLATE:
		case E_ITEM_IRON_CHESTPLATE:
		case E_ITEM_DIAMOND_CHESTPLATE:
		case E_ITEM_LEATHER_PANTS:
		case E_ITEM_GOLD_LEGGINGS:
		case E_ITEM_CHAIN_LEGGINGS:
		case E_ITEM_IRON_LEGGINGS:
		case E_ITEM_DIAMOND_LEGGINGS:
		case E_ITEM_LEATHER_BOOTS:
		case E_ITEM_GOLD_BOOTS:
		case E_ITEM_CHAIN_BOOTS:
		case E_ITEM_IRON_BOOTS:
		case E_ITEM_DIAMOND_BOOTS:
		{
			return new cItemArmorHandler(a_ItemType);
		}

		case E_ITEM_ACACIA_BOAT:
		case E_ITEM_BIRCH_BOAT:
		case E_ITEM_BOAT:
		case E_ITEM_DARK_OAK_BOAT:
		case E_ITEM_JUNGLE_BOAT:
		case E_ITEM_SPRUCE_BOAT:
		{
			return new cItemBoatHandler(a_ItemType);
		}
	}
}





void cItemHandler::Deinit()
{
	for (size_t i = 0; i < ARRAYCOUNT(m_ItemHandler); i++)
	{
		delete m_ItemHandler[i];
		m_ItemHandler[i] = nullptr;
	}
	memset(m_ItemHandler, 0, sizeof(m_ItemHandler));  // Don't leave any dangling pointers around, just in case
	m_HandlerInitialized = false;
}





cItemHandler::cItemHandler(int a_ItemType)
{
	m_ItemType = a_ItemType;
}





bool cItemHandler::OnPlayerPlace(
	cWorld & a_World,
	cPlayer & a_Player,
	const cItem & a_EquippedItem,
	const Vector3i a_ClickedBlockPos,
	eBlockFace a_ClickedBlockFace,
	const Vector3i a_CursorPos
)
{
	if (a_ClickedBlockFace == BLOCK_FACE_NONE)
	{
		// Clicked in the air, no placement possible
		return false;
	}

	if (!cChunkDef::IsValidHeight(a_ClickedBlockPos.y))
	{
		// The clicked block is outside the world, ignore this call altogether (#128)
		return false;
	}

	BLOCKTYPE ClickedBlockType;
	NIBBLETYPE ClickedBlockMeta;

	a_World.GetBlockTypeMeta(a_ClickedBlockPos, ClickedBlockType, ClickedBlockMeta);
	cChunkInterface ChunkInterface(a_World.GetChunkMap());

	// Check if the block ignores build collision (water, grass etc.):
	auto PlacedBlockPos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
	if (cBlockHandler::For(ClickedBlockType).DoesIgnoreBuildCollision(ChunkInterface, a_ClickedBlockPos, a_Player, ClickedBlockMeta))
	{
		// Replace the clicked block:
		a_World.DropBlockAsPickups(a_ClickedBlockPos, &a_Player, nullptr);
		PlacedBlockPos = a_ClickedBlockPos;
	}
	else
	{
		if (!cChunkDef::IsValidHeight(PlacedBlockPos.y))
		{
			// The block is being placed outside the world, ignore this packet altogether (#128)
			return false;
		}

		NIBBLETYPE PlaceMeta;
		BLOCKTYPE PlaceBlock;
		a_World.GetBlockTypeMeta(PlacedBlockPos, PlaceBlock, PlaceMeta);

		// Clicked on side of block, make sure that placement won't be cancelled if there is a slab able to be double slabbed.
		// No need to do combinability (dblslab) checks, client will do that here.
		if (!cBlockHandler::For(PlaceBlock).DoesIgnoreBuildCollision(ChunkInterface, PlacedBlockPos, a_Player, PlaceMeta))
		{
			// Tried to place a block into another?
			// Happens when you place a block aiming at side of block with a torch on it or stem beside it
			return false;
		}
	}

	// Get all the blocks to place:
	sSetBlockVector blocks;
	if (!GetBlocksToPlace(a_World, a_Player, a_EquippedItem, PlacedBlockPos, a_ClickedBlockFace, a_CursorPos, blocks))
	{
		// Handler refused the placement, send that information back to the client:
		for (const auto & blk: blocks)
		{
			const auto & AbsPos = blk.GetAbsolutePos();
			a_World.SendBlockTo(AbsPos, a_Player);
		}
		a_World.SendBlockTo(PlacedBlockPos, a_Player);
		a_Player.GetInventory().SendEquippedSlot();
		return false;
	}

	// Try to place the blocks:
	if (!a_Player.PlaceBlocks(blocks))
	{
		// The placement failed, the blocks have already been re-sent, re-send inventory:
		a_Player.GetInventory().SendEquippedSlot();
		return false;
	}

	// Remove the "placed" item:
	if (a_Player.IsGameModeSurvival())
	{
		a_Player.GetInventory().RemoveOneEquippedItem();
	}
	return true;
}





bool cItemHandler::GetBlocksToPlace(
	cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
	const Vector3i a_PlacedBlockPos,
	eBlockFace a_ClickedBlockFace,
	const Vector3i a_CursorPos,
	sSetBlockVector & a_BlocksToSet
)
{
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	if (!GetPlacementBlockTypeMeta(&a_World, &a_Player, a_PlacedBlockPos, a_ClickedBlockFace, a_CursorPos, BlockType, BlockMeta))
	{
		return false;
	}
	a_BlocksToSet.emplace_back(a_PlacedBlockPos, BlockType, BlockMeta);
	return true;
}





bool cItemHandler::OnItemUse(
	cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
	const Vector3i a_ClickedBlockPos, eBlockFace a_ClickedBlockFace
)
{
	UNUSED(a_World);
	UNUSED(a_Player);
	UNUSED(a_PluginInterface);
	UNUSED(a_Item);
	UNUSED(a_ClickedBlockPos);
	UNUSED(a_ClickedBlockFace);

	return false;
}





bool cItemHandler::OnDiggingBlock(
	cWorld * a_World,
	cPlayer * a_Player,
	const cItem & a_HeldItem,
	const Vector3i a_ClickedBlockPos,
	eBlockFace a_ClickedBlockFace
)
{
	UNUSED(a_World);
	UNUSED(a_Player);
	UNUSED(a_HeldItem);
	UNUSED(a_ClickedBlockPos);
	UNUSED(a_ClickedBlockFace);

	return false;
}





void cItemHandler::OnEntityAttack(cPlayer * a_Attacker, cEntity * a_AttackedEntity)
{
	UNUSED(a_AttackedEntity);
	a_Attacker->UseEquippedItem(dlaAttackEntity);
}





void cItemHandler::OnFoodEaten(cWorld * a_World, cPlayer * a_Player, cItem * a_Item)
{
	UNUSED(a_World);
	UNUSED(a_Player);
	UNUSED(a_Item);
}





short cItemHandler::GetDurabilityLossByAction(eDurabilityLostAction a_Action)
{
	UNUSED(a_Action);

	return 0;
}





char cItemHandler::GetMaxStackSize(void)
{
	if (m_ItemType < 256)
	{
		// All blocks can stack up to 64
		return 64;
	}

	switch (m_ItemType)
	{
		case E_ITEM_ACACIA_DOOR:          return 64;
		case E_ITEM_ARMOR_STAND:          return 16;
		case E_ITEM_ARROW:                return 64;
		case E_ITEM_BAKED_POTATO:         return 64;
		case E_ITEM_BANNER:               return 16;
		case E_ITEM_BEETROOT:             return 64;
		case E_ITEM_BEETROOT_SEEDS:       return 64;
		case E_ITEM_BIRCH_DOOR:           return 64;
		case E_ITEM_BLAZE_POWDER:         return 64;
		case E_ITEM_BLAZE_ROD:            return 64;
		case E_ITEM_BONE:                 return 64;
		case E_ITEM_BOOK:                 return 64;
		case E_ITEM_BOTTLE_O_ENCHANTING:  return 64;
		case E_ITEM_BOWL:                 return 64;
		case E_ITEM_BREAD:                return 64;
		case E_ITEM_BREWING_STAND:        return 64;
		case E_ITEM_BUCKET:               return 16;
		case E_ITEM_CARROT:               return 64;
		case E_ITEM_CAULDRON:             return 64;
		case E_ITEM_CHORUS_FRUIT:         return 64;
		case E_ITEM_CLAY:                 return 64;
		case E_ITEM_CLAY_BRICK:           return 64;
		case E_ITEM_CLOCK:                return 64;
		case E_ITEM_COAL:                 return 64;
		case E_ITEM_COMPARATOR:           return 64;
		case E_ITEM_COMPASS:              return 64;
		case E_ITEM_COOKED_CHICKEN:       return 64;
		case E_ITEM_COOKED_FISH:          return 64;
		case E_ITEM_COOKED_MUTTON:        return 64;
		case E_ITEM_COOKED_PORKCHOP:      return 64;
		case E_ITEM_COOKED_RABBIT:        return 64;
		case E_ITEM_COOKIE:               return 64;
		case E_ITEM_DARK_OAK_DOOR:        return 64;
		case E_ITEM_DIAMOND:              return 64;
		case E_ITEM_DRAGON_BREATH:        return 64;
		case E_ITEM_DYE:                  return 64;
		case E_ITEM_EGG:                  return 16;
		case E_ITEM_EMERALD:              return 64;
		case E_ITEM_EMPTY_MAP:            return 64;
		case E_ITEM_ENDER_PEARL:          return 16;
		case E_ITEM_EYE_OF_ENDER:         return 64;
		case E_ITEM_FEATHER:              return 64;
		case E_ITEM_FERMENTED_SPIDER_EYE: return 64;
		case E_ITEM_FIRE_CHARGE:          return 64;
		case E_ITEM_FIREWORK_ROCKET:      return 64;
		case E_ITEM_FIREWORK_STAR:        return 64;
		case E_ITEM_FLINT:                return 64;
		case E_ITEM_FLOWER_POT:           return 64;
		case E_ITEM_GHAST_TEAR:           return 64;
		case E_ITEM_GLASS_BOTTLE:         return 64;
		case E_ITEM_GLISTERING_MELON:     return 64;
		case E_ITEM_GLOWSTONE_DUST:       return 64;
		case E_ITEM_GOLD:                 return 64;
		case E_ITEM_GOLDEN_APPLE:         return 64;
		case E_ITEM_GOLDEN_CARROT:        return 64;
		case E_ITEM_GOLD_NUGGET:          return 64;
		case E_ITEM_GUNPOWDER:            return 64;
		case E_ITEM_HEAD:                 return 64;
		case E_ITEM_JUNGLE_DOOR:          return 64;
		case E_ITEM_IRON:                 return 64;
		case E_ITEM_IRON_DOOR:            return 64;
		case E_ITEM_IRON_NUGGET:          return 64;
		case E_ITEM_ITEM_FRAME:           return 64;
		case E_ITEM_LEAD:                 return 64;
		case E_ITEM_LEATHER:              return 64;
		case E_ITEM_MAGMA_CREAM:          return 64;
		case E_ITEM_MAP:                  return 64;
		case E_ITEM_MELON_SEEDS:          return 64;
		case E_ITEM_MELON_SLICE:          return 64;
		case E_ITEM_NETHER_BRICK:         return 64;
		case E_ITEM_NETHER_QUARTZ:        return 64;
		case E_ITEM_NETHER_WART:          return 64;
		case E_ITEM_PAINTING:             return 64;
		case E_ITEM_PAPER:                return 64;
		case E_ITEM_POISONOUS_POTATO:     return 64;
		case E_ITEM_POPPED_CHORUS_FRUIT:  return 64;
		case E_ITEM_POTATO:               return 64;
		case E_ITEM_PRISMARINE_CRYSTALS:  return 64;
		case E_ITEM_PRISMARINE_SHARD:     return 64;
		case E_ITEM_PUMPKIN_PIE:          return 64;
		case E_ITEM_PUMPKIN_SEEDS:        return 64;
		case E_ITEM_RABBITS_FOOT:         return 64;
		case E_ITEM_RABBIT_HIDE:          return 64;
		case E_ITEM_RAW_BEEF:             return 64;
		case E_ITEM_RAW_CHICKEN:          return 64;
		case E_ITEM_RAW_FISH:             return 64;
		case E_ITEM_RAW_MUTTON:           return 64;
		case E_ITEM_RAW_PORKCHOP:         return 64;
		case E_ITEM_RAW_RABBIT:           return 64;
		case E_ITEM_RED_APPLE:            return 64;
		case E_ITEM_REDSTONE_DUST:        return 64;
		case E_ITEM_REDSTONE_REPEATER:    return 64;
		case E_ITEM_ROTTEN_FLESH:         return 64;
		case E_ITEM_SEEDS:                return 64;
		case E_ITEM_SIGN:                 return 16;
		case E_ITEM_SLIMEBALL:            return 64;
		case E_ITEM_SNOWBALL:             return 16;
		case E_ITEM_SPAWN_EGG:            return 64;
		case E_ITEM_SPECTRAL_ARROW:       return 64;
		case E_ITEM_SPIDER_EYE:           return 64;
		case E_ITEM_SPRUCE_DOOR:          return 64;
		case E_ITEM_STEAK:                return 64;
		case E_ITEM_STICK:                return 64;
		case E_ITEM_STRING:               return 64;
		case E_ITEM_SUGAR:                return 64;
		case E_ITEM_SUGAR_CANE:           return 64;
		case E_ITEM_TIPPED_ARROW:         return 64;
		case E_ITEM_WHEAT:                return 64;
		case E_ITEM_WOODEN_DOOR:          return 64;
		// By default items don't stack:
		default:                          return 1;
	}
}





bool cItemHandler::IsTool()
{
	// TODO: Rewrite this to list all tools specifically
	return
		((m_ItemType >= 256) && (m_ItemType <= 259)) ||
		(m_ItemType == 261) ||
		((m_ItemType >= 267) && (m_ItemType <= 279)) ||
		((m_ItemType >= 283) && (m_ItemType <= 286)) ||
		((m_ItemType >= 290) && (m_ItemType <= 294)) ||
		(m_ItemType == 325) ||
		(m_ItemType == 346);
}





bool cItemHandler::IsFood(void)
{
	return false;
}





bool cItemHandler::IsDrinkable(short a_ItemDamage)
{
	UNUSED(a_ItemDamage);

	return false;
}





bool cItemHandler::IsPlaceable(void)
{
	// We can place any block that has a corresponding E_BLOCK_TYPE:
	return (m_ItemType >= 1) && (m_ItemType <= E_BLOCK_MAX_TYPE_ID);
}





bool cItemHandler::CanRepairWithRawMaterial(short a_ItemType)
{
	UNUSED(a_ItemType);
	return false;
}





bool cItemHandler::CanHarvestBlock(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ANVIL:
		case E_BLOCK_BLACK_GLAZED_TERRACOTTA:
		case E_BLOCK_BLOCK_OF_COAL:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_BLUE_GLAZED_TERRACOTTA:
		case E_BLOCK_BONE_BLOCK:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_BRICK:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_BROWN_GLAZED_TERRACOTTA:
		case E_BLOCK_CAULDRON:
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_COBWEB:
		case E_BLOCK_CONCRETE:
		case E_BLOCK_CYAN_GLAZED_TERRACOTTA:
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_DROPPER:
		case E_BLOCK_EMERALD_ORE:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_END_BRICKS:
		case E_BLOCK_END_STONE:
		case E_BLOCK_FURNACE:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_GRAY_GLAZED_TERRACOTTA:
		case E_BLOCK_GREEN_GLAZED_TERRACOTTA:
		case E_BLOCK_HARDENED_CLAY:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_IRON_BARS:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_LAPIS_BLOCK:
		case E_BLOCK_LAPIS_ORE:
		case E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA:
		case E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_LIME_GLAZED_TERRACOTTA:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_MAGENTA_GLAZED_TERRACOTTA:
		case E_BLOCK_MAGMA:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_NETHER_QUARTZ_ORE:
		case E_BLOCK_OBSERVER:
		case E_BLOCK_OBSIDIAN:
		case E_BLOCK_ORANGE_GLAZED_TERRACOTTA:
		case E_BLOCK_PINK_GLAZED_TERRACOTTA:
		case E_BLOCK_PRISMARINE_BLOCK:
		case E_BLOCK_PURPLE_GLAZED_TERRACOTTA:
		case E_BLOCK_PURPUR_BLOCK:
		case E_BLOCK_PURPUR_DOUBLE_SLAB:
		case E_BLOCK_PURPUR_PILLAR:
		case E_BLOCK_PURPUR_SLAB:
		case E_BLOCK_PURPUR_STAIRS:
		case E_BLOCK_QUARTZ_BLOCK:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_RED_GLAZED_TERRACOTTA:
		case E_BLOCK_RED_NETHER_BRICK:
		case E_BLOCK_RED_SANDSTONE:
		case E_BLOCK_RED_SANDSTONE_SLAB:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_SNOW:
		case E_BLOCK_STONE:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_VINES:
		case E_BLOCK_WHITE_GLAZED_TERRACOTTA:
		case E_BLOCK_YELLOW_GLAZED_TERRACOTTA:
		{
			return false;
		}
		default: return true;
	}
}





bool cItemHandler::GetPlacementBlockTypeMeta(
	cWorld * a_World, cPlayer * a_Player,
	const Vector3i a_PlacedBlockPos, eBlockFace a_ClickedBlockFace,
	const Vector3i a_CursorPos,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
)
{
	ASSERT(m_ItemType < 256);  // Items with IDs above 255 should all be handled by specific handlers

	if (m_ItemType >= 256)
	{
		LOGERROR("%s: Item %d is not eligible for direct block placement!", __FUNCTION__, m_ItemType);
		return false;
	}

	cChunkInterface ChunkInterface(a_World->GetChunkMap());
	return cBlockHandler::For(static_cast<BLOCKTYPE>(m_ItemType)).GetPlacementBlockTypeMeta(
		ChunkInterface, *a_Player,
		a_PlacedBlockPos, a_ClickedBlockFace,
		a_CursorPos,
		a_BlockType, a_BlockMeta
	);
}





bool cItemHandler::EatItem(cPlayer * a_Player, cItem * a_Item)
{
	auto FoodInfo = GetFoodInfo(a_Item);
	return a_Player->Feed(FoodInfo.FoodLevel, FoodInfo.Saturation);
}





cItemHandler::FoodInfo cItemHandler::GetFoodInfo(const cItem * a_Item)
{
	UNUSED(a_Item);
	return FoodInfo(0, 0);
}





float cItemHandler::GetBlockBreakingStrength(BLOCKTYPE a_Block)
{
	return 1.0f;
}
