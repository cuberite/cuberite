
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
#include "ItemCloth.h"
#include "ItemComparator.h"
#include "ItemCookedFish.h"
#include "ItemDefaultItem.h"
#include "ItemDoor.h"
#include "ItemDropSpenser.h"
#include "ItemDye.h"
#include "ItemEmptyMap.h"
#include "ItemEnchantingTable.h"
#include "ItemEndCrystal.h"
#include "ItemEnderChest.h"
#include "ItemEndPortalFrame.h"
#include "ItemEyeOfEnder.h"
#include "ItemFenceGate.h"
#include "ItemFishingRod.h"
#include "ItemFood.h"
#include "ItemFoodSeeds.h"
#include "ItemFurnace.h"
#include "ItemGlazedTerracotta.h"
#include "ItemGoldenApple.h"
#include "ItemHoe.h"
#include "ItemHopper.h"
#include "ItemItemFrame.h"
#include "ItemJackOLantern.h"
#include "ItemLadder.h"
#include "ItemLeaves.h"
#include "ItemLever.h"
#include "ItemLighter.h"
#include "ItemLilypad.h"
#include "ItemMap.h"
#include "ItemMilk.h"
#include "ItemMinecart.h"
#include "ItemMobHead.h"
#include "ItemNetherWart.h"
#include "ItemObserver.h"
#include "ItemPainting.h"
#include "ItemPickaxe.h"
#include "ItemPiston.h"
#include "ItemPlanks.h"
#include "ItemPoisonousPotato.h"
#include "ItemPotion.h"
#include "ItemPumpkin.h"
#include "ItemQuartz.h"
#include "ItemRail.h"
#include "ItemRawChicken.h"
#include "ItemRawFish.h"
#include "ItemRedstoneDust.h"
#include "ItemRedstoneRepeater.h"
#include "ItemRottenFlesh.h"
#include "ItemSapling.h"
#include "ItemSeeds.h"
#include "ItemShears.h"
#include "ItemShulkerBox.h"
#include "ItemShovel.h"
#include "ItemSideways.h"
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
#include "ItemVines.h"

#include "../Blocks/BlockHandler.h"
#include "SimplePlaceableItemHandler.h"





