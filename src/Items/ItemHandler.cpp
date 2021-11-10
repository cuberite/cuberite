
#include "Globals.h"
#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../FastRandom.h"
#include "../BlockInServerPluginInterface.h"
#include "../Chunk.h"

// Handlers:
#include "ItemAnvil.h"
#include "ItemArmor.h"
#include "ItemAxe.h"
#include "ItemBanner.h"
#include "ItemBed.h"
#include "ItemBigFlower.h"
#include "ItemBoat.h"
#include "ItemBottle.h"
#include "ItemBow.h"
#include "ItemBucket.h"
#include "ItemButton.h"
#include "ItemChest.h"
#include "ItemChorusFruit.h"
#include "ItemComparator.h"
#include "ItemCookedFish.h"
#include "ItemDoor.h"
#include "ItemDropSpenser.h"
#include "ItemDye.h"
#include "ItemEmptyMap.h"
#include "ItemEnchantingTable.h"
#include "ItemEndCrystal.h"
#include "ItemEndPortalFrame.h"
#include "ItemEnderchest.h"
#include "ItemEyeOfEnder.h"
#include "ItemFenceGate.h"
#include "ItemFishingRod.h"
#include "ItemFood.h"
#include "ItemFoodSeeds.h"
#include "ItemFurnace.h"
#include "ItemGlazedTerracotta.h"
#include "ItemGoldenApple.h"
#include "ItemHayBale.h"
#include "ItemHoe.h"
#include "ItemHopper.h"
#include "ItemItemFrame.h"
#include "ItemJackOLantern.h"
#include "ItemLadder.h"
#include "ItemLeaves.h"
#include "ItemLever.h"
#include "ItemLighter.h"
#include "ItemLilypad.h"
#include "ItemLog.h"
#include "ItemMap.h"
#include "ItemMilk.h"
#include "ItemMinecart.h"
#include "ItemMobHead.h"
#include "ItemNetherWart.h"
#include "ItemObserver.h"
#include "ItemPainting.h"
#include "ItemPickaxe.h"
#include "ItemPiston.h"
#include "ItemPoisonousPotato.h"
#include "ItemPotion.h"
#include "ItemPumpkin.h"
#include "ItemQuartz.h"
#include "ItemRail.h"
#include "ItemRawChicken.h"
#include "ItemRawFish.h"
#include "ItemRedstoneDust.h"
#include "ItemRepeater.h"
#include "ItemRottenFlesh.h"
#include "ItemSeeds.h"
#include "ItemShears.h"
#include "ItemShovel.h"
#include "ItemSign.h"
#include "ItemSlab.h"
#include "ItemSnow.h"
#include "ItemSoup.h"
#include "ItemSpawnEgg.h"
#include "ItemSpiderEye.h"
#include "ItemStairs.h"
#include "ItemSword.h"
#include "ItemThrowable.h"
#include "ItemTorch.h"
#include "ItemTrapdoor.h"
#include "ItemTripwireHook.h"
#include "ItemVine.h"
#include "ItemWool.h"


#include "../Blocks/BlockHandler.h"
#include "SimplePlaceableItemHandler.h"
#include "../Registries/BlockItemConverter.h"
#include "../Protocol/Palettes/Upgrade.h"




std::map<Item, cItemHandler> cItemHandler::m_ItemHandlers = std::map<Item, cItemHandler>();




cItemHandler * cItemHandler::GetItemHandler(Item a_ItemType)
{
	if (m_ItemHandlers.count(a_ItemType) == 0)
	{
		m_ItemHandlers.insert(std::make_pair(a_ItemType, CreateItemHandler(a_ItemType)));
	}
	return & m_ItemHandlers[a_ItemType];
}





