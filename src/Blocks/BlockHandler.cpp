#include "Globals.h"

#include "BlockHandler.h"
#include "../Item.h"
#include "../World.h"
#include "../Chunk.h"
#include "BlockPluginInterface.h"
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
#include "BlockDirt.h"
#include "BlockDoor.h"
#include "BlockDropSpenser.h"
#include "BlockEnchantingTable.h"
#include "BlockEnderchest.h"
#include "BlockEndPortalFrame.h"
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
#include "BlockGlowstone.h"
#include "BlockGrass.h"
#include "BlockGravel.h"
#include "BlockMobHead.h"
#include "BlockHopper.h"
#include "BlockIce.h"
#include "BlockJukebox.h"
#include "BlockLadder.h"
#include "BlockLeaves.h"
#include "BlockLilypad.h"
#include "BlockLever.h"
#include "BlockMelon.h"
#include "BlockMobSpawner.h"
#include "BlockMushroom.h"
#include "BlockMycelium.h"
#include "BlockNetherrack.h"
#include "BlockNetherWart.h"
#include "BlockObserver.h"
#include "BlockOre.h"
#include "BlockPiston.h"
#include "BlockPlanks.h"
#include "BlockPortal.h"
#include "BlockPumpkin.h"
#include "BlockPressurePlate.h"
#include "BlockQuartz.h"
#include "BlockRail.h"
#include "BlockRedstone.h"
#include "BlockRedstoneLamp.h"
#include "BlockRedstoneOre.h"
#include "BlockRedstoneRepeater.h"
#include "BlockRedstoneTorch.h"
#include "BlockTNT.h"
#include "BlockTripwire.h"
#include "BlockTripwireHook.h"
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
#include "BlockTallGrass.h"
#include "BlockTorch.h"
#include "BlockTrapdoor.h"
#include "BlockVine.h"
#include "BlockWallSign.h"
#include "BlockWorkbench.h"





