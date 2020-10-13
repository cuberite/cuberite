#include "Globals.h"

#include "BlockHandler.h"
#include "../Item.h"
#include "../World.h"
#include "../Chunk.h"
#include "BlockPluginInterface.h"
#include "BlockAir.h"
#include "BlockAnvil.h"
#include "BlockBed.h"
#include "BlockBigFlower.h"
#include "BlockBookShelf.h"
#include "BlockBrewingStand.h"
#include "BlockButton.h"
#include "BlockCactus.h"
#include "BlockCake.h"
#include "BlockCarpet.h"
#include "BlockCauldron.h"
#include "BlockChest.h"
#include "BlockCloth.h"
#include "BlockCobWeb.h"
#include "BlockCocoaPod.h"
#include "BlockCommandBlock.h"
#include "BlockComparator.h"
#include "BlockConcretePowder.h"
#include "BlockCrops.h"
#include "BlockDeadBush.h"
#include "BlockDefaultBlock.h"
#include "BlockDirt.h"
#include "BlockDoor.h"
#include "BlockDropSpenser.h"
#include "BlockEnchantingTable.h"
#include "BlockEndPortalFrame.h"
#include "BlockEnderchest.h"
#include "BlockEntity.h"
#include "BlockFarmland.h"
#include "BlockFence.h"
#include "BlockFenceGate.h"
#include "BlockFire.h"
#include "BlockFlower.h"
#include "BlockFlowerPot.h"
#include "BlockFluid.h"
#include "BlockFurnace.h"
#include "BlockGlass.h"
#include "BlockGlazedTerracotta.h"
#include "BlockGlowstone.h"
#include "BlockGrass.h"
#include "BlockGravel.h"
#include "BlockHopper.h"
#include "BlockIce.h"
#include "BlockJukebox.h"
#include "BlockLadder.h"
#include "BlockLeaves.h"
#include "BlockLever.h"
#include "BlockLilypad.h"
#include "BlockMelon.h"
#include "BlockMobHead.h"
#include "BlockMobSpawner.h"
#include "BlockInfested.h"
#include "BlockMushroom.h"
#include "BlockMycelium.h"
#include "BlockNetherWart.h"
#include "BlockNetherrack.h"
#include "BlockObserver.h"
#include "BlockOre.h"
#include "BlockPackedIce.h"
#include "BlockPiston.h"
#include "BlockPlanks.h"
#include "BlockPortal.h"
#include "BlockPressurePlate.h"
#include "BlockPumpkin.h"
#include "BlockQuartz.h"
#include "BlockRail.h"
#include "BlockRedstone.h"
#include "BlockRedstoneLamp.h"
#include "BlockRedstoneOre.h"
#include "BlockRedstoneRepeater.h"
#include "BlockSand.h"
#include "BlockSapling.h"
#include "BlockSeaLantern.h"
#include "BlockSideways.h"
#include "BlockSignPost.h"
#include "BlockSlab.h"
#include "BlockSlime.h"
#include "BlockSnow.h"
#include "BlockSponge.h"
#include "BlockStairs.h"
#include "BlockStems.h"
#include "BlockStone.h"
#include "BlockSugarcane.h"
#include "BlockTNT.h"
#include "BlockTallGrass.h"
#include "BlockTorch.h"
#include "BlockTrapdoor.h"
#include "BlockTripwire.h"
#include "BlockTripwireHook.h"
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
			if (Handler == nullptr)
			{
				printf("nullptr handler for block type %d!\n", Type);
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





/** Static list of all block handlers.
Ensures that a handler can never be nullptr (FS #243) on invalid during restarts or other transition periods. */
namespace
{
	constexpr cBlockDoorHandler               BlockAcaciaDoorHandler            (E_BLOCK_ACACIA_DOOR);
	constexpr cBlockFenceGateHandler          BlockAcaciaFenceGateHandler       (E_BLOCK_ACACIA_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockAcaciaFenceHandler           (E_BLOCK_ACACIA_FENCE);
	constexpr cBlockStairsHandler             BlockAcaciaWoodStairsHandler      (E_BLOCK_ACACIA_WOOD_STAIRS);
	constexpr cBlockRailHandler               BlockActivatorRailHandler         (E_BLOCK_ACTIVATOR_RAIL);
	constexpr cBlockComparatorHandler         BlockActiveComparatorHandler      (E_BLOCK_ACTIVE_COMPARATOR);
	constexpr cBlockAirHandler                BlockAirHandler                   (E_BLOCK_AIR);
	constexpr cBlockAnvilHandler              BlockAnvilHandler                 (E_BLOCK_ANVIL);
	constexpr cDefaultBlockHandler            BlockBarrierHandler               (E_BLOCK_BARRIER);
	constexpr cDefaultBlockEntityHandler      BlockBeaconHandler                (E_BLOCK_BEACON);
	constexpr cBlockBedHandler                BlockBedHandler                   (E_BLOCK_BED);
	constexpr cDefaultBlockHandler            BlockBedrockHandler               (E_BLOCK_BEDROCK);
	constexpr cBlockCropsHandler<3>           BlockBeetrootsHandler             (E_BLOCK_BEETROOTS);  // 4 stages of growth
	constexpr cBlockBigFlowerHandler          BlockBigFlowerHandler             (E_BLOCK_BIG_FLOWER);
	constexpr cBlockDoorHandler               BlockBirchDoorHandler             (E_BLOCK_BIRCH_DOOR);
	constexpr cBlockFenceGateHandler          BlockBirchFenceGateHandler        (E_BLOCK_BIRCH_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockBirchFenceHandler            (E_BLOCK_BIRCH_FENCE);
	constexpr cBlockStairsHandler             BlockBirchWoodStairsHandler       (E_BLOCK_BIRCH_WOOD_STAIRS);
	constexpr cBlockGlazedTerracottaHandler   BlockBlackGlazedTerracottaHandler (E_BLOCK_BLACK_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockBlackShulkerBoxHandler       (E_BLOCK_BLACK_SHULKER_BOX);
	constexpr cDefaultBlockHandler            BlockBlockOfCoalHandler           (E_BLOCK_BLOCK_OF_COAL);
	constexpr cDefaultBlockHandler            BlockBlockOfRedstoneHandler       (E_BLOCK_BLOCK_OF_REDSTONE);
	constexpr cBlockGlazedTerracottaHandler   BlockBlueGlazedTerracottaHandler  (E_BLOCK_BLUE_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockBlueShulkerBoxHandler        (E_BLOCK_BLUE_SHULKER_BOX);
	constexpr cDefaultBlockHandler            BlockBoneBlockHandler             (E_BLOCK_BONE_BLOCK);
	constexpr cBlockBookShelfHandler          BlockBookcaseHandler              (E_BLOCK_BOOKCASE);
	constexpr cBlockBrewingStandHandler       BlockBrewingStandHandler          (E_BLOCK_BREWING_STAND);
	constexpr cDefaultBlockHandler            BlockBrickHandler                 (E_BLOCK_BRICK);
	constexpr cBlockStairsHandler             BlockBrickStairsHandler           (E_BLOCK_BRICK_STAIRS);
	constexpr cBlockGlazedTerracottaHandler   BlockBrownGlazedTerracottaHandler (E_BLOCK_BROWN_GLAZED_TERRACOTTA);
	constexpr cBlockMushroomHandler           BlockBrownMushroomHandler         (E_BLOCK_BROWN_MUSHROOM);
	constexpr cDefaultBlockHandler            BlockBrownShulkerBoxHandler       (E_BLOCK_BROWN_SHULKER_BOX);
	constexpr cBlockCactusHandler             BlockCactusHandler                (E_BLOCK_CACTUS);
	constexpr cBlockCakeHandler               BlockCakeHandler                  (E_BLOCK_CAKE);
	constexpr cBlockCarpetHandler             BlockCarpetHandler                (E_BLOCK_CARPET);
	constexpr cBlockCropsHandler<7>           BlockCarrotsHandler               (E_BLOCK_CARROTS);  // 8 stages of growth
	constexpr cBlockCauldronHandler           BlockCauldronHandler              (E_BLOCK_CAULDRON);
	constexpr cBlockCommandBlockHandler       BlockChainCommandBlockHandler     (E_BLOCK_CHAIN_COMMAND_BLOCK);
	constexpr cBlockChestHandler              BlockChestHandler                 (E_BLOCK_CHEST);
	constexpr cDefaultBlockHandler            BlockChorusFlowerHandler          (E_BLOCK_CHORUS_FLOWER);
	constexpr cDefaultBlockHandler            BlockChorusPlantHandler           (E_BLOCK_CHORUS_PLANT);
	constexpr cDefaultOreHandler              BlockClayHandler                  (E_BLOCK_CLAY);
	constexpr cDefaultOreHandler              BlockCoalOreHandler               (E_BLOCK_COAL_ORE);
	constexpr cBlockStoneHandler              BlockCobblestoneHandler           (E_BLOCK_COBBLESTONE);
	constexpr cBlockStairsHandler             BlockCobblestoneStairsHandler     (E_BLOCK_COBBLESTONE_STAIRS);
	constexpr cDefaultBlockHandler            BlockCobblestoneWallHandler       (E_BLOCK_COBBLESTONE_WALL);
	constexpr cBlockCobWebHandler             BlockCobwebHandler                (E_BLOCK_COBWEB);
	constexpr cBlockCocoaPodHandler           BlockCocoaPodHandler              (E_BLOCK_COCOA_POD);
	constexpr cBlockCommandBlockHandler       BlockCommandBlockHandler          (E_BLOCK_COMMAND_BLOCK);
	constexpr cDefaultBlockHandler            BlockConcreteHandler              (E_BLOCK_CONCRETE);
	constexpr cBlockConcretePowderHandler     BlockConcretePowderHandler        (E_BLOCK_CONCRETE_POWDER);
	constexpr cBlockCropsHandler<7>           BlockCropsHandler                 (E_BLOCK_CROPS);  // 8 stages of growth
	constexpr cBlockGlazedTerracottaHandler   BlockCyanGlazedTerracottaHandler  (E_BLOCK_CYAN_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockCyanShulkerBoxHandler        (E_BLOCK_CYAN_SHULKER_BOX);
	constexpr cBlockDoorHandler               BlockDarkOakDoorHandler           (E_BLOCK_DARK_OAK_DOOR);
	constexpr cBlockFenceGateHandler          BlockDarkOakFenceGateHandler      (E_BLOCK_DARK_OAK_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockDarkOakFenceHandler          (E_BLOCK_DARK_OAK_FENCE);
	constexpr cBlockStairsHandler             BlockDarkOakWoodStairsHandler     (E_BLOCK_DARK_OAK_WOOD_STAIRS);
	constexpr cDefaultBlockHandler            BlockDaylightSensorHandler        (E_BLOCK_DAYLIGHT_SENSOR);
	constexpr cBlockDeadBushHandler           BlockDeadBushHandler              (E_BLOCK_DEAD_BUSH);
	constexpr cBlockRailHandler               BlockDetectorRailHandler          (E_BLOCK_DETECTOR_RAIL);
	constexpr cDefaultBlockHandler            BlockDiamondBlockHandler          (E_BLOCK_DIAMOND_BLOCK);
	constexpr cDefaultOreHandler              BlockDiamondOreHandler            (E_BLOCK_DIAMOND_ORE);
	constexpr cBlockDirtHandler               BlockDirtHandler                  (E_BLOCK_DIRT);
	constexpr cBlockDropSpenserHandler        BlockDispenserHandler             (E_BLOCK_DISPENSER);
	constexpr cBlockDoubleSlabHandler         BlockDoubleRedSandstoneSlabHandler(E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB);
	constexpr cBlockDoubleSlabHandler         BlockDoubleStoneSlabHandler       (E_BLOCK_DOUBLE_STONE_SLAB);
	constexpr cBlockDoubleSlabHandler         BlockDoubleWoodenSlabHandler      (E_BLOCK_DOUBLE_WOODEN_SLAB);
	constexpr cDefaultBlockHandler            BlockDragonEggHandler             (E_BLOCK_DRAGON_EGG);
	constexpr cBlockDropSpenserHandler        BlockDropperHandler               (E_BLOCK_DROPPER);
	constexpr cDefaultBlockHandler            BlockEmeraldBlockHandler          (E_BLOCK_EMERALD_BLOCK);
	constexpr cDefaultOreHandler              BlockEmeraldOreHandler            (E_BLOCK_EMERALD_ORE);
	constexpr cBlockEnchantingTableHandler    BlockEnchantingTableHandler       (E_BLOCK_ENCHANTMENT_TABLE);
	constexpr cDefaultBlockHandler            BlockEndBricksHandler             (E_BLOCK_END_BRICKS);
	constexpr cDefaultBlockHandler            BlockEndGatewayHandler            (E_BLOCK_END_GATEWAY);
	constexpr cBlockEndPortalFrameHandler     BlockEndPortalFrameHandler        (E_BLOCK_END_PORTAL_FRAME);
	constexpr cDefaultBlockHandler            BlockEndPortalHandler             (E_BLOCK_END_PORTAL);
	constexpr cDefaultBlockHandler            BlockEndRodHandler                (E_BLOCK_END_ROD);
	constexpr cDefaultBlockHandler            BlockEndStoneHandler              (E_BLOCK_END_STONE);
	constexpr cBlockEnderchestHandler         BlockEnderChestHandler            (E_BLOCK_ENDER_CHEST);
	constexpr cBlockFarmlandHandler           BlockFarmlandHandler              (E_BLOCK_FARMLAND);
	constexpr cBlockFenceHandler              BlockFenceHandler                 (E_BLOCK_FENCE);
	constexpr cBlockFireHandler               BlockFireHandler                  (E_BLOCK_FIRE);
	constexpr cBlockFlowerPotHandler          BlockFlowerPotHandler             (E_BLOCK_FLOWER_POT);
	constexpr cBlockIceHandler                BlockFrostedIceHandler            (E_BLOCK_FROSTED_ICE);
	constexpr cBlockFurnaceHandler            BlockFurnaceHandler               (E_BLOCK_FURNACE);
	constexpr cBlockGlassHandler              BlockGlassHandler                 (E_BLOCK_GLASS);
	constexpr cBlockGlassHandler              BlockGlassPaneHandler             (E_BLOCK_GLASS_PANE);
	constexpr cBlockGlowstoneHandler          BlockGlowstoneHandler             (E_BLOCK_GLOWSTONE);
	constexpr cDefaultBlockHandler            BlockGoldBlockHandler             (E_BLOCK_GOLD_BLOCK);
	constexpr cDefaultOreHandler              BlockGoldOreHandler               (E_BLOCK_GOLD_ORE);
	constexpr cBlockGrassHandler              BlockGrassHandler                 (E_BLOCK_GRASS);
	constexpr cDefaultBlockHandler            BlockGrassPathHandler             (E_BLOCK_GRASS_PATH);
	constexpr cBlockGravelHandler             BlockGravelHandler                (E_BLOCK_GRAVEL);
	constexpr cBlockGlazedTerracottaHandler   BlockGrayGlazedTerracottaHandler  (E_BLOCK_GRAY_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockGrayShulkerBoxHandler        (E_BLOCK_GRAY_SHULKER_BOX);
	constexpr cBlockGlazedTerracottaHandler   BlockGreenGlazedTerracottaHandler (E_BLOCK_GREEN_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockGreenShulkerBoxHandler       (E_BLOCK_GREEN_SHULKER_BOX);
	constexpr cDefaultBlockHandler            BlockHardenedClayHandler          (E_BLOCK_HARDENED_CLAY);
	constexpr cBlockSidewaysHandler           BlockHayBaleHandler               (E_BLOCK_HAY_BALE);
	constexpr cBlockMobHeadHandler            BlockHeadHandler                  (E_BLOCK_HEAD);
	constexpr cBlockPressurePlateHandler      BlockHeavyWeightedPressurePHandler(E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE);
	constexpr cBlockHopperHandler             BlockHopperHandler                (E_BLOCK_HOPPER);
	constexpr cDefaultBlockHandler            BlockHugeBrownMushroomHandler     (E_BLOCK_HUGE_BROWN_MUSHROOM);
	constexpr cDefaultBlockHandler            BlockHugeRedMushroomHandler       (E_BLOCK_HUGE_RED_MUSHROOM);
	constexpr cBlockIceHandler                BlockIceHandler                   (E_BLOCK_ICE);
	constexpr cBlockComparatorHandler         BlockInactiveComparatorHandler    (E_BLOCK_INACTIVE_COMPARATOR);
	constexpr cBlockInfestedHandler           BlockInfestedBlockHandler         (E_BLOCK_SILVERFISH_EGG);
	constexpr cDefaultBlockHandler            BlockIronBarsHandler              (E_BLOCK_IRON_BARS);
	constexpr cDefaultBlockHandler            BlockIronBlockHandler             (E_BLOCK_IRON_BLOCK);
	constexpr cBlockDoorHandler               BlockIronDoorHandler              (E_BLOCK_IRON_DOOR);
	constexpr cDefaultOreHandler              BlockIronOreHandler               (E_BLOCK_IRON_ORE);
	constexpr cBlockTrapdoorHandler           BlockIronTrapdoorHandler          (E_BLOCK_IRON_TRAPDOOR);
	constexpr cBlockPumpkinHandler            BlockJackOLanternHandler          (E_BLOCK_JACK_O_LANTERN);
	constexpr cBlockJukeboxHandler            BlockJukeboxHandler               (E_BLOCK_JUKEBOX);
	constexpr cBlockDoorHandler               BlockJungleDoorHandler            (E_BLOCK_JUNGLE_DOOR);
	constexpr cBlockFenceGateHandler          BlockJungleFenceGateHandler       (E_BLOCK_JUNGLE_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockJungleFenceHandler           (E_BLOCK_JUNGLE_FENCE);
	constexpr cBlockStairsHandler             BlockJungleWoodStairsHandler      (E_BLOCK_JUNGLE_WOOD_STAIRS);
	constexpr cBlockLadderHandler             BlockLadderHandler                (E_BLOCK_LADDER);
	constexpr cDefaultBlockHandler            BlockLapisBlockHandler            (E_BLOCK_LAPIS_BLOCK);
	constexpr cDefaultOreHandler              BlockLapisOreHandler              (E_BLOCK_LAPIS_ORE);
	constexpr cBlockLavaHandler               BlockLavaHandler                  (E_BLOCK_LAVA);
	constexpr cBlockLeavesHandler             BlockLeavesHandler                (E_BLOCK_LEAVES);
	constexpr cBlockLeverHandler              BlockLeverHandler                 (E_BLOCK_LEVER);
	constexpr cBlockGlazedTerracottaHandler   BlockLightBlueGlazedTerracoHandler(E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockLightBlueShulkerBoxHandler   (E_BLOCK_LIGHT_BLUE_SHULKER_BOX);
	constexpr cBlockGlazedTerracottaHandler   BlockLightGrayGlazedTerracoHandler(E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockLightGrayShulkerBoxHandler   (E_BLOCK_LIGHT_GRAY_SHULKER_BOX);
	constexpr cBlockPressurePlateHandler      BlockLightWeightedPressurePHandler(E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE);
	constexpr cBlockLilypadHandler            BlockLilyPadHandler               (E_BLOCK_LILY_PAD);
	constexpr cBlockGlazedTerracottaHandler   BlockLimeGlazedTerracottaHandler  (E_BLOCK_LIME_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockLimeShulkerBoxHandler        (E_BLOCK_LIME_SHULKER_BOX);
	constexpr cBlockFurnaceHandler            BlockLitFurnaceHandler            (E_BLOCK_LIT_FURNACE);
	constexpr cBlockSidewaysHandler           BlockLogHandler                   (E_BLOCK_LOG);
	constexpr cBlockGlazedTerracottaHandler   BlockMagentaGlazedTerracottHandler(E_BLOCK_MAGENTA_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockMagentaShulkerBoxHandler     (E_BLOCK_MAGENTA_SHULKER_BOX);
	constexpr cDefaultBlockHandler            BlockMagmaHandler                 (E_BLOCK_MAGMA);
	constexpr cBlockMelonHandler              BlockMelonHandler                 (E_BLOCK_MELON);
	constexpr cBlockMelonStemHandler          BlockMelonStemHandler             (E_BLOCK_MELON_STEM);
	constexpr cBlockMobSpawnerHandler         BlockMobSpawnerHandler            (E_BLOCK_MOB_SPAWNER);
	constexpr cDefaultBlockHandler            BlockMossyCobblestoneHandler      (E_BLOCK_MOSSY_COBBLESTONE);
	constexpr cBlockMyceliumHandler           BlockMyceliumHandler              (E_BLOCK_MYCELIUM);
	constexpr cBlockFenceHandler              BlockNetherBrickFenceHandler      (E_BLOCK_NETHER_BRICK_FENCE);
	constexpr cDefaultBlockHandler            BlockNetherBrickHandler           (E_BLOCK_NETHER_BRICK);
	constexpr cBlockStairsHandler             BlockNetherBrickStairsHandler     (E_BLOCK_NETHER_BRICK_STAIRS);
	constexpr cBlockPortalHandler             BlockNetherPortalHandler          (E_BLOCK_NETHER_PORTAL);
	constexpr cDefaultOreHandler              BlockNetherQuartzOreHandler       (E_BLOCK_NETHER_QUARTZ_ORE);
	constexpr cDefaultBlockHandler            BlockNetherWartBlockHandler       (E_BLOCK_NETHER_WART_BLOCK);
	constexpr cBlockNetherWartHandler         BlockNetherWartHandler            (E_BLOCK_NETHER_WART);
	constexpr cBlockNetherrack                BlockNetherrackHandler            (E_BLOCK_NETHERRACK);
	constexpr cBlockLeavesHandler             BlockNewLeavesHandler             (E_BLOCK_NEW_LEAVES);
	constexpr cBlockSidewaysHandler           BlockNewLogHandler                (E_BLOCK_NEW_LOG);
	constexpr cDefaultBlockEntityHandler      BlockNoteBlockHandler             (E_BLOCK_NOTE_BLOCK);
	constexpr cBlockDoorHandler               BlockOakDoorHandler               (E_BLOCK_OAK_DOOR);
	constexpr cBlockFenceGateHandler          BlockOakFenceGateHandler          (E_BLOCK_OAK_FENCE_GATE);
	constexpr cBlockStairsHandler             BlockOakWoodStairsHandler         (E_BLOCK_OAK_WOOD_STAIRS);
	constexpr cBlockObserverHandler           BlockObserverHandler              (E_BLOCK_OBSERVER);
	constexpr cDefaultBlockHandler            BlockObsidianHandler              (E_BLOCK_OBSIDIAN);
	constexpr cBlockGlazedTerracottaHandler   BlockOrangeGlazedTerracottaHandler(E_BLOCK_ORANGE_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockOrangeShulkerBoxHandler      (E_BLOCK_ORANGE_SHULKER_BOX);
	constexpr cBlockPackedIceHandler          BlockPackedIceHandler             (E_BLOCK_PACKED_ICE);
	constexpr cBlockGlazedTerracottaHandler   BlockPinkGlazedTerracottaHandler  (E_BLOCK_PINK_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockPinkShulkerBoxHandler        (E_BLOCK_PINK_SHULKER_BOX);
	constexpr cBlockPistonHandler             BlockPistonHandler                (E_BLOCK_PISTON);
	constexpr cBlockPistonHeadHandler         BlockPistonHeadHandler;
	constexpr cDefaultBlockHandler            BlockPistonMovedBlockHandler      (E_BLOCK_PISTON_MOVED_BLOCK);
	constexpr cBlockPlanksHandler             BlockPlanksHandler                (E_BLOCK_PLANKS);
	constexpr cBlockCropsHandler<7>           BlockPotatoesHandler              (E_BLOCK_POTATOES);  // 8 stages of growth
	constexpr cBlockRailHandler               BlockPoweredRailHandler           (E_BLOCK_POWERED_RAIL);
	constexpr cDefaultBlockHandler            BlockPrismarineBlockHandler       (E_BLOCK_PRISMARINE_BLOCK);
	constexpr cBlockPumpkinHandler            BlockPumpkinHandler               (E_BLOCK_PUMPKIN);
	constexpr cBlockPumpkinStemHandler        BlockPumpkinStemHandler           (E_BLOCK_PUMPKIN_STEM);
	constexpr cBlockGlazedTerracottaHandler   BlockPurpleGlazedTerracottaHandler(E_BLOCK_PURPLE_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockPurpleShulkerBoxHandler      (E_BLOCK_PURPLE_SHULKER_BOX);
	constexpr cDefaultBlockHandler            BlockPurpurBlockHandler           (E_BLOCK_PURPUR_BLOCK);
	constexpr cBlockDoubleSlabHandler         BlockPurpurDoubleSlabHandler      (E_BLOCK_PURPUR_DOUBLE_SLAB);
	constexpr cDefaultBlockHandler            BlockPurpurPillarHandler          (E_BLOCK_PURPUR_PILLAR);
	constexpr cBlockSlabHandler               BlockPurpurSlabHandler            (E_BLOCK_PURPUR_SLAB);
	constexpr cBlockStairsHandler             BlockPurpurStairsHandler          (E_BLOCK_PURPUR_STAIRS);
	constexpr cBlockQuartzHandler             BlockQuartzBlockHandler           (E_BLOCK_QUARTZ_BLOCK);
	constexpr cBlockStairsHandler             BlockQuartzStairsHandler          (E_BLOCK_QUARTZ_STAIRS);
	constexpr cBlockRailHandler               BlockRailHandler                  (E_BLOCK_RAIL);
	constexpr cBlockGlazedTerracottaHandler   BlockRedGlazedTerracottaHandler   (E_BLOCK_RED_GLAZED_TERRACOTTA);
	constexpr cBlockMushroomHandler           BlockRedMushroomHandler           (E_BLOCK_RED_MUSHROOM);
	constexpr cDefaultBlockHandler            BlockRedNetherBrickHandler        (E_BLOCK_RED_NETHER_BRICK);
	constexpr cBlockFlowerHandler             BlockRedRoseHandler               (E_BLOCK_RED_ROSE);
	constexpr cDefaultBlockHandler            BlockRedSandstoneHandler          (E_BLOCK_RED_SANDSTONE);
	constexpr cBlockSlabHandler               BlockRedSandstoneSlabHandler      (E_BLOCK_RED_SANDSTONE_SLAB);
	constexpr cBlockStairsHandler             BlockRedSandstoneStairsHandler    (E_BLOCK_RED_SANDSTONE_STAIRS);
	constexpr cDefaultBlockHandler            BlockRedShulkerBoxHandler         (E_BLOCK_RED_SHULKER_BOX);
	constexpr cBlockRedstoneLampHandler       BlockRedstoneLampHandler          (E_BLOCK_REDSTONE_LAMP_ON);
	constexpr cBlockGlowingRedstoneOreHandler BlockRedstoneOreGlowingHandler    (E_BLOCK_REDSTONE_ORE_GLOWING);
	constexpr cBlockRedstoneOreHandler        BlockRedstoneOreHandler           (E_BLOCK_REDSTONE_ORE);
	constexpr cBlockRedstoneRepeaterHandler   BlockRedstoneRepeaterOffHandler   (E_BLOCK_REDSTONE_REPEATER_OFF);
	constexpr cBlockRedstoneRepeaterHandler   BlockRedstoneRepeaterOnHandler    (E_BLOCK_REDSTONE_REPEATER_ON);
	constexpr cBlockRedstoneTorchHandler      BlockRedstoneTorchOffHandler      (E_BLOCK_REDSTONE_TORCH_OFF);
	constexpr cBlockRedstoneTorchHandler      BlockRedstoneTorchOnHandler       (E_BLOCK_REDSTONE_TORCH_ON);
	constexpr cBlockRedstoneHandler           BlockRedstoneWireHandler          (E_BLOCK_REDSTONE_WIRE);
	constexpr cBlockCommandBlockHandler       BlockRepeatingCommandBlockHandler (E_BLOCK_REPEATING_COMMAND_BLOCK);
	constexpr cBlockSandHandler               BlockSandHandler                  (E_BLOCK_SAND);
	constexpr cDefaultBlockHandler            BlockSandstoneHandler             (E_BLOCK_SANDSTONE);
	constexpr cBlockStairsHandler             BlockSandstoneStairsHandler       (E_BLOCK_SANDSTONE_STAIRS);
	constexpr cBlockSaplingHandler            BlockSaplingHandler               (E_BLOCK_SAPLING);
	constexpr cBlockSeaLanternHandler         BlockSeaLanternHandler            (E_BLOCK_SEA_LANTERN);
	constexpr cBlockSignPostHandler           BlockSignPostHandler              (E_BLOCK_SIGN_POST);
	constexpr cBlockSlimeHandler              BlockSlimeBlockHandler            (E_BLOCK_SLIME_BLOCK);
	constexpr cDefaultBlockHandler            BlockSnowBlockHandler             (E_BLOCK_SNOW_BLOCK);
	constexpr cBlockSnowHandler               BlockSnowHandler                  (E_BLOCK_SNOW);
	constexpr cDefaultBlockHandler            BlockSoulSandHandler              (E_BLOCK_SOULSAND);
	constexpr cBlockSpongeHandler             BlockSpongeHandler                (E_BLOCK_SPONGE);
	constexpr cBlockDoorHandler               BlockSpruceDoorHandler            (E_BLOCK_SPRUCE_DOOR);
	constexpr cBlockFenceGateHandler          BlockSpruceFenceGateHandler       (E_BLOCK_SPRUCE_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockSpruceFenceHandler           (E_BLOCK_SPRUCE_FENCE);
	constexpr cBlockStairsHandler             BlockSpruceWoodStairsHandler      (E_BLOCK_SPRUCE_WOOD_STAIRS);
	constexpr cDefaultBlockHandler            BlockStainedClayHandler           (E_BLOCK_STAINED_CLAY);
	constexpr cBlockGlassHandler              BlockStainedGlassHandler          (E_BLOCK_STAINED_GLASS);
	constexpr cBlockGlassHandler              BlockStainedGlassPaneHandler      (E_BLOCK_STAINED_GLASS_PANE);
	constexpr cDefaultBlockHandler            BlockStandingBannerHandler        (E_BLOCK_STANDING_BANNER);  // TODO: drops correct?
	constexpr cBlockLavaHandler               BlockStationaryLavaHandler        (E_BLOCK_STATIONARY_LAVA);
	constexpr cBlockWaterHandler              BlockStationaryWaterHandler       (E_BLOCK_STATIONARY_WATER);
	constexpr cBlockPistonHandler             BlockStickyPistonHandler          (E_BLOCK_STICKY_PISTON);
	constexpr cBlockStairsHandler             BlockStoneBrickStairsHandler      (E_BLOCK_STONE_BRICK_STAIRS);
	constexpr cDefaultBlockHandler            BlockStoneBricksHandler           (E_BLOCK_STONE_BRICKS);
	constexpr cBlockButtonHandler             BlockStoneButtonHandler           (E_BLOCK_STONE_BUTTON);
	constexpr cBlockStoneHandler              BlockStoneHandler                 (E_BLOCK_STONE);
	constexpr cBlockPressurePlateHandler      BlockStonePressurePlateHandler    (E_BLOCK_STONE_PRESSURE_PLATE);
	constexpr cBlockSlabHandler               BlockStoneSlabHandler             (E_BLOCK_STONE_SLAB);
	constexpr cDefaultBlockHandler            BlockStructureBlockHandler        (E_BLOCK_STRUCTURE_BLOCK);
	constexpr cDefaultBlockHandler            BlockStructureVoidHandler         (E_BLOCK_STRUCTURE_VOID);
	constexpr cBlockSugarcaneHandler          BlockSugarcaneHandler             (E_BLOCK_SUGARCANE);
	constexpr cBlockTallGrassHandler          BlockTallGrassHandler             (E_BLOCK_TALL_GRASS);
	constexpr cBlockTNTHandler                BlockTntHandler                   (E_BLOCK_TNT);
	constexpr cBlockTorchHandler              BlockTorchHandler                 (E_BLOCK_TORCH);
	constexpr cBlockTrapdoorHandler           BlockTrapdoorHandler              (E_BLOCK_TRAPDOOR);
	constexpr cBlockChestHandler              BlockTrappedChestHandler          (E_BLOCK_TRAPPED_CHEST);
	constexpr cBlockTripwireHandler           BlockTripwireHandler              (E_BLOCK_TRIPWIRE);
	constexpr cBlockTripwireHookHandler       BlockTripwireHookHandler          (E_BLOCK_TRIPWIRE_HOOK);
	constexpr cBlockVineHandler               BlockVinesHandler                 (E_BLOCK_VINES);
	constexpr cDefaultBlockHandler            BlockWallBannerHandler            (E_BLOCK_WALL_BANNER);  // TODO: drops correct?
	constexpr cBlockWallSignHandler           BlockWallsignHandler              (E_BLOCK_WALLSIGN);
	constexpr cBlockWaterHandler              BlockWaterHandler                 (E_BLOCK_WATER);
	constexpr cBlockGlazedTerracottaHandler   BlockWhiteGlazedTerracottaHandler (E_BLOCK_WHITE_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockWhiteShulkerBoxHandler       (E_BLOCK_WHITE_SHULKER_BOX);
	constexpr cBlockButtonHandler             BlockWoodenButtonHandler          (E_BLOCK_WOODEN_BUTTON);
	constexpr cBlockPressurePlateHandler      BlockWoodenPressurePlateHandler   (E_BLOCK_WOODEN_PRESSURE_PLATE);
	constexpr cBlockSlabHandler               BlockWoodenSlabHandler            (E_BLOCK_WOODEN_SLAB);
	constexpr cBlockClothHandler              BlockWoolHandler                  (E_BLOCK_WOOL);
	constexpr cBlockWorkbenchHandler          BlockWorkbenchHandler             (E_BLOCK_WORKBENCH);
	constexpr cBlockFlowerHandler             BlockYellowFlowerHandler          (E_BLOCK_YELLOW_FLOWER);
	constexpr cBlockGlazedTerracottaHandler   BlockYellowGlazedTerracottaHandler(E_BLOCK_YELLOW_GLAZED_TERRACOTTA);
	constexpr cDefaultBlockHandler            BlockYellowShulkerBoxHandler      (E_BLOCK_YELLOW_SHULKER_BOX);
}





////////////////////////////////////////////////////////////////////////////////
// cBlockHandler:

bool cBlockHandler::GetPlacementBlockTypeMeta(
	cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
	const Vector3i a_ClickedBlockPos,
	eBlockFace a_ClickedBlockFace,
	const Vector3i a_CursorPos,
	BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
) const
{
	// By default, all blocks can be placed and the meta is copied over from the item's damage value:
	a_BlockType = m_BlockType;
	a_BlockMeta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage & 0x0f);
	return true;
}





void cBlockHandler::OnUpdate(
	cChunkInterface & a_ChunkInterface,
	cWorldInterface & a_WorldInterface,
	cBlockPluginInterface & a_PluginInterface,
	cChunk & a_Chunk,
	const Vector3i a_RelPos
) const
{
}





void cBlockHandler::OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const
{
	if (a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk) { return CanBeAt(a_ChunkInterface, a_Chunk.AbsoluteToRelative(a_BlockPos), a_Chunk); }))
	{
		return;
	}

	if (DoesDropOnUnsuitable())
	{
		a_ChunkInterface.DropBlockAsPickups(a_BlockPos);
	}
	else
	{
		a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_AIR, 0);
	}
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_NeighborPos, eBlockFace a_WhichNeighbor)
{
	if (!cChunkDef::IsValidHeight(a_NeighborPos.y))
	{
		return;
	}

	cBlockHandler::For(a_ChunkInterface.GetBlock(a_NeighborPos)).OnNeighborChanged(a_ChunkInterface, a_NeighborPos, a_WhichNeighbor);
}





cItems cBlockHandler::ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const
{
	UNUSED(a_Digger);
	UNUSED(a_Tool);

	// Add self:
	return cItem(m_BlockType, 1, a_BlockMeta);
}





bool cBlockHandler::CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const
{
	return true;
}





bool cBlockHandler::IsUseable() const
{
	return false;
}





bool cBlockHandler::IsClickedThrough(void) const
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) const
{
	return (m_BlockType == E_BLOCK_AIR);
}





bool cBlockHandler::DoesDropOnUnsuitable(void) const
{
	return true;
}





bool cBlockHandler::IsInsideBlock(const Vector3d a_RelPosition, const NIBBLETYPE a_BlockMeta) const
{
	// Default functionality: Test the height, since we assume full voxels with varying height
	return (a_RelPosition.y < cBlockInfo::GetBlockHeight(m_BlockType));
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BLOCKTYPE a_XM, BLOCKTYPE a_XP, BLOCKTYPE a_YM, BLOCKTYPE a_YP, BLOCKTYPE a_ZM, BLOCKTYPE a_ZP) const
{
	if (!cBlockInfo::IsSolid(m_BlockType))
	{
		// Non-solid things can be placed within entities
		return cBoundingBox(0, 0, 0, 0, 0, 0);
	}
	return cBoundingBox(0, 1, 0, cBlockInfo::GetBlockHeight(m_BlockType), 0, 1);
}





void cBlockHandler::Check(
	cChunkInterface & a_ChunkInterface, cBlockPluginInterface & a_PluginInterface,
	Vector3i a_RelPos,
	cChunk & a_Chunk
) const
{
	const auto Position = cChunkDef::RelativeToAbsolute(a_RelPos, a_Chunk.GetPos());
	NeighborChanged(a_ChunkInterface, Position.addedX(-1), BLOCK_FACE_XP);
	NeighborChanged(a_ChunkInterface, Position.addedX(1), BLOCK_FACE_XM);
	NeighborChanged(a_ChunkInterface, Position.addedY(-1), BLOCK_FACE_YP);
	NeighborChanged(a_ChunkInterface, Position.addedY(1), BLOCK_FACE_YM);
	NeighborChanged(a_ChunkInterface, Position.addedZ(-1), BLOCK_FACE_ZP);
	NeighborChanged(a_ChunkInterface, Position.addedZ(1), BLOCK_FACE_ZM);
}





ColourID cBlockHandler::GetMapBaseColourID(NIBBLETYPE a_Meta) const
{
	// Zero for transparent
	return 0;
}





bool cBlockHandler::ToolHasSilkTouch(const cItem * a_Tool)
{
	return ((a_Tool != nullptr) && (a_Tool->m_Enchantments.GetLevel(cEnchantments::enchSilkTouch) > 0));
}





unsigned char cBlockHandler::ToolFortuneLevel(const cItem * a_Tool)
{
	if ((a_Tool != nullptr) && ItemCategory::IsTool(a_Tool->m_ItemType))
	{
		// Return enchantment level, limited to avoid spawning excessive pickups (crashing the server) when modified items are used:
		return static_cast<unsigned char>(std::min(8U, a_Tool->m_Enchantments.GetLevel(cEnchantments::enchFortune)));
	}

	// Not a tool:
	return 0;
}





char cBlockHandler::FortuneDiscreteRandom(char a_MinDrop, char a_DefaultMax, unsigned char a_BonusMax, char a_DropCap)
{
	// First sample the discrete random distribution.
	char DropNum = GetRandomProvider().RandInt<char>(a_MinDrop, a_DefaultMax + a_BonusMax);

	// Then clamp to within range (clamp instead of min incase of overflow):
	return std::clamp<char>(DropNum, a_MinDrop, a_DropCap);
}





const cBlockHandler & cBlockHandler::For(BLOCKTYPE a_BlockType)
{
	// Switch on the block type, as an enumeration
	// Clang will fail if any type is unhandled:
	switch (static_cast<ENUM_BLOCK_TYPE>(a_BlockType))
	{
		// Block handlers, alphabetically sorted:
		case E_BLOCK_ACACIA_DOOR:                   return BlockAcaciaDoorHandler;
		case E_BLOCK_ACACIA_FENCE:                  return BlockAcaciaFenceHandler;
		case E_BLOCK_ACACIA_FENCE_GATE:             return BlockAcaciaFenceGateHandler;
		case E_BLOCK_ACACIA_WOOD_STAIRS:            return BlockAcaciaWoodStairsHandler;
		case E_BLOCK_ACTIVATOR_RAIL:                return BlockActivatorRailHandler;
		case E_BLOCK_ACTIVE_COMPARATOR:             return BlockActiveComparatorHandler;
		case E_BLOCK_AIR:                           return BlockAirHandler;
		case E_BLOCK_ANVIL:                         return BlockAnvilHandler;
		case E_BLOCK_BARRIER:                       return BlockBarrierHandler;
		case E_BLOCK_BEACON:                        return BlockBeaconHandler;
		case E_BLOCK_BED:                           return BlockBedHandler;
		case E_BLOCK_BEDROCK:                       return BlockBedrockHandler;
		case E_BLOCK_BEETROOTS:                     return BlockBeetrootsHandler;
		case E_BLOCK_BIG_FLOWER:                    return BlockBigFlowerHandler;
		case E_BLOCK_BIRCH_DOOR:                    return BlockBirchDoorHandler;
		case E_BLOCK_BIRCH_FENCE:                   return BlockBirchFenceHandler;
		case E_BLOCK_BIRCH_FENCE_GATE:              return BlockBirchFenceGateHandler;
		case E_BLOCK_BIRCH_WOOD_STAIRS:             return BlockBirchWoodStairsHandler;
		case E_BLOCK_BLACK_GLAZED_TERRACOTTA:       return BlockBlackGlazedTerracottaHandler;
		case E_BLOCK_BLACK_SHULKER_BOX:             return BlockBlackShulkerBoxHandler;
		case E_BLOCK_BLOCK_OF_COAL:                 return BlockBlockOfCoalHandler;
		case E_BLOCK_BLOCK_OF_REDSTONE:             return BlockBlockOfRedstoneHandler;
		case E_BLOCK_BLUE_GLAZED_TERRACOTTA:        return BlockBlueGlazedTerracottaHandler;
		case E_BLOCK_BLUE_SHULKER_BOX:              return BlockBlueShulkerBoxHandler;
		case E_BLOCK_BONE_BLOCK:                    return BlockBoneBlockHandler;
		case E_BLOCK_BOOKCASE:                      return BlockBookcaseHandler;
		case E_BLOCK_BREWING_STAND:                 return BlockBrewingStandHandler;
		case E_BLOCK_BRICK:                         return BlockBrickHandler;
		case E_BLOCK_BRICK_STAIRS:                  return BlockBrickStairsHandler;
		case E_BLOCK_BROWN_GLAZED_TERRACOTTA:       return BlockBrownGlazedTerracottaHandler;
		case E_BLOCK_BROWN_MUSHROOM:                return BlockBrownMushroomHandler;
		case E_BLOCK_BROWN_SHULKER_BOX:             return BlockBrownShulkerBoxHandler;
		case E_BLOCK_CACTUS:                        return BlockCactusHandler;
		case E_BLOCK_CAKE:                          return BlockCakeHandler;
		case E_BLOCK_CARPET:                        return BlockCarpetHandler;
		case E_BLOCK_CARROTS:                       return BlockCarrotsHandler;
		case E_BLOCK_CAULDRON:                      return BlockCauldronHandler;
		case E_BLOCK_CHAIN_COMMAND_BLOCK:           return BlockChainCommandBlockHandler;
		case E_BLOCK_CHEST:                         return BlockChestHandler;
		case E_BLOCK_CHORUS_FLOWER:                 return BlockChorusFlowerHandler;
		case E_BLOCK_CHORUS_PLANT:                  return BlockChorusPlantHandler;
		case E_BLOCK_CLAY:                          return BlockClayHandler;
		case E_BLOCK_COAL_ORE:                      return BlockCoalOreHandler;
		case E_BLOCK_COBBLESTONE:                   return BlockCobblestoneHandler;
		case E_BLOCK_COBBLESTONE_STAIRS:            return BlockCobblestoneStairsHandler;
		case E_BLOCK_COBBLESTONE_WALL:              return BlockCobblestoneWallHandler;
		case E_BLOCK_COBWEB:                        return BlockCobwebHandler;
		case E_BLOCK_COCOA_POD:                     return BlockCocoaPodHandler;
		case E_BLOCK_COMMAND_BLOCK:                 return BlockCommandBlockHandler;
		case E_BLOCK_CONCRETE:                      return BlockConcreteHandler;
		case E_BLOCK_CONCRETE_POWDER:               return BlockConcretePowderHandler;
		case E_BLOCK_CROPS:                         return BlockCropsHandler;
		case E_BLOCK_CYAN_GLAZED_TERRACOTTA:        return BlockCyanGlazedTerracottaHandler;
		case E_BLOCK_CYAN_SHULKER_BOX:              return BlockCyanShulkerBoxHandler;
		case E_BLOCK_DARK_OAK_DOOR:                 return BlockDarkOakDoorHandler;
		case E_BLOCK_DARK_OAK_FENCE:                return BlockDarkOakFenceHandler;
		case E_BLOCK_DARK_OAK_FENCE_GATE:           return BlockDarkOakFenceGateHandler;
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:          return BlockDarkOakWoodStairsHandler;
		case E_BLOCK_DAYLIGHT_SENSOR:               return BlockDaylightSensorHandler;
		case E_BLOCK_DEAD_BUSH:                     return BlockDeadBushHandler;
		case E_BLOCK_DETECTOR_RAIL:                 return BlockDetectorRailHandler;
		case E_BLOCK_DIAMOND_BLOCK:                 return BlockDiamondBlockHandler;
		case E_BLOCK_DIAMOND_ORE:                   return BlockDiamondOreHandler;
		case E_BLOCK_DIRT:                          return BlockDirtHandler;
		case E_BLOCK_DISPENSER:                     return BlockDispenserHandler;
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:     return BlockDoubleRedSandstoneSlabHandler;
		case E_BLOCK_DOUBLE_STONE_SLAB:             return BlockDoubleStoneSlabHandler;
		case E_BLOCK_DOUBLE_WOODEN_SLAB:            return BlockDoubleWoodenSlabHandler;
		case E_BLOCK_DRAGON_EGG:                    return BlockDragonEggHandler;
		case E_BLOCK_DROPPER:                       return BlockDropperHandler;
		case E_BLOCK_EMERALD_BLOCK:                 return BlockEmeraldBlockHandler;
		case E_BLOCK_EMERALD_ORE:                   return BlockEmeraldOreHandler;
		case E_BLOCK_ENCHANTMENT_TABLE:             return BlockEnchantingTableHandler;
		case E_BLOCK_ENDER_CHEST:                   return BlockEnderChestHandler;
		case E_BLOCK_END_BRICKS:                    return BlockEndBricksHandler;
		case E_BLOCK_END_GATEWAY:                   return BlockEndGatewayHandler;
		case E_BLOCK_END_PORTAL:                    return BlockEndPortalHandler;
		case E_BLOCK_END_PORTAL_FRAME:              return BlockEndPortalFrameHandler;
		case E_BLOCK_END_ROD:                       return BlockEndRodHandler;
		case E_BLOCK_END_STONE:                     return BlockEndStoneHandler;
		case E_BLOCK_FARMLAND:                      return BlockFarmlandHandler;
		case E_BLOCK_FENCE:                         return BlockFenceHandler;
		case E_BLOCK_FIRE:                          return BlockFireHandler;
		case E_BLOCK_FLOWER_POT:                    return BlockFlowerPotHandler;
		case E_BLOCK_FROSTED_ICE:                   return BlockFrostedIceHandler;
		case E_BLOCK_FURNACE:                       return BlockFurnaceHandler;
		case E_BLOCK_GLASS:                         return BlockGlassHandler;
		case E_BLOCK_GLASS_PANE:                    return BlockGlassPaneHandler;
		case E_BLOCK_GLOWSTONE:                     return BlockGlowstoneHandler;
		case E_BLOCK_GOLD_BLOCK:                    return BlockGoldBlockHandler;
		case E_BLOCK_GOLD_ORE:                      return BlockGoldOreHandler;
		case E_BLOCK_GRASS:                         return BlockGrassHandler;
		case E_BLOCK_GRASS_PATH:                    return BlockGrassPathHandler;
		case E_BLOCK_GRAVEL:                        return BlockGravelHandler;
		case E_BLOCK_GRAY_GLAZED_TERRACOTTA:        return BlockGrayGlazedTerracottaHandler;
		case E_BLOCK_GRAY_SHULKER_BOX:              return BlockGrayShulkerBoxHandler;
		case E_BLOCK_GREEN_GLAZED_TERRACOTTA:       return BlockGreenGlazedTerracottaHandler;
		case E_BLOCK_GREEN_SHULKER_BOX:             return BlockGreenShulkerBoxHandler;
		case E_BLOCK_HARDENED_CLAY:                 return BlockHardenedClayHandler;
		case E_BLOCK_HAY_BALE:                      return BlockHayBaleHandler;
		case E_BLOCK_HEAD:                          return BlockHeadHandler;
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return BlockHeavyWeightedPressurePHandler;
		case E_BLOCK_HOPPER:                        return BlockHopperHandler;
		case E_BLOCK_HUGE_BROWN_MUSHROOM:           return BlockHugeBrownMushroomHandler;
		case E_BLOCK_HUGE_RED_MUSHROOM:             return BlockHugeRedMushroomHandler;
		case E_BLOCK_ICE:                           return BlockIceHandler;
		case E_BLOCK_INACTIVE_COMPARATOR:           return BlockInactiveComparatorHandler;
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:      return BlockDaylightSensorHandler;
		case E_BLOCK_IRON_BARS:                     return BlockIronBarsHandler;
		case E_BLOCK_IRON_BLOCK:                    return BlockIronBlockHandler;
		case E_BLOCK_IRON_DOOR:                     return BlockIronDoorHandler;
		case E_BLOCK_IRON_ORE:                      return BlockIronOreHandler;
		case E_BLOCK_IRON_TRAPDOOR:                 return BlockIronTrapdoorHandler;
		case E_BLOCK_JACK_O_LANTERN:                return BlockJackOLanternHandler;
		case E_BLOCK_JUKEBOX:                       return BlockJukeboxHandler;
		case E_BLOCK_JUNGLE_DOOR:                   return BlockJungleDoorHandler;
		case E_BLOCK_JUNGLE_FENCE:                  return BlockJungleFenceHandler;
		case E_BLOCK_JUNGLE_FENCE_GATE:             return BlockJungleFenceGateHandler;
		case E_BLOCK_JUNGLE_WOOD_STAIRS:            return BlockJungleWoodStairsHandler;
		case E_BLOCK_LADDER:                        return BlockLadderHandler;
		case E_BLOCK_LAPIS_BLOCK:                   return BlockLapisBlockHandler;
		case E_BLOCK_LAPIS_ORE:                     return BlockLapisOreHandler;
		case E_BLOCK_LAVA:                          return BlockLavaHandler;
		case E_BLOCK_LEAVES:                        return BlockLeavesHandler;
		case E_BLOCK_LEVER:                         return BlockLeverHandler;
		case E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA:  return BlockLightBlueGlazedTerracoHandler;
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:        return BlockLightBlueShulkerBoxHandler;
		case E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA:  return BlockLightGrayGlazedTerracoHandler;
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:        return BlockLightGrayShulkerBoxHandler;
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return BlockLightWeightedPressurePHandler;
		case E_BLOCK_LILY_PAD:                      return BlockLilyPadHandler;
		case E_BLOCK_LIME_GLAZED_TERRACOTTA:        return BlockLimeGlazedTerracottaHandler;
		case E_BLOCK_LIME_SHULKER_BOX:              return BlockLimeShulkerBoxHandler;
		case E_BLOCK_LIT_FURNACE:                   return BlockLitFurnaceHandler;
		case E_BLOCK_LOG:                           return BlockLogHandler;
		case E_BLOCK_MAGENTA_GLAZED_TERRACOTTA:     return BlockMagentaGlazedTerracottHandler;
		case E_BLOCK_MAGENTA_SHULKER_BOX:           return BlockMagentaShulkerBoxHandler;
		case E_BLOCK_MAGMA:                         return BlockMagmaHandler;
		case E_BLOCK_MELON:                         return BlockMelonHandler;
		case E_BLOCK_MELON_STEM:                    return BlockMelonStemHandler;
		case E_BLOCK_MOB_SPAWNER:                   return BlockMobSpawnerHandler;
		case E_BLOCK_MOSSY_COBBLESTONE:             return BlockMossyCobblestoneHandler;
		case E_BLOCK_MYCELIUM:                      return BlockMyceliumHandler;
		case E_BLOCK_NETHERRACK:                    return BlockNetherrackHandler;
		case E_BLOCK_NETHER_BRICK:                  return BlockNetherBrickHandler;
		case E_BLOCK_NETHER_BRICK_FENCE:            return BlockNetherBrickFenceHandler;
		case E_BLOCK_NETHER_BRICK_STAIRS:           return BlockNetherBrickStairsHandler;
		case E_BLOCK_NETHER_PORTAL:                 return BlockNetherPortalHandler;
		case E_BLOCK_NETHER_QUARTZ_ORE:             return BlockNetherQuartzOreHandler;
		case E_BLOCK_NETHER_WART:                   return BlockNetherWartHandler;
		case E_BLOCK_NETHER_WART_BLOCK:             return BlockNetherWartBlockHandler;
		case E_BLOCK_NEW_LEAVES:                    return BlockNewLeavesHandler;
		case E_BLOCK_NEW_LOG:                       return BlockNewLogHandler;
		case E_BLOCK_NOTE_BLOCK:                    return BlockNoteBlockHandler;
		case E_BLOCK_OAK_DOOR:                      return BlockOakDoorHandler;
		case E_BLOCK_OAK_FENCE_GATE:                return BlockOakFenceGateHandler;
		case E_BLOCK_OAK_WOOD_STAIRS:               return BlockOakWoodStairsHandler;
		case E_BLOCK_OBSERVER:                      return BlockObserverHandler;
		case E_BLOCK_OBSIDIAN:                      return BlockObsidianHandler;
		case E_BLOCK_ORANGE_GLAZED_TERRACOTTA:      return BlockOrangeGlazedTerracottaHandler;
		case E_BLOCK_ORANGE_SHULKER_BOX:            return BlockOrangeShulkerBoxHandler;
		case E_BLOCK_PACKED_ICE:                    return BlockPackedIceHandler;
		case E_BLOCK_PINK_GLAZED_TERRACOTTA:        return BlockPinkGlazedTerracottaHandler;
		case E_BLOCK_PINK_SHULKER_BOX:              return BlockPinkShulkerBoxHandler;
		case E_BLOCK_PISTON:                        return BlockPistonHandler;
		case E_BLOCK_PISTON_EXTENSION:              return BlockPistonHeadHandler;
		case E_BLOCK_PISTON_MOVED_BLOCK:            return BlockPistonMovedBlockHandler;
		case E_BLOCK_PLANKS:                        return BlockPlanksHandler;
		case E_BLOCK_POTATOES:                      return BlockPotatoesHandler;
		case E_BLOCK_POWERED_RAIL:                  return BlockPoweredRailHandler;
		case E_BLOCK_PRISMARINE_BLOCK:              return BlockPrismarineBlockHandler;
		case E_BLOCK_PUMPKIN:                       return BlockPumpkinHandler;
		case E_BLOCK_PUMPKIN_STEM:                  return BlockPumpkinStemHandler;
		case E_BLOCK_PURPLE_GLAZED_TERRACOTTA:      return BlockPurpleGlazedTerracottaHandler;
		case E_BLOCK_PURPLE_SHULKER_BOX:            return BlockPurpleShulkerBoxHandler;
		case E_BLOCK_PURPUR_BLOCK:                  return BlockPurpurBlockHandler;
		case E_BLOCK_PURPUR_DOUBLE_SLAB:            return BlockPurpurDoubleSlabHandler;
		case E_BLOCK_PURPUR_PILLAR:                 return BlockPurpurPillarHandler;
		case E_BLOCK_PURPUR_SLAB:                   return BlockPurpurSlabHandler;
		case E_BLOCK_PURPUR_STAIRS:                 return BlockPurpurStairsHandler;
		case E_BLOCK_QUARTZ_BLOCK:                  return BlockQuartzBlockHandler;
		case E_BLOCK_QUARTZ_STAIRS:                 return BlockQuartzStairsHandler;
		case E_BLOCK_RAIL:                          return BlockRailHandler;
		case E_BLOCK_REDSTONE_LAMP_OFF:             return BlockRedstoneLampHandler;
		case E_BLOCK_REDSTONE_LAMP_ON:              return BlockRedstoneLampHandler;
		case E_BLOCK_REDSTONE_ORE:                  return BlockRedstoneOreHandler;
		case E_BLOCK_REDSTONE_ORE_GLOWING:          return BlockRedstoneOreGlowingHandler;
		case E_BLOCK_REDSTONE_REPEATER_OFF:         return BlockRedstoneRepeaterOffHandler;
		case E_BLOCK_REDSTONE_REPEATER_ON:          return BlockRedstoneRepeaterOnHandler;
		case E_BLOCK_REDSTONE_TORCH_OFF:            return BlockRedstoneTorchOffHandler;
		case E_BLOCK_REDSTONE_TORCH_ON:             return BlockRedstoneTorchOnHandler;
		case E_BLOCK_REDSTONE_WIRE:                 return BlockRedstoneWireHandler;
		case E_BLOCK_RED_GLAZED_TERRACOTTA:         return BlockRedGlazedTerracottaHandler;
		case E_BLOCK_RED_MUSHROOM:                  return BlockRedMushroomHandler;
		case E_BLOCK_RED_NETHER_BRICK:              return BlockRedNetherBrickHandler;
		case E_BLOCK_RED_ROSE:                      return BlockRedRoseHandler;
		case E_BLOCK_RED_SANDSTONE:                 return BlockRedSandstoneHandler;
		case E_BLOCK_RED_SANDSTONE_SLAB:            return BlockRedSandstoneSlabHandler;
		case E_BLOCK_RED_SANDSTONE_STAIRS:          return BlockRedSandstoneStairsHandler;
		case E_BLOCK_RED_SHULKER_BOX:               return BlockRedShulkerBoxHandler;
		case E_BLOCK_REPEATING_COMMAND_BLOCK:       return BlockRepeatingCommandBlockHandler;
		case E_BLOCK_SAND:                          return BlockSandHandler;
		case E_BLOCK_SANDSTONE:                     return BlockSandstoneHandler;
		case E_BLOCK_SANDSTONE_STAIRS:              return BlockSandstoneStairsHandler;
		case E_BLOCK_SAPLING:                       return BlockSaplingHandler;
		case E_BLOCK_SEA_LANTERN:                   return BlockSeaLanternHandler;
		case E_BLOCK_SIGN_POST:                     return BlockSignPostHandler;
		case E_BLOCK_SILVERFISH_EGG:                return BlockInfestedBlockHandler;
		case E_BLOCK_SLIME_BLOCK:                   return BlockSlimeBlockHandler;
		case E_BLOCK_SNOW:                          return BlockSnowHandler;
		case E_BLOCK_SNOW_BLOCK:                    return BlockSnowBlockHandler;
		case E_BLOCK_SOULSAND:                      return BlockSoulSandHandler;
		case E_BLOCK_SPONGE:                        return BlockSpongeHandler;
		case E_BLOCK_SPRUCE_DOOR:                   return BlockSpruceDoorHandler;
		case E_BLOCK_SPRUCE_FENCE:                  return BlockSpruceFenceHandler;
		case E_BLOCK_SPRUCE_FENCE_GATE:             return BlockSpruceFenceGateHandler;
		case E_BLOCK_SPRUCE_WOOD_STAIRS:            return BlockSpruceWoodStairsHandler;
		case E_BLOCK_STAINED_CLAY:                  return BlockStainedClayHandler;
		case E_BLOCK_STAINED_GLASS:                 return BlockStainedGlassHandler;
		case E_BLOCK_STAINED_GLASS_PANE:            return BlockStainedGlassPaneHandler;
		case E_BLOCK_STANDING_BANNER:               return BlockStandingBannerHandler;
		case E_BLOCK_STATIONARY_LAVA:               return BlockStationaryLavaHandler;
		case E_BLOCK_STATIONARY_WATER:              return BlockStationaryWaterHandler;
		case E_BLOCK_STICKY_PISTON:                 return BlockStickyPistonHandler;
		case E_BLOCK_STONE:                         return BlockStoneHandler;
		case E_BLOCK_STONE_BRICKS:                  return BlockStoneBricksHandler;
		case E_BLOCK_STONE_BRICK_STAIRS:            return BlockStoneBrickStairsHandler;
		case E_BLOCK_STONE_BUTTON:                  return BlockStoneButtonHandler;
		case E_BLOCK_STONE_PRESSURE_PLATE:          return BlockStonePressurePlateHandler;
		case E_BLOCK_STONE_SLAB:                    return BlockStoneSlabHandler;
		case E_BLOCK_STRUCTURE_BLOCK:               return BlockStructureBlockHandler;
		case E_BLOCK_STRUCTURE_VOID:                return BlockStructureVoidHandler;
		case E_BLOCK_SUGARCANE:                     return BlockSugarcaneHandler;
		case E_BLOCK_TALL_GRASS:                    return BlockTallGrassHandler;
		case E_BLOCK_TNT:                           return BlockTntHandler;
		case E_BLOCK_TORCH:                         return BlockTorchHandler;
		case E_BLOCK_TRAPDOOR:                      return BlockTrapdoorHandler;
		case E_BLOCK_TRAPPED_CHEST:                 return BlockTrappedChestHandler;
		case E_BLOCK_TRIPWIRE:                      return BlockTripwireHandler;
		case E_BLOCK_TRIPWIRE_HOOK:                 return BlockTripwireHookHandler;
		case E_BLOCK_VINES:                         return BlockVinesHandler;
		case E_BLOCK_WALLSIGN:                      return BlockWallsignHandler;
		case E_BLOCK_WALL_BANNER:                   return BlockWallBannerHandler;
		case E_BLOCK_WATER:                         return BlockWaterHandler;
		case E_BLOCK_WHITE_GLAZED_TERRACOTTA:       return BlockWhiteGlazedTerracottaHandler;
		case E_BLOCK_WHITE_SHULKER_BOX:             return BlockWhiteShulkerBoxHandler;
		case E_BLOCK_WOODEN_BUTTON:                 return BlockWoodenButtonHandler;
		case E_BLOCK_WOODEN_PRESSURE_PLATE:         return BlockWoodenPressurePlateHandler;
		case E_BLOCK_WOODEN_SLAB:                   return BlockWoodenSlabHandler;
		case E_BLOCK_WOOL:                          return BlockWoolHandler;
		case E_BLOCK_WORKBENCH:                     return BlockWorkbenchHandler;
		case E_BLOCK_YELLOW_FLOWER:                 return BlockYellowFlowerHandler;
		case E_BLOCK_YELLOW_GLAZED_TERRACOTTA:      return BlockYellowGlazedTerracottaHandler;
		case E_BLOCK_YELLOW_SHULKER_BOX:            return BlockYellowShulkerBoxHandler;

		// TODO: these will be removed in 1.13+ migration
		case E_BLOCK_NUMBER_OF_TYPES:
		case E_BLOCK_UNFINISHED: return BlockAirHandler;
	}

	UNREACHABLE("Getting handler for unexpected block type");
}