cItemHandler cItemHandler::CreateItemHandler(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::AcaciaBoat:                       return cItemBoatHandler(a_ItemType);
		case Item::AcaciaButton:                     return cItemButtonHandler(a_ItemType);
		case Item::AcaciaDoor:                       return cItemDoorHandler(a_ItemType);
		case Item::AcaciaFence:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::AcaciaFenceGate:                  return cItemFenceGateHandler(a_ItemType);
		case Item::AcaciaLeaves:                     return cItemLeavesHandler(a_ItemType);
		case Item::AcaciaLog:                        return cItemLogHandler(a_ItemType);
		case Item::AcaciaPlanks:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::AcaciaPressurePlate:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::AcaciaSapling:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::AcaciaSign:                       return cItemSignHandler(a_ItemType);
		case Item::AcaciaSlab:                       return cItemSlabHandler(a_ItemType);
		case Item::AcaciaStairs:                     return cItemStairsHandler(a_ItemType);
		case Item::AcaciaTrapdoor:                   return cItemTrapdoorHandler(a_ItemType);
		case Item::AcaciaWood:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ActivatorRail:                    return cItemRailHandler(a_ItemType);
		case Item::Air:                              return cItemHandler(a_ItemType);
		case Item::Allium:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::AncientDebris:                    return cItemHandler(a_ItemType);
		case Item::Andesite:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::AndesiteSlab:                     return cItemSlabHandler(a_ItemType);
		case Item::AndesiteStairs:                   return cItemStairsHandler(a_ItemType);
		case Item::AndesiteWall:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Anvil:                            return cItemAnvilHandler(a_ItemType);
		case Item::Apple:                            return cItemFoodHandler(a_ItemType, FoodInfo(4, 2.4));
		case Item::ArmorStand:                       return cItemHandler(a_ItemType);
		case Item::Arrow:                            return cItemHandler(a_ItemType);
		case Item::AzureBluet:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BakedPotato:                      return cItemFoodHandler(a_ItemType, FoodInfo(5, 6));
		case Item::Bamboo:                           return cItemHandler(a_ItemType);
		case Item::Barrel:                           return cItemHandler(a_ItemType);
		case Item::Barrier:                          return cItemHandler(a_ItemType);
		case Item::Basalt:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BatSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::Beacon:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Bedrock:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BeeNest:                          return cItemHandler(a_ItemType);
		case Item::BeeSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::Beef:                             return cItemFoodHandler(a_ItemType, FoodInfo(3, 1.8));
		case Item::Beehive:                          return cItemHandler(a_ItemType);
		case Item::Beetroot:                         return cItemFoodHandler(a_ItemType, FoodInfo(1, 1.2));
		case Item::BeetrootSeeds:                    return cItemSeedsHandler(a_ItemType);
		case Item::BeetrootSoup:                     return cItemSoupHandler(a_ItemType, FoodInfo(6, 7.2));
		case Item::Bell:                             return cItemHandler(a_ItemType);
		case Item::BirchBoat:                        return cItemBoatHandler(a_ItemType);
		case Item::BirchButton:                      return cItemButtonHandler(a_ItemType);
		case Item::BirchDoor:                        return cItemDoorHandler(a_ItemType);
		case Item::BirchFence:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BirchFenceGate:                   return cItemFenceGateHandler(a_ItemType);
		case Item::BirchLeaves:                      return cItemLeavesHandler(a_ItemType);
		case Item::BirchLog:                         return cItemLogHandler(a_ItemType);
		case Item::BirchPlanks:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BirchPressurePlate:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BirchSapling:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BirchSign:                        return cItemSignHandler(a_ItemType);
		case Item::BirchSlab:                        return cItemSlabHandler(a_ItemType);
		case Item::BirchStairs:                      return cItemStairsHandler(a_ItemType);
		case Item::BirchTrapdoor:                    return cItemTrapdoorHandler(a_ItemType);
		case Item::BirchWood:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackBanner:                      return cItemBannerHandler(a_ItemType);
		case Item::BlackBed:                         return cItemBedHandler(a_ItemType);
		case Item::BlackCarpet:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackConcrete:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackConcretePowder:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackDye:                         return cItemDyeHandler(a_ItemType);
		case Item::BlackGlazedTerracotta:            return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::BlackShulkerBox:                  return cItemHandler(a_ItemType);
		case Item::BlackStainedGlass:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackStainedGlassPane:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackTerracotta:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackWool:                        return cItemWoolHandler(a_ItemType);
		case Item::Blackstone:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlackstoneSlab:                   return cItemSlabHandler(a_ItemType);
		case Item::BlackstoneStairs:                 return cItemStairsHandler(a_ItemType);
		case Item::BlackstoneWall:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlastFurnace:                     return cItemFurnaceHandler(a_ItemType);
		case Item::BlazePowder:                      return cItemHandler(a_ItemType);
		case Item::BlazeRod:                         return cItemHandler(a_ItemType);
		case Item::BlazeSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::BlueBanner:                       return cItemBannerHandler(a_ItemType);
		case Item::BlueBed:                          return cItemBedHandler(a_ItemType);
		case Item::BlueCarpet:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueConcrete:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueConcretePowder:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueDye:                          return cItemDyeHandler(a_ItemType);
		case Item::BlueGlazedTerracotta:             return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::BlueIce:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueOrchid:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueShulkerBox:                   return cItemHandler(a_ItemType);
		case Item::BlueStainedGlass:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueStainedGlassPane:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueTerracotta:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BlueWool:                         return cItemWoolHandler(a_ItemType);
		case Item::Bone:                             return cItemHandler(a_ItemType);
		case Item::BoneBlock:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BoneMeal:                         return cItemDyeHandler(a_ItemType);
		case Item::Book:                             return cItemHandler(a_ItemType);
		case Item::Bookshelf:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Bow:                              return cItemBowHandler();
		case Item::Bowl:                             return cItemHandler(a_ItemType);
		case Item::BrainCoral:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrainCoralBlock:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrainCoralFan:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Bread:                            return cItemFoodHandler(a_ItemType, FoodInfo(5, 6));
		case Item::BrewingStand:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Brick:                            return cItemHandler(a_ItemType);
		case Item::BrickSlab:                        return cItemSlabHandler(a_ItemType);
		case Item::BrickStairs:                      return cItemStairsHandler(a_ItemType);
		case Item::BrickWall:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Bricks:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownBanner:                      return cItemBannerHandler(a_ItemType);
		case Item::BrownBed:                         return cItemBedHandler(a_ItemType);
		case Item::BrownCarpet:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownConcrete:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownConcretePowder:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownDye:                         return cItemDyeHandler(a_ItemType);
		case Item::BrownGlazedTerracotta:            return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::BrownMushroom:                    return cItemHandler(a_ItemType);
		case Item::BrownMushroomBlock:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownShulkerBox:                  return cItemHandler(a_ItemType);
		case Item::BrownStainedGlass:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownStainedGlassPane:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownTerracotta:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BrownWool:                        return cItemWoolHandler(a_ItemType);
		case Item::BubbleCoral:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BubbleCoralBlock:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::BubbleCoralFan:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Bucket:                           return cItemBucketHandler(a_ItemType);
		case Item::Cactus:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Cake:                             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Campfire:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Carrot:                           return cItemFoodSeedsHandler(a_ItemType, FoodInfo(3, 6.6));
		case Item::CarrotOnAStick:                   return cItemHandler(a_ItemType);
		case Item::CartographyTable:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CarvedPumpkin:                    return cItemPumpkinHandler(a_ItemType);
		case Item::CatSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::Cauldron:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CaveSpiderSpawnEgg:               return cItemSpawnEggHandler(a_ItemType);
		case Item::Chain:                            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChainCommandBlock:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChainmailBoots:                   return cItemArmorHandler(a_ItemType);
		case Item::ChainmailChestplate:              return cItemArmorHandler(a_ItemType);
		case Item::ChainmailHelmet:                  return cItemArmorHandler(a_ItemType);
		case Item::ChainmailLeggings:                return cItemArmorHandler(a_ItemType);
		case Item::Charcoal:                         return cItemHandler(a_ItemType);
		case Item::Chest:                            return cItemChestHandler(a_ItemType);
		case Item::ChestMinecart:                    return cItemMinecartHandler(a_ItemType);
		case Item::Chicken:                          return cItemRawChickenHandler();
		case Item::ChickenSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::ChippedAnvil:                     return cItemAnvilHandler(a_ItemType);
		case Item::ChiseledNetherBricks:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChiseledPolishedBlackstone:       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChiseledQuartzBlock:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChiseledRedSandstone:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChiseledSandstone:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChiseledStoneBricks:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChorusFlower:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ChorusFruit:                      return cItemChorusFruitHandler();
		case Item::ChorusPlant:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Clay:                             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ClayBall:                         return cItemHandler(a_ItemType);
		case Item::Clock:                            return cItemHandler(a_ItemType);
		case Item::Coal:                             return cItemHandler(a_ItemType);
		case Item::CoalBlock:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CoalOre:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CoarseDirt:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Cobblestone:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CobblestoneSlab:                  return cItemSlabHandler(a_ItemType);
		case Item::CobblestoneStairs:                return cItemStairsHandler(a_ItemType);
		case Item::CobblestoneWall:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Cobweb:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CocoaBeans:                       return cItemHandler(a_ItemType);
		case Item::Cod:                              return cItemRawFishHandler();
		case Item::CodBucket:                        return cItemHandler(a_ItemType);
		case Item::CodSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::CommandBlock:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CommandBlockMinecart:             return cItemMinecartHandler(a_ItemType);
		case Item::Comparator:                       return cItemComparatorHandler(a_ItemType);
		case Item::Compass:                          return cItemHandler(a_ItemType);
		case Item::Composter:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Conduit:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CookedBeef:                       return cItemFoodHandler(a_ItemType, FoodInfo(8, 12.8));
		case Item::CookedChicken:                    return cItemFoodHandler(a_ItemType, FoodInfo(6, 7.2));
		case Item::CookedCod:                        return cItemCookedFishHandler();
		case Item::CookedMutton:                     return cItemFoodHandler(a_ItemType, FoodInfo(6, 9.6));
		case Item::CookedPorkchop:                   return cItemFoodHandler(a_ItemType, FoodInfo(8, 12.8));
		case Item::CookedRabbit:                     return cItemFoodHandler(a_ItemType, FoodInfo(5, 6));
		case Item::CookedSalmon:                     return cItemFoodHandler(a_ItemType, FoodInfo(6, 9.6));
		case Item::Cookie:                           return cItemFoodHandler(a_ItemType, FoodInfo(2, 0.5));
		case Item::Cornflower:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CowSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::CrackedNetherBricks:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CrackedPolishedBlackstoneBricks:  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CrackedStoneBricks:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CraftingTable:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CreeperBannerPattern:             return cItemBannerHandler(a_ItemType);
		case Item::CreeperHead:                      return cItemMobHeadHandler(a_ItemType);
		case Item::CreeperSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::CrimsonButton:                    return cItemButtonHandler(a_ItemType);
		case Item::CrimsonDoor:                      return cItemDoorHandler(a_ItemType);
		case Item::CrimsonFence:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CrimsonFenceGate:                 return cItemFenceGateHandler(a_ItemType);
		case Item::CrimsonFungus:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CrimsonHyphae:                    return cItemLogHandler(a_ItemType);
		case Item::CrimsonNylium:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CrimsonPlanks:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CrimsonPressurePlate:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CrimsonRoots:                     return cItemHandler(a_ItemType);
		case Item::CrimsonSign:                      return cItemSignHandler(a_ItemType);
		case Item::CrimsonSlab:                      return cItemSlabHandler(a_ItemType);
		case Item::CrimsonStairs:                    return cItemStairsHandler(a_ItemType);
		case Item::CrimsonStem:                      return cItemLogHandler(a_ItemType);
		case Item::CrimsonTrapdoor:                  return cItemTrapdoorHandler(a_ItemType);
		case Item::Crossbow:                         return cItemHandler(a_ItemType);
		case Item::CryingObsidian:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CutRedSandstone:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CutRedSandstoneSlab:              return cItemSlabHandler(a_ItemType);
		case Item::CutSandstone:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CutSandstoneSlab:                 return cItemSlabHandler(a_ItemType);
		case Item::CyanBanner:                       return cItemBannerHandler(a_ItemType);
		case Item::CyanBed:                          return cItemBedHandler(a_ItemType);
		case Item::CyanCarpet:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CyanConcrete:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CyanConcretePowder:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CyanDye:                          return cItemDyeHandler(a_ItemType);
		case Item::CyanGlazedTerracotta:             return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::CyanShulkerBox:                   return cItemHandler(a_ItemType);
		case Item::CyanStainedGlass:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CyanStainedGlassPane:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CyanTerracotta:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::CyanWool:                         return cItemWoolHandler(a_ItemType);
		case Item::DamagedAnvil:                     return cItemAnvilHandler(a_ItemType);
		case Item::Dandelion:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DarkOakBoat:                      return cItemBoatHandler(a_ItemType);
		case Item::DarkOakButton:                    return cItemButtonHandler(a_ItemType);
		case Item::DarkOakDoor:                      return cItemDoorHandler(a_ItemType);
		case Item::DarkOakFence:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DarkOakFenceGate:                 return cItemFenceGateHandler(a_ItemType);
		case Item::DarkOakLeaves:                    return cItemLeavesHandler(a_ItemType);
		case Item::DarkOakLog:                       return cItemLogHandler(a_ItemType);
		case Item::DarkOakPlanks:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DarkOakPressurePlate:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DarkOakSapling:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DarkOakSign:                      return cItemSignHandler(a_ItemType);
		case Item::DarkOakSlab:                      return cItemSlabHandler(a_ItemType);
		case Item::DarkOakStairs:                    return cItemStairsHandler(a_ItemType);
		case Item::DarkOakTrapdoor:                  return cItemTrapdoorHandler(a_ItemType);
		case Item::DarkOakWood:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DarkPrismarine:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DarkPrismarineSlab:               return cItemSlabHandler(a_ItemType);
		case Item::DarkPrismarineStairs:             return cItemStairsHandler(a_ItemType);
		case Item::DaylightDetector:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadBrainCoral:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadBrainCoralBlock:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadBrainCoralFan:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadBubbleCoral:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadBubbleCoralBlock:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadBubbleCoralFan:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadBush:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadFireCoral:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadFireCoralBlock:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadFireCoralFan:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadHornCoral:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadHornCoralBlock:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadHornCoralFan:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadTubeCoral:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadTubeCoralBlock:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DeadTubeCoralFan:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DebugStick:                       return cItemHandler(a_ItemType);
		case Item::DetectorRail:                     return cItemRailHandler(a_ItemType);
		case Item::Diamond:                          return cItemHandler(a_ItemType);
		case Item::DiamondAxe:                       return cItemAxeHandler(a_ItemType);
		case Item::DiamondBlock:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DiamondBoots:                     return cItemArmorHandler(a_ItemType);
		case Item::DiamondChestplate:                return cItemArmorHandler(a_ItemType);
		case Item::DiamondHelmet:                    return cItemArmorHandler(a_ItemType);
		case Item::DiamondHoe:                       return cItemHoeHandler(a_ItemType);
		case Item::DiamondHorseArmor:                return cItemHandler(a_ItemType);
		case Item::DiamondLeggings:                  return cItemArmorHandler(a_ItemType);
		case Item::DiamondOre:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DiamondPickaxe:                   return cItemPickaxeHandler(a_ItemType);
		case Item::DiamondShovel:                    return cItemShovelHandler(a_ItemType);
		case Item::DiamondSword:                     return cItemSwordHandler(a_ItemType);
		case Item::Diorite:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::DioriteSlab:                      return cItemSlabHandler(a_ItemType);
		case Item::DioriteStairs:                    return cItemStairsHandler(a_ItemType);
		case Item::DioriteWall:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Dirt:                             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Dispenser:                        return cItemDropSpenserHandler(a_ItemType);
		case Item::DolphinSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::DonkeySpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::DragonBreath:                     return cItemHandler(a_ItemType);
		case Item::DragonEgg:                        return cItemHandler(a_ItemType);
		case Item::DragonHead:                       return cItemMobHeadHandler(a_ItemType);
		case Item::DriedKelp:                        return cItemHandler(a_ItemType);
		case Item::DriedKelpBlock:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Dropper:                          return cItemDropSpenserHandler(a_ItemType);
		case Item::DrownedSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::Egg:                              return cItemEggHandler();
		case Item::ElderGuardianSpawnEgg:            return cItemSpawnEggHandler(a_ItemType);
		case Item::Elytra:                           return cItemArmorHandler(a_ItemType);
		case Item::Emerald:                          return cItemHandler(a_ItemType);
		case Item::EmeraldBlock:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::EmeraldOre:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::EnchantedBook:                    return cItemHandler(a_ItemType);
		case Item::EnchantedGoldenApple:             return cItemHandler(a_ItemType);
		case Item::EnchantingTable:                  return cItemEnchantingTableHandler(a_ItemType);
		case Item::EndCrystal:                       return cItemEndCrystalHandler(a_ItemType);
		case Item::EndPortalFrame:                   return cItemEndPortalFrameHandler(a_ItemType);
		case Item::EndRod:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::EndStone:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::EndStoneBrickSlab:                return cItemSlabHandler(a_ItemType);
		case Item::EndStoneBrickStairs:              return cItemStairsHandler(a_ItemType);
		case Item::EndStoneBrickWall:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::EndStoneBricks:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::EnderChest:                       return cItemEnderchestHandler(a_ItemType);
		case Item::EnderEye:                         return cItemEyeOfEnderHandler();
		case Item::EnderPearl:                       return cItemEnderPearlHandler();
		case Item::EndermanSpawnEgg:                 return cItemSpawnEggHandler(a_ItemType);
		case Item::EndermiteSpawnEgg:                return cItemSpawnEggHandler(a_ItemType);
		case Item::EvokerSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::ExperienceBottle:                 return cItemBottleOEnchantingHandler();
		case Item::Farmland:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Feather:                          return cItemHandler(a_ItemType);
		case Item::FermentedSpiderEye:               return cItemHandler(a_ItemType);
		case Item::Fern:                             return cItemBigFlowerHandler(a_ItemType);
		case Item::FilledMap:                        return cItemMilkHandler();
		case Item::FireCharge:                       return cItemLighterHandler(a_ItemType);
		case Item::FireCoral:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::FireCoralBlock:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::FireCoralFan:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::FireworkRocket:                   return cItemFireworkHandler();
		case Item::FireworkStar:                     return cItemFireworkHandler();
		case Item::FishingRod:                       return cItemFishingRodHandler(a_ItemType);
		case Item::FletchingTable:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Flint:                            return cItemHandler(a_ItemType);
		case Item::FlintAndSteel:                    return cItemLighterHandler(a_ItemType);
		case Item::FlowerBannerPattern:              return cItemHandler(a_ItemType);
		case Item::FlowerPot:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::FoxSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::Furnace:                          return cItemFurnaceHandler(a_ItemType);
		case Item::FurnaceMinecart:                  return cItemMinecartHandler(a_ItemType);
		case Item::GhastSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::GhastTear:                        return cItemHandler(a_ItemType);
		case Item::GildedBlackstone:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Glass:                            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GlassBottle:                      return cItemBottleHandler();
		case Item::GlassPane:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GlisteringMelonSlice:             return cItemHandler(a_ItemType);
		case Item::GlobeBannerPattern:               return cItemHandler(a_ItemType);
		case Item::Glowstone:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GlowstoneDust:                    return cItemHandler(a_ItemType);
		case Item::GoldBlock:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GoldIngot:                        return cItemHandler(a_ItemType);
		case Item::GoldNugget:                       return cItemHandler(a_ItemType);
		case Item::GoldOre:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GoldenApple:                      return cItemGoldenAppleHandler();
		case Item::GoldenAxe:                        return cItemAxeHandler(a_ItemType);
		case Item::GoldenBoots:                      return cItemArmorHandler(a_ItemType);
		case Item::GoldenCarrot:                     return cItemFoodHandler(a_ItemType, FoodInfo(6, 14.4));
		case Item::GoldenChestplate:                 return cItemArmorHandler(a_ItemType);
		case Item::GoldenHelmet:                     return cItemArmorHandler(a_ItemType);
		case Item::GoldenHoe:                        return cItemHoeHandler(a_ItemType);
		case Item::GoldenHorseArmor:                 return cItemHandler(a_ItemType);
		case Item::GoldenLeggings:                   return cItemArmorHandler(a_ItemType);
		case Item::GoldenPickaxe:                    return cItemPickaxeHandler(a_ItemType);
		case Item::GoldenShovel:                     return cItemShovelHandler(a_ItemType);
		case Item::GoldenSword:                      return cItemSwordHandler(a_ItemType);
		case Item::Granite:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GraniteSlab:                      return cItemSlabHandler(a_ItemType);
		case Item::GraniteStairs:                    return cItemStairsHandler(a_ItemType);
		case Item::GraniteWall:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Grass:                            return cItemHandler(a_ItemType);
		case Item::GrassBlock:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrassPath:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Gravel:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrayBanner:                       return cItemBannerHandler(a_ItemType);
		case Item::GrayBed:                          return cItemBedHandler(a_ItemType);
		case Item::GrayCarpet:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrayConcrete:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrayConcretePowder:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrayDye:                          return cItemDyeHandler(a_ItemType);
		case Item::GrayGlazedTerracotta:             return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::GrayShulkerBox:                   return cItemHandler(a_ItemType);
		case Item::GrayStainedGlass:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrayStainedGlassPane:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrayTerracotta:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GrayWool:                         return cItemWoolHandler(a_ItemType);
		case Item::GreenBanner:                      return cItemBannerHandler(a_ItemType);
		case Item::GreenBed:                         return cItemBedHandler(a_ItemType);
		case Item::GreenCarpet:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GreenConcrete:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GreenConcretePowder:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GreenDye:                         return cItemDyeHandler(a_ItemType);
		case Item::GreenGlazedTerracotta:            return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::GreenShulkerBox:                  return cItemHandler(a_ItemType);
		case Item::GreenStainedGlass:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GreenStainedGlassPane:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GreenTerracotta:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GreenWool:                        return cItemWoolHandler(a_ItemType);
		case Item::Grindstone:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::GuardianSpawnEgg:                 return cItemSpawnEggHandler(a_ItemType);
		case Item::Gunpowder:                        return cItemHandler(a_ItemType);
		case Item::HayBale:                          return cItemHayBaleHandler();
		case Item::HeartOfTheSea:                    return cItemHandler(a_ItemType);
		case Item::HeavyWeightedPressurePlate:       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::HoglinSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::HoneyBlock:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::HoneyBottle:                      return cItemHandler(a_ItemType);
		case Item::Honeycomb:                        return cItemHandler(a_ItemType);
		case Item::HoneycombBlock:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Hopper:                           return cItemHopperHandler(a_ItemType);
		case Item::HopperMinecart:                   return cItemMinecartHandler(a_ItemType);
		case Item::HornCoral:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::HornCoralBlock:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::HornCoralFan:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::HorseSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::HuskSpawnEgg:                     return cItemSpawnEggHandler(a_ItemType);
		case Item::Ice:                              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::InfestedChiseledStoneBricks:      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::InfestedCobblestone:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::InfestedCrackedStoneBricks:       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::InfestedMossyStoneBricks:         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::InfestedStone:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::InfestedStoneBricks:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::InkSac:                           return cItemHandler(a_ItemType);
		case Item::IronAxe:                          return cItemAxeHandler(a_ItemType);
		case Item::IronBars:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::IronBlock:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::IronBoots:                        return cItemArmorHandler(a_ItemType);
		case Item::IronChestplate:                   return cItemArmorHandler(a_ItemType);
		case Item::IronDoor:                         return cItemDoorHandler(a_ItemType);
		case Item::IronHelmet:                       return cItemArmorHandler(a_ItemType);
		case Item::IronHoe:                          return cItemHoeHandler(a_ItemType);
		case Item::IronHorseArmor:                   return cItemHandler(a_ItemType);
		case Item::IronIngot:                        return cItemHandler(a_ItemType);
		case Item::IronLeggings:                     return cItemArmorHandler(a_ItemType);
		case Item::IronNugget:                       return cItemHandler(a_ItemType);
		case Item::IronOre:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::IronPickaxe:                      return cItemPickaxeHandler(a_ItemType);
		case Item::IronShovel:                       return cItemShovelHandler(a_ItemType);
		case Item::IronSword:                        return cItemSwordHandler(a_ItemType);
		case Item::IronTrapdoor:                     return cItemTrapdoorHandler(a_ItemType);
		case Item::ItemFrame:                        return cItemItemFrameHandler(a_ItemType);
		case Item::JackOLantern:                     return cItemJackOLanternHandler(a_ItemType);
		case Item::Jigsaw:                           return cItemHandler(a_ItemType);
		case Item::Jukebox:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::JungleBoat:                       return cItemBoatHandler(a_ItemType);
		case Item::JungleButton:                     return cItemButtonHandler(a_ItemType);
		case Item::JungleDoor:                       return cItemDoorHandler(a_ItemType);
		case Item::JungleFence:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::JungleFenceGate:                  return cItemFenceGateHandler(a_ItemType);
		case Item::JungleLeaves:                     return cItemLeavesHandler(a_ItemType);
		case Item::JungleLog:                        return cItemLogHandler(a_ItemType);
		case Item::JunglePlanks:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::JunglePressurePlate:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::JungleSapling:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::JungleSign:                       return cItemSignHandler(a_ItemType);
		case Item::JungleSlab:                       return cItemSlabHandler(a_ItemType);
		case Item::JungleStairs:                     return cItemStairsHandler(a_ItemType);
		case Item::JungleTrapdoor:                   return cItemTrapdoorHandler(a_ItemType);
		case Item::JungleWood:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Kelp:                             return cItemHandler(a_ItemType);
		case Item::KnowledgeBook:                    return cItemHandler(a_ItemType);
		case Item::Ladder:                           return cItemLadderHandler(a_ItemType);
		case Item::Lantern:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LapisBlock:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LapisLazuli:                      return cItemHandler(a_ItemType);
		case Item::LapisOre:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LargeFern:                        return cItemBigFlowerHandler(a_ItemType);
		case Item::LavaBucket:                       return cItemBucketHandler(a_ItemType);
		case Item::Lead:                             return cItemHandler(a_ItemType);
		case Item::Leather:                          return cItemHandler(a_ItemType);
		case Item::LeatherBoots:                     return cItemArmorHandler(a_ItemType);
		case Item::LeatherChestplate:                return cItemArmorHandler(a_ItemType);
		case Item::LeatherHelmet:                    return cItemArmorHandler(a_ItemType);
		case Item::LeatherHorseArmor:                return cItemHandler(a_ItemType);
		case Item::LeatherLeggings:                  return cItemArmorHandler(a_ItemType);
		case Item::Lectern:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Lever:                            return cItemLeverHandler(a_ItemType);
		case Item::LightBlueBanner:                  return cItemBannerHandler(a_ItemType);
		case Item::LightBlueBed:                     return cItemBedHandler(a_ItemType);
		case Item::LightBlueCarpet:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightBlueConcrete:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightBlueConcretePowder:          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightBlueDye:                     return cItemDyeHandler(a_ItemType);
		case Item::LightBlueGlazedTerracotta:        return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::LightBlueShulkerBox:              return cItemHandler(a_ItemType);
		case Item::LightBlueStainedGlass:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightBlueStainedGlassPane:        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightBlueTerracotta:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightBlueWool:                    return cItemWoolHandler(a_ItemType);
		case Item::LightGrayBanner:                  return cItemBannerHandler(a_ItemType);
		case Item::LightGrayBed:                     return cItemBedHandler(a_ItemType);
		case Item::LightGrayCarpet:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightGrayConcrete:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightGrayConcretePowder:          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightGrayDye:                     return cItemDyeHandler(a_ItemType);
		case Item::LightGrayGlazedTerracotta:        return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::LightGrayShulkerBox:              return cItemHandler(a_ItemType);
		case Item::LightGrayStainedGlass:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightGrayStainedGlassPane:        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightGrayTerracotta:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LightGrayWool:                    return cItemWoolHandler(a_ItemType);
		case Item::LightWeightedPressurePlate:       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Lilac:                            return cItemBigFlowerHandler(a_ItemType);
		case Item::LilyOfTheValley:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LilyPad:                          return cItemLilypadHandler(a_ItemType);
		case Item::LimeBanner:                       return cItemBannerHandler(a_ItemType);
		case Item::LimeBed:                          return cItemBedHandler(a_ItemType);
		case Item::LimeCarpet:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LimeConcrete:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LimeConcretePowder:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LimeDye:                          return cItemDyeHandler(a_ItemType);
		case Item::LimeGlazedTerracotta:             return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::LimeShulkerBox:                   return cItemHandler(a_ItemType);
		case Item::LimeStainedGlass:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LimeStainedGlassPane:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LimeTerracotta:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::LimeWool:                         return cItemWoolHandler(a_ItemType);
		case Item::LingeringPotion:                  return cItemPotionHandler();
		case Item::LlamaSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::Lodestone:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Loom:                             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagentaBanner:                    return cItemBannerHandler(a_ItemType);
		case Item::MagentaBed:                       return cItemBedHandler(a_ItemType);
		case Item::MagentaCarpet:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagentaConcrete:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagentaConcretePowder:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagentaDye:                       return cItemDyeHandler(a_ItemType);
		case Item::MagentaGlazedTerracotta:          return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::MagentaShulkerBox:                return cItemHandler(a_ItemType);
		case Item::MagentaStainedGlass:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagentaStainedGlassPane:          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagentaTerracotta:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagentaWool:                      return cItemWoolHandler(a_ItemType);
		case Item::MagmaBlock:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MagmaCream:                       return cItemHandler(a_ItemType);
		case Item::MagmaCubeSpawnEgg:                return cItemSpawnEggHandler(a_ItemType);
		case Item::Map:                              return cItemEmptyMapHandler();
		case Item::Melon:                            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MelonSeeds:                       return cItemSeedsHandler(a_ItemType);
		case Item::MelonSlice:                       return cItemFoodHandler(a_ItemType, FoodInfo(2, 1.2));
		case Item::MilkBucket:                       return cItemMilkHandler();
		case Item::Minecart:                         return cItemMinecartHandler(a_ItemType);
		case Item::MojangBannerPattern:              return cItemHandler(a_ItemType);
		case Item::MooshroomSpawnEgg:                return cItemSpawnEggHandler(a_ItemType);
		case Item::MossyCobblestone:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MossyCobblestoneSlab:             return cItemSlabHandler(a_ItemType);
		case Item::MossyCobblestoneStairs:           return cItemStairsHandler(a_ItemType);
		case Item::MossyCobblestoneWall:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MossyStoneBrickSlab:              return cItemSlabHandler(a_ItemType);
		case Item::MossyStoneBrickStairs:            return cItemStairsHandler(a_ItemType);
		case Item::MossyStoneBrickWall:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MossyStoneBricks:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MuleSpawnEgg:                     return cItemSpawnEggHandler(a_ItemType);
		case Item::MushroomStem:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::MushroomStew:                     return cItemSoupHandler(a_ItemType, FoodInfo(6, 7.2));
		case Item::MusicDiscBlocks:                  return cItemHandler(a_ItemType);
		case Item::MusicDiscCat:                     return cItemHandler(a_ItemType);
		case Item::MusicDiscChirp:                   return cItemHandler(a_ItemType);
		case Item::MusicDiscFar:                     return cItemHandler(a_ItemType);
		case Item::MusicDiscMall:                    return cItemHandler(a_ItemType);
		case Item::MusicDiscMellohi:                 return cItemHandler(a_ItemType);
		case Item::MusicDiscPigstep:                 return cItemHandler(a_ItemType);
		case Item::MusicDiscStal:                    return cItemHandler(a_ItemType);
		case Item::MusicDiscStrad:                   return cItemHandler(a_ItemType);
		case Item::MusicDiscWait:                    return cItemHandler(a_ItemType);
		case Item::MusicDiscWard:                    return cItemHandler(a_ItemType);
		case Item::MusicDisc11:                      return cItemHandler(a_ItemType);
		case Item::MusicDisc13:                      return cItemHandler(a_ItemType);
		case Item::Mutton:                           return cItemFoodHandler(a_ItemType, FoodInfo(2, 1.2));
		case Item::Mycelium:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NameTag:                          return cItemHandler(a_ItemType);
		case Item::NautilusShell:                    return cItemHandler(a_ItemType);
		case Item::NetherBrick:                      return cItemHandler(a_ItemType);
		case Item::NetherBrickFence:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NetherBrickSlab:                  return cItemSlabHandler(a_ItemType);
		case Item::NetherBrickStairs:                return cItemStairsHandler(a_ItemType);
		case Item::NetherBrickWall:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NetherBricks:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NetherGoldOre:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NetherQuartzOre:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NetherSprouts:                    return cItemHandler(a_ItemType);
		case Item::NetherStar:                       return cItemHandler(a_ItemType);
		case Item::NetherWart:                       return cItemNetherWartHandler(a_ItemType);
		case Item::NetherWartBlock:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NetheriteAxe:                     return cItemAxeHandler(a_ItemType);
		case Item::NetheriteBlock:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NetheriteBoots:                   return cItemArmorHandler(a_ItemType);
		case Item::NetheriteChestplate:              return cItemArmorHandler(a_ItemType);
		case Item::NetheriteHelmet:                  return cItemArmorHandler(a_ItemType);
		case Item::NetheriteHoe:                     return cItemHoeHandler(a_ItemType);
		case Item::NetheriteIngot:                   return cItemHandler(a_ItemType);
		case Item::NetheriteLeggings:                return cItemArmorHandler(a_ItemType);
		case Item::NetheritePickaxe:                 return cItemPickaxeHandler(a_ItemType);
		case Item::NetheriteScrap:                   return cItemHandler(a_ItemType);
		case Item::NetheriteShovel:                  return cItemShovelHandler(a_ItemType);
		case Item::NetheriteSword:                   return cItemSwordHandler(a_ItemType);
		case Item::Netherrack:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::NoteBlock:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OakBoat:                          return cItemBoatHandler(a_ItemType);
		case Item::OakButton:                        return cItemButtonHandler(a_ItemType);
		case Item::OakDoor:                          return cItemDoorHandler(a_ItemType);
		case Item::OakFence:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OakFenceGate:                     return cItemFenceGateHandler(a_ItemType);
		case Item::OakLeaves:                        return cItemLeavesHandler(a_ItemType);
		case Item::OakLog:                           return cItemLogHandler(a_ItemType);
		case Item::OakPlanks:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OakPressurePlate:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OakSapling:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OakSign:                          return cItemSignHandler(a_ItemType);
		case Item::OakSlab:                          return cItemSlabHandler(a_ItemType);
		case Item::OakStairs:                        return cItemStairsHandler(a_ItemType);
		case Item::OakTrapdoor:                      return cItemTrapdoorHandler(a_ItemType);
		case Item::OakWood:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Observer:                         return cItemObserverHandler(a_ItemType);
		case Item::Obsidian:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OcelotSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::OrangeBanner:                     return cItemBannerHandler(a_ItemType);
		case Item::OrangeBed:                        return cItemBedHandler(a_ItemType);
		case Item::OrangeCarpet:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OrangeConcrete:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OrangeConcretePowder:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OrangeDye:                        return cItemDyeHandler(a_ItemType);
		case Item::OrangeGlazedTerracotta:           return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::OrangeShulkerBox:                 return cItemHandler(a_ItemType);
		case Item::OrangeStainedGlass:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OrangeStainedGlassPane:           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OrangeTerracotta:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OrangeTulip:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::OrangeWool:                       return cItemWoolHandler(a_ItemType);
		case Item::OxeyeDaisy:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PackedIce:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Painting:                         return cItemPaintingHandler(a_ItemType);
		case Item::PandaSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::Paper:                            return cItemHandler(a_ItemType);
		case Item::ParrotSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::Peony:                            return cItemBigFlowerHandler(a_ItemType);
		case Item::PetrifiedOakSlab:                 return cItemSlabHandler(a_ItemType);
		case Item::PhantomMembrane:                  return cItemHandler(a_ItemType);
		case Item::PhantomSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::PigSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::PiglinBannerPattern:              return cItemHandler(a_ItemType);
		case Item::PiglinSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::PillagerSpawnEgg:                 return cItemSpawnEggHandler(a_ItemType);
		case Item::PinkBanner:                       return cItemBannerHandler(a_ItemType);
		case Item::PinkBed:                          return cItemBedHandler(a_ItemType);
		case Item::PinkCarpet:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PinkConcrete:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PinkConcretePowder:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PinkDye:                          return cItemDyeHandler(a_ItemType);
		case Item::PinkGlazedTerracotta:             return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::PinkShulkerBox:                   return cItemHandler(a_ItemType);
		case Item::PinkStainedGlass:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PinkStainedGlassPane:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PinkTerracotta:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PinkTulip:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PinkWool:                         return cItemWoolHandler(a_ItemType);
		case Item::Piston:                           return cItemPistonHandler(a_ItemType);
		case Item::PlayerHead:                       return cItemMobHeadHandler(a_ItemType);
		case Item::Podzol:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PoisonousPotato:                  return cItemPoisonousPotatoHandler();
		case Item::PolarBearSpawnEgg:                return cItemSpawnEggHandler(a_ItemType);
		case Item::PolishedAndesite:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedAndesiteSlab:             return cItemSlabHandler(a_ItemType);
		case Item::PolishedAndesiteStairs:           return cItemStairsHandler(a_ItemType);
		case Item::PolishedBasalt:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedBlackstone:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedBlackstoneBrickSlab:      return cItemSlabHandler(a_ItemType);
		case Item::PolishedBlackstoneBrickStairs:    return cItemStairsHandler(a_ItemType);
		case Item::PolishedBlackstoneBrickWall:      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedBlackstoneBricks:         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedBlackstoneButton:         return cItemButtonHandler(a_ItemType);
		case Item::PolishedBlackstonePressurePlate:  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedBlackstoneSlab:           return cItemSlabHandler(a_ItemType);
		case Item::PolishedBlackstoneStairs:         return cItemStairsHandler(a_ItemType);
		case Item::PolishedBlackstoneWall:           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedDiorite:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedDioriteSlab:              return cItemSlabHandler(a_ItemType);
		case Item::PolishedDioriteStairs:            return cItemStairsHandler(a_ItemType);
		case Item::PolishedGranite:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PolishedGraniteSlab:              return cItemSlabHandler(a_ItemType);
		case Item::PolishedGraniteStairs:            return cItemStairsHandler(a_ItemType);
		case Item::PoppedChorusFruit:                return cItemHandler(a_ItemType);
		case Item::Poppy:                            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Porkchop:                         return cItemFoodHandler(a_ItemType, FoodInfo(3, 1.8));
		case Item::Potato:                           return cItemFoodSeedsHandler(a_ItemType, FoodInfo(1, 0.6));
		case Item::Potion:                           return cItemPotionHandler();
		case Item::PoweredRail:                      return cItemRailHandler(a_ItemType);
		case Item::Prismarine:                       return cItemHandler(a_ItemType);
		case Item::PrismarineBrickSlab:              return cItemSlabHandler(a_ItemType);
		case Item::PrismarineBrickStairs:            return cItemStairsHandler(a_ItemType);
		case Item::PrismarineBricks:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PrismarineCrystals:               return cItemHandler(a_ItemType);
		case Item::PrismarineShard:                  return cItemHandler(a_ItemType);
		case Item::PrismarineSlab:                   return cItemSlabHandler(a_ItemType);
		case Item::PrismarineStairs:                 return cItemStairsHandler(a_ItemType);
		case Item::PrismarineWall:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Pufferfish:                       return cItemHandler(a_ItemType);
		case Item::PufferfishBucket:                 return cItemHandler(a_ItemType);
		case Item::PufferfishSpawnEgg:               return cItemSpawnEggHandler(a_ItemType);
		case Item::Pumpkin:                          return cItemPumpkinHandler(a_ItemType);
		case Item::PumpkinPie:                       return cItemFoodHandler(a_ItemType, FoodInfo(8, 4.8));
		case Item::PumpkinSeeds:                     return cItemSeedsHandler(a_ItemType);
		case Item::PurpleBanner:                     return cItemBannerHandler(a_ItemType);
		case Item::PurpleBed:                        return cItemBedHandler(a_ItemType);
		case Item::PurpleCarpet:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpleConcrete:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpleConcretePowder:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpleDye:                        return cItemDyeHandler(a_ItemType);
		case Item::PurpleGlazedTerracotta:           return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::PurpleShulkerBox:                 return cItemHandler(a_ItemType);
		case Item::PurpleStainedGlass:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpleStainedGlassPane:           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpleTerracotta:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpleWool:                       return cItemWoolHandler(a_ItemType);
		case Item::PurpurBlock:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpurPillar:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::PurpurSlab:                       return cItemSlabHandler(a_ItemType);
		case Item::PurpurStairs:                     return cItemStairsHandler(a_ItemType);
		case Item::Quartz:                           return cItemQuartzHandler(a_ItemType);
		case Item::QuartzBlock:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::QuartzBricks:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::QuartzPillar:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::QuartzSlab:                       return cItemSlabHandler(a_ItemType);
		case Item::QuartzStairs:                     return cItemStairsHandler(a_ItemType);
		case Item::Rabbit:                           return cItemFoodHandler(a_ItemType, FoodInfo(3, 1.8));
		case Item::RabbitFoot:                       return cItemHandler(a_ItemType);
		case Item::RabbitHide:                       return cItemHandler(a_ItemType);
		case Item::RabbitSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::RabbitStew:                       return cItemSoupHandler(a_ItemType, FoodInfo(10, 12));
		case Item::Rail:                             return cItemRailHandler(a_ItemType);
		case Item::RavagerSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::RedBanner:                        return cItemBannerHandler(a_ItemType);
		case Item::RedBed:                           return cItemBedHandler(a_ItemType);
		case Item::RedCarpet:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedConcrete:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedConcretePowder:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedDye:                           return cItemDyeHandler(a_ItemType);
		case Item::RedGlazedTerracotta:              return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::RedMushroom:                      return cItemHandler(a_ItemType);
		case Item::RedMushroomBlock:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedNetherBrickSlab:               return cItemSlabHandler(a_ItemType);
		case Item::RedNetherBrickStairs:             return cItemStairsHandler(a_ItemType);
		case Item::RedNetherBrickWall:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedNetherBricks:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedSand:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedSandstone:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedSandstoneSlab:                 return cItemSlabHandler(a_ItemType);
		case Item::RedSandstoneStairs:               return cItemStairsHandler(a_ItemType);
		case Item::RedSandstoneWall:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedShulkerBox:                    return cItemHandler(a_ItemType);
		case Item::RedStainedGlass:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedStainedGlassPane:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedTerracotta:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedTulip:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedWool:                          return cItemWoolHandler(a_ItemType);
		case Item::Redstone:                         return cItemRedstoneDustHandler(a_ItemType);
		case Item::RedstoneBlock:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedstoneLamp:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedstoneOre:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RedstoneTorch:                    return cItemTorchHandler(a_ItemType);
		case Item::Repeater:                         return cItemRepeaterHandler(a_ItemType);
		case Item::RepeatingCommandBlock:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::RespawnAnchor:                    return cItemHandler(a_ItemType);
		case Item::RoseBush:                         return cItemBigFlowerHandler(a_ItemType);
		case Item::RottenFlesh:                      return cItemRottenFleshHandler();
		case Item::Saddle:                           return cItemHandler(a_ItemType);
		case Item::Salmon:                           return cItemFoodHandler(a_ItemType, FoodInfo(2, 0.2));
		case Item::SalmonBucket:                     return cItemHandler(a_ItemType);
		case Item::SalmonSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::Sand:                             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Sandstone:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SandstoneSlab:                    return cItemSlabHandler(a_ItemType);
		case Item::SandstoneStairs:                  return cItemStairsHandler(a_ItemType);
		case Item::SandstoneWall:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Scaffolding:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Scute:                            return cItemHandler(a_ItemType);
		case Item::SeaLantern:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SeaPickle:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Seagrass:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Shears:                           return cItemShearsHandler(a_ItemType);
		case Item::SheepSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::Shield:                           return cItemHandler(a_ItemType);
		case Item::Shroomlight:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::ShulkerBox:                       return cItemHandler(a_ItemType);
		case Item::ShulkerShell:                     return cItemHandler(a_ItemType);
		case Item::ShulkerSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::SilverfishSpawnEgg:               return cItemSpawnEggHandler(a_ItemType);
		case Item::SkeletonHorseSpawnEgg:            return cItemSpawnEggHandler(a_ItemType);
		case Item::SkeletonSkull:                    return cItemMobHeadHandler(a_ItemType);
		case Item::SkeletonSpawnEgg:                 return cItemSpawnEggHandler(a_ItemType);
		case Item::SkullBannerPattern:               return cItemHandler(a_ItemType);
		case Item::SlimeBall:                        return cItemHandler(a_ItemType);
		case Item::SlimeBlock:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SlimeSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::SmithingTable:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Smoker:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SmoothQuartz:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SmoothQuartzSlab:                 return cItemSlabHandler(a_ItemType);
		case Item::SmoothQuartzStairs:               return cItemStairsHandler(a_ItemType);
		case Item::SmoothRedSandstone:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SmoothRedSandstoneSlab:           return cItemSlabHandler(a_ItemType);
		case Item::SmoothRedSandstoneStairs:         return cItemStairsHandler(a_ItemType);
		case Item::SmoothSandstone:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SmoothSandstoneSlab:              return cItemSlabHandler(a_ItemType);
		case Item::SmoothSandstoneStairs:            return cItemStairsHandler(a_ItemType);
		case Item::SmoothStone:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SmoothStoneSlab:                  return cItemSlabHandler(a_ItemType);
		case Item::Snow:                             return cItemSnowHandler(a_ItemType);
		case Item::SnowBlock:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Snowball:                         return cItemSnowballHandler();
		case Item::SoulCampfire:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SoulLantern:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SoulSand:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SoulSoil:                         return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SoulTorch:                        return cItemTorchHandler(a_ItemType);
		case Item::Spawner:                          return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SpectralArrow:                    return cItemHandler(a_ItemType);
		case Item::SpiderEye:                        return cItemSpiderEyeHandler();
		case Item::SpiderSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::SplashPotion:                     return cItemPotionHandler();
		case Item::Sponge:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SpruceBoat:                       return cItemBoatHandler(a_ItemType);
		case Item::SpruceButton:                     return cItemButtonHandler(a_ItemType);
		case Item::SpruceDoor:                       return cItemDoorHandler(a_ItemType);
		case Item::SpruceFence:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SpruceFenceGate:                  return cItemFenceGateHandler(a_ItemType);
		case Item::SpruceLeaves:                     return cItemLeavesHandler(a_ItemType);
		case Item::SpruceLog:                        return cItemLogHandler(a_ItemType);
		case Item::SprucePlanks:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SprucePressurePlate:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SpruceSapling:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SpruceSign:                       return cItemSignHandler(a_ItemType);
		case Item::SpruceSlab:                       return cItemSlabHandler(a_ItemType);
		case Item::SpruceStairs:                     return cItemStairsHandler(a_ItemType);
		case Item::SpruceTrapdoor:                   return cItemTrapdoorHandler(a_ItemType);
		case Item::SpruceWood:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::SquidSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::Stick:                            return cItemHandler(a_ItemType);
		case Item::StickyPiston:                     return cItemPistonHandler(a_ItemType);
		case Item::Stone:                            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StoneAxe:                         return cItemAxeHandler(a_ItemType);
		case Item::StoneBrickSlab:                   return cItemSlabHandler(a_ItemType);
		case Item::StoneBrickStairs:                 return cItemStairsHandler(a_ItemType);
		case Item::StoneBrickWall:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StoneBricks:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StoneButton:                      return cItemButtonHandler(a_ItemType);
		case Item::StoneHoe:                         return cItemHoeHandler(a_ItemType);
		case Item::StonePickaxe:                     return cItemPickaxeHandler(a_ItemType);
		case Item::StonePressurePlate:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StoneShovel:                      return cItemShovelHandler(a_ItemType);
		case Item::StoneSlab:                        return cItemSlabHandler(a_ItemType);
		case Item::StoneStairs:                      return cItemStairsHandler(a_ItemType);
		case Item::StoneSword:                       return cItemSwordHandler(a_ItemType);
		case Item::Stonecutter:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StraySpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::StriderSpawnEgg:                  return cItemSpawnEggHandler(a_ItemType);
		case Item::String:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedAcaciaLog:                return cItemLogHandler(a_ItemType);
		case Item::StrippedAcaciaWood:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedBirchLog:                 return cItemLogHandler(a_ItemType);
		case Item::StrippedBirchWood:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedCrimsonHyphae:            return cItemLogHandler(a_ItemType);
		case Item::StrippedCrimsonStem:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedDarkOakLog:               return cItemLogHandler(a_ItemType);
		case Item::StrippedDarkOakWood:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedJungleLog:                return cItemLogHandler(a_ItemType);
		case Item::StrippedJungleWood:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedOakLog:                   return cItemLogHandler(a_ItemType);
		case Item::StrippedOakWood:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedSpruceLog:                return cItemLogHandler(a_ItemType);
		case Item::StrippedSpruceWood:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StrippedWarpedHyphae:             return cItemLogHandler(a_ItemType);
		case Item::StrippedWarpedStem:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StructureBlock:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::StructureVoid:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Sugar:                            return cItemHandler(a_ItemType);
		case Item::SugarCane:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Sunflower:                        return cItemBigFlowerHandler(a_ItemType);
		case Item::SuspiciousStew:                   return cItemHandler(a_ItemType);
		case Item::SweetBerries:                     return cItemHandler(a_ItemType);
		case Item::TallGrass:                        return cItemBigFlowerHandler(a_ItemType);
		case Item::Target:                           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Terracotta:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::TippedArrow:                      return cItemHandler(a_ItemType);
		case Item::TNT:                              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::TNTMinecart:                      return cItemMinecartHandler(a_ItemType);
		case Item::Torch:                            return cItemTorchHandler(a_ItemType);
		case Item::TotemOfUndying:                   return cItemHandler(a_ItemType);
		case Item::TraderLlamaSpawnEgg:              return cItemSpawnEggHandler(a_ItemType);
		case Item::TrappedChest:                     return cItemChestHandler(a_ItemType);
		case Item::Trident:                          return cItemHandler(a_ItemType);
		case Item::TripwireHook:                     return cItemTripwireHookHandler(a_ItemType);
		case Item::TropicalFish:                     return cItemHandler(a_ItemType);
		case Item::TropicalFishBucket:               return cItemHandler(a_ItemType);
		case Item::TropicalFishSpawnEgg:             return cItemSpawnEggHandler(a_ItemType);
		case Item::TubeCoral:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::TubeCoralBlock:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::TubeCoralFan:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::TurtleEgg:                        return cItemHandler(a_ItemType);
		case Item::TurtleHelmet:                     return cItemHandler(a_ItemType);
		case Item::TurtleSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::TwistingVines:                    return cItemHandler(a_ItemType);
		case Item::VexSpawnEgg:                      return cItemSpawnEggHandler(a_ItemType);
		case Item::VillagerSpawnEgg:                 return cItemSpawnEggHandler(a_ItemType);
		case Item::VindicatorSpawnEgg:               return cItemSpawnEggHandler(a_ItemType);
		case Item::Vine:                             return cItemVineHandler(a_ItemType);
		case Item::WanderingTraderSpawnEgg:          return cItemSpawnEggHandler(a_ItemType);
		case Item::WarpedButton:                     return cItemButtonHandler(a_ItemType);
		case Item::WarpedDoor:                       return cItemDoorHandler(a_ItemType);
		case Item::WarpedFence:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WarpedFenceGate:                  return cItemFenceGateHandler(a_ItemType);
		case Item::WarpedFungus:                     return cItemHandler(a_ItemType);
		case Item::WarpedFungusOnA_stick:            return cItemHandler(a_ItemType);
		case Item::WarpedHyphae:                     return cItemHandler(a_ItemType);
		case Item::WarpedNylium:                     return cItemHandler(a_ItemType);
		case Item::WarpedPlanks:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WarpedPressurePlate:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WarpedRoots:                      return cItemHandler(a_ItemType);
		case Item::WarpedSign:                       return cItemSignHandler(a_ItemType);
		case Item::WarpedSlab:                       return cItemSlabHandler(a_ItemType);
		case Item::WarpedStairs:                     return cItemStairsHandler(a_ItemType);
		case Item::WarpedStem:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WarpedTrapdoor:                   return cItemTrapdoorHandler(a_ItemType);
		case Item::WarpedWartBlock:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WaterBucket:                      return cItemBucketHandler(a_ItemType);
		case Item::WeepingVines:                     return cItemHandler(a_ItemType);
		case Item::WetSponge:                        return cSimplePlaceableItemHandler(a_ItemType);
		case Item::Wheat:                            return cItemHandler(a_ItemType);
		case Item::WheatSeeds:                       return cItemSeedsHandler(a_ItemType);
		case Item::WhiteBanner:                      return cItemBannerHandler(a_ItemType);
		case Item::WhiteBed:                         return cItemBedHandler(a_ItemType);
		case Item::WhiteCarpet:                      return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WhiteConcrete:                    return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WhiteConcretePowder:              return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WhiteDye:                         return cItemDyeHandler(a_ItemType);
		case Item::WhiteGlazedTerracotta:            return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::WhiteShulkerBox:                  return cItemHandler(a_ItemType);
		case Item::WhiteStainedGlass:                return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WhiteStainedGlassPane:            return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WhiteTerracotta:                  return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WhiteTulip:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WhiteWool:                        return cItemWoolHandler(a_ItemType);
		case Item::WitchSpawnEgg:                    return cItemSpawnEggHandler(a_ItemType);
		case Item::WitherRose:                       return cSimplePlaceableItemHandler(a_ItemType);
		case Item::WitherSkeletonSkull:              return cItemMobHeadHandler(a_ItemType);
		case Item::WitherSkeletonSpawnEgg:           return cItemSpawnEggHandler(a_ItemType);
		case Item::WolfSpawnEgg:                     return cItemSpawnEggHandler(a_ItemType);
		case Item::WoodenAxe:                        return cItemAxeHandler(a_ItemType);
		case Item::WoodenHoe:                        return cItemHoeHandler(a_ItemType);
		case Item::WoodenPickaxe:                    return cItemPickaxeHandler(a_ItemType);
		case Item::WoodenShovel:                     return cItemShovelHandler(a_ItemType);
		case Item::WoodenSword:                      return cItemSwordHandler(a_ItemType);
		case Item::WritableBook:                     return cItemHandler(a_ItemType);
		case Item::WrittenBook:                      return cItemHandler(a_ItemType);
		case Item::YellowBanner:                     return cItemBannerHandler(a_ItemType);
		case Item::YellowBed:                        return cItemBedHandler(a_ItemType);
		case Item::YellowCarpet:                     return cSimplePlaceableItemHandler(a_ItemType);
		case Item::YellowConcrete:                   return cSimplePlaceableItemHandler(a_ItemType);
		case Item::YellowConcretePowder:             return cSimplePlaceableItemHandler(a_ItemType);
		case Item::YellowDye:                        return cItemDyeHandler(a_ItemType);
		case Item::YellowGlazedTerracotta:           return cItemGlazedTerracottaHandler(a_ItemType);
		case Item::YellowShulkerBox:                 return cItemHandler(a_ItemType);
		case Item::YellowStainedGlass:               return cSimplePlaceableItemHandler(a_ItemType);
		case Item::YellowStainedGlassPane:           return cSimplePlaceableItemHandler(a_ItemType);
		case Item::YellowTerracotta:                 return cSimplePlaceableItemHandler(a_ItemType);
		case Item::YellowWool:                       return cItemWoolHandler(a_ItemType);
		case Item::ZoglinSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::ZombieHead:                       return cItemMobHeadHandler(a_ItemType);
		case Item::ZombieHorseSpawnEgg:              return cItemSpawnEggHandler(a_ItemType);
		case Item::ZombieSpawnEgg:                   return cItemSpawnEggHandler(a_ItemType);
		case Item::ZombieVillagerSpawnEgg:           return cItemSpawnEggHandler(a_ItemType);
		case Item::ZombiePigmanSpawnEgg:             return cItemSpawnEggHandler(a_ItemType);
	}
}





