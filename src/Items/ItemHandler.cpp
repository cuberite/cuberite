
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
#include "ItemDefaultItem.h"
#include "ItemDoor.h"
#include "ItemDropSpenser.h"
#include "ItemDye.h"
#include "ItemEmptyMap.h"
#include "ItemEnchantingTable.h"
#include "ItemEndCrystal.h"
#include "ItemEndPortalFrame.h"
#include "ItemEnderChest.h"
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
#include "ItemChicken.h"
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
#include "ItemVines.h"
#include "ItemWool.h"
#include "ItemFence.h"
#include "ItemWood.h"

#include "../Blocks/BlockHandler.h"
#include "SimplePlaceableItemHandler.h"
#include "../Registries/BlockItemConverter.h"
#include "../Protocol/Palettes/Upgrade.h"


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
	constexpr cItemBoatHandler              ItemAcaciaBoatHandler                      (Item::AcaciaBoat);
	constexpr cItemButtonHandler            ItemAcaciaButtonHandler                    (Item::AcaciaButton);
	constexpr cItemDoorHandler              ItemAcaciaDoorHandler                      (Item::AcaciaDoor);
	constexpr cItemFenceGateHandler         ItemAcaciaFenceGateHandler                 (Item::AcaciaFenceGate);
	constexpr cItemFenceHandler             ItemAcaciaFenceHandler                     (Item::AcaciaFence);
	constexpr cItemLeavesHandler            ItemAcaciaLeavesHandler                    (Item::AcaciaLeaves);
	constexpr cItemLogHandler               ItemAcaciaLogHandler                       (Item::AcaciaLog);
	constexpr cSimplePlaceableItemHandler   ItemAcaciaPlanksHandler                    (Item::AcaciaPlanks);
	constexpr cSimplePlaceableItemHandler   ItemAcaciaPressurePlateHandler             (Item::AcaciaPressurePlate);
	constexpr cSimplePlaceableItemHandler   ItemAcaciaSaplingHandler                   (Item::AcaciaSapling);
	constexpr cItemSignHandler              ItemAcaciaSignHandler                      (Item::AcaciaSign);
	constexpr cItemSlabHandler              ItemAcaciaSlabHandler                      (Item::AcaciaSlab);
	constexpr cItemStairsHandler            ItemAcaciaStairsHandler                    (Item::AcaciaStairs);
	constexpr cItemTrapdoorHandler          ItemAcaciaTrapdoorHandler                  (Item::AcaciaTrapdoor);
	constexpr cItemWoodHandler              ItemAcaciaWoodHandler                      (Item::AcaciaWood);
	constexpr cItemRailHandler              ItemActivatorRailHandler                   (Item::ActivatorRail);
	constexpr cDefaultItemHandler           ItemAirHandler                             (Item::Air);
	constexpr cUnimplementedItemHandler     ItemAlliumHandler                          (Item::Allium);
	constexpr cUnimplementedItemHandler     ItemAncientDebrisHandler                   (Item::AncientDebris);
	constexpr cSimplePlaceableItemHandler   ItemAndesiteHandler                        (Item::Andesite);
	constexpr cItemSlabHandler              ItemAndesiteSlabHandler                    (Item::AndesiteSlab);
	constexpr cItemStairsHandler            ItemAndesiteStairsHandler                  (Item::AndesiteStairs);
	constexpr cUnimplementedItemHandler     ItemAndesiteWallHandler                    (Item::AndesiteWall);
	constexpr cItemAnvilHandler             ItemAnvilHandler                           (Item::Anvil);
	constexpr cItemSimpleFoodHandler        ItemAppleHandler                           (Item::Apple, cItemHandler::FoodInfo(4, 2.4));
	constexpr cUnimplementedItemHandler     ItemArmorStandHandler                      (Item::ArmorStand);
	constexpr cDefaultItemHandler           ItemArrowHandler                           (Item::Arrow);
	constexpr cUnimplementedItemHandler     ItemAzureBluetHandler                      (Item::AzureBluet);
	constexpr cItemSimpleFoodHandler        ItemBakedPotatoHandler                     (Item::BakedPotato, cItemHandler::FoodInfo(5, 6));
	constexpr cUnimplementedItemHandler     ItemBambooHandler                          (Item::Bamboo);
	constexpr cUnimplementedItemHandler     ItemBarrelHandler                          (Item::Barrel);
	constexpr cDefaultItemHandler           ItemBarrierHandler                         (Item::Barrier);
	constexpr cUnimplementedItemHandler     ItemBasaltHandler                          (Item::Basalt);
	constexpr cItemSpawnEggHandler          ItemBatSpawnEggHandler                     (Item::BatSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemBeaconHandler                          (Item::Beacon);
	constexpr cSimplePlaceableItemHandler   ItemBedrockHandler                         (Item::Bedrock);
	constexpr cUnimplementedItemHandler     ItemBeeNestHandler                         (Item::BeeNest);
	constexpr cItemSpawnEggHandler          ItemBeeSpawnEggHandler                     (Item::BeeSpawnEgg);
	constexpr cItemSimpleFoodHandler        ItemBeefHandler                            (Item::Beef, cItemHandler::FoodInfo(3, 1.8));
	constexpr cUnimplementedItemHandler     ItemBeehiveHandler                         (Item::Beehive);
	constexpr cItemSimpleFoodHandler        ItemBeetrootHandler                        (Item::Beetroot, cItemHandler::FoodInfo(1, 1.2));
	constexpr cItemSimpleSeedsHandler       ItemBeetrootSeedsHandler                   (Item::BeetrootSeeds);
	constexpr cItemSoupHandler              ItemBeetrootSoupHandler                    (Item::BeetrootSoup, cItemHandler::FoodInfo(6, 7.2));
	constexpr cUnimplementedItemHandler     ItemBellHandler                            (Item::Bell);
	constexpr cItemBoatHandler              ItemBirchBoatHandler                       (Item::BirchBoat);
	constexpr cItemButtonHandler            ItemBirchButtonHandler                     (Item::BirchButton);
	constexpr cItemDoorHandler              ItemBirchDoorHandler                       (Item::BirchDoor);
	constexpr cItemFenceGateHandler         ItemBirchFenceGateHandler                  (Item::BirchFenceGate);
	constexpr cItemFenceHandler             ItemBirchFenceHandler                      (Item::BirchFence);
	constexpr cItemLeavesHandler            ItemBirchLeavesHandler                     (Item::BirchLeaves);
	constexpr cItemLogHandler               ItemBirchLogHandler                        (Item::BirchLog);
	constexpr cSimplePlaceableItemHandler   ItemBirchPlanksHandler                     (Item::BirchPlanks);
	constexpr cSimplePlaceableItemHandler   ItemBirchPressurePlateHandler              (Item::BirchPressurePlate);
	constexpr cSimplePlaceableItemHandler   ItemBirchSaplingHandler                    (Item::BirchSapling);
	constexpr cItemSignHandler              ItemBirchSignHandler                       (Item::BirchSign);
	constexpr cItemSlabHandler              ItemBirchSlabHandler                       (Item::BirchSlab);
	constexpr cItemStairsHandler            ItemBirchStairsHandler                     (Item::BirchStairs);
	constexpr cItemTrapdoorHandler          ItemBirchTrapdoorHandler                   (Item::BirchTrapdoor);
	constexpr cItemWoodHandler              ItemBirchWoodHandler                       (Item::BirchWood);
	constexpr cItemBannerHandler            ItemBlackBannerHandler                     (Item::BlackBanner);
	constexpr cItemBedHandler               ItemBlackBedHandler                        (Item::BlackBed);
	constexpr cSimplePlaceableItemHandler   ItemBlackCarpetHandler                     (Item::BlackCarpet);
	constexpr cUnimplementedItemHandler     ItemBlackConcreteHandler                   (Item::BlackConcrete);
	constexpr cUnimplementedItemHandler     ItemBlackConcretePowderHandler             (Item::BlackConcretePowder);
	constexpr cItemDyeHandler               ItemBlackDyeHandler                        (Item::BlackDye);
	constexpr cItemGlazedTerracottaHandler  ItemBlackGlazedTerracottaHandler           (Item::BlackGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemBlackShulkerBoxHandler                 (Item::BlackShulkerBox);
	constexpr cSimplePlaceableItemHandler   ItemBlackStainedGlassHandler               (Item::BlackStainedGlass);
	constexpr cUnimplementedItemHandler     ItemBlackStainedGlassPaneHandler           (Item::BlackStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemBlackTerracottaHandler                 (Item::BlackTerracotta);
	constexpr cItemWoolHandler              ItemBlackWoolHandler                       (Item::BlackWool);
	constexpr cUnimplementedItemHandler     ItemBlackstoneHandler                      (Item::Blackstone);
	constexpr cItemSlabHandler              ItemBlackstoneSlabHandler                  (Item::BlackstoneSlab);
	constexpr cItemStairsHandler            ItemBlackstoneStairsHandler                (Item::BlackstoneStairs);
	constexpr cUnimplementedItemHandler     ItemBlackstoneWallHandler                  (Item::BlackstoneWall);
	constexpr cUnimplementedItemHandler     ItemBlastFurnaceHandler                    (Item::BlastFurnace);
	constexpr cDefaultItemHandler           ItemBlazePowderHandler                     (Item::BlazePowder);
	constexpr cDefaultItemHandler           ItemBlazeRodHandler                        (Item::BlazeRod);
	constexpr cItemSpawnEggHandler          ItemBlazeSpawnEggHandler                   (Item::BlazeSpawnEgg);
	constexpr cItemBannerHandler            ItemBlueBannerHandler                      (Item::BlueBanner);
	constexpr cItemBedHandler               ItemBlueBedHandler                         (Item::BlueBed);
	constexpr cSimplePlaceableItemHandler   ItemBlueCarpetHandler                      (Item::BlueCarpet);
	constexpr cUnimplementedItemHandler     ItemBlueConcreteHandler                    (Item::BlueConcrete);
	constexpr cUnimplementedItemHandler     ItemBlueConcretePowderHandler              (Item::BlueConcretePowder);
	constexpr cItemDyeHandler               ItemBlueDyeHandler                         (Item::BlueDye);
	constexpr cItemGlazedTerracottaHandler  ItemBlueGlazedTerracottaHandler            (Item::BlueGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemBlueIceHandler                         (Item::BlueIce);
	constexpr cUnimplementedItemHandler     ItemBlueOrchidHandler                      (Item::BlueOrchid);
	constexpr cUnimplementedItemHandler     ItemBlueShulkerBoxHandler                  (Item::BlueShulkerBox);
	constexpr cSimplePlaceableItemHandler   ItemBlueStainedGlassHandler                (Item::BlueStainedGlass);
	constexpr cUnimplementedItemHandler     ItemBlueStainedGlassPaneHandler            (Item::BlueStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemBlueTerracottaHandler                  (Item::BlueTerracotta);
	constexpr cItemWoolHandler              ItemBlueWoolHandler                        (Item::BlueWool);
	constexpr cUnimplementedItemHandler     ItemBoneBlockHandler                       (Item::BoneBlock);
	constexpr cDefaultItemHandler           ItemBoneHandler                            (Item::Bone);
	constexpr cItemDyeHandler               ItemBoneMealHandler                        (Item::BoneMeal);
	constexpr cDefaultItemHandler           ItemBookHandler                            (Item::Book);
	constexpr cSimplePlaceableItemHandler   ItemBookshelfHandler                       (Item::Bookshelf);
	constexpr cItemBowHandler               ItemBowHandler                             (Item::Bow);
	constexpr cDefaultItemHandler           ItemBowlHandler                            (Item::Bowl);
	constexpr cUnimplementedItemHandler     ItemBrainCoralBlockHandler                 (Item::BrainCoralBlock);
	constexpr cUnimplementedItemHandler     ItemBrainCoralFanHandler                   (Item::BrainCoralFan);
	constexpr cUnimplementedItemHandler     ItemBrainCoralHandler                      (Item::BrainCoral);
	constexpr cItemSimpleFoodHandler        ItemBreadHandler                           (Item::Bread, cItemHandler::FoodInfo(5, 6));
	constexpr cSimplePlaceableItemHandler   ItemBrewingStandHandler                    (Item::BrewingStand);
	constexpr cDefaultItemHandler           ItemBrickHandler                           (Item::Brick);
	constexpr cItemSlabHandler              ItemBrickSlabHandler                       (Item::BrickSlab);
	constexpr cItemStairsHandler            ItemBrickStairsHandler                     (Item::BrickStairs);
	constexpr cUnimplementedItemHandler     ItemBrickWallHandler                       (Item::BrickWall);
	constexpr cSimplePlaceableItemHandler   ItemBricksHandler                          (Item::Bricks);
	constexpr cItemBannerHandler            ItemBrownBannerHandler                     (Item::BrownBanner);
	constexpr cItemBedHandler               ItemBrownBedHandler                        (Item::BrownBed);
	constexpr cSimplePlaceableItemHandler   ItemBrownCarpetHandler                     (Item::BrownCarpet);
	constexpr cUnimplementedItemHandler     ItemBrownConcreteHandler                   (Item::BrownConcrete);
	constexpr cUnimplementedItemHandler     ItemBrownConcretePowderHandler             (Item::BrownConcretePowder);
	constexpr cItemDyeHandler               ItemBrownDyeHandler                        (Item::BrownDye);
	constexpr cItemGlazedTerracottaHandler  ItemBrownGlazedTerracottaHandler           (Item::BrownGlazedTerracotta);
	constexpr cSimplePlaceableItemHandler   ItemBrownMushroomBlockHandler              (Item::BrownMushroomBlock);
	constexpr cDefaultItemHandler           ItemBrownMushroomHandler                   (Item::BrownMushroom);
	constexpr cUnimplementedItemHandler     ItemBrownShulkerBoxHandler                 (Item::BrownShulkerBox);
	constexpr cSimplePlaceableItemHandler   ItemBrownStainedGlassHandler               (Item::BrownStainedGlass);
	constexpr cUnimplementedItemHandler     ItemBrownStainedGlassPaneHandler           (Item::BrownStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemBrownTerracottaHandler                 (Item::BrownTerracotta);
	constexpr cItemWoolHandler              ItemBrownWoolHandler                       (Item::BrownWool);
	constexpr cUnimplementedItemHandler     ItemBubbleCoralBlockHandler                (Item::BubbleCoralBlock);
	constexpr cUnimplementedItemHandler     ItemBubbleCoralFanHandler                  (Item::BubbleCoralFan);
	constexpr cUnimplementedItemHandler     ItemBubbleCoralHandler                     (Item::BubbleCoral);
	constexpr cItemBucketHandler            ItemBucketHandler                          (Item::Bucket);
	constexpr cSimplePlaceableItemHandler   ItemCactusHandler                          (Item::Cactus);
	constexpr cSimplePlaceableItemHandler   ItemCakeHandler                            (Item::Cake);
	constexpr cUnimplementedItemHandler     ItemCampfireHandler                        (Item::Campfire);
	constexpr cItemFoodSeedsHandler         ItemCarrotHandler                          (Item::Carrot, cItemHandler::FoodInfo(3, 6.6));
	constexpr cDefaultItemHandler           ItemCarrotOnAStickHandler                  (Item::CarrotOnAStick);
	constexpr cUnimplementedItemHandler     ItemCartographyTableHandler                (Item::CartographyTable);
	constexpr cUnimplementedItemHandler     ItemCarvedPumpkinHandler                   (Item::CarvedPumpkin);
	constexpr cItemSpawnEggHandler          ItemCatSpawnEggHandler                     (Item::CatSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemCauldronHandler                        (Item::Cauldron);
	constexpr cItemSpawnEggHandler          ItemCaveSpiderSpawnEggHandler              (Item::CaveSpiderSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemChainCommandBlockHandler               (Item::ChainCommandBlock);
	constexpr cUnimplementedItemHandler     ItemChainHandler                           (Item::Chain);
	constexpr cItemArmorHandler             ItemChainmailBootsHandler                  (Item::ChainmailBoots);
	constexpr cItemArmorHandler             ItemChainmailChestplateHandler             (Item::ChainmailChestplate);
	constexpr cItemArmorHandler             ItemChainmailHelmetHandler                 (Item::ChainmailHelmet);
	constexpr cItemArmorHandler             ItemChainmailLeggingsHandler               (Item::ChainmailLeggings);
	constexpr cDefaultItemHandler           ItemCharcoalHandler                        (Item::Charcoal);
	constexpr cItemChestHandler             ItemChestHandler                           (Item::Chest);
	constexpr cItemMinecartHandler          ItemChestMinecartHandler                   (Item::ChestMinecart);
	constexpr cItemChickenHandler           ItemChickenHandler                         (Item::Chicken);
	constexpr cItemSpawnEggHandler          ItemChickenSpawnEggHandler                 (Item::ChickenSpawnEgg);
	constexpr cItemAnvilHandler             ItemChippedAnvilHandler                    (Item::ChippedAnvil);
	constexpr cUnimplementedItemHandler     ItemChiseledNetherBricksHandler            (Item::ChiseledNetherBricks);
	constexpr cUnimplementedItemHandler     ItemChiseledPolishedBlackstoneHandler      (Item::ChiseledPolishedBlackstone);
	constexpr cUnimplementedItemHandler     ItemChiseledQuartzBlockHandler             (Item::ChiseledQuartzBlock);
	constexpr cUnimplementedItemHandler     ItemChiseledRedSandstoneHandler            (Item::ChiseledRedSandstone);
	constexpr cUnimplementedItemHandler     ItemChiseledSandstoneHandler               (Item::ChiseledSandstone);
	constexpr cUnimplementedItemHandler     ItemChiseledStoneBricksHandler             (Item::ChiseledStoneBricks);
	constexpr cUnimplementedItemHandler     ItemChorusFlowerHandler                    (Item::ChorusFlower);
	constexpr cItemChorusFruitHandler       ItemChorusFruitHandler                     (Item::ChorusFruit);
	constexpr cUnimplementedItemHandler     ItemChorusPlantHandler                     (Item::ChorusPlant);
	constexpr cDefaultItemHandler           ItemClayBallHandler                        (Item::ClayBall);
	constexpr cSimplePlaceableItemHandler   ItemClayHandler                            (Item::Clay);
	constexpr cDefaultItemHandler           ItemClockHandler                           (Item::Clock);
	constexpr cSimplePlaceableItemHandler   ItemCoalBlockHandler                       (Item::CoalBlock);
	constexpr cDefaultItemHandler           ItemCoalHandler                            (Item::Coal);
	constexpr cSimplePlaceableItemHandler   ItemCoalOreHandler                         (Item::CoalOre);
	constexpr cSimplePlaceableItemHandler   ItemCoarseDirtHandler                      (Item::CoarseDirt);
	constexpr cSimplePlaceableItemHandler   ItemCobblestoneHandler                     (Item::Cobblestone);
	constexpr cItemSlabHandler              ItemCobblestoneSlabHandler                 (Item::CobblestoneSlab);
	constexpr cItemStairsHandler            ItemCobblestoneStairsHandler               (Item::CobblestoneStairs);
	constexpr cUnimplementedItemHandler     ItemCobblestoneWallHandler                 (Item::CobblestoneWall);
	constexpr cSimplePlaceableItemHandler   ItemCobwebHandler                          (Item::Cobweb);
	constexpr cDefaultItemHandler           ItemCocoaBeansHandler                      (Item::CocoaBeans);
	constexpr cUnimplementedItemHandler     ItemCodBucketHandler                       (Item::CodBucket);
	constexpr cItemRawFishHandler           ItemCodHandler                             (Item::Cod);
	constexpr cItemSpawnEggHandler          ItemCodSpawnEggHandler                     (Item::CodSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemCommandBlockHandler                    (Item::CommandBlock);
	constexpr cItemMinecartHandler          ItemCommandBlockMinecartHandler            (Item::CommandBlockMinecart);
	constexpr cItemComparatorHandler        ItemComparatorHandler                      (Item::Comparator);
	constexpr cDefaultItemHandler           ItemCompassHandler                         (Item::Compass);
	constexpr cUnimplementedItemHandler     ItemComposterHandler                       (Item::Composter);
	constexpr cUnimplementedItemHandler     ItemConduitHandler                         (Item::Conduit);
	constexpr cItemSimpleFoodHandler        ItemCookedBeefHandler                      (Item::CookedBeef, cItemHandler::FoodInfo(8, 12.8));
	constexpr cItemSimpleFoodHandler        ItemCookedChickenHandler                   (Item::CookedChicken, cItemHandler::FoodInfo(6, 7.2));
	constexpr cItemCookedFishHandler        ItemCookedCodHandler                       (Item::CookedCod);
	constexpr cItemSimpleFoodHandler        ItemCookedMuttonHandler                    (Item::CookedMutton, cItemHandler::FoodInfo(6, 9.6));
	constexpr cItemSimpleFoodHandler        ItemCookedPorkchopHandler                  (Item::CookedPorkchop, cItemHandler::FoodInfo(8, 12.8));
	constexpr cItemSimpleFoodHandler        ItemCookedRabbitHandler                    (Item::CookedRabbit, cItemHandler::FoodInfo(5, 6));
	constexpr cItemCookedFishHandler        ItemCookedSalmonHandler                    (Item::CookedSalmon);
	constexpr cItemSimpleFoodHandler        ItemCookieHandler                          (Item::Cookie, cItemHandler::FoodInfo(2, 0.5));
	constexpr cUnimplementedItemHandler     ItemCornflowerHandler                      (Item::Cornflower);
	constexpr cItemSpawnEggHandler          ItemCowSpawnEggHandler                     (Item::CowSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemCrackedNetherBricksHandler             (Item::CrackedNetherBricks);
	constexpr cUnimplementedItemHandler     ItemCrackedPolishedBlackstoneBricksHandler (Item::CrackedPolishedBlackstoneBricks);
	constexpr cUnimplementedItemHandler     ItemCrackedStoneBricksHandler              (Item::CrackedStoneBricks);
	constexpr cSimplePlaceableItemHandler   ItemCraftingTableHandler                   (Item::CraftingTable);
	constexpr cUnimplementedItemHandler     ItemCreeperBannerPatternHandler            (Item::CreeperBannerPattern);
	constexpr cItemMobHeadHandler           ItemCreeperHeadHandler                     (Item::CreeperHead);
	constexpr cItemSpawnEggHandler          ItemCreeperSpawnEggHandler                 (Item::CreeperSpawnEgg);
	constexpr cItemButtonHandler            ItemCrimsonButtonHandler                   (Item::CrimsonButton);
	constexpr cItemDoorHandler              ItemCrimsonDoorHandler                     (Item::CrimsonDoor);
	constexpr cItemFenceGateHandler         ItemCrimsonFenceGateHandler                (Item::CrimsonFenceGate);
	constexpr cItemFenceHandler             ItemCrimsonFenceHandler                    (Item::CrimsonFence);
	constexpr cUnimplementedItemHandler     ItemCrimsonFungusHandler                   (Item::CrimsonFungus);
	constexpr cUnimplementedItemHandler     ItemCrimsonHyphaeHandler                   (Item::CrimsonHyphae);
	constexpr cUnimplementedItemHandler     ItemCrimsonNyliumHandler                   (Item::CrimsonNylium);
	constexpr cSimplePlaceableItemHandler   ItemCrimsonPlanksHandler                   (Item::CrimsonPlanks);
	constexpr cSimplePlaceableItemHandler   ItemCrimsonPressurePlateHandler            (Item::CrimsonPressurePlate);
	constexpr cUnimplementedItemHandler     ItemCrimsonRootsHandler                    (Item::CrimsonRoots);
	constexpr cItemSignHandler              ItemCrimsonSignHandler                     (Item::CrimsonSign);
	constexpr cItemSlabHandler              ItemCrimsonSlabHandler                     (Item::CrimsonSlab);
	constexpr cItemStairsHandler            ItemCrimsonStairsHandler                   (Item::CrimsonStairs);
	constexpr cItemLogHandler               ItemCrimsonStemHandler                     (Item::CrimsonStem);
	constexpr cItemTrapdoorHandler          ItemCrimsonTrapdoorHandler                 (Item::CrimsonTrapdoor);
	constexpr cUnimplementedItemHandler     ItemCrossbowHandler                        (Item::Crossbow);
	constexpr cUnimplementedItemHandler     ItemCryingObsidianHandler                  (Item::CryingObsidian);
	constexpr cUnimplementedItemHandler     ItemCutRedSandstoneHandler                 (Item::CutRedSandstone);
	constexpr cItemSlabHandler              ItemCutRedSandstoneSlabHandler             (Item::CutRedSandstoneSlab);
	constexpr cUnimplementedItemHandler     ItemCutSandstoneHandler                    (Item::CutSandstone);
	constexpr cItemSlabHandler              ItemCutSandstoneSlabHandler                (Item::CutSandstoneSlab);
	constexpr cItemBannerHandler            ItemCyanBannerHandler                      (Item::CyanBanner);
	constexpr cItemBedHandler               ItemCyanBedHandler                         (Item::CyanBed);
	constexpr cSimplePlaceableItemHandler   ItemCyanCarpetHandler                      (Item::CyanCarpet);
	constexpr cUnimplementedItemHandler     ItemCyanConcreteHandler                    (Item::CyanConcrete);
	constexpr cUnimplementedItemHandler     ItemCyanConcretePowderHandler              (Item::CyanConcretePowder);
	constexpr cItemDyeHandler               ItemCyanDyeHandler                         (Item::CyanDye);
	constexpr cItemGlazedTerracottaHandler  ItemCyanGlazedTerracottaHandler            (Item::CyanGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemCyanShulkerBoxHandler                  (Item::CyanShulkerBox);
	constexpr cUnimplementedItemHandler     ItemCyanStainedGlassHandler                (Item::CyanStainedGlass);
	constexpr cUnimplementedItemHandler     ItemCyanStainedGlassPaneHandler            (Item::CyanStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemCyanTerracottaHandler                  (Item::CyanTerracotta);
	constexpr cItemWoolHandler              ItemCyanWoolHandler                        (Item::CyanWool);
	constexpr cItemAnvilHandler             ItemDamagedAnvilHandler                    (Item::DamagedAnvil);
	constexpr cUnimplementedItemHandler     ItemDandelionHandler                       (Item::Dandelion);
	constexpr cItemBoatHandler              ItemDarkOakBoatHandler                     (Item::DarkOakBoat);
	constexpr cItemButtonHandler            ItemDarkOakButtonHandler                   (Item::DarkOakButton);
	constexpr cItemDoorHandler              ItemDarkOakDoorHandler                     (Item::DarkOakDoor);
	constexpr cItemFenceGateHandler         ItemDarkOakFenceGateHandler                (Item::DarkOakFenceGate);
	constexpr cItemFenceHandler             ItemDarkOakFenceHandler                    (Item::DarkOakFence);
	constexpr cItemLeavesHandler            ItemDarkOakLeavesHandler                   (Item::DarkOakLeaves);
	constexpr cItemLogHandler               ItemDarkOakLogHandler                      (Item::DarkOakLog);
	constexpr cSimplePlaceableItemHandler   ItemDarkOakPlanksHandler                   (Item::DarkOakPlanks);
	constexpr cSimplePlaceableItemHandler   ItemDarkOakPressurePlateHandler            (Item::DarkOakPressurePlate);
	constexpr cSimplePlaceableItemHandler   ItemDarkOakSaplingHandler                  (Item::DarkOakSapling);
	constexpr cItemSignHandler              ItemDarkOakSignHandler                     (Item::DarkOakSign);
	constexpr cItemSlabHandler              ItemDarkOakSlabHandler                     (Item::DarkOakSlab);
	constexpr cItemStairsHandler            ItemDarkOakStairsHandler                   (Item::DarkOakStairs);
	constexpr cItemTrapdoorHandler          ItemDarkOakTrapdoorHandler                 (Item::DarkOakTrapdoor);
	constexpr cItemWoodHandler              ItemDarkOakWoodHandler                     (Item::DarkOakWood);
	constexpr cUnimplementedItemHandler     ItemDarkPrismarineHandler                  (Item::DarkPrismarine);
	constexpr cItemSlabHandler              ItemDarkPrismarineSlabHandler              (Item::DarkPrismarineSlab);
	constexpr cItemStairsHandler            ItemDarkPrismarineStairsHandler            (Item::DarkPrismarineStairs);
	constexpr cSimplePlaceableItemHandler   ItemDaylightDetectorHandler                (Item::DaylightDetector);
	constexpr cUnimplementedItemHandler     ItemDeadBrainCoralBlockHandler             (Item::DeadBrainCoralBlock);
	constexpr cUnimplementedItemHandler     ItemDeadBrainCoralFanHandler               (Item::DeadBrainCoralFan);
	constexpr cUnimplementedItemHandler     ItemDeadBrainCoralHandler                  (Item::DeadBrainCoral);
	constexpr cUnimplementedItemHandler     ItemDeadBubbleCoralBlockHandler            (Item::DeadBubbleCoralBlock);
	constexpr cUnimplementedItemHandler     ItemDeadBubbleCoralFanHandler              (Item::DeadBubbleCoralFan);
	constexpr cUnimplementedItemHandler     ItemDeadBubbleCoralHandler                 (Item::DeadBubbleCoral);
	constexpr cUnimplementedItemHandler     ItemDeadBushHandler                        (Item::DeadBush);
	constexpr cUnimplementedItemHandler     ItemDeadFireCoralBlockHandler              (Item::DeadFireCoralBlock);
	constexpr cUnimplementedItemHandler     ItemDeadFireCoralFanHandler                (Item::DeadFireCoralFan);
	constexpr cUnimplementedItemHandler     ItemDeadFireCoralHandler                   (Item::DeadFireCoral);
	constexpr cUnimplementedItemHandler     ItemDeadHornCoralBlockHandler              (Item::DeadHornCoralBlock);
	constexpr cUnimplementedItemHandler     ItemDeadHornCoralFanHandler                (Item::DeadHornCoralFan);
	constexpr cUnimplementedItemHandler     ItemDeadHornCoralHandler                   (Item::DeadHornCoral);
	constexpr cUnimplementedItemHandler     ItemDeadTubeCoralBlockHandler              (Item::DeadTubeCoralBlock);
	constexpr cUnimplementedItemHandler     ItemDeadTubeCoralFanHandler                (Item::DeadTubeCoralFan);
	constexpr cUnimplementedItemHandler     ItemDeadTubeCoralHandler                   (Item::DeadTubeCoral);
	constexpr cUnimplementedItemHandler     ItemDebugStickHandler                      (Item::DebugStick);
	constexpr cItemRailHandler              ItemDetectorRailHandler                    (Item::DetectorRail);
	constexpr cItemAxeHandler               ItemDiamondAxeHandler                      (Item::DiamondAxe);
	constexpr cSimplePlaceableItemHandler   ItemDiamondBlockHandler                    (Item::DiamondBlock);
	constexpr cItemArmorHandler             ItemDiamondBootsHandler                    (Item::DiamondBoots);
	constexpr cItemArmorHandler             ItemDiamondChestplateHandler               (Item::DiamondChestplate);
	constexpr cDefaultItemHandler           ItemDiamondHandler                         (Item::Diamond);
	constexpr cItemArmorHandler             ItemDiamondHelmetHandler                   (Item::DiamondHelmet);
	constexpr cItemHoeHandler               ItemDiamondHoeHandler                      (Item::DiamondHoe);
	constexpr cDefaultItemHandler           ItemDiamondHorseArmorHandler               (Item::DiamondHorseArmor);
	constexpr cItemArmorHandler             ItemDiamondLeggingsHandler                 (Item::DiamondLeggings);
	constexpr cSimplePlaceableItemHandler   ItemDiamondOreHandler                      (Item::DiamondOre);
	constexpr cItemPickaxeHandler           ItemDiamondPickaxeHandler                  (Item::DiamondPickaxe);
	constexpr cItemShovelHandler            ItemDiamondShovelHandler                   (Item::DiamondShovel);
	constexpr cItemSwordHandler             ItemDiamondSwordHandler                    (Item::DiamondSword);
	constexpr cSimplePlaceableItemHandler   ItemDioriteHandler                         (Item::Diorite);
	constexpr cItemSlabHandler              ItemDioriteSlabHandler                     (Item::DioriteSlab);
	constexpr cItemStairsHandler            ItemDioriteStairsHandler                   (Item::DioriteStairs);
	constexpr cUnimplementedItemHandler     ItemDioriteWallHandler                     (Item::DioriteWall);
	constexpr cSimplePlaceableItemHandler   ItemDirtHandler                            (Item::Dirt);
	constexpr cItemDropSpenserHandler       ItemDispenserHandler                       (Item::Dispenser);
	constexpr cItemSpawnEggHandler          ItemDolphinSpawnEggHandler                 (Item::DolphinSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemDonkeySpawnEggHandler                  (Item::DonkeySpawnEgg);
	constexpr cUnimplementedItemHandler     ItemDragonBreathHandler                    (Item::DragonBreath);
	constexpr cUnimplementedItemHandler     ItemDragonEggHandler                       (Item::DragonEgg);
	constexpr cItemMobHeadHandler           ItemDragonHeadHandler                      (Item::DragonHead);
	constexpr cUnimplementedItemHandler     ItemDriedKelpBlockHandler                  (Item::DriedKelpBlock);
	constexpr cUnimplementedItemHandler     ItemDriedKelpHandler                       (Item::DriedKelp);
	constexpr cItemDropSpenserHandler       ItemDropperHandler                         (Item::Dropper);
	constexpr cItemSpawnEggHandler          ItemDrownedSpawnEggHandler                 (Item::DrownedSpawnEgg);
	constexpr cItemEggHandler               ItemEggHandler                             (Item::Egg);
	constexpr cItemSpawnEggHandler          ItemElderGuardianSpawnEggHandler           (Item::ElderGuardianSpawnEgg);
	constexpr cItemArmorHandler             ItemElytraHandler                          (Item::Elytra);
	constexpr cSimplePlaceableItemHandler   ItemEmeraldBlockHandler                    (Item::EmeraldBlock);
	constexpr cDefaultItemHandler           ItemEmeraldHandler                         (Item::Emerald);
	constexpr cSimplePlaceableItemHandler   ItemEmeraldOreHandler                      (Item::EmeraldOre);
	constexpr cDefaultItemHandler           ItemEnchantedBookHandler                   (Item::EnchantedBook);
	constexpr cUnimplementedItemHandler     ItemEnchantedGoldenAppleHandler            (Item::EnchantedGoldenApple);
	constexpr cItemEnchantingTableHandler   ItemEnchantingTableHandler                 (Item::EnchantingTable);
	constexpr cItemEndCrystalHandler        ItemEndCrystalHandler                      (Item::EndCrystal);
	constexpr cItemEndPortalFrameHandler    ItemEndPortalFrameHandler                  (Item::EndPortalFrame);
	constexpr cUnimplementedItemHandler     ItemEndRodHandler                          (Item::EndRod);
	constexpr cItemSlabHandler              ItemEndStoneBrickSlabHandler               (Item::EndStoneBrickSlab);
	constexpr cItemStairsHandler            ItemEndStoneBrickStairsHandler             (Item::EndStoneBrickStairs);
	constexpr cUnimplementedItemHandler     ItemEndStoneBrickWallHandler               (Item::EndStoneBrickWall);
	constexpr cUnimplementedItemHandler     ItemEndStoneBricksHandler                  (Item::EndStoneBricks);
	constexpr cSimplePlaceableItemHandler   ItemEndStoneHandler                        (Item::EndStone);
	constexpr cItemEnderChestHandler        ItemEnderChestHandler                      (Item::EnderChest);
	constexpr cItemEyeOfEnderHandler        ItemEnderEyeHandler                        (Item::EnderEye);
	constexpr cItemEnderPearlHandler        ItemEnderPearlHandler                      (Item::EnderPearl);
	constexpr cItemSpawnEggHandler          ItemEndermanSpawnEggHandler                (Item::EndermanSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemEndermiteSpawnEggHandler               (Item::EndermiteSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemEvokerSpawnEggHandler                  (Item::EvokerSpawnEgg);
	constexpr cItemBottleOEnchantingHandler ItemExperienceBottleHandler                (Item::ExperienceBottle);
	constexpr cSimplePlaceableItemHandler   ItemFarmlandHandler                        (Item::Farmland);
	constexpr cDefaultItemHandler           ItemFeatherHandler                         (Item::Feather);
	constexpr cDefaultItemHandler           ItemFermentedSpiderEyeHandler              (Item::FermentedSpiderEye);
	constexpr cUnimplementedItemHandler     ItemFernHandler                            (Item::Fern);
	constexpr cItemMapHandler               ItemFilledMapHandler                       (Item::FilledMap);
	constexpr cItemLighterHandler           ItemFireChargeHandler                      (Item::FireCharge);
	constexpr cUnimplementedItemHandler     ItemFireCoralBlockHandler                  (Item::FireCoralBlock);
	constexpr cUnimplementedItemHandler     ItemFireCoralFanHandler                    (Item::FireCoralFan);
	constexpr cUnimplementedItemHandler     ItemFireCoralHandler                       (Item::FireCoral);
	constexpr cItemFireworkHandler          ItemFireworkRocketHandler                  (Item::FireworkRocket);
	constexpr cDefaultItemHandler           ItemFireworkStarHandler                    (Item::FireworkStar);
	constexpr cItemFishingRodHandler        ItemFishingRodHandler                      (Item::FishingRod);
	constexpr cUnimplementedItemHandler     ItemFletchingTableHandler                  (Item::FletchingTable);
	constexpr cItemLighterHandler           ItemFlintAndSteelHandler                   (Item::FlintAndSteel);
	constexpr cDefaultItemHandler           ItemFlintHandler                           (Item::Flint);
	constexpr cUnimplementedItemHandler     ItemFlowerBannerPatternHandler             (Item::FlowerBannerPattern);
	constexpr cSimplePlaceableItemHandler   ItemFlowerPotHandler                       (Item::FlowerPot);
	constexpr cItemSpawnEggHandler          ItemFoxSpawnEggHandler                     (Item::FoxSpawnEgg);
	constexpr cItemFurnaceHandler           ItemFurnaceHandler                         (Item::Furnace);
	constexpr cItemMinecartHandler          ItemFurnaceMinecartHandler                 (Item::FurnaceMinecart);
	constexpr cItemSpawnEggHandler          ItemGhastSpawnEggHandler                   (Item::GhastSpawnEgg);
	constexpr cDefaultItemHandler           ItemGhastTearHandler                       (Item::GhastTear);
	constexpr cUnimplementedItemHandler     ItemGildedBlackstoneHandler                (Item::GildedBlackstone);
	constexpr cItemBottleHandler            ItemGlassBottleHandler                     (Item::GlassBottle);
	constexpr cUnimplementedItemHandler     ItemGlassHandler                           (Item::Glass);
	constexpr cUnimplementedItemHandler     ItemGlassPaneHandler                       (Item::GlassPane);
	constexpr cDefaultItemHandler           ItemGlisteringMelonSliceHandler            (Item::GlisteringMelonSlice);
	constexpr cUnimplementedItemHandler     ItemGlobeBannerPatternHandler              (Item::GlobeBannerPattern);
	constexpr cDefaultItemHandler           ItemGlowstoneDustHandler                   (Item::GlowstoneDust);
	constexpr cSimplePlaceableItemHandler   ItemGlowstoneHandler                       (Item::Glowstone);
	constexpr cSimplePlaceableItemHandler   ItemGoldBlockHandler                       (Item::GoldBlock);
	constexpr cDefaultItemHandler           ItemGoldIngotHandler                       (Item::GoldIngot);
	constexpr cDefaultItemHandler           ItemGoldNuggetHandler                      (Item::GoldNugget);
	constexpr cSimplePlaceableItemHandler   ItemGoldOreHandler                         (Item::GoldOre);
	constexpr cItemGoldenAppleHandler       ItemGoldenAppleHandler                     (Item::GoldenApple);
	constexpr cItemAxeHandler               ItemGoldenAxeHandler                       (Item::GoldenAxe);
	constexpr cItemArmorHandler             ItemGoldenBootsHandler                     (Item::GoldenBoots);
	constexpr cItemSimpleFoodHandler        ItemGoldenCarrotHandler                    (Item::GoldenCarrot, cItemHandler::FoodInfo(6, 14.4));
	constexpr cItemArmorHandler             ItemGoldenChestplateHandler                (Item::GoldenChestplate);
	constexpr cItemArmorHandler             ItemGoldenHelmetHandler                    (Item::GoldenHelmet);
	constexpr cItemHoeHandler               ItemGoldenHoeHandler                       (Item::GoldenHoe);
	constexpr cDefaultItemHandler           ItemGoldenHorseArmorHandler                (Item::GoldenHorseArmor);
	constexpr cItemArmorHandler             ItemGoldenLeggingsHandler                  (Item::GoldenLeggings);
	constexpr cItemPickaxeHandler           ItemGoldenPickaxeHandler                   (Item::GoldenPickaxe);
	constexpr cItemShovelHandler            ItemGoldenShovelHandler                    (Item::GoldenShovel);
	constexpr cItemSwordHandler             ItemGoldenSwordHandler                     (Item::GoldenSword);
	constexpr cSimplePlaceableItemHandler   ItemGraniteHandler                         (Item::Granite);
	constexpr cItemSlabHandler              ItemGraniteSlabHandler                     (Item::GraniteSlab);
	constexpr cItemStairsHandler            ItemGraniteStairsHandler                   (Item::GraniteStairs);
	constexpr cUnimplementedItemHandler     ItemGraniteWallHandler                     (Item::GraniteWall);
	constexpr cSimplePlaceableItemHandler   ItemGrassBlockHandler                      (Item::GrassBlock);
	constexpr cUnimplementedItemHandler     ItemGrassHandler                           (Item::Grass);
	constexpr cSimplePlaceableItemHandler   ItemGrassPathHandler                       (Item::GrassPath);
	constexpr cSimplePlaceableItemHandler   ItemGravelHandler                          (Item::Gravel);
	constexpr cItemBannerHandler            ItemGrayBannerHandler                      (Item::GrayBanner);
	constexpr cItemBedHandler               ItemGrayBedHandler                         (Item::GrayBed);
	constexpr cSimplePlaceableItemHandler   ItemGrayCarpetHandler                      (Item::GrayCarpet);
	constexpr cUnimplementedItemHandler     ItemGrayConcreteHandler                    (Item::GrayConcrete);
	constexpr cUnimplementedItemHandler     ItemGrayConcretePowderHandler              (Item::GrayConcretePowder);
	constexpr cItemDyeHandler               ItemGrayDyeHandler                         (Item::GrayDye);
	constexpr cItemGlazedTerracottaHandler  ItemGrayGlazedTerracottaHandler            (Item::GrayGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemGrayShulkerBoxHandler                  (Item::GrayShulkerBox);
	constexpr cUnimplementedItemHandler     ItemGrayStainedGlassHandler                (Item::GrayStainedGlass);
	constexpr cUnimplementedItemHandler     ItemGrayStainedGlassPaneHandler            (Item::GrayStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemGrayTerracottaHandler                  (Item::GrayTerracotta);
	constexpr cItemWoolHandler              ItemGrayWoolHandler                        (Item::GrayWool);
	constexpr cItemBannerHandler            ItemGreenBannerHandler                     (Item::GreenBanner);
	constexpr cItemBedHandler               ItemGreenBedHandler                        (Item::GreenBed);
	constexpr cSimplePlaceableItemHandler   ItemGreenCarpetHandler                     (Item::GreenCarpet);
	constexpr cUnimplementedItemHandler     ItemGreenConcreteHandler                   (Item::GreenConcrete);
	constexpr cUnimplementedItemHandler     ItemGreenConcretePowderHandler             (Item::GreenConcretePowder);
	constexpr cItemDyeHandler               ItemGreenDyeHandler                        (Item::GreenDye);
	constexpr cItemGlazedTerracottaHandler  ItemGreenGlazedTerracottaHandler           (Item::GreenGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemGreenShulkerBoxHandler                 (Item::GreenShulkerBox);
	constexpr cUnimplementedItemHandler     ItemGreenStainedGlassHandler               (Item::GreenStainedGlass);
	constexpr cUnimplementedItemHandler     ItemGreenStainedGlassPaneHandler           (Item::GreenStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemGreenTerracottaHandler                 (Item::GreenTerracotta);
	constexpr cItemWoolHandler              ItemGreenWoolHandler                       (Item::GreenWool);
	constexpr cUnimplementedItemHandler     ItemGrindstoneHandler                      (Item::Grindstone);
	constexpr cItemSpawnEggHandler          ItemGuardianSpawnEggHandler                (Item::GuardianSpawnEgg);
	constexpr cDefaultItemHandler           ItemGunpowderHandler                       (Item::Gunpowder);
	constexpr cItemHayBaleHandler           ItemHayBaleHandler                         (Item::HayBale);
	constexpr cUnimplementedItemHandler     ItemHeartOfTheSeaHandler                   (Item::HeartOfTheSea);
	constexpr cSimplePlaceableItemHandler   ItemHeavyWeightedPressurePlateHandler      (Item::HeavyWeightedPressurePlate);
	constexpr cItemSpawnEggHandler          ItemHoglinSpawnEggHandler                  (Item::HoglinSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemHoneyBlockHandler                      (Item::HoneyBlock);
	constexpr cUnimplementedItemHandler     ItemHoneyBottleHandler                     (Item::HoneyBottle);
	constexpr cUnimplementedItemHandler     ItemHoneycombBlockHandler                  (Item::HoneycombBlock);
	constexpr cUnimplementedItemHandler     ItemHoneycombHandler                       (Item::Honeycomb);
	constexpr cItemHopperHandler            ItemHopperHandler                          (Item::Hopper);
	constexpr cItemMinecartHandler          ItemHopperMinecartHandler                  (Item::HopperMinecart);
	constexpr cUnimplementedItemHandler     ItemHornCoralBlockHandler                  (Item::HornCoralBlock);
	constexpr cUnimplementedItemHandler     ItemHornCoralFanHandler                    (Item::HornCoralFan);
	constexpr cUnimplementedItemHandler     ItemHornCoralHandler                       (Item::HornCoral);
	constexpr cItemSpawnEggHandler          ItemHorseSpawnEggHandler                   (Item::HorseSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemHuskSpawnEggHandler                    (Item::HuskSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemIceHandler                             (Item::Ice);
	constexpr cSimplePlaceableItemHandler   ItemInfestedChiseledStoneBricksHandler     (Item::InfestedChiseledStoneBricks);
	constexpr cSimplePlaceableItemHandler   ItemInfestedCobblestoneHandler             (Item::InfestedCobblestone);
	constexpr cSimplePlaceableItemHandler   ItemInfestedCrackedStoneBricksHandler      (Item::InfestedCrackedStoneBricks);
	constexpr cSimplePlaceableItemHandler   ItemInfestedMossyStoneBricksHandler        (Item::InfestedMossyStoneBricks);
	constexpr cSimplePlaceableItemHandler   ItemInfestedStoneBricksHandler             (Item::InfestedStoneBricks);
	constexpr cSimplePlaceableItemHandler   ItemInfestedStoneHandler                   (Item::InfestedStone);
	constexpr cItemDyeHandler               ItemInkSacHandler                          (Item::InkSac);
	constexpr cItemAxeHandler               ItemIronAxeHandler                         (Item::IronAxe);
	constexpr cSimplePlaceableItemHandler   ItemIronBarsHandler                        (Item::IronBars);
	constexpr cSimplePlaceableItemHandler   ItemIronBlockHandler                       (Item::IronBlock);
	constexpr cItemArmorHandler             ItemIronBootsHandler                       (Item::IronBoots);
	constexpr cItemArmorHandler             ItemIronChestplateHandler                  (Item::IronChestplate);
	constexpr cItemDoorHandler              ItemIronDoorHandler                        (Item::IronDoor);
	constexpr cItemArmorHandler             ItemIronHelmetHandler                      (Item::IronHelmet);
	constexpr cItemHoeHandler               ItemIronHoeHandler                         (Item::IronHoe);
	constexpr cDefaultItemHandler           ItemIronHorseArmorHandler                  (Item::IronHorseArmor);
	constexpr cDefaultItemHandler           ItemIronIngotHandler                       (Item::IronIngot);
	constexpr cItemArmorHandler             ItemIronLeggingsHandler                    (Item::IronLeggings);
	constexpr cDefaultItemHandler           ItemIronNuggetHandler                      (Item::IronNugget);
	constexpr cSimplePlaceableItemHandler   ItemIronOreHandler                         (Item::IronOre);
	constexpr cItemPickaxeHandler           ItemIronPickaxeHandler                     (Item::IronPickaxe);
	constexpr cItemShovelHandler            ItemIronShovelHandler                      (Item::IronShovel);
	constexpr cItemSwordHandler             ItemIronSwordHandler                       (Item::IronSword);
	constexpr cItemTrapdoorHandler          ItemIronTrapdoorHandler                    (Item::IronTrapdoor);
	constexpr cItemItemFrameHandler         ItemItemFrameHandler                       (Item::ItemFrame);
	constexpr cItemJackOLanternHandler      ItemJackOLanternHandler                    (Item::JackOLantern);
	constexpr cUnimplementedItemHandler     ItemJigsawHandler                          (Item::Jigsaw);
	constexpr cSimplePlaceableItemHandler   ItemJukeboxHandler                         (Item::Jukebox);
	constexpr cItemBoatHandler              ItemJungleBoatHandler                      (Item::JungleBoat);
	constexpr cItemButtonHandler            ItemJungleButtonHandler                    (Item::JungleButton);
	constexpr cItemDoorHandler              ItemJungleDoorHandler                      (Item::JungleDoor);
	constexpr cItemFenceGateHandler         ItemJungleFenceGateHandler                 (Item::JungleFenceGate);
	constexpr cItemFenceHandler             ItemJungleFenceHandler                     (Item::JungleFence);
	constexpr cItemLeavesHandler            ItemJungleLeavesHandler                    (Item::JungleLeaves);
	constexpr cItemLogHandler               ItemJungleLogHandler                       (Item::JungleLog);
	constexpr cSimplePlaceableItemHandler   ItemJunglePlanksHandler                    (Item::JunglePlanks);
	constexpr cSimplePlaceableItemHandler   ItemJunglePressurePlateHandler             (Item::JunglePressurePlate);
	constexpr cSimplePlaceableItemHandler   ItemJungleSaplingHandler                   (Item::JungleSapling);
	constexpr cItemSignHandler              ItemJungleSignHandler                      (Item::JungleSign);
	constexpr cItemSlabHandler              ItemJungleSlabHandler                      (Item::JungleSlab);
	constexpr cItemStairsHandler            ItemJungleStairsHandler                    (Item::JungleStairs);
	constexpr cItemTrapdoorHandler          ItemJungleTrapdoorHandler                  (Item::JungleTrapdoor);
	constexpr cItemWoodHandler              ItemJungleWoodHandler                      (Item::JungleWood);
	constexpr cUnimplementedItemHandler     ItemKelpHandler                            (Item::Kelp);
	constexpr cUnimplementedItemHandler     ItemKnowledgeBookHandler                   (Item::KnowledgeBook);
	constexpr cItemLadderHandler            ItemLadderHandler                          (Item::Ladder);
	constexpr cUnimplementedItemHandler     ItemLanternHandler                         (Item::Lantern);
	constexpr cSimplePlaceableItemHandler   ItemLapisBlockHandler                      (Item::LapisBlock);
	constexpr cItemDyeHandler               ItemLapisLazuliHandler                     (Item::LapisLazuli);
	constexpr cSimplePlaceableItemHandler   ItemLapisOreHandler                        (Item::LapisOre);
	constexpr cUnimplementedItemHandler     ItemLargeFernHandler                       (Item::LargeFern);
	constexpr cItemBucketHandler            ItemLavaBucketHandler                      (Item::LavaBucket);
	constexpr cDefaultItemHandler           ItemLeadHandler                            (Item::Lead);
	constexpr cItemArmorHandler             ItemLeatherBootsHandler                    (Item::LeatherBoots);
	constexpr cItemArmorHandler             ItemLeatherChestplateHandler               (Item::LeatherChestplate);
	constexpr cDefaultItemHandler           ItemLeatherHandler                         (Item::Leather);
	constexpr cItemArmorHandler             ItemLeatherHelmetHandler                   (Item::LeatherHelmet);
	constexpr cDefaultItemHandler           ItemLeatherHorseArmorHandler               (Item::LeatherHorseArmor);
	constexpr cItemArmorHandler             ItemLeatherLeggingsHandler                 (Item::LeatherLeggings);
	constexpr cUnimplementedItemHandler     ItemLecternHandler                         (Item::Lectern);
	constexpr cItemLeverHandler             ItemLeverHandler                           (Item::Lever);
	constexpr cItemBannerHandler            ItemLightBlueBannerHandler                 (Item::LightBlueBanner);
	constexpr cItemBedHandler               ItemLightBlueBedHandler                    (Item::LightBlueBed);
	constexpr cSimplePlaceableItemHandler   ItemLightBlueCarpetHandler                 (Item::LightBlueCarpet);
	constexpr cUnimplementedItemHandler     ItemLightBlueConcreteHandler               (Item::LightBlueConcrete);
	constexpr cUnimplementedItemHandler     ItemLightBlueConcretePowderHandler         (Item::LightBlueConcretePowder);
	constexpr cItemDyeHandler               ItemLightBlueDyeHandler                    (Item::LightBlueDye);
	constexpr cItemGlazedTerracottaHandler  ItemLightBlueGlazedTerracottaHandler       (Item::LightBlueGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemLightBlueShulkerBoxHandler             (Item::LightBlueShulkerBox);
	constexpr cUnimplementedItemHandler     ItemLightBlueStainedGlassHandler           (Item::LightBlueStainedGlass);
	constexpr cUnimplementedItemHandler     ItemLightBlueStainedGlassPaneHandler       (Item::LightBlueStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemLightBlueTerracottaHandler             (Item::LightBlueTerracotta);
	constexpr cItemWoolHandler              ItemLightBlueWoolHandler                   (Item::LightBlueWool);
	constexpr cItemBannerHandler            ItemLightGrayBannerHandler                 (Item::LightGrayBanner);
	constexpr cItemBedHandler               ItemLightGrayBedHandler                    (Item::LightGrayBed);
	constexpr cSimplePlaceableItemHandler   ItemLightGrayCarpetHandler                 (Item::LightGrayCarpet);
	constexpr cUnimplementedItemHandler     ItemLightGrayConcreteHandler               (Item::LightGrayConcrete);
	constexpr cUnimplementedItemHandler     ItemLightGrayConcretePowderHandler         (Item::LightGrayConcretePowder);
	constexpr cItemDyeHandler               ItemLightGrayDyeHandler                    (Item::LightGrayDye);
	constexpr cItemGlazedTerracottaHandler  ItemLightGrayGlazedTerracottaHandler       (Item::LightGrayGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemLightGrayShulkerBoxHandler             (Item::LightGrayShulkerBox);
	constexpr cUnimplementedItemHandler     ItemLightGrayStainedGlassHandler           (Item::LightGrayStainedGlass);
	constexpr cUnimplementedItemHandler     ItemLightGrayStainedGlassPaneHandler       (Item::LightGrayStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemLightGrayTerracottaHandler             (Item::LightGrayTerracotta);
	constexpr cItemWoolHandler              ItemLightGrayWoolHandler                   (Item::LightGrayWool);
	constexpr cSimplePlaceableItemHandler   ItemLightWeightedPressurePlateHandler      (Item::LightWeightedPressurePlate);
	constexpr cUnimplementedItemHandler     ItemLilacHandler                           (Item::Lilac);
	constexpr cUnimplementedItemHandler     ItemLilyOfTheValleyHandler                 (Item::LilyOfTheValley);
	constexpr cItemLilypadHandler           ItemLilyPadHandler                         (Item::LilyPad);
	constexpr cItemBannerHandler            ItemLimeBannerHandler                      (Item::LimeBanner);
	constexpr cItemBedHandler               ItemLimeBedHandler                         (Item::LimeBed);
	constexpr cSimplePlaceableItemHandler   ItemLimeCarpetHandler                      (Item::LimeCarpet);
	constexpr cUnimplementedItemHandler     ItemLimeConcreteHandler                    (Item::LimeConcrete);
	constexpr cUnimplementedItemHandler     ItemLimeConcretePowderHandler              (Item::LimeConcretePowder);
	constexpr cItemDyeHandler               ItemLimeDyeHandler                         (Item::LimeDye);
	constexpr cItemGlazedTerracottaHandler  ItemLimeGlazedTerracottaHandler            (Item::LimeGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemLimeShulkerBoxHandler                  (Item::LimeShulkerBox);
	constexpr cUnimplementedItemHandler     ItemLimeStainedGlassHandler                (Item::LimeStainedGlass);
	constexpr cUnimplementedItemHandler     ItemLimeStainedGlassPaneHandler            (Item::LimeStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemLimeTerracottaHandler                  (Item::LimeTerracotta);
	constexpr cItemWoolHandler              ItemLimeWoolHandler                        (Item::LimeWool);
	constexpr cItemPotionHandler            ItemLingeringPotionHandler                 (Item::LingeringPotion);
	constexpr cItemSpawnEggHandler          ItemLlamaSpawnEggHandler                   (Item::LlamaSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemLodestoneHandler                       (Item::Lodestone);
	constexpr cUnimplementedItemHandler     ItemLoomHandler                            (Item::Loom);
	constexpr cItemBannerHandler            ItemMagentaBannerHandler                   (Item::MagentaBanner);
	constexpr cItemBedHandler               ItemMagentaBedHandler                      (Item::MagentaBed);
	constexpr cSimplePlaceableItemHandler   ItemMagentaCarpetHandler                   (Item::MagentaCarpet);
	constexpr cUnimplementedItemHandler     ItemMagentaConcreteHandler                 (Item::MagentaConcrete);
	constexpr cUnimplementedItemHandler     ItemMagentaConcretePowderHandler           (Item::MagentaConcretePowder);
	constexpr cItemDyeHandler               ItemMagentaDyeHandler                      (Item::MagentaDye);
	constexpr cItemGlazedTerracottaHandler  ItemMagentaGlazedTerracottaHandler         (Item::MagentaGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemMagentaShulkerBoxHandler               (Item::MagentaShulkerBox);
	constexpr cUnimplementedItemHandler     ItemMagentaStainedGlassHandler             (Item::MagentaStainedGlass);
	constexpr cUnimplementedItemHandler     ItemMagentaStainedGlassPaneHandler         (Item::MagentaStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemMagentaTerracottaHandler               (Item::MagentaTerracotta);
	constexpr cItemWoolHandler              ItemMagentaWoolHandler                     (Item::MagentaWool);
	constexpr cUnimplementedItemHandler     ItemMagmaBlockHandler                      (Item::MagmaBlock);
	constexpr cDefaultItemHandler           ItemMagmaCreamHandler                      (Item::MagmaCream);
	constexpr cItemSpawnEggHandler          ItemMagmaCubeSpawnEggHandler               (Item::MagmaCubeSpawnEgg);
	constexpr cItemEmptyMapHandler          ItemMapHandler                             (Item::Map);
	constexpr cSimplePlaceableItemHandler   ItemMelonHandler                           (Item::Melon);
	constexpr cItemSimpleSeedsHandler       ItemMelonSeedsHandler                      (Item::MelonSeeds);
	constexpr cItemSimpleFoodHandler        ItemMelonSliceHandler                      (Item::MelonSlice, cItemHandler::FoodInfo(2, 1.2));
	constexpr cItemBucketHandler            ItemMilkBucketHandler                      (Item::MilkBucket);
	constexpr cItemMinecartHandler          ItemMinecartHandler                        (Item::Minecart);
	constexpr cUnimplementedItemHandler     ItemMojangBannerPatternHandler             (Item::MojangBannerPattern);
	constexpr cItemSpawnEggHandler          ItemMooshroomSpawnEggHandler               (Item::MooshroomSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemMossyCobblestoneHandler                (Item::MossyCobblestone);
	constexpr cItemSlabHandler              ItemMossyCobblestoneSlabHandler            (Item::MossyCobblestoneSlab);
	constexpr cItemStairsHandler            ItemMossyCobblestoneStairsHandler          (Item::MossyCobblestoneStairs);
	constexpr cUnimplementedItemHandler     ItemMossyCobblestoneWallHandler            (Item::MossyCobblestoneWall);
	constexpr cItemSlabHandler              ItemMossyStoneBrickSlabHandler             (Item::MossyStoneBrickSlab);
	constexpr cItemStairsHandler            ItemMossyStoneBrickStairsHandler           (Item::MossyStoneBrickStairs);
	constexpr cUnimplementedItemHandler     ItemMossyStoneBrickWallHandler             (Item::MossyStoneBrickWall);
	constexpr cSimplePlaceableItemHandler   ItemMossyStoneBricksHandler                (Item::MossyStoneBricks);
	constexpr cItemSpawnEggHandler          ItemMuleSpawnEggHandler                    (Item::MuleSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemMushroomStemHandler                    (Item::MushroomStem);
	constexpr cItemSoupHandler              ItemMushroomStewHandler                    (Item::MushroomStew, cItemHandler::FoodInfo(6, 7.2));
	constexpr cDefaultItemHandler           ItemMusicDisc11Handler                     (Item::MusicDisc11);
	constexpr cDefaultItemHandler           ItemMusicDisc13Handler                     (Item::MusicDisc13);
	constexpr cDefaultItemHandler           ItemMusicDiscBlocksHandler                 (Item::MusicDiscBlocks);
	constexpr cDefaultItemHandler           ItemMusicDiscCatHandler                    (Item::MusicDiscCat);
	constexpr cDefaultItemHandler           ItemMusicDiscChirpHandler                  (Item::MusicDiscChirp);
	constexpr cDefaultItemHandler           ItemMusicDiscFarHandler                    (Item::MusicDiscFar);
	constexpr cDefaultItemHandler           ItemMusicDiscMallHandler                   (Item::MusicDiscMall);
	constexpr cDefaultItemHandler           ItemMusicDiscMellohiHandler                (Item::MusicDiscMellohi);
	constexpr cDefaultItemHandler           ItemMusicDiscPigstepHandler                (Item::MusicDiscPigstep);
	constexpr cDefaultItemHandler           ItemMusicDiscStalHandler                   (Item::MusicDiscStal);
	constexpr cDefaultItemHandler           ItemMusicDiscStradHandler                  (Item::MusicDiscStrad);
	constexpr cDefaultItemHandler           ItemMusicDiscWaitHandler                   (Item::MusicDiscWait);
	constexpr cDefaultItemHandler           ItemMusicDiscWardHandler                   (Item::MusicDiscWard);
	constexpr cItemSimpleFoodHandler        ItemMuttonHandler                          (Item::Mutton, cItemHandler::FoodInfo(2, 1.2));
	constexpr cSimplePlaceableItemHandler   ItemMyceliumHandler                        (Item::Mycelium);
	constexpr cDefaultItemHandler           ItemNameTagHandler                         (Item::NameTag);
	constexpr cUnimplementedItemHandler     ItemNautilusShellHandler                   (Item::NautilusShell);
	constexpr cItemFenceHandler             ItemNetherBrickFenceHandler                (Item::NetherBrickFence);
	constexpr cDefaultItemHandler           ItemNetherBrickHandler                     (Item::NetherBrick);
	constexpr cItemSlabHandler              ItemNetherBrickSlabHandler                 (Item::NetherBrickSlab);
	constexpr cItemStairsHandler            ItemNetherBrickStairsHandler               (Item::NetherBrickStairs);
	constexpr cUnimplementedItemHandler     ItemNetherBrickWallHandler                 (Item::NetherBrickWall);
	constexpr cSimplePlaceableItemHandler   ItemNetherBricksHandler                    (Item::NetherBricks);
	constexpr cSimplePlaceableItemHandler   ItemNetherGoldOreHandler                   (Item::NetherGoldOre);
	constexpr cSimplePlaceableItemHandler   ItemNetherQuartzOreHandler                 (Item::NetherQuartzOre);
	constexpr cUnimplementedItemHandler     ItemNetherSproutsHandler                   (Item::NetherSprouts);
	constexpr cDefaultItemHandler           ItemNetherStarHandler                      (Item::NetherStar);
	constexpr cUnimplementedItemHandler     ItemNetherWartBlockHandler                 (Item::NetherWartBlock);
	constexpr cItemNetherWartHandler        ItemNetherWartHandler                      (Item::NetherWart);
	constexpr cItemAxeHandler               ItemNetheriteAxeHandler                    (Item::NetheriteAxe);
	constexpr cUnimplementedItemHandler     ItemNetheriteBlockHandler                  (Item::NetheriteBlock);
	constexpr cItemArmorHandler             ItemNetheriteBootsHandler                  (Item::NetheriteBoots);
	constexpr cItemArmorHandler             ItemNetheriteChestplateHandler             (Item::NetheriteChestplate);
	constexpr cItemArmorHandler             ItemNetheriteHelmetHandler                 (Item::NetheriteHelmet);
	constexpr cItemHoeHandler               ItemNetheriteHoeHandler                    (Item::NetheriteHoe);
	constexpr cUnimplementedItemHandler     ItemNetheriteIngotHandler                  (Item::NetheriteIngot);
	constexpr cItemArmorHandler             ItemNetheriteLeggingsHandler               (Item::NetheriteLeggings);
	constexpr cItemPickaxeHandler           ItemNetheritePickaxeHandler                (Item::NetheritePickaxe);
	constexpr cUnimplementedItemHandler     ItemNetheriteScrapHandler                  (Item::NetheriteScrap);
	constexpr cItemShovelHandler            ItemNetheriteShovelHandler                 (Item::NetheriteShovel);
	constexpr cItemSwordHandler             ItemNetheriteSwordHandler                  (Item::NetheriteSword);
	constexpr cSimplePlaceableItemHandler   ItemNetherrackHandler                      (Item::Netherrack);
	constexpr cSimplePlaceableItemHandler   ItemNoteBlockHandler                       (Item::NoteBlock);
	constexpr cItemBoatHandler              ItemOakBoatHandler                         (Item::OakBoat);
	constexpr cItemButtonHandler            ItemOakButtonHandler                       (Item::OakButton);
	constexpr cItemDoorHandler              ItemOakDoorHandler                         (Item::OakDoor);
	constexpr cItemFenceGateHandler         ItemOakFenceGateHandler                    (Item::OakFenceGate);
	constexpr cItemFenceHandler             ItemOakFenceHandler                        (Item::OakFence);
	constexpr cItemLeavesHandler            ItemOakLeavesHandler                       (Item::OakLeaves);
	constexpr cItemLogHandler               ItemOakLogHandler                          (Item::OakLog);
	constexpr cSimplePlaceableItemHandler   ItemOakPlanksHandler                       (Item::OakPlanks);
	constexpr cSimplePlaceableItemHandler   ItemOakPressurePlateHandler                (Item::OakPressurePlate);
	constexpr cSimplePlaceableItemHandler   ItemOakSaplingHandler                      (Item::OakSapling);
	constexpr cItemSignHandler              ItemOakSignHandler                         (Item::OakSign);
	constexpr cItemSlabHandler              ItemOakSlabHandler                         (Item::OakSlab);
	constexpr cItemStairsHandler            ItemOakStairsHandler                       (Item::OakStairs);
	constexpr cItemTrapdoorHandler          ItemOakTrapdoorHandler                     (Item::OakTrapdoor);
	constexpr cItemWoodHandler              ItemOakWoodHandler                         (Item::OakWood);
	constexpr cItemObserverHandler          ItemObserverHandler                        (Item::Observer);
	constexpr cSimplePlaceableItemHandler   ItemObsidianHandler                        (Item::Obsidian);
	constexpr cItemSpawnEggHandler          ItemOcelotSpawnEggHandler                  (Item::OcelotSpawnEgg);
	constexpr cItemBannerHandler            ItemOrangeBannerHandler                    (Item::OrangeBanner);
	constexpr cItemBedHandler               ItemOrangeBedHandler                       (Item::OrangeBed);
	constexpr cSimplePlaceableItemHandler   ItemOrangeCarpetHandler                    (Item::OrangeCarpet);
	constexpr cUnimplementedItemHandler     ItemOrangeConcreteHandler                  (Item::OrangeConcrete);
	constexpr cUnimplementedItemHandler     ItemOrangeConcretePowderHandler            (Item::OrangeConcretePowder);
	constexpr cItemDyeHandler               ItemOrangeDyeHandler                       (Item::OrangeDye);
	constexpr cItemGlazedTerracottaHandler  ItemOrangeGlazedTerracottaHandler          (Item::OrangeGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemOrangeShulkerBoxHandler                (Item::OrangeShulkerBox);
	constexpr cUnimplementedItemHandler     ItemOrangeStainedGlassHandler              (Item::OrangeStainedGlass);
	constexpr cUnimplementedItemHandler     ItemOrangeStainedGlassPaneHandler          (Item::OrangeStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemOrangeTerracottaHandler                (Item::OrangeTerracotta);
	constexpr cUnimplementedItemHandler     ItemOrangeTulipHandler                     (Item::OrangeTulip);
	constexpr cItemWoolHandler              ItemOrangeWoolHandler                      (Item::OrangeWool);
	constexpr cUnimplementedItemHandler     ItemOxeyeDaisyHandler                      (Item::OxeyeDaisy);
	constexpr cUnimplementedItemHandler     ItemPackedIceHandler                       (Item::PackedIce);
	constexpr cItemPaintingHandler          ItemPaintingHandler                        (Item::Painting);
	constexpr cItemSpawnEggHandler          ItemPandaSpawnEggHandler                   (Item::PandaSpawnEgg);
	constexpr cDefaultItemHandler           ItemPaperHandler                           (Item::Paper);
	constexpr cItemSpawnEggHandler          ItemParrotSpawnEggHandler                  (Item::ParrotSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemPeonyHandler                           (Item::Peony);
	constexpr cItemSlabHandler              ItemPetrifiedOakSlabHandler                (Item::PetrifiedOakSlab);
	constexpr cUnimplementedItemHandler     ItemPhantomMembraneHandler                 (Item::PhantomMembrane);
	constexpr cItemSpawnEggHandler          ItemPhantomSpawnEggHandler                 (Item::PhantomSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemPigSpawnEggHandler                     (Item::PigSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemPiglinBannerPatternHandler             (Item::PiglinBannerPattern);
	constexpr cItemSpawnEggHandler          ItemPiglinSpawnEggHandler                  (Item::PiglinSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemPillagerSpawnEggHandler                (Item::PillagerSpawnEgg);
	constexpr cItemBannerHandler            ItemPinkBannerHandler                      (Item::PinkBanner);
	constexpr cItemBedHandler               ItemPinkBedHandler                         (Item::PinkBed);
	constexpr cSimplePlaceableItemHandler   ItemPinkCarpetHandler                      (Item::PinkCarpet);
	constexpr cUnimplementedItemHandler     ItemPinkConcreteHandler                    (Item::PinkConcrete);
	constexpr cUnimplementedItemHandler     ItemPinkConcretePowderHandler              (Item::PinkConcretePowder);
	constexpr cItemDyeHandler               ItemPinkDyeHandler                         (Item::PinkDye);
	constexpr cItemGlazedTerracottaHandler  ItemPinkGlazedTerracottaHandler            (Item::PinkGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemPinkShulkerBoxHandler                  (Item::PinkShulkerBox);
	constexpr cUnimplementedItemHandler     ItemPinkStainedGlassHandler                (Item::PinkStainedGlass);
	constexpr cUnimplementedItemHandler     ItemPinkStainedGlassPaneHandler            (Item::PinkStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemPinkTerracottaHandler                  (Item::PinkTerracotta);
	constexpr cUnimplementedItemHandler     ItemPinkTulipHandler                       (Item::PinkTulip);
	constexpr cItemWoolHandler              ItemPinkWoolHandler                        (Item::PinkWool);
	constexpr cItemPistonHandler            ItemPistonHandler                          (Item::Piston);
	constexpr cItemMobHeadHandler           ItemPlayerHeadHandler                      (Item::PlayerHead);
	constexpr cSimplePlaceableItemHandler   ItemPodzolHandler                          (Item::Podzol);
	constexpr cItemPoisonousPotatoHandler   ItemPoisonousPotatoHandler                 (Item::PoisonousPotato);
	constexpr cItemSpawnEggHandler          ItemPolarBearSpawnEggHandler               (Item::PolarBearSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemPolishedAndesiteHandler                (Item::PolishedAndesite);
	constexpr cItemSlabHandler              ItemPolishedAndesiteSlabHandler            (Item::PolishedAndesiteSlab);
	constexpr cItemStairsHandler            ItemPolishedAndesiteStairsHandler          (Item::PolishedAndesiteStairs);
	constexpr cUnimplementedItemHandler     ItemPolishedBasaltHandler                  (Item::PolishedBasalt);
	constexpr cItemSlabHandler              ItemPolishedBlackstoneBrickSlabHandler     (Item::PolishedBlackstoneBrickSlab);
	constexpr cItemStairsHandler            ItemPolishedBlackstoneBrickStairsHandler   (Item::PolishedBlackstoneBrickStairs);
	constexpr cUnimplementedItemHandler     ItemPolishedBlackstoneBrickWallHandler     (Item::PolishedBlackstoneBrickWall);
	constexpr cUnimplementedItemHandler     ItemPolishedBlackstoneBricksHandler        (Item::PolishedBlackstoneBricks);
	constexpr cItemButtonHandler            ItemPolishedBlackstoneButtonHandler        (Item::PolishedBlackstoneButton);
	constexpr cUnimplementedItemHandler     ItemPolishedBlackstoneHandler              (Item::PolishedBlackstone);
	constexpr cSimplePlaceableItemHandler   ItemPolishedBlackstonePressurePlateHandler (Item::PolishedBlackstonePressurePlate);
	constexpr cItemSlabHandler              ItemPolishedBlackstoneSlabHandler          (Item::PolishedBlackstoneSlab);
	constexpr cItemStairsHandler            ItemPolishedBlackstoneStairsHandler        (Item::PolishedBlackstoneStairs);
	constexpr cUnimplementedItemHandler     ItemPolishedBlackstoneWallHandler          (Item::PolishedBlackstoneWall);
	constexpr cUnimplementedItemHandler     ItemPolishedDioriteHandler                 (Item::PolishedDiorite);
	constexpr cItemSlabHandler              ItemPolishedDioriteSlabHandler             (Item::PolishedDioriteSlab);
	constexpr cItemStairsHandler            ItemPolishedDioriteStairsHandler           (Item::PolishedDioriteStairs);
	constexpr cUnimplementedItemHandler     ItemPolishedGraniteHandler                 (Item::PolishedGranite);
	constexpr cItemSlabHandler              ItemPolishedGraniteSlabHandler             (Item::PolishedGraniteSlab);
	constexpr cItemStairsHandler            ItemPolishedGraniteStairsHandler           (Item::PolishedGraniteStairs);
	constexpr cUnimplementedItemHandler     ItemPoppedChorusFruitHandler               (Item::PoppedChorusFruit);
	constexpr cSimplePlaceableItemHandler   ItemPoppyHandler                           (Item::Poppy);
	constexpr cItemSimpleFoodHandler        ItemPorkchopHandler                        (Item::Porkchop, cItemHandler::FoodInfo(3, 1.8));
	constexpr cItemFoodSeedsHandler         ItemPotatoHandler                          (Item::Potato, cItemHandler::FoodInfo(1, 0.6));
	constexpr cItemPotionHandler            ItemPotionHandler                          (Item::Potion);
	constexpr cItemRailHandler              ItemPoweredRailHandler                     (Item::PoweredRail);
	constexpr cItemSlabHandler              ItemPrismarineBrickSlabHandler             (Item::PrismarineBrickSlab);
	constexpr cItemStairsHandler            ItemPrismarineBrickStairsHandler           (Item::PrismarineBrickStairs);
	constexpr cUnimplementedItemHandler     ItemPrismarineBricksHandler                (Item::PrismarineBricks);
	constexpr cUnimplementedItemHandler     ItemPrismarineCrystalsHandler              (Item::PrismarineCrystals);
	constexpr cUnimplementedItemHandler     ItemPrismarineHandler                      (Item::Prismarine);
	constexpr cDefaultItemHandler           ItemPrismarineShardHandler                 (Item::PrismarineShard);
	constexpr cItemSlabHandler              ItemPrismarineSlabHandler                  (Item::PrismarineSlab);
	constexpr cItemStairsHandler            ItemPrismarineStairsHandler                (Item::PrismarineStairs);
	constexpr cUnimplementedItemHandler     ItemPrismarineWallHandler                  (Item::PrismarineWall);
	constexpr cUnimplementedItemHandler     ItemPufferfishBucketHandler                (Item::PufferfishBucket);
	constexpr cUnimplementedItemHandler     ItemPufferfishHandler                      (Item::Pufferfish);
	constexpr cItemSpawnEggHandler          ItemPufferfishSpawnEggHandler              (Item::PufferfishSpawnEgg);
	constexpr cItemPumpkinHandler           ItemPumpkinHandler                         (Item::Pumpkin);
	constexpr cItemSimpleFoodHandler        ItemPumpkinPieHandler                      (Item::PumpkinPie, cItemHandler::FoodInfo(8, 4.8));
	constexpr cItemSimpleSeedsHandler       ItemPumpkinSeedsHandler                    (Item::PumpkinSeeds);
	constexpr cItemBannerHandler            ItemPurpleBannerHandler                    (Item::PurpleBanner);
	constexpr cItemBedHandler               ItemPurpleBedHandler                       (Item::PurpleBed);
	constexpr cSimplePlaceableItemHandler   ItemPurpleCarpetHandler                    (Item::PurpleCarpet);
	constexpr cUnimplementedItemHandler     ItemPurpleConcreteHandler                  (Item::PurpleConcrete);
	constexpr cUnimplementedItemHandler     ItemPurpleConcretePowderHandler            (Item::PurpleConcretePowder);
	constexpr cItemDyeHandler               ItemPurpleDyeHandler                       (Item::PurpleDye);
	constexpr cItemGlazedTerracottaHandler  ItemPurpleGlazedTerracottaHandler          (Item::PurpleGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemPurpleShulkerBoxHandler                (Item::PurpleShulkerBox);
	constexpr cUnimplementedItemHandler     ItemPurpleStainedGlassHandler              (Item::PurpleStainedGlass);
	constexpr cUnimplementedItemHandler     ItemPurpleStainedGlassPaneHandler          (Item::PurpleStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemPurpleTerracottaHandler                (Item::PurpleTerracotta);
	constexpr cItemWoolHandler              ItemPurpleWoolHandler                      (Item::PurpleWool);
	constexpr cUnimplementedItemHandler     ItemPurpurBlockHandler                     (Item::PurpurBlock);
	constexpr cUnimplementedItemHandler     ItemPurpurPillarHandler                    (Item::PurpurPillar);
	constexpr cItemSlabHandler              ItemPurpurSlabHandler                      (Item::PurpurSlab);
	constexpr cItemStairsHandler            ItemPurpurStairsHandler                    (Item::PurpurStairs);
	constexpr cItemQuartzHandler            ItemQuartzBlockHandler                     (Item::QuartzBlock);
	constexpr cUnimplementedItemHandler     ItemQuartzBricksHandler                    (Item::QuartzBricks);
	constexpr cUnimplementedItemHandler     ItemQuartzHandler                          (Item::Quartz);
	constexpr cUnimplementedItemHandler     ItemQuartzPillarHandler                    (Item::QuartzPillar);
	constexpr cItemSlabHandler              ItemQuartzSlabHandler                      (Item::QuartzSlab);
	constexpr cItemStairsHandler            ItemQuartzStairsHandler                    (Item::QuartzStairs);
	constexpr cDefaultItemHandler           ItemRabbitFootHandler                      (Item::RabbitFoot);
	constexpr cItemSimpleFoodHandler        ItemRabbitHandler                          (Item::Rabbit,  cItemHandler::FoodInfo(3, 1.8));
	constexpr cDefaultItemHandler           ItemRabbitHideHandler                      (Item::RabbitHide);
	constexpr cItemSpawnEggHandler          ItemRabbitSpawnEggHandler                  (Item::RabbitSpawnEgg);
	constexpr cItemSoupHandler              ItemRabbitStewHandler                      (Item::RabbitStew, cItemHandler::FoodInfo(10, 12));
	constexpr cItemRailHandler              ItemRailHandler                            (Item::Rail);
	constexpr cItemSpawnEggHandler          ItemRavagerSpawnEggHandler                 (Item::RavagerSpawnEgg);
	constexpr cItemBannerHandler            ItemRedBannerHandler                       (Item::RedBanner);
	constexpr cItemBedHandler               ItemRedBedHandler                          (Item::RedBed);
	constexpr cSimplePlaceableItemHandler   ItemRedCarpetHandler                       (Item::RedCarpet);
	constexpr cUnimplementedItemHandler     ItemRedConcreteHandler                     (Item::RedConcrete);
	constexpr cUnimplementedItemHandler     ItemRedConcretePowderHandler               (Item::RedConcretePowder);
	constexpr cItemDyeHandler               ItemRedDyeHandler                          (Item::RedDye);
	constexpr cItemGlazedTerracottaHandler  ItemRedGlazedTerracottaHandler             (Item::RedGlazedTerracotta);
	constexpr cSimplePlaceableItemHandler   ItemRedMushroomBlockHandler                (Item::RedMushroomBlock);
	constexpr cDefaultItemHandler           ItemRedMushroomHandler                     (Item::RedMushroom);
	constexpr cItemSlabHandler              ItemRedNetherBrickSlabHandler              (Item::RedNetherBrickSlab);
	constexpr cItemStairsHandler            ItemRedNetherBrickStairsHandler            (Item::RedNetherBrickStairs);
	constexpr cUnimplementedItemHandler     ItemRedNetherBrickWallHandler              (Item::RedNetherBrickWall);
	constexpr cUnimplementedItemHandler     ItemRedNetherBricksHandler                 (Item::RedNetherBricks);
	constexpr cUnimplementedItemHandler     ItemRedSandHandler                         (Item::RedSand);
	constexpr cUnimplementedItemHandler     ItemRedSandstoneHandler                    (Item::RedSandstone);
	constexpr cItemSlabHandler              ItemRedSandstoneSlabHandler                (Item::RedSandstoneSlab);
	constexpr cItemStairsHandler            ItemRedSandstoneStairsHandler              (Item::RedSandstoneStairs);
	constexpr cUnimplementedItemHandler     ItemRedSandstoneWallHandler                (Item::RedSandstoneWall);
	constexpr cUnimplementedItemHandler     ItemRedShulkerBoxHandler                   (Item::RedShulkerBox);
	constexpr cUnimplementedItemHandler     ItemRedStainedGlassHandler                 (Item::RedStainedGlass);
	constexpr cUnimplementedItemHandler     ItemRedStainedGlassPaneHandler             (Item::RedStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemRedTerracottaHandler                   (Item::RedTerracotta);
	constexpr cUnimplementedItemHandler     ItemRedTulipHandler                        (Item::RedTulip);
	constexpr cItemWoolHandler              ItemRedWoolHandler                         (Item::RedWool);
	constexpr cSimplePlaceableItemHandler   ItemRedstoneBlockHandler                   (Item::RedstoneBlock);
	constexpr cUnimplementedItemHandler     ItemRedstoneHandler                        (Item::Redstone);
	constexpr cSimplePlaceableItemHandler   ItemRedstoneLampHandler                    (Item::RedstoneLamp);
	constexpr cSimplePlaceableItemHandler   ItemRedstoneOreHandler                     (Item::RedstoneOre);
	constexpr cItemTorchHandler             ItemRedstoneTorchHandler                   (Item::RedstoneTorch);
	constexpr cItemRepeaterHandler          ItemRepeaterHandler                        (Item::Repeater);
	constexpr cSimplePlaceableItemHandler   ItemRepeatingCommandBlockHandler           (Item::RepeatingCommandBlock);
	constexpr cUnimplementedItemHandler     ItemRespawnAnchorHandler                   (Item::RespawnAnchor);
	constexpr cUnimplementedItemHandler     ItemRoseBushHandler                        (Item::RoseBush);
	constexpr cItemRottenFleshHandler       ItemRottenFleshHandler                     (Item::RottenFlesh);
	constexpr cDefaultItemHandler           ItemSaddleHandler                          (Item::Saddle);
	constexpr cUnimplementedItemHandler     ItemSalmonBucketHandler                    (Item::SalmonBucket);
	constexpr cItemRawFishHandler           ItemSalmonHandler                          (Item::Salmon);
	constexpr cItemSpawnEggHandler          ItemSalmonSpawnEggHandler                  (Item::SalmonSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemSandHandler                            (Item::Sand);
	constexpr cSimplePlaceableItemHandler   ItemSandstoneHandler                       (Item::Sandstone);
	constexpr cItemSlabHandler              ItemSandstoneSlabHandler                   (Item::SandstoneSlab);
	constexpr cItemStairsHandler            ItemSandstoneStairsHandler                 (Item::SandstoneStairs);
	constexpr cUnimplementedItemHandler     ItemSandstoneWallHandler                   (Item::SandstoneWall);
	constexpr cUnimplementedItemHandler     ItemScaffoldingHandler                     (Item::Scaffolding);
	constexpr cUnimplementedItemHandler     ItemScuteHandler                           (Item::Scute);
	constexpr cUnimplementedItemHandler     ItemSeaLanternHandler                      (Item::SeaLantern);
	constexpr cUnimplementedItemHandler     ItemSeaPickleHandler                       (Item::SeaPickle);
	constexpr cUnimplementedItemHandler     ItemSeagrassHandler                        (Item::Seagrass);
	constexpr cItemShearsHandler            ItemShearsHandler                          (Item::Shears);
	constexpr cItemSpawnEggHandler          ItemSheepSpawnEggHandler                   (Item::SheepSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemShieldHandler                          (Item::Shield);
	constexpr cUnimplementedItemHandler     ItemShroomlightHandler                     (Item::Shroomlight);
	constexpr cUnimplementedItemHandler     ItemShulkerBoxHandler                      (Item::ShulkerBox);
	constexpr cUnimplementedItemHandler     ItemShulkerShellHandler                    (Item::ShulkerShell);
	constexpr cItemSpawnEggHandler          ItemShulkerSpawnEggHandler                 (Item::ShulkerSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemSilverfishSpawnEggHandler              (Item::SilverfishSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemSkeletonHorseSpawnEggHandler           (Item::SkeletonHorseSpawnEgg);
	constexpr cItemMobHeadHandler           ItemSkeletonSkullHandler                   (Item::SkeletonSkull);
	constexpr cItemSpawnEggHandler          ItemSkeletonSpawnEggHandler                (Item::SkeletonSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemSkullBannerPatternHandler              (Item::SkullBannerPattern);
	constexpr cDefaultItemHandler           ItemSlimeBallHandler                       (Item::SlimeBall);
	constexpr cUnimplementedItemHandler     ItemSlimeBlockHandler                      (Item::SlimeBlock);
	constexpr cItemSpawnEggHandler          ItemSlimeSpawnEggHandler                   (Item::SlimeSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemSmithingTableHandler                   (Item::SmithingTable);
	constexpr cUnimplementedItemHandler     ItemSmokerHandler                          (Item::Smoker);
	constexpr cSimplePlaceableItemHandler   ItemSmoothQuartzHandler                    (Item::SmoothQuartz);
	constexpr cItemSlabHandler              ItemSmoothQuartzSlabHandler                (Item::SmoothQuartzSlab);
	constexpr cItemStairsHandler            ItemSmoothQuartzStairsHandler              (Item::SmoothQuartzStairs);
	constexpr cSimplePlaceableItemHandler   ItemSmoothRedSandstoneHandler              (Item::SmoothRedSandstone);
	constexpr cItemSlabHandler              ItemSmoothRedSandstoneSlabHandler          (Item::SmoothRedSandstoneSlab);
	constexpr cItemStairsHandler            ItemSmoothRedSandstoneStairsHandler        (Item::SmoothRedSandstoneStairs);
	constexpr cSimplePlaceableItemHandler   ItemSmoothSandstoneHandler                 (Item::SmoothSandstone);
	constexpr cItemSlabHandler              ItemSmoothSandstoneSlabHandler             (Item::SmoothSandstoneSlab);
	constexpr cItemStairsHandler            ItemSmoothSandstoneStairsHandler           (Item::SmoothSandstoneStairs);
	constexpr cSimplePlaceableItemHandler   ItemSmoothStoneHandler                     (Item::SmoothStone);
	constexpr cItemSlabHandler              ItemSmoothStoneSlabHandler                 (Item::SmoothStoneSlab);
	constexpr cSimplePlaceableItemHandler   ItemSnowBlockHandler                       (Item::SnowBlock);
	constexpr cItemSnowHandler              ItemSnowHandler                            (Item::Snow);
	constexpr cItemSnowballHandler          ItemSnowballHandler                        (Item::Snowball);
	constexpr cUnimplementedItemHandler     ItemSoulCampfireHandler                    (Item::SoulCampfire);
	constexpr cUnimplementedItemHandler     ItemSoulLanternHandler                     (Item::SoulLantern);
	constexpr cSimplePlaceableItemHandler   ItemSoulSandHandler                        (Item::SoulSand);
	constexpr cUnimplementedItemHandler     ItemSoulSoilHandler                        (Item::SoulSoil);
	constexpr cUnimplementedItemHandler     ItemSoulTorchHandler                       (Item::SoulTorch);
	constexpr cSimplePlaceableItemHandler   ItemSpawnerHandler                         (Item::Spawner);
	constexpr cUnimplementedItemHandler     ItemSpectralArrowHandler                   (Item::SpectralArrow);
	constexpr cItemSpiderEyeHandler         ItemSpiderEyeHandler                       (Item::SpiderEye);
	constexpr cItemSpawnEggHandler          ItemSpiderSpawnEggHandler                  (Item::SpiderSpawnEgg);
	constexpr cItemPotionHandler            ItemSplashPotionHandler                    (Item::SplashPotion);
	constexpr cUnimplementedItemHandler     ItemSpongeHandler                          (Item::Sponge);
	constexpr cItemBoatHandler              ItemSpruceBoatHandler                      (Item::SpruceBoat);
	constexpr cItemButtonHandler            ItemSpruceButtonHandler                    (Item::SpruceButton);
	constexpr cItemDoorHandler              ItemSpruceDoorHandler                      (Item::SpruceDoor);
	constexpr cItemFenceGateHandler         ItemSpruceFenceGateHandler                 (Item::SpruceFenceGate);
	constexpr cItemFenceHandler             ItemSpruceFenceHandler                     (Item::SpruceFence);
	constexpr cItemLeavesHandler            ItemSpruceLeavesHandler                    (Item::SpruceLeaves);
	constexpr cItemLogHandler               ItemSpruceLogHandler                       (Item::SpruceLog);
	constexpr cSimplePlaceableItemHandler   ItemSprucePlanksHandler                    (Item::SprucePlanks);
	constexpr cSimplePlaceableItemHandler   ItemSprucePressurePlateHandler             (Item::SprucePressurePlate);
	constexpr cSimplePlaceableItemHandler   ItemSpruceSaplingHandler                   (Item::SpruceSapling);
	constexpr cItemSignHandler              ItemSpruceSignHandler                      (Item::SpruceSign);
	constexpr cItemSlabHandler              ItemSpruceSlabHandler                      (Item::SpruceSlab);
	constexpr cItemStairsHandler            ItemSpruceStairsHandler                    (Item::SpruceStairs);
	constexpr cItemTrapdoorHandler          ItemSpruceTrapdoorHandler                  (Item::SpruceTrapdoor);
	constexpr cItemWoodHandler              ItemSpruceWoodHandler                      (Item::SpruceWood);
	constexpr cItemSpawnEggHandler          ItemSquidSpawnEggHandler                   (Item::SquidSpawnEgg);
	constexpr cDefaultItemHandler           ItemStickHandler                           (Item::Stick);
	constexpr cItemPistonHandler            ItemStickyPistonHandler                    (Item::StickyPiston);
	constexpr cItemAxeHandler               ItemStoneAxeHandler                        (Item::StoneAxe);
	constexpr cItemSlabHandler              ItemStoneBrickSlabHandler                  (Item::StoneBrickSlab);
	constexpr cItemStairsHandler            ItemStoneBrickStairsHandler                (Item::StoneBrickStairs);
	constexpr cUnimplementedItemHandler     ItemStoneBrickWallHandler                  (Item::StoneBrickWall);
	constexpr cSimplePlaceableItemHandler   ItemStoneBricksHandler                     (Item::StoneBricks);
	constexpr cItemButtonHandler            ItemStoneButtonHandler                     (Item::StoneButton);
	constexpr cSimplePlaceableItemHandler   ItemStoneHandler                           (Item::Stone);
	constexpr cItemHoeHandler               ItemStoneHoeHandler                        (Item::StoneHoe);
	constexpr cItemPickaxeHandler           ItemStonePickaxeHandler                    (Item::StonePickaxe);
	constexpr cSimplePlaceableItemHandler   ItemStonePressurePlateHandler              (Item::StonePressurePlate);
	constexpr cItemShovelHandler            ItemStoneShovelHandler                     (Item::StoneShovel);
	constexpr cItemSlabHandler              ItemStoneSlabHandler                       (Item::StoneSlab);
	constexpr cItemStairsHandler            ItemStoneStairsHandler                     (Item::StoneStairs);
	constexpr cItemSwordHandler             ItemStoneSwordHandler                      (Item::StoneSword);
	constexpr cUnimplementedItemHandler     ItemStonecutterHandler                     (Item::Stonecutter);
	constexpr cItemSpawnEggHandler          ItemStraySpawnEggHandler                   (Item::StraySpawnEgg);
	constexpr cItemSpawnEggHandler          ItemStriderSpawnEggHandler                 (Item::StriderSpawnEgg);
	constexpr cSimplePlaceableItemHandler   ItemStringHandler                          (Item::String);
	constexpr cUnimplementedItemHandler     ItemStrippedAcaciaLogHandler               (Item::StrippedAcaciaLog);
	constexpr cUnimplementedItemHandler     ItemStrippedAcaciaWoodHandler              (Item::StrippedAcaciaWood);
	constexpr cUnimplementedItemHandler     ItemStrippedBirchLogHandler                (Item::StrippedBirchLog);
	constexpr cUnimplementedItemHandler     ItemStrippedBirchWoodHandler               (Item::StrippedBirchWood);
	constexpr cUnimplementedItemHandler     ItemStrippedCrimsonHyphaeHandler           (Item::StrippedCrimsonHyphae);
	constexpr cUnimplementedItemHandler     ItemStrippedCrimsonStemHandler             (Item::StrippedCrimsonStem);
	constexpr cUnimplementedItemHandler     ItemStrippedDarkOakLogHandler              (Item::StrippedDarkOakLog);
	constexpr cUnimplementedItemHandler     ItemStrippedDarkOakWoodHandler             (Item::StrippedDarkOakWood);
	constexpr cUnimplementedItemHandler     ItemStrippedJungleLogHandler               (Item::StrippedJungleLog);
	constexpr cUnimplementedItemHandler     ItemStrippedJungleWoodHandler              (Item::StrippedJungleWood);
	constexpr cUnimplementedItemHandler     ItemStrippedOakLogHandler                  (Item::StrippedOakLog);
	constexpr cUnimplementedItemHandler     ItemStrippedOakWoodHandler                 (Item::StrippedOakWood);
	constexpr cUnimplementedItemHandler     ItemStrippedSpruceLogHandler               (Item::StrippedSpruceLog);
	constexpr cUnimplementedItemHandler     ItemStrippedSpruceWoodHandler              (Item::StrippedSpruceWood);
	constexpr cUnimplementedItemHandler     ItemStrippedWarpedHyphaeHandler            (Item::StrippedWarpedHyphae);
	constexpr cUnimplementedItemHandler     ItemStrippedWarpedStemHandler              (Item::StrippedWarpedStem);
	constexpr cUnimplementedItemHandler     ItemStructureBlockHandler                  (Item::StructureBlock);
	constexpr cUnimplementedItemHandler     ItemStructureVoidHandler                   (Item::StructureVoid);
	constexpr cSimplePlaceableItemHandler   ItemSugarCaneHandler                       (Item::SugarCane);
	constexpr cDefaultItemHandler           ItemSugarHandler                           (Item::Sugar);
	constexpr cUnimplementedItemHandler     ItemSunflowerHandler                       (Item::Sunflower);
	constexpr cUnimplementedItemHandler     ItemSuspiciousStewHandler                  (Item::SuspiciousStew);
	constexpr cUnimplementedItemHandler     ItemSweetBerriesHandler                    (Item::SweetBerries);
	constexpr cSimplePlaceableItemHandler   ItemTNTHandler                             (Item::TNT);
	constexpr cItemMinecartHandler          ItemTNTMinecartHandler                     (Item::TNTMinecart);
	constexpr cUnimplementedItemHandler     ItemTallGrassHandler                       (Item::TallGrass);
	constexpr cUnimplementedItemHandler     ItemTargetHandler                          (Item::Target);
	constexpr cUnimplementedItemHandler     ItemTerracottaHandler                      (Item::Terracotta);
	constexpr cUnimplementedItemHandler     ItemTippedArrowHandler                     (Item::TippedArrow);
	constexpr cItemTorchHandler             ItemTorchHandler                           (Item::Torch);
	constexpr cDefaultItemHandler           ItemTotemOfUndyingHandler                  (Item::TotemOfUndying);
	constexpr cItemSpawnEggHandler          ItemTraderLlamaSpawnEggHandler             (Item::TraderLlamaSpawnEgg);
	constexpr cItemChestHandler             ItemTrappedChestHandler                    (Item::TrappedChest);
	constexpr cUnimplementedItemHandler     ItemTridentHandler                         (Item::Trident);
	constexpr cItemTripwireHookHandler      ItemTripwireHookHandler                    (Item::TripwireHook);
	constexpr cUnimplementedItemHandler     ItemTropicalFishBucketHandler              (Item::TropicalFishBucket);
	constexpr cUnimplementedItemHandler     ItemTropicalFishHandler                    (Item::TropicalFish);
	constexpr cItemSpawnEggHandler          ItemTropicalFishSpawnEggHandler            (Item::TropicalFishSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemTubeCoralBlockHandler                  (Item::TubeCoralBlock);
	constexpr cUnimplementedItemHandler     ItemTubeCoralFanHandler                    (Item::TubeCoralFan);
	constexpr cUnimplementedItemHandler     ItemTubeCoralHandler                       (Item::TubeCoral);
	constexpr cUnimplementedItemHandler     ItemTurtleEggHandler                       (Item::TurtleEgg);
	constexpr cUnimplementedItemHandler     ItemTurtleHelmetHandler                    (Item::TurtleHelmet);
	constexpr cItemSpawnEggHandler          ItemTurtleSpawnEggHandler                  (Item::TurtleSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemTwistingVinesHandler                   (Item::TwistingVines);
	constexpr cItemSpawnEggHandler          ItemVexSpawnEggHandler                     (Item::VexSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemVillagerSpawnEggHandler                (Item::VillagerSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemVindicatorSpawnEggHandler              (Item::VindicatorSpawnEgg);
	constexpr cItemVinesHandler             ItemVineHandler                            (Item::Vine);
	constexpr cItemSpawnEggHandler          ItemWanderingTraderSpawnEggHandler         (Item::WanderingTraderSpawnEgg);
	constexpr cItemButtonHandler            ItemWarpedButtonHandler                    (Item::WarpedButton);
	constexpr cItemDoorHandler              ItemWarpedDoorHandler                      (Item::WarpedDoor);
	constexpr cItemFenceGateHandler         ItemWarpedFenceGateHandler                 (Item::WarpedFenceGate);
	constexpr cItemFenceHandler             ItemWarpedFenceHandler                     (Item::WarpedFence);
	constexpr cUnimplementedItemHandler     ItemWarpedFungusHandler                    (Item::WarpedFungus);
	constexpr cUnimplementedItemHandler     ItemWarpedFungusOnA_stickHandler           (Item::WarpedFungusOnA_stick);
	constexpr cUnimplementedItemHandler     ItemWarpedHyphaeHandler                    (Item::WarpedHyphae);
	constexpr cUnimplementedItemHandler     ItemWarpedNyliumHandler                    (Item::WarpedNylium);
	constexpr cSimplePlaceableItemHandler   ItemWarpedPlanksHandler                    (Item::WarpedPlanks);
	constexpr cSimplePlaceableItemHandler   ItemWarpedPressurePlateHandler             (Item::WarpedPressurePlate);
	constexpr cUnimplementedItemHandler     ItemWarpedRootsHandler                     (Item::WarpedRoots);
	constexpr cItemSignHandler              ItemWarpedSignHandler                      (Item::WarpedSign);
	constexpr cItemSlabHandler              ItemWarpedSlabHandler                      (Item::WarpedSlab);
	constexpr cItemStairsHandler            ItemWarpedStairsHandler                    (Item::WarpedStairs);
	constexpr cItemLogHandler               ItemWarpedStemHandler                      (Item::WarpedStem);
	constexpr cItemTrapdoorHandler          ItemWarpedTrapdoorHandler                  (Item::WarpedTrapdoor);
	constexpr cUnimplementedItemHandler     ItemWarpedWartBlockHandler                 (Item::WarpedWartBlock);
	constexpr cItemBucketHandler            ItemWaterBucketHandler                     (Item::WaterBucket);
	constexpr cUnimplementedItemHandler     ItemWeepingVinesHandler                    (Item::WeepingVines);
	constexpr cUnimplementedItemHandler     ItemWetSpongeHandler                       (Item::WetSponge);
	constexpr cDefaultItemHandler           ItemWheatHandler                           (Item::Wheat);
	constexpr cItemSimpleSeedsHandler       ItemWheatSeedsHandler                      (Item::WheatSeeds);
	constexpr cItemBannerHandler            ItemWhiteBannerHandler                     (Item::WhiteBanner);
	constexpr cItemBedHandler               ItemWhiteBedHandler                        (Item::WhiteBed);
	constexpr cSimplePlaceableItemHandler   ItemWhiteCarpetHandler                     (Item::WhiteCarpet);
	constexpr cUnimplementedItemHandler     ItemWhiteConcreteHandler                   (Item::WhiteConcrete);
	constexpr cUnimplementedItemHandler     ItemWhiteConcretePowderHandler             (Item::WhiteConcretePowder);
	constexpr cItemDyeHandler               ItemWhiteDyeHandler                        (Item::WhiteDye);
	constexpr cItemGlazedTerracottaHandler  ItemWhiteGlazedTerracottaHandler           (Item::WhiteGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemWhiteShulkerBoxHandler                 (Item::WhiteShulkerBox);
	constexpr cUnimplementedItemHandler     ItemWhiteStainedGlassHandler               (Item::WhiteStainedGlass);
	constexpr cUnimplementedItemHandler     ItemWhiteStainedGlassPaneHandler           (Item::WhiteStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemWhiteTerracottaHandler                 (Item::WhiteTerracotta);
	constexpr cUnimplementedItemHandler     ItemWhiteTulipHandler                      (Item::WhiteTulip);
	constexpr cItemWoolHandler              ItemWhiteWoolHandler                       (Item::WhiteWool);
	constexpr cItemSpawnEggHandler          ItemWitchSpawnEggHandler                   (Item::WitchSpawnEgg);
	constexpr cUnimplementedItemHandler     ItemWitherRoseHandler                      (Item::WitherRose);
	constexpr cItemMobHeadHandler           ItemWitherSkeletonSkullHandler             (Item::WitherSkeletonSkull);
	constexpr cItemSpawnEggHandler          ItemWitherSkeletonSpawnEggHandler          (Item::WitherSkeletonSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemWolfSpawnEggHandler                    (Item::WolfSpawnEgg);
	constexpr cItemAxeHandler               ItemWoodenAxeHandler                       (Item::WoodenAxe);
	constexpr cItemHoeHandler               ItemWoodenHoeHandler                       (Item::WoodenHoe);
	constexpr cItemPickaxeHandler           ItemWoodenPickaxeHandler                   (Item::WoodenPickaxe);
	constexpr cItemShovelHandler            ItemWoodenShovelHandler                    (Item::WoodenShovel);
	constexpr cItemSwordHandler             ItemWoodenSwordHandler                     (Item::WoodenSword);
	constexpr cDefaultItemHandler           ItemWritableBookHandler                    (Item::WritableBook);
	constexpr cDefaultItemHandler           ItemWrittenBookHandler                     (Item::WrittenBook);
	constexpr cItemBannerHandler            ItemYellowBannerHandler                    (Item::YellowBanner);
	constexpr cItemBedHandler               ItemYellowBedHandler                       (Item::YellowBed);
	constexpr cSimplePlaceableItemHandler   ItemYellowCarpetHandler                    (Item::YellowCarpet);
	constexpr cUnimplementedItemHandler     ItemYellowConcreteHandler                  (Item::YellowConcrete);
	constexpr cUnimplementedItemHandler     ItemYellowConcretePowderHandler            (Item::YellowConcretePowder);
	constexpr cItemDyeHandler               ItemYellowDyeHandler                       (Item::YellowDye);
	constexpr cItemGlazedTerracottaHandler  ItemYellowGlazedTerracottaHandler          (Item::YellowGlazedTerracotta);
	constexpr cUnimplementedItemHandler     ItemYellowShulkerBoxHandler                (Item::YellowShulkerBox);
	constexpr cUnimplementedItemHandler     ItemYellowStainedGlassHandler              (Item::YellowStainedGlass);
	constexpr cUnimplementedItemHandler     ItemYellowStainedGlassPaneHandler          (Item::YellowStainedGlassPane);
	constexpr cUnimplementedItemHandler     ItemYellowTerracottaHandler                (Item::YellowTerracotta);
	constexpr cItemWoolHandler              ItemYellowWoolHandler                      (Item::YellowWool);
	constexpr cItemSpawnEggHandler          ItemZoglinSpawnEggHandler                  (Item::ZoglinSpawnEgg);
	constexpr cItemMobHeadHandler           ItemZombieHeadHandler                      (Item::ZombieHead);
	constexpr cItemSpawnEggHandler          ItemZombieHorseSpawnEggHandler             (Item::ZombieHorseSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemZombiePigmanSpawnEggHandler            (Item::ZombiePigmanSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemZombieSpawnEggHandler                  (Item::ZombieSpawnEgg);
	constexpr cItemSpawnEggHandler          ItemZombieVillagerSpawnEggHandler          (Item::ZombieVillagerSpawnEgg);
}





const cItemHandler & cItemHandler::For(Item a_ItemType)
{
	switch (a_ItemType)
	{
		case Item::AcaciaBoat:                       return ItemAcaciaBoatHandler;
		case Item::AcaciaButton:                     return ItemAcaciaButtonHandler;
		case Item::AcaciaDoor:                       return ItemAcaciaDoorHandler;
		case Item::AcaciaFence:                      return ItemAcaciaFenceHandler;
		case Item::AcaciaFenceGate:                  return ItemAcaciaFenceGateHandler;
		case Item::AcaciaLeaves:                     return ItemAcaciaLeavesHandler;
		case Item::AcaciaLog:                        return ItemAcaciaLogHandler;
		case Item::AcaciaPlanks:                     return ItemAcaciaPlanksHandler;
		case Item::AcaciaPressurePlate:              return ItemAcaciaPressurePlateHandler;
		case Item::AcaciaSapling:                    return ItemAcaciaSaplingHandler;
		case Item::AcaciaSign:                       return ItemAcaciaSignHandler;
		case Item::AcaciaSlab:                       return ItemAcaciaSlabHandler;
		case Item::AcaciaStairs:                     return ItemAcaciaStairsHandler;
		case Item::AcaciaTrapdoor:                   return ItemAcaciaTrapdoorHandler;
		case Item::AcaciaWood:                       return ItemAcaciaWoodHandler;
		case Item::ActivatorRail:                    return ItemActivatorRailHandler;
		case Item::Air:                              return ItemAirHandler;
		case Item::Allium:                           return ItemAlliumHandler;
		case Item::AncientDebris:                    return ItemAncientDebrisHandler;
		case Item::Andesite:                         return ItemAndesiteHandler;
		case Item::AndesiteSlab:                     return ItemAndesiteSlabHandler;
		case Item::AndesiteStairs:                   return ItemAndesiteStairsHandler;
		case Item::AndesiteWall:                     return ItemAndesiteWallHandler;
		case Item::Anvil:                            return ItemAnvilHandler;
		case Item::Apple:                            return ItemAppleHandler;
		case Item::ArmorStand:                       return ItemArmorStandHandler;
		case Item::Arrow:                            return ItemArrowHandler;
		case Item::AzureBluet:                       return ItemAzureBluetHandler;
		case Item::BakedPotato:                      return ItemBakedPotatoHandler;
		case Item::Bamboo:                           return ItemBambooHandler;
		case Item::Barrel:                           return ItemBarrelHandler;
		case Item::Barrier:                          return ItemBarrierHandler;
		case Item::Basalt:                           return ItemBasaltHandler;
		case Item::BatSpawnEgg:                      return ItemBatSpawnEggHandler;
		case Item::Beacon:                           return ItemBeaconHandler;
		case Item::Bedrock:                          return ItemBedrockHandler;
		case Item::BeeNest:                          return ItemBeeNestHandler;
		case Item::BeeSpawnEgg:                      return ItemBeeSpawnEggHandler;
		case Item::Beef:                             return ItemBeefHandler;
		case Item::Beehive:                          return ItemBeehiveHandler;
		case Item::Beetroot:                         return ItemBeetrootHandler;
		case Item::BeetrootSeeds:                    return ItemBeetrootSeedsHandler;
		case Item::BeetrootSoup:                     return ItemBeetrootSoupHandler;
		case Item::Bell:                             return ItemBellHandler;
		case Item::BirchBoat:                        return ItemBirchBoatHandler;
		case Item::BirchButton:                      return ItemBirchButtonHandler;
		case Item::BirchDoor:                        return ItemBirchDoorHandler;
		case Item::BirchFence:                       return ItemBirchFenceHandler;
		case Item::BirchFenceGate:                   return ItemBirchFenceGateHandler;
		case Item::BirchLeaves:                      return ItemBirchLeavesHandler;
		case Item::BirchLog:                         return ItemBirchLogHandler;
		case Item::BirchPlanks:                      return ItemBirchPlanksHandler;
		case Item::BirchPressurePlate:               return ItemBirchPressurePlateHandler;
		case Item::BirchSapling:                     return ItemBirchSaplingHandler;
		case Item::BirchSign:                        return ItemBirchSignHandler;
		case Item::BirchSlab:                        return ItemBirchSlabHandler;
		case Item::BirchStairs:                      return ItemBirchStairsHandler;
		case Item::BirchTrapdoor:                    return ItemBirchTrapdoorHandler;
		case Item::BirchWood:                        return ItemBirchWoodHandler;
		case Item::BlackBanner:                      return ItemBlackBannerHandler;
		case Item::BlackBed:                         return ItemBlackBedHandler;
		case Item::BlackCarpet:                      return ItemBlackCarpetHandler;
		case Item::BlackConcrete:                    return ItemBlackConcreteHandler;
		case Item::BlackConcretePowder:              return ItemBlackConcretePowderHandler;
		case Item::BlackDye:                         return ItemBlackDyeHandler;
		case Item::BlackGlazedTerracotta:            return ItemBlackGlazedTerracottaHandler;
		case Item::BlackShulkerBox:                  return ItemBlackShulkerBoxHandler;
		case Item::BlackStainedGlass:                return ItemBlackStainedGlassHandler;
		case Item::BlackStainedGlassPane:            return ItemBlackStainedGlassPaneHandler;
		case Item::BlackTerracotta:                  return ItemBlackTerracottaHandler;
		case Item::BlackWool:                        return ItemBlackWoolHandler;
		case Item::Blackstone:                       return ItemBlackstoneHandler;
		case Item::BlackstoneSlab:                   return ItemBlackstoneSlabHandler;
		case Item::BlackstoneStairs:                 return ItemBlackstoneStairsHandler;
		case Item::BlackstoneWall:                   return ItemBlackstoneWallHandler;
		case Item::BlastFurnace:                     return ItemBlastFurnaceHandler;
		case Item::BlazePowder:                      return ItemBlazePowderHandler;
		case Item::BlazeRod:                         return ItemBlazeRodHandler;
		case Item::BlazeSpawnEgg:                    return ItemBlazeSpawnEggHandler;
		case Item::BlueBanner:                       return ItemBlueBannerHandler;
		case Item::BlueBed:                          return ItemBlueBedHandler;
		case Item::BlueCarpet:                       return ItemBlueCarpetHandler;
		case Item::BlueConcrete:                     return ItemBlueConcreteHandler;
		case Item::BlueConcretePowder:               return ItemBlueConcretePowderHandler;
		case Item::BlueDye:                          return ItemBlueDyeHandler;
		case Item::BlueGlazedTerracotta:             return ItemBlueGlazedTerracottaHandler;
		case Item::BlueIce:                          return ItemBlueIceHandler;
		case Item::BlueOrchid:                       return ItemBlueOrchidHandler;
		case Item::BlueShulkerBox:                   return ItemBlueShulkerBoxHandler;
		case Item::BlueStainedGlass:                 return ItemBlueStainedGlassHandler;
		case Item::BlueStainedGlassPane:             return ItemBlueStainedGlassPaneHandler;
		case Item::BlueTerracotta:                   return ItemBlueTerracottaHandler;
		case Item::BlueWool:                         return ItemBlueWoolHandler;
		case Item::Bone:                             return ItemBoneHandler;
		case Item::BoneBlock:                        return ItemBoneBlockHandler;
		case Item::BoneMeal:                         return ItemBoneMealHandler;
		case Item::Book:                             return ItemBookHandler;
		case Item::Bookshelf:                        return ItemBookshelfHandler;
		case Item::Bow:                              return ItemBowHandler;
		case Item::Bowl:                             return ItemBowlHandler;
		case Item::BrainCoral:                       return ItemBrainCoralHandler;
		case Item::BrainCoralBlock:                  return ItemBrainCoralBlockHandler;
		case Item::BrainCoralFan:                    return ItemBrainCoralFanHandler;
		case Item::Bread:                            return ItemBreadHandler;
		case Item::BrewingStand:                     return ItemBrewingStandHandler;
		case Item::Brick:                            return ItemBrickHandler;
		case Item::BrickSlab:                        return ItemBrickSlabHandler;
		case Item::BrickStairs:                      return ItemBrickStairsHandler;
		case Item::BrickWall:                        return ItemBrickWallHandler;
		case Item::Bricks:                           return ItemBricksHandler;
		case Item::BrownBanner:                      return ItemBrownBannerHandler;
		case Item::BrownBed:                         return ItemBrownBedHandler;
		case Item::BrownCarpet:                      return ItemBrownCarpetHandler;
		case Item::BrownConcrete:                    return ItemBrownConcreteHandler;
		case Item::BrownConcretePowder:              return ItemBrownConcretePowderHandler;
		case Item::BrownDye:                         return ItemBrownDyeHandler;
		case Item::BrownGlazedTerracotta:            return ItemBrownGlazedTerracottaHandler;
		case Item::BrownMushroom:                    return ItemBrownMushroomHandler;
		case Item::BrownMushroomBlock:               return ItemBrownMushroomBlockHandler;
		case Item::BrownShulkerBox:                  return ItemBrownShulkerBoxHandler;
		case Item::BrownStainedGlass:                return ItemBrownStainedGlassHandler;
		case Item::BrownStainedGlassPane:            return ItemBrownStainedGlassPaneHandler;
		case Item::BrownTerracotta:                  return ItemBrownTerracottaHandler;
		case Item::BrownWool:                        return ItemBrownWoolHandler;
		case Item::BubbleCoral:                      return ItemBubbleCoralHandler;
		case Item::BubbleCoralBlock:                 return ItemBubbleCoralBlockHandler;
		case Item::BubbleCoralFan:                   return ItemBubbleCoralFanHandler;
		case Item::Bucket:                           return ItemBucketHandler;
		case Item::Cactus:                           return ItemCactusHandler;
		case Item::Cake:                             return ItemCakeHandler;
		case Item::Campfire:                         return ItemCampfireHandler;
		case Item::Carrot:                           return ItemCarrotHandler;
		case Item::CarrotOnAStick:                   return ItemCarrotOnAStickHandler;
		case Item::CartographyTable:                 return ItemCartographyTableHandler;
		case Item::CarvedPumpkin:                    return ItemCarvedPumpkinHandler;
		case Item::CatSpawnEgg:                      return ItemCatSpawnEggHandler;
		case Item::Cauldron:                         return ItemCauldronHandler;
		case Item::CaveSpiderSpawnEgg:               return ItemCaveSpiderSpawnEggHandler;
		case Item::Chain:                            return ItemChainHandler;
		case Item::ChainCommandBlock:                return ItemChainCommandBlockHandler;
		case Item::ChainmailBoots:                   return ItemChainmailBootsHandler;
		case Item::ChainmailChestplate:              return ItemChainmailChestplateHandler;
		case Item::ChainmailHelmet:                  return ItemChainmailHelmetHandler;
		case Item::ChainmailLeggings:                return ItemChainmailLeggingsHandler;
		case Item::Charcoal:                         return ItemCharcoalHandler;
		case Item::Chest:                            return ItemChestHandler;
		case Item::ChestMinecart:                    return ItemChestMinecartHandler;
		case Item::Chicken:                          return ItemChickenHandler;
		case Item::ChickenSpawnEgg:                  return ItemChickenSpawnEggHandler;
		case Item::ChippedAnvil:                     return ItemChippedAnvilHandler;
		case Item::ChiseledNetherBricks:             return ItemChiseledNetherBricksHandler;
		case Item::ChiseledPolishedBlackstone:       return ItemChiseledPolishedBlackstoneHandler;
		case Item::ChiseledQuartzBlock:              return ItemChiseledQuartzBlockHandler;
		case Item::ChiseledRedSandstone:             return ItemChiseledRedSandstoneHandler;
		case Item::ChiseledSandstone:                return ItemChiseledSandstoneHandler;
		case Item::ChiseledStoneBricks:              return ItemChiseledStoneBricksHandler;
		case Item::ChorusFlower:                     return ItemChorusFlowerHandler;
		case Item::ChorusFruit:                      return ItemChorusFruitHandler;
		case Item::ChorusPlant:                      return ItemChorusPlantHandler;
		case Item::Clay:                             return ItemClayHandler;
		case Item::ClayBall:                         return ItemClayBallHandler;
		case Item::Clock:                            return ItemClockHandler;
		case Item::Coal:                             return ItemCoalHandler;
		case Item::CoalBlock:                        return ItemCoalBlockHandler;
		case Item::CoalOre:                          return ItemCoalOreHandler;
		case Item::CoarseDirt:                       return ItemCoarseDirtHandler;
		case Item::Cobblestone:                      return ItemCobblestoneHandler;
		case Item::CobblestoneSlab:                  return ItemCobblestoneSlabHandler;
		case Item::CobblestoneStairs:                return ItemCobblestoneStairsHandler;
		case Item::CobblestoneWall:                  return ItemCobblestoneWallHandler;
		case Item::Cobweb:                           return ItemCobwebHandler;
		case Item::CocoaBeans:                       return ItemCocoaBeansHandler;
		case Item::Cod:                              return ItemCodHandler;
		case Item::CodBucket:                        return ItemCodBucketHandler;
		case Item::CodSpawnEgg:                      return ItemCodSpawnEggHandler;
		case Item::CommandBlock:                     return ItemCommandBlockHandler;
		case Item::CommandBlockMinecart:             return ItemCommandBlockMinecartHandler;
		case Item::Comparator:                       return ItemComparatorHandler;
		case Item::Compass:                          return ItemCompassHandler;
		case Item::Composter:                        return ItemComposterHandler;
		case Item::Conduit:                          return ItemConduitHandler;
		case Item::CookedBeef:                       return ItemCookedBeefHandler;
		case Item::CookedChicken:                    return ItemCookedChickenHandler;
		case Item::CookedCod:                        return ItemCookedCodHandler;
		case Item::CookedMutton:                     return ItemCookedMuttonHandler;
		case Item::CookedPorkchop:                   return ItemCookedPorkchopHandler;
		case Item::CookedRabbit:                     return ItemCookedRabbitHandler;
		case Item::CookedSalmon:                     return ItemCookedSalmonHandler;
		case Item::Cookie:                           return ItemCookieHandler;
		case Item::Cornflower:                       return ItemCornflowerHandler;
		case Item::CowSpawnEgg:                      return ItemCowSpawnEggHandler;
		case Item::CrackedNetherBricks:              return ItemCrackedNetherBricksHandler;
		case Item::CrackedPolishedBlackstoneBricks:  return ItemCrackedPolishedBlackstoneBricksHandler;
		case Item::CrackedStoneBricks:               return ItemCrackedStoneBricksHandler;
		case Item::CraftingTable:                    return ItemCraftingTableHandler;
		case Item::CreeperBannerPattern:             return ItemCreeperBannerPatternHandler;
		case Item::CreeperHead:                      return ItemCreeperHeadHandler;
		case Item::CreeperSpawnEgg:                  return ItemCreeperSpawnEggHandler;
		case Item::CrimsonButton:                    return ItemCrimsonButtonHandler;
		case Item::CrimsonDoor:                      return ItemCrimsonDoorHandler;
		case Item::CrimsonFence:                     return ItemCrimsonFenceHandler;
		case Item::CrimsonFenceGate:                 return ItemCrimsonFenceGateHandler;
		case Item::CrimsonFungus:                    return ItemCrimsonFungusHandler;
		case Item::CrimsonHyphae:                    return ItemCrimsonHyphaeHandler;
		case Item::CrimsonNylium:                    return ItemCrimsonNyliumHandler;
		case Item::CrimsonPlanks:                    return ItemCrimsonPlanksHandler;
		case Item::CrimsonPressurePlate:             return ItemCrimsonPressurePlateHandler;
		case Item::CrimsonRoots:                     return ItemCrimsonRootsHandler;
		case Item::CrimsonSign:                      return ItemCrimsonSignHandler;
		case Item::CrimsonSlab:                      return ItemCrimsonSlabHandler;
		case Item::CrimsonStairs:                    return ItemCrimsonStairsHandler;
		case Item::CrimsonStem:                      return ItemCrimsonStemHandler;
		case Item::CrimsonTrapdoor:                  return ItemCrimsonTrapdoorHandler;
		case Item::Crossbow:                         return ItemCrossbowHandler;
		case Item::CryingObsidian:                   return ItemCryingObsidianHandler;
		case Item::CutRedSandstone:                  return ItemCutRedSandstoneHandler;
		case Item::CutRedSandstoneSlab:              return ItemCutRedSandstoneSlabHandler;
		case Item::CutSandstone:                     return ItemCutSandstoneHandler;
		case Item::CutSandstoneSlab:                 return ItemCutSandstoneSlabHandler;
		case Item::CyanBanner:                       return ItemCyanBannerHandler;
		case Item::CyanBed:                          return ItemCyanBedHandler;
		case Item::CyanCarpet:                       return ItemCyanCarpetHandler;
		case Item::CyanConcrete:                     return ItemCyanConcreteHandler;
		case Item::CyanConcretePowder:               return ItemCyanConcretePowderHandler;
		case Item::CyanDye:                          return ItemCyanDyeHandler;
		case Item::CyanGlazedTerracotta:             return ItemCyanGlazedTerracottaHandler;
		case Item::CyanShulkerBox:                   return ItemCyanShulkerBoxHandler;
		case Item::CyanStainedGlass:                 return ItemCyanStainedGlassHandler;
		case Item::CyanStainedGlassPane:             return ItemCyanStainedGlassPaneHandler;
		case Item::CyanTerracotta:                   return ItemCyanTerracottaHandler;
		case Item::CyanWool:                         return ItemCyanWoolHandler;
		case Item::DamagedAnvil:                     return ItemDamagedAnvilHandler;
		case Item::Dandelion:                        return ItemDandelionHandler;
		case Item::DarkOakBoat:                      return ItemDarkOakBoatHandler;
		case Item::DarkOakButton:                    return ItemDarkOakButtonHandler;
		case Item::DarkOakDoor:                      return ItemDarkOakDoorHandler;
		case Item::DarkOakFence:                     return ItemDarkOakFenceHandler;
		case Item::DarkOakFenceGate:                 return ItemDarkOakFenceGateHandler;
		case Item::DarkOakLeaves:                    return ItemDarkOakLeavesHandler;
		case Item::DarkOakLog:                       return ItemDarkOakLogHandler;
		case Item::DarkOakPlanks:                    return ItemDarkOakPlanksHandler;
		case Item::DarkOakPressurePlate:             return ItemDarkOakPressurePlateHandler;
		case Item::DarkOakSapling:                   return ItemDarkOakSaplingHandler;
		case Item::DarkOakSign:                      return ItemDarkOakSignHandler;
		case Item::DarkOakSlab:                      return ItemDarkOakSlabHandler;
		case Item::DarkOakStairs:                    return ItemDarkOakStairsHandler;
		case Item::DarkOakTrapdoor:                  return ItemDarkOakTrapdoorHandler;
		case Item::DarkOakWood:                      return ItemDarkOakWoodHandler;
		case Item::DarkPrismarine:                   return ItemDarkPrismarineHandler;
		case Item::DarkPrismarineSlab:               return ItemDarkPrismarineSlabHandler;
		case Item::DarkPrismarineStairs:             return ItemDarkPrismarineStairsHandler;
		case Item::DaylightDetector:                 return ItemDaylightDetectorHandler;
		case Item::DeadBrainCoral:                   return ItemDeadBrainCoralHandler;
		case Item::DeadBrainCoralBlock:              return ItemDeadBrainCoralBlockHandler;
		case Item::DeadBrainCoralFan:                return ItemDeadBrainCoralFanHandler;
		case Item::DeadBubbleCoral:                  return ItemDeadBubbleCoralHandler;
		case Item::DeadBubbleCoralBlock:             return ItemDeadBubbleCoralBlockHandler;
		case Item::DeadBubbleCoralFan:               return ItemDeadBubbleCoralFanHandler;
		case Item::DeadBush:                         return ItemDeadBushHandler;
		case Item::DeadFireCoral:                    return ItemDeadFireCoralHandler;
		case Item::DeadFireCoralBlock:               return ItemDeadFireCoralBlockHandler;
		case Item::DeadFireCoralFan:                 return ItemDeadFireCoralFanHandler;
		case Item::DeadHornCoral:                    return ItemDeadHornCoralHandler;
		case Item::DeadHornCoralBlock:               return ItemDeadHornCoralBlockHandler;
		case Item::DeadHornCoralFan:                 return ItemDeadHornCoralFanHandler;
		case Item::DeadTubeCoral:                    return ItemDeadTubeCoralHandler;
		case Item::DeadTubeCoralBlock:               return ItemDeadTubeCoralBlockHandler;
		case Item::DeadTubeCoralFan:                 return ItemDeadTubeCoralFanHandler;
		case Item::DebugStick:                       return ItemDebugStickHandler;
		case Item::DetectorRail:                     return ItemDetectorRailHandler;
		case Item::Diamond:                          return ItemDiamondHandler;
		case Item::DiamondAxe:                       return ItemDiamondAxeHandler;
		case Item::DiamondBlock:                     return ItemDiamondBlockHandler;
		case Item::DiamondBoots:                     return ItemDiamondBootsHandler;
		case Item::DiamondChestplate:                return ItemDiamondChestplateHandler;
		case Item::DiamondHelmet:                    return ItemDiamondHelmetHandler;
		case Item::DiamondHoe:                       return ItemDiamondHoeHandler;
		case Item::DiamondHorseArmor:                return ItemDiamondHorseArmorHandler;
		case Item::DiamondLeggings:                  return ItemDiamondLeggingsHandler;
		case Item::DiamondOre:                       return ItemDiamondOreHandler;
		case Item::DiamondPickaxe:                   return ItemDiamondPickaxeHandler;
		case Item::DiamondShovel:                    return ItemDiamondShovelHandler;
		case Item::DiamondSword:                     return ItemDiamondSwordHandler;
		case Item::Diorite:                          return ItemDioriteHandler;
		case Item::DioriteSlab:                      return ItemDioriteSlabHandler;
		case Item::DioriteStairs:                    return ItemDioriteStairsHandler;
		case Item::DioriteWall:                      return ItemDioriteWallHandler;
		case Item::Dirt:                             return ItemDirtHandler;
		case Item::Dispenser:                        return ItemDispenserHandler;
		case Item::DolphinSpawnEgg:                  return ItemDolphinSpawnEggHandler;
		case Item::DonkeySpawnEgg:                   return ItemDonkeySpawnEggHandler;
		case Item::DragonBreath:                     return ItemDragonBreathHandler;
		case Item::DragonEgg:                        return ItemDragonEggHandler;
		case Item::DragonHead:                       return ItemDragonHeadHandler;
		case Item::DriedKelp:                        return ItemDriedKelpHandler;
		case Item::DriedKelpBlock:                   return ItemDriedKelpBlockHandler;
		case Item::Dropper:                          return ItemDropperHandler;
		case Item::DrownedSpawnEgg:                  return ItemDrownedSpawnEggHandler;
		case Item::Egg:                              return ItemEggHandler;
		case Item::ElderGuardianSpawnEgg:            return ItemElderGuardianSpawnEggHandler;
		case Item::Elytra:                           return ItemElytraHandler;
		case Item::Emerald:                          return ItemEmeraldHandler;
		case Item::EmeraldBlock:                     return ItemEmeraldBlockHandler;
		case Item::EmeraldOre:                       return ItemEmeraldOreHandler;
		case Item::EnchantedBook:                    return ItemEnchantedBookHandler;
		case Item::EnchantedGoldenApple:             return ItemEnchantedGoldenAppleHandler;
		case Item::EnchantingTable:                  return ItemEnchantingTableHandler;
		case Item::EndCrystal:                       return ItemEndCrystalHandler;
		case Item::EndPortalFrame:                   return ItemEndPortalFrameHandler;
		case Item::EndRod:                           return ItemEndRodHandler;
		case Item::EndStone:                         return ItemEndStoneHandler;
		case Item::EndStoneBrickSlab:                return ItemEndStoneBrickSlabHandler;
		case Item::EndStoneBrickStairs:              return ItemEndStoneBrickStairsHandler;
		case Item::EndStoneBrickWall:                return ItemEndStoneBrickWallHandler;
		case Item::EndStoneBricks:                   return ItemEndStoneBricksHandler;
		case Item::EnderChest:                       return ItemEnderChestHandler;
		case Item::EnderEye:                         return ItemEnderEyeHandler;
		case Item::EnderPearl:                       return ItemEnderPearlHandler;
		case Item::EndermanSpawnEgg:                 return ItemEndermanSpawnEggHandler;
		case Item::EndermiteSpawnEgg:                return ItemEndermiteSpawnEggHandler;
		case Item::EvokerSpawnEgg:                   return ItemEvokerSpawnEggHandler;
		case Item::ExperienceBottle:                 return ItemExperienceBottleHandler;
		case Item::Farmland:                         return ItemFarmlandHandler;
		case Item::Feather:                          return ItemFeatherHandler;
		case Item::FermentedSpiderEye:               return ItemFermentedSpiderEyeHandler;
		case Item::Fern:                             return ItemFernHandler;
		case Item::FilledMap:                        return ItemFilledMapHandler;
		case Item::FireCharge:                       return ItemFireChargeHandler;
		case Item::FireCoral:                        return ItemFireCoralHandler;
		case Item::FireCoralBlock:                   return ItemFireCoralBlockHandler;
		case Item::FireCoralFan:                     return ItemFireCoralFanHandler;
		case Item::FireworkRocket:                   return ItemFireworkRocketHandler;
		case Item::FireworkStar:                     return ItemFireworkStarHandler;
		case Item::FishingRod:                       return ItemFishingRodHandler;
		case Item::FletchingTable:                   return ItemFletchingTableHandler;
		case Item::Flint:                            return ItemFlintHandler;
		case Item::FlintAndSteel:                    return ItemFlintAndSteelHandler;
		case Item::FlowerBannerPattern:              return ItemFlowerBannerPatternHandler;
		case Item::FlowerPot:                        return ItemFlowerPotHandler;
		case Item::FoxSpawnEgg:                      return ItemFoxSpawnEggHandler;
		case Item::Furnace:                          return ItemFurnaceHandler;
		case Item::FurnaceMinecart:                  return ItemFurnaceMinecartHandler;
		case Item::GhastSpawnEgg:                    return ItemGhastSpawnEggHandler;
		case Item::GhastTear:                        return ItemGhastTearHandler;
		case Item::GildedBlackstone:                 return ItemGildedBlackstoneHandler;
		case Item::Glass:                            return ItemGlassHandler;
		case Item::GlassBottle:                      return ItemGlassBottleHandler;
		case Item::GlassPane:                        return ItemGlassPaneHandler;
		case Item::GlisteringMelonSlice:             return ItemGlisteringMelonSliceHandler;
		case Item::GlobeBannerPattern:               return ItemGlobeBannerPatternHandler;
		case Item::Glowstone:                        return ItemGlowstoneHandler;
		case Item::GlowstoneDust:                    return ItemGlowstoneDustHandler;
		case Item::GoldBlock:                        return ItemGoldBlockHandler;
		case Item::GoldIngot:                        return ItemGoldIngotHandler;
		case Item::GoldNugget:                       return ItemGoldNuggetHandler;
		case Item::GoldOre:                          return ItemGoldOreHandler;
		case Item::GoldenApple:                      return ItemGoldenAppleHandler;
		case Item::GoldenAxe:                        return ItemGoldenAxeHandler;
		case Item::GoldenBoots:                      return ItemGoldenBootsHandler;
		case Item::GoldenCarrot:                     return ItemGoldenCarrotHandler;
		case Item::GoldenChestplate:                 return ItemGoldenChestplateHandler;
		case Item::GoldenHelmet:                     return ItemGoldenHelmetHandler;
		case Item::GoldenHoe:                        return ItemGoldenHoeHandler;
		case Item::GoldenHorseArmor:                 return ItemGoldenHorseArmorHandler;
		case Item::GoldenLeggings:                   return ItemGoldenLeggingsHandler;
		case Item::GoldenPickaxe:                    return ItemGoldenPickaxeHandler;
		case Item::GoldenShovel:                     return ItemGoldenShovelHandler;
		case Item::GoldenSword:                      return ItemGoldenSwordHandler;
		case Item::Granite:                          return ItemGraniteHandler;
		case Item::GraniteSlab:                      return ItemGraniteSlabHandler;
		case Item::GraniteStairs:                    return ItemGraniteStairsHandler;
		case Item::GraniteWall:                      return ItemGraniteWallHandler;
		case Item::Grass:                            return ItemGrassHandler;
		case Item::GrassBlock:                       return ItemGrassBlockHandler;
		case Item::GrassPath:                        return ItemGrassPathHandler;
		case Item::Gravel:                           return ItemGravelHandler;
		case Item::GrayBanner:                       return ItemGrayBannerHandler;
		case Item::GrayBed:                          return ItemGrayBedHandler;
		case Item::GrayCarpet:                       return ItemGrayCarpetHandler;
		case Item::GrayConcrete:                     return ItemGrayConcreteHandler;
		case Item::GrayConcretePowder:               return ItemGrayConcretePowderHandler;
		case Item::GrayDye:                          return ItemGrayDyeHandler;
		case Item::GrayGlazedTerracotta:             return ItemGrayGlazedTerracottaHandler;
		case Item::GrayShulkerBox:                   return ItemGrayShulkerBoxHandler;
		case Item::GrayStainedGlass:                 return ItemGrayStainedGlassHandler;
		case Item::GrayStainedGlassPane:             return ItemGrayStainedGlassPaneHandler;
		case Item::GrayTerracotta:                   return ItemGrayTerracottaHandler;
		case Item::GrayWool:                         return ItemGrayWoolHandler;
		case Item::GreenBanner:                      return ItemGreenBannerHandler;
		case Item::GreenBed:                         return ItemGreenBedHandler;
		case Item::GreenCarpet:                      return ItemGreenCarpetHandler;
		case Item::GreenConcrete:                    return ItemGreenConcreteHandler;
		case Item::GreenConcretePowder:              return ItemGreenConcretePowderHandler;
		case Item::GreenDye:                         return ItemGreenDyeHandler;
		case Item::GreenGlazedTerracotta:            return ItemGreenGlazedTerracottaHandler;
		case Item::GreenShulkerBox:                  return ItemGreenShulkerBoxHandler;
		case Item::GreenStainedGlass:                return ItemGreenStainedGlassHandler;
		case Item::GreenStainedGlassPane:            return ItemGreenStainedGlassPaneHandler;
		case Item::GreenTerracotta:                  return ItemGreenTerracottaHandler;
		case Item::GreenWool:                        return ItemGreenWoolHandler;
		case Item::Grindstone:                       return ItemGrindstoneHandler;
		case Item::GuardianSpawnEgg:                 return ItemGuardianSpawnEggHandler;
		case Item::Gunpowder:                        return ItemGunpowderHandler;
		case Item::HayBale:                          return ItemHayBaleHandler;
		case Item::HeartOfTheSea:                    return ItemHeartOfTheSeaHandler;
		case Item::HeavyWeightedPressurePlate:       return ItemHeavyWeightedPressurePlateHandler;
		case Item::HoglinSpawnEgg:                   return ItemHoglinSpawnEggHandler;
		case Item::HoneyBlock:                       return ItemHoneyBlockHandler;
		case Item::HoneyBottle:                      return ItemHoneyBottleHandler;
		case Item::Honeycomb:                        return ItemHoneycombHandler;
		case Item::HoneycombBlock:                   return ItemHoneycombBlockHandler;
		case Item::Hopper:                           return ItemHopperHandler;
		case Item::HopperMinecart:                   return ItemHopperMinecartHandler;
		case Item::HornCoral:                        return ItemHornCoralHandler;
		case Item::HornCoralBlock:                   return ItemHornCoralBlockHandler;
		case Item::HornCoralFan:                     return ItemHornCoralFanHandler;
		case Item::HorseSpawnEgg:                    return ItemHorseSpawnEggHandler;
		case Item::HuskSpawnEgg:                     return ItemHuskSpawnEggHandler;
		case Item::Ice:                              return ItemIceHandler;
		case Item::InfestedChiseledStoneBricks:      return ItemInfestedChiseledStoneBricksHandler;
		case Item::InfestedCobblestone:              return ItemInfestedCobblestoneHandler;
		case Item::InfestedCrackedStoneBricks:       return ItemInfestedCrackedStoneBricksHandler;
		case Item::InfestedMossyStoneBricks:         return ItemInfestedMossyStoneBricksHandler;
		case Item::InfestedStone:                    return ItemInfestedStoneHandler;
		case Item::InfestedStoneBricks:              return ItemInfestedStoneBricksHandler;
		case Item::InkSac:                           return ItemInkSacHandler;
		case Item::IronAxe:                          return ItemIronAxeHandler;
		case Item::IronBars:                         return ItemIronBarsHandler;
		case Item::IronBlock:                        return ItemIronBlockHandler;
		case Item::IronBoots:                        return ItemIronBootsHandler;
		case Item::IronChestplate:                   return ItemIronChestplateHandler;
		case Item::IronDoor:                         return ItemIronDoorHandler;
		case Item::IronHelmet:                       return ItemIronHelmetHandler;
		case Item::IronHoe:                          return ItemIronHoeHandler;
		case Item::IronHorseArmor:                   return ItemIronHorseArmorHandler;
		case Item::IronIngot:                        return ItemIronIngotHandler;
		case Item::IronLeggings:                     return ItemIronLeggingsHandler;
		case Item::IronNugget:                       return ItemIronNuggetHandler;
		case Item::IronOre:                          return ItemIronOreHandler;
		case Item::IronPickaxe:                      return ItemIronPickaxeHandler;
		case Item::IronShovel:                       return ItemIronShovelHandler;
		case Item::IronSword:                        return ItemIronSwordHandler;
		case Item::IronTrapdoor:                     return ItemIronTrapdoorHandler;
		case Item::ItemFrame:                        return ItemItemFrameHandler;
		case Item::JackOLantern:                     return ItemJackOLanternHandler;
		case Item::Jigsaw:                           return ItemJigsawHandler;
		case Item::Jukebox:                          return ItemJukeboxHandler;
		case Item::JungleBoat:                       return ItemJungleBoatHandler;
		case Item::JungleButton:                     return ItemJungleButtonHandler;
		case Item::JungleDoor:                       return ItemJungleDoorHandler;
		case Item::JungleFence:                      return ItemJungleFenceHandler;
		case Item::JungleFenceGate:                  return ItemJungleFenceGateHandler;
		case Item::JungleLeaves:                     return ItemJungleLeavesHandler;
		case Item::JungleLog:                        return ItemJungleLogHandler;
		case Item::JunglePlanks:                     return ItemJunglePlanksHandler;
		case Item::JunglePressurePlate:              return ItemJunglePressurePlateHandler;
		case Item::JungleSapling:                    return ItemJungleSaplingHandler;
		case Item::JungleSign:                       return ItemJungleSignHandler;
		case Item::JungleSlab:                       return ItemJungleSlabHandler;
		case Item::JungleStairs:                     return ItemJungleStairsHandler;
		case Item::JungleTrapdoor:                   return ItemJungleTrapdoorHandler;
		case Item::JungleWood:                       return ItemJungleWoodHandler;
		case Item::Kelp:                             return ItemKelpHandler;
		case Item::KnowledgeBook:                    return ItemKnowledgeBookHandler;
		case Item::Ladder:                           return ItemLadderHandler;
		case Item::Lantern:                          return ItemLanternHandler;
		case Item::LapisBlock:                       return ItemLapisBlockHandler;
		case Item::LapisLazuli:                      return ItemLapisLazuliHandler;
		case Item::LapisOre:                         return ItemLapisOreHandler;
		case Item::LargeFern:                        return ItemLargeFernHandler;
		case Item::LavaBucket:                       return ItemLavaBucketHandler;
		case Item::Lead:                             return ItemLeadHandler;
		case Item::Leather:                          return ItemLeatherHandler;
		case Item::LeatherBoots:                     return ItemLeatherBootsHandler;
		case Item::LeatherChestplate:                return ItemLeatherChestplateHandler;
		case Item::LeatherHelmet:                    return ItemLeatherHelmetHandler;
		case Item::LeatherHorseArmor:                return ItemLeatherHorseArmorHandler;
		case Item::LeatherLeggings:                  return ItemLeatherLeggingsHandler;
		case Item::Lectern:                          return ItemLecternHandler;
		case Item::Lever:                            return ItemLeverHandler;
		case Item::LightBlueBanner:                  return ItemLightBlueBannerHandler;
		case Item::LightBlueBed:                     return ItemLightBlueBedHandler;
		case Item::LightBlueCarpet:                  return ItemLightBlueCarpetHandler;
		case Item::LightBlueConcrete:                return ItemLightBlueConcreteHandler;
		case Item::LightBlueConcretePowder:          return ItemLightBlueConcretePowderHandler;
		case Item::LightBlueDye:                     return ItemLightBlueDyeHandler;
		case Item::LightBlueGlazedTerracotta:        return ItemLightBlueGlazedTerracottaHandler;
		case Item::LightBlueShulkerBox:              return ItemLightBlueShulkerBoxHandler;
		case Item::LightBlueStainedGlass:            return ItemLightBlueStainedGlassHandler;
		case Item::LightBlueStainedGlassPane:        return ItemLightBlueStainedGlassPaneHandler;
		case Item::LightBlueTerracotta:              return ItemLightBlueTerracottaHandler;
		case Item::LightBlueWool:                    return ItemLightBlueWoolHandler;
		case Item::LightGrayBanner:                  return ItemLightGrayBannerHandler;
		case Item::LightGrayBed:                     return ItemLightGrayBedHandler;
		case Item::LightGrayCarpet:                  return ItemLightGrayCarpetHandler;
		case Item::LightGrayConcrete:                return ItemLightGrayConcreteHandler;
		case Item::LightGrayConcretePowder:          return ItemLightGrayConcretePowderHandler;
		case Item::LightGrayDye:                     return ItemLightGrayDyeHandler;
		case Item::LightGrayGlazedTerracotta:        return ItemLightGrayGlazedTerracottaHandler;
		case Item::LightGrayShulkerBox:              return ItemLightGrayShulkerBoxHandler;
		case Item::LightGrayStainedGlass:            return ItemLightGrayStainedGlassHandler;
		case Item::LightGrayStainedGlassPane:        return ItemLightGrayStainedGlassPaneHandler;
		case Item::LightGrayTerracotta:              return ItemLightGrayTerracottaHandler;
		case Item::LightGrayWool:                    return ItemLightGrayWoolHandler;
		case Item::LightWeightedPressurePlate:       return ItemLightWeightedPressurePlateHandler;
		case Item::Lilac:                            return ItemLilacHandler;
		case Item::LilyOfTheValley:                  return ItemLilyOfTheValleyHandler;
		case Item::LilyPad:                          return ItemLilyPadHandler;
		case Item::LimeBanner:                       return ItemLimeBannerHandler;
		case Item::LimeBed:                          return ItemLimeBedHandler;
		case Item::LimeCarpet:                       return ItemLimeCarpetHandler;
		case Item::LimeConcrete:                     return ItemLimeConcreteHandler;
		case Item::LimeConcretePowder:               return ItemLimeConcretePowderHandler;
		case Item::LimeDye:                          return ItemLimeDyeHandler;
		case Item::LimeGlazedTerracotta:             return ItemLimeGlazedTerracottaHandler;
		case Item::LimeShulkerBox:                   return ItemLimeShulkerBoxHandler;
		case Item::LimeStainedGlass:                 return ItemLimeStainedGlassHandler;
		case Item::LimeStainedGlassPane:             return ItemLimeStainedGlassPaneHandler;
		case Item::LimeTerracotta:                   return ItemLimeTerracottaHandler;
		case Item::LimeWool:                         return ItemLimeWoolHandler;
		case Item::LingeringPotion:                  return ItemLingeringPotionHandler;
		case Item::LlamaSpawnEgg:                    return ItemLlamaSpawnEggHandler;
		case Item::Lodestone:                        return ItemLodestoneHandler;
		case Item::Loom:                             return ItemLoomHandler;
		case Item::MagentaBanner:                    return ItemMagentaBannerHandler;
		case Item::MagentaBed:                       return ItemMagentaBedHandler;
		case Item::MagentaCarpet:                    return ItemMagentaCarpetHandler;
		case Item::MagentaConcrete:                  return ItemMagentaConcreteHandler;
		case Item::MagentaConcretePowder:            return ItemMagentaConcretePowderHandler;
		case Item::MagentaDye:                       return ItemMagentaDyeHandler;
		case Item::MagentaGlazedTerracotta:          return ItemMagentaGlazedTerracottaHandler;
		case Item::MagentaShulkerBox:                return ItemMagentaShulkerBoxHandler;
		case Item::MagentaStainedGlass:              return ItemMagentaStainedGlassHandler;
		case Item::MagentaStainedGlassPane:          return ItemMagentaStainedGlassPaneHandler;
		case Item::MagentaTerracotta:                return ItemMagentaTerracottaHandler;
		case Item::MagentaWool:                      return ItemMagentaWoolHandler;
		case Item::MagmaBlock:                       return ItemMagmaBlockHandler;
		case Item::MagmaCream:                       return ItemMagmaCreamHandler;
		case Item::MagmaCubeSpawnEgg:                return ItemMagmaCubeSpawnEggHandler;
		case Item::Map:                              return ItemMapHandler;
		case Item::Melon:                            return ItemMelonHandler;
		case Item::MelonSeeds:                       return ItemMelonSeedsHandler;
		case Item::MelonSlice:                       return ItemMelonSliceHandler;
		case Item::MilkBucket:                       return ItemMilkBucketHandler;
		case Item::Minecart:                         return ItemMinecartHandler;
		case Item::MojangBannerPattern:              return ItemMojangBannerPatternHandler;
		case Item::MooshroomSpawnEgg:                return ItemMooshroomSpawnEggHandler;
		case Item::MossyCobblestone:                 return ItemMossyCobblestoneHandler;
		case Item::MossyCobblestoneSlab:             return ItemMossyCobblestoneSlabHandler;
		case Item::MossyCobblestoneStairs:           return ItemMossyCobblestoneStairsHandler;
		case Item::MossyCobblestoneWall:             return ItemMossyCobblestoneWallHandler;
		case Item::MossyStoneBrickSlab:              return ItemMossyStoneBrickSlabHandler;
		case Item::MossyStoneBrickStairs:            return ItemMossyStoneBrickStairsHandler;
		case Item::MossyStoneBrickWall:              return ItemMossyStoneBrickWallHandler;
		case Item::MossyStoneBricks:                 return ItemMossyStoneBricksHandler;
		case Item::MuleSpawnEgg:                     return ItemMuleSpawnEggHandler;
		case Item::MushroomStem:                     return ItemMushroomStemHandler;
		case Item::MushroomStew:                     return ItemMushroomStewHandler;
		case Item::MusicDiscBlocks:                  return ItemMusicDiscBlocksHandler;
		case Item::MusicDiscCat:                     return ItemMusicDiscCatHandler;
		case Item::MusicDiscChirp:                   return ItemMusicDiscChirpHandler;
		case Item::MusicDiscFar:                     return ItemMusicDiscFarHandler;
		case Item::MusicDiscMall:                    return ItemMusicDiscMallHandler;
		case Item::MusicDiscMellohi:                 return ItemMusicDiscMellohiHandler;
		case Item::MusicDiscPigstep:                 return ItemMusicDiscPigstepHandler;
		case Item::MusicDiscStal:                    return ItemMusicDiscStalHandler;
		case Item::MusicDiscStrad:                   return ItemMusicDiscStradHandler;
		case Item::MusicDiscWait:                    return ItemMusicDiscWaitHandler;
		case Item::MusicDiscWard:                    return ItemMusicDiscWardHandler;
		case Item::MusicDisc11:                      return ItemMusicDisc11Handler;
		case Item::MusicDisc13:                      return ItemMusicDisc13Handler;
		case Item::Mutton:                           return ItemMuttonHandler;
		case Item::Mycelium:                         return ItemMyceliumHandler;
		case Item::NameTag:                          return ItemNameTagHandler;
		case Item::NautilusShell:                    return ItemNautilusShellHandler;
		case Item::NetherBrick:                      return ItemNetherBrickHandler;
		case Item::NetherBrickFence:                 return ItemNetherBrickFenceHandler;
		case Item::NetherBrickSlab:                  return ItemNetherBrickSlabHandler;
		case Item::NetherBrickStairs:                return ItemNetherBrickStairsHandler;
		case Item::NetherBrickWall:                  return ItemNetherBrickWallHandler;
		case Item::NetherBricks:                     return ItemNetherBricksHandler;
		case Item::NetherGoldOre:                    return ItemNetherGoldOreHandler;
		case Item::NetherQuartzOre:                  return ItemNetherQuartzOreHandler;
		case Item::NetherSprouts:                    return ItemNetherSproutsHandler;
		case Item::NetherStar:                       return ItemNetherStarHandler;
		case Item::NetherWart:                       return ItemNetherWartHandler;
		case Item::NetherWartBlock:                  return ItemNetherWartBlockHandler;
		case Item::NetheriteAxe:                     return ItemNetheriteAxeHandler;
		case Item::NetheriteBlock:                   return ItemNetheriteBlockHandler;
		case Item::NetheriteBoots:                   return ItemNetheriteBootsHandler;
		case Item::NetheriteChestplate:              return ItemNetheriteChestplateHandler;
		case Item::NetheriteHelmet:                  return ItemNetheriteHelmetHandler;
		case Item::NetheriteHoe:                     return ItemNetheriteHoeHandler;
		case Item::NetheriteIngot:                   return ItemNetheriteIngotHandler;
		case Item::NetheriteLeggings:                return ItemNetheriteLeggingsHandler;
		case Item::NetheritePickaxe:                 return ItemNetheritePickaxeHandler;
		case Item::NetheriteScrap:                   return ItemNetheriteScrapHandler;
		case Item::NetheriteShovel:                  return ItemNetheriteShovelHandler;
		case Item::NetheriteSword:                   return ItemNetheriteSwordHandler;
		case Item::Netherrack:                       return ItemNetherrackHandler;
		case Item::NoteBlock:                        return ItemNoteBlockHandler;
		case Item::OakBoat:                          return ItemOakBoatHandler;
		case Item::OakButton:                        return ItemOakButtonHandler;
		case Item::OakDoor:                          return ItemOakDoorHandler;
		case Item::OakFence:                         return ItemOakFenceHandler;
		case Item::OakFenceGate:                     return ItemOakFenceGateHandler;
		case Item::OakLeaves:                        return ItemOakLeavesHandler;
		case Item::OakLog:                           return ItemOakLogHandler;
		case Item::OakPlanks:                        return ItemOakPlanksHandler;
		case Item::OakPressurePlate:                 return ItemOakPressurePlateHandler;
		case Item::OakSapling:                       return ItemOakSaplingHandler;
		case Item::OakSign:                          return ItemOakSignHandler;
		case Item::OakSlab:                          return ItemOakSlabHandler;
		case Item::OakStairs:                        return ItemOakStairsHandler;
		case Item::OakTrapdoor:                      return ItemOakTrapdoorHandler;
		case Item::OakWood:                          return ItemOakWoodHandler;
		case Item::Observer:                         return ItemObserverHandler;
		case Item::Obsidian:                         return ItemObsidianHandler;
		case Item::OcelotSpawnEgg:                   return ItemOcelotSpawnEggHandler;
		case Item::OrangeBanner:                     return ItemOrangeBannerHandler;
		case Item::OrangeBed:                        return ItemOrangeBedHandler;
		case Item::OrangeCarpet:                     return ItemOrangeCarpetHandler;
		case Item::OrangeConcrete:                   return ItemOrangeConcreteHandler;
		case Item::OrangeConcretePowder:             return ItemOrangeConcretePowderHandler;
		case Item::OrangeDye:                        return ItemOrangeDyeHandler;
		case Item::OrangeGlazedTerracotta:           return ItemOrangeGlazedTerracottaHandler;
		case Item::OrangeShulkerBox:                 return ItemOrangeShulkerBoxHandler;
		case Item::OrangeStainedGlass:               return ItemOrangeStainedGlassHandler;
		case Item::OrangeStainedGlassPane:           return ItemOrangeStainedGlassPaneHandler;
		case Item::OrangeTerracotta:                 return ItemOrangeTerracottaHandler;
		case Item::OrangeTulip:                      return ItemOrangeTulipHandler;
		case Item::OrangeWool:                       return ItemOrangeWoolHandler;
		case Item::OxeyeDaisy:                       return ItemOxeyeDaisyHandler;
		case Item::PackedIce:                        return ItemPackedIceHandler;
		case Item::Painting:                         return ItemPaintingHandler;
		case Item::PandaSpawnEgg:                    return ItemPandaSpawnEggHandler;
		case Item::Paper:                            return ItemPaperHandler;
		case Item::ParrotSpawnEgg:                   return ItemParrotSpawnEggHandler;
		case Item::Peony:                            return ItemPeonyHandler;
		case Item::PetrifiedOakSlab:                 return ItemPetrifiedOakSlabHandler;
		case Item::PhantomMembrane:                  return ItemPhantomMembraneHandler;
		case Item::PhantomSpawnEgg:                  return ItemPhantomSpawnEggHandler;
		case Item::PigSpawnEgg:                      return ItemPigSpawnEggHandler;
		case Item::PiglinBannerPattern:              return ItemPiglinBannerPatternHandler;
		case Item::PiglinSpawnEgg:                   return ItemPiglinSpawnEggHandler;
		case Item::PillagerSpawnEgg:                 return ItemPillagerSpawnEggHandler;
		case Item::PinkBanner:                       return ItemPinkBannerHandler;
		case Item::PinkBed:                          return ItemPinkBedHandler;
		case Item::PinkCarpet:                       return ItemPinkCarpetHandler;
		case Item::PinkConcrete:                     return ItemPinkConcreteHandler;
		case Item::PinkConcretePowder:               return ItemPinkConcretePowderHandler;
		case Item::PinkDye:                          return ItemPinkDyeHandler;
		case Item::PinkGlazedTerracotta:             return ItemPinkGlazedTerracottaHandler;
		case Item::PinkShulkerBox:                   return ItemPinkShulkerBoxHandler;
		case Item::PinkStainedGlass:                 return ItemPinkStainedGlassHandler;
		case Item::PinkStainedGlassPane:             return ItemPinkStainedGlassPaneHandler;
		case Item::PinkTerracotta:                   return ItemPinkTerracottaHandler;
		case Item::PinkTulip:                        return ItemPinkTulipHandler;
		case Item::PinkWool:                         return ItemPinkWoolHandler;
		case Item::Piston:                           return ItemPistonHandler;
		case Item::PlayerHead:                       return ItemPlayerHeadHandler;
		case Item::Podzol:                           return ItemPodzolHandler;
		case Item::PoisonousPotato:                  return ItemPoisonousPotatoHandler;
		case Item::PolarBearSpawnEgg:                return ItemPolarBearSpawnEggHandler;
		case Item::PolishedAndesite:                 return ItemPolishedAndesiteHandler;
		case Item::PolishedAndesiteSlab:             return ItemPolishedAndesiteSlabHandler;
		case Item::PolishedAndesiteStairs:           return ItemPolishedAndesiteStairsHandler;
		case Item::PolishedBasalt:                   return ItemPolishedBasaltHandler;
		case Item::PolishedBlackstone:               return ItemPolishedBlackstoneHandler;
		case Item::PolishedBlackstoneBrickSlab:      return ItemPolishedBlackstoneBrickSlabHandler;
		case Item::PolishedBlackstoneBrickStairs:    return ItemPolishedBlackstoneBrickStairsHandler;
		case Item::PolishedBlackstoneBrickWall:      return ItemPolishedBlackstoneBrickWallHandler;
		case Item::PolishedBlackstoneBricks:         return ItemPolishedBlackstoneBricksHandler;
		case Item::PolishedBlackstoneButton:         return ItemPolishedBlackstoneButtonHandler;
		case Item::PolishedBlackstonePressurePlate:  return ItemPolishedBlackstonePressurePlateHandler;
		case Item::PolishedBlackstoneSlab:           return ItemPolishedBlackstoneSlabHandler;
		case Item::PolishedBlackstoneStairs:         return ItemPolishedBlackstoneStairsHandler;
		case Item::PolishedBlackstoneWall:           return ItemPolishedBlackstoneWallHandler;
		case Item::PolishedDiorite:                  return ItemPolishedDioriteHandler;
		case Item::PolishedDioriteSlab:              return ItemPolishedDioriteSlabHandler;
		case Item::PolishedDioriteStairs:            return ItemPolishedDioriteStairsHandler;
		case Item::PolishedGranite:                  return ItemPolishedGraniteHandler;
		case Item::PolishedGraniteSlab:              return ItemPolishedGraniteSlabHandler;
		case Item::PolishedGraniteStairs:            return ItemPolishedGraniteStairsHandler;
		case Item::PoppedChorusFruit:                return ItemPoppedChorusFruitHandler;
		case Item::Poppy:                            return ItemPoppyHandler;
		case Item::Porkchop:                         return ItemPorkchopHandler;
		case Item::Potato:                           return ItemPotatoHandler;
		case Item::Potion:                           return ItemPotionHandler;
		case Item::PoweredRail:                      return ItemPoweredRailHandler;
		case Item::Prismarine:                       return ItemPrismarineHandler;
		case Item::PrismarineBrickSlab:              return ItemPrismarineBrickSlabHandler;
		case Item::PrismarineBrickStairs:            return ItemPrismarineBrickStairsHandler;
		case Item::PrismarineBricks:                 return ItemPrismarineBricksHandler;
		case Item::PrismarineCrystals:               return ItemPrismarineCrystalsHandler;
		case Item::PrismarineShard:                  return ItemPrismarineShardHandler;
		case Item::PrismarineSlab:                   return ItemPrismarineSlabHandler;
		case Item::PrismarineStairs:                 return ItemPrismarineStairsHandler;
		case Item::PrismarineWall:                   return ItemPrismarineWallHandler;
		case Item::Pufferfish:                       return ItemPufferfishHandler;
		case Item::PufferfishBucket:                 return ItemPufferfishBucketHandler;
		case Item::PufferfishSpawnEgg:               return ItemPufferfishSpawnEggHandler;
		case Item::Pumpkin:                          return ItemPumpkinHandler;
		case Item::PumpkinPie:                       return ItemPumpkinPieHandler;
		case Item::PumpkinSeeds:                     return ItemPumpkinSeedsHandler;
		case Item::PurpleBanner:                     return ItemPurpleBannerHandler;
		case Item::PurpleBed:                        return ItemPurpleBedHandler;
		case Item::PurpleCarpet:                     return ItemPurpleCarpetHandler;
		case Item::PurpleConcrete:                   return ItemPurpleConcreteHandler;
		case Item::PurpleConcretePowder:             return ItemPurpleConcretePowderHandler;
		case Item::PurpleDye:                        return ItemPurpleDyeHandler;
		case Item::PurpleGlazedTerracotta:           return ItemPurpleGlazedTerracottaHandler;
		case Item::PurpleShulkerBox:                 return ItemPurpleShulkerBoxHandler;
		case Item::PurpleStainedGlass:               return ItemPurpleStainedGlassHandler;
		case Item::PurpleStainedGlassPane:           return ItemPurpleStainedGlassPaneHandler;
		case Item::PurpleTerracotta:                 return ItemPurpleTerracottaHandler;
		case Item::PurpleWool:                       return ItemPurpleWoolHandler;
		case Item::PurpurBlock:                      return ItemPurpurBlockHandler;
		case Item::PurpurPillar:                     return ItemPurpurPillarHandler;
		case Item::PurpurSlab:                       return ItemPurpurSlabHandler;
		case Item::PurpurStairs:                     return ItemPurpurStairsHandler;
		case Item::Quartz:                           return ItemQuartzHandler;
		case Item::QuartzBlock:                      return ItemQuartzBlockHandler;
		case Item::QuartzBricks:                     return ItemQuartzBricksHandler;
		case Item::QuartzPillar:                     return ItemQuartzPillarHandler;
		case Item::QuartzSlab:                       return ItemQuartzSlabHandler;
		case Item::QuartzStairs:                     return ItemQuartzStairsHandler;
		case Item::Rabbit:                           return ItemRabbitHandler;
		case Item::RabbitFoot:                       return ItemRabbitFootHandler;
		case Item::RabbitHide:                       return ItemRabbitHideHandler;
		case Item::RabbitSpawnEgg:                   return ItemRabbitSpawnEggHandler;
		case Item::RabbitStew:                       return ItemRabbitStewHandler;
		case Item::Rail:                             return ItemRailHandler;
		case Item::RavagerSpawnEgg:                  return ItemRavagerSpawnEggHandler;
		case Item::RedBanner:                        return ItemRedBannerHandler;
		case Item::RedBed:                           return ItemRedBedHandler;
		case Item::RedCarpet:                        return ItemRedCarpetHandler;
		case Item::RedConcrete:                      return ItemRedConcreteHandler;
		case Item::RedConcretePowder:                return ItemRedConcretePowderHandler;
		case Item::RedDye:                           return ItemRedDyeHandler;
		case Item::RedGlazedTerracotta:              return ItemRedGlazedTerracottaHandler;
		case Item::RedMushroom:                      return ItemRedMushroomHandler;
		case Item::RedMushroomBlock:                 return ItemRedMushroomBlockHandler;
		case Item::RedNetherBrickSlab:               return ItemRedNetherBrickSlabHandler;
		case Item::RedNetherBrickStairs:             return ItemRedNetherBrickStairsHandler;
		case Item::RedNetherBrickWall:               return ItemRedNetherBrickWallHandler;
		case Item::RedNetherBricks:                  return ItemRedNetherBricksHandler;
		case Item::RedSand:                          return ItemRedSandHandler;
		case Item::RedSandstone:                     return ItemRedSandstoneHandler;
		case Item::RedSandstoneSlab:                 return ItemRedSandstoneSlabHandler;
		case Item::RedSandstoneStairs:               return ItemRedSandstoneStairsHandler;
		case Item::RedSandstoneWall:                 return ItemRedSandstoneWallHandler;
		case Item::RedShulkerBox:                    return ItemRedShulkerBoxHandler;
		case Item::RedStainedGlass:                  return ItemRedStainedGlassHandler;
		case Item::RedStainedGlassPane:              return ItemRedStainedGlassPaneHandler;
		case Item::RedTerracotta:                    return ItemRedTerracottaHandler;
		case Item::RedTulip:                         return ItemRedTulipHandler;
		case Item::RedWool:                          return ItemRedWoolHandler;
		case Item::Redstone:                         return ItemRedstoneHandler;
		case Item::RedstoneBlock:                    return ItemRedstoneBlockHandler;
		case Item::RedstoneLamp:                     return ItemRedstoneLampHandler;
		case Item::RedstoneOre:                      return ItemRedstoneOreHandler;
		case Item::RedstoneTorch:                    return ItemRedstoneTorchHandler;
		case Item::Repeater:                         return ItemRepeaterHandler;
		case Item::RepeatingCommandBlock:            return ItemRepeatingCommandBlockHandler;
		case Item::RespawnAnchor:                    return ItemRespawnAnchorHandler;
		case Item::RoseBush:                         return ItemRoseBushHandler;
		case Item::RottenFlesh:                      return ItemRottenFleshHandler;
		case Item::Saddle:                           return ItemSaddleHandler;
		case Item::Salmon:                           return ItemSalmonHandler;
		case Item::SalmonBucket:                     return ItemSalmonBucketHandler;
		case Item::SalmonSpawnEgg:                   return ItemSalmonSpawnEggHandler;
		case Item::Sand:                             return ItemSandHandler;
		case Item::Sandstone:                        return ItemSandstoneHandler;
		case Item::SandstoneSlab:                    return ItemSandstoneSlabHandler;
		case Item::SandstoneStairs:                  return ItemSandstoneStairsHandler;
		case Item::SandstoneWall:                    return ItemSandstoneWallHandler;
		case Item::Scaffolding:                      return ItemScaffoldingHandler;
		case Item::Scute:                            return ItemScuteHandler;
		case Item::SeaLantern:                       return ItemSeaLanternHandler;
		case Item::SeaPickle:                        return ItemSeaPickleHandler;
		case Item::Seagrass:                         return ItemSeagrassHandler;
		case Item::Shears:                           return ItemShearsHandler;
		case Item::SheepSpawnEgg:                    return ItemSheepSpawnEggHandler;
		case Item::Shield:                           return ItemShieldHandler;
		case Item::Shroomlight:                      return ItemShroomlightHandler;
		case Item::ShulkerBox:                       return ItemShulkerBoxHandler;
		case Item::ShulkerShell:                     return ItemShulkerShellHandler;
		case Item::ShulkerSpawnEgg:                  return ItemShulkerSpawnEggHandler;
		case Item::SilverfishSpawnEgg:               return ItemSilverfishSpawnEggHandler;
		case Item::SkeletonHorseSpawnEgg:            return ItemSkeletonHorseSpawnEggHandler;
		case Item::SkeletonSkull:                    return ItemSkeletonSkullHandler;
		case Item::SkeletonSpawnEgg:                 return ItemSkeletonSpawnEggHandler;
		case Item::SkullBannerPattern:               return ItemSkullBannerPatternHandler;
		case Item::SlimeBall:                        return ItemSlimeBallHandler;
		case Item::SlimeBlock:                       return ItemSlimeBlockHandler;
		case Item::SlimeSpawnEgg:                    return ItemSlimeSpawnEggHandler;
		case Item::SmithingTable:                    return ItemSmithingTableHandler;
		case Item::Smoker:                           return ItemSmokerHandler;
		case Item::SmoothQuartz:                     return ItemSmoothQuartzHandler;
		case Item::SmoothQuartzSlab:                 return ItemSmoothQuartzSlabHandler;
		case Item::SmoothQuartzStairs:               return ItemSmoothQuartzStairsHandler;
		case Item::SmoothRedSandstone:               return ItemSmoothRedSandstoneHandler;
		case Item::SmoothRedSandstoneSlab:           return ItemSmoothRedSandstoneSlabHandler;
		case Item::SmoothRedSandstoneStairs:         return ItemSmoothRedSandstoneStairsHandler;
		case Item::SmoothSandstone:                  return ItemSmoothSandstoneHandler;
		case Item::SmoothSandstoneSlab:              return ItemSmoothSandstoneSlabHandler;
		case Item::SmoothSandstoneStairs:            return ItemSmoothSandstoneStairsHandler;
		case Item::SmoothStone:                      return ItemSmoothStoneHandler;
		case Item::SmoothStoneSlab:                  return ItemSmoothStoneSlabHandler;
		case Item::Snow:                             return ItemSnowHandler;
		case Item::SnowBlock:                        return ItemSnowBlockHandler;
		case Item::Snowball:                         return ItemSnowballHandler;
		case Item::SoulCampfire:                     return ItemSoulCampfireHandler;
		case Item::SoulLantern:                      return ItemSoulLanternHandler;
		case Item::SoulSand:                         return ItemSoulSandHandler;
		case Item::SoulSoil:                         return ItemSoulSoilHandler;
		case Item::SoulTorch:                        return ItemSoulTorchHandler;
		case Item::Spawner:                          return ItemSpawnerHandler;
		case Item::SpectralArrow:                    return ItemSpectralArrowHandler;
		case Item::SpiderEye:                        return ItemSpiderEyeHandler;
		case Item::SpiderSpawnEgg:                   return ItemSpiderSpawnEggHandler;
		case Item::SplashPotion:                     return ItemSplashPotionHandler;
		case Item::Sponge:                           return ItemSpongeHandler;
		case Item::SpruceBoat:                       return ItemSpruceBoatHandler;
		case Item::SpruceButton:                     return ItemSpruceButtonHandler;
		case Item::SpruceDoor:                       return ItemSpruceDoorHandler;
		case Item::SpruceFence:                      return ItemSpruceFenceHandler;
		case Item::SpruceFenceGate:                  return ItemSpruceFenceGateHandler;
		case Item::SpruceLeaves:                     return ItemSpruceLeavesHandler;
		case Item::SpruceLog:                        return ItemSpruceLogHandler;
		case Item::SprucePlanks:                     return ItemSprucePlanksHandler;
		case Item::SprucePressurePlate:              return ItemSprucePressurePlateHandler;
		case Item::SpruceSapling:                    return ItemSpruceSaplingHandler;
		case Item::SpruceSign:                       return ItemSpruceSignHandler;
		case Item::SpruceSlab:                       return ItemSpruceSlabHandler;
		case Item::SpruceStairs:                     return ItemSpruceStairsHandler;
		case Item::SpruceTrapdoor:                   return ItemSpruceTrapdoorHandler;
		case Item::SpruceWood:                       return ItemSpruceWoodHandler;
		case Item::SquidSpawnEgg:                    return ItemSquidSpawnEggHandler;
		case Item::Stick:                            return ItemStickHandler;
		case Item::StickyPiston:                     return ItemStickyPistonHandler;
		case Item::Stone:                            return ItemStoneHandler;
		case Item::StoneAxe:                         return ItemStoneAxeHandler;
		case Item::StoneBrickSlab:                   return ItemStoneBrickSlabHandler;
		case Item::StoneBrickStairs:                 return ItemStoneBrickStairsHandler;
		case Item::StoneBrickWall:                   return ItemStoneBrickWallHandler;
		case Item::StoneBricks:                      return ItemStoneBricksHandler;
		case Item::StoneButton:                      return ItemStoneButtonHandler;
		case Item::StoneHoe:                         return ItemStoneHoeHandler;
		case Item::StonePickaxe:                     return ItemStonePickaxeHandler;
		case Item::StonePressurePlate:               return ItemStonePressurePlateHandler;
		case Item::StoneShovel:                      return ItemStoneShovelHandler;
		case Item::StoneSlab:                        return ItemStoneSlabHandler;
		case Item::StoneStairs:                      return ItemStoneStairsHandler;
		case Item::StoneSword:                       return ItemStoneSwordHandler;
		case Item::Stonecutter:                      return ItemStonecutterHandler;
		case Item::StraySpawnEgg:                    return ItemStraySpawnEggHandler;
		case Item::StriderSpawnEgg:                  return ItemStriderSpawnEggHandler;
		case Item::String:                           return ItemStringHandler;
		case Item::StrippedAcaciaLog:                return ItemStrippedAcaciaLogHandler;
		case Item::StrippedAcaciaWood:               return ItemStrippedAcaciaWoodHandler;
		case Item::StrippedBirchLog:                 return ItemStrippedBirchLogHandler;
		case Item::StrippedBirchWood:                return ItemStrippedBirchWoodHandler;
		case Item::StrippedCrimsonHyphae:            return ItemStrippedCrimsonHyphaeHandler;
		case Item::StrippedCrimsonStem:              return ItemStrippedCrimsonStemHandler;
		case Item::StrippedDarkOakLog:               return ItemStrippedDarkOakLogHandler;
		case Item::StrippedDarkOakWood:              return ItemStrippedDarkOakWoodHandler;
		case Item::StrippedJungleLog:                return ItemStrippedJungleLogHandler;
		case Item::StrippedJungleWood:               return ItemStrippedJungleWoodHandler;
		case Item::StrippedOakLog:                   return ItemStrippedOakLogHandler;
		case Item::StrippedOakWood:                  return ItemStrippedOakWoodHandler;
		case Item::StrippedSpruceLog:                return ItemStrippedSpruceLogHandler;
		case Item::StrippedSpruceWood:               return ItemStrippedSpruceWoodHandler;
		case Item::StrippedWarpedHyphae:             return ItemStrippedWarpedHyphaeHandler;
		case Item::StrippedWarpedStem:               return ItemStrippedWarpedStemHandler;
		case Item::StructureBlock:                   return ItemStructureBlockHandler;
		case Item::StructureVoid:                    return ItemStructureVoidHandler;
		case Item::Sugar:                            return ItemSugarHandler;
		case Item::SugarCane:                        return ItemSugarCaneHandler;
		case Item::Sunflower:                        return ItemSunflowerHandler;
		case Item::SuspiciousStew:                   return ItemSuspiciousStewHandler;
		case Item::SweetBerries:                     return ItemSweetBerriesHandler;
		case Item::TallGrass:                        return ItemTallGrassHandler;
		case Item::Target:                           return ItemTargetHandler;
		case Item::Terracotta:                       return ItemTerracottaHandler;
		case Item::TippedArrow:                      return ItemTippedArrowHandler;
		case Item::TNT:                              return ItemTNTHandler;
		case Item::TNTMinecart:                      return ItemTNTMinecartHandler;
		case Item::Torch:                            return ItemTorchHandler;
		case Item::TotemOfUndying:                   return ItemTotemOfUndyingHandler;
		case Item::TraderLlamaSpawnEgg:              return ItemTraderLlamaSpawnEggHandler;
		case Item::TrappedChest:                     return ItemTrappedChestHandler;
		case Item::Trident:                          return ItemTridentHandler;
		case Item::TripwireHook:                     return ItemTripwireHookHandler;
		case Item::TropicalFish:                     return ItemTropicalFishHandler;
		case Item::TropicalFishBucket:               return ItemTropicalFishBucketHandler;
		case Item::TropicalFishSpawnEgg:             return ItemTropicalFishSpawnEggHandler;
		case Item::TubeCoral:                        return ItemTubeCoralHandler;
		case Item::TubeCoralBlock:                   return ItemTubeCoralBlockHandler;
		case Item::TubeCoralFan:                     return ItemTubeCoralFanHandler;
		case Item::TurtleEgg:                        return ItemTurtleEggHandler;
		case Item::TurtleHelmet:                     return ItemTurtleHelmetHandler;
		case Item::TurtleSpawnEgg:                   return ItemTurtleSpawnEggHandler;
		case Item::TwistingVines:                    return ItemTwistingVinesHandler;
		case Item::VexSpawnEgg:                      return ItemVexSpawnEggHandler;
		case Item::VillagerSpawnEgg:                 return ItemVillagerSpawnEggHandler;
		case Item::VindicatorSpawnEgg:               return ItemVindicatorSpawnEggHandler;
		case Item::Vine:                             return ItemVineHandler;
		case Item::WanderingTraderSpawnEgg:          return ItemWanderingTraderSpawnEggHandler;
		case Item::WarpedButton:                     return ItemWarpedButtonHandler;
		case Item::WarpedDoor:                       return ItemWarpedDoorHandler;
		case Item::WarpedFence:                      return ItemWarpedFenceHandler;
		case Item::WarpedFenceGate:                  return ItemWarpedFenceGateHandler;
		case Item::WarpedFungus:                     return ItemWarpedFungusHandler;
		case Item::WarpedFungusOnA_stick:            return ItemWarpedFungusOnA_stickHandler;
		case Item::WarpedHyphae:                     return ItemWarpedHyphaeHandler;
		case Item::WarpedNylium:                     return ItemWarpedNyliumHandler;
		case Item::WarpedPlanks:                     return ItemWarpedPlanksHandler;
		case Item::WarpedPressurePlate:              return ItemWarpedPressurePlateHandler;
		case Item::WarpedRoots:                      return ItemWarpedRootsHandler;
		case Item::WarpedSign:                       return ItemWarpedSignHandler;
		case Item::WarpedSlab:                       return ItemWarpedSlabHandler;
		case Item::WarpedStairs:                     return ItemWarpedStairsHandler;
		case Item::WarpedStem:                       return ItemWarpedStemHandler;
		case Item::WarpedTrapdoor:                   return ItemWarpedTrapdoorHandler;
		case Item::WarpedWartBlock:                  return ItemWarpedWartBlockHandler;
		case Item::WaterBucket:                      return ItemWaterBucketHandler;
		case Item::WeepingVines:                     return ItemWeepingVinesHandler;
		case Item::WetSponge:                        return ItemWetSpongeHandler;
		case Item::Wheat:                            return ItemWheatHandler;
		case Item::WheatSeeds:                       return ItemWheatSeedsHandler;
		case Item::WhiteBanner:                      return ItemWhiteBannerHandler;
		case Item::WhiteBed:                         return ItemWhiteBedHandler;
		case Item::WhiteCarpet:                      return ItemWhiteCarpetHandler;
		case Item::WhiteConcrete:                    return ItemWhiteConcreteHandler;
		case Item::WhiteConcretePowder:              return ItemWhiteConcretePowderHandler;
		case Item::WhiteDye:                         return ItemWhiteDyeHandler;
		case Item::WhiteGlazedTerracotta:            return ItemWhiteGlazedTerracottaHandler;
		case Item::WhiteShulkerBox:                  return ItemWhiteShulkerBoxHandler;
		case Item::WhiteStainedGlass:                return ItemWhiteStainedGlassHandler;
		case Item::WhiteStainedGlassPane:            return ItemWhiteStainedGlassPaneHandler;
		case Item::WhiteTerracotta:                  return ItemWhiteTerracottaHandler;
		case Item::WhiteTulip:                       return ItemWhiteTulipHandler;
		case Item::WhiteWool:                        return ItemWhiteWoolHandler;
		case Item::WitchSpawnEgg:                    return ItemWitchSpawnEggHandler;
		case Item::WitherRose:                       return ItemWitherRoseHandler;
		case Item::WitherSkeletonSkull:              return ItemWitherSkeletonSkullHandler;
		case Item::WitherSkeletonSpawnEgg:           return ItemWitherSkeletonSpawnEggHandler;
		case Item::WolfSpawnEgg:                     return ItemWolfSpawnEggHandler;
		case Item::WoodenAxe:                        return ItemWoodenAxeHandler;
		case Item::WoodenHoe:                        return ItemWoodenHoeHandler;
		case Item::WoodenPickaxe:                    return ItemWoodenPickaxeHandler;
		case Item::WoodenShovel:                     return ItemWoodenShovelHandler;
		case Item::WoodenSword:                      return ItemWoodenSwordHandler;
		case Item::WritableBook:                     return ItemWritableBookHandler;
		case Item::WrittenBook:                      return ItemWrittenBookHandler;
		case Item::YellowBanner:                     return ItemYellowBannerHandler;
		case Item::YellowBed:                        return ItemYellowBedHandler;
		case Item::YellowCarpet:                     return ItemYellowCarpetHandler;
		case Item::YellowConcrete:                   return ItemYellowConcreteHandler;
		case Item::YellowConcretePowder:             return ItemYellowConcretePowderHandler;
		case Item::YellowDye:                        return ItemYellowDyeHandler;
		case Item::YellowGlazedTerracotta:           return ItemYellowGlazedTerracottaHandler;
		case Item::YellowShulkerBox:                 return ItemYellowShulkerBoxHandler;
		case Item::YellowStainedGlass:               return ItemYellowStainedGlassHandler;
		case Item::YellowStainedGlassPane:           return ItemYellowStainedGlassPaneHandler;
		case Item::YellowTerracotta:                 return ItemYellowTerracottaHandler;
		case Item::YellowWool:                       return ItemYellowWoolHandler;
		case Item::ZoglinSpawnEgg:                   return ItemZoglinSpawnEggHandler;
		case Item::ZombieHead:                       return ItemZombieHeadHandler;
		case Item::ZombieHorseSpawnEgg:              return ItemZombieHorseSpawnEggHandler;
		case Item::ZombieSpawnEgg:                   return ItemZombieSpawnEggHandler;
		case Item::ZombieVillagerSpawnEgg:           return ItemZombieVillagerSpawnEggHandler;
		case Item::ZombiePigmanSpawnEgg:             return ItemZombiePigmanSpawnEggHandler;
	}
	ASSERT("Unknown item type!");
	return ItemAirHandler;
}





void cItemHandler::OnPlayerPlace(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_ClickedPosition, const BlockState a_ClickedBlock, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const
{
	const auto & World = *a_Player.GetWorld();

	// Check if the block ignores build collision (water, grass etc.):
	if (cBlockHandler::For(a_ClickedBlock.Type()).DoesIgnoreBuildCollision(World, a_HeldItem, a_ClickedPosition, a_ClickedBlock, a_ClickedBlockFace, true))
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
		BlockState PlaceBlock;
		const auto PlacePosition = AddFaceDirection(a_ClickedPosition, a_ClickedBlockFace);

		if (!cChunkDef::IsValidHeight(PlacePosition) || !World.GetBlock(PlacePosition, PlaceBlock))
		{
			// The block is being placed outside the world, ignore this packet altogether (GH #128):
			return;
		}

		auto PlacedBlock = World.GetBlock(PlacePosition);

		// Clicked on side of block, make sure that placement won't be cancelled if there is a slab able to be double slabbed.
		// No need to do combinability (dblslab) checks, client will do that here.
		if (!cBlockHandler::For(PlacedBlock.Type()).DoesIgnoreBuildCollision(World, a_HeldItem, PlacePosition, PlacedBlock, a_ClickedBlockFace, false))
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
	cWorld * aWorld, cPlayer * a_Player, cBlockPluginInterface & aPluginInterface, const cItem & aItem,
	const Vector3i a_ClickedBlockPosition, eBlockFace a_ClickedBlockFace
) const
{
	UNUSED(aWorld);
	UNUSED(a_Player);
	UNUSED(aPluginInterface);
	UNUSED(aItem);
	UNUSED(a_ClickedBlockPosition);
	UNUSED(a_ClickedBlockFace);

	return false;
}





bool cItemHandler::OnDiggingBlock(
	cWorld * aWorld,
	cPlayer * a_Player,
	const cItem & a_HeldItem,
	const Vector3i a_ClickedBlockPosition,
	eBlockFace a_ClickedBlockFace
) const
{
	UNUSED(aWorld);
	UNUSED(a_Player);
	UNUSED(a_HeldItem);
	UNUSED(a_ClickedBlockPosition);
	UNUSED(a_ClickedBlockFace);

	return false;
}





void cItemHandler::OnEntityAttack(cPlayer * aAttacker, cEntity * aAttackedEntity) const
{
	UNUSED(aAttackedEntity);
	aAttacker->UseEquippedItem(dlaAttackEntity);
}





void cItemHandler::OnFoodEaten(cWorld * aWorld, cPlayer * a_Player, cItem * aItem) const
{
	UNUSED(aWorld);
	UNUSED(a_Player);
	UNUSED(aItem);
}





short cItemHandler::GetDurabilityLossByAction(eDurabilityLostAction aAction) const
{
	UNUSED(aAction);

	return 0;
}





char cItemHandler::GetMaxStackSize(void) const
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





bool cItemHandler::IsFood(void) const
{
	return false;
}





bool cItemHandler::IsDrinkable(short aItemDamage) const
{
	UNUSED(aItemDamage);

	return false;
}





bool cItemHandler::IsPlaceable(void) const
{
	return false;
}





bool cItemHandler::CanRepairWithRawMaterial(const cItem & aItem) const
{
	UNUSED(aItem);
	return false;
}





bool cItemHandler::CanHarvestBlock(BlockState aBlock) const
{
	switch (aBlock.Type())
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





bool cItemHandler::EatItem(cPlayer * a_Player, cItem * aItem) const
{
	auto FoodInfo = GetFoodInfo(aItem);
	return a_Player->Feed(FoodInfo.FoodLevel, FoodInfo.Saturation);
}





cItemHandler::FoodInfo cItemHandler::GetFoodInfo(const cItem * aItem) const
{
	UNUSED(aItem);
	return FoodInfo(0, 0);
}





float cItemHandler::GetBlockBreakingStrength(BlockState aBlock) const
{
	return 1.0f;
}





bool cItemHandler::CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i aPlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const
{
	return false;
}