namespace
{
	constexpr cDefaultItemHandler           Item11DiscHandler                          (E_ITEM_11_DISC);
	constexpr cDefaultItemHandler           Item13DiscHandler                          (E_ITEM_13_DISC);
	constexpr cItemBoatHandler              ItemAcaciaBoatHandler                      (E_ITEM_ACACIA_BOAT);
	constexpr cDefaultItemHandler           ItemAcaciaDoorBlockHandler                 (E_BLOCK_ACACIA_DOOR);
	constexpr cItemDoorHandler              ItemAcaciaDoorHandler                      (E_ITEM_ACACIA_DOOR);
	constexpr cItemFenceGateHandler         ItemAcaciaFenceGateHandler                 (E_BLOCK_ACACIA_FENCE_GATE);
	constexpr cDefaultItemHandler           ItemAcaciaFenceHandler                     (E_BLOCK_ACACIA_FENCE);
	constexpr cItemStairsHandler            ItemAcaciaStairsHandler                    (E_BLOCK_ACACIA_WOOD_STAIRS);
	constexpr cItemRailHandler              ItemActivatorRailHandler                   (E_BLOCK_ACTIVATOR_RAIL);
	constexpr cDefaultItemHandler           ItemActiveComparatorHandler                (E_BLOCK_ACTIVE_COMPARATOR);
	constexpr cDefaultItemHandler           ItemAirHandler                             (E_BLOCK_AIR);
	constexpr cItemAnvilHandler             ItemAnvilHandler                           (E_BLOCK_ANVIL);
	constexpr cItemSimpleFoodHandler        ItemAppleHandler                           (E_ITEM_RED_APPLE, cItemHandler::FoodInfo(4, 2.4));
	constexpr cDefaultItemHandler           ItemArmorStandHandler                      (E_ITEM_ARMOR_STAND);
	constexpr cDefaultItemHandler           ItemArrowHandler                           (E_ITEM_ARROW);
	constexpr cItemSimpleFoodHandler        ItemBakedPotatoHandler                     (E_ITEM_BAKED_POTATO, cItemHandler::FoodInfo(5, 6));
	constexpr cItemBannerHandler            ItemBannerHandler                          (E_ITEM_BANNER);
	constexpr cDefaultItemHandler           ItemBarrierHandler                         (E_BLOCK_BARRIER);
	constexpr cDefaultItemHandler           ItemBeaconHandler                          (E_BLOCK_BEACON);
	constexpr cDefaultItemHandler           ItemBedBlockHandler                        (E_BLOCK_BED);
	constexpr cItemBedHandler               ItemBedHandler                             (E_ITEM_BED);
	constexpr cDefaultItemHandler           ItemBedrockHandler                         (E_BLOCK_BEDROCK);
	constexpr cItemSimpleFoodHandler        ItemBeetrootHandler                        (E_ITEM_BEETROOT, cItemHandler::FoodInfo(1, 1.2));
	constexpr cDefaultItemHandler           ItemBeetrootsBlockHandler                  (E_BLOCK_BEETROOTS);
	constexpr cItemSimpleSeedsHandler       ItemBeetrootSeedsHandler                   (E_ITEM_BEETROOT_SEEDS);
	constexpr cItemSoupHandler              ItemBeetrootSoupHandler                    (E_ITEM_BEETROOT_SOUP, cItemHandler::FoodInfo(6, 7.2));
	constexpr cItemBigFlowerHandler         ItemBigFlowerHandler                       (E_BLOCK_BIG_FLOWER);
	constexpr cItemBoatHandler              ItemBirchBoatHandler                       (E_ITEM_BIRCH_BOAT);
	constexpr cDefaultItemHandler           ItemBirchDoorBlockHandler                  (E_BLOCK_BIRCH_DOOR);
	constexpr cItemDoorHandler              ItemBirchDoorHandler                       (E_ITEM_BIRCH_DOOR);
	constexpr cItemFenceGateHandler         ItemBirchFenceGateHandler                  (E_BLOCK_BIRCH_FENCE_GATE);
	constexpr cDefaultItemHandler           ItemBirchFenceHandler                      (E_BLOCK_BIRCH_FENCE);
	constexpr cItemStairsHandler            ItemBirchStairsHandler                     (E_BLOCK_BIRCH_WOOD_STAIRS);
	constexpr cItemGlazedTerracottaHandler  ItemBlackGlazedTerracottaHandler           (E_BLOCK_BLACK_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemBlackShulkerBoxHandler                 (E_BLOCK_BLACK_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemBlazePowderHandler                     (E_ITEM_BLAZE_POWDER);
	constexpr cDefaultItemHandler           ItemBlazeRodHandler                        (E_ITEM_BLAZE_ROD);
	constexpr cDefaultItemHandler           ItemBlocksDiscHandler                      (E_ITEM_BLOCKS_DISC);
	constexpr cItemGlazedTerracottaHandler  ItemBlueGlazedTerracottaHandler            (E_BLOCK_BLUE_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemBlueShulkerBoxHandler                  (E_BLOCK_BLUE_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemBoneBlockHandler                       (E_BLOCK_BONE_BLOCK);
	constexpr cDefaultItemHandler           ItemBoneHandler                            (E_ITEM_BONE);
	constexpr cDefaultItemHandler           ItemBookAndQuillHandler                    (E_ITEM_BOOK_AND_QUILL);
	constexpr cDefaultItemHandler           ItemBookHandler                            (E_ITEM_BOOK);
	constexpr cDefaultItemHandler           ItemBookshelfHandler                       (E_BLOCK_BOOKCASE);
	constexpr cItemBottleOEnchantingHandler ItemBottleOEnchantingHandler               (E_ITEM_BOTTLE_O_ENCHANTING);
	constexpr cItemBowHandler               ItemBowHandler                             (E_ITEM_BOW);
	constexpr cDefaultItemHandler           ItemBowlHandler                            (E_ITEM_BOWL);
	constexpr cItemSimpleFoodHandler        ItemBreadHandler                           (E_ITEM_BREAD, cItemHandler::FoodInfo(5, 6));
	constexpr cDefaultItemHandler           ItemBrewingStandBlockHandler               (E_BLOCK_BREWING_STAND);
	constexpr cSimplePlaceableItemHandler   ItemBrewingStandHandler                    (E_ITEM_BREWING_STAND, E_BLOCK_BREWING_STAND);
	constexpr cDefaultItemHandler           ItemBrickHandler                           (E_ITEM_CLAY_BRICK);
	constexpr cDefaultItemHandler           ItemBricksHandler                          (E_BLOCK_BRICK);
	constexpr cItemStairsHandler            ItemBricksStairsHandler                    (E_BLOCK_BRICK_STAIRS);
	constexpr cItemGlazedTerracottaHandler  ItemBrownGlazedTerracottaHandler           (E_BLOCK_BROWN_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemBrownMushroomBlockHandler              (E_BLOCK_BROWN_MUSHROOM);
	constexpr cDefaultItemHandler           ItemBrownShulkerBoxHandler                 (E_BLOCK_BROWN_SHULKER_BOX);
	constexpr cItemBucketHandler            ItemBucketHandler                          (E_ITEM_BUCKET);
	constexpr cDefaultItemHandler           ItemCactusHandler                          (E_BLOCK_CACTUS);
	constexpr cDefaultItemHandler           ItemCakeBlockHandler                       (E_BLOCK_CAKE);
	constexpr cSimplePlaceableItemHandler   ItemCakeHandler                            (E_ITEM_CAKE, E_BLOCK_CAKE);
	constexpr cDefaultItemHandler           ItemCarpetHandler                          (E_BLOCK_CARPET);
	constexpr cItemFoodSeedsHandler         ItemCarrotHandler                          (E_ITEM_CARROT, cItemHandler::FoodInfo(3, 6.6));
	constexpr cDefaultItemHandler           ItemCarrotOnStickHandler                   (E_ITEM_CARROT_ON_STICK);
	constexpr cDefaultItemHandler           ItemCarrotsBlockHandler                    (E_BLOCK_CARROTS);
	constexpr cDefaultItemHandler           ItemCatDiscHandler                         (E_ITEM_CAT_DISC);
	constexpr cDefaultItemHandler           ItemCauldronBlockHandler                   (E_BLOCK_CAULDRON);
	constexpr cSimplePlaceableItemHandler   ItemCauldronHandler                        (E_ITEM_CAULDRON, E_BLOCK_CAULDRON);;
	constexpr cItemArmorHandler             ItemChainBootsHandler                      (E_ITEM_CHAIN_BOOTS);
	constexpr cItemArmorHandler             ItemChainChestplateHandler                 (E_ITEM_CHAIN_CHESTPLATE);
	constexpr cDefaultItemHandler           ItemChainCommandBlockHandler               (E_BLOCK_CHAIN_COMMAND_BLOCK);
	constexpr cItemArmorHandler             ItemChainHelmetHandler                     (E_ITEM_CHAIN_HELMET);
	constexpr cItemArmorHandler             ItemChainLeggingsHandler                   (E_ITEM_CHAIN_LEGGINGS);
	constexpr cItemChestHandler             ItemChestHandler                           (E_BLOCK_CHEST);
	constexpr cItemMinecartHandler          ItemChestMinecartHandler                   (E_ITEM_CHEST_MINECART);
	constexpr cDefaultItemHandler           ItemChirpDiscHandler                       (E_ITEM_CHIRP_DISC);
	constexpr cDefaultItemHandler           ItemChorusFlowerHandler                    (E_BLOCK_CHORUS_FLOWER);
	constexpr cItemChorusFruitHandler       ItemChorusFruitHandler                     (E_ITEM_CHORUS_FRUIT);
	constexpr cDefaultItemHandler           ItemChorusPlantHandler                     (E_BLOCK_CHORUS_PLANT);
	constexpr cDefaultItemHandler           ItemClayBlockHandler                       (E_BLOCK_CLAY);
	constexpr cDefaultItemHandler           ItemClayHandler                            (E_ITEM_CLAY);
	constexpr cDefaultItemHandler           ItemClockHandler                           (E_ITEM_CLOCK);
	constexpr cDefaultItemHandler           ItemCoalBlockHandler                       (E_BLOCK_BLOCK_OF_COAL);
	constexpr cDefaultItemHandler           ItemCoalHandler                            (E_ITEM_COAL);
	constexpr cDefaultItemHandler           ItemCoalOreHandler                         (E_BLOCK_COAL_ORE);
	constexpr cDefaultItemHandler           ItemCobblestoneHandler                     (E_BLOCK_COBBLESTONE);
	constexpr cItemStairsHandler            ItemCobblestoneStairsHandler               (E_BLOCK_COBBLESTONE_STAIRS);
	constexpr cDefaultItemHandler           ItemCobblestoneWallHandler                 (E_BLOCK_COBBLESTONE_WALL);
	constexpr cDefaultItemHandler           ItemCobwebHandler                          (E_BLOCK_COBWEB);
	constexpr cDefaultItemHandler           ItemCocoaPodHandler                        (E_BLOCK_COCOA_POD);
	constexpr cDefaultItemHandler           ItemCommandBlockHandler                    (E_BLOCK_COMMAND_BLOCK);
	constexpr cItemComparatorHandler        ItemComparatorHandler                      (E_ITEM_COMPARATOR);
	constexpr cDefaultItemHandler           ItemCompassHandler                         (E_ITEM_COMPASS);
	constexpr cDefaultItemHandler           ItemConcreteBlockHandler                   (E_BLOCK_CONCRETE);
	constexpr cDefaultItemHandler           ItemConcretePowderHandler                  (E_BLOCK_CONCRETE_POWDER);
	constexpr cItemSimpleFoodHandler        ItemCookedBeefHandler                      (E_ITEM_STEAK, cItemHandler::FoodInfo(8, 12.8));
	constexpr cItemSimpleFoodHandler        ItemCookedChickenHandler                   (E_ITEM_COOKED_CHICKEN, cItemHandler::FoodInfo(6, 7.2));
	constexpr cItemCookedFishHandler        ItemCookedFishHandler                      (E_ITEM_COOKED_FISH);
	constexpr cItemSimpleFoodHandler        ItemCookedMuttonHandler                    (E_ITEM_COOKED_MUTTON, cItemHandler::FoodInfo(6, 9.6));
	constexpr cItemSimpleFoodHandler        ItemCookedPorkchopHandler                  (E_ITEM_COOKED_PORKCHOP, cItemHandler::FoodInfo(8, 12.8));
	constexpr cItemSimpleFoodHandler        ItemCookedRabbitHandler                    (E_ITEM_COOKED_RABBIT, cItemHandler::FoodInfo(5, 6));
	constexpr cItemSimpleFoodHandler        ItemCookieHandler                          (E_ITEM_COOKIE, cItemHandler::FoodInfo(2, 0.5));
	constexpr cItemGlazedTerracottaHandler  ItemCyanGlazedTerracottaHandler            (E_BLOCK_CYAN_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemCyanShulkerBoxHandler                  (E_BLOCK_CYAN_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemDandelionHandler                       (E_BLOCK_DANDELION);
	constexpr cItemBoatHandler              ItemDarkOakBoatHandler                     (E_ITEM_DARK_OAK_BOAT);
	constexpr cDefaultItemHandler           ItemDarkOakDoorBlockHandler                (E_BLOCK_DARK_OAK_DOOR);
	constexpr cItemDoorHandler              ItemDarkOakDoorHandler                     (E_ITEM_DARK_OAK_DOOR);
	constexpr cItemFenceGateHandler         ItemDarkOakFenceGateHandler                (E_BLOCK_DARK_OAK_FENCE_GATE);
	constexpr cDefaultItemHandler           ItemDarkOakFenceHandler                    (E_BLOCK_DARK_OAK_FENCE);
	constexpr cItemStairsHandler            ItemDarkOakStairsHandler                   (E_BLOCK_DARK_OAK_WOOD_STAIRS);
	constexpr cDefaultItemHandler           ItemDaylightSensorBlockHandler             (E_BLOCK_DAYLIGHT_SENSOR);
	constexpr cDefaultItemHandler           ItemDeadBushHandler                        (E_BLOCK_DEAD_BUSH);
	constexpr cItemRailHandler              ItemDetectorRailHandler                    (E_BLOCK_DETECTOR_RAIL);
	constexpr cItemAxeHandler               ItemDiamondAxeHandler                      (E_ITEM_DIAMOND_AXE);
	constexpr cDefaultItemHandler           ItemDiamondBlockHandler                    (E_BLOCK_DIAMOND_BLOCK);
	constexpr cItemArmorHandler             ItemDiamondBootsHandler                    (E_ITEM_DIAMOND_BOOTS);
	constexpr cItemArmorHandler             ItemDiamondChestplateHandler               (E_ITEM_DIAMOND_CHESTPLATE);
	constexpr cDefaultItemHandler           ItemDiamondHandler                         (E_ITEM_DIAMOND);
	constexpr cItemArmorHandler             ItemDiamondHelmetHandler                   (E_ITEM_DIAMOND_HELMET);
	constexpr cItemHoeHandler               ItemDiamondHoeHandler                      (E_ITEM_DIAMOND_HOE);
	constexpr cDefaultItemHandler           ItemDiamondHorseArmorHandler               (E_ITEM_DIAMOND_HORSE_ARMOR);
	constexpr cItemArmorHandler             ItemDiamondLeggingsHandler                 (E_ITEM_DIAMOND_LEGGINGS);
	constexpr cDefaultItemHandler           ItemDiamondOreHandler                      (E_BLOCK_DIAMOND_ORE);
	constexpr cItemPickaxeHandler           ItemDiamondPickaxeHandler                  (E_ITEM_DIAMOND_PICKAXE);
	constexpr cItemShovelHandler            ItemDiamondShovelHandler                   (E_ITEM_DIAMOND_SHOVEL);
	constexpr cItemSwordHandler             ItemDiamondSwordHandler                    (E_ITEM_DIAMOND_SWORD);
	constexpr cDefaultItemHandler           ItemDirtHandler                            (E_BLOCK_DIRT);
	constexpr cItemDropSpenserHandler       ItemDispenserHandler                       (E_BLOCK_DISPENSER);
	constexpr cDefaultItemHandler           ItemDoubleOakSlabHandler                   (E_BLOCK_DOUBLE_WOODEN_SLAB);
	constexpr cDefaultItemHandler           ItemDoubleRedSandstoneSlabHandler          (E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB);
	constexpr cDefaultItemHandler           ItemDoubleStoneSlabHandler                 (E_BLOCK_DOUBLE_STONE_SLAB);
	constexpr cDefaultItemHandler           ItemDragonBreathHandler                    (E_ITEM_DRAGON_BREATH);
	constexpr cDefaultItemHandler           ItemDragonEggBlockHandler                  (E_BLOCK_DRAGON_EGG);
	constexpr cItemDropSpenserHandler       ItemDropperHandler                         (E_BLOCK_DROPPER);
	constexpr cItemDyeHandler               ItemDyeHandler                             (E_ITEM_DYE);
	constexpr cItemEggHandler               ItemEggHandler                             (E_ITEM_EGG);
	constexpr cItemArmorHandler             ItemElytraHandler                          (E_ITEM_ELYTRA);
	constexpr cDefaultItemHandler           ItemEmeraldBlockHandler                    (E_BLOCK_EMERALD_BLOCK);
	constexpr cDefaultItemHandler           ItemEmeraldHandler                         (E_ITEM_EMERALD);
	constexpr cDefaultItemHandler           ItemEmeraldOreHandler                      (E_BLOCK_EMERALD_ORE);
	constexpr cItemEmptyMapHandler          ItemEmptyMapHandler                        (E_ITEM_EMPTY_MAP);
	constexpr cDefaultItemHandler           ItemEnchantedBookHandler                   (E_ITEM_ENCHANTED_BOOK);
	constexpr cItemEnchantingTableHandler   ItemEnchantingTableHandler                 (E_BLOCK_ENCHANTMENT_TABLE);
	constexpr cDefaultItemHandler           ItemEndBricksHandler                       (E_BLOCK_END_BRICKS);
	constexpr cItemEndCrystalHandler        ItemEndCrystalHandler                      (E_ITEM_END_CRYSTAL);
	constexpr cItemEnderChestHandler        ItemEnderChestHandler                      (E_BLOCK_ENDER_CHEST);
	constexpr cItemEnderPearlHandler        ItemEnderPearlHandler                      (E_ITEM_ENDER_PEARL);
	constexpr cDefaultItemHandler           ItemEndGatewayHandler                      (E_BLOCK_END_GATEWAY);
	constexpr cDefaultItemHandler           ItemEndPortalBlockHandler                  (E_BLOCK_END_PORTAL);
	constexpr cItemEndPortalFrameHandler    ItemEndPortalFrameHandler                  (E_BLOCK_END_PORTAL_FRAME);
	constexpr cDefaultItemHandler           ItemEndRodHandler                          (E_BLOCK_END_ROD);
	constexpr cDefaultItemHandler           ItemEndStoneBlockHandler                   (E_BLOCK_END_STONE);
	constexpr cItemEyeOfEnderHandler        ItemEyeOfEnderHandler                      (E_ITEM_EYE_OF_ENDER);
	constexpr cDefaultItemHandler           ItemFarDiscHandler                         (E_ITEM_FAR_DISC);
	constexpr cDefaultItemHandler           ItemFarmlandHandler                        (E_BLOCK_FARMLAND);
	constexpr cDefaultItemHandler           ItemFeatherHandler                         (E_ITEM_FEATHER);
	constexpr cDefaultItemHandler           ItemFermentedSpiderEyeHandler              (E_ITEM_FERMENTED_SPIDER_EYE);
	constexpr cDefaultItemHandler           ItemFireBlockHandler                       (E_BLOCK_FIRE);
	constexpr cItemLighterHandler           ItemFireChargeHandler                      (E_ITEM_FIRE_CHARGE);
	constexpr cItemFireworkHandler          ItemFireworkRocketHandler                  (E_ITEM_FIREWORK_ROCKET);
	constexpr cDefaultItemHandler           ItemFireworkStarHandler                    (E_ITEM_FIREWORK_STAR);
	constexpr cItemFishingRodHandler        ItemFishingRodHandler                      (E_ITEM_FISHING_ROD);
	constexpr cItemLighterHandler           ItemFlintAndSteelHandler                   (E_ITEM_FLINT_AND_STEEL);
	constexpr cDefaultItemHandler           ItemFlintHandler                           (E_ITEM_FLINT);
	constexpr cDefaultItemHandler           ItemFlowerPotBlockHandler                  (E_BLOCK_FLOWER_POT);
	constexpr cSimplePlaceableItemHandler   ItemFlowerPotHandler                       (E_ITEM_FLOWER_POT, E_BLOCK_FLOWER_POT);
	constexpr cDefaultItemHandler           ItemFrostedIceHandler                      (E_BLOCK_FROSTED_ICE);
	constexpr cItemFurnaceHandler           ItemFurnaceHandler                         (E_BLOCK_FURNACE);
	constexpr cItemMinecartHandler          ItemFurnaceMinecartHandler                 (E_ITEM_FURNACE_MINECART);
	constexpr cDefaultItemHandler           ItemGhastTearHandler                       (E_ITEM_GHAST_TEAR);
	constexpr cItemBottleHandler            ItemGlassBottleHandler                     (E_ITEM_GLASS_BOTTLE);
	constexpr cDefaultItemHandler           ItemGlassHandler                           (E_BLOCK_GLASS);
	constexpr cDefaultItemHandler           ItemGlassPaneHandler                       (E_BLOCK_GLASS_PANE);
	constexpr cDefaultItemHandler           ItemGlisteringMelonHandler                 (E_ITEM_GLISTERING_MELON);
	constexpr cDefaultItemHandler           ItemGlowstoneBlockHandler                  (E_BLOCK_GLOWSTONE);
	constexpr cDefaultItemHandler           ItemGlowstoneDustHandler                   (E_ITEM_GLOWSTONE_DUST);
	constexpr cDefaultItemHandler           ItemGoldBlockHandler                       (E_BLOCK_GOLD_BLOCK);
	constexpr cItemGoldenAppleHandler       ItemGoldenAppleHandler                     (E_ITEM_GOLDEN_APPLE);
	constexpr cItemAxeHandler               ItemGoldenAxeHandler                       (E_ITEM_GOLD_AXE);
	constexpr cItemArmorHandler             ItemGoldenBootsHandler                     (E_ITEM_GOLD_BOOTS);
	constexpr cItemSimpleFoodHandler        ItemGoldenCarrotHandler                    (E_ITEM_GOLDEN_CARROT, cItemHandler::FoodInfo(6, 14.4));
	constexpr cItemArmorHandler             ItemGoldenChestplateHandler                (E_ITEM_GOLD_CHESTPLATE);
	constexpr cItemArmorHandler             ItemGoldenHelmetHandler                    (E_ITEM_GOLD_HELMET);
	constexpr cItemHoeHandler               ItemGoldenHoeHandler                       (E_ITEM_GOLD_HOE);
	constexpr cDefaultItemHandler           ItemGoldenHorseArmorHandler                (E_ITEM_GOLD_HORSE_ARMOR);
	constexpr cItemArmorHandler             ItemGoldenLeggingsHandler                  (E_ITEM_GOLD_LEGGINGS);
	constexpr cItemPickaxeHandler           ItemGoldenPickaxeHandler                   (E_ITEM_GOLD_PICKAXE);
	constexpr cItemShovelHandler            ItemGoldenShovelHandler                    (E_ITEM_GOLD_SHOVEL);
	constexpr cItemSwordHandler             ItemGoldenSwordHandler                     (E_ITEM_GOLD_SWORD);
	constexpr cDefaultItemHandler           ItemGoldHandler                            (E_ITEM_GOLD);
	constexpr cDefaultItemHandler           ItemGoldNuggetHandler                      (E_ITEM_GOLD_NUGGET);
	constexpr cDefaultItemHandler           ItemGoldOreHandler                         (E_BLOCK_GOLD_ORE);
	constexpr cDefaultItemHandler           ItemGrassBlockHandler                      (E_BLOCK_GRASS);
	constexpr cDefaultItemHandler           ItemGrassPathHandler                       (E_BLOCK_GRASS_PATH);
	constexpr cDefaultItemHandler           ItemGravelHandler                          (E_BLOCK_GRAVEL);
	constexpr cItemGlazedTerracottaHandler  ItemGrayGlazedTerracottaHandler            (E_BLOCK_GRAY_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemGrayShulkerBoxHandler                  (E_BLOCK_GRAY_SHULKER_BOX);
	constexpr cItemGlazedTerracottaHandler  ItemGreenGlazedTerracottaHandler           (E_BLOCK_GREEN_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemGreenShulkerBoxHandler                 (E_BLOCK_GREEN_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemGunpowderHandler                       (E_ITEM_GUNPOWDER);
	constexpr cDefaultItemHandler           ItemHardenedClayHandler                    (E_BLOCK_HARDENED_CLAY);
	constexpr cItemSidewaysHandler          ItemHayBaleHandler                         (E_BLOCK_HAY_BALE);
	constexpr cDefaultItemHandler           ItemHeadBlockHandler                       (E_BLOCK_HEAD);
	constexpr cItemMobHeadHandler           ItemHeadHandler                            (E_ITEM_HEAD);
	constexpr cDefaultItemHandler           ItemHeavyWeightedPressurePlateHandler      (E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE);
	constexpr cItemHopperHandler            ItemHopperHandler                          (E_BLOCK_HOPPER);
	constexpr cDefaultItemHandler           ItemHugeBrownMushroomBlockHandler          (E_BLOCK_HUGE_BROWN_MUSHROOM);
	constexpr cDefaultItemHandler           ItemHugeRedMushroomBlockHandler            (E_BLOCK_HUGE_RED_MUSHROOM);
	constexpr cDefaultItemHandler           ItemIceHandler                             (E_BLOCK_ICE);
	constexpr cDefaultItemHandler           ItemInactiveComparatorHandler              (E_BLOCK_INACTIVE_COMPARATOR);
	constexpr cDefaultItemHandler           ItemInvertedDaylightSensorHandler          (E_BLOCK_INVERTED_DAYLIGHT_SENSOR);
	constexpr cItemAxeHandler               ItemIronAxeHandler                         (E_ITEM_IRON_AXE);
	constexpr cDefaultItemHandler           ItemIronBarsBlockHandler                   (E_BLOCK_IRON_BARS);
	constexpr cDefaultItemHandler           ItemIronBlockHandler                       (E_BLOCK_IRON_BLOCK);
	constexpr cItemArmorHandler             ItemIronBootsHandler                       (E_ITEM_IRON_BOOTS);
	constexpr cItemArmorHandler             ItemIronChestplateHandler                  (E_ITEM_IRON_CHESTPLATE);
	constexpr cDefaultItemHandler           ItemIronDoorBlockHandler                   (E_BLOCK_IRON_DOOR);
	constexpr cItemDoorHandler              ItemIronDoorHandler                        (E_ITEM_IRON_DOOR);
	constexpr cDefaultItemHandler           ItemIronHandler                            (E_ITEM_IRON);
	constexpr cItemArmorHandler             ItemIronHelmetHandler                      (E_ITEM_IRON_HELMET);
	constexpr cItemHoeHandler               ItemIronHoeHandler                         (E_ITEM_IRON_HOE);
	constexpr cDefaultItemHandler           ItemIronHorseArmorHandler                  (E_ITEM_IRON_HORSE_ARMOR);
	constexpr cItemArmorHandler             ItemIronLeggingsHandler                    (E_ITEM_IRON_LEGGINGS);
	constexpr cDefaultItemHandler           ItemIronNuggetHandler                      (E_ITEM_IRON_NUGGET);
	constexpr cDefaultItemHandler           ItemIronOreHandler                         (E_BLOCK_IRON_ORE);
	constexpr cItemPickaxeHandler           ItemIronPickaxeHandler                     (E_ITEM_IRON_PICKAXE);
	constexpr cItemShovelHandler            ItemIronShovelHandler                      (E_ITEM_IRON_SHOVEL);
	constexpr cItemSwordHandler             ItemIronSwordHandler                       (E_ITEM_IRON_SWORD);
	constexpr cItemTrapdoorHandler          ItemIronTrapdoorHandler                    (E_BLOCK_IRON_TRAPDOOR);
	constexpr cItemItemFrameHandler         ItemItemFrameHandler                       (E_ITEM_ITEM_FRAME);
	constexpr cItemJackOLanternHandler      ItemJackOLanternHandler                    (E_BLOCK_JACK_O_LANTERN);
	constexpr cDefaultItemHandler           ItemJukeboxHandler                         (E_BLOCK_JUKEBOX);
	constexpr cItemBoatHandler              ItemJungleBoatHandler                      (E_ITEM_JUNGLE_BOAT);
	constexpr cDefaultItemHandler           ItemJungleDoorBlockHandler                 (E_BLOCK_JUNGLE_DOOR);
	constexpr cItemDoorHandler              ItemJungleDoorHandler                      (E_ITEM_JUNGLE_DOOR);
	constexpr cItemFenceGateHandler         ItemJungleFenceGateHandler                 (E_BLOCK_JUNGLE_FENCE_GATE);
	constexpr cDefaultItemHandler           ItemJungleFenceHandler                     (E_BLOCK_JUNGLE_FENCE);
	constexpr cItemStairsHandler            ItemJungleStairsHandler                    (E_BLOCK_JUNGLE_WOOD_STAIRS);
	constexpr cItemLadderHandler            ItemLadderHandler                          (E_BLOCK_LADDER);
	constexpr cDefaultItemHandler           ItemLapisBlockHandler                      (E_BLOCK_LAPIS_BLOCK);
	constexpr cDefaultItemHandler           ItemLapisOreHandler                        (E_BLOCK_LAPIS_ORE);
	constexpr cItemBucketHandler            ItemLavaBucketHandler                      (E_ITEM_LAVA_BUCKET);
	constexpr cDefaultItemHandler           ItemLavaHandler                            (E_BLOCK_LAVA);
	constexpr cDefaultItemHandler           ItemLeashHandler                           (E_ITEM_LEAD);
	constexpr cItemArmorHandler             ItemLeatherBootsHandler                    (E_ITEM_LEATHER_BOOTS);
	constexpr cItemArmorHandler             ItemLeatherCapHandler                      (E_ITEM_LEATHER_CAP);
	constexpr cDefaultItemHandler           ItemLeatherHandler                         (E_ITEM_LEATHER);
	constexpr cItemArmorHandler             ItemLeatherPantsHandler                    (E_ITEM_LEATHER_PANTS);
	constexpr cItemArmorHandler             ItemLeatherTunicHandler                    (E_ITEM_LEATHER_TUNIC);
	constexpr cItemLeavesHandler            ItemLeavesHandler                          (E_BLOCK_LEAVES);
	constexpr cItemLeverHandler             ItemLeverHandler                           (E_BLOCK_LEVER);
	constexpr cItemGlazedTerracottaHandler  ItemLightBlueGlazedTerracottaHandler       (E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemLightBlueShulkerBoxHandler             (E_BLOCK_LIGHT_BLUE_SHULKER_BOX);
	constexpr cItemGlazedTerracottaHandler  ItemLightGrayGlazedTerracottaHandler       (E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemLightGrayShulkerBoxHandler             (E_BLOCK_LIGHT_GRAY_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemLightWeightedPressurePlateHandler      (E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE);
	constexpr cItemLilypadHandler           ItemLilypadHandler                         (E_BLOCK_LILY_PAD);
	constexpr cItemGlazedTerracottaHandler  ItemLimeGlazedTerracottaHandler            (E_BLOCK_LIME_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemLimeShulkerBoxHandler                  (E_BLOCK_LIME_SHULKER_BOX);
	constexpr cItemPotionHandler            ItemLingeringPotionHandler                 (E_ITEM_LINGERING_POTION);
	constexpr cDefaultItemHandler           ItemLitFurnaceHandler                      (E_BLOCK_LIT_FURNACE);
	constexpr cItemSidewaysHandler          ItemLogHandler                             (E_BLOCK_LOG);
	constexpr cItemGlazedTerracottaHandler  ItemMagentaGlazedTerracottaHandler         (E_BLOCK_MAGENTA_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemMagentaShulkerBoxHandler               (E_BLOCK_MAGENTA_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemMagmaBlockHandler                      (E_BLOCK_MAGMA);
	constexpr cDefaultItemHandler           ItemMagmaCreamHandler                      (E_ITEM_MAGMA_CREAM);
	constexpr cDefaultItemHandler           ItemMallDiscHandler                        (E_ITEM_MALL_DISC);
	constexpr cItemMapHandler               ItemMapHandler                             (E_ITEM_MAP);
	constexpr cDefaultItemHandler           ItemMellohiDiscHandler                     (E_ITEM_MELLOHI_DISC);
	constexpr cDefaultItemHandler           ItemMelonBlockHandler                      (E_BLOCK_MELON);
	constexpr cItemSimpleSeedsHandler       ItemMelonSeedsHandler                      (E_ITEM_MELON_SEEDS);
	constexpr cItemSimpleFoodHandler        ItemMelonSliceHandler                      (E_ITEM_MELON_SLICE, cItemHandler::FoodInfo(2, 1.2));
	constexpr cDefaultItemHandler           ItemMelonStemHandler                       (E_BLOCK_MELON_STEM);
	constexpr cItemMilkHandler              ItemMilkHandler                            (E_ITEM_MILK);
	constexpr cItemMinecartHandler          ItemMinecartHandler                        (E_ITEM_MINECART);
	constexpr cItemMinecartHandler          ItemMinecartWithCommandBlockHandler        (E_ITEM_MINECART_WITH_COMMAND_BLOCK);
	constexpr cItemMinecartHandler          ItemMinecartWithHopperHandler              (E_ITEM_MINECART_WITH_HOPPER);
	constexpr cItemMinecartHandler          ItemMinecartWithTNTHandler                 (E_ITEM_MINECART_WITH_TNT);
	constexpr cDefaultItemHandler           ItemMobSpawnerBlockHandler                 (E_BLOCK_MOB_SPAWNER);
	constexpr cDefaultItemHandler           ItemMossyCobblestoneHandler                (E_BLOCK_MOSSY_COBBLESTONE);
	constexpr cItemSoupHandler              ItemMushroomSoupHandler                    (E_ITEM_MUSHROOM_SOUP, cItemHandler::FoodInfo(6, 7.2));
	constexpr cDefaultItemHandler           ItemMyceliumHandler                        (E_BLOCK_MYCELIUM);
	constexpr cDefaultItemHandler           ItemNameTagHandler                         (E_ITEM_NAME_TAG);
	constexpr cDefaultItemHandler           ItemNetherBrickFenceHandler                (E_BLOCK_NETHER_BRICK_FENCE);
	constexpr cDefaultItemHandler           ItemNetherBrickHandler                     (E_ITEM_NETHER_BRICK);
	constexpr cDefaultItemHandler           ItemNetherBricksHandler                    (E_BLOCK_NETHER_BRICK);
	constexpr cItemStairsHandler            ItemNetherBrickStairsHandler               (E_BLOCK_NETHER_BRICK_STAIRS);
	constexpr cDefaultItemHandler           ItemNetherPortalBlockHandler               (E_BLOCK_NETHER_PORTAL);
	constexpr cDefaultItemHandler           ItemNetherQuartzHandler                    (E_ITEM_NETHER_QUARTZ);
	constexpr cDefaultItemHandler           ItemNetherQuartzOreHandler                 (E_BLOCK_NETHER_QUARTZ_ORE);
	constexpr cDefaultItemHandler           ItemNetherrackHandler                      (E_BLOCK_NETHERRACK);
	constexpr cDefaultItemHandler           ItemNetherStarHandler                      (E_ITEM_NETHER_STAR);
	constexpr cDefaultItemHandler           ItemNetherWartBlockHandler                 (E_BLOCK_NETHER_WART_BLOCK);
	constexpr cItemNetherWartHandler        ItemNetherWartHandler                      (E_ITEM_NETHER_WART);
	constexpr cDefaultItemHandler           ItemNetherWartTechnicalBlockHandler        (E_BLOCK_NETHER_WART);
	constexpr cItemLeavesHandler            ItemNewLeavesHandler                       (E_BLOCK_NEW_LEAVES);
	constexpr cItemSidewaysHandler          ItemNewLogHandler                          (E_BLOCK_NEW_LOG);
	constexpr cDefaultItemHandler           ItemNoteBlockHandler                       (E_BLOCK_NOTE_BLOCK);
	constexpr cItemBoatHandler              ItemOakBoatHandler                         (E_ITEM_BOAT);
	constexpr cItemButtonHandler            ItemOakButtonHandler                       (E_BLOCK_WOODEN_BUTTON);
	constexpr cDefaultItemHandler           ItemOakDoorBlockHandler                    (E_BLOCK_OAK_DOOR);
	constexpr cItemDoorHandler              ItemOakDoorHandler                         (E_ITEM_WOODEN_DOOR);
	constexpr cItemFenceGateHandler         ItemOakFenceGateHandler                    (E_BLOCK_OAK_FENCE_GATE);
	constexpr cDefaultItemHandler           ItemOakFenceHandler                        (E_BLOCK_FENCE);
	constexpr cItemSlabHandler              ItemOakSlabHandler                         (E_BLOCK_WOODEN_SLAB);
	constexpr cItemStairsHandler            ItemOakStairsHandler                       (E_BLOCK_OAK_WOOD_STAIRS);
	constexpr cItemTrapdoorHandler          ItemOakTrapdoorHandler                     (E_BLOCK_TRAPDOOR);
	constexpr cItemObserverHandler          ItemObserverHandler                        (E_BLOCK_OBSERVER);
	constexpr cDefaultItemHandler           ItemObsidianHandler                        (E_BLOCK_OBSIDIAN);
	constexpr cItemGlazedTerracottaHandler  ItemOrangeGlazedTerracottaHandler          (E_BLOCK_ORANGE_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemOrangeShulkerBoxHandler                (E_BLOCK_ORANGE_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemPackedIceHandler                       (E_BLOCK_PACKED_ICE);
	constexpr cItemPaintingHandler          ItemPaintingHandler                        (E_ITEM_PAINTING);
	constexpr cDefaultItemHandler           ItemPaperHandler                           (E_ITEM_PAPER);
	constexpr cItemGlazedTerracottaHandler  ItemPinkGlazedTerracottaHandler            (E_BLOCK_PINK_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemPinkShulkerBoxHandler                  (E_BLOCK_PINK_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemPistonExtensionHandler                 (E_BLOCK_PISTON_EXTENSION);
	constexpr cItemPistonHandler            ItemPistonHandler                          (E_BLOCK_PISTON);
	constexpr cDefaultItemHandler           ItemPistonMovedBlockHandler                (E_BLOCK_PISTON_MOVED_BLOCK);
	constexpr cItemPlanksHandler            ItemPlanksHandler                          (E_BLOCK_PLANKS);
	constexpr cItemPoisonousPotatoHandler   ItemPoisonousPotatoHandler                 (E_ITEM_POISONOUS_POTATO);
	constexpr cDefaultItemHandler           ItemPoppedChorusFruitHandler               (E_ITEM_POPPED_CHORUS_FRUIT);
	constexpr cDefaultItemHandler           ItemPoppyHandler                           (E_BLOCK_FLOWER);
	constexpr cDefaultItemHandler           ItemPotatoesBlockHandler                   (E_BLOCK_POTATOES);
	constexpr cItemFoodSeedsHandler         ItemPotatoHandler                          (E_ITEM_POTATO, cItemHandler::FoodInfo(1, 0.6));
	constexpr cItemPotionHandler            ItemPotionHandler                          (E_ITEM_POTION);
	constexpr cItemRailHandler              ItemPoweredRailHandler                     (E_BLOCK_POWERED_RAIL);
	constexpr cDefaultItemHandler           ItemPrismarineBlockHandler                 (E_BLOCK_PRISMARINE_BLOCK);
	constexpr cDefaultItemHandler           ItemPrismarineCrystalsHandler              (E_ITEM_PRISMARINE_CRYSTALS);
	constexpr cDefaultItemHandler           ItemPrismarineShardHandler                 (E_ITEM_PRISMARINE_SHARD);
	constexpr cItemPumpkinHandler           ItemPumpkinHandler                         (E_BLOCK_PUMPKIN);
	constexpr cItemSimpleFoodHandler        ItemPumpkinPieHandler                      (E_ITEM_PUMPKIN_PIE, cItemHandler::FoodInfo(8, 4.8));
	constexpr cItemSimpleSeedsHandler       ItemPumpkinSeedsHandler                    (E_ITEM_PUMPKIN_SEEDS);
	constexpr cDefaultItemHandler           ItemPumpkinStemHandler                     (E_BLOCK_PUMPKIN_STEM);
	constexpr cItemGlazedTerracottaHandler  ItemPurpleGlazedTerracottaHandler          (E_BLOCK_PURPLE_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemPurpleShulkerBoxHandler                (E_BLOCK_PURPLE_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemPurpurBlockHandler                     (E_BLOCK_PURPUR_BLOCK);
	constexpr cDefaultItemHandler           ItemPurpurDoubleSlabHandler                (E_BLOCK_PURPUR_DOUBLE_SLAB);
	constexpr cDefaultItemHandler           ItemPurpurPillarHandler                    (E_BLOCK_PURPUR_PILLAR);
	constexpr cItemSlabHandler              ItemPurpurSlabHandler                      (E_BLOCK_PURPUR_SLAB);
	constexpr cItemStairsHandler            ItemPurpurStairsHandler                    (E_BLOCK_PURPUR_STAIRS);
	constexpr cItemQuartzHandler            ItemQuartzBlockHandler                     (E_BLOCK_QUARTZ_BLOCK);
	constexpr cItemStairsHandler            ItemQuartzStairsHandler                    (E_BLOCK_QUARTZ_STAIRS);
	constexpr cDefaultItemHandler           ItemRabbitHideHandler                      (E_ITEM_RABBIT_HIDE);
	constexpr cDefaultItemHandler           ItemRabbitsFootHandler                     (E_ITEM_RABBITS_FOOT);
	constexpr cItemSoupHandler              ItemRabbitStewHandler                      (E_ITEM_RABBIT_STEW, cItemHandler::FoodInfo(10, 12));
	constexpr cItemRailHandler              ItemRailHandler                            (E_BLOCK_RAIL);
	constexpr cItemSimpleFoodHandler        ItemRawBeefHandler                         (E_ITEM_RAW_BEEF, cItemHandler::FoodInfo(3, 1.8));
	constexpr cItemRawChickenHandler        ItemRawChickenHandler                      (E_ITEM_RAW_CHICKEN);
	constexpr cItemRawFishHandler           ItemRawFishHandler                         (E_ITEM_RAW_FISH);
	constexpr cItemSimpleFoodHandler        ItemRawMuttonHandler                       (E_ITEM_RAW_MUTTON, cItemHandler::FoodInfo(2, 1.2));
	constexpr cItemSimpleFoodHandler        ItemRawPorkchopHandler                     (E_ITEM_RAW_PORKCHOP, cItemHandler::FoodInfo(3, 1.8));
	constexpr cItemSimpleFoodHandler        ItemRawRabbitHandler                       (E_ITEM_RAW_RABBIT, cItemHandler::FoodInfo(3, 1.8));
	constexpr cItemGlazedTerracottaHandler  ItemRedGlazedTerracottaHandler             (E_BLOCK_RED_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemRedMushroomBlockHandler                (E_BLOCK_RED_MUSHROOM);
	constexpr cDefaultItemHandler           ItemRedNetherBricksHandler                 (E_BLOCK_RED_NETHER_BRICK);
	constexpr cDefaultItemHandler           ItemRedSandstoneHandler                    (E_BLOCK_RED_SANDSTONE);
	constexpr cItemSlabHandler              ItemRedSandstoneSlabHandler                (E_BLOCK_RED_SANDSTONE_SLAB);
	constexpr cItemStairsHandler            ItemRedSandstoneStairsHandler              (E_BLOCK_RED_SANDSTONE_STAIRS);
	constexpr cDefaultItemHandler           ItemRedShulkerBoxHandler                   (E_BLOCK_RED_SHULKER_BOX);
	constexpr cDefaultItemHandler           ItemRedstoneBlockHandler                   (E_BLOCK_BLOCK_OF_REDSTONE);
	constexpr cItemRedstoneDustHandler      ItemRedstoneDustHandler                    (E_ITEM_REDSTONE_DUST);
	constexpr cDefaultItemHandler           ItemRedstoneLampOffHandler                 (E_BLOCK_REDSTONE_LAMP_OFF);
	constexpr cDefaultItemHandler           ItemRedstoneLampOnHandler                  (E_BLOCK_REDSTONE_LAMP_ON);
	constexpr cDefaultItemHandler           ItemRedstoneOreGlowingHandler              (E_BLOCK_REDSTONE_ORE_GLOWING);
	constexpr cDefaultItemHandler           ItemRedstoneOreHandler                     (E_BLOCK_REDSTONE_ORE);
	constexpr cItemRedstoneRepeaterHandler  ItemRedstoneRepeaterHandler                (E_ITEM_REDSTONE_REPEATER);
	constexpr cDefaultItemHandler           ItemRedstoneRepeaterOffHandler             (E_BLOCK_REDSTONE_REPEATER_OFF);
	constexpr cDefaultItemHandler           ItemRedstoneRepeaterOnHandler              (E_BLOCK_REDSTONE_REPEATER_ON);
	constexpr cItemTorchHandler             ItemRedstoneTorchHandler                   (E_BLOCK_REDSTONE_TORCH_ON);
	constexpr cDefaultItemHandler           ItemRedstoneTorchOffBlockHandler           (E_BLOCK_REDSTONE_TORCH_OFF);
	constexpr cDefaultItemHandler           ItemRedstoneWireHandler                    (E_BLOCK_REDSTONE_WIRE);
	constexpr cDefaultItemHandler           ItemRepeatingCommandBlockHandler           (E_BLOCK_REPEATING_COMMAND_BLOCK);
	constexpr cItemRottenFleshHandler       ItemRottenFleshHandler                     (E_ITEM_ROTTEN_FLESH);
	constexpr cDefaultItemHandler           ItemSaddleHandler                          (E_ITEM_SADDLE);
	constexpr cDefaultItemHandler           ItemSandHandler                            (E_BLOCK_SAND);
	constexpr cDefaultItemHandler           ItemSandstoneHandler                       (E_BLOCK_SANDSTONE);
	constexpr cItemStairsHandler            ItemSandstoneStairsHandler                 (E_BLOCK_SANDSTONE_STAIRS);
	constexpr cItemSaplingHandler           ItemSaplingHandler                         (E_BLOCK_SAPLING);
	constexpr cDefaultItemHandler           ItemSeaLanternHandler                      (E_BLOCK_SEA_LANTERN);
	constexpr cItemSimpleSeedsHandler       ItemSeedsHandler                           (E_ITEM_SEEDS);
	constexpr cItemShearsHandler            ItemShearsHandler                          (E_ITEM_SHEARS);
	constexpr cDefaultItemHandler           ItemShieldHandler                          (E_ITEM_SHIELD);
	constexpr cDefaultItemHandler           ItemShulkerShellHandler                    (E_ITEM_SHULKER_SHELL);
	constexpr cItemSignHandler              ItemSignHandler                            (E_ITEM_SIGN);
	constexpr cDefaultItemHandler           ItemSignPostHandler                        (E_BLOCK_SIGN_POST);
	constexpr cDefaultItemHandler           ItemSilverfishEggBlockHandler              (E_BLOCK_SILVERFISH_EGG);
	constexpr cDefaultItemHandler           ItemSlimeballHandler                       (E_ITEM_SLIMEBALL);
	constexpr cDefaultItemHandler           ItemSlimeBlockHandler                      (E_BLOCK_SLIME_BLOCK);
	constexpr cItemSnowballHandler          ItemSnowballHandler                        (E_ITEM_SNOWBALL);
	constexpr cDefaultItemHandler           ItemSnowBlockHandler                       (E_BLOCK_SNOW_BLOCK);
	constexpr cItemSnowHandler              ItemSnowHandler                            (E_BLOCK_SNOW);
	constexpr cDefaultItemHandler           ItemSoulsandHandler                        (E_BLOCK_SOULSAND);
	constexpr cItemSpawnEggHandler          ItemSpawnEggHandler                        (E_ITEM_SPAWN_EGG);
	constexpr cDefaultItemHandler           ItemSpectralArrowHandler                   (E_ITEM_SPECTRAL_ARROW);
	constexpr cItemSpiderEyeHandler         ItemSpiderEyeHandler                       (E_ITEM_SPIDER_EYE);
	constexpr cItemPotionHandler            ItemSplashPotionHandler                    (E_ITEM_SPLASH_POTION);
	constexpr cDefaultItemHandler           ItemSpongeHandler                          (E_BLOCK_SPONGE);
	constexpr cItemBoatHandler              ItemSpruceBoatHandler                      (E_ITEM_SPRUCE_BOAT);
	constexpr cDefaultItemHandler           ItemSpruceDoorBlockHandler                 (E_BLOCK_SPRUCE_DOOR);
	constexpr cItemDoorHandler              ItemSpruceDoorHandler                      (E_ITEM_SPRUCE_DOOR);
	constexpr cItemFenceGateHandler         ItemSpruceFenceGateHandler                 (E_BLOCK_SPRUCE_FENCE_GATE);
	constexpr cDefaultItemHandler           ItemSpruceFenceHandler                     (E_BLOCK_SPRUCE_FENCE);
	constexpr cItemStairsHandler            ItemSpruceStairsHandler                    (E_BLOCK_SPRUCE_WOOD_STAIRS);
	constexpr cDefaultItemHandler           ItemStainedGlassHandler                    (E_BLOCK_STAINED_GLASS);
	constexpr cDefaultItemHandler           ItemStainedGlassPaneHandler                (E_BLOCK_STAINED_GLASS_PANE);
	constexpr cDefaultItemHandler           ItemStalDiscHandler                        (E_ITEM_STAL_DISC);
	constexpr cDefaultItemHandler           ItemStandingBannerHandler                  (E_BLOCK_STANDING_BANNER);
	constexpr cDefaultItemHandler           ItemStationaryLavaHandler                  (E_BLOCK_STATIONARY_LAVA);
	constexpr cDefaultItemHandler           ItemStationaryWaterHandler                 (E_BLOCK_STATIONARY_WATER);
	constexpr cDefaultItemHandler           ItemStickHandler                           (E_ITEM_STICK);
	constexpr cItemPistonHandler            ItemStickyPistonHandler                    (E_BLOCK_STICKY_PISTON);
	constexpr cItemAxeHandler               ItemStoneAxeHandler                        (E_ITEM_STONE_AXE);
	constexpr cDefaultItemHandler           ItemStoneBricksHandler                     (E_BLOCK_STONE_BRICKS);
	constexpr cItemStairsHandler            ItemStoneBrickStairsHandler                (E_BLOCK_STONE_BRICK_STAIRS);
	constexpr cItemButtonHandler            ItemStoneButtonHandler                     (E_BLOCK_STONE_BUTTON);
	constexpr cDefaultItemHandler           ItemStoneHandler                           (E_BLOCK_STONE);
	constexpr cItemHoeHandler               ItemStoneHoeHandler                        (E_ITEM_STONE_HOE);
	constexpr cItemPickaxeHandler           ItemStonePickaxeHandler                    (E_ITEM_STONE_PICKAXE);
	constexpr cDefaultItemHandler           ItemStonePressurePlateHandler              (E_BLOCK_STONE_PRESSURE_PLATE);
	constexpr cItemShovelHandler            ItemStoneShovelHandler                     (E_ITEM_STONE_SHOVEL);
	constexpr cItemSlabHandler              ItemStoneSlabHandler                       (E_BLOCK_STONE_SLAB);
	constexpr cItemSwordHandler             ItemStoneSwordHandler                      (E_ITEM_STONE_SWORD);
	constexpr cDefaultItemHandler           ItemStradDiscHandler                       (E_ITEM_STRAD_DISC);
	constexpr cSimplePlaceableItemHandler   ItemStringHandler                          (E_ITEM_STRING, E_BLOCK_TRIPWIRE);
	constexpr cDefaultItemHandler           ItemStructureBlockHandler                  (E_BLOCK_STRUCTURE_BLOCK);
	constexpr cDefaultItemHandler           ItemStructureVoidHandler                   (E_BLOCK_STRUCTURE_VOID);
	constexpr cDefaultItemHandler           ItemSugarCaneBlockHandler                  (E_BLOCK_SUGARCANE);
	constexpr cSimplePlaceableItemHandler   ItemSugarCaneHandler                       (E_ITEM_SUGARCANE, E_BLOCK_SUGARCANE);
	constexpr cDefaultItemHandler           ItemSugarHandler                           (E_ITEM_SUGAR);
	constexpr cDefaultItemHandler           ItemTallGrassHandler                       (E_BLOCK_TALL_GRASS);
	constexpr cDefaultItemHandler           ItemTerracottaHandler                      (E_BLOCK_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemTippedArrowHandler                     (E_ITEM_TIPPED_ARROW);
	constexpr cDefaultItemHandler           ItemTNTHandler                             (E_BLOCK_TNT);
	constexpr cItemTorchHandler             ItemTorchHandler                           (E_BLOCK_TORCH);
	constexpr cDefaultItemHandler           ItemTotemOfUndyingHandler                  (E_ITEM_TOTEM_OF_UNDYING);
	constexpr cItemChestHandler             ItemTrappedChestHandler                    (E_BLOCK_TRAPPED_CHEST);
	constexpr cDefaultItemHandler           ItemTripwireHandler                        (E_BLOCK_TRIPWIRE);
	constexpr cItemTripwireHookHandler      ItemTripwireHookHandler                    (E_BLOCK_TRIPWIRE_HOOK);
	constexpr cItemVinesHandler             ItemVinesHandler                           (E_BLOCK_VINES);
	constexpr cDefaultItemHandler           ItemWaitDiscHandler                        (E_ITEM_WAIT_DISC);
	constexpr cDefaultItemHandler           ItemWallBannerHandler                      (E_BLOCK_WALL_BANNER);
	constexpr cDefaultItemHandler           ItemWallsignHandler                        (E_BLOCK_WALLSIGN);
	constexpr cDefaultItemHandler           ItemWardDiscHandler                        (E_ITEM_WARD_DISC);
	constexpr cItemBucketHandler            ItemWaterBucketHandler                     (E_ITEM_WATER_BUCKET);
	constexpr cDefaultItemHandler           ItemWaterHandler                           (E_BLOCK_WATER);
	constexpr cDefaultItemHandler           ItemWheatBlockHandler                      (E_BLOCK_WHEAT);
	constexpr cDefaultItemHandler           ItemWheatHandler                           (E_ITEM_WHEAT);
	constexpr cItemGlazedTerracottaHandler  ItemWhiteGlazedTerracottaHandler           (E_BLOCK_WHITE_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemWhiteShulkerBoxHandler                 (E_BLOCK_WHITE_SHULKER_BOX);
	constexpr cItemAxeHandler               ItemWoodenAxeHandler                       (E_ITEM_WOODEN_AXE);
	constexpr cItemHoeHandler               ItemWoodenHoeHandler                       (E_ITEM_WOODEN_HOE);
	constexpr cItemPickaxeHandler           ItemWoodenPickaxeHandler                   (E_ITEM_WOODEN_PICKAXE);
	constexpr cDefaultItemHandler           ItemWoodenPressurePlateHandler             (E_BLOCK_WOODEN_PRESSURE_PLATE);
	constexpr cItemShovelHandler            ItemWoodenShovelHandler                    (E_ITEM_WOODEN_SHOVEL);
	constexpr cItemSwordHandler             ItemWoodenSwordHandler                     (E_ITEM_WOODEN_SWORD);
	constexpr cItemClothHandler             ItemWoolHandler                            (E_BLOCK_WOOL);
	constexpr cDefaultItemHandler           ItemWorkbenchHandler                       (E_BLOCK_WORKBENCH);
	constexpr cDefaultItemHandler           ItemWrittenBookHandler                     (E_ITEM_WRITTEN_BOOK);
	constexpr cItemGlazedTerracottaHandler  ItemYellowGlazedTerracottaHandler          (E_BLOCK_YELLOW_GLAZED_TERRACOTTA);
	constexpr cDefaultItemHandler           ItemYellowShulkerBoxHandler                (E_BLOCK_YELLOW_SHULKER_BOX);
}





const cItemHandler & cItemHandler::For(int a_ItemType)
{
	switch (a_ItemType)
	{
		case E_BLOCK_ACACIA_DOOR:                    return ItemAcaciaDoorBlockHandler;
		case E_BLOCK_ACACIA_FENCE:                   return ItemAcaciaFenceHandler;
		case E_BLOCK_ACACIA_FENCE_GATE:              return ItemAcaciaFenceGateHandler;
		case E_BLOCK_ACACIA_WOOD_STAIRS:             return ItemAcaciaStairsHandler;
		case E_BLOCK_ACTIVATOR_RAIL:                 return ItemActivatorRailHandler;
		case E_BLOCK_ACTIVE_COMPARATOR:              return ItemActiveComparatorHandler;
		case E_BLOCK_AIR:                            return ItemAirHandler;
		case E_BLOCK_ANVIL:                          return ItemAnvilHandler;
		case E_BLOCK_BARRIER:                        return ItemBarrierHandler;
		case E_BLOCK_BEACON:                         return ItemBeaconHandler;
		case E_BLOCK_BED:                            return ItemBedBlockHandler;
		case E_BLOCK_BEDROCK:                        return ItemBedrockHandler;
		case E_BLOCK_BEETROOTS:                      return ItemBeetrootsBlockHandler;
		case E_BLOCK_BIG_FLOWER:                     return ItemBigFlowerHandler;
		case E_BLOCK_BIRCH_DOOR:                     return ItemBirchDoorBlockHandler;
		case E_BLOCK_BIRCH_FENCE:                    return ItemBirchFenceHandler;
		case E_BLOCK_BIRCH_FENCE_GATE:               return ItemBirchFenceGateHandler;
		case E_BLOCK_BIRCH_WOOD_STAIRS:              return ItemBirchStairsHandler;
		case E_BLOCK_BLACK_GLAZED_TERRACOTTA:        return ItemBlackGlazedTerracottaHandler;
		case E_BLOCK_BLACK_SHULKER_BOX:              return ItemBlackShulkerBoxHandler;
		case E_BLOCK_BLOCK_OF_COAL:                  return ItemCoalBlockHandler;
		case E_BLOCK_BLOCK_OF_REDSTONE:              return ItemRedstoneBlockHandler;
		case E_BLOCK_BLUE_GLAZED_TERRACOTTA:         return ItemBlueGlazedTerracottaHandler;
		case E_BLOCK_BLUE_SHULKER_BOX:               return ItemBlueShulkerBoxHandler;
		case E_BLOCK_BONE_BLOCK:                     return ItemBoneBlockHandler;
		case E_BLOCK_BOOKCASE:                       return ItemBookshelfHandler;
		case E_BLOCK_BREWING_STAND:                  return ItemBrewingStandBlockHandler;
		case E_BLOCK_BRICK:                          return ItemBrickHandler;
		case E_BLOCK_BRICK_STAIRS:                   return ItemBricksStairsHandler;
		case E_BLOCK_BROWN_GLAZED_TERRACOTTA:        return ItemBrownGlazedTerracottaHandler;
		case E_BLOCK_BROWN_MUSHROOM:                 return ItemBrownMushroomBlockHandler;
		case E_BLOCK_BROWN_SHULKER_BOX:              return ItemBrownShulkerBoxHandler;
		case E_BLOCK_CACTUS:                         return ItemCactusHandler;
		case E_BLOCK_CAKE:                           return ItemCakeBlockHandler;
		case E_BLOCK_CARPET:                         return ItemCarpetHandler;
		case E_BLOCK_CARROTS:                        return ItemCarrotsBlockHandler;
		case E_BLOCK_CAULDRON:                       return ItemCauldronBlockHandler;
		case E_BLOCK_CHAIN_COMMAND_BLOCK:            return ItemChainCommandBlockHandler;
		case E_BLOCK_CHEST:                          return ItemChestHandler;
		case E_BLOCK_CHORUS_FLOWER:                  return ItemChorusFlowerHandler;
		case E_BLOCK_CHORUS_PLANT:                   return ItemChorusPlantHandler;
		case E_BLOCK_CLAY:                           return ItemClayBlockHandler;
		case E_BLOCK_COAL_ORE:                       return ItemCoalOreHandler;
		case E_BLOCK_COBBLESTONE:                    return ItemCobblestoneHandler;
		case E_BLOCK_COBBLESTONE_STAIRS:             return ItemCobblestoneStairsHandler;
		case E_BLOCK_COBBLESTONE_WALL:               return ItemCobblestoneWallHandler;
		case E_BLOCK_COBWEB:                         return ItemCobwebHandler;
		case E_BLOCK_COCOA_POD:                      return ItemCocoaPodHandler;
		case E_BLOCK_COMMAND_BLOCK:                  return ItemCommandBlockHandler;
		case E_BLOCK_CONCRETE:                       return ItemConcreteBlockHandler;
		case E_BLOCK_CONCRETE_POWDER:                return ItemConcretePowderHandler;
		case E_BLOCK_CRAFTING_TABLE:                 return ItemWorkbenchHandler;
		case E_BLOCK_CYAN_GLAZED_TERRACOTTA:         return ItemCyanGlazedTerracottaHandler;
		case E_BLOCK_CYAN_SHULKER_BOX:               return ItemCyanShulkerBoxHandler;
		case E_BLOCK_DANDELION:                      return ItemDandelionHandler;
		case E_BLOCK_DARK_OAK_DOOR:                  return ItemDarkOakDoorBlockHandler;
		case E_BLOCK_DARK_OAK_FENCE:                 return ItemDarkOakFenceHandler;
		case E_BLOCK_DARK_OAK_FENCE_GATE:            return ItemDarkOakFenceGateHandler;
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:           return ItemDarkOakStairsHandler;
		case E_BLOCK_DAYLIGHT_SENSOR:                return ItemDaylightSensorBlockHandler;
		case E_BLOCK_DEAD_BUSH:                      return ItemDeadBushHandler;
		case E_BLOCK_DETECTOR_RAIL:                  return ItemDetectorRailHandler;
		case E_BLOCK_DIAMOND_BLOCK:                  return ItemDiamondBlockHandler;
		case E_BLOCK_DIAMOND_ORE:                    return ItemDiamondOreHandler;
		case E_BLOCK_DIRT:                           return ItemDirtHandler;
		case E_BLOCK_DISPENSER:                      return ItemDispenserHandler;
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:      return ItemDoubleRedSandstoneSlabHandler;
		case E_BLOCK_DOUBLE_STONE_SLAB:              return ItemDoubleStoneSlabHandler;
		case E_BLOCK_DOUBLE_WOODEN_SLAB:             return ItemDoubleOakSlabHandler;
		case E_BLOCK_DRAGON_EGG:                     return ItemDragonEggBlockHandler;
		case E_BLOCK_DROPPER:                        return ItemDropperHandler;
		case E_BLOCK_EMERALD_BLOCK:                  return ItemEmeraldBlockHandler;
		case E_BLOCK_EMERALD_ORE:                    return ItemEmeraldOreHandler;
		case E_BLOCK_ENCHANTMENT_TABLE:              return ItemEnchantingTableHandler;
		case E_BLOCK_ENDER_CHEST:                    return ItemEnderChestHandler;
		case E_BLOCK_END_BRICKS:                     return ItemEndBricksHandler;
		case E_BLOCK_END_GATEWAY:                    return ItemEndGatewayHandler;
		case E_BLOCK_END_PORTAL:                     return ItemEndPortalBlockHandler;
		case E_BLOCK_END_PORTAL_FRAME:               return ItemEndPortalFrameHandler;
		case E_BLOCK_END_ROD:                        return ItemEndRodHandler;
		case E_BLOCK_END_STONE:                      return ItemEndStoneBlockHandler;
		case E_BLOCK_FARMLAND:                       return ItemFarmlandHandler;
		case E_BLOCK_FENCE:                          return ItemOakFenceHandler;
		case E_BLOCK_FIRE:                           return ItemFireBlockHandler;
		case E_BLOCK_FLOWER:                         return ItemPoppyHandler;
		case E_BLOCK_FLOWER_POT:                     return ItemFlowerPotBlockHandler;
		case E_BLOCK_FROSTED_ICE:                    return ItemFrostedIceHandler;
		case E_BLOCK_FURNACE:                        return ItemFurnaceHandler;
		case E_BLOCK_GLASS:                          return ItemGlassHandler;
		case E_BLOCK_GLASS_PANE:                     return ItemGlassPaneHandler;
		case E_BLOCK_GLOWSTONE:                      return ItemGlowstoneBlockHandler;
		case E_BLOCK_GOLD_BLOCK:                     return ItemGoldBlockHandler;
		case E_BLOCK_GOLD_ORE:                       return ItemGoldOreHandler;
		case E_BLOCK_GRASS:                          return ItemGrassBlockHandler;
		case E_BLOCK_GRASS_PATH:                     return ItemGrassPathHandler;
		case E_BLOCK_GRAVEL:                         return ItemGravelHandler;
		case E_BLOCK_GRAY_GLAZED_TERRACOTTA:         return ItemGrayGlazedTerracottaHandler;
		case E_BLOCK_GRAY_SHULKER_BOX:               return ItemGrayShulkerBoxHandler;
		case E_BLOCK_GREEN_GLAZED_TERRACOTTA:        return ItemGreenGlazedTerracottaHandler;
		case E_BLOCK_GREEN_SHULKER_BOX:              return ItemGreenShulkerBoxHandler;
		case E_BLOCK_HARDENED_CLAY:                  return ItemHardenedClayHandler;
		case E_BLOCK_HAY_BALE:                       return ItemHayBaleHandler;
		case E_BLOCK_HEAD:                           return ItemHeadBlockHandler;
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:  return ItemHeavyWeightedPressurePlateHandler;
		case E_BLOCK_HOPPER:                         return ItemHopperHandler;
		case E_BLOCK_HUGE_BROWN_MUSHROOM:            return ItemHugeBrownMushroomBlockHandler;
		case E_BLOCK_HUGE_RED_MUSHROOM:              return ItemHugeRedMushroomBlockHandler;
		case E_BLOCK_ICE:                            return ItemIceHandler;
		case E_BLOCK_INACTIVE_COMPARATOR:            return ItemInactiveComparatorHandler;
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:       return ItemInvertedDaylightSensorHandler;
		case E_BLOCK_IRON_BARS:                      return ItemIronBarsBlockHandler;
		case E_BLOCK_IRON_BLOCK:                     return ItemIronBlockHandler;
		case E_BLOCK_IRON_DOOR:                      return ItemIronDoorBlockHandler;
		case E_BLOCK_IRON_ORE:                       return ItemIronOreHandler;
		case E_BLOCK_IRON_TRAPDOOR:                  return ItemIronTrapdoorHandler;
		case E_BLOCK_JACK_O_LANTERN:                 return ItemJackOLanternHandler;
		case E_BLOCK_JUKEBOX:                        return ItemJukeboxHandler;
		case E_BLOCK_JUNGLE_DOOR:                    return ItemJungleDoorBlockHandler;
		case E_BLOCK_JUNGLE_FENCE:                   return ItemJungleFenceHandler;
		case E_BLOCK_JUNGLE_FENCE_GATE:              return ItemJungleFenceGateHandler;
		case E_BLOCK_JUNGLE_WOOD_STAIRS:             return ItemJungleStairsHandler;
		case E_BLOCK_LADDER:                         return ItemLadderHandler;
		case E_BLOCK_LAPIS_BLOCK:                    return ItemLapisBlockHandler;
		case E_BLOCK_LAPIS_ORE:                      return ItemLapisOreHandler;
		case E_BLOCK_LAVA:                           return ItemLavaHandler;
		case E_BLOCK_LEAVES:                         return ItemLeavesHandler;
		case E_BLOCK_LEVER:                          return ItemLeverHandler;
		case E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA:   return ItemLightBlueGlazedTerracottaHandler;
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:         return ItemLightBlueShulkerBoxHandler;
		case E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA:   return ItemLightGrayGlazedTerracottaHandler;
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:         return ItemLightGrayShulkerBoxHandler;
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:  return ItemLightWeightedPressurePlateHandler;
		case E_BLOCK_LILY_PAD:                       return ItemLilypadHandler;
		case E_BLOCK_LIME_GLAZED_TERRACOTTA:         return ItemLimeGlazedTerracottaHandler;
		case E_BLOCK_LIME_SHULKER_BOX:               return ItemLimeShulkerBoxHandler;
		case E_BLOCK_LIT_FURNACE:                    return ItemLitFurnaceHandler;
		case E_BLOCK_LOG:                            return ItemLogHandler;
		case E_BLOCK_MAGENTA_GLAZED_TERRACOTTA:      return ItemMagentaGlazedTerracottaHandler;
		case E_BLOCK_MAGENTA_SHULKER_BOX:            return ItemMagentaShulkerBoxHandler;
		case E_BLOCK_MAGMA:                          return ItemMagmaBlockHandler;
		case E_BLOCK_MELON:                          return ItemMelonBlockHandler;
		case E_BLOCK_MELON_STEM:                     return ItemMelonStemHandler;
		case E_BLOCK_MOB_SPAWNER:                    return ItemMobSpawnerBlockHandler;
		case E_BLOCK_MOSSY_COBBLESTONE:              return ItemMossyCobblestoneHandler;
		case E_BLOCK_MYCELIUM:                       return ItemMyceliumHandler;
		case E_BLOCK_NETHERRACK:                     return ItemNetherrackHandler;
		case E_BLOCK_NETHER_BRICK:                   return ItemNetherBricksHandler;
		case E_BLOCK_NETHER_BRICK_FENCE:             return ItemNetherBrickFenceHandler;
		case E_BLOCK_NETHER_BRICK_STAIRS:            return ItemNetherBrickStairsHandler;
		case E_BLOCK_NETHER_PORTAL:                  return ItemNetherPortalBlockHandler;
		case E_BLOCK_NETHER_QUARTZ_ORE:              return ItemNetherQuartzOreHandler;
		case E_BLOCK_NETHER_WART:                    return ItemNetherWartTechnicalBlockHandler;
		case E_BLOCK_NETHER_WART_BLOCK:              return ItemNetherWartBlockHandler;
		case E_BLOCK_NEW_LEAVES:                     return ItemNewLeavesHandler;
		case E_BLOCK_NEW_LOG:                        return ItemNewLogHandler;
		case E_BLOCK_NOTE_BLOCK:                     return ItemNoteBlockHandler;
		case E_BLOCK_OAK_DOOR:                       return ItemOakDoorBlockHandler;
		case E_BLOCK_OAK_FENCE_GATE:                 return ItemOakFenceGateHandler;
		case E_BLOCK_OAK_WOOD_STAIRS:                return ItemOakStairsHandler;
		case E_BLOCK_OBSERVER:                       return ItemObserverHandler;
		case E_BLOCK_OBSIDIAN:                       return ItemObsidianHandler;
		case E_BLOCK_ORANGE_GLAZED_TERRACOTTA:       return ItemOrangeGlazedTerracottaHandler;
		case E_BLOCK_ORANGE_SHULKER_BOX:             return ItemOrangeShulkerBoxHandler;
		case E_BLOCK_PACKED_ICE:                     return ItemPackedIceHandler;
		case E_BLOCK_PINK_GLAZED_TERRACOTTA:         return ItemPinkGlazedTerracottaHandler;
		case E_BLOCK_PINK_SHULKER_BOX:               return ItemPinkShulkerBoxHandler;
		case E_BLOCK_PISTON:                         return ItemPistonHandler;
		case E_BLOCK_PISTON_EXTENSION:               return ItemPistonExtensionHandler;
		case E_BLOCK_PISTON_MOVED_BLOCK:             return ItemPistonMovedBlockHandler;
		case E_BLOCK_PLANKS:                         return ItemPlanksHandler;
		case E_BLOCK_POTATOES:                       return ItemPotatoesBlockHandler;
		case E_BLOCK_POWERED_RAIL:                   return ItemPoweredRailHandler;
		case E_BLOCK_PRISMARINE_BLOCK:               return ItemPrismarineBlockHandler;
		case E_BLOCK_PUMPKIN:                        return ItemPumpkinHandler;
		case E_BLOCK_PUMPKIN_STEM:                   return ItemPumpkinStemHandler;
		case E_BLOCK_PURPLE_GLAZED_TERRACOTTA:       return ItemPurpleGlazedTerracottaHandler;
		case E_BLOCK_PURPLE_SHULKER_BOX:             return ItemPurpleShulkerBoxHandler;
		case E_BLOCK_PURPUR_BLOCK:                   return ItemPurpurBlockHandler;
		case E_BLOCK_PURPUR_DOUBLE_SLAB:             return ItemPurpurDoubleSlabHandler;
		case E_BLOCK_PURPUR_PILLAR:                  return ItemPurpurPillarHandler;
		case E_BLOCK_PURPUR_SLAB:                    return ItemPurpurSlabHandler;
		case E_BLOCK_PURPUR_STAIRS:                  return ItemPurpurStairsHandler;
		case E_BLOCK_QUARTZ_BLOCK:                   return ItemQuartzBlockHandler;
		case E_BLOCK_QUARTZ_STAIRS:                  return ItemQuartzStairsHandler;
		case E_BLOCK_RAIL:                           return ItemRailHandler;
		case E_BLOCK_REDSTONE_LAMP_OFF:              return ItemRedstoneLampOffHandler;
		case E_BLOCK_REDSTONE_LAMP_ON:               return ItemRedstoneLampOnHandler;
		case E_BLOCK_REDSTONE_ORE:                   return ItemRedstoneOreHandler;
		case E_BLOCK_REDSTONE_ORE_GLOWING:           return ItemRedstoneOreGlowingHandler;
		case E_BLOCK_REDSTONE_REPEATER_OFF:          return ItemRedstoneRepeaterOffHandler;
		case E_BLOCK_REDSTONE_REPEATER_ON:           return ItemRedstoneRepeaterOnHandler;
		case E_BLOCK_REDSTONE_TORCH_OFF:             return ItemRedstoneTorchOffBlockHandler;
		case E_BLOCK_REDSTONE_TORCH_ON:              return ItemRedstoneTorchHandler;
		case E_BLOCK_REDSTONE_WIRE:                  return ItemRedstoneWireHandler;
		case E_BLOCK_RED_GLAZED_TERRACOTTA:          return ItemRedGlazedTerracottaHandler;
		case E_BLOCK_RED_MUSHROOM:                   return ItemRedMushroomBlockHandler;
		case E_BLOCK_RED_NETHER_BRICK:               return ItemRedNetherBricksHandler;
		case E_BLOCK_RED_SANDSTONE:                  return ItemRedSandstoneHandler;
		case E_BLOCK_RED_SANDSTONE_SLAB:             return ItemRedSandstoneSlabHandler;
		case E_BLOCK_RED_SANDSTONE_STAIRS:           return ItemRedSandstoneStairsHandler;
		case E_BLOCK_RED_SHULKER_BOX:                return ItemRedShulkerBoxHandler;
		case E_BLOCK_REPEATING_COMMAND_BLOCK:        return ItemRepeatingCommandBlockHandler;
		case E_BLOCK_SAND:                           return ItemSandHandler;
		case E_BLOCK_SANDSTONE:                      return ItemSandstoneHandler;
		case E_BLOCK_SANDSTONE_STAIRS:               return ItemSandstoneStairsHandler;
		case E_BLOCK_SAPLING:                        return ItemSaplingHandler;
		case E_BLOCK_SEA_LANTERN:                    return ItemSeaLanternHandler;
		case E_BLOCK_SIGN_POST:                      return ItemSignPostHandler;
		case E_BLOCK_SILVERFISH_EGG:                 return ItemSilverfishEggBlockHandler;
		case E_BLOCK_SLIME_BLOCK:                    return ItemSlimeBlockHandler;
		case E_BLOCK_SNOW:                           return ItemSnowHandler;
		case E_BLOCK_SNOW_BLOCK:                     return ItemSnowBlockHandler;
		case E_BLOCK_SOULSAND:                       return ItemSoulsandHandler;
		case E_BLOCK_SPONGE:                         return ItemSpongeHandler;
		case E_BLOCK_SPRUCE_DOOR:                    return ItemSpruceDoorBlockHandler;
		case E_BLOCK_SPRUCE_FENCE:                   return ItemSpruceFenceHandler;
		case E_BLOCK_SPRUCE_FENCE_GATE:              return ItemSpruceFenceGateHandler;
		case E_BLOCK_SPRUCE_WOOD_STAIRS:             return ItemSpruceStairsHandler;
		case E_BLOCK_STAINED_GLASS:                  return ItemStainedGlassHandler;
		case E_BLOCK_STAINED_GLASS_PANE:             return ItemStainedGlassPaneHandler;
		case E_BLOCK_STANDING_BANNER:                return ItemStandingBannerHandler;
		case E_BLOCK_STATIONARY_LAVA :               return ItemStationaryLavaHandler;
		case E_BLOCK_STATIONARY_WATER:               return ItemStationaryWaterHandler;
		case E_BLOCK_STICKY_PISTON:                  return ItemStickyPistonHandler;
		case E_BLOCK_STONE:                          return ItemStoneHandler;
		case E_BLOCK_STONE_BRICKS:                   return ItemStoneBricksHandler;
		case E_BLOCK_STONE_BRICK_STAIRS:             return ItemStoneBrickStairsHandler;
		case E_BLOCK_STONE_BUTTON:                   return ItemStoneButtonHandler;
		case E_BLOCK_STONE_PRESSURE_PLATE:           return ItemStonePressurePlateHandler;
		case E_BLOCK_STONE_SLAB:                     return ItemStoneSlabHandler;
		case E_BLOCK_STRUCTURE_BLOCK:                return ItemStructureBlockHandler;
		case E_BLOCK_STRUCTURE_VOID:                 return ItemStructureVoidHandler;
		case E_BLOCK_SUGARCANE:                      return ItemSugarCaneBlockHandler;
		case E_BLOCK_TALL_GRASS:                     return ItemTallGrassHandler;
		case E_BLOCK_TERRACOTTA:                     return ItemTerracottaHandler;
		case E_BLOCK_TNT:                            return ItemTNTHandler;
		case E_BLOCK_TORCH:                          return ItemTorchHandler;
		case E_BLOCK_TRAPDOOR:                       return ItemOakTrapdoorHandler;
		case E_BLOCK_TRAPPED_CHEST:                  return ItemTrappedChestHandler;
		case E_BLOCK_TRIPWIRE:                       return ItemTripwireHandler;
		case E_BLOCK_TRIPWIRE_HOOK:                  return ItemTripwireHookHandler;
		case E_BLOCK_VINES:                          return ItemVinesHandler;
		case E_BLOCK_WALLSIGN:                       return ItemWallsignHandler;
		case E_BLOCK_WALL_BANNER:                    return ItemWallBannerHandler;
		case E_BLOCK_WATER:                          return ItemWaterHandler;
		case E_BLOCK_WHEAT:                          return ItemWheatBlockHandler;
		case E_BLOCK_WHITE_GLAZED_TERRACOTTA:        return ItemWhiteGlazedTerracottaHandler;
		case E_BLOCK_WHITE_SHULKER_BOX:              return ItemWhiteShulkerBoxHandler;
		case E_BLOCK_WOODEN_BUTTON:                  return ItemOakButtonHandler;
		case E_BLOCK_WOODEN_PRESSURE_PLATE:          return ItemWoodenPressurePlateHandler;
		case E_BLOCK_WOODEN_SLAB:                    return ItemOakSlabHandler;
		case E_BLOCK_WOOL:                           return ItemWoolHandler;
		case E_BLOCK_YELLOW_GLAZED_TERRACOTTA:       return ItemYellowGlazedTerracottaHandler;
		case E_BLOCK_YELLOW_SHULKER_BOX:             return ItemYellowShulkerBoxHandler;
		case E_ITEM_11_DISC:                         return Item11DiscHandler;
		case E_ITEM_13_DISC:                         return Item13DiscHandler;
		case E_ITEM_ACACIA_BOAT:                     return ItemAcaciaBoatHandler;
		case E_ITEM_ACACIA_DOOR:                     return ItemAcaciaDoorHandler;
		case E_ITEM_ARMOR_STAND:                     return ItemArmorStandHandler;
		case E_ITEM_ARROW:                           return ItemArrowHandler;
		case E_ITEM_BAKED_POTATO:                    return ItemBakedPotatoHandler;
		case E_ITEM_BANNER:                          return ItemBannerHandler;
		case E_ITEM_BED:                             return ItemBedHandler;
		case E_ITEM_BEETROOT:                        return ItemBeetrootHandler;
		case E_ITEM_BEETROOT_SEEDS:                  return ItemBeetrootSeedsHandler;
		case E_ITEM_BEETROOT_SOUP:                   return ItemBeetrootSoupHandler;
		case E_ITEM_BIRCH_BOAT:                      return ItemBirchBoatHandler;
		case E_ITEM_BIRCH_DOOR:                      return ItemBirchDoorHandler;
		case E_ITEM_BLAZE_POWDER:                    return ItemBlazePowderHandler;
		case E_ITEM_BLAZE_ROD:                       return ItemBlazeRodHandler;
		case E_ITEM_BLOCKS_DISC:                     return ItemBlocksDiscHandler;
		case E_ITEM_BOAT:                            return ItemOakBoatHandler;
		case E_ITEM_BONE:                            return ItemBoneHandler;
		case E_ITEM_BOOK:                            return ItemBookHandler;
		case E_ITEM_BOOK_AND_QUILL:                  return ItemBookAndQuillHandler;
		case E_ITEM_BOTTLE_O_ENCHANTING:             return ItemBottleOEnchantingHandler;
		case E_ITEM_BOW:                             return ItemBowHandler;
		case E_ITEM_BOWL:                            return ItemBowlHandler;
		case E_ITEM_BREAD:                           return ItemBreadHandler;
		case E_ITEM_BREWING_STAND:                   return ItemBrewingStandHandler;
		case E_ITEM_BUCKET:                          return ItemBucketHandler;
		case E_ITEM_CAKE:                            return ItemCakeHandler;
		case E_ITEM_CARROT:                          return ItemCarrotHandler;
		case E_ITEM_CARROT_ON_STICK:                 return ItemCarrotOnStickHandler;
		case E_ITEM_CAT_DISC:                        return ItemCatDiscHandler;
		case E_ITEM_CAULDRON:                        return ItemCauldronHandler;
		case E_ITEM_CHAIN_BOOTS:                     return ItemChainBootsHandler;
		case E_ITEM_CHAIN_CHESTPLATE:                return ItemChainChestplateHandler;
		case E_ITEM_CHAIN_HELMET:                    return ItemChainHelmetHandler;
		case E_ITEM_CHAIN_LEGGINGS:                  return ItemChainLeggingsHandler;
		case E_ITEM_CHEST_MINECART:                  return ItemChestMinecartHandler;
		case E_ITEM_CHIRP_DISC:                      return ItemChirpDiscHandler;
		case E_ITEM_CHORUS_FRUIT:                    return ItemChorusFruitHandler;
		case E_ITEM_CLAY:                            return ItemClayHandler;
		case E_ITEM_CLAY_BRICK:                      return ItemBricksHandler;
		case E_ITEM_CLOCK:                           return ItemClockHandler;
		case E_ITEM_COAL:                            return ItemCoalHandler;
		case E_ITEM_COMPARATOR:                      return ItemComparatorHandler;
		case E_ITEM_COMPASS:                         return ItemCompassHandler;
		case E_ITEM_COOKED_CHICKEN:                  return ItemCookedChickenHandler;
		case E_ITEM_COOKED_FISH:                     return ItemCookedFishHandler;
		case E_ITEM_COOKED_MUTTON:                   return ItemCookedMuttonHandler;
		case E_ITEM_COOKED_PORKCHOP:                 return ItemCookedPorkchopHandler;
		case E_ITEM_COOKED_RABBIT:                   return ItemCookedRabbitHandler;
		case E_ITEM_COOKIE:                          return ItemCookieHandler;
		case E_ITEM_DARK_OAK_BOAT:                   return ItemDarkOakBoatHandler;
		case E_ITEM_DARK_OAK_DOOR:                   return ItemDarkOakDoorHandler;
		case E_ITEM_DIAMOND:                         return ItemDiamondHandler;
		case E_ITEM_DIAMOND_AXE:                     return ItemDiamondAxeHandler;
		case E_ITEM_DIAMOND_BOOTS:                   return ItemDiamondBootsHandler;
		case E_ITEM_DIAMOND_CHESTPLATE:              return ItemDiamondChestplateHandler;
		case E_ITEM_DIAMOND_HELMET:                  return ItemDiamondHelmetHandler;
		case E_ITEM_DIAMOND_HOE:                     return ItemDiamondHoeHandler;
		case E_ITEM_DIAMOND_HORSE_ARMOR:             return ItemDiamondHorseArmorHandler;
		case E_ITEM_DIAMOND_LEGGINGS:                return ItemDiamondLeggingsHandler;
		case E_ITEM_DIAMOND_PICKAXE:                 return ItemDiamondPickaxeHandler;
		case E_ITEM_DIAMOND_SHOVEL:                  return ItemDiamondShovelHandler;
		case E_ITEM_DIAMOND_SWORD:                   return ItemDiamondSwordHandler;
		case E_ITEM_DRAGON_BREATH:                   return ItemDragonBreathHandler;
		case E_ITEM_DYE:                             return ItemDyeHandler;
		case E_ITEM_EGG:                             return ItemEggHandler;
		case E_ITEM_ELYTRA:                          return ItemElytraHandler;
		case E_ITEM_EMERALD:                         return ItemEmeraldHandler;
		case E_ITEM_EMPTY_MAP:                       return ItemEmptyMapHandler;
		case E_ITEM_ENCHANTED_BOOK:                  return ItemEnchantedBookHandler;
		case E_ITEM_ENDER_PEARL:                     return ItemEnderPearlHandler;
		case E_ITEM_END_CRYSTAL:                     return ItemEndCrystalHandler;
		case E_ITEM_EYE_OF_ENDER:                    return ItemEyeOfEnderHandler;
		case E_ITEM_FAR_DISC:                        return ItemFarDiscHandler;
		case E_ITEM_FEATHER:                         return ItemFeatherHandler;
		case E_ITEM_FERMENTED_SPIDER_EYE:            return ItemFermentedSpiderEyeHandler;
		case E_ITEM_FIREWORK_ROCKET:                 return ItemFireworkRocketHandler;
		case E_ITEM_FIREWORK_STAR:                   return ItemFireworkStarHandler;
		case E_ITEM_FIRE_CHARGE:                     return ItemFireChargeHandler;
		case E_ITEM_FISHING_ROD:                     return ItemFishingRodHandler;
		case E_ITEM_FLINT:                           return ItemFlintHandler;
		case E_ITEM_FLINT_AND_STEEL:                 return ItemFlintAndSteelHandler;
		case E_ITEM_FLOWER_POT:                      return ItemFlowerPotHandler;
		case E_ITEM_FURNACE_MINECART:                return ItemFurnaceMinecartHandler;
		case E_ITEM_GHAST_TEAR:                      return ItemGhastTearHandler;
		case E_ITEM_GLASS_BOTTLE:                    return ItemGlassBottleHandler;
		case E_ITEM_GLISTERING_MELON:                return ItemGlisteringMelonHandler;
		case E_ITEM_GLOWSTONE_DUST:                  return ItemGlowstoneDustHandler;
		case E_ITEM_GOLD:                            return ItemGoldHandler;
		case E_ITEM_GOLDEN_APPLE:                    return ItemGoldenAppleHandler;
		case E_ITEM_GOLDEN_CARROT:                   return ItemGoldenCarrotHandler;
		case E_ITEM_GOLD_AXE:                        return ItemGoldenAxeHandler;
		case E_ITEM_GOLD_BOOTS:                      return ItemGoldenBootsHandler;
		case E_ITEM_GOLD_CHESTPLATE:                 return ItemGoldenChestplateHandler;
		case E_ITEM_GOLD_HELMET:                     return ItemGoldenHelmetHandler;
		case E_ITEM_GOLD_HOE:                        return ItemGoldenHoeHandler;
		case E_ITEM_GOLD_HORSE_ARMOR:                return ItemGoldenHorseArmorHandler;
		case E_ITEM_GOLD_LEGGINGS:                   return ItemGoldenLeggingsHandler;
		case E_ITEM_GOLD_NUGGET:                     return ItemGoldNuggetHandler;
		case E_ITEM_GOLD_PICKAXE:                    return ItemGoldenPickaxeHandler;
		case E_ITEM_GOLD_SHOVEL:                     return ItemGoldenShovelHandler;
		case E_ITEM_GOLD_SWORD:                      return ItemGoldenSwordHandler;
		case E_ITEM_GUNPOWDER:                       return ItemGunpowderHandler;
		case E_ITEM_HEAD:                            return ItemHeadHandler;
		case E_ITEM_IRON:                            return ItemIronHandler;
		case E_ITEM_IRON_AXE:                        return ItemIronAxeHandler;
		case E_ITEM_IRON_BOOTS:                      return ItemIronBootsHandler;
		case E_ITEM_IRON_CHESTPLATE:                 return ItemIronChestplateHandler;
		case E_ITEM_IRON_DOOR:                       return ItemIronDoorHandler;
		case E_ITEM_IRON_HELMET:                     return ItemIronHelmetHandler;
		case E_ITEM_IRON_HOE:                        return ItemIronHoeHandler;
		case E_ITEM_IRON_HORSE_ARMOR:                return ItemIronHorseArmorHandler;
		case E_ITEM_IRON_LEGGINGS:                   return ItemIronLeggingsHandler;
		case E_ITEM_IRON_NUGGET:                     return ItemIronNuggetHandler;
		case E_ITEM_IRON_PICKAXE:                    return ItemIronPickaxeHandler;
		case E_ITEM_IRON_SHOVEL:                     return ItemIronShovelHandler;
		case E_ITEM_IRON_SWORD:                      return ItemIronSwordHandler;
		case E_ITEM_ITEM_FRAME:                      return ItemItemFrameHandler;
		case E_ITEM_JUNGLE_BOAT:                     return ItemJungleBoatHandler;
		case E_ITEM_JUNGLE_DOOR:                     return ItemJungleDoorHandler;
		case E_ITEM_LAVA_BUCKET:                     return ItemLavaBucketHandler;
		case E_ITEM_LEASH:                           return ItemLeashHandler;
		case E_ITEM_LEATHER:                         return ItemLeatherHandler;
		case E_ITEM_LEATHER_BOOTS:                   return ItemLeatherBootsHandler;
		case E_ITEM_LEATHER_CAP:                     return ItemLeatherCapHandler;
		case E_ITEM_LEATHER_PANTS:                   return ItemLeatherPantsHandler;
		case E_ITEM_LEATHER_TUNIC:                   return ItemLeatherTunicHandler;
		case E_ITEM_LINGERING_POTION:                return ItemLingeringPotionHandler;
		case E_ITEM_MAGMA_CREAM:                     return ItemMagmaCreamHandler;
		case E_ITEM_MALL_DISC:                       return ItemMallDiscHandler;
		case E_ITEM_MAP:                             return ItemMapHandler;
		case E_ITEM_MELLOHI_DISC:                    return ItemMellohiDiscHandler;
		case E_ITEM_MELON_SEEDS:                     return ItemMelonSeedsHandler;
		case E_ITEM_MELON_SLICE:                     return ItemMelonSliceHandler;
		case E_ITEM_MILK:                            return ItemMilkHandler;
		case E_ITEM_MINECART:                        return ItemMinecartHandler;
		case E_ITEM_MINECART_WITH_COMMAND_BLOCK:     return ItemMinecartWithCommandBlockHandler;
		case E_ITEM_MINECART_WITH_HOPPER:            return ItemMinecartWithHopperHandler;
		case E_ITEM_MINECART_WITH_TNT:               return ItemMinecartWithTNTHandler;
		case E_ITEM_MUSHROOM_SOUP:                   return ItemMushroomSoupHandler;
		case E_ITEM_NAME_TAG:                        return ItemNameTagHandler;
		case E_ITEM_NETHER_BRICK:                    return ItemNetherBrickHandler;
		case E_ITEM_NETHER_QUARTZ:                   return ItemNetherQuartzHandler;
		case E_ITEM_NETHER_STAR:                     return ItemNetherStarHandler;
		case E_ITEM_NETHER_WART:                     return ItemNetherWartHandler;
		case E_ITEM_PAINTING:                        return ItemPaintingHandler;
		case E_ITEM_PAPER:                           return ItemPaperHandler;
		case E_ITEM_POISONOUS_POTATO:                return ItemPoisonousPotatoHandler;
		case E_ITEM_POPPED_CHORUS_FRUIT:             return ItemPoppedChorusFruitHandler;
		case E_ITEM_POTATO:                          return ItemPotatoHandler;
		case E_ITEM_POTION:                          return ItemPotionHandler;
		case E_ITEM_PRISMARINE_CRYSTALS:             return ItemPrismarineCrystalsHandler;
		case E_ITEM_PRISMARINE_SHARD:                return ItemPrismarineShardHandler;
		case E_ITEM_PUMPKIN_PIE:                     return ItemPumpkinPieHandler;
		case E_ITEM_PUMPKIN_SEEDS:                   return ItemPumpkinSeedsHandler;
		case E_ITEM_RABBITS_FOOT:                    return ItemRabbitsFootHandler;
		case E_ITEM_RABBIT_HIDE:                     return ItemRabbitHideHandler;
		case E_ITEM_RABBIT_STEW:                     return ItemRabbitStewHandler;
		case E_ITEM_RAW_BEEF:                        return ItemRawBeefHandler;
		case E_ITEM_RAW_CHICKEN:                     return ItemRawChickenHandler;
		case E_ITEM_RAW_FISH:                        return ItemRawFishHandler;
		case E_ITEM_RAW_MUTTON:                      return ItemRawMuttonHandler;
		case E_ITEM_RAW_PORKCHOP:                    return ItemRawPorkchopHandler;
		case E_ITEM_RAW_RABBIT:                      return ItemRawRabbitHandler;
		case E_ITEM_REDSTONE_DUST:                   return ItemRedstoneDustHandler;
		case E_ITEM_REDSTONE_REPEATER:               return ItemRedstoneRepeaterHandler;
		case E_ITEM_RED_APPLE:                       return ItemAppleHandler;
		case E_ITEM_ROTTEN_FLESH:                    return ItemRottenFleshHandler;
		case E_ITEM_SADDLE:                          return ItemSaddleHandler;
		case E_ITEM_SEEDS:                           return ItemSeedsHandler;
		case E_ITEM_SHEARS:                          return ItemShearsHandler;
		case E_ITEM_SHIELD:                          return ItemShieldHandler;
		case E_ITEM_SHULKER_SHELL:                   return ItemShulkerShellHandler;
		case E_ITEM_SIGN:                            return ItemSignHandler;
		case E_ITEM_SLIMEBALL:                       return ItemSlimeballHandler;
		case E_ITEM_SNOWBALL:                        return ItemSnowballHandler;
		case E_ITEM_SPAWN_EGG:                       return ItemSpawnEggHandler;
		case E_ITEM_SPECTRAL_ARROW:                  return ItemSpectralArrowHandler;
		case E_ITEM_SPIDER_EYE:                      return ItemSpiderEyeHandler;
		case E_ITEM_SPLASH_POTION:                   return ItemSplashPotionHandler;
		case E_ITEM_SPRUCE_BOAT:                     return ItemSpruceBoatHandler;
		case E_ITEM_SPRUCE_DOOR:                     return ItemSpruceDoorHandler;
		case E_ITEM_STAL_DISC:                       return ItemStalDiscHandler;
		case E_ITEM_STEAK:                           return ItemCookedBeefHandler;
		case E_ITEM_STICK:                           return ItemStickHandler;
		case E_ITEM_STONE_AXE:                       return ItemStoneAxeHandler;
		case E_ITEM_STONE_HOE:                       return ItemStoneHoeHandler;
		case E_ITEM_STONE_PICKAXE:                   return ItemStonePickaxeHandler;
		case E_ITEM_STONE_SHOVEL:                    return ItemStoneShovelHandler;
		case E_ITEM_STONE_SWORD:                     return ItemStoneSwordHandler;
		case E_ITEM_STRAD_DISC:                      return ItemStradDiscHandler;
		case E_ITEM_STRING:                          return ItemStringHandler;
		case E_ITEM_SUGAR:                           return ItemSugarHandler;
		case E_ITEM_SUGAR_CANE:                      return ItemSugarCaneHandler;
		case E_ITEM_TIPPED_ARROW:                    return ItemTippedArrowHandler;
		case E_ITEM_TOTEM_OF_UNDYING:                return ItemTotemOfUndyingHandler;
		case E_ITEM_WAIT_DISC:                       return ItemWaitDiscHandler;
		case E_ITEM_WARD_DISC:                       return ItemWardDiscHandler;
		case E_ITEM_WATER_BUCKET:                    return ItemWaterBucketHandler;
		case E_ITEM_WHEAT:                           return ItemWheatHandler;
		case E_ITEM_WOODEN_AXE:                      return ItemWoodenAxeHandler;
		case E_ITEM_WOODEN_DOOR:                     return ItemOakDoorHandler;
		case E_ITEM_WOODEN_HOE:                      return ItemWoodenHoeHandler;
		case E_ITEM_WOODEN_PICKAXE:                  return ItemWoodenPickaxeHandler;
		case E_ITEM_WOODEN_SHOVEL:                   return ItemWoodenShovelHandler;
		case E_ITEM_WOODEN_SWORD:                    return ItemWoodenSwordHandler;
		case E_ITEM_WRITTEN_BOOK:                    return ItemWrittenBookHandler;

	}
	ASSERT("Unknown item type!");
	return ItemAirHandler;
}





void cItemHandler::OnPlayerPlace(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_ClickedPosition, const BLOCKTYPE a_ClickedBlockType, const NIBBLETYPE a_ClickedBlockMeta, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const
{
	const auto & World = *a_Player.GetWorld();

	// Check if the block ignores build collision (water, grass etc.):
	if (cBlockHandler::For(a_ClickedBlockType).DoesIgnoreBuildCollision(World, a_HeldItem, a_ClickedPosition, a_ClickedBlockMeta, a_ClickedBlockFace, true))
	{
		// Try to place the block at the clicked position:
		if (!CommitPlacement(a_Player, a_HeldItem, a_ClickedPosition, a_ClickedBlockFace, a_CursorPosition))
		{
			// The placement failed, the blocks have already been re-sent, re-send inventory:
			a_Player.GetInventory().SendEquippedSlot();
			return;
		}
	}
	else
	{
		BLOCKTYPE PlaceBlock;
		NIBBLETYPE PlaceMeta;
		const auto PlacePosition = AddFaceDirection(a_ClickedPosition, a_ClickedBlockFace);

		if (!cChunkDef::IsValidHeight(PlacePosition) || !World.GetBlockTypeMeta(PlacePosition, PlaceBlock, PlaceMeta))
		{
			// The block is being placed outside the world, ignore this packet altogether (GH #128):
			return;
		}

		// Clicked on side of block, make sure that placement won't be cancelled if there is a slab able to be double slabbed.
		// No need to do combinability (dblslab) checks, client will do that here.
		if (!cBlockHandler::For(PlaceBlock).DoesIgnoreBuildCollision(World, a_HeldItem, PlacePosition, PlaceMeta, a_ClickedBlockFace, false))
		{
			// Tried to place a block into another?
			// Happens when you place a block aiming at side of block with a torch on it or stem beside it.
			a_Player.SendBlocksAround(PlacePosition, 2);
			a_Player.GetInventory().SendEquippedSlot();
			return;
		}

		// Try to place the block:
		if (!CommitPlacement(a_Player, a_HeldItem, PlacePosition, a_ClickedBlockFace, a_CursorPosition))
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
	const Vector3i a_ClickedBlockPos, eBlockFace a_ClickedBlockFace
) const
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
) const
{
	UNUSED(a_World);
	UNUSED(a_Player);
	UNUSED(a_HeldItem);
	UNUSED(a_ClickedBlockPos);
	UNUSED(a_ClickedBlockFace);

	return false;
}





void cItemHandler::OnEntityAttack(cPlayer * a_Attacker, cEntity * a_AttackedEntity) const
{
	UNUSED(a_AttackedEntity);
	a_Attacker->UseEquippedItem(dlaAttackEntity);
}





void cItemHandler::OnFoodEaten(cWorld * a_World, cPlayer * a_Player, cItem * a_Item) const
{
	UNUSED(a_World);
	UNUSED(a_Player);
	UNUSED(a_Item);
}





short cItemHandler::GetDurabilityLossByAction(eDurabilityLostAction a_Action) const
{
	UNUSED(a_Action);

	return 0;
}





char cItemHandler::GetMaxStackSize(void) const
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





bool cItemHandler::IsFood(void) const
{
	return false;
}





bool cItemHandler::IsDrinkable(short a_ItemDamage) const
{
	UNUSED(a_ItemDamage);

	return false;
}





bool cItemHandler::IsPlaceable(void) const
{
	// We can place any block that has a corresponding E_BLOCK_TYPE:
	return (m_ItemType >= 1) && (m_ItemType <= E_BLOCK_MAX_TYPE_ID);
}





bool cItemHandler::CanRepairWithRawMaterial(short a_ItemType) const
{
	UNUSED(a_ItemType);
	return false;
}





bool cItemHandler::CanHarvestBlock(BLOCKTYPE a_BlockType) const
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





bool cItemHandler::EatItem(cPlayer * a_Player, cItem * a_Item) const
{
	auto FoodInfo = GetFoodInfo(a_Item);
	return a_Player->Feed(FoodInfo.FoodLevel, FoodInfo.Saturation);
}





cItemHandler::FoodInfo cItemHandler::GetFoodInfo(const cItem * a_Item) const
{
	UNUSED(a_Item);
	return FoodInfo(0, 0);
}





float cItemHandler::GetBlockBreakingStrength(BLOCKTYPE a_Block) const
{
	return 1.0f;
}





bool cItemHandler::CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const
{
	ASSERT(m_ItemType < 256);  // Items with IDs above 255 should all be handled by specific handlers.

	// By default, all blocks can be placed and the meta is copied over from the item's damage value:
	return a_Player.PlaceBlock(
		a_PlacePosition,
		static_cast<BLOCKTYPE>(m_ItemType),
		static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage & 0x0f)
	);
}