cItemHandler::cItemHandler(Item a_ItemType) : m_ItemType(a_ItemType)
{
}





void cItemHandler::OnPlayerPlace(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_ClickedBlockPosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition)
{
	if (a_ClickedBlockFace == BLOCK_FACE_NONE)
	{
		// Clicked in the air, no placement possible
		return;
	}

	if (!cChunkDef::IsValidHeight(a_ClickedBlockPosition.y))
	{
		// The clicked block is outside the world, ignore this call altogether (GH #128): return cItemHandler(a_ItemType);
		return;
	}

	const auto & World = *a_Player.GetWorld();
	auto ClickedBlock = World.GetBlock(a_ClickedBlockPosition);

	// Check if the block ignores build collision (water, grass etc.):
	if (cBlockHandler::For(ClickedBlock.Type()).DoesIgnoreBuildCollision(World, a_HeldItem, a_ClickedBlockPosition, ClickedBlock, a_ClickedBlockFace, true))
	{
		// Try to place the block at the clicked position:
		if (!CommitPlacement(a_Player, a_HeldItem, a_ClickedBlockPosition, a_ClickedBlockFace, a_CursorPosition))
		{
			// The placement failed, the blocks have already been re-sent, re-send inventory:
			a_Player.GetInventory().SendEquippedSlot();
			return;
		}
	}
	else
	{
		const auto PlacedPosition = AddFaceDirection(a_ClickedBlockPosition, a_ClickedBlockFace);

		if (!cChunkDef::IsValidHeight(PlacedPosition.y))
		{
			// The block is being placed outside the world, ignore this packet altogether (GH #128):
			return;
		}

		auto PlacedBlock = World.GetBlock(PlacedPosition);

		// Clicked on side of block, make sure that placement won't be cancelled if there is a slab able to be double slabbed.
		// No need to do combinability (dblslab) checks, client will do that here.
		if (!cBlockHandler::For(PlacedBlock.Type()).DoesIgnoreBuildCollision(World, a_HeldItem, PlacedPosition, PlacedBlock, a_ClickedBlockFace, false))
		{
			// Tried to place a block into another?
			// Happens when you place a block aiming at side of block with a torch on it or stem beside it
			return;
		}

		// Try to place the block:
		if (!CommitPlacement(a_Player, a_HeldItem, PlacedPosition, a_ClickedBlockFace, a_CursorPosition))
		{
			// The placement failed, the blocks have already been re-sent, re-send inventory:
			a_Player.GetInventory().SendEquippedSlot();
			return;
		}
	}

	// Remove the "placed" item:
	if (a_Player.IsGameModeSurvival())
	{
		a_Player.GetInventory().RemoveOneEquippedItem();
	}
}