using cBlockGlazedTerracottaHandler = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>;





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
	constexpr cBlockWithNoDrops<>             BlockAirHandler                   (E_BLOCK_AIR);
	constexpr cBlockAnvilHandler              BlockAnvilHandler                 (E_BLOCK_ANVIL);
	constexpr cBlockHandler                   BlockBarrierHandler               (E_BLOCK_BARRIER);
	constexpr cBlockEntityHandler             BlockBeaconHandler                (E_BLOCK_BEACON);
	constexpr cBlockBedHandler                BlockBedHandler                   (E_BLOCK_BED);
	constexpr cBlockHandler                   BlockBedrockHandler               (E_BLOCK_BEDROCK);
	constexpr cBlockCropsHandler<3>           BlockBeetrootsHandler             (E_BLOCK_BEETROOTS);  // 4 stages of growth
	constexpr cBlockBigFlowerHandler          BlockBigFlowerHandler             (E_BLOCK_BIG_FLOWER);
	constexpr cBlockDoorHandler               BlockBirchDoorHandler             (E_BLOCK_BIRCH_DOOR);
	constexpr cBlockFenceGateHandler          BlockBirchFenceGateHandler        (E_BLOCK_BIRCH_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockBirchFenceHandler            (E_BLOCK_BIRCH_FENCE);
	constexpr cBlockStairsHandler             BlockBirchWoodStairsHandler       (E_BLOCK_BIRCH_WOOD_STAIRS);
	constexpr cBlockGlazedTerracottaHandler   BlockBlackGlazedTerracottaHandler (E_BLOCK_BLACK_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockBlackShulkerBoxHandler       (E_BLOCK_BLACK_SHULKER_BOX);
	constexpr cBlockHandler                   BlockBlockOfCoalHandler           (E_BLOCK_BLOCK_OF_COAL);
	constexpr cBlockHandler                   BlockBlockOfRedstoneHandler       (E_BLOCK_BLOCK_OF_REDSTONE);
	constexpr cBlockGlazedTerracottaHandler   BlockBlueGlazedTerracottaHandler  (E_BLOCK_BLUE_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockBlueShulkerBoxHandler        (E_BLOCK_BLUE_SHULKER_BOX);
	constexpr cBlockHandler                   BlockBoneBlockHandler             (E_BLOCK_BONE_BLOCK);
	constexpr cBlockBookShelfHandler          BlockBookcaseHandler              (E_BLOCK_BOOKCASE);
	constexpr cBlockBrewingStandHandler       BlockBrewingStandHandler          (E_BLOCK_BREWING_STAND);
	constexpr cBlockHandler                   BlockBrickHandler                 (E_BLOCK_BRICK);
	constexpr cBlockStairsHandler             BlockBrickStairsHandler           (E_BLOCK_BRICK_STAIRS);
	constexpr cBlockGlazedTerracottaHandler   BlockBrownGlazedTerracottaHandler (E_BLOCK_BROWN_GLAZED_TERRACOTTA);
	constexpr cBlockMushroomHandler           BlockBrownMushroomHandler         (E_BLOCK_BROWN_MUSHROOM);
	constexpr cBlockHandler                   BlockBrownShulkerBoxHandler       (E_BLOCK_BROWN_SHULKER_BOX);
	constexpr cBlockCactusHandler             BlockCactusHandler                (E_BLOCK_CACTUS);
	constexpr cBlockCakeHandler               BlockCakeHandler                  (E_BLOCK_CAKE);
	constexpr cBlockCarpetHandler             BlockCarpetHandler                (E_BLOCK_CARPET);
	constexpr cBlockCropsHandler<7>           BlockCarrotsHandler               (E_BLOCK_CARROTS);  // 8 stages of growth
	constexpr cBlockCauldronHandler           BlockCauldronHandler              (E_BLOCK_CAULDRON);
	constexpr cBlockCommandBlockHandler       BlockChainCommandBlockHandler     (E_BLOCK_CHAIN_COMMAND_BLOCK);
	constexpr cBlockChestHandler              BlockChestHandler                 (E_BLOCK_CHEST);
	constexpr cBlockHandler                   BlockChorusFlowerHandler          (E_BLOCK_CHORUS_FLOWER);
	constexpr cBlockHandler                   BlockChorusPlantHandler           (E_BLOCK_CHORUS_PLANT);
	constexpr cBlockOreHandler                BlockClayHandler                  (E_BLOCK_CLAY);
	constexpr cBlockOreHandler                BlockCoalOreHandler               (E_BLOCK_COAL_ORE);
	constexpr cBlockStoneHandler              BlockCobblestoneHandler           (E_BLOCK_COBBLESTONE);
	constexpr cBlockStairsHandler             BlockCobblestoneStairsHandler     (E_BLOCK_COBBLESTONE_STAIRS);
	constexpr cBlockHandler                   BlockCobblestoneWallHandler       (E_BLOCK_COBBLESTONE_WALL);
	constexpr cBlockCobWebHandler             BlockCobwebHandler                (E_BLOCK_COBWEB);
	constexpr cBlockCocoaPodHandler           BlockCocoaPodHandler              (E_BLOCK_COCOA_POD);
	constexpr cBlockCommandBlockHandler       BlockCommandBlockHandler          (E_BLOCK_COMMAND_BLOCK);
	constexpr cBlockHandler                   BlockConcreteHandler              (E_BLOCK_CONCRETE);
	constexpr cBlockConcretePowderHandler     BlockConcretePowderHandler        (E_BLOCK_CONCRETE_POWDER);
	constexpr cBlockCropsHandler<7>           BlockCropsHandler                 (E_BLOCK_CROPS);  // 8 stages of growth
	constexpr cBlockGlazedTerracottaHandler   BlockCyanGlazedTerracottaHandler  (E_BLOCK_CYAN_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockCyanShulkerBoxHandler        (E_BLOCK_CYAN_SHULKER_BOX);
	constexpr cBlockDoorHandler               BlockDarkOakDoorHandler           (E_BLOCK_DARK_OAK_DOOR);
	constexpr cBlockFenceGateHandler          BlockDarkOakFenceGateHandler      (E_BLOCK_DARK_OAK_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockDarkOakFenceHandler          (E_BLOCK_DARK_OAK_FENCE);
	constexpr cBlockStairsHandler             BlockDarkOakWoodStairsHandler     (E_BLOCK_DARK_OAK_WOOD_STAIRS);
	constexpr cBlockHandler                   BlockDaylightSensorHandler        (E_BLOCK_DAYLIGHT_SENSOR);
	constexpr cBlockDeadBushHandler           BlockDeadBushHandler              (E_BLOCK_DEAD_BUSH);
	constexpr cBlockRailHandler               BlockDetectorRailHandler          (E_BLOCK_DETECTOR_RAIL);
	constexpr cBlockHandler                   BlockDiamondBlockHandler          (E_BLOCK_DIAMOND_BLOCK);
	constexpr cBlockOreHandler                BlockDiamondOreHandler            (E_BLOCK_DIAMOND_ORE);
	constexpr cBlockDirtHandler               BlockDirtHandler                  (E_BLOCK_DIRT);
	constexpr cBlockDropSpenserHandler        BlockDispenserHandler             (E_BLOCK_DISPENSER);
	constexpr cBlockDoubleSlabHandler         BlockDoubleRedSandstoneSlabHandler(E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB);
	constexpr cBlockDoubleSlabHandler         BlockDoubleStoneSlabHandler       (E_BLOCK_DOUBLE_STONE_SLAB);
	constexpr cBlockDoubleSlabHandler         BlockDoubleWoodenSlabHandler      (E_BLOCK_DOUBLE_WOODEN_SLAB);
	constexpr cBlockHandler                   BlockDragonEggHandler             (E_BLOCK_DRAGON_EGG);
	constexpr cBlockDropSpenserHandler        BlockDropperHandler               (E_BLOCK_DROPPER);
	constexpr cBlockHandler                   BlockEmeraldBlockHandler          (E_BLOCK_EMERALD_BLOCK);
	constexpr cBlockOreHandler                BlockEmeraldOreHandler            (E_BLOCK_EMERALD_ORE);
	constexpr cBlockEnchantingTableHandler    BlockEnchantingTableHandler       (E_BLOCK_ENCHANTMENT_TABLE);
	constexpr cBlockHandler                   BlockEndBricksHandler             (E_BLOCK_END_BRICKS);
	constexpr cBlockHandler                   BlockEndGatewayHandler            (E_BLOCK_END_GATEWAY);
	constexpr cBlockEndPortalFrameHandler     BlockEndPortalFrameHandler        (E_BLOCK_END_PORTAL_FRAME);
	constexpr cBlockHandler                   BlockEndPortalHandler             (E_BLOCK_END_PORTAL);
	constexpr cBlockHandler                   BlockEndRodHandler                (E_BLOCK_END_ROD);
	constexpr cBlockHandler                   BlockEndStoneHandler              (E_BLOCK_END_STONE);
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
	constexpr cBlockHandler                   BlockGoldBlockHandler             (E_BLOCK_GOLD_BLOCK);
	constexpr cBlockOreHandler                BlockGoldOreHandler               (E_BLOCK_GOLD_ORE);
	constexpr cBlockGrassHandler              BlockGrassHandler                 (E_BLOCK_GRASS);
	constexpr cBlockHandler                   BlockGrassPathHandler             (E_BLOCK_GRASS_PATH);
	constexpr cBlockGravelHandler             BlockGravelHandler                (E_BLOCK_GRAVEL);
	constexpr cBlockGlazedTerracottaHandler   BlockGrayGlazedTerracottaHandler  (E_BLOCK_GRAY_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockGrayShulkerBoxHandler        (E_BLOCK_GRAY_SHULKER_BOX);
	constexpr cBlockGlazedTerracottaHandler   BlockGreenGlazedTerracottaHandler (E_BLOCK_GREEN_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockGreenShulkerBoxHandler       (E_BLOCK_GREEN_SHULKER_BOX);
	constexpr cBlockHandler                   BlockHardenedClayHandler          (E_BLOCK_HARDENED_CLAY);
	constexpr cBlockSidewaysHandler           BlockHayBaleHandler               (E_BLOCK_HAY_BALE);
	constexpr cBlockMobHeadHandler            BlockHeadHandler                  (E_BLOCK_HEAD);
	constexpr cBlockPressurePlateHandler      BlockHeavyWeightedPressurePHandler(E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE);
	constexpr cBlockHopperHandler             BlockHopperHandler                (E_BLOCK_HOPPER);
	constexpr cBlockHandler                   BlockHugeBrownMushroomHandler     (E_BLOCK_HUGE_BROWN_MUSHROOM);
	constexpr cBlockHandler                   BlockHugeRedMushroomHandler       (E_BLOCK_HUGE_RED_MUSHROOM);
	constexpr cBlockIceHandler                BlockIceHandler                   (E_BLOCK_ICE);
	constexpr cBlockComparatorHandler         BlockInactiveComparatorHandler    (E_BLOCK_INACTIVE_COMPARATOR);
	constexpr cBlockHandler                   BlockInfestedBlockHandler         (E_BLOCK_SILVERFISH_EGG);
	constexpr cBlockHandler                   BlockIronBarsHandler              (E_BLOCK_IRON_BARS);
	constexpr cBlockHandler                   BlockIronBlockHandler             (E_BLOCK_IRON_BLOCK);
	constexpr cBlockDoorHandler               BlockIronDoorHandler              (E_BLOCK_IRON_DOOR);
	constexpr cBlockOreHandler                BlockIronOreHandler               (E_BLOCK_IRON_ORE);
	constexpr cBlockTrapdoorHandler           BlockIronTrapdoorHandler          (E_BLOCK_IRON_TRAPDOOR);
	constexpr cBlockPumpkinHandler            BlockJackOLanternHandler          (E_BLOCK_JACK_O_LANTERN);
	constexpr cBlockJukeboxHandler            BlockJukeboxHandler               (E_BLOCK_JUKEBOX);
	constexpr cBlockDoorHandler               BlockJungleDoorHandler            (E_BLOCK_JUNGLE_DOOR);
	constexpr cBlockFenceGateHandler          BlockJungleFenceGateHandler       (E_BLOCK_JUNGLE_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockJungleFenceHandler           (E_BLOCK_JUNGLE_FENCE);
	constexpr cBlockStairsHandler             BlockJungleWoodStairsHandler      (E_BLOCK_JUNGLE_WOOD_STAIRS);
	constexpr cBlockLadderHandler             BlockLadderHandler                (E_BLOCK_LADDER);
	constexpr cBlockHandler                   BlockLapisBlockHandler            (E_BLOCK_LAPIS_BLOCK);
	constexpr cBlockOreHandler                BlockLapisOreHandler              (E_BLOCK_LAPIS_ORE);
	constexpr cBlockLavaHandler               BlockLavaHandler                  (E_BLOCK_LAVA);
	constexpr cBlockLeavesHandler             BlockLeavesHandler                (E_BLOCK_LEAVES);
	constexpr cBlockLeverHandler              BlockLeverHandler                 (E_BLOCK_LEVER);
	constexpr cBlockGlazedTerracottaHandler   BlockLightBlueGlazedTerracoHandler(E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockLightBlueShulkerBoxHandler   (E_BLOCK_LIGHT_BLUE_SHULKER_BOX);
	constexpr cBlockGlazedTerracottaHandler   BlockLightGrayGlazedTerracoHandler(E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockLightGrayShulkerBoxHandler   (E_BLOCK_LIGHT_GRAY_SHULKER_BOX);
	constexpr cBlockPressurePlateHandler      BlockLightWeightedPressurePHandler(E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE);
	constexpr cBlockLilypadHandler            BlockLilyPadHandler               (E_BLOCK_LILY_PAD);
	constexpr cBlockGlazedTerracottaHandler   BlockLimeGlazedTerracottaHandler  (E_BLOCK_LIME_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockLimeShulkerBoxHandler        (E_BLOCK_LIME_SHULKER_BOX);
	constexpr cBlockFurnaceHandler            BlockLitFurnaceHandler            (E_BLOCK_LIT_FURNACE);
	constexpr cBlockSidewaysHandler           BlockLogHandler                   (E_BLOCK_LOG);
	constexpr cBlockGlazedTerracottaHandler   BlockMagentaGlazedTerracottHandler(E_BLOCK_MAGENTA_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockMagentaShulkerBoxHandler     (E_BLOCK_MAGENTA_SHULKER_BOX);
	constexpr cBlockHandler                   BlockMagmaHandler                 (E_BLOCK_MAGMA);
	constexpr cBlockMelonHandler              BlockMelonHandler                 (E_BLOCK_MELON);
	constexpr cBlockMelonStemHandler          BlockMelonStemHandler             (E_BLOCK_MELON_STEM);
	constexpr cBlockMobSpawnerHandler         BlockMobSpawnerHandler            (E_BLOCK_MOB_SPAWNER);
	constexpr cBlockHandler                   BlockMossyCobblestoneHandler      (E_BLOCK_MOSSY_COBBLESTONE);
	constexpr cBlockMyceliumHandler           BlockMyceliumHandler              (E_BLOCK_MYCELIUM);
	constexpr cBlockFenceHandler              BlockNetherBrickFenceHandler      (E_BLOCK_NETHER_BRICK_FENCE);
	constexpr cBlockHandler                   BlockNetherBrickHandler           (E_BLOCK_NETHER_BRICK);
	constexpr cBlockStairsHandler             BlockNetherBrickStairsHandler     (E_BLOCK_NETHER_BRICK_STAIRS);
	constexpr cBlockPortalHandler             BlockNetherPortalHandler          (E_BLOCK_NETHER_PORTAL);
	constexpr cBlockOreHandler                BlockNetherQuartzOreHandler       (E_BLOCK_NETHER_QUARTZ_ORE);
	constexpr cBlockHandler                   BlockNetherWartBlockHandler       (E_BLOCK_NETHER_WART_BLOCK);
	constexpr cBlockNetherWartHandler         BlockNetherWartHandler            (E_BLOCK_NETHER_WART);
	constexpr cBlockNetherrack                BlockNetherrackHandler            (E_BLOCK_NETHERRACK);
	constexpr cBlockLeavesHandler             BlockNewLeavesHandler             (E_BLOCK_NEW_LEAVES);
	constexpr cBlockSidewaysHandler           BlockNewLogHandler                (E_BLOCK_NEW_LOG);
	constexpr cBlockEntityHandler             BlockNoteBlockHandler             (E_BLOCK_NOTE_BLOCK);
	constexpr cBlockDoorHandler               BlockOakDoorHandler               (E_BLOCK_OAK_DOOR);
	constexpr cBlockFenceGateHandler          BlockOakFenceGateHandler          (E_BLOCK_OAK_FENCE_GATE);
	constexpr cBlockStairsHandler             BlockOakWoodStairsHandler         (E_BLOCK_OAK_WOOD_STAIRS);
	constexpr cBlockObserverHandler           BlockObserverHandler              (E_BLOCK_OBSERVER);
	constexpr cBlockHandler                   BlockObsidianHandler              (E_BLOCK_OBSIDIAN);
	constexpr cBlockGlazedTerracottaHandler   BlockOrangeGlazedTerracottaHandler(E_BLOCK_ORANGE_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockOrangeShulkerBoxHandler      (E_BLOCK_ORANGE_SHULKER_BOX);
	constexpr cBlockIceHandler                BlockPackedIceHandler             (E_BLOCK_PACKED_ICE);
	constexpr cBlockGlazedTerracottaHandler   BlockPinkGlazedTerracottaHandler  (E_BLOCK_PINK_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockPinkShulkerBoxHandler        (E_BLOCK_PINK_SHULKER_BOX);
	constexpr cBlockPistonHandler             BlockPistonHandler                (E_BLOCK_PISTON);
	constexpr cBlockPistonHeadHandler         BlockPistonHeadHandler;
	constexpr cBlockHandler                   BlockPistonMovedBlockHandler      (E_BLOCK_PISTON_MOVED_BLOCK);
	constexpr cBlockPlanksHandler             BlockPlanksHandler                (E_BLOCK_PLANKS);
	constexpr cBlockCropsHandler<7>           BlockPotatoesHandler              (E_BLOCK_POTATOES);  // 8 stages of growth
	constexpr cBlockRailHandler               BlockPoweredRailHandler           (E_BLOCK_POWERED_RAIL);
	constexpr cBlockHandler                   BlockPrismarineBlockHandler       (E_BLOCK_PRISMARINE_BLOCK);
	constexpr cBlockPumpkinHandler            BlockPumpkinHandler               (E_BLOCK_PUMPKIN);
	constexpr cBlockPumpkinStemHandler        BlockPumpkinStemHandler           (E_BLOCK_PUMPKIN_STEM);
	constexpr cBlockGlazedTerracottaHandler   BlockPurpleGlazedTerracottaHandler(E_BLOCK_PURPLE_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockPurpleShulkerBoxHandler      (E_BLOCK_PURPLE_SHULKER_BOX);
	constexpr cBlockHandler                   BlockPurpurBlockHandler           (E_BLOCK_PURPUR_BLOCK);
	constexpr cBlockDoubleSlabHandler         BlockPurpurDoubleSlabHandler      (E_BLOCK_PURPUR_DOUBLE_SLAB);
	constexpr cBlockHandler                   BlockPurpurPillarHandler          (E_BLOCK_PURPUR_PILLAR);
	constexpr cBlockSlabHandler               BlockPurpurSlabHandler            (E_BLOCK_PURPUR_SLAB);
	constexpr cBlockStairsHandler             BlockPurpurStairsHandler          (E_BLOCK_PURPUR_STAIRS);
	constexpr cBlockQuartzHandler             BlockQuartzBlockHandler           (E_BLOCK_QUARTZ_BLOCK);
	constexpr cBlockStairsHandler             BlockQuartzStairsHandler          (E_BLOCK_QUARTZ_STAIRS);
	constexpr cBlockRailHandler               BlockRailHandler                  (E_BLOCK_RAIL);
	constexpr cBlockGlazedTerracottaHandler   BlockRedGlazedTerracottaHandler   (E_BLOCK_RED_GLAZED_TERRACOTTA);
	constexpr cBlockMushroomHandler           BlockRedMushroomHandler           (E_BLOCK_RED_MUSHROOM);
	constexpr cBlockHandler                   BlockRedNetherBrickHandler        (E_BLOCK_RED_NETHER_BRICK);
	constexpr cBlockFlowerHandler             BlockRedRoseHandler               (E_BLOCK_RED_ROSE);
	constexpr cBlockHandler                   BlockRedSandstoneHandler          (E_BLOCK_RED_SANDSTONE);
	constexpr cBlockSlabHandler               BlockRedSandstoneSlabHandler      (E_BLOCK_RED_SANDSTONE_SLAB);
	constexpr cBlockStairsHandler             BlockRedSandstoneStairsHandler    (E_BLOCK_RED_SANDSTONE_STAIRS);
	constexpr cBlockHandler                   BlockRedShulkerBoxHandler         (E_BLOCK_RED_SHULKER_BOX);
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
	constexpr cBlockHandler                   BlockSandstoneHandler             (E_BLOCK_SANDSTONE);
	constexpr cBlockStairsHandler             BlockSandstoneStairsHandler       (E_BLOCK_SANDSTONE_STAIRS);
	constexpr cBlockSaplingHandler            BlockSaplingHandler               (E_BLOCK_SAPLING);
	constexpr cBlockSeaLanternHandler         BlockSeaLanternHandler            (E_BLOCK_SEA_LANTERN);
	constexpr cBlockSignPostHandler           BlockSignPostHandler              (E_BLOCK_SIGN_POST);
	constexpr cBlockSlimeHandler              BlockSlimeBlockHandler            (E_BLOCK_SLIME_BLOCK);
	constexpr cBlockHandler                   BlockSnowBlockHandler             (E_BLOCK_SNOW_BLOCK);
	constexpr cBlockSnowHandler               BlockSnowHandler                  (E_BLOCK_SNOW);
	constexpr cBlockHandler                   BlockSoulSandHandler              (E_BLOCK_SOULSAND);
	constexpr cBlockSpongeHandler             BlockSpongeHandler                (E_BLOCK_SPONGE);
	constexpr cBlockDoorHandler               BlockSpruceDoorHandler            (E_BLOCK_SPRUCE_DOOR);
	constexpr cBlockFenceGateHandler          BlockSpruceFenceGateHandler       (E_BLOCK_SPRUCE_FENCE_GATE);
	constexpr cBlockFenceHandler              BlockSpruceFenceHandler           (E_BLOCK_SPRUCE_FENCE);
	constexpr cBlockStairsHandler             BlockSpruceWoodStairsHandler      (E_BLOCK_SPRUCE_WOOD_STAIRS);
	constexpr cBlockHandler                   BlockStainedClayHandler           (E_BLOCK_STAINED_CLAY);
	constexpr cBlockGlassHandler              BlockStainedGlassHandler          (E_BLOCK_STAINED_GLASS);
	constexpr cBlockGlassHandler              BlockStainedGlassPaneHandler      (E_BLOCK_STAINED_GLASS_PANE);
	constexpr cBlockHandler                   BlockStandingBannerHandler        (E_BLOCK_STANDING_BANNER);  // TODO: drops correct?
	constexpr cBlockLavaHandler               BlockStationaryLavaHandler        (E_BLOCK_STATIONARY_LAVA);
	constexpr cBlockFluidHandler              BlockStationaryWaterHandler       (E_BLOCK_STATIONARY_WATER);
	constexpr cBlockPistonHandler             BlockStickyPistonHandler          (E_BLOCK_STICKY_PISTON);
	constexpr cBlockStairsHandler             BlockStoneBrickStairsHandler      (E_BLOCK_STONE_BRICK_STAIRS);
	constexpr cBlockHandler                   BlockStoneBricksHandler           (E_BLOCK_STONE_BRICKS);
	constexpr cBlockButtonHandler             BlockStoneButtonHandler           (E_BLOCK_STONE_BUTTON);
	constexpr cBlockStoneHandler              BlockStoneHandler                 (E_BLOCK_STONE);
	constexpr cBlockPressurePlateHandler      BlockStonePressurePlateHandler    (E_BLOCK_STONE_PRESSURE_PLATE);
	constexpr cBlockSlabHandler               BlockStoneSlabHandler             (E_BLOCK_STONE_SLAB);
	constexpr cBlockHandler                   BlockStructureBlockHandler        (E_BLOCK_STRUCTURE_BLOCK);
	constexpr cBlockHandler                   BlockStructureVoidHandler         (E_BLOCK_STRUCTURE_VOID);
	constexpr cBlockSugarcaneHandler          BlockSugarcaneHandler             (E_BLOCK_SUGARCANE);
	constexpr cBlockTallGrassHandler          BlockTallGrassHandler             (E_BLOCK_TALL_GRASS);
	constexpr cBlockTNTHandler                BlockTntHandler                   (E_BLOCK_TNT);
	constexpr cBlockTorchHandler              BlockTorchHandler                 (E_BLOCK_TORCH);
	constexpr cBlockTrapdoorHandler           BlockTrapdoorHandler              (E_BLOCK_TRAPDOOR);
	constexpr cBlockChestHandler              BlockTrappedChestHandler          (E_BLOCK_TRAPPED_CHEST);
	constexpr cBlockTripwireHandler           BlockTripwireHandler              (E_BLOCK_TRIPWIRE);
	constexpr cBlockTripwireHookHandler       BlockTripwireHookHandler          (E_BLOCK_TRIPWIRE_HOOK);
	constexpr cBlockVineHandler               BlockVinesHandler                 (E_BLOCK_VINES);
	constexpr cBlockHandler                   BlockWallBannerHandler            (E_BLOCK_WALL_BANNER);  // TODO: drops correct?
	constexpr cBlockWallSignHandler           BlockWallsignHandler              (E_BLOCK_WALLSIGN);
	constexpr cBlockFluidHandler              BlockWaterHandler                 (E_BLOCK_WATER);
	constexpr cBlockGlazedTerracottaHandler   BlockWhiteGlazedTerracottaHandler (E_BLOCK_WHITE_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockWhiteShulkerBoxHandler       (E_BLOCK_WHITE_SHULKER_BOX);
	constexpr cBlockButtonHandler             BlockWoodenButtonHandler          (E_BLOCK_WOODEN_BUTTON);
	constexpr cBlockPressurePlateHandler      BlockWoodenPressurePlateHandler   (E_BLOCK_WOODEN_PRESSURE_PLATE);
	constexpr cBlockSlabHandler               BlockWoodenSlabHandler            (E_BLOCK_WOODEN_SLAB);
	constexpr cBlockClothHandler              BlockWoolHandler                  (E_BLOCK_WOOL);
	constexpr cBlockWorkbenchHandler          BlockWorkbenchHandler             (E_BLOCK_WORKBENCH);
	constexpr cBlockFlowerHandler             BlockYellowFlowerHandler          (E_BLOCK_YELLOW_FLOWER);
	constexpr cBlockGlazedTerracottaHandler   BlockYellowGlazedTerracottaHandler(E_BLOCK_YELLOW_GLAZED_TERRACOTTA);
	constexpr cBlockHandler                   BlockYellowShulkerBoxHandler      (E_BLOCK_YELLOW_SHULKER_BOX);
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





cItems cBlockHandler::ConvertToPickups(
	NIBBLETYPE a_BlockMeta,
	cBlockEntity * a_BlockEntity,
	const cEntity * a_Digger,
	const cItem * a_Tool
) const
{
	UNUSED(a_BlockEntity);
	UNUSED(a_Digger);
	UNUSED(a_Tool);

	// Add self:
	cItems res;
	res.push_back(cItem(m_BlockType, 1, a_BlockMeta));
	return res;
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





unsigned int cBlockHandler::ToolFortuneLevel(const cItem * a_Tool)
{
	if (a_Tool != nullptr)
	{
		return a_Tool->m_Enchantments.GetLevel(cEnchantments::enchFortune);
	}
	else  // Not a tool
	{
		return 0;
	}
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





////////////////////////////////////////////////////////////////////////////////
// Temporary:

void Temporary::RegisterAllBlockHandlers(BlockTypeRegistry & aRegistry)
{
	static struct
	{
		const char * newBlockTypeName;
		BLOCKTYPE oldBlockType;
	} regs[] =
	{
		{"minecraft:acacia_button",                  E_BLOCK_WOODEN_BUTTON},
		{"minecraft:acacia_door",                    E_BLOCK_ACACIA_DOOR},
		{"minecraft:acacia_fence",                   E_BLOCK_ACACIA_FENCE},
		{"minecraft:acacia_fence_gate",              E_BLOCK_ACACIA_FENCE_GATE},
		{"minecraft:acacia_leaves",                  E_BLOCK_LEAVES},
		{"minecraft:acacia_log",                     E_BLOCK_LOG},
		{"minecraft:acacia_planks",                  E_BLOCK_PLANKS},
		{"minecraft:acacia_pressure_plate",          E_BLOCK_WOODEN_PRESSURE_PLATE},
		{"minecraft:acacia_sapling",                 E_BLOCK_SAPLING},
		{"minecraft:acacia_sign",                    E_BLOCK_SIGN_POST},
		{"minecraft:acacia_slab",                    E_BLOCK_WOODEN_SLAB},
		{"minecraft:acacia_stairs",                  E_BLOCK_ACACIA_WOOD_STAIRS},
		{"minecraft:acacia_trapdoor",                E_BLOCK_TRAPDOOR},
		{"minecraft:acacia_wall_sign",               E_BLOCK_WALLSIGN},
		{"minecraft:acacia_wood",                    E_BLOCK_UNFINISHED},
		{"minecraft:activator_rail",                 E_BLOCK_ACTIVATOR_RAIL},
		{"minecraft:air",                            E_BLOCK_AIR},
		{"minecraft:allium",                         E_BLOCK_FLOWER},
		{"minecraft:andesite",                       E_BLOCK_UNFINISHED},
		{"minecraft:andesite_slab",                  E_BLOCK_STONE_SLAB},
		{"minecraft:andesite_stairs",                E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:andesite_wall",                  E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:anvil",                          E_BLOCK_ANVIL},
		{"minecraft:attached_melon_stem",            E_BLOCK_MELON_STEM},
		{"minecraft:attached_pumpkin_stem",          E_BLOCK_PUMPKIN_STEM},
		{"minecraft:azure_bluet",                    E_BLOCK_FLOWER},
		{"minecraft:bamboo",                         E_BLOCK_UNFINISHED},
		{"minecraft:bamboo_sapling",                 E_BLOCK_SAPLING},
		{"minecraft:barrel",                         E_BLOCK_UNFINISHED},
		{"minecraft:barrier",                        E_BLOCK_BARRIER},
		{"minecraft:beacon",                         E_BLOCK_BEACON},
		{"minecraft:bedrock",                        E_BLOCK_BEDROCK},
		{"minecraft:beetroots",                      E_BLOCK_BEETROOTS},
		{"minecraft:bell",                           E_BLOCK_UNFINISHED},
		{"minecraft:birch_button",                   E_BLOCK_WOODEN_BUTTON},
		{"minecraft:birch_door",                     E_BLOCK_ACACIA_DOOR},
		{"minecraft:birch_fence",                    E_BLOCK_ACACIA_FENCE},
		{"minecraft:birch_fence_gate",               E_BLOCK_ACACIA_FENCE_GATE},
		{"minecraft:birch_leaves",                   E_BLOCK_LEAVES},
		{"minecraft:birch_log",                      E_BLOCK_LOG},
		{"minecraft:birch_planks",                   E_BLOCK_PLANKS},
		{"minecraft:birch_pressure_plate",           E_BLOCK_WOODEN_PRESSURE_PLATE},
		{"minecraft:birch_sapling",                  E_BLOCK_SAPLING},
		{"minecraft:birch_sign",                     E_BLOCK_SIGN_POST},
		{"minecraft:birch_slab",                     E_BLOCK_WOODEN_SLAB},
		{"minecraft:birch_stairs",                   E_BLOCK_BIRCH_WOOD_STAIRS},
		{"minecraft:birch_trapdoor",                 E_BLOCK_TRAPDOOR},
		{"minecraft:birch_wall_sign",                E_BLOCK_WALLSIGN},
		{"minecraft:birch_wood",                     E_BLOCK_UNFINISHED},
		{"minecraft:black_banner",                   E_BLOCK_STANDING_BANNER},
		{"minecraft:black_bed",                      E_BLOCK_BED},
		{"minecraft:black_carpet",                   E_BLOCK_CARPET},
		{"minecraft:black_concrete",                 E_BLOCK_CONCRETE},
		{"minecraft:black_concrete_powder",          E_BLOCK_CONCRETE_POWDER},
		{"minecraft:black_glazed_terracotta",        E_BLOCK_BLACK_GLAZED_TERRACOTTA},
		{"minecraft:black_shulker_box",              E_BLOCK_BLACK_SHULKER_BOX},
		{"minecraft:black_stained_glass",            E_BLOCK_STAINED_GLASS},
		{"minecraft:black_stained_glass_pane",       E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:black_terracotta",               E_BLOCK_TERRACOTTA},
		{"minecraft:black_wall_banner",              E_BLOCK_WALL_BANNER},
		{"minecraft:black_wool",                     E_BLOCK_WOOL},
		{"minecraft:blast_furnace",                  E_BLOCK_UNFINISHED},
		{"minecraft:blue_banner",                    E_BLOCK_STANDING_BANNER},
		{"minecraft:blue_bed",                       E_BLOCK_BED},
		{"minecraft:blue_carpet",                    E_BLOCK_CARPET},
		{"minecraft:blue_concrete",                  E_BLOCK_CONCRETE},
		{"minecraft:blue_concrete_powder",           E_BLOCK_CONCRETE_POWDER},
		{"minecraft:blue_glazed_terracotta",         E_BLOCK_UNFINISHED},
		{"minecraft:blue_ice",                       E_BLOCK_UNFINISHED},
		{"minecraft:blue_orchid",                    E_BLOCK_FLOWER},
		{"minecraft:blue_shulker_box",               E_BLOCK_BLUE_SHULKER_BOX},
		{"minecraft:blue_stained_glass",             E_BLOCK_STAINED_GLASS},
		{"minecraft:blue_stained_glass_pane",        E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:blue_terracotta",                E_BLOCK_TERRACOTTA},
		{"minecraft:blue_wall_banner",               E_BLOCK_WALL_BANNER},
		{"minecraft:blue_wool",                      E_BLOCK_WOOL},
		{"minecraft:bone_block",                     E_BLOCK_BONE_BLOCK},
		{"minecraft:bookshelf",                      E_BLOCK_BOOKCASE},
		{"minecraft:brain_coral",                    E_BLOCK_UNFINISHED},
		{"minecraft:brain_coral_block",              E_BLOCK_UNFINISHED},
		{"minecraft:brain_coral_fan",                E_BLOCK_UNFINISHED},
		{"minecraft:brain_coral_wall_fan",           E_BLOCK_UNFINISHED},
		{"minecraft:brewing_stand",                  E_BLOCK_BREWING_STAND},
		{"minecraft:brick_slab",                     E_BLOCK_WOODEN_SLAB},
		{"minecraft:brick_stairs",                   E_BLOCK_BRICK_STAIRS},
		{"minecraft:brick_wall",                     E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:bricks",                         E_BLOCK_BRICK},
		{"minecraft:brown_banner",                   E_BLOCK_STANDING_BANNER},
		{"minecraft:brown_bed",                      E_BLOCK_BED},
		{"minecraft:brown_carpet",                   E_BLOCK_CARPET},
		{"minecraft:brown_concrete",                 E_BLOCK_CONCRETE},
		{"minecraft:brown_concrete_powder",          E_BLOCK_CONCRETE_POWDER},
		{"minecraft:brown_glazed_terracotta",        E_BLOCK_BROWN_GLAZED_TERRACOTTA},
		{"minecraft:brown_mushroom",                 E_BLOCK_BROWN_MUSHROOM},
		{"minecraft:brown_mushroom_block",           E_BLOCK_HUGE_BROWN_MUSHROOM},
		{"minecraft:brown_shulker_box",              E_BLOCK_BROWN_SHULKER_BOX},
		{"minecraft:brown_stained_glass",            E_BLOCK_STAINED_GLASS},
		{"minecraft:brown_stained_glass_pane",       E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:brown_terracotta",               E_BLOCK_TERRACOTTA},
		{"minecraft:brown_wall_banner",              E_BLOCK_WALL_BANNER},
		{"minecraft:brown_wool",                     E_BLOCK_WOOL},
		{"minecraft:bubble_column",                  E_BLOCK_UNFINISHED},
		{"minecraft:bubble_coral",                   E_BLOCK_UNFINISHED},
		{"minecraft:bubble_coral_block",             E_BLOCK_UNFINISHED},
		{"minecraft:bubble_coral_fan",               E_BLOCK_UNFINISHED},
		{"minecraft:bubble_coral_wall_fan",          E_BLOCK_UNFINISHED},
		{"minecraft:cactus",                         E_BLOCK_CACTUS},
		{"minecraft:cake",                           E_BLOCK_CAKE},
		{"minecraft:campfire",                       E_BLOCK_UNFINISHED},
		{"minecraft:carrots",                        E_BLOCK_CARROTS},
		{"minecraft:cartography_table",              E_BLOCK_UNFINISHED},
		{"minecraft:carved_pumpkin",                 E_BLOCK_UNFINISHED},
		{"minecraft:cauldron",                       E_BLOCK_CAULDRON},
		{"minecraft:cave_air",                       E_BLOCK_UNFINISHED},
		{"minecraft:chain_command_block",            E_BLOCK_COMMAND_BLOCK},
		{"minecraft:chest",                          E_BLOCK_CHEST},
		{"minecraft:chipped_anvil",                  E_BLOCK_ANVIL},
		{"minecraft:chiseled_quartz_block",          E_BLOCK_QUARTZ_BLOCK},
		{"minecraft:chiseled_red_sandstone",         E_BLOCK_RED_SANDSTONE},
		{"minecraft:chiseled_sandstone",             E_BLOCK_SANDSTONE},
		{"minecraft:chiseled_stone_bricks",          E_BLOCK_STONE_BRICKS},
		{"minecraft:chorus_flower",                  E_BLOCK_CHORUS_FLOWER},
		{"minecraft:chorus_plant",                   E_BLOCK_CHORUS_PLANT},
		{"minecraft:clay",                           E_BLOCK_CLAY},
		{"minecraft:coal_block",                     E_BLOCK_BLOCK_OF_COAL},
		{"minecraft:coal_ore",                       E_BLOCK_COAL_ORE},
		{"minecraft:coarse_dirt",                    E_BLOCK_DIRT},
		{"minecraft:cobblestone",                    E_BLOCK_COBBLESTONE},
		{"minecraft:cobblestone_slab",               E_BLOCK_STONE_SLAB},
		{"minecraft:cobblestone_stairs",             E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:cobblestone_wall",               E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:cobweb",                         E_BLOCK_COBWEB},
		{"minecraft:cocoa",                          E_BLOCK_COCOA_POD},
		{"minecraft:command_block",                  E_BLOCK_COMMAND_BLOCK},
		{"minecraft:comparator",                     E_BLOCK_UNFINISHED},  // We only have active / inactive blocks
		{"minecraft:composter",                      E_BLOCK_UNFINISHED},
		{"minecraft:conduit",                        E_BLOCK_UNFINISHED},
		{"minecraft:cornflower",                     E_BLOCK_FLOWER},
		{"minecraft:cracked_stone_bricks",           E_BLOCK_STONE_BRICKS},
		{"minecraft:crafting_table",                 E_BLOCK_CRAFTING_TABLE},
		{"minecraft:creeper_head",                   E_BLOCK_HEAD},
		{"minecraft:creeper_wall_head",              E_BLOCK_HEAD},
		{"minecraft:cut_red_sandstone",              E_BLOCK_RED_SANDSTONE},
		{"minecraft:cut_red_sandstone_slab",         E_BLOCK_RED_SANDSTONE_SLAB},
		{"minecraft:cut_sandstone",                  E_BLOCK_SANDSTONE},
		{"minecraft:cut_sandstone_slab",             E_BLOCK_RED_SANDSTONE_SLAB},
		{"minecraft:cyan_banner",                    E_BLOCK_STANDING_BANNER},
		{"minecraft:cyan_bed",                       E_BLOCK_BED},
		{"minecraft:cyan_carpet",                    E_BLOCK_CARPET},
		{"minecraft:cyan_concrete",                  E_BLOCK_CONCRETE},
		{"minecraft:cyan_concrete_powder",           E_BLOCK_CONCRETE_POWDER},
		{"minecraft:cyan_glazed_terracotta",         E_BLOCK_CYAN_GLAZED_TERRACOTTA},
		{"minecraft:cyan_shulker_box",               E_BLOCK_CYAN_SHULKER_BOX},
		{"minecraft:cyan_stained_glass",             E_BLOCK_STAINED_GLASS},
		{"minecraft:cyan_stained_glass_pane",        E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:cyan_terracotta",                E_BLOCK_TERRACOTTA},
		{"minecraft:cyan_wall_banner",               E_BLOCK_WALL_BANNER},
		{"minecraft:cyan_wool",                      E_BLOCK_WOOL},
		{"minecraft:damaged_anvil",                  E_BLOCK_ANVIL},
		{"minecraft:dandelion",                      E_BLOCK_FLOWER},
		{"minecraft:dark_oak_button",                E_BLOCK_WOODEN_BUTTON},
		{"minecraft:dark_oak_door",                  E_BLOCK_WOODEN_DOOR},
		{"minecraft:dark_oak_fence",                 E_BLOCK_FENCE},
		{"minecraft:dark_oak_fence_gate",            E_BLOCK_FENCE_GATE},
		{"minecraft:dark_oak_leaves",                E_BLOCK_LEAVES},
		{"minecraft:dark_oak_log",                   E_BLOCK_LOG},
		{"minecraft:dark_oak_planks",                E_BLOCK_PLANKS},
		{"minecraft:dark_oak_pressure_plate",        E_BLOCK_WOODEN_PRESSURE_PLATE},
		{"minecraft:dark_oak_sapling",               E_BLOCK_SAPLING},
		{"minecraft:dark_oak_sign",                  E_BLOCK_SIGN_POST},
		{"minecraft:dark_oak_slab",                  E_BLOCK_WOODEN_SLAB},
		{"minecraft:dark_oak_stairs",                E_BLOCK_WOODEN_STAIRS},
		{"minecraft:dark_oak_trapdoor",              E_BLOCK_TRAPDOOR},
		{"minecraft:dark_oak_wall_sign",             E_BLOCK_WALLSIGN},
		{"minecraft:dark_oak_wood",                  E_BLOCK_UNFINISHED},
		{"minecraft:dark_prismarine",                E_BLOCK_PRISMARINE_BLOCK},
		{"minecraft:dark_prismarine_slab",           E_BLOCK_UNFINISHED},
		{"minecraft:dark_prismarine_stairs",         E_BLOCK_UNFINISHED},
		{"minecraft:daylight_detector",              E_BLOCK_DAYLIGHT_SENSOR},
		{"minecraft:dead_brain_coral",               E_BLOCK_UNFINISHED},
		{"minecraft:dead_brain_coral_block",         E_BLOCK_UNFINISHED},
		{"minecraft:dead_brain_coral_fan",           E_BLOCK_UNFINISHED},
		{"minecraft:dead_brain_coral_wall_fan",      E_BLOCK_UNFINISHED},
		{"minecraft:dead_bubble_coral",              E_BLOCK_UNFINISHED},
		{"minecraft:dead_bubble_coral_block",        E_BLOCK_UNFINISHED},
		{"minecraft:dead_bubble_coral_fan",          E_BLOCK_UNFINISHED},
		{"minecraft:dead_bubble_coral_wall_fan",     E_BLOCK_UNFINISHED},
		{"minecraft:dead_bush",                      E_BLOCK_DEAD_BUSH},
		{"minecraft:dead_fire_coral",                E_BLOCK_UNFINISHED},
		{"minecraft:dead_fire_coral_block",          E_BLOCK_UNFINISHED},
		{"minecraft:dead_fire_coral_fan",            E_BLOCK_UNFINISHED},
		{"minecraft:dead_fire_coral_wall_fan",       E_BLOCK_UNFINISHED},
		{"minecraft:dead_horn_coral",                E_BLOCK_UNFINISHED},
		{"minecraft:dead_horn_coral_block",          E_BLOCK_UNFINISHED},
		{"minecraft:dead_horn_coral_fan",            E_BLOCK_UNFINISHED},
		{"minecraft:dead_horn_coral_wall_fan",       E_BLOCK_UNFINISHED},
		{"minecraft:dead_tube_coral",                E_BLOCK_UNFINISHED},
		{"minecraft:dead_tube_coral_block",          E_BLOCK_UNFINISHED},
		{"minecraft:dead_tube_coral_fan",            E_BLOCK_UNFINISHED},
		{"minecraft:dead_tube_coral_wall_fan",       E_BLOCK_UNFINISHED},
		{"minecraft:detector_rail",                  E_BLOCK_DETECTOR_RAIL},
		{"minecraft:diamond_block",                  E_BLOCK_DIAMOND_BLOCK},
		{"minecraft:diamond_ore",                    E_BLOCK_DIAMOND_ORE},
		{"minecraft:diorite",                        E_BLOCK_UNFINISHED},
		{"minecraft:diorite_slab",                   E_BLOCK_STONE_SLAB},
		{"minecraft:diorite_stairs",                 E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:diorite_wall",                   E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:dirt",                           E_BLOCK_DIRT},
		{"minecraft:dispenser",                      E_BLOCK_DISPENSER},
		{"minecraft:dragon_egg",                     E_BLOCK_DRAGON_EGG},
		{"minecraft:dragon_head",                    E_BLOCK_UNFINISHED},
		{"minecraft:dragon_wall_head",               E_BLOCK_UNFINISHED},
		{"minecraft:dried_kelp_block",               E_BLOCK_UNFINISHED},
		{"minecraft:dropper",                        E_BLOCK_DROPPER},
		{"minecraft:emerald_block",                  E_BLOCK_EMERALD_BLOCK},
		{"minecraft:emerald_ore",                    E_BLOCK_EMERALD_ORE},
		{"minecraft:enchanting_table",               E_BLOCK_ENCHANTMENT_TABLE},
		{"minecraft:end_gateway",                    E_BLOCK_END_GATEWAY},
		{"minecraft:end_portal",                     E_BLOCK_END_PORTAL},
		{"minecraft:end_portal_frame",               E_BLOCK_END_PORTAL_FRAME},
		{"minecraft:end_rod",                        E_BLOCK_END_ROD},
		{"minecraft:end_stone",                      E_BLOCK_END_STONE},
		{"minecraft:end_stone_brick_slab",           E_BLOCK_STONE_SLAB},
		{"minecraft:end_stone_brick_stairs",         E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:end_stone_brick_wall",           E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:end_stone_bricks",               E_BLOCK_UNFINISHED},
		{"minecraft:ender_chest",                    E_BLOCK_ENDER_CHEST},
		{"minecraft:farmland",                       E_BLOCK_FARMLAND},
		{"minecraft:fern",                           E_BLOCK_TALL_GRASS},
		{"minecraft:fire",                           E_BLOCK_FIRE},
		{"minecraft:fire_coral",                     E_BLOCK_UNFINISHED},
		{"minecraft:fire_coral_block",               E_BLOCK_UNFINISHED},
		{"minecraft:fire_coral_fan",                 E_BLOCK_UNFINISHED},
		{"minecraft:fire_coral_wall_fan",            E_BLOCK_UNFINISHED},
		{"minecraft:fletching_table",                E_BLOCK_UNFINISHED},
		{"minecraft:flower_pot",                     E_BLOCK_FLOWER_POT},
		{"minecraft:frosted_ice",                    E_BLOCK_FROSTED_ICE},
		{"minecraft:furnace",                        E_BLOCK_FURNACE},
		{"minecraft:glass",                          E_BLOCK_GLASS},
		{"minecraft:glass_pane",                     E_BLOCK_GLASS_PANE},
		{"minecraft:glowstone",                      E_BLOCK_GLOWSTONE},
		{"minecraft:gold_block",                     E_BLOCK_GOLD_BLOCK},
		{"minecraft:gold_ore",                       E_BLOCK_GOLD_ORE},
		{"minecraft:granite",                        E_BLOCK_UNFINISHED},
		{"minecraft:granite_slab",                   E_BLOCK_STONE_SLAB},
		{"minecraft:granite_stairs",                 E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:granite_wall",                   E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:grass",                          E_BLOCK_TALL_GRASS},
		{"minecraft:grass_block",                    E_BLOCK_GRASS},
		{"minecraft:grass_path",                     E_BLOCK_UNFINISHED},
		{"minecraft:gravel",                         E_BLOCK_GRAVEL},
		{"minecraft:gray_banner",                    E_BLOCK_STANDING_BANNER},
		{"minecraft:gray_bed",                       E_BLOCK_BED},
		{"minecraft:gray_carpet",                    E_BLOCK_CARPET},
		{"minecraft:gray_concrete",                  E_BLOCK_CONCRETE},
		{"minecraft:gray_concrete_powder",           E_BLOCK_CONCRETE_POWDER},
		{"minecraft:gray_glazed_terracotta",         E_BLOCK_GRAY_GLAZED_TERRACOTTA},
		{"minecraft:gray_shulker_box",               E_BLOCK_GRAY_SHULKER_BOX},
		{"minecraft:gray_stained_glass",             E_BLOCK_STAINED_GLASS},
		{"minecraft:gray_stained_glass_pane",        E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:gray_terracotta",                E_BLOCK_TERRACOTTA},
		{"minecraft:gray_wall_banner",               E_BLOCK_WALL_BANNER},
		{"minecraft:gray_wool",                      E_BLOCK_WOOL},
		{"minecraft:green_banner",                   E_BLOCK_STANDING_BANNER},
		{"minecraft:green_bed",                      E_BLOCK_BED},
		{"minecraft:green_carpet",                   E_BLOCK_CARPET},
		{"minecraft:green_concrete",                 E_BLOCK_CONCRETE},
		{"minecraft:green_concrete_powder",          E_BLOCK_CONCRETE_POWDER},
		{"minecraft:green_glazed_terracotta",        E_BLOCK_GREEN_GLAZED_TERRACOTTA},
		{"minecraft:green_shulker_box",              E_BLOCK_GREEN_SHULKER_BOX},
		{"minecraft:green_stained_glass",            E_BLOCK_STAINED_GLASS},
		{"minecraft:green_stained_glass_pane",       E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:green_terracotta",               E_BLOCK_TERRACOTTA},
		{"minecraft:green_wall_banner",              E_BLOCK_WALL_BANNER},
		{"minecraft:green_wool",                     E_BLOCK_WOOL},
		{"minecraft:grindstone",                     E_BLOCK_UNFINISHED},
		{"minecraft:hay_block",                      E_BLOCK_HAY_BALE},
		{"minecraft:heavy_weighted_pressure_plate",  E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE},
		{"minecraft:hopper",                         E_BLOCK_HOPPER},
		{"minecraft:horn_coral",                     E_BLOCK_UNFINISHED},
		{"minecraft:horn_coral_block",               E_BLOCK_UNFINISHED},
		{"minecraft:horn_coral_fan",                 E_BLOCK_UNFINISHED},
		{"minecraft:horn_coral_wall_fan",            E_BLOCK_UNFINISHED},
		{"minecraft:ice",                            E_BLOCK_ICE},
		{"minecraft:infested_chiseled_stone_bricks", E_BLOCK_SILVERFISH_EGG},
		{"minecraft:infested_cobblestone",           E_BLOCK_SILVERFISH_EGG},
		{"minecraft:infested_cracked_stone_bricks",  E_BLOCK_SILVERFISH_EGG},
		{"minecraft:infested_mossy_stone_bricks",    E_BLOCK_SILVERFISH_EGG},
		{"minecraft:infested_stone",                 E_BLOCK_SILVERFISH_EGG},
		{"minecraft:infested_stone_bricks",          E_BLOCK_SILVERFISH_EGG},
		{"minecraft:iron_bars",                      E_BLOCK_IRON_BARS},
		{"minecraft:iron_block",                     E_BLOCK_IRON_BLOCK},
		{"minecraft:iron_door",                      E_BLOCK_IRON_DOOR},
		{"minecraft:iron_ore",                       E_BLOCK_IRON_ORE},
		{"minecraft:iron_trapdoor",                  E_BLOCK_IRON_TRAPDOOR},
		{"minecraft:jack_o_lantern",                 E_BLOCK_JACK_O_LANTERN},
		{"minecraft:jigsaw",                         E_BLOCK_UNFINISHED},
		{"minecraft:jukebox",                        E_BLOCK_JUKEBOX},
		{"minecraft:jungle_button",                  E_BLOCK_WOODEN_BUTTON},
		{"minecraft:jungle_door",                    E_BLOCK_WOODEN_DOOR},
		{"minecraft:jungle_fence",                   E_BLOCK_FENCE},
		{"minecraft:jungle_fence_gate",              E_BLOCK_FENCE_GATE},
		{"minecraft:jungle_leaves",                  E_BLOCK_LEAVES},
		{"minecraft:jungle_log",                     E_BLOCK_LOG},
		{"minecraft:jungle_planks",                  E_BLOCK_PLANKS},
		{"minecraft:jungle_pressure_plate",          E_BLOCK_WOODEN_PRESSURE_PLATE},
		{"minecraft:jungle_sapling",                 E_BLOCK_SAPLING},
		{"minecraft:jungle_sign",                    E_BLOCK_SIGN_POST},
		{"minecraft:jungle_slab",                    E_BLOCK_WOODEN_SLAB},
		{"minecraft:jungle_stairs",                  E_BLOCK_WOODEN_STAIRS},
		{"minecraft:jungle_trapdoor",                E_BLOCK_TRAPDOOR},
		{"minecraft:jungle_wall_sign",               E_BLOCK_WALLSIGN},
		{"minecraft:jungle_wood",                    E_BLOCK_UNFINISHED},
		{"minecraft:kelp",                           E_BLOCK_UNFINISHED},
		{"minecraft:kelp_plant",                     E_BLOCK_UNFINISHED},
		{"minecraft:ladder",                         E_BLOCK_LADDER},
		{"minecraft:lantern",                        E_BLOCK_UNFINISHED},
		{"minecraft:lapis_block",                    E_BLOCK_LAPIS_BLOCK},
		{"minecraft:lapis_ore",                      E_BLOCK_LAPIS_ORE},
		{"minecraft:large_fern",                     E_BLOCK_BIG_FLOWER},
		{"minecraft:lava",                           E_BLOCK_LAVA},
		{"minecraft:lectern",                        E_BLOCK_UNFINISHED},
		{"minecraft:lever",                          E_BLOCK_LEVER},
		{"minecraft:light_blue_banner",              E_BLOCK_STANDING_BANNER},
		{"minecraft:light_blue_bed",                 E_BLOCK_BED},
		{"minecraft:light_blue_carpet",              E_BLOCK_CARPET},
		{"minecraft:light_blue_concrete",            E_BLOCK_CONCRETE},
		{"minecraft:light_blue_concrete_powder",     E_BLOCK_CONCRETE_POWDER},
		{"minecraft:light_blue_glazed_terracotta",   E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA},
		{"minecraft:light_blue_shulker_box",         E_BLOCK_LIGHT_BLUE_SHULKER_BOX},
		{"minecraft:light_blue_stained_glass",       E_BLOCK_STAINED_GLASS},
		{"minecraft:light_blue_stained_glass_pane",  E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:light_blue_terracotta",          E_BLOCK_TERRACOTTA},
		{"minecraft:light_blue_wall_banner",         E_BLOCK_WALL_BANNER},
		{"minecraft:light_blue_wool",                E_BLOCK_WOOL},
		{"minecraft:light_gray_banner",              E_BLOCK_STANDING_BANNER},
		{"minecraft:light_gray_bed",                 E_BLOCK_BED},
		{"minecraft:light_gray_carpet",              E_BLOCK_CARPET},
		{"minecraft:light_gray_concrete",            E_BLOCK_CONCRETE},
		{"minecraft:light_gray_concrete_powder",     E_BLOCK_CONCRETE_POWDER},
		{"minecraft:light_gray_glazed_terracotta",   E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA},
		{"minecraft:light_gray_shulker_box",         E_BLOCK_LIGHT_GRAY_SHULKER_BOX},
		{"minecraft:light_gray_stained_glass",       E_BLOCK_STAINED_GLASS},
		{"minecraft:light_gray_stained_glass_pane",  E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:light_gray_terracotta",          E_BLOCK_TERRACOTTA},
		{"minecraft:light_gray_wall_banner",         E_BLOCK_WALL_BANNER},
		{"minecraft:light_gray_wool",                E_BLOCK_WOOL},
		{"minecraft:light_weighted_pressure_plate",  E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE},
		{"minecraft:lilac",                          E_BLOCK_BIG_FLOWER},
		{"minecraft:lily_of_the_valley",             E_BLOCK_FLOWER},
		{"minecraft:lily_pad",                       E_BLOCK_LILY_PAD},
		{"minecraft:lime_banner",                    E_BLOCK_STANDING_BANNER},
		{"minecraft:lime_bed",                       E_BLOCK_BED},
		{"minecraft:lime_carpet",                    E_BLOCK_CARPET},
		{"minecraft:lime_concrete",                  E_BLOCK_CONCRETE},
		{"minecraft:lime_concrete_powder",           E_BLOCK_CONCRETE_POWDER},
		{"minecraft:lime_glazed_terracotta",         E_BLOCK_LIME_GLAZED_TERRACOTTA},
		{"minecraft:lime_shulker_box",               E_BLOCK_LIME_SHULKER_BOX},
		{"minecraft:lime_stained_glass",             E_BLOCK_STAINED_GLASS},
		{"minecraft:lime_stained_glass_pane",        E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:lime_terracotta",                E_BLOCK_TERRACOTTA},
		{"minecraft:lime_wall_banner",               E_BLOCK_WALL_BANNER},
		{"minecraft:lime_wool",                      E_BLOCK_WOOL},
		{"minecraft:loom",                           E_BLOCK_UNFINISHED},
		{"minecraft:magenta_banner",                 E_BLOCK_STANDING_BANNER},
		{"minecraft:magenta_bed",                    E_BLOCK_BED},
		{"minecraft:magenta_carpet",                 E_BLOCK_CARPET},
		{"minecraft:magenta_concrete",               E_BLOCK_CONCRETE},
		{"minecraft:magenta_concrete_powder",        E_BLOCK_CONCRETE_POWDER},
		{"minecraft:magenta_glazed_terracotta",      E_BLOCK_MAGENTA_GLAZED_TERRACOTTA},
		{"minecraft:magenta_shulker_box",            E_BLOCK_MAGENTA_SHULKER_BOX},
		{"minecraft:magenta_stained_glass",          E_BLOCK_STAINED_GLASS},
		{"minecraft:magenta_stained_glass_pane",     E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:magenta_terracotta",             E_BLOCK_TERRACOTTA},
		{"minecraft:magenta_wall_banner",            E_BLOCK_WALL_BANNER},
		{"minecraft:magenta_wool",                   E_BLOCK_WOOL},
		{"minecraft:magma_block",                    E_BLOCK_MAGMA},
		{"minecraft:melon",                          E_BLOCK_MELON},
		{"minecraft:melon_stem",                     E_BLOCK_MELON_STEM},
		{"minecraft:mossy_cobblestone",              E_BLOCK_MOSSY_COBBLESTONE},
		{"minecraft:mossy_cobblestone_slab",         E_BLOCK_STONE_SLAB},
		{"minecraft:mossy_cobblestone_stairs",       E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:mossy_cobblestone_wall",         E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:mossy_stone_brick_slab",         E_BLOCK_STONE_SLAB},
		{"minecraft:mossy_stone_brick_stairs",       E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:mossy_stone_brick_wall",         E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:mossy_stone_bricks",             E_BLOCK_STONE_BRICKS},
		{"minecraft:moving_piston",                  E_BLOCK_UNFINISHED},
		{"minecraft:mushroom_stem",                  E_BLOCK_HUGE_BROWN_MUSHROOM},
		{"minecraft:mycelium",                       E_BLOCK_MYCELIUM},
		{"minecraft:nether_brick_fence",             E_BLOCK_NETHER_BRICK_FENCE},
		{"minecraft:nether_brick_slab",              E_BLOCK_STONE_SLAB},
		{"minecraft:nether_brick_stairs",            E_BLOCK_NETHER_BRICK_STAIRS},
		{"minecraft:nether_brick_wall",              E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:nether_bricks",                  E_BLOCK_NETHER_BRICK},
		{"minecraft:nether_portal",                  E_BLOCK_NETHER_PORTAL},
		{"minecraft:nether_quartz_ore",              E_BLOCK_NETHER_QUARTZ_ORE},
		{"minecraft:nether_wart",                    E_BLOCK_NETHER_WART},
		{"minecraft:nether_wart_block",              E_BLOCK_NETHER_WART_BLOCK},
		{"minecraft:netherrack",                     E_BLOCK_NETHERRACK},
		{"minecraft:note_block",                     E_BLOCK_NOTE_BLOCK},
		{"minecraft:oak_button",                     E_BLOCK_WOODEN_BUTTON},
		{"minecraft:oak_door",                       E_BLOCK_WOODEN_DOOR},
		{"minecraft:oak_fence",                      E_BLOCK_FENCE},
		{"minecraft:oak_fence_gate",                 E_BLOCK_FENCE_GATE},
		{"minecraft:oak_leaves",                     E_BLOCK_LEAVES},
		{"minecraft:oak_log",                        E_BLOCK_LOG},
		{"minecraft:oak_planks",                     E_BLOCK_PLANKS},
		{"minecraft:oak_pressure_plate",             E_BLOCK_WOODEN_PRESSURE_PLATE},
		{"minecraft:oak_sapling",                    E_BLOCK_SAPLING},
		{"minecraft:oak_sign",                       E_BLOCK_SIGN_POST},
		{"minecraft:oak_slab",                       E_BLOCK_WOODEN_SLAB},
		{"minecraft:oak_stairs",                     E_BLOCK_WOODEN_STAIRS},
		{"minecraft:oak_trapdoor",                   E_BLOCK_TRAPDOOR},
		{"minecraft:oak_wall_sign",                  E_BLOCK_WALLSIGN},
		{"minecraft:oak_wood",                       E_BLOCK_UNFINISHED},
		{"minecraft:observer",                       E_BLOCK_OBSERVER},
		{"minecraft:obsidian",                       E_BLOCK_OBSIDIAN},
		{"minecraft:orange_banner",                  E_BLOCK_STANDING_BANNER},
		{"minecraft:orange_bed",                     E_BLOCK_BED},
		{"minecraft:orange_carpet",                  E_BLOCK_CARPET},
		{"minecraft:orange_concrete",                E_BLOCK_CONCRETE},
		{"minecraft:orange_concrete_powder",         E_BLOCK_CONCRETE_POWDER},
		{"minecraft:orange_glazed_terracotta",       E_BLOCK_ORANGE_GLAZED_TERRACOTTA},
		{"minecraft:orange_shulker_box",             E_BLOCK_ORANGE_SHULKER_BOX},
		{"minecraft:orange_stained_glass",           E_BLOCK_STAINED_GLASS},
		{"minecraft:orange_stained_glass_pane",      E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:orange_terracotta",              E_BLOCK_TERRACOTTA},
		{"minecraft:orange_tulip",                   E_BLOCK_FLOWER},
		{"minecraft:orange_wall_banner",             E_BLOCK_WALL_BANNER},
		{"minecraft:orange_wool",                    E_BLOCK_WOOL},
		{"minecraft:oxeye_daisy",                    E_BLOCK_FLOWER},
		{"minecraft:packed_ice",                     E_BLOCK_PACKED_ICE},
		{"minecraft:peony",                          E_BLOCK_BIG_FLOWER},
		{"minecraft:petrified_oak_slab",             E_BLOCK_WOODEN_SLAB},
		{"minecraft:pink_banner",                    E_BLOCK_STANDING_BANNER},
		{"minecraft:pink_bed",                       E_BLOCK_BED},
		{"minecraft:pink_carpet",                    E_BLOCK_CARPET},
		{"minecraft:pink_concrete",                  E_BLOCK_CONCRETE},
		{"minecraft:pink_concrete_powder",           E_BLOCK_CONCRETE_POWDER},
		{"minecraft:pink_glazed_terracotta",         E_BLOCK_PINK_GLAZED_TERRACOTTA},
		{"minecraft:pink_shulker_box",               E_BLOCK_PINK_SHULKER_BOX},
		{"minecraft:pink_stained_glass",             E_BLOCK_STAINED_GLASS},
		{"minecraft:pink_stained_glass_pane",        E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:pink_terracotta",                E_BLOCK_TERRACOTTA},
		{"minecraft:pink_tulip",                     E_BLOCK_FLOWER},
		{"minecraft:pink_wall_banner",               E_BLOCK_WALL_BANNER},
		{"minecraft:pink_wool",                      E_BLOCK_WOOL},
		{"minecraft:piston",                         E_BLOCK_PISTON},
		{"minecraft:piston_head",                    E_BLOCK_PISTON_EXTENSION},
		{"minecraft:player_head",                    E_BLOCK_HEAD},
		{"minecraft:player_wall_head",               E_BLOCK_HEAD},
		{"minecraft:podzol",                         E_BLOCK_UNFINISHED},
		{"minecraft:polished_andesite",              E_BLOCK_UNFINISHED},
		{"minecraft:polished_andesite_slab",         E_BLOCK_STONE_SLAB},
		{"minecraft:polished_andesite_stairs",       E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:polished_diorite",               E_BLOCK_UNFINISHED},
		{"minecraft:polished_diorite_slab",          E_BLOCK_STONE_SLAB},
		{"minecraft:polished_diorite_stairs",        E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:polished_granite",               E_BLOCK_UNFINISHED},
		{"minecraft:polished_granite_slab",          E_BLOCK_STONE_SLAB},
		{"minecraft:polished_granite_stairs",        E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:poppy",                          E_BLOCK_FLOWER},
		{"minecraft:potatoes",                       E_BLOCK_POTATOES},
		{"minecraft:potted_acacia_sapling",          E_BLOCK_UNFINISHED},
		{"minecraft:potted_allium",                  E_BLOCK_UNFINISHED},
		{"minecraft:potted_azure_bluet",             E_BLOCK_UNFINISHED},
		{"minecraft:potted_bamboo",                  E_BLOCK_UNFINISHED},
		{"minecraft:potted_birch_sapling",           E_BLOCK_UNFINISHED},
		{"minecraft:potted_blue_orchid",             E_BLOCK_UNFINISHED},
		{"minecraft:potted_brown_mushroom",          E_BLOCK_UNFINISHED},
		{"minecraft:potted_cactus",                  E_BLOCK_UNFINISHED},
		{"minecraft:potted_cornflower",              E_BLOCK_UNFINISHED},
		{"minecraft:potted_dandelion",               E_BLOCK_UNFINISHED},
		{"minecraft:potted_dark_oak_sapling",        E_BLOCK_UNFINISHED},
		{"minecraft:potted_dead_bush",               E_BLOCK_UNFINISHED},
		{"minecraft:potted_fern",                    E_BLOCK_UNFINISHED},
		{"minecraft:potted_jungle_sapling",          E_BLOCK_UNFINISHED},
		{"minecraft:potted_lily_of_the_valley",      E_BLOCK_UNFINISHED},
		{"minecraft:potted_oak_sapling",             E_BLOCK_UNFINISHED},
		{"minecraft:potted_orange_tulip",            E_BLOCK_UNFINISHED},
		{"minecraft:potted_oxeye_daisy",             E_BLOCK_UNFINISHED},
		{"minecraft:potted_pink_tulip",              E_BLOCK_UNFINISHED},
		{"minecraft:potted_poppy",                   E_BLOCK_UNFINISHED},
		{"minecraft:potted_red_mushroom",            E_BLOCK_UNFINISHED},
		{"minecraft:potted_red_tulip",               E_BLOCK_UNFINISHED},
		{"minecraft:potted_spruce_sapling",          E_BLOCK_UNFINISHED},
		{"minecraft:potted_white_tulip",             E_BLOCK_UNFINISHED},
		{"minecraft:potted_wither_rose",             E_BLOCK_UNFINISHED},
		{"minecraft:powered_rail",                   E_BLOCK_POWERED_RAIL},
		{"minecraft:prismarine",                     E_BLOCK_PRISMARINE_BLOCK},
		{"minecraft:prismarine_brick_slab",          E_BLOCK_STONE_SLAB},
		{"minecraft:prismarine_brick_stairs",        E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:prismarine_bricks",              E_BLOCK_PRISMARINE_BLOCK},
		{"minecraft:prismarine_slab",                E_BLOCK_STONE_SLAB},
		{"minecraft:prismarine_stairs",              E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:prismarine_wall",                E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:pumpkin",                        E_BLOCK_PUMPKIN},
		{"minecraft:pumpkin_stem",                   E_BLOCK_PUMPKIN_STEM},
		{"minecraft:purple_banner",                  E_BLOCK_STANDING_BANNER},
		{"minecraft:purple_bed",                     E_BLOCK_BED},
		{"minecraft:purple_carpet",                  E_BLOCK_CARPET},
		{"minecraft:purple_concrete",                E_BLOCK_CONCRETE},
		{"minecraft:purple_concrete_powder",         E_BLOCK_CONCRETE_POWDER},
		{"minecraft:purple_glazed_terracotta",       E_BLOCK_PURPLE_GLAZED_TERRACOTTA},
		{"minecraft:purple_shulker_box",             E_BLOCK_PURPLE_SHULKER_BOX},
		{"minecraft:purple_stained_glass",           E_BLOCK_STAINED_GLASS},
		{"minecraft:purple_stained_glass_pane",      E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:purple_terracotta",              E_BLOCK_TERRACOTTA},
		{"minecraft:purple_wall_banner",             E_BLOCK_WALL_BANNER},
		{"minecraft:purple_wool",                    E_BLOCK_WOOL},
		{"minecraft:purpur_block",                   E_BLOCK_PURPUR_BLOCK},
		{"minecraft:purpur_pillar",                  E_BLOCK_PURPUR_PILLAR},
		{"minecraft:purpur_slab",                    E_BLOCK_PURPUR_SLAB},
		{"minecraft:purpur_stairs",                  E_BLOCK_PURPUR_STAIRS},
		{"minecraft:quartz_block",                   E_BLOCK_QUARTZ_BLOCK},
		{"minecraft:quartz_pillar",                  E_BLOCK_UNFINISHED},
		{"minecraft:quartz_slab",                    E_BLOCK_STONE_SLAB},
		{"minecraft:quartz_stairs",                  E_BLOCK_QUARTZ_STAIRS},
		{"minecraft:rail",                           E_BLOCK_RAIL},
		{"minecraft:red_banner",                     E_BLOCK_STANDING_BANNER},
		{"minecraft:red_bed",                        E_BLOCK_BED},
		{"minecraft:red_carpet",                     E_BLOCK_CARPET},
		{"minecraft:red_concrete",                   E_BLOCK_CONCRETE},
		{"minecraft:red_concrete_powder",            E_BLOCK_CONCRETE_POWDER},
		{"minecraft:red_glazed_terracotta",          E_BLOCK_RED_GLAZED_TERRACOTTA},
		{"minecraft:red_mushroom",                   E_BLOCK_RED_MUSHROOM},
		{"minecraft:red_mushroom_block",             E_BLOCK_HUGE_RED_MUSHROOM},
		{"minecraft:red_nether_brick_slab",          E_BLOCK_STONE_SLAB},
		{"minecraft:red_nether_brick_stairs",        E_BLOCK_STONE_BRICK_STAIRS},
		{"minecraft:red_nether_brick_wall",          E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:red_nether_bricks",              E_BLOCK_RED_NETHER_BRICK},
		{"minecraft:red_sand",                       E_BLOCK_UNFINISHED},
		{"minecraft:red_sandstone",                  E_BLOCK_RED_SANDSTONE},
		{"minecraft:red_sandstone_slab",             E_BLOCK_STONE_SLAB},
		{"minecraft:red_sandstone_stairs",           E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:red_sandstone_wall",             E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:red_shulker_box",                E_BLOCK_RED_SHULKER_BOX},
		{"minecraft:red_stained_glass",              E_BLOCK_STAINED_GLASS},
		{"minecraft:red_stained_glass_pane",         E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:red_terracotta",                 E_BLOCK_TERRACOTTA},
		{"minecraft:red_tulip",                      E_BLOCK_FLOWER},
		{"minecraft:red_wall_banner",                E_BLOCK_WALL_BANNER},
		{"minecraft:red_wool",                       E_BLOCK_WOOL},
		{"minecraft:redstone_block",                 E_BLOCK_BLOCK_OF_REDSTONE},
		{"minecraft:redstone_lamp",                  E_BLOCK_UNFINISHED},  // We only have handlers for either ON or OFF variant
		{"minecraft:redstone_ore",                   E_BLOCK_REDSTONE_ORE},
		{"minecraft:redstone_torch",                 E_BLOCK_UNFINISHED},  // We only have handlers for either ON or OFF variant
		{"minecraft:redstone_wall_torch",            E_BLOCK_UNFINISHED},
		{"minecraft:redstone_wire",                  E_BLOCK_REDSTONE_WIRE},
		{"minecraft:repeater",                       E_BLOCK_UNFINISHED},
		{"minecraft:repeating_command_block",        E_BLOCK_REPEATING_COMMAND_BLOCK},
		{"minecraft:rose_bush",                      E_BLOCK_BIG_FLOWER},
		{"minecraft:sand",                           E_BLOCK_SAND},
		{"minecraft:sandstone",                      E_BLOCK_SANDSTONE},
		{"minecraft:sandstone_slab",                 E_BLOCK_STONE_SLAB},
		{"minecraft:sandstone_stairs",               E_BLOCK_SANDSTONE_STAIRS},
		{"minecraft:sandstone_wall",                 E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:scaffolding",                    E_BLOCK_UNFINISHED},
		{"minecraft:sea_lantern",                    E_BLOCK_SEA_LANTERN},
		{"minecraft:sea_pickle",                     E_BLOCK_UNFINISHED},
		{"minecraft:seagrass",                       E_BLOCK_UNFINISHED},
		{"minecraft:shulker_box",                    E_BLOCK_PURPLE_SHULKER_BOX},  // Color is irrelevant in the handler
		{"minecraft:skeleton_skull",                 E_BLOCK_HEAD},
		{"minecraft:skeleton_wall_skull",            E_BLOCK_HEAD},
		{"minecraft:slime_block",                    E_BLOCK_SLIME_BLOCK},
		{"minecraft:smithing_table",                 E_BLOCK_UNFINISHED},
		{"minecraft:smoker",                         E_BLOCK_UNFINISHED},
		{"minecraft:smooth_quartz",                  E_BLOCK_QUARTZ_BLOCK},
		{"minecraft:smooth_quartz_slab",             E_BLOCK_STONE_SLAB},
		{"minecraft:smooth_quartz_stairs",           E_BLOCK_QUARTZ_STAIRS},
		{"minecraft:smooth_red_sandstone",           E_BLOCK_SANDSTONE},
		{"minecraft:smooth_red_sandstone_slab",      E_BLOCK_STONE_SLAB},
		{"minecraft:smooth_red_sandstone_stairs",    E_BLOCK_SANDSTONE_STAIRS},
		{"minecraft:smooth_sandstone",               E_BLOCK_SANDSTONE},
		{"minecraft:smooth_sandstone_slab",          E_BLOCK_STONE_SLAB},
		{"minecraft:smooth_sandstone_stairs",        E_BLOCK_SANDSTONE_STAIRS},
		{"minecraft:smooth_stone",                   E_BLOCK_SANDSTONE},
		{"minecraft:smooth_stone_slab",              E_BLOCK_STONE_SLAB},
		{"minecraft:snow",                           E_BLOCK_SNOW},
		{"minecraft:snow_block",                     E_BLOCK_SNOW_BLOCK},
		{"minecraft:soul_sand",                      E_BLOCK_SOULSAND},
		{"minecraft:spawner",                        E_BLOCK_MOB_SPAWNER},
		{"minecraft:sponge",                         E_BLOCK_SPONGE},
		{"minecraft:spruce_button",                  E_BLOCK_WOODEN_BUTTON},
		{"minecraft:spruce_door",                    E_BLOCK_WOODEN_DOOR},
		{"minecraft:spruce_fence",                   E_BLOCK_FENCE},
		{"minecraft:spruce_fence_gate",              E_BLOCK_FENCE_GATE},
		{"minecraft:spruce_leaves",                  E_BLOCK_LEAVES},
		{"minecraft:spruce_log",                     E_BLOCK_LOG},
		{"minecraft:spruce_planks",                  E_BLOCK_PLANKS},
		{"minecraft:spruce_pressure_plate",          E_BLOCK_WOODEN_PRESSURE_PLATE},
		{"minecraft:spruce_sapling",                 E_BLOCK_SAPLING},
		{"minecraft:spruce_sign",                    E_BLOCK_SIGN_POST},
		{"minecraft:spruce_slab",                    E_BLOCK_WOODEN_SLAB},
		{"minecraft:spruce_stairs",                  E_BLOCK_WOODEN_STAIRS},
		{"minecraft:spruce_trapdoor",                E_BLOCK_TRAPDOOR},
		{"minecraft:spruce_wall_sign",               E_BLOCK_WALLSIGN},
		{"minecraft:spruce_wood",                    E_BLOCK_UNFINISHED},
		{"minecraft:sticky_piston",                  E_BLOCK_STICKY_PISTON},
		{"minecraft:stone",                          E_BLOCK_STONE},
		{"minecraft:stone_brick_slab",               E_BLOCK_STONE_SLAB},
		{"minecraft:stone_brick_stairs",             E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:stone_brick_wall",               E_BLOCK_COBBLESTONE_WALL},
		{"minecraft:stone_bricks",                   E_BLOCK_STONE_BRICKS},
		{"minecraft:stone_button",                   E_BLOCK_STONE_BUTTON},
		{"minecraft:stone_pressure_plate",           E_BLOCK_STONE_PRESSURE_PLATE},
		{"minecraft:stone_slab",                     E_BLOCK_STONE_SLAB},
		{"minecraft:stone_stairs",                   E_BLOCK_COBBLESTONE_STAIRS},
		{"minecraft:stonecutter",                    E_BLOCK_UNFINISHED},
		{"minecraft:stripped_acacia_log",            E_BLOCK_LOG},
		{"minecraft:stripped_acacia_wood",           E_BLOCK_UNFINISHED},
		{"minecraft:stripped_birch_log",             E_BLOCK_LOG},
		{"minecraft:stripped_birch_wood",            E_BLOCK_UNFINISHED},
		{"minecraft:stripped_dark_oak_log",          E_BLOCK_LOG},
		{"minecraft:stripped_dark_oak_wood",         E_BLOCK_UNFINISHED},
		{"minecraft:stripped_jungle_log",            E_BLOCK_LOG},
		{"minecraft:stripped_jungle_wood",           E_BLOCK_UNFINISHED},
		{"minecraft:stripped_oak_log",               E_BLOCK_LOG},
		{"minecraft:stripped_oak_wood",              E_BLOCK_UNFINISHED},
		{"minecraft:stripped_spruce_log",            E_BLOCK_LOG},
		{"minecraft:stripped_spruce_wood",           E_BLOCK_UNFINISHED},
		{"minecraft:structure_block",                E_BLOCK_STRUCTURE_BLOCK},
		{"minecraft:structure_void",                 E_BLOCK_STRUCTURE_VOID},
		{"minecraft:sugar_cane",                     E_BLOCK_SUGARCANE},
		{"minecraft:sunflower",                      E_BLOCK_BIG_FLOWER},
		{"minecraft:sweet_berry_bush",               E_BLOCK_FLOWER},
		{"minecraft:tall_grass",                     E_BLOCK_TALL_GRASS},
		{"minecraft:tall_seagrass",                  E_BLOCK_UNFINISHED},
		{"minecraft:terracotta",                     E_BLOCK_TERRACOTTA},
		{"minecraft:tnt",                            E_BLOCK_TNT},
		{"minecraft:torch",                          E_BLOCK_TORCH},
		{"minecraft:trapped_chest",                  E_BLOCK_TRAPPED_CHEST},
		{"minecraft:tripwire",                       E_BLOCK_TRIPWIRE},
		{"minecraft:tripwire_hook",                  E_BLOCK_TRIPWIRE_HOOK},
		{"minecraft:tube_coral",                     E_BLOCK_UNFINISHED},
		{"minecraft:tube_coral_block",               E_BLOCK_UNFINISHED},
		{"minecraft:tube_coral_fan",                 E_BLOCK_UNFINISHED},
		{"minecraft:tube_coral_wall_fan",            E_BLOCK_UNFINISHED},
		{"minecraft:turtle_egg",                     E_BLOCK_UNFINISHED},
		{"minecraft:vine",                           E_BLOCK_VINES},
		{"minecraft:void_air",                       E_BLOCK_AIR},
		{"minecraft:wall_sign",                      E_BLOCK_WALLSIGN},
		{"minecraft:wall_torch",                     E_BLOCK_TORCH},
		{"minecraft:water",                          E_BLOCK_WATER},
		{"minecraft:wet_sponge",                     E_BLOCK_SPONGE},
		{"minecraft:wheat",                          E_BLOCK_CROPS},
		{"minecraft:white_banner",                   E_BLOCK_STANDING_BANNER},
		{"minecraft:white_bed",                      E_BLOCK_BED},
		{"minecraft:white_carpet",                   E_BLOCK_CARPET},
		{"minecraft:white_concrete",                 E_BLOCK_CONCRETE},
		{"minecraft:white_concrete_powder",          E_BLOCK_CONCRETE_POWDER},
		{"minecraft:white_glazed_terracotta",        E_BLOCK_WHITE_GLAZED_TERRACOTTA},
		{"minecraft:white_shulker_box",              E_BLOCK_WHITE_SHULKER_BOX},
		{"minecraft:white_stained_glass",            E_BLOCK_STAINED_GLASS},
		{"minecraft:white_stained_glass_pane",       E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:white_terracotta",               E_BLOCK_TERRACOTTA},
		{"minecraft:white_tulip",                    E_BLOCK_FLOWER},
		{"minecraft:white_wall_banner",              E_BLOCK_WALL_BANNER},
		{"minecraft:white_wool",                     E_BLOCK_WOOL},
		{"minecraft:wither_rose",                    E_BLOCK_FLOWER},
		{"minecraft:wither_skeleton_skull",          E_BLOCK_HEAD},
		{"minecraft:wither_skeleton_wall_skull",     E_BLOCK_HEAD},
		{"minecraft:yellow_banner",                  E_BLOCK_STANDING_BANNER},
		{"minecraft:yellow_bed",                     E_BLOCK_BED},
		{"minecraft:yellow_carpet",                  E_BLOCK_CARPET},
		{"minecraft:yellow_concrete",                E_BLOCK_CONCRETE},
		{"minecraft:yellow_concrete_powder",         E_BLOCK_CONCRETE_POWDER},
		{"minecraft:yellow_glazed_terracotta",       E_BLOCK_YELLOW_GLAZED_TERRACOTTA},
		{"minecraft:yellow_shulker_box",             E_BLOCK_YELLOW_SHULKER_BOX},
		{"minecraft:yellow_stained_glass",           E_BLOCK_STAINED_GLASS},
		{"minecraft:yellow_stained_glass_pane",      E_BLOCK_STAINED_GLASS_PANE},
		{"minecraft:yellow_terracotta",              E_BLOCK_TERRACOTTA},
		{"minecraft:yellow_wall_banner",             E_BLOCK_WALL_BANNER},
		{"minecraft:yellow_wool",                    E_BLOCK_WOOL},
		{"minecraft:zombie_head",                    E_BLOCK_HEAD},
		{"minecraft:zombie_wall_head",               E_BLOCK_HEAD},
	};

	for (const auto & r: regs)
	{
		// std::shared_ptr<cBlockHandler> handler(::CreateBlockHandler(r.oldBlockType));
		// aRegistry.registerBlockType("<built-in>", r.newBlockTypeName, handler);
	}
}