bool cItemHandler::OnItemUse(
	cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
	const Vector3i a_ClickedBlockPosition, eBlockFace a_ClickedBlockFace
)
{
	UNUSED(a_World);
	UNUSED(a_Player);
	UNUSED(a_PluginInterface);
	UNUSED(a_Item);
	UNUSED(a_ClickedBlockPosition);
	UNUSED(a_ClickedBlockFace);

	return false;
}





bool cItemHandler::OnDiggingBlock(
	cWorld * a_World,
	cPlayer * a_Player,
	const cItem & a_HeldItem,
	const Vector3i a_ClickedBlockPosition,
	eBlockFace a_ClickedBlockFace
)
{
	UNUSED(a_World);
	UNUSED(a_Player);
	UNUSED(a_HeldItem);
	UNUSED(a_ClickedBlockPosition);
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
	if (IsPlaceable())
	{
		// All blocks can stack up to 64
		return 64;
	}

	switch (m_ItemType)
	{
		case Item::AcaciaDoor:              return 64;
		case Item::AcaciaSign:              return 16;
		case Item::Apple:                   return 64;
		case Item::ArmorStand:              return 16;
		case Item::Arrow:                   return 64;
		case Item::BakedPotato:             return 64;
		case Item::BatSpawnEgg:             return 64;
		case Item::Beef:                    return 64;
		case Item::Beetroot:                return 64;
		case Item::BeetrootSeeds:           return 64;
		case Item::BirchDoor:               return 64;
		case Item::BirchSign:               return 16;
		case Item::BlackBanner:             return 16;
		case Item::BlackDye:                return 64;
		case Item::BlazePowder:             return 64;
		case Item::BlazeRod:                return 64;
		case Item::BlazeSpawnEgg:           return 64;
		case Item::BlueBanner:              return 16;
		case Item::BlueDye:                 return 64;
		case Item::Bone:                    return 64;
		case Item::BoneMeal:                return 64;
		case Item::Book:                    return 64;
		case Item::Bowl:                    return 64;
		case Item::Bread:                   return 64;
		case Item::BrewingStand:            return 64;
		case Item::Brick:                   return 64;
		case Item::BrownBanner:             return 16;
		case Item::BrownDye:                return 64;
		case Item::Bucket:                  return 16;
		case Item::Carrot:                  return 64;
		case Item::Cauldron:                return 64;
		case Item::CaveSpiderSpawnEgg:      return 64;
		case Item::Chicken:                 return 64;
		case Item::ChickenSpawnEgg:         return 64;
		case Item::ChorusFruit:             return 64;
		case Item::ClayBall:                return 64;
		case Item::Clock:                   return 64;
		case Item::Coal:                    return 64;
		case Item::Cod:                     return 64;
		case Item::Comparator:              return 64;
		case Item::Compass:                 return 64;
		case Item::CookedBeef:              return 64;
		case Item::CookedChicken:           return 64;
		case Item::CookedCod:               return 64;
		case Item::CookedMutton:            return 64;
		case Item::CookedPorkchop:          return 64;
		case Item::CookedRabbit:            return 64;
		case Item::Cookie:                  return 64;
		case Item::CowSpawnEgg:             return 64;
		case Item::CreeperHead:             return 64;
		case Item::CreeperSpawnEgg:         return 64;
		case Item::CrimsonDoor:             return 64;
		case Item::CrimsonSign:             return 16;
		case Item::CyanBanner:              return 16;
		case Item::CyanDye:                 return 64;
		case Item::DarkOakDoor:             return 64;
		case Item::DarkOakSign:             return 16;
		case Item::Diamond:                 return 64;
		case Item::DragonBreath:            return 64;
		case Item::DragonHead:              return 64;
		case Item::Egg:                     return 16;
		case Item::Emerald:                 return 64;
		case Item::EnderEye:                return 64;
		case Item::EnderPearl:              return 16;
		case Item::EndermanSpawnEgg:        return 64;
		case Item::ExperienceBottle:        return 64;
		case Item::Feather:                 return 64;
		case Item::FermentedSpiderEye:      return 64;
		case Item::FilledMap:               return 64;
		case Item::FireCharge:              return 64;
		case Item::FireworkRocket:          return 64;
		case Item::FireworkStar:            return 64;
		case Item::Flint:                   return 64;
		case Item::FlowerPot:               return 64;
		case Item::GhastSpawnEgg:           return 64;
		case Item::GhastTear:               return 64;
		case Item::GlassBottle:             return 64;
		case Item::GlisteringMelonSlice:    return 64;
		case Item::GlowstoneDust:           return 64;
		case Item::GoldIngot:               return 64;
		case Item::GoldNugget:              return 64;
		case Item::GoldenApple:             return 64;
		case Item::GoldenCarrot:            return 64;
		case Item::GrayBanner:              return 16;
		case Item::GrayDye:                 return 64;
		case Item::GreenBanner:             return 16;
		case Item::GreenDye:                return 64;
		case Item::GuardianSpawnEgg:        return 64;
		case Item::Gunpowder:               return 64;
		case Item::HorseSpawnEgg:           return 64;
		case Item::IronDoor:                return 64;
		case Item::IronIngot:               return 64;
		case Item::IronNugget:              return 64;
		case Item::ItemFrame:               return 64;
		case Item::JungleDoor:              return 64;
		case Item::JungleSign:              return 16;
		case Item::Lead:                    return 64;
		case Item::Leather:                 return 64;
		case Item::LightBlueBanner:         return 16;
		case Item::LightBlueDye:            return 64;
		case Item::LightGrayBanner:         return 16;
		case Item::LightGrayDye:            return 64;
		case Item::LimeBanner:              return 16;
		case Item::LimeDye:                 return 64;
		case Item::MagentaBanner:           return 16;
		case Item::MagentaDye:              return 64;
		case Item::MagmaCream:              return 64;
		case Item::MagmaCubeSpawnEgg:       return 64;
		case Item::Map:                     return 64;
		case Item::MelonSeeds:              return 64;
		case Item::MelonSlice:              return 64;
		case Item::MooshroomSpawnEgg:       return 64;
		case Item::Mutton:                  return 64;
		case Item::NetherBrick:             return 64;
		case Item::NetherWart:              return 64;
		case Item::OakDoor:                 return 64;
		case Item::OakSign:                 return 16;
		case Item::OcelotSpawnEgg:          return 64;
		case Item::OrangeBanner:            return 16;
		case Item::OrangeDye:               return 64;
		case Item::Painting:                return 64;
		case Item::Paper:                   return 64;
		case Item::PigSpawnEgg:             return 64;
		case Item::PinkBanner:              return 16;
		case Item::PinkDye:                 return 64;
		case Item::PlayerHead:              return 64;
		case Item::PoisonousPotato:         return 64;
		case Item::PoppedChorusFruit:       return 64;
		case Item::Porkchop:                return 64;
		case Item::Potato:                  return 64;
		case Item::PrismarineCrystals:      return 64;
		case Item::PrismarineShard:         return 64;
		case Item::PumpkinPie:              return 64;
		case Item::PumpkinSeeds:            return 64;
		case Item::PurpleBanner:            return 16;
		case Item::PurpleDye:               return 64;
		case Item::Quartz:                  return 64;
		case Item::Rabbit:                  return 64;
		case Item::RabbitFoot:              return 64;
		case Item::RabbitHide:              return 64;
		case Item::RabbitSpawnEgg:          return 64;
		case Item::RedBanner:               return 16;
		case Item::RedDye:                  return 64;
		case Item::Redstone:                return 64;
		case Item::Repeater:                return 64;
		case Item::RottenFlesh:             return 64;
		case Item::SheepSpawnEgg:           return 64;
		case Item::SilverfishSpawnEgg:      return 64;
		case Item::SkeletonSkull:           return 64;
		case Item::SkeletonSpawnEgg:        return 64;
		case Item::SlimeBall:               return 64;
		case Item::SlimeSpawnEgg:           return 64;
		case Item::Snowball:                return 16;
		case Item::SpectralArrow:           return 64;
		case Item::SpiderEye:               return 64;
		case Item::SpiderSpawnEgg:          return 64;
		case Item::SpruceDoor:              return 64;
		case Item::SpruceSign:              return 16;
		case Item::SquidSpawnEgg:           return 64;
		case Item::Stick:                   return 64;
		case Item::String:                  return 64;
		case Item::Sugar:                   return 64;
		case Item::SugarCane:               return 64;
		case Item::TippedArrow:             return 64;
		case Item::VillagerSpawnEgg:        return 64;
		case Item::WarpedDoor:              return 64;
		case Item::WarpedSign:              return 16;
		case Item::Wheat:                   return 64;
		case Item::WheatSeeds:              return 64;
		case Item::WhiteBanner:             return 16;
		case Item::WhiteDye:                return 64;
		case Item::WitchSpawnEgg:           return 64;
		case Item::WitherSkeletonSkull:     return 64;
		case Item::WitherSkeletonSpawnEgg:  return 64;
		case Item::WolfSpawnEgg:            return 64;
		case Item::YellowBanner:            return 16;
		case Item::YellowDye:               return 64;
		case Item::ZombieHead:              return 64;
		case Item::ZombiePigmanSpawnEgg:    return 64;
		case Item::ZombieSpawnEgg:          return 64;
		case Item::ZombieVillagerSpawnEgg:  return 64;
		// By default items don't stack:
		default: return 1;
	}
}





bool cItemHandler::IsTool()
{
	switch (m_ItemType)
	{
		case Item::DiamondAxe:
		case Item::DiamondHoe:
		case Item::DiamondPickaxe:
		case Item::DiamondShovel:
		case Item::GoldenAxe:
		case Item::GoldenHoe:
		case Item::GoldenPickaxe:
		case Item::GoldenShovel:
		case Item::IronAxe:
		case Item::IronHoe:
		case Item::IronPickaxe:
		case Item::IronShovel:
		case Item::NetheriteAxe:
		case Item::NetheriteHoe:
		case Item::NetheritePickaxe:
		case Item::NetheriteShovel:
		case Item::WoodenAxe:
		case Item::WoodenHoe:
		case Item::WoodenPickaxe:
		case Item::WoodenShovel:
			return true;
		default: return false;
	}
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
	return false;
}





bool cItemHandler::CanRepairWithRawMaterial(const cItem & a_Item)
{
	UNUSED(a_Item);
	return false;
}





bool cItemHandler::CanHarvestBlock(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Anvil:
		case BlockType::ChippedAnvil:
		case BlockType::DamagedAnvil:
		case BlockType::BlackGlazedTerracotta:
		case BlockType::CoalBlock:
		case BlockType::RedstoneBlock:
		case BlockType::BlueGlazedTerracotta:
		case BlockType::BoneBlock:
		case BlockType::BrewingStand:
		case BlockType::Bricks:
		case BlockType::BrickStairs:
		case BlockType::BrownGlazedTerracotta:
		case BlockType::Cauldron:
		case BlockType::CoalOre:
		case BlockType::Cobblestone:
		case BlockType::CobblestoneStairs:
		case BlockType::CobblestoneWall:
		case BlockType::Cobweb:
		case BlockType::CyanGlazedTerracotta:
		case BlockType::DiamondBlock:
		case BlockType::DiamondOre:
		case BlockType::Dispenser:
		case BlockType::Dropper:
		case BlockType::EmeraldOre:
		case BlockType::EnchantingTable:
		case BlockType::EndStoneBricks:
		case BlockType::EndStone:
		case BlockType::Furnace:
		case BlockType::GoldBlock:
		case BlockType::GoldOre:
		case BlockType::GrayGlazedTerracotta:
		case BlockType::GreenGlazedTerracotta:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::Hopper:
		case BlockType::IronBars:
		case BlockType::IronBlock:
		case BlockType::IronDoor:
		case BlockType::IronOre:
		case BlockType::IronTrapdoor:
		case BlockType::LapisBlock:
		case BlockType::LapisOre:
		case BlockType::LightBlueGlazedTerracotta:
		case BlockType::LightGrayGlazedTerracotta:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::LimeGlazedTerracotta:
		case BlockType::MagentaGlazedTerracotta:
		case BlockType::MagmaBlock:
		case BlockType::Spawner:
		case BlockType::MossyCobblestone:
		case BlockType::Netherrack:
		case BlockType::NetherBricks:
		case BlockType::NetherBrickFence:
		case BlockType::NetherBrickStairs:
		case BlockType::NetherQuartzOre:
		case BlockType::Observer:
		case BlockType::Obsidian:
		case BlockType::OrangeGlazedTerracotta:
		case BlockType::PinkGlazedTerracotta:
		case BlockType::Prismarine:
		case BlockType::PurpleGlazedTerracotta:
		case BlockType::PurpurBlock:
		case BlockType::PurpurPillar:
		case BlockType::PurpurSlab:
		case BlockType::PurpurStairs:
		case BlockType::QuartzBlock:
		case BlockType::QuartzStairs:
		case BlockType::RedstoneOre:
		case BlockType::RedGlazedTerracotta:
		case BlockType::RedNetherBricks:
		case BlockType::RedSandstone:
		case BlockType::RedSandstoneSlab:
		case BlockType::RedSandstoneStairs:
		case BlockType::Sandstone:
		case BlockType::SandstoneStairs:
		case BlockType::SnowBlock:
		case BlockType::Stone:
		case BlockType::StoneBricks:
		case BlockType::StoneBrickStairs:
		case BlockType::StonePressurePlate:
		case BlockType::StoneSlab:
		case BlockType::Vine:
		case BlockType::WhiteGlazedTerracotta:
		case BlockType::YellowGlazedTerracotta:
			// Concrete
		case BlockType::BlackConcretePowder:
		case BlockType::BlueConcretePowder:
		case BlockType::BrownConcretePowder:
		case BlockType::CyanConcretePowder:
		case BlockType::GrayConcretePowder:
		case BlockType::GreenConcretePowder:
		case BlockType::LightBlueConcretePowder:
		case BlockType::LightGrayConcretePowder:
		case BlockType::LimeConcretePowder:
		case BlockType::MagentaConcretePowder:
		case BlockType::OrangeConcretePowder:
		case BlockType::PinkConcretePowder:
		case BlockType::PurpleConcretePowder:
		case BlockType::RedConcretePowder:
		case BlockType::WhiteConcretePowder:
		case BlockType::YellowConcretePowder:
		{
			return false;
		}
		default: return true;
	}
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





float cItemHandler::GetBlockBreakingStrength(BlockState a_Block)
{
	return 1.0f;
}





bool cItemHandler::CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition)
{
	return false;
}
