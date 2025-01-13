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
#include "BlockWool.h"
#include "BlockCobWeb.h"
#include "BlockCocoaPod.h"
#include "BlockCommandBlock.h"
#include "BlockComparator.h"
#include "BlockConcretePowder.h"
#include "BlockCrops.h"
#include "BlockDaylightSensor.h"
#include "BlockDeadBush.h"
#include "BlockDefaultBlock.h"
#include "BlockDirt.h"
#include "BlockDoor.h"
#include "BlockDropSpenser.h"
#include "BlockEnchantingTable.h"
#include "BlockEndPortalFrame.h"
#include "BlockEnderChest.h"
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
#include "BlockHayBale.h"
#include "BlockHugeMushroom.h"
#include "BlockIce.h"
#include "BlockJukebox.h"
#include "BlockLadder.h"
#include "BlockLeaves.h"
#include "BlockLever.h"
#include "BlockLilypad.h"
#include "BlockLog.h"
#include "BlockMelon.h"
#include "BlockMobHead.h"
#include "BlockMobSpawner.h"
#include "BlockInfested.h"
#include "BlockMushroom.h"
#include "BlockMycelium.h"
#include "BlockNetherWart.h"
#include "BlockNetherrack.h"
#include "BlockNoteBlock.h"
#include "BlockObserver.h"
#include "BlockOre.h"
#include "BlockPackedIce.h"
#include "BlockPiston.h"
#include "BlockPlanks.h"
#include "BlockPodzol.h"
#include "BlockPortal.h"
#include "BlockPressurePlate.h"
#include "BlockPumpkin.h"
#include "BlockQuartz.h"
#include "BlockRail.h"
#include "BlockRedstoneWire.h"
#include "BlockRedstoneLamp.h"
#include "BlockRedstoneOre.h"
#include "BlockRedstoneRepeater.h"
#include "BlockSand.h"
#include "BlockSapling.h"
#include "BlockSeaLantern.h"
#include "BlockSignPost.h"
#include "BlockSlab.h"
#include "BlockSlime.h"
#include "BlockSnow.h"
#include "BlockSponge.h"
#include "BlockStairs.h"
#include "BlockStandingBanner.h"
#include "BlockStems.h"
#include "BlockStone.h"
#include "BlockSugarCane.h"
#include "BlockTNT.h"
#include "BlockTallGrass.h"
#include "BlockTorch.h"
#include "BlockTrapdoor.h"
#include "BlockTripwire.h"
#include "BlockTripwireHook.h"
#include "BlockVines.h"
#include "BlockWallBanner.h"
#include "BlockWallSign.h"
#include "BlockCraftingTable.h"





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
	constexpr cBlockButtonHandler                          BlockAcaciaButtonHandler(BlockType::AcaciaButton);
	constexpr cBlockDoorHandler                            BlockAcaciaDoorHandler(BlockType::AcaciaDoor);
	constexpr cBlockFenceHandler                           BlockAcaciaFenceHandler(BlockType::AcaciaFence);
	constexpr cBlockFenceGateHandler                       BlockAcaciaFenceGateHandler(BlockType::AcaciaFenceGate);
	constexpr cBlockLeavesHandler                          BlockAcaciaLeavesHandler(BlockType::AcaciaLeaves);
	constexpr cBlockLogHandler                             BlockAcaciaLogHandler(BlockType::AcaciaLog);
	constexpr cBlockPlanksHandler                          BlockAcaciaPlanksHandler(BlockType::AcaciaPlanks);
	constexpr cBlockPressurePlateHandler                   BlockAcaciaPressurePlateHandler(BlockType::AcaciaPressurePlate);
	constexpr cBlockSaplingHandler                         BlockAcaciaSaplingHandler(BlockType::AcaciaSapling);
	constexpr cBlockSignPostHandler                        BlockAcaciaSignHandler(BlockType::AcaciaSign);
	constexpr cBlockSlabHandler                            BlockAcaciaSlabHandler(BlockType::AcaciaSlab);
	constexpr cBlockStairsHandler                          BlockAcaciaStairsHandler(BlockType::AcaciaStairs);
	constexpr cBlockTrapdoorHandler                        BlockAcaciaTrapdoorHandler(BlockType::AcaciaTrapdoor);
	constexpr cBlockWallSignHandler                        BlockAcaciaWallSignHandler(BlockType::AcaciaWallSign);
	constexpr cDefaultBlockHandler                         BlockAcaciaWoodHandler(BlockType::AcaciaWood);
	constexpr cBlockRailHandler                            BlockActivatorRailHandler(BlockType::ActivatorRail);
	constexpr cBlockAirHandler                             BlockAirHandler(BlockType::Air);
	constexpr cBlockFlowerHandler                          BlockAlliumHandler(BlockType::Allium);
	constexpr cDefaultBlockHandler                         BlockAncientDebrisHandler(BlockType::AncientDebris);
	constexpr cDefaultBlockHandler                         BlockAndesiteHandler(BlockType::Andesite);
	constexpr cBlockSlabHandler                            BlockAndesiteSlabHandler(BlockType::AndesiteSlab);
	constexpr cBlockStairsHandler                          BlockAndesiteStairsHandler(BlockType::AndesiteStairs);
	constexpr cDefaultBlockHandler                         BlockAndesiteWallHandler(BlockType::AndesiteWall);
	constexpr cBlockAnvilHandler                           BlockAnvilHandler(BlockType::Anvil);
	constexpr cBlockMelonStemHandler                       BlockAttachedMelonStemHandler(BlockType::AttachedMelonStem);
	constexpr cBlockPumpkinStemHandler                     BlockAttachedPumpkinStemHandler(BlockType::AttachedPumpkinStem);
	constexpr cBlockFlowerHandler                          BlockAzureBluetHandler(BlockType::AzureBluet);
	constexpr cDefaultBlockHandler                         BlockBambooHandler(BlockType::Bamboo);
	constexpr cDefaultBlockHandler                         BlockBambooSaplingHandler(BlockType::BambooSapling);
	constexpr cDefaultBlockHandler                         BlockBarrelHandler(BlockType::Barrel);
	constexpr cDefaultBlockHandler                         BlockBarrierHandler(BlockType::Barrier);
	constexpr cDefaultBlockHandler                         BlockBasaltHandler(BlockType::Basalt);
	constexpr cDefaultBlockEntityHandler                   BlockBeaconHandler(BlockType::Beacon);
	constexpr cDefaultBlockHandler                         BlockBedrockHandler(BlockType::Bedrock);
	constexpr cDefaultBlockHandler                         BlockBeeNestHandler(BlockType::BeeNest);
	constexpr cDefaultBlockHandler                         BlockBeehiveHandler(BlockType::Beehive);
	constexpr cBlockCropsHandler                           BlockBeetrootsHandler(BlockType::Beetroots);
	constexpr cDefaultBlockHandler                         BlockBellHandler(BlockType::Bell);
	constexpr cBlockButtonHandler                          BlockBirchButtonHandler(BlockType::BirchButton);
	constexpr cBlockDoorHandler                            BlockBirchDoorHandler(BlockType::BirchDoor);
	constexpr cBlockFenceHandler                           BlockBirchFenceHandler(BlockType::BirchFence);
	constexpr cBlockFenceGateHandler                       BlockBirchFenceGateHandler(BlockType::BirchFenceGate);
	constexpr cBlockLeavesHandler                          BlockBirchLeavesHandler(BlockType::BirchLeaves);
	constexpr cBlockLogHandler                             BlockBirchLogHandler(BlockType::BirchLog);
	constexpr cBlockPlanksHandler                          BlockBirchPlanksHandler(BlockType::BirchPlanks);
	constexpr cBlockPressurePlateHandler                   BlockBirchPressurePlateHandler(BlockType::BirchPressurePlate);
	constexpr cBlockSaplingHandler                         BlockBirchSaplingHandler(BlockType::BirchSapling);
	constexpr cBlockSignPostHandler                        BlockBirchSignHandler(BlockType::BirchSign);
	constexpr cBlockSlabHandler                            BlockBirchSlabHandler(BlockType::BirchSlab);
	constexpr cBlockStairsHandler                          BlockBirchStairsHandler(BlockType::BirchStairs);
	constexpr cBlockTrapdoorHandler                        BlockBirchTrapdoorHandler(BlockType::BirchTrapdoor);
	constexpr cBlockWallSignHandler                        BlockBirchWallSignHandler(BlockType::BirchWallSign);
	constexpr cDefaultBlockHandler                         BlockBirchWoodHandler(BlockType::BirchWood);
	constexpr cDefaultBlockHandler                         BlockBlackBannerHandler(BlockType::BlackBanner);
	constexpr cBlockBedHandler                             BlockBlackBedHandler(BlockType::BlackBed);
	constexpr cBlockCarpetHandler                          BlockBlackCarpetHandler(BlockType::BlackCarpet);
	constexpr cDefaultBlockHandler                         BlockBlackConcreteHandler(BlockType::BlackConcrete);
	constexpr cBlockConcretePowderHandler                  BlockBlackConcretePowderHandler(BlockType::BlackConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockBlackGlazedTerracottaHandler(BlockType::BlackGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockBlackShulkerBoxHandler(BlockType::BlackShulkerBox);
	constexpr cBlockGlassHandler                           BlockBlackStainedGlassHandler(BlockType::BlackStainedGlass);
	constexpr cBlockGlassHandler                           BlockBlackStainedGlassPaneHandler(BlockType::BlackStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockBlackTerracottaHandler(BlockType::BlackTerracotta);
	constexpr cDefaultBlockHandler                         BlockBlackWallBannerHandler(BlockType::BlackWallBanner);
	constexpr cBlockWoolHandler                            BlockBlackWoolHandler(BlockType::BlackWool);
	constexpr cDefaultBlockHandler                         BlockBlackstoneHandler(BlockType::Blackstone);
	constexpr cBlockSlabHandler                            BlockBlackstoneSlabHandler(BlockType::BlackstoneSlab);
	constexpr cBlockStairsHandler                          BlockBlackstoneStairsHandler(BlockType::BlackstoneStairs);
	constexpr cDefaultBlockHandler                         BlockBlackstoneWallHandler(BlockType::BlackstoneWall);
	constexpr cDefaultBlockHandler                         BlockBlastFurnaceHandler(BlockType::BlastFurnace);
	constexpr cDefaultBlockHandler                         BlockBlueBannerHandler(BlockType::BlueBanner);
	constexpr cBlockBedHandler                             BlockBlueBedHandler(BlockType::BlueBed);
	constexpr cBlockCarpetHandler                          BlockBlueCarpetHandler(BlockType::BlueCarpet);
	constexpr cDefaultBlockHandler                         BlockBlueConcreteHandler(BlockType::BlueConcrete);
	constexpr cBlockConcretePowderHandler                  BlockBlueConcretePowderHandler(BlockType::BlueConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockBlueGlazedTerracottaHandler(BlockType::BlueGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockBlueIceHandler(BlockType::BlueIce);
	constexpr cBlockFlowerHandler                          BlockBlueOrchidHandler(BlockType::BlueOrchid);
	constexpr cDefaultBlockHandler                         BlockBlueShulkerBoxHandler(BlockType::BlueShulkerBox);
	constexpr cBlockGlassHandler                           BlockBlueStainedGlassHandler(BlockType::BlueStainedGlass);
	constexpr cBlockGlassHandler                           BlockBlueStainedGlassPaneHandler(BlockType::BlueStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockBlueTerracottaHandler(BlockType::BlueTerracotta);
	constexpr cDefaultBlockHandler                         BlockBlueWallBannerHandler(BlockType::BlueWallBanner);
	constexpr cBlockWoolHandler                            BlockBlueWoolHandler(BlockType::BlueWool);
	constexpr cDefaultBlockHandler                         BlockBoneBlockHandler(BlockType::BoneBlock);
	constexpr cBlockBookShelfHandler                       BlockBookshelfHandler(BlockType::Bookshelf);
	constexpr cDefaultBlockHandler                         BlockBrainCoralHandler(BlockType::BrainCoral);
	constexpr cDefaultBlockHandler                         BlockBrainCoralBlockHandler(BlockType::BrainCoralBlock);
	constexpr cDefaultBlockHandler                         BlockBrainCoralFanHandler(BlockType::BrainCoralFan);
	constexpr cDefaultBlockHandler                         BlockBrainCoralWallFanHandler(BlockType::BrainCoralWallFan);
	constexpr cBlockBrewingStandHandler                    BlockBrewingStandHandler(BlockType::BrewingStand);
	constexpr cBlockSlabHandler                            BlockBrickSlabHandler(BlockType::BrickSlab);
	constexpr cBlockStairsHandler                          BlockBrickStairsHandler(BlockType::BrickStairs);
	constexpr cDefaultBlockHandler                         BlockBrickWallHandler(BlockType::BrickWall);
	constexpr cDefaultBlockHandler                         BlockBricksHandler(BlockType::Bricks);
	constexpr cDefaultBlockHandler                         BlockBrownBannerHandler(BlockType::BrownBanner);
	constexpr cBlockBedHandler                             BlockBrownBedHandler(BlockType::BrownBed);
	constexpr cBlockCarpetHandler                          BlockBrownCarpetHandler(BlockType::BrownCarpet);
	constexpr cDefaultBlockHandler                         BlockBrownConcreteHandler(BlockType::BrownConcrete);
	constexpr cBlockConcretePowderHandler                  BlockBrownConcretePowderHandler(BlockType::BrownConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockBrownGlazedTerracottaHandler(BlockType::BrownGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockBrownMushroomHandler(BlockType::BrownMushroom);
	constexpr cBlockHugeMushroomHandler                    BlockBrownMushroomBlockHandler(BlockType::BrownMushroomBlock);
	constexpr cDefaultBlockHandler                         BlockBrownShulkerBoxHandler(BlockType::BrownShulkerBox);
	constexpr cBlockGlassHandler                           BlockBrownStainedGlassHandler(BlockType::BrownStainedGlass);
	constexpr cBlockGlassHandler                           BlockBrownStainedGlassPaneHandler(BlockType::BrownStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockBrownTerracottaHandler(BlockType::BrownTerracotta);
	constexpr cDefaultBlockHandler                         BlockBrownWallBannerHandler(BlockType::BrownWallBanner);
	constexpr cBlockWoolHandler                            BlockBrownWoolHandler(BlockType::BrownWool);
	constexpr cDefaultBlockHandler                         BlockBubbleColumnHandler(BlockType::BubbleColumn);
	constexpr cDefaultBlockHandler                         BlockBubbleCoralHandler(BlockType::BubbleCoral);
	constexpr cDefaultBlockHandler                         BlockBubbleCoralBlockHandler(BlockType::BubbleCoralBlock);
	constexpr cDefaultBlockHandler                         BlockBubbleCoralFanHandler(BlockType::BubbleCoralFan);
	constexpr cDefaultBlockHandler                         BlockBubbleCoralWallFanHandler(BlockType::BubbleCoralWallFan);
	constexpr cBlockCactusHandler                          BlockCactusHandler(BlockType::Cactus);
	constexpr cBlockCakeHandler                            BlockCakeHandler(BlockType::Cake);
	constexpr cDefaultBlockHandler                         BlockCampfireHandler(BlockType::Campfire);
	constexpr cBlockCropsHandler                           BlockCarrotsHandler(BlockType::Carrots);
	constexpr cDefaultBlockHandler                         BlockCartographyTableHandler(BlockType::CartographyTable);
	constexpr cBlockPumpkinHandler                         BlockCarvedPumpkinHandler(BlockType::CarvedPumpkin);
	constexpr cBlockCauldronHandler                        BlockCauldronHandler(BlockType::Cauldron);
	constexpr cBlockAirHandler                             BlockCaveAirHandler(BlockType::CaveAir);
	constexpr cDefaultBlockHandler                         BlockChainHandler(BlockType::Chain);
	constexpr cBlockCommandBlockHandler                    BlockChainCommandBlockHandler(BlockType::ChainCommandBlock);
	constexpr cBlockChestHandler                           BlockChestHandler(BlockType::Chest);
	constexpr cBlockAnvilHandler                           BlockChippedAnvilHandler(BlockType::ChippedAnvil);
	constexpr cDefaultBlockHandler                         BlockChiseledNetherBricksHandler(BlockType::ChiseledNetherBricks);
	constexpr cDefaultBlockHandler                         BlockChiseledPolishedBlackstoneHandler(BlockType::ChiseledPolishedBlackstone);
	constexpr cDefaultBlockHandler                         BlockChiseledQuartzBlockHandler(BlockType::ChiseledQuartzBlock);
	constexpr cDefaultBlockHandler                         BlockChiseledRedSandstoneHandler(BlockType::ChiseledRedSandstone);
	constexpr cDefaultBlockHandler                         BlockChiseledSandstoneHandler(BlockType::ChiseledSandstone);
	constexpr cDefaultBlockHandler                         BlockChiseledStoneBricksHandler(BlockType::ChiseledStoneBricks);
	constexpr cDefaultBlockHandler                         BlockChorusFlowerHandler(BlockType::ChorusFlower);
	constexpr cDefaultBlockHandler                         BlockChorusPlantHandler(BlockType::ChorusPlant);
	constexpr cDefaultOreHandler                           BlockClayHandler(BlockType::Clay);
	constexpr cDefaultBlockHandler                         BlockCoalBlockHandler(BlockType::CoalBlock);
	constexpr cDefaultOreHandler                           BlockCoalOreHandler(BlockType::CoalOre);
	constexpr cBlockDirtHandler                            BlockCoarseDirtHandler(BlockType::CoarseDirt);
	constexpr cBlockStoneHandler                           BlockCobblestoneHandler(BlockType::Cobblestone);
	constexpr cBlockSlabHandler                            BlockCobblestoneSlabHandler(BlockType::CobblestoneSlab);
	constexpr cBlockStairsHandler                          BlockCobblestoneStairsHandler(BlockType::CobblestoneStairs);
	constexpr cDefaultBlockHandler                         BlockCobblestoneWallHandler(BlockType::CobblestoneWall);
	constexpr cBlockCobWebHandler                          BlockCobwebHandler(BlockType::Cobweb);
	constexpr cBlockCocoaPodHandler                        BlockCocoaHandler(BlockType::Cocoa);
	constexpr cBlockCommandBlockHandler                    BlockCommandBlockHandler(BlockType::CommandBlock);
	constexpr cBlockComparatorHandler                      BlockComparatorHandler(BlockType::Comparator);
	constexpr cDefaultBlockHandler                         BlockComposterHandler(BlockType::Composter);
	constexpr cDefaultBlockHandler                         BlockConduitHandler(BlockType::Conduit);
	constexpr cBlockFlowerHandler                          BlockCornflowerHandler(BlockType::Cornflower);
	constexpr cDefaultBlockHandler                         BlockCrackedNetherBricksHandler(BlockType::CrackedNetherBricks);
	constexpr cDefaultBlockHandler                         BlockCrackedPolishedBlackstoneBricksHandler(BlockType::CrackedPolishedBlackstoneBricks);
	constexpr cDefaultBlockHandler                         BlockCrackedStoneBricksHandler(BlockType::CrackedStoneBricks);
	constexpr cBlockCraftingTableHandler                   BlockCraftingTableHandler(BlockType::CraftingTable);
	constexpr cBlockMobHeadHandler                         BlockCreeperHeadHandler(BlockType::CreeperHead);
	constexpr cBlockMobHeadHandler                         BlockCreeperWallHeadHandler(BlockType::CreeperWallHead);
	constexpr cBlockButtonHandler                          BlockCrimsonButtonHandler(BlockType::CrimsonButton);
	constexpr cBlockDoorHandler                            BlockCrimsonDoorHandler(BlockType::CrimsonDoor);
	constexpr cBlockFenceHandler                           BlockCrimsonFenceHandler(BlockType::CrimsonFence);
	constexpr cBlockFenceGateHandler                       BlockCrimsonFenceGateHandler(BlockType::CrimsonFenceGate);
	constexpr cDefaultBlockHandler                         BlockCrimsonFungusHandler(BlockType::CrimsonFungus);
	constexpr cDefaultBlockHandler                         BlockCrimsonHyphaeHandler(BlockType::CrimsonHyphae);
	constexpr cDefaultBlockHandler                         BlockCrimsonNyliumHandler(BlockType::CrimsonNylium);
	constexpr cDefaultBlockHandler                         BlockCrimsonPlanksHandler(BlockType::CrimsonPlanks);
	constexpr cBlockPressurePlateHandler                   BlockCrimsonPressurePlateHandler(BlockType::CrimsonPressurePlate);
	constexpr cDefaultBlockHandler                         BlockCrimsonRootsHandler(BlockType::CrimsonRoots);
	constexpr cBlockSignPostHandler                        BlockCrimsonSignHandler(BlockType::CrimsonSign);
	constexpr cBlockSlabHandler                            BlockCrimsonSlabHandler(BlockType::CrimsonSlab);
	constexpr cBlockStairsHandler                          BlockCrimsonStairsHandler(BlockType::CrimsonStairs);
	constexpr cDefaultBlockHandler                         BlockCrimsonStemHandler(BlockType::CrimsonStem);
	constexpr cBlockTrapdoorHandler                        BlockCrimsonTrapdoorHandler(BlockType::CrimsonTrapdoor);
	constexpr cBlockWallSignHandler                        BlockCrimsonWallSignHandler(BlockType::CrimsonWallSign);
	constexpr cDefaultBlockHandler                         BlockCryingObsidianHandler(BlockType::CryingObsidian);
	constexpr cDefaultBlockHandler                         BlockCutRedSandstoneHandler(BlockType::CutRedSandstone);
	constexpr cBlockSlabHandler                            BlockCutRedSandstoneSlabHandler(BlockType::CutRedSandstoneSlab);
	constexpr cDefaultBlockHandler                         BlockCutSandstoneHandler(BlockType::CutSandstone);
	constexpr cBlockSlabHandler                            BlockCutSandstoneSlabHandler(BlockType::CutSandstoneSlab);
	constexpr cDefaultBlockHandler                         BlockCyanBannerHandler(BlockType::CyanBanner);
	constexpr cBlockBedHandler                             BlockCyanBedHandler(BlockType::CyanBed);
	constexpr cBlockCarpetHandler                          BlockCyanCarpetHandler(BlockType::CyanCarpet);
	constexpr cDefaultBlockHandler                         BlockCyanConcreteHandler(BlockType::CyanConcrete);
	constexpr cBlockConcretePowderHandler                  BlockCyanConcretePowderHandler(BlockType::CyanConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockCyanGlazedTerracottaHandler(BlockType::CyanGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockCyanShulkerBoxHandler(BlockType::CyanShulkerBox);
	constexpr cBlockGlassHandler                           BlockCyanStainedGlassHandler(BlockType::CyanStainedGlass);
	constexpr cBlockGlassHandler                           BlockCyanStainedGlassPaneHandler(BlockType::CyanStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockCyanTerracottaHandler(BlockType::CyanTerracotta);
	constexpr cDefaultBlockHandler                         BlockCyanWallBannerHandler(BlockType::CyanWallBanner);
	constexpr cBlockWoolHandler                            BlockCyanWoolHandler(BlockType::CyanWool);
	constexpr cBlockAnvilHandler                           BlockDamagedAnvilHandler(BlockType::DamagedAnvil);
	constexpr cBlockFlowerHandler                          BlockDandelionHandler(BlockType::Dandelion);
	constexpr cBlockButtonHandler                          BlockDarkOakButtonHandler(BlockType::DarkOakButton);
	constexpr cBlockDoorHandler                            BlockDarkOakDoorHandler(BlockType::DarkOakDoor);
	constexpr cBlockFenceHandler                           BlockDarkOakFenceHandler(BlockType::DarkOakFence);
	constexpr cBlockFenceGateHandler                       BlockDarkOakFenceGateHandler(BlockType::DarkOakFenceGate);
	constexpr cBlockLeavesHandler                          BlockDarkOakLeavesHandler(BlockType::DarkOakLeaves);
	constexpr cBlockLogHandler                             BlockDarkOakLogHandler(BlockType::DarkOakLog);
	constexpr cBlockPlanksHandler                          BlockDarkOakPlanksHandler(BlockType::DarkOakPlanks);
	constexpr cBlockPressurePlateHandler                   BlockDarkOakPressurePlateHandler(BlockType::DarkOakPressurePlate);
	constexpr cBlockSaplingHandler                         BlockDarkOakSaplingHandler(BlockType::DarkOakSapling);
	constexpr cBlockSignPostHandler                        BlockDarkOakSignHandler(BlockType::DarkOakSign);
	constexpr cBlockSlabHandler                            BlockDarkOakSlabHandler(BlockType::DarkOakSlab);
	constexpr cBlockStairsHandler                          BlockDarkOakStairsHandler(BlockType::DarkOakStairs);
	constexpr cBlockTrapdoorHandler                        BlockDarkOakTrapdoorHandler(BlockType::DarkOakTrapdoor);
	constexpr cBlockWallSignHandler                        BlockDarkOakWallSignHandler(BlockType::DarkOakWallSign);
	constexpr cDefaultBlockHandler                         BlockDarkOakWoodHandler(BlockType::DarkOakWood);
	constexpr cDefaultBlockHandler                         BlockDarkPrismarineHandler(BlockType::DarkPrismarine);
	constexpr cBlockSlabHandler                            BlockDarkPrismarineSlabHandler(BlockType::DarkPrismarineSlab);
	constexpr cBlockStairsHandler                          BlockDarkPrismarineStairsHandler(BlockType::DarkPrismarineStairs);
	constexpr cBlockDaylightSensorHandler                  BlockDaylightDetectorHandler(BlockType::DaylightDetector);
	constexpr cDefaultBlockHandler                         BlockDeadBrainCoralHandler(BlockType::DeadBrainCoral);
	constexpr cDefaultBlockHandler                         BlockDeadBrainCoralBlockHandler(BlockType::DeadBrainCoralBlock);
	constexpr cDefaultBlockHandler                         BlockDeadBrainCoralFanHandler(BlockType::DeadBrainCoralFan);
	constexpr cDefaultBlockHandler                         BlockDeadBrainCoralWallFanHandler(BlockType::DeadBrainCoralWallFan);
	constexpr cDefaultBlockHandler                         BlockDeadBubbleCoralHandler(BlockType::DeadBubbleCoral);
	constexpr cDefaultBlockHandler                         BlockDeadBubbleCoralBlockHandler(BlockType::DeadBubbleCoralBlock);
	constexpr cDefaultBlockHandler                         BlockDeadBubbleCoralFanHandler(BlockType::DeadBubbleCoralFan);
	constexpr cDefaultBlockHandler                         BlockDeadBubbleCoralWallFanHandler(BlockType::DeadBubbleCoralWallFan);
	constexpr cBlockDeadBushHandler                        BlockDeadBushHandler(BlockType::DeadBush);
	constexpr cDefaultBlockHandler                         BlockDeadFireCoralHandler(BlockType::DeadFireCoral);
	constexpr cDefaultBlockHandler                         BlockDeadFireCoralBlockHandler(BlockType::DeadFireCoralBlock);
	constexpr cDefaultBlockHandler                         BlockDeadFireCoralFanHandler(BlockType::DeadFireCoralFan);
	constexpr cDefaultBlockHandler                         BlockDeadFireCoralWallFanHandler(BlockType::DeadFireCoralWallFan);
	constexpr cDefaultBlockHandler                         BlockDeadHornCoralHandler(BlockType::DeadHornCoral);
	constexpr cDefaultBlockHandler                         BlockDeadHornCoralBlockHandler(BlockType::DeadHornCoralBlock);
	constexpr cDefaultBlockHandler                         BlockDeadHornCoralFanHandler(BlockType::DeadHornCoralFan);
	constexpr cDefaultBlockHandler                         BlockDeadHornCoralWallFanHandler(BlockType::DeadHornCoralWallFan);
	constexpr cDefaultBlockHandler                         BlockDeadTubeCoralHandler(BlockType::DeadTubeCoral);
	constexpr cDefaultBlockHandler                         BlockDeadTubeCoralBlockHandler(BlockType::DeadTubeCoralBlock);
	constexpr cDefaultBlockHandler                         BlockDeadTubeCoralFanHandler(BlockType::DeadTubeCoralFan);
	constexpr cDefaultBlockHandler                         BlockDeadTubeCoralWallFanHandler(BlockType::DeadTubeCoralWallFan);
	constexpr cBlockRailHandler                            BlockDetectorRailHandler(BlockType::DetectorRail);
	constexpr cDefaultBlockHandler                         BlockDiamondBlockHandler(BlockType::DiamondBlock);
	constexpr cDefaultOreHandler                           BlockDiamondOreHandler(BlockType::DiamondOre);
	constexpr cDefaultBlockHandler                         BlockDioriteHandler(BlockType::Diorite);
	constexpr cBlockSlabHandler                            BlockDioriteSlabHandler(BlockType::DioriteSlab);
	constexpr cBlockStairsHandler                          BlockDioriteStairsHandler(BlockType::DioriteStairs);
	constexpr cDefaultBlockHandler                         BlockDioriteWallHandler(BlockType::DioriteWall);
	constexpr cBlockDirtHandler                            BlockDirtHandler(BlockType::Dirt);
	constexpr cBlockDropSpenserHandler                     BlockDispenserHandler(BlockType::Dispenser);
	constexpr cDefaultBlockHandler                         BlockDragonEggHandler(BlockType::DragonEgg);
	constexpr cBlockMobHeadHandler                         BlockDragonHeadHandler(BlockType::DragonHead);
	constexpr cBlockMobHeadHandler                         BlockDragonWallHeadHandler(BlockType::DragonWallHead);
	constexpr cDefaultBlockHandler                         BlockDriedKelpBlockHandler(BlockType::DriedKelpBlock);
	constexpr cBlockDropSpenserHandler                     BlockDropperHandler(BlockType::Dropper);
	constexpr cDefaultBlockHandler                         BlockEmeraldBlockHandler(BlockType::EmeraldBlock);
	constexpr cDefaultOreHandler                           BlockEmeraldOreHandler(BlockType::EmeraldOre);
	constexpr cBlockEnchantingTableHandler                 BlockEnchantingTableHandler(BlockType::EnchantingTable);
	constexpr cDefaultBlockHandler                         BlockEndGatewayHandler(BlockType::EndGateway);
	constexpr cDefaultBlockHandler                         BlockEndPortalHandler(BlockType::EndPortal);
	constexpr cBlockEndPortalFrameHandler                  BlockEndPortalFrameHandler(BlockType::EndPortalFrame);
	constexpr cDefaultBlockHandler                         BlockEndRodHandler(BlockType::EndRod);
	constexpr cDefaultBlockHandler                         BlockEndStoneHandler(BlockType::EndStone);
	constexpr cBlockSlabHandler                            BlockEndStoneBrickSlabHandler(BlockType::EndStoneBrickSlab);
	constexpr cBlockStairsHandler                          BlockEndStoneBrickStairsHandler(BlockType::EndStoneBrickStairs);
	constexpr cDefaultBlockHandler                         BlockEndStoneBrickWallHandler(BlockType::EndStoneBrickWall);
	constexpr cDefaultBlockHandler                         BlockEndStoneBricksHandler(BlockType::EndStoneBricks);
	constexpr cBlockEnderChestHandler                      BlockEnderChestHandler(BlockType::EnderChest);
	constexpr cBlockFarmlandHandler                        BlockFarmlandHandler(BlockType::Farmland);
	constexpr cBlockTallGrassHandler                       BlockFernHandler(BlockType::Fern);
	constexpr cBlockFireHandler                            BlockFireHandler(BlockType::Fire);
	constexpr cDefaultBlockHandler                         BlockFireCoralHandler(BlockType::FireCoral);
	constexpr cDefaultBlockHandler                         BlockFireCoralBlockHandler(BlockType::FireCoralBlock);
	constexpr cDefaultBlockHandler                         BlockFireCoralFanHandler(BlockType::FireCoralFan);
	constexpr cDefaultBlockHandler                         BlockFireCoralWallFanHandler(BlockType::FireCoralWallFan);
	constexpr cDefaultBlockHandler                         BlockFletchingTableHandler(BlockType::FletchingTable);
	constexpr cBlockFlowerPotHandler                       BlockFlowerPotHandler(BlockType::FlowerPot);
	constexpr cDefaultBlockHandler                         BlockFrostedIceHandler(BlockType::FrostedIce);
	constexpr cBlockFurnaceHandler                         BlockFurnaceHandler(BlockType::Furnace);
	constexpr cDefaultBlockHandler                         BlockGildedBlackstoneHandler(BlockType::GildedBlackstone);
	constexpr cBlockGlassHandler                           BlockGlassHandler(BlockType::Glass);
	constexpr cBlockGlassHandler                           BlockGlassPaneHandler(BlockType::GlassPane);
	constexpr cBlockGlowstoneHandler                       BlockGlowstoneHandler(BlockType::Glowstone);
	constexpr cDefaultBlockHandler                         BlockGoldBlockHandler(BlockType::GoldBlock);
	constexpr cDefaultOreHandler                           BlockGoldOreHandler(BlockType::GoldOre);
	constexpr cDefaultBlockHandler                         BlockGraniteHandler(BlockType::Granite);
	constexpr cBlockSlabHandler                            BlockGraniteSlabHandler(BlockType::GraniteSlab);
	constexpr cBlockStairsHandler                          BlockGraniteStairsHandler(BlockType::GraniteStairs);
	constexpr cDefaultBlockHandler                         BlockGraniteWallHandler(BlockType::GraniteWall);
	constexpr cBlockGrassHandler                           BlockGrassBlockHandler(BlockType::GrassBlock);
	constexpr cBlockGravelHandler                          BlockGravelHandler(BlockType::Gravel);
	constexpr cDefaultBlockHandler                         BlockGrayBannerHandler(BlockType::GrayBanner);
	constexpr cBlockBedHandler                             BlockGrayBedHandler(BlockType::GrayBed);
	constexpr cBlockCarpetHandler                          BlockGrayCarpetHandler(BlockType::GrayCarpet);
	constexpr cDefaultBlockHandler                         BlockGrayConcreteHandler(BlockType::GrayConcrete);
	constexpr cBlockConcretePowderHandler                  BlockGrayConcretePowderHandler(BlockType::GrayConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockGrayGlazedTerracottaHandler(BlockType::GrayGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockGrayShulkerBoxHandler(BlockType::GrayShulkerBox);
	constexpr cBlockGlassHandler                           BlockGrayStainedGlassHandler(BlockType::GrayStainedGlass);
	constexpr cBlockGlassHandler                           BlockGrayStainedGlassPaneHandler(BlockType::GrayStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockGrayTerracottaHandler(BlockType::GrayTerracotta);
	constexpr cDefaultBlockHandler                         BlockGrayWallBannerHandler(BlockType::GrayWallBanner);
	constexpr cBlockWoolHandler                            BlockGrayWoolHandler(BlockType::GrayWool);
	constexpr cDefaultBlockHandler                         BlockGreenBannerHandler(BlockType::GreenBanner);
	constexpr cBlockBedHandler                             BlockGreenBedHandler(BlockType::GreenBed);
	constexpr cBlockCarpetHandler                          BlockGreenCarpetHandler(BlockType::GreenCarpet);
	constexpr cDefaultBlockHandler                         BlockGreenConcreteHandler(BlockType::GreenConcrete);
	constexpr cBlockConcretePowderHandler                  BlockGreenConcretePowderHandler(BlockType::GreenConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockGreenGlazedTerracottaHandler(BlockType::GreenGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockGreenShulkerBoxHandler(BlockType::GreenShulkerBox);
	constexpr cBlockGlassHandler                           BlockGreenStainedGlassHandler(BlockType::GreenStainedGlass);
	constexpr cBlockGlassHandler                           BlockGreenStainedGlassPaneHandler(BlockType::GreenStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockGreenTerracottaHandler(BlockType::GreenTerracotta);
	constexpr cDefaultBlockHandler                         BlockGreenWallBannerHandler(BlockType::GreenWallBanner);
	constexpr cBlockWoolHandler                            BlockGreenWoolHandler(BlockType::GreenWool);
	constexpr cDefaultBlockHandler                         BlockGrindstoneHandler(BlockType::Grindstone);
	constexpr cBlockPressurePlateHandler                   BlockHeavyWeightedPressurePlateHandler(BlockType::HeavyWeightedPressurePlate);
	constexpr cDefaultBlockHandler                         BlockHoneyBlockHandler(BlockType::HoneyBlock);
	constexpr cDefaultBlockHandler                         BlockHoneycombBlockHandler(BlockType::HoneycombBlock);
	constexpr cBlockHopperHandler                          BlockHopperHandler(BlockType::Hopper);
	constexpr cDefaultBlockHandler                         BlockHornCoralHandler(BlockType::HornCoral);
	constexpr cDefaultBlockHandler                         BlockHornCoralBlockHandler(BlockType::HornCoralBlock);
	constexpr cDefaultBlockHandler                         BlockHornCoralFanHandler(BlockType::HornCoralFan);
	constexpr cDefaultBlockHandler                         BlockHornCoralWallFanHandler(BlockType::HornCoralWallFan);
	constexpr cBlockIceHandler                             BlockIceHandler(BlockType::Ice);
	constexpr cBlockInfestedHandler                        BlockInfestedChiseledStoneBricksHandler(BlockType::InfestedChiseledStoneBricks);
	constexpr cBlockInfestedHandler                        BlockInfestedCobblestoneHandler(BlockType::InfestedCobblestone);
	constexpr cBlockInfestedHandler                        BlockInfestedCrackedStoneBricksHandler(BlockType::InfestedCrackedStoneBricks);
	constexpr cBlockInfestedHandler                        BlockInfestedMossyStoneBricksHandler(BlockType::InfestedMossyStoneBricks);
	constexpr cBlockInfestedHandler                        BlockInfestedStoneHandler(BlockType::InfestedStone);
	constexpr cBlockInfestedHandler                        BlockInfestedStoneBricksHandler(BlockType::InfestedStoneBricks);
	constexpr cDefaultBlockHandler                         BlockIronBarsHandler(BlockType::IronBars);
	constexpr cDefaultBlockHandler                         BlockIronBlockHandler(BlockType::IronBlock);
	constexpr cBlockDoorHandler                            BlockIronDoorHandler(BlockType::IronDoor);
	constexpr cDefaultOreHandler                           BlockIronOreHandler(BlockType::IronOre);
	constexpr cBlockTrapdoorHandler                        BlockIronTrapdoorHandler(BlockType::IronTrapdoor);
	constexpr cBlockPumpkinHandler                         BlockJackOLanternHandler(BlockType::JackOLantern);
	constexpr cDefaultBlockHandler                         BlockJigsawHandler(BlockType::Jigsaw);
	constexpr cBlockJukeboxHandler                         BlockJukeboxHandler(BlockType::Jukebox);
	constexpr cBlockButtonHandler                          BlockJungleButtonHandler(BlockType::JungleButton);
	constexpr cBlockDoorHandler                            BlockJungleDoorHandler(BlockType::JungleDoor);
	constexpr cBlockFenceHandler                           BlockJungleFenceHandler(BlockType::JungleFence);
	constexpr cBlockFenceGateHandler                       BlockJungleFenceGateHandler(BlockType::JungleFenceGate);
	constexpr cBlockLeavesHandler                          BlockJungleLeavesHandler(BlockType::JungleLeaves);
	constexpr cBlockLogHandler                             BlockJungleLogHandler(BlockType::JungleLog);
	constexpr cBlockPlanksHandler                          BlockJunglePlanksHandler(BlockType::JunglePlanks);
	constexpr cBlockPressurePlateHandler                   BlockJunglePressurePlateHandler(BlockType::JunglePressurePlate);
	constexpr cBlockSaplingHandler                         BlockJungleSaplingHandler(BlockType::JungleSapling);
	constexpr cBlockSignPostHandler                        BlockJungleSignHandler(BlockType::JungleSign);
	constexpr cBlockSlabHandler                            BlockJungleSlabHandler(BlockType::JungleSlab);
	constexpr cBlockStairsHandler                          BlockJungleStairsHandler(BlockType::JungleStairs);
	constexpr cBlockTrapdoorHandler                        BlockJungleTrapdoorHandler(BlockType::JungleTrapdoor);
	constexpr cBlockWallSignHandler                        BlockJungleWallSignHandler(BlockType::JungleWallSign);
	constexpr cDefaultBlockHandler                         BlockJungleWoodHandler(BlockType::JungleWood);
	constexpr cDefaultBlockHandler                         BlockKelpHandler(BlockType::Kelp);
	constexpr cDefaultBlockHandler                         BlockKelpPlantHandler(BlockType::KelpPlant);
	constexpr cBlockLadderHandler                          BlockLadderHandler(BlockType::Ladder);
	constexpr cDefaultBlockHandler                         BlockLanternHandler(BlockType::Lantern);
	constexpr cDefaultBlockHandler                         BlockLapisBlockHandler(BlockType::LapisBlock);
	constexpr cDefaultOreHandler                           BlockLapisOreHandler(BlockType::LapisOre);
	constexpr cBlockTallGrassHandler                       BlockLargeFernHandler(BlockType::LargeFern);
	constexpr cBlockLavaHandler                            BlockLavaHandler(BlockType::Lava);
	constexpr cDefaultBlockHandler                         BlockLecternHandler(BlockType::Lectern);
	constexpr cBlockLeverHandler                           BlockLeverHandler(BlockType::Lever);
	constexpr cDefaultBlockHandler                         BlockLightBlueBannerHandler(BlockType::LightBlueBanner);
	constexpr cBlockBedHandler                             BlockLightBlueBedHandler(BlockType::LightBlueBed);
	constexpr cBlockCarpetHandler                          BlockLightBlueCarpetHandler(BlockType::LightBlueCarpet);
	constexpr cDefaultBlockHandler                         BlockLightBlueConcreteHandler(BlockType::LightBlueConcrete);
	constexpr cBlockConcretePowderHandler                  BlockLightBlueConcretePowderHandler(BlockType::LightBlueConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockLightBlueGlazedTerracottaHandler(BlockType::LightBlueGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockLightBlueShulkerBoxHandler(BlockType::LightBlueShulkerBox);
	constexpr cBlockGlassHandler                           BlockLightBlueStainedGlassHandler(BlockType::LightBlueStainedGlass);
	constexpr cBlockGlassHandler                           BlockLightBlueStainedGlassPaneHandler(BlockType::LightBlueStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockLightBlueTerracottaHandler(BlockType::LightBlueTerracotta);
	constexpr cDefaultBlockHandler                         BlockLightBlueWallBannerHandler(BlockType::LightBlueWallBanner);
	constexpr cBlockWoolHandler                            BlockLightBlueWoolHandler(BlockType::LightBlueWool);
	constexpr cDefaultBlockHandler                         BlockLightGrayBannerHandler(BlockType::LightGrayBanner);
	constexpr cBlockBedHandler                             BlockLightGrayBedHandler(BlockType::LightGrayBed);
	constexpr cBlockCarpetHandler                          BlockLightGrayCarpetHandler(BlockType::LightGrayCarpet);
	constexpr cDefaultBlockHandler                         BlockLightGrayConcreteHandler(BlockType::LightGrayConcrete);
	constexpr cBlockConcretePowderHandler                  BlockLightGrayConcretePowderHandler(BlockType::LightGrayConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockLightGrayGlazedTerracottaHandler(BlockType::LightGrayGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockLightGrayShulkerBoxHandler(BlockType::LightGrayShulkerBox);
	constexpr cBlockGlassHandler                           BlockLightGrayStainedGlassHandler(BlockType::LightGrayStainedGlass);
	constexpr cBlockGlassHandler                           BlockLightGrayStainedGlassPaneHandler(BlockType::LightGrayStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockLightGrayTerracottaHandler(BlockType::LightGrayTerracotta);
	constexpr cDefaultBlockHandler                         BlockLightGrayWallBannerHandler(BlockType::LightGrayWallBanner);
	constexpr cBlockWoolHandler                            BlockLightGrayWoolHandler(BlockType::LightGrayWool);
	constexpr cBlockPressurePlateHandler                   BlockLightWeightedPressurePlateHandler(BlockType::LightWeightedPressurePlate);
	constexpr cBlockBigFlowerHandler                       BlockLilacHandler(BlockType::Lilac);
	constexpr cBlockFlowerHandler                          BlockLilyOfTheValleyHandler(BlockType::LilyOfTheValley);
	constexpr cBlockLilypadHandler                         BlockLilyPadHandler(BlockType::LilyPad);
	constexpr cDefaultBlockHandler                         BlockLimeBannerHandler(BlockType::LimeBanner);
	constexpr cBlockBedHandler                             BlockLimeBedHandler(BlockType::LimeBed);
	constexpr cBlockCarpetHandler                          BlockLimeCarpetHandler(BlockType::LimeCarpet);
	constexpr cDefaultBlockHandler                         BlockLimeConcreteHandler(BlockType::LimeConcrete);
	constexpr cBlockConcretePowderHandler                  BlockLimeConcretePowderHandler(BlockType::LimeConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockLimeGlazedTerracottaHandler(BlockType::LimeGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockLimeShulkerBoxHandler(BlockType::LimeShulkerBox);
	constexpr cBlockGlassHandler                           BlockLimeStainedGlassHandler(BlockType::LimeStainedGlass);
	constexpr cBlockGlassHandler                           BlockLimeStainedGlassPaneHandler(BlockType::LimeStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockLimeTerracottaHandler(BlockType::LimeTerracotta);
	constexpr cDefaultBlockHandler                         BlockLimeWallBannerHandler(BlockType::LimeWallBanner);
	constexpr cBlockWoolHandler                            BlockLimeWoolHandler(BlockType::LimeWool);
	constexpr cDefaultBlockHandler                         BlockLodestoneHandler(BlockType::Lodestone);
	constexpr cDefaultBlockHandler                         BlockLoomHandler(BlockType::Loom);
	constexpr cDefaultBlockHandler                         BlockMagentaBannerHandler(BlockType::MagentaBanner);
	constexpr cBlockBedHandler                             BlockMagentaBedHandler(BlockType::MagentaBed);
	constexpr cBlockCarpetHandler                          BlockMagentaCarpetHandler(BlockType::MagentaCarpet);
	constexpr cDefaultBlockHandler                         BlockMagentaConcreteHandler(BlockType::MagentaConcrete);
	constexpr cBlockConcretePowderHandler                  BlockMagentaConcretePowderHandler(BlockType::MagentaConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockMagentaGlazedTerracottaHandler(BlockType::MagentaGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockMagentaShulkerBoxHandler(BlockType::MagentaShulkerBox);
	constexpr cBlockGlassHandler                           BlockMagentaStainedGlassHandler(BlockType::MagentaStainedGlass);
	constexpr cBlockGlassHandler                           BlockMagentaStainedGlassPaneHandler(BlockType::MagentaStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockMagentaTerracottaHandler(BlockType::MagentaTerracotta);
	constexpr cDefaultBlockHandler                         BlockMagentaWallBannerHandler(BlockType::MagentaWallBanner);
	constexpr cBlockWoolHandler                            BlockMagentaWoolHandler(BlockType::MagentaWool);
	constexpr cDefaultBlockHandler                         BlockMagmaBlockHandler(BlockType::MagmaBlock);
	constexpr cBlockMelonHandler                           BlockMelonHandler(BlockType::Melon);
	constexpr cBlockMelonStemHandler                       BlockMelonStemHandler(BlockType::MelonStem);
	constexpr cDefaultBlockHandler                         BlockMossyCobblestoneHandler(BlockType::MossyCobblestone);
	constexpr cBlockSlabHandler                            BlockMossyCobblestoneSlabHandler(BlockType::MossyCobblestoneSlab);
	constexpr cBlockStairsHandler                          BlockMossyCobblestoneStairsHandler(BlockType::MossyCobblestoneStairs);
	constexpr cDefaultBlockHandler                         BlockMossyCobblestoneWallHandler(BlockType::MossyCobblestoneWall);
	constexpr cBlockSlabHandler                            BlockMossyStoneBrickSlabHandler(BlockType::MossyStoneBrickSlab);
	constexpr cBlockStairsHandler                          BlockMossyStoneBrickStairsHandler(BlockType::MossyStoneBrickStairs);
	constexpr cDefaultBlockHandler                         BlockMossyStoneBrickWallHandler(BlockType::MossyStoneBrickWall);
	constexpr cDefaultBlockHandler                         BlockMossyStoneBricksHandler(BlockType::MossyStoneBricks);
	constexpr cBlockPistonHandler                          BlockMovingPistonHandler(BlockType::MovingPiston);
	constexpr cBlockHugeMushroomHandler                    BlockMushroomStemHandler(BlockType::MushroomStem);
	constexpr cBlockMyceliumHandler                        BlockMyceliumHandler(BlockType::Mycelium);
	constexpr cBlockFenceHandler                           BlockNetherBrickFenceHandler(BlockType::NetherBrickFence);
	constexpr cBlockSlabHandler                            BlockNetherBrickSlabHandler(BlockType::NetherBrickSlab);
	constexpr cBlockStairsHandler                          BlockNetherBrickStairsHandler(BlockType::NetherBrickStairs);
	constexpr cDefaultBlockHandler                         BlockNetherBrickWallHandler(BlockType::NetherBrickWall);
	constexpr cDefaultBlockHandler                         BlockNetherBricksHandler(BlockType::NetherBricks);
	constexpr cDefaultOreHandler                           BlockNetherGoldOreHandler(BlockType::NetherGoldOre);
	constexpr cBlockPortalHandler                          BlockNetherPortalHandler(BlockType::NetherPortal);
	constexpr cDefaultOreHandler                           BlockNetherQuartzOreHandler(BlockType::NetherQuartzOre);
	constexpr cDefaultBlockHandler                         BlockNetherSproutsHandler(BlockType::NetherSprouts);
	constexpr cBlockNetherWartHandler                      BlockNetherWartHandler(BlockType::NetherWart);
	constexpr cBlockNetherWartHandler                      BlockNetherWartBlockHandler(BlockType::NetherWartBlock);
	constexpr cDefaultOreHandler                           BlockNetheriteBlockHandler(BlockType::NetheriteBlock);
	constexpr cDefaultBlockHandler                         BlockNetherrackHandler(BlockType::Netherrack);
	constexpr cBlockNoteBlockHandler                       BlockNoteBlockHandler(BlockType::NoteBlock);
	constexpr cBlockButtonHandler                          BlockOakButtonHandler(BlockType::OakButton);
	constexpr cBlockDoorHandler                            BlockOakDoorHandler(BlockType::OakDoor);
	constexpr cBlockFenceHandler                           BlockOakFenceHandler(BlockType::OakFence);
	constexpr cBlockFenceGateHandler                       BlockOakFenceGateHandler(BlockType::OakFenceGate);
	constexpr cBlockLeavesHandler                          BlockOakLeavesHandler(BlockType::OakLeaves);
	constexpr cBlockLogHandler                             BlockOakLogHandler(BlockType::OakLog);
	constexpr cBlockPlanksHandler                          BlockOakPlanksHandler(BlockType::OakPlanks);
	constexpr cBlockPressurePlateHandler                   BlockOakPressurePlateHandler(BlockType::OakPressurePlate);
	constexpr cBlockSaplingHandler                         BlockOakSaplingHandler(BlockType::OakSapling);
	constexpr cBlockSignPostHandler                        BlockOakSignHandler(BlockType::OakSign);
	constexpr cBlockSlabHandler                            BlockOakSlabHandler(BlockType::OakSlab);
	constexpr cBlockStairsHandler                          BlockOakStairsHandler(BlockType::OakStairs);
	constexpr cBlockTrapdoorHandler                        BlockOakTrapdoorHandler(BlockType::OakTrapdoor);
	constexpr cBlockWallSignHandler                        BlockOakWallSignHandler(BlockType::OakWallSign);
	constexpr cDefaultBlockHandler                         BlockOakWoodHandler(BlockType::OakWood);
	constexpr cBlockObserverHandler                        BlockObserverHandler(BlockType::Observer);
	constexpr cDefaultBlockHandler                         BlockObsidianHandler(BlockType::Obsidian);
	constexpr cDefaultBlockHandler                         BlockOrangeBannerHandler(BlockType::OrangeBanner);
	constexpr cBlockBedHandler                             BlockOrangeBedHandler(BlockType::OrangeBed);
	constexpr cBlockCarpetHandler                          BlockOrangeCarpetHandler(BlockType::OrangeCarpet);
	constexpr cDefaultBlockHandler                         BlockOrangeConcreteHandler(BlockType::OrangeConcrete);
	constexpr cBlockConcretePowderHandler                  BlockOrangeConcretePowderHandler(BlockType::OrangeConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockOrangeGlazedTerracottaHandler(BlockType::OrangeGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockOrangeShulkerBoxHandler(BlockType::OrangeShulkerBox);
	constexpr cBlockGlassHandler                           BlockOrangeStainedGlassHandler(BlockType::OrangeStainedGlass);
	constexpr cBlockGlassHandler                           BlockOrangeStainedGlassPaneHandler(BlockType::OrangeStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockOrangeTerracottaHandler(BlockType::OrangeTerracotta);
	constexpr cBlockFlowerHandler                          BlockOrangeTulipHandler(BlockType::OrangeTulip);
	constexpr cDefaultBlockHandler                         BlockOrangeWallBannerHandler(BlockType::OrangeWallBanner);
	constexpr cBlockWoolHandler                            BlockOrangeWoolHandler(BlockType::OrangeWool);
	constexpr cBlockFlowerHandler                          BlockOxeyeDaisyHandler(BlockType::OxeyeDaisy);
	constexpr cBlockPackedIceHandler                       BlockPackedIceHandler(BlockType::PackedIce);
	constexpr cBlockBigFlowerHandler                       BlockPeonyHandler(BlockType::Peony);
	constexpr cBlockSlabHandler                            BlockPetrifiedOakSlabHandler(BlockType::PetrifiedOakSlab);
	constexpr cDefaultBlockHandler                         BlockPinkBannerHandler(BlockType::PinkBanner);
	constexpr cBlockBedHandler                             BlockPinkBedHandler(BlockType::PinkBed);
	constexpr cBlockCarpetHandler                          BlockPinkCarpetHandler(BlockType::PinkCarpet);
	constexpr cDefaultBlockHandler                         BlockPinkConcreteHandler(BlockType::PinkConcrete);
	constexpr cBlockConcretePowderHandler                  BlockPinkConcretePowderHandler(BlockType::PinkConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockPinkGlazedTerracottaHandler(BlockType::PinkGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockPinkShulkerBoxHandler(BlockType::PinkShulkerBox);
	constexpr cBlockGlassHandler                           BlockPinkStainedGlassHandler(BlockType::PinkStainedGlass);
	constexpr cBlockGlassHandler                           BlockPinkStainedGlassPaneHandler(BlockType::PinkStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockPinkTerracottaHandler(BlockType::PinkTerracotta);
	constexpr cBlockFlowerHandler                          BlockPinkTulipHandler(BlockType::PinkTulip);
	constexpr cDefaultBlockHandler                         BlockPinkWallBannerHandler(BlockType::PinkWallBanner);
	constexpr cBlockWoolHandler                            BlockPinkWoolHandler(BlockType::PinkWool);
	constexpr cBlockPistonHandler                          BlockPistonHandler(BlockType::Piston);
	constexpr cBlockPistonHeadHandler                      BlockPistonHeadHandler;
	constexpr cBlockMobHeadHandler                         BlockPlayerHeadHandler(BlockType::PlayerHead);
	constexpr cBlockMobHeadHandler                         BlockPlayerWallHeadHandler(BlockType::PlayerWallHead);
	constexpr cBlockPodzolHandler                          BlockPodzolHandler(BlockType::Podzol);
	constexpr cDefaultBlockHandler                         BlockPolishedAndesiteHandler(BlockType::PolishedAndesite);
	constexpr cBlockSlabHandler                            BlockPolishedAndesiteSlabHandler(BlockType::PolishedAndesiteSlab);
	constexpr cBlockStairsHandler                          BlockPolishedAndesiteStairsHandler(BlockType::PolishedAndesiteStairs);
	constexpr cDefaultBlockHandler                         BlockPolishedBasaltHandler(BlockType::PolishedBasalt);
	constexpr cDefaultBlockHandler                         BlockPolishedBlackstoneHandler(BlockType::PolishedBlackstone);
	constexpr cBlockSlabHandler                            BlockPolishedBlackstoneBrickSlabHandler(BlockType::PolishedBlackstoneBrickSlab);
	constexpr cBlockStairsHandler                          BlockPolishedBlackstoneBrickStairsHandler(BlockType::PolishedBlackstoneBrickStairs);
	constexpr cDefaultBlockHandler                         BlockPolishedBlackstoneBrickWallHandler(BlockType::PolishedBlackstoneBrickWall);
	constexpr cDefaultBlockHandler                         BlockPolishedBlackstoneBricksHandler(BlockType::PolishedBlackstoneBricks);
	constexpr cBlockButtonHandler                          BlockPolishedBlackstoneButtonHandler(BlockType::PolishedBlackstoneButton);
	constexpr cBlockPressurePlateHandler                   BlockPolishedBlackstonePressurePlateHandler(BlockType::PolishedBlackstonePressurePlate);
	constexpr cBlockSlabHandler                            BlockPolishedBlackstoneSlabHandler(BlockType::PolishedBlackstoneSlab);
	constexpr cBlockStairsHandler                          BlockPolishedBlackstoneStairsHandler(BlockType::PolishedBlackstoneStairs);
	constexpr cDefaultBlockHandler                         BlockPolishedBlackstoneWallHandler(BlockType::PolishedBlackstoneWall);
	constexpr cDefaultBlockHandler                         BlockPolishedDioriteHandler(BlockType::PolishedDiorite);
	constexpr cBlockSlabHandler                            BlockPolishedDioriteSlabHandler(BlockType::PolishedDioriteSlab);
	constexpr cBlockStairsHandler                          BlockPolishedDioriteStairsHandler(BlockType::PolishedDioriteStairs);
	constexpr cDefaultBlockHandler                         BlockPolishedGraniteHandler(BlockType::PolishedGranite);
	constexpr cBlockSlabHandler                            BlockPolishedGraniteSlabHandler(BlockType::PolishedGraniteSlab);
	constexpr cBlockStairsHandler                          BlockPolishedGraniteStairsHandler(BlockType::PolishedGraniteStairs);
	constexpr cBlockFlowerHandler                          BlockPoppyHandler(BlockType::Poppy);
	constexpr cBlockCropsHandler                           BlockPotatoesHandler(BlockType::Potatoes);
	constexpr cBlockFlowerPotHandler                       BlockPottedAcaciaSaplingHandler(BlockType::PottedAcaciaSapling);
	constexpr cBlockFlowerPotHandler                       BlockPottedAlliumHandler(BlockType::PottedAllium);
	constexpr cBlockFlowerPotHandler                       BlockPottedAzureBluetHandler(BlockType::PottedAzureBluet);
	constexpr cBlockFlowerPotHandler                       BlockPottedBambooHandler(BlockType::PottedBamboo);
	constexpr cBlockFlowerPotHandler                       BlockPottedBirchSaplingHandler(BlockType::PottedBirchSapling);
	constexpr cBlockFlowerPotHandler                       BlockPottedBlueOrchidHandler(BlockType::PottedBlueOrchid);
	constexpr cBlockFlowerPotHandler                       BlockPottedBrownMushroomHandler(BlockType::PottedBrownMushroom);
	constexpr cBlockFlowerPotHandler                       BlockPottedCactusHandler(BlockType::PottedCactus);
	constexpr cBlockFlowerPotHandler                       BlockPottedCornflowerHandler(BlockType::PottedCornflower);
	constexpr cBlockFlowerPotHandler                       BlockPottedCrimsonFungusHandler(BlockType::PottedCrimsonFungus);
	constexpr cBlockFlowerPotHandler                       BlockPottedCrimsonRootsHandler(BlockType::PottedCrimsonRoots);
	constexpr cBlockFlowerPotHandler                       BlockPottedDandelionHandler(BlockType::PottedDandelion);
	constexpr cBlockFlowerPotHandler                       BlockPottedDarkOakSaplingHandler(BlockType::PottedDarkOakSapling);
	constexpr cBlockFlowerPotHandler                       BlockPottedDeadBushHandler(BlockType::PottedDeadBush);
	constexpr cBlockFlowerPotHandler                       BlockPottedFernHandler(BlockType::PottedFern);
	constexpr cBlockFlowerPotHandler                       BlockPottedJungleSaplingHandler(BlockType::PottedJungleSapling);
	constexpr cBlockFlowerPotHandler                       BlockPottedLilyOfTheValleyHandler(BlockType::PottedLilyOfTheValley);
	constexpr cBlockFlowerPotHandler                       BlockPottedOakSaplingHandler(BlockType::PottedOakSapling);
	constexpr cBlockFlowerPotHandler                       BlockPottedOrangeTulipHandler(BlockType::PottedOrangeTulip);
	constexpr cBlockFlowerPotHandler                       BlockPottedOxeyeDaisyHandler(BlockType::PottedOxeyeDaisy);
	constexpr cBlockFlowerPotHandler                       BlockPottedPinkTulipHandler(BlockType::PottedPinkTulip);
	constexpr cBlockFlowerPotHandler                       BlockPottedPoppyHandler(BlockType::PottedPoppy);
	constexpr cBlockFlowerPotHandler                       BlockPottedRedMushroomHandler(BlockType::PottedRedMushroom);
	constexpr cBlockFlowerPotHandler                       BlockPottedRedTulipHandler(BlockType::PottedRedTulip);
	constexpr cBlockFlowerPotHandler                       BlockPottedSpruceSaplingHandler(BlockType::PottedSpruceSapling);
	constexpr cBlockFlowerPotHandler                       BlockPottedWarpedFungusHandler(BlockType::PottedWarpedFungus);
	constexpr cBlockFlowerPotHandler                       BlockPottedWarpedRootsHandler(BlockType::PottedWarpedRoots);
	constexpr cBlockFlowerPotHandler                       BlockPottedWhiteTulipHandler(BlockType::PottedWhiteTulip);
	constexpr cBlockFlowerPotHandler                       BlockPottedWitherRoseHandler(BlockType::PottedWitherRose);
	constexpr cBlockRailHandler                            BlockPoweredRailHandler(BlockType::PoweredRail);
	constexpr cDefaultBlockHandler                         BlockPrismarineHandler(BlockType::Prismarine);
	constexpr cBlockSlabHandler                            BlockPrismarineBrickSlabHandler(BlockType::PrismarineBrickSlab);
	constexpr cBlockStairsHandler                          BlockPrismarineBrickStairsHandler(BlockType::PrismarineBrickStairs);
	constexpr cDefaultBlockHandler                         BlockPrismarineBricksHandler(BlockType::PrismarineBricks);
	constexpr cBlockSlabHandler                            BlockPrismarineSlabHandler(BlockType::PrismarineSlab);
	constexpr cBlockStairsHandler                          BlockPrismarineStairsHandler(BlockType::PrismarineStairs);
	constexpr cDefaultBlockHandler                         BlockPrismarineWallHandler(BlockType::PrismarineWall);
	constexpr cBlockPumpkinHandler                         BlockPumpkinHandler(BlockType::Pumpkin);
	constexpr cBlockPumpkinStemHandler                     BlockPumpkinStemHandler(BlockType::PumpkinStem);
	constexpr cDefaultBlockHandler                         BlockPurpleBannerHandler(BlockType::PurpleBanner);
	constexpr cBlockBedHandler                             BlockPurpleBedHandler(BlockType::PurpleBed);
	constexpr cBlockCarpetHandler                          BlockPurpleCarpetHandler(BlockType::PurpleCarpet);
	constexpr cDefaultBlockHandler                         BlockPurpleConcreteHandler(BlockType::PurpleConcrete);
	constexpr cBlockConcretePowderHandler                  BlockPurpleConcretePowderHandler(BlockType::PurpleConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockPurpleGlazedTerracottaHandler(BlockType::PurpleGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockPurpleShulkerBoxHandler(BlockType::PurpleShulkerBox);
	constexpr cBlockGlassHandler                           BlockPurpleStainedGlassHandler(BlockType::PurpleStainedGlass);
	constexpr cBlockGlassHandler                           BlockPurpleStainedGlassPaneHandler(BlockType::PurpleStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockPurpleTerracottaHandler(BlockType::PurpleTerracotta);
	constexpr cDefaultBlockHandler                         BlockPurpleWallBannerHandler(BlockType::PurpleWallBanner);
	constexpr cBlockWoolHandler                            BlockPurpleWoolHandler(BlockType::PurpleWool);
	constexpr cDefaultBlockHandler                         BlockPurpurBlockHandler(BlockType::PurpurBlock);
	constexpr cDefaultBlockHandler                         BlockPurpurPillarHandler(BlockType::PurpurPillar);
	constexpr cBlockSlabHandler                            BlockPurpurSlabHandler(BlockType::PurpurSlab);
	constexpr cBlockStairsHandler                          BlockPurpurStairsHandler(BlockType::PurpurStairs);
	constexpr cDefaultBlockHandler                         BlockQuartzBlockHandler(BlockType::QuartzBlock);
	constexpr cDefaultBlockHandler                         BlockQuartzBricksHandler(BlockType::QuartzBricks);
	constexpr cDefaultBlockHandler                         BlockQuartzPillarHandler(BlockType::QuartzPillar);
	constexpr cBlockSlabHandler                            BlockQuartzSlabHandler(BlockType::QuartzSlab);
	constexpr cBlockStairsHandler                          BlockQuartzStairsHandler(BlockType::QuartzStairs);
	constexpr cBlockRailHandler                            BlockRailHandler(BlockType::Rail);
	constexpr cDefaultBlockHandler                         BlockRedBannerHandler(BlockType::RedBanner);
	constexpr cBlockBedHandler                             BlockRedBedHandler(BlockType::RedBed);
	constexpr cBlockCarpetHandler                          BlockRedCarpetHandler(BlockType::RedCarpet);
	constexpr cDefaultBlockHandler                         BlockRedConcreteHandler(BlockType::RedConcrete);
	constexpr cBlockConcretePowderHandler                  BlockRedConcretePowderHandler(BlockType::RedConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockRedGlazedTerracottaHandler(BlockType::RedGlazedTerracotta);
	constexpr cBlockMushroomHandler                        BlockRedMushroomHandler(BlockType::RedMushroom);
	constexpr cBlockHugeMushroomHandler                    BlockRedMushroomBlockHandler(BlockType::RedMushroomBlock);
	constexpr cBlockSlabHandler                            BlockRedNetherBrickSlabHandler(BlockType::RedNetherBrickSlab);
	constexpr cBlockStairsHandler                          BlockRedNetherBrickStairsHandler(BlockType::RedNetherBrickStairs);
	constexpr cDefaultBlockHandler                         BlockRedNetherBrickWallHandler(BlockType::RedNetherBrickWall);
	constexpr cDefaultBlockHandler                         BlockRedNetherBricksHandler(BlockType::RedNetherBricks);
	constexpr cDefaultBlockHandler                         BlockRedSandHandler(BlockType::RedSand);
	constexpr cDefaultBlockHandler                         BlockRedSandstoneHandler(BlockType::RedSandstone);
	constexpr cBlockSlabHandler                            BlockRedSandstoneSlabHandler(BlockType::RedSandstoneSlab);
	constexpr cBlockStairsHandler                          BlockRedSandstoneStairsHandler(BlockType::RedSandstoneStairs);
	constexpr cDefaultBlockHandler                         BlockRedSandstoneWallHandler(BlockType::RedSandstoneWall);
	constexpr cDefaultBlockHandler                         BlockRedShulkerBoxHandler(BlockType::RedShulkerBox);
	constexpr cBlockGlassHandler                           BlockRedStainedGlassHandler(BlockType::RedStainedGlass);
	constexpr cBlockGlassHandler                           BlockRedStainedGlassPaneHandler(BlockType::RedStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockRedTerracottaHandler(BlockType::RedTerracotta);
	constexpr cBlockFlowerHandler                          BlockRedTulipHandler(BlockType::RedTulip);
	constexpr cDefaultBlockHandler                         BlockRedWallBannerHandler(BlockType::RedWallBanner);
	constexpr cBlockWoolHandler                            BlockRedWoolHandler(BlockType::RedWool);
	constexpr cDefaultBlockHandler                         BlockRedstoneBlockHandler(BlockType::RedstoneBlock);
	constexpr cBlockRedstoneLampHandler                    BlockRedstoneLampHandler(BlockType::RedstoneLamp);
	constexpr cBlockRedstoneOreHandler                     BlockRedstoneOreHandler(BlockType::RedstoneOre);
	constexpr cBlockTorchHandler                           BlockRedstoneTorchHandler(BlockType::RedstoneTorch);
	constexpr cBlockTorchHandler                           BlockRedstoneWallTorchHandler(BlockType::RedstoneWallTorch);
	constexpr cBlockRedstoneWireHandler                    BlockRedstoneWireHandler(BlockType::RedstoneWire);
	constexpr cBlockRedstoneRepeaterHandler                BlockRepeaterHandler(BlockType::Repeater);
	constexpr cBlockCommandBlockHandler                    BlockRepeatingCommandBlockHandler(BlockType::RepeatingCommandBlock);
	constexpr cDefaultBlockHandler                         BlockRespawnAnchorHandler(BlockType::RespawnAnchor);
	constexpr cBlockBigFlowerHandler                       BlockRoseBushHandler(BlockType::RoseBush);
	constexpr cBlockSandHandler                            BlockSandHandler(BlockType::Sand);
	constexpr cDefaultBlockHandler                         BlockSandstoneHandler(BlockType::Sandstone);
	constexpr cBlockSlabHandler                            BlockSandstoneSlabHandler(BlockType::SandstoneSlab);
	constexpr cBlockStairsHandler                          BlockSandstoneStairsHandler(BlockType::SandstoneStairs);
	constexpr cDefaultBlockHandler                         BlockSandstoneWallHandler(BlockType::SandstoneWall);
	constexpr cDefaultBlockHandler                         BlockScaffoldingHandler(BlockType::Scaffolding);
	constexpr cBlockSeaLanternHandler                      BlockSeaLanternHandler(BlockType::SeaLantern);
	constexpr cDefaultBlockHandler                         BlockSeaPickleHandler(BlockType::SeaPickle);
	constexpr cDefaultBlockHandler                         BlockSeagrassHandler(BlockType::Seagrass);
	constexpr cDefaultBlockHandler                         BlockShroomlightHandler(BlockType::Shroomlight);
	constexpr cDefaultBlockHandler                         BlockShulkerBoxHandler(BlockType::ShulkerBox);
	constexpr cBlockMobHeadHandler                         BlockSkeletonSkullHandler(BlockType::SkeletonSkull);
	constexpr cBlockMobHeadHandler                         BlockSkeletonWallSkullHandler(BlockType::SkeletonWallSkull);
	constexpr cBlockSlimeHandler                           BlockSlimeBlockHandler(BlockType::SlimeBlock);
	constexpr cDefaultBlockHandler                         BlockSmithingTableHandler(BlockType::SmithingTable);
	constexpr cDefaultBlockHandler                         BlockSmokerHandler(BlockType::Smoker);
	constexpr cDefaultBlockHandler                         BlockSmoothQuartzHandler(BlockType::SmoothQuartz);
	constexpr cBlockSlabHandler                            BlockSmoothQuartzSlabHandler(BlockType::SmoothQuartzSlab);
	constexpr cBlockStairsHandler                          BlockSmoothQuartzStairsHandler(BlockType::SmoothQuartzStairs);
	constexpr cDefaultBlockHandler                         BlockSmoothRedSandstoneHandler(BlockType::SmoothRedSandstone);
	constexpr cBlockSlabHandler                            BlockSmoothRedSandstoneSlabHandler(BlockType::SmoothRedSandstoneSlab);
	constexpr cBlockStairsHandler                          BlockSmoothRedSandstoneStairsHandler(BlockType::SmoothRedSandstoneStairs);
	constexpr cDefaultBlockHandler                         BlockSmoothSandstoneHandler(BlockType::SmoothSandstone);
	constexpr cBlockSlabHandler                            BlockSmoothSandstoneSlabHandler(BlockType::SmoothSandstoneSlab);
	constexpr cBlockStairsHandler                          BlockSmoothSandstoneStairsHandler(BlockType::SmoothSandstoneStairs);
	constexpr cDefaultBlockHandler                         BlockSmoothStoneHandler(BlockType::SmoothStone);
	constexpr cBlockSlabHandler                            BlockSmoothStoneSlabHandler(BlockType::SmoothStoneSlab);
	constexpr cBlockSnowHandler                            BlockSnowHandler(BlockType::Snow);
	constexpr cDefaultBlockHandler                         BlockSnowBlockHandler(BlockType::SnowBlock);
	constexpr cDefaultBlockHandler                         BlockSoulCampfireHandler(BlockType::SoulCampfire);
	constexpr cDefaultBlockHandler                         BlockSoulFireHandler(BlockType::SoulFire);
	constexpr cDefaultBlockHandler                         BlockSoulLanternHandler(BlockType::SoulLantern);
	constexpr cDefaultBlockHandler                         BlockSoulSandHandler(BlockType::SoulSand);
	constexpr cDefaultBlockHandler                         BlockSoulSoilHandler(BlockType::SoulSoil);
	constexpr cDefaultBlockHandler                         BlockSoulTorchHandler(BlockType::SoulTorch);
	constexpr cDefaultBlockHandler                         BlockSoulWallTorchHandler(BlockType::SoulWallTorch);
	constexpr cBlockMobSpawnerHandler                      BlockSpawnerHandler(BlockType::Spawner);
	constexpr cBlockSpongeHandler                          BlockSpongeHandler(BlockType::Sponge);
	constexpr cBlockButtonHandler                          BlockSpruceButtonHandler(BlockType::SpruceButton);
	constexpr cBlockDoorHandler                            BlockSpruceDoorHandler(BlockType::SpruceDoor);
	constexpr cBlockFenceHandler                           BlockSpruceFenceHandler(BlockType::SpruceFence);
	constexpr cBlockFenceGateHandler                       BlockSpruceFenceGateHandler(BlockType::SpruceFenceGate);
	constexpr cBlockLeavesHandler                          BlockSpruceLeavesHandler(BlockType::SpruceLeaves);
	constexpr cBlockLogHandler                             BlockSpruceLogHandler(BlockType::SpruceLog);
	constexpr cBlockPlanksHandler                          BlockSprucePlanksHandler(BlockType::SprucePlanks);
	constexpr cBlockPressurePlateHandler                   BlockSprucePressurePlateHandler(BlockType::SprucePressurePlate);
	constexpr cBlockSaplingHandler                         BlockSpruceSaplingHandler(BlockType::SpruceSapling);
	constexpr cBlockSignPostHandler                        BlockSpruceSignHandler(BlockType::SpruceSign);
	constexpr cBlockSlabHandler                            BlockSpruceSlabHandler(BlockType::SpruceSlab);
	constexpr cBlockStairsHandler                          BlockSpruceStairsHandler(BlockType::SpruceStairs);
	constexpr cBlockTrapdoorHandler                        BlockSpruceTrapdoorHandler(BlockType::SpruceTrapdoor);
	constexpr cBlockWallSignHandler                        BlockSpruceWallSignHandler(BlockType::SpruceWallSign);
	constexpr cDefaultBlockHandler                         BlockSpruceWoodHandler(BlockType::SpruceWood);
	constexpr cBlockPistonHandler                          BlockStickyPistonHandler(BlockType::StickyPiston);
	constexpr cBlockStoneHandler                           BlockStoneHandler(BlockType::Stone);
	constexpr cBlockSlabHandler                            BlockStoneBrickSlabHandler(BlockType::StoneBrickSlab);
	constexpr cBlockStairsHandler                          BlockStoneBrickStairsHandler(BlockType::StoneBrickStairs);
	constexpr cDefaultBlockHandler                         BlockStoneBrickWallHandler(BlockType::StoneBrickWall);
	constexpr cDefaultBlockHandler                         BlockStoneBricksHandler(BlockType::StoneBricks);
	constexpr cBlockButtonHandler                          BlockStoneButtonHandler(BlockType::StoneButton);
	constexpr cBlockPressurePlateHandler                   BlockStonePressurePlateHandler(BlockType::StonePressurePlate);
	constexpr cBlockSlabHandler                            BlockStoneSlabHandler(BlockType::StoneSlab);
	constexpr cBlockStairsHandler                          BlockStoneStairsHandler(BlockType::StoneStairs);
	constexpr cDefaultBlockHandler                         BlockStonecutterHandler(BlockType::Stonecutter);
	constexpr cDefaultBlockHandler                         BlockStrippedAcaciaLogHandler(BlockType::StrippedAcaciaLog);
	constexpr cDefaultBlockHandler                         BlockStrippedAcaciaWoodHandler(BlockType::StrippedAcaciaWood);
	constexpr cDefaultBlockHandler                         BlockStrippedBirchLogHandler(BlockType::StrippedBirchLog);
	constexpr cDefaultBlockHandler                         BlockStrippedBirchWoodHandler(BlockType::StrippedBirchWood);
	constexpr cDefaultBlockHandler                         BlockStrippedCrimsonHyphaeHandler(BlockType::StrippedCrimsonHyphae);
	constexpr cDefaultBlockHandler                         BlockStrippedCrimsonStemHandler(BlockType::StrippedCrimsonStem);
	constexpr cDefaultBlockHandler                         BlockStrippedDarkOakLogHandler(BlockType::StrippedDarkOakLog);
	constexpr cDefaultBlockHandler                         BlockStrippedDarkOakWoodHandler(BlockType::StrippedDarkOakWood);
	constexpr cDefaultBlockHandler                         BlockStrippedJungleLogHandler(BlockType::StrippedJungleLog);
	constexpr cDefaultBlockHandler                         BlockStrippedJungleWoodHandler(BlockType::StrippedJungleWood);
	constexpr cDefaultBlockHandler                         BlockStrippedOakLogHandler(BlockType::StrippedOakLog);
	constexpr cDefaultBlockHandler                         BlockStrippedOakWoodHandler(BlockType::StrippedOakWood);
	constexpr cDefaultBlockHandler                         BlockStrippedSpruceLogHandler(BlockType::StrippedSpruceLog);
	constexpr cDefaultBlockHandler                         BlockStrippedSpruceWoodHandler(BlockType::StrippedSpruceWood);
	constexpr cDefaultBlockHandler                         BlockStrippedWarpedHyphaeHandler(BlockType::StrippedWarpedHyphae);
	constexpr cDefaultBlockHandler                         BlockStrippedWarpedStemHandler(BlockType::StrippedWarpedStem);
	constexpr cDefaultBlockHandler                         BlockStructureBlockHandler(BlockType::StructureBlock);
	constexpr cDefaultBlockHandler                         BlockStructureVoidHandler(BlockType::StructureVoid);
	constexpr cBlockSugarCaneHandler                       BlockSugarCaneHandler(BlockType::SugarCane);
	constexpr cBlockBigFlowerHandler                       BlockSunflowerHandler(BlockType::Sunflower);
	constexpr cDefaultBlockHandler                         BlockSweetBerryBushHandler(BlockType::SweetBerryBush);
	constexpr cBlockTallGrassHandler                       BlockTallGrassHandler(BlockType::TallGrass);
	constexpr cDefaultBlockHandler                         BlockTallSeagrassHandler(BlockType::TallSeagrass);
	constexpr cDefaultBlockHandler                         BlockTargetHandler(BlockType::Target);
	constexpr cDefaultBlockHandler                         BlockTerracottaHandler(BlockType::Terracotta);
	constexpr cBlockTorchHandler                           BlockTorchHandler(BlockType::Torch);
	constexpr cBlockChestHandler                           BlockTrappedChestHandler(BlockType::TrappedChest);
	constexpr cBlockTripwireHandler                        BlockTripwireHandler(BlockType::Tripwire);
	constexpr cBlockTripwireHookHandler                    BlockTripwireHookHandler(BlockType::TripwireHook);
	constexpr cDefaultBlockHandler                         BlockTubeCoralHandler(BlockType::TubeCoral);
	constexpr cDefaultBlockHandler                         BlockTubeCoralBlockHandler(BlockType::TubeCoralBlock);
	constexpr cDefaultBlockHandler                         BlockTubeCoralFanHandler(BlockType::TubeCoralFan);
	constexpr cDefaultBlockHandler                         BlockTubeCoralWallFanHandler(BlockType::TubeCoralWallFan);
	constexpr cDefaultBlockHandler                         BlockTurtleEggHandler(BlockType::TurtleEgg);
	constexpr cDefaultBlockHandler                         BlockTwistingVinesHandler(BlockType::TwistingVines);
	constexpr cDefaultBlockHandler                         BlockTwistingVinesPlantHandler(BlockType::TwistingVinesPlant);
	constexpr cBlockVinesHandler                           BlockVineHandler(BlockType::Vine);
	constexpr cBlockAirHandler                             BlockVoidAirHandler(BlockType::VoidAir);
	constexpr cBlockTorchHandler                           BlockWallTorchHandler(BlockType::WallTorch);
	constexpr cBlockButtonHandler                          BlockWarpedButtonHandler(BlockType::WarpedButton);
	constexpr cBlockDoorHandler                            BlockWarpedDoorHandler(BlockType::WarpedDoor);
	constexpr cBlockFenceHandler                           BlockWarpedFenceHandler(BlockType::WarpedFence);
	constexpr cBlockFenceGateHandler                       BlockWarpedFenceGateHandler(BlockType::WarpedFenceGate);
	constexpr cDefaultBlockHandler                         BlockWarpedFungusHandler(BlockType::WarpedFungus);
	constexpr cDefaultBlockHandler                         BlockWarpedHyphaeHandler(BlockType::WarpedHyphae);
	constexpr cDefaultBlockHandler                         BlockWarpedNyliumHandler(BlockType::WarpedNylium);
	constexpr cBlockPlanksHandler                          BlockWarpedPlanksHandler(BlockType::WarpedPlanks);
	constexpr cBlockPressurePlateHandler                   BlockWarpedPressurePlateHandler(BlockType::WarpedPressurePlate);
	constexpr cDefaultBlockHandler                         BlockWarpedRootsHandler(BlockType::WarpedRoots);
	constexpr cBlockSignPostHandler                        BlockWarpedSignHandler(BlockType::WarpedSign);
	constexpr cBlockSlabHandler                            BlockWarpedSlabHandler(BlockType::WarpedSlab);
	constexpr cBlockStairsHandler                          BlockWarpedStairsHandler(BlockType::WarpedStairs);
	constexpr cDefaultBlockHandler                         BlockWarpedStemHandler(BlockType::WarpedStem);
	constexpr cBlockTrapdoorHandler                        BlockWarpedTrapdoorHandler(BlockType::WarpedTrapdoor);
	constexpr cBlockWallSignHandler                        BlockWarpedWallSignHandler(BlockType::WarpedWallSign);
	constexpr cDefaultBlockHandler                         BlockWarpedWartBlockHandler(BlockType::WarpedWartBlock);
	constexpr cBlockWaterHandler                           BlockWaterHandler(BlockType::Water);
	constexpr cDefaultBlockHandler                         BlockWeepingVinesHandler(BlockType::WeepingVines);
	constexpr cDefaultBlockHandler                         BlockWeepingVinesPlantHandler(BlockType::WeepingVinesPlant);
	constexpr cBlockSpongeHandler                          BlockWetSpongeHandler(BlockType::WetSponge);
	constexpr cBlockCropsHandler                           BlockWheatHandler(BlockType::Wheat);
	constexpr cDefaultBlockHandler                         BlockWhiteBannerHandler(BlockType::WhiteBanner);
	constexpr cBlockBedHandler                             BlockWhiteBedHandler(BlockType::WhiteBed);
	constexpr cBlockCarpetHandler                          BlockWhiteCarpetHandler(BlockType::WhiteCarpet);
	constexpr cDefaultBlockHandler                         BlockWhiteConcreteHandler(BlockType::WhiteConcrete);
	constexpr cBlockConcretePowderHandler                  BlockWhiteConcretePowderHandler(BlockType::WhiteConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockWhiteGlazedTerracottaHandler(BlockType::WhiteGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockWhiteShulkerBoxHandler(BlockType::WhiteShulkerBox);
	constexpr cBlockGlassHandler                           BlockWhiteStainedGlassHandler(BlockType::WhiteStainedGlass);
	constexpr cBlockGlassHandler                           BlockWhiteStainedGlassPaneHandler(BlockType::WhiteStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockWhiteTerracottaHandler(BlockType::WhiteTerracotta);
	constexpr cBlockFlowerHandler                          BlockWhiteTulipHandler(BlockType::WhiteTulip);
	constexpr cDefaultBlockHandler                         BlockWhiteWallBannerHandler(BlockType::WhiteWallBanner);
	constexpr cBlockWoolHandler                            BlockWhiteWoolHandler(BlockType::WhiteWool);
	constexpr cBlockFlowerHandler                          BlockWitherRoseHandler(BlockType::WitherRose);
	constexpr cBlockMobHeadHandler                         BlockWitherSkeletonSkullHandler(BlockType::WitherSkeletonSkull);
	constexpr cBlockMobHeadHandler                         BlockWitherSkeletonWallSkullHandler(BlockType::WitherSkeletonWallSkull);
	constexpr cDefaultBlockHandler                         BlockYellowBannerHandler(BlockType::YellowBanner);
	constexpr cBlockBedHandler                             BlockYellowBedHandler(BlockType::YellowBed);
	constexpr cBlockCarpetHandler                          BlockYellowCarpetHandler(BlockType::YellowCarpet);
	constexpr cDefaultBlockHandler                         BlockYellowConcreteHandler(BlockType::YellowConcrete);
	constexpr cBlockConcretePowderHandler                  BlockYellowConcretePowderHandler(BlockType::YellowConcretePowder);
	constexpr cBlockGlazedTerracottaHandler                BlockYellowGlazedTerracottaHandler(BlockType::YellowGlazedTerracotta);
	constexpr cDefaultBlockHandler                         BlockYellowShulkerBoxHandler(BlockType::YellowShulkerBox);
	constexpr cBlockGlassHandler                           BlockYellowStainedGlassHandler(BlockType::YellowStainedGlass);
	constexpr cBlockGlassHandler                           BlockYellowStainedGlassPaneHandler(BlockType::YellowStainedGlassPane);
	constexpr cDefaultBlockHandler                         BlockYellowTerracottaHandler(BlockType::YellowTerracotta);
	constexpr cDefaultBlockHandler                         BlockYellowWallBannerHandler(BlockType::YellowWallBanner);
	constexpr cBlockWoolHandler                            BlockYellowWoolHandler(BlockType::YellowWool);
	constexpr cBlockMobHeadHandler                         BlockZombieHeadHandler(BlockType::ZombieHead);
	constexpr cBlockMobHeadHandler                         BlockZombieWallHeadHandler(BlockType::ZombieWallHead);
	constexpr cBlockSignPostHandler                         BlockAcaciaHangingSignHandler(BlockType::AcaciaHangingSign);
	constexpr cBlockSignPostHandler                         BlockAcaciaWallHangingSignHandler(BlockType::AcaciaWallHangingSign);
	constexpr cDefaultBlockHandler                          BlockAmethystBlockHandler(BlockType::AmethystBlock);
	constexpr cDefaultBlockHandler                          BlockAmethystClusterHandler(BlockType::AmethystCluster);
	constexpr cDefaultBlockHandler                          BlockAzaleaHandler(BlockType::Azalea);
	constexpr cBlockLeavesHandler                           BlockAzaleaLeavesHandler(BlockType::AzaleaLeaves);
	constexpr cDefaultBlockHandler                          BlockBambooBlockHandler(BlockType::BambooBlock);
	constexpr cBlockButtonHandler                           BlockBambooButtonHandler(BlockType::BambooButton);
	constexpr cBlockDoorHandler                             BlockBambooDoorHandler(BlockType::BambooDoor);
	constexpr cBlockFenceHandler                            BlockBambooFenceHandler(BlockType::BambooFence);
	constexpr cBlockFenceGateHandler                        BlockBambooFenceGateHandler(BlockType::BambooFenceGate);
	constexpr cBlockSignPostHandler                         BlockBambooHangingSignHandler(BlockType::BambooHangingSign);
	constexpr cDefaultBlockHandler                          BlockBambooMosaicHandler(BlockType::BambooMosaic);
	constexpr cBlockSlabHandler                             BlockBambooMosaicSlabHandler(BlockType::BambooMosaicSlab);
	constexpr cBlockStairsHandler                           BlockBambooMosaicStairsHandler(BlockType::BambooMosaicStairs);
	constexpr cDefaultBlockHandler                          BlockBambooPlanksHandler(BlockType::BambooPlanks);
	constexpr cBlockPressurePlateHandler                    BlockBambooPressurePlateHandler(BlockType::BambooPressurePlate);
	constexpr cBlockSignPostHandler                         BlockBambooSignHandler(BlockType::BambooSign);
	constexpr cBlockSlabHandler                             BlockBambooSlabHandler(BlockType::BambooSlab);
	constexpr cBlockStairsHandler                           BlockBambooStairsHandler(BlockType::BambooStairs);
	constexpr cBlockTrapdoorHandler                         BlockBambooTrapdoorHandler(BlockType::BambooTrapdoor);
	constexpr cBlockSignPostHandler                         BlockBambooWallHangingSignHandler(BlockType::BambooWallHangingSign);
	constexpr cBlockSignPostHandler                         BlockBambooWallSignHandler(BlockType::BambooWallSign);
	constexpr cDefaultBlockHandler                          BlockBigDripleafHandler(BlockType::BigDripleaf);
	constexpr cDefaultBlockHandler                          BlockBigDripleafStemHandler(BlockType::BigDripleafStem);
	constexpr cBlockSignPostHandler                         BlockBirchHangingSignHandler(BlockType::BirchHangingSign);
	constexpr cBlockSignPostHandler                         BlockBirchWallHangingSignHandler(BlockType::BirchWallHangingSign);
	constexpr cDefaultBlockHandler                          BlockBlackCandleHandler(BlockType::BlackCandle);
	constexpr cDefaultBlockHandler                          BlockBlackCandleCakeHandler(BlockType::BlackCandleCake);
	constexpr cDefaultBlockHandler                          BlockBlueCandleHandler(BlockType::BlueCandle);
	constexpr cDefaultBlockHandler                          BlockBlueCandleCakeHandler(BlockType::BlueCandleCake);
	constexpr cDefaultBlockHandler                          BlockBrownCandleHandler(BlockType::BrownCandle);
	constexpr cDefaultBlockHandler                          BlockBrownCandleCakeHandler(BlockType::BrownCandleCake);
	constexpr cDefaultBlockHandler                          BlockBuddingAmethystHandler(BlockType::BuddingAmethyst);
	constexpr cDefaultBlockHandler                          BlockCalciteHandler(BlockType::Calcite);
	constexpr cDefaultBlockHandler                          BlockCalibratedSculkSensorHandler(BlockType::CalibratedSculkSensor);
	constexpr cDefaultBlockHandler                          BlockCandleHandler(BlockType::Candle);
	constexpr cDefaultBlockHandler                          BlockCandleCakeHandler(BlockType::CandleCake);
	constexpr cDefaultBlockHandler                          BlockCaveVinesHandler(BlockType::CaveVines);
	constexpr cDefaultBlockHandler                          BlockCaveVinesPlantHandler(BlockType::CaveVinesPlant);
	constexpr cBlockButtonHandler                           BlockCherryButtonHandler(BlockType::CherryButton);
	constexpr cBlockDoorHandler                             BlockCherryDoorHandler(BlockType::CherryDoor);
	constexpr cBlockFenceHandler                            BlockCherryFenceHandler(BlockType::CherryFence);
	constexpr cBlockFenceGateHandler                        BlockCherryFenceGateHandler(BlockType::CherryFenceGate);
	constexpr cBlockSignPostHandler                         BlockCherryHangingSignHandler(BlockType::CherryHangingSign);
	constexpr cBlockLeavesHandler                           BlockCherryLeavesHandler(BlockType::CherryLeaves);
	constexpr cBlockLogHandler                              BlockCherryLogHandler(BlockType::CherryLog);
	constexpr cDefaultBlockHandler                          BlockCherryPlanksHandler(BlockType::CherryPlanks);
	constexpr cBlockPressurePlateHandler                    BlockCherryPressurePlateHandler(BlockType::CherryPressurePlate);
	constexpr cDefaultBlockHandler                          BlockCherrySaplingHandler(BlockType::CherrySapling);
	constexpr cBlockSignPostHandler                         BlockCherrySignHandler(BlockType::CherrySign);
	constexpr cBlockSlabHandler                             BlockCherrySlabHandler(BlockType::CherrySlab);
	constexpr cBlockStairsHandler                           BlockCherryStairsHandler(BlockType::CherryStairs);
	constexpr cBlockTrapdoorHandler                         BlockCherryTrapdoorHandler(BlockType::CherryTrapdoor);
	constexpr cBlockSignPostHandler                         BlockCherryWallHangingSignHandler(BlockType::CherryWallHangingSign);
	constexpr cBlockSignPostHandler                         BlockCherryWallSignHandler(BlockType::CherryWallSign);
	constexpr cDefaultBlockHandler                          BlockCherryWoodHandler(BlockType::CherryWood);
	constexpr cDefaultBlockHandler                          BlockChiseledBookshelfHandler(BlockType::ChiseledBookshelf);
	constexpr cDefaultBlockHandler                          BlockChiseledCopperHandler(BlockType::ChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockChiseledDeepslateHandler(BlockType::ChiseledDeepslate);
	constexpr cDefaultBlockHandler                          BlockChiseledTuffHandler(BlockType::ChiseledTuff);
	constexpr cDefaultBlockHandler                          BlockChiseledTuffBricksHandler(BlockType::ChiseledTuffBricks);
	constexpr cDefaultBlockHandler                          BlockCobbledDeepslateHandler(BlockType::CobbledDeepslate);
	constexpr cBlockSlabHandler                             BlockCobbledDeepslateSlabHandler(BlockType::CobbledDeepslateSlab);
	constexpr cBlockStairsHandler                           BlockCobbledDeepslateStairsHandler(BlockType::CobbledDeepslateStairs);
	constexpr cDefaultBlockHandler                          BlockCobbledDeepslateWallHandler(BlockType::CobbledDeepslateWall);
	constexpr cDefaultBlockHandler                          BlockCopperBlockHandler(BlockType::CopperBlock);
	constexpr cDefaultBlockHandler                          BlockCopperBulbHandler(BlockType::CopperBulb);
	constexpr cBlockDoorHandler                             BlockCopperDoorHandler(BlockType::CopperDoor);
	constexpr cDefaultBlockHandler                          BlockCopperGrateHandler(BlockType::CopperGrate);
	constexpr cDefaultBlockHandler                          BlockCopperOreHandler(BlockType::CopperOre);
	constexpr cBlockTrapdoorHandler                         BlockCopperTrapdoorHandler(BlockType::CopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockCrackedDeepslateBricksHandler(BlockType::CrackedDeepslateBricks);
	constexpr cDefaultBlockHandler                          BlockCrackedDeepslateTilesHandler(BlockType::CrackedDeepslateTiles);
	constexpr cDefaultBlockHandler                          BlockCrafterHandler(BlockType::Crafter);
	constexpr cBlockSignPostHandler                         BlockCrimsonHangingSignHandler(BlockType::CrimsonHangingSign);
	constexpr cBlockSignPostHandler                         BlockCrimsonWallHangingSignHandler(BlockType::CrimsonWallHangingSign);
	constexpr cDefaultBlockHandler                          BlockCutCopperHandler(BlockType::CutCopper);
	constexpr cBlockSlabHandler                             BlockCutCopperSlabHandler(BlockType::CutCopperSlab);
	constexpr cBlockStairsHandler                           BlockCutCopperStairsHandler(BlockType::CutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockCyanCandleHandler(BlockType::CyanCandle);
	constexpr cDefaultBlockHandler                          BlockCyanCandleCakeHandler(BlockType::CyanCandleCake);
	constexpr cBlockSignPostHandler                         BlockDarkOakHangingSignHandler(BlockType::DarkOakHangingSign);
	constexpr cBlockSignPostHandler                         BlockDarkOakWallHangingSignHandler(BlockType::DarkOakWallHangingSign);
	constexpr cDefaultBlockHandler                          BlockDecoratedPotHandler(BlockType::DecoratedPot);
	constexpr cDefaultBlockHandler                          BlockDeepslateHandler(BlockType::Deepslate);
	constexpr cBlockSlabHandler                             BlockDeepslateBrickSlabHandler(BlockType::DeepslateBrickSlab);
	constexpr cBlockStairsHandler                           BlockDeepslateBrickStairsHandler(BlockType::DeepslateBrickStairs);
	constexpr cDefaultBlockHandler                          BlockDeepslateBrickWallHandler(BlockType::DeepslateBrickWall);
	constexpr cDefaultBlockHandler                          BlockDeepslateBricksHandler(BlockType::DeepslateBricks);
	constexpr cDefaultOreHandler                            BlockDeepslateCoalOreHandler(BlockType::DeepslateCoalOre);
	constexpr cDefaultOreHandler                            BlockDeepslateCopperOreHandler(BlockType::DeepslateCopperOre);
	constexpr cDefaultOreHandler                            BlockDeepslateDiamondOreHandler(BlockType::DeepslateDiamondOre);
	constexpr cDefaultOreHandler                            BlockDeepslateEmeraldOreHandler(BlockType::DeepslateEmeraldOre);
	constexpr cDefaultOreHandler                            BlockDeepslateGoldOreHandler(BlockType::DeepslateGoldOre);
	constexpr cDefaultOreHandler                            BlockDeepslateIronOreHandler(BlockType::DeepslateIronOre);
	constexpr cDefaultOreHandler                            BlockDeepslateLapisOreHandler(BlockType::DeepslateLapisOre);
	constexpr cDefaultOreHandler                            BlockDeepslateRedstoneOreHandler(BlockType::DeepslateRedstoneOre);
	constexpr cBlockSlabHandler                             BlockDeepslateTileSlabHandler(BlockType::DeepslateTileSlab);
	constexpr cBlockStairsHandler                           BlockDeepslateTileStairsHandler(BlockType::DeepslateTileStairs);
	constexpr cDefaultBlockHandler                          BlockDeepslateTileWallHandler(BlockType::DeepslateTileWall);
	constexpr cDefaultBlockHandler                          BlockDeepslateTilesHandler(BlockType::DeepslateTiles);
	constexpr cDefaultBlockHandler                          BlockDirtPathHandler(BlockType::DirtPath);
	constexpr cDefaultBlockHandler                          BlockDripstoneBlockHandler(BlockType::DripstoneBlock);
	constexpr cDefaultBlockHandler                          BlockExposedChiseledCopperHandler(BlockType::ExposedChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockExposedCopperHandler(BlockType::ExposedCopper);
	constexpr cDefaultBlockHandler                          BlockExposedCopperBulbHandler(BlockType::ExposedCopperBulb);
	constexpr cBlockDoorHandler                             BlockExposedCopperDoorHandler(BlockType::ExposedCopperDoor);
	constexpr cDefaultBlockHandler                          BlockExposedCopperGrateHandler(BlockType::ExposedCopperGrate);
	constexpr cBlockTrapdoorHandler                         BlockExposedCopperTrapdoorHandler(BlockType::ExposedCopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockExposedCutCopperHandler(BlockType::ExposedCutCopper);
	constexpr cBlockSlabHandler                             BlockExposedCutCopperSlabHandler(BlockType::ExposedCutCopperSlab);
	constexpr cBlockStairsHandler                           BlockExposedCutCopperStairsHandler(BlockType::ExposedCutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockFloweringAzaleaHandler(BlockType::FloweringAzalea);
	constexpr cBlockLeavesHandler                           BlockFloweringAzaleaLeavesHandler(BlockType::FloweringAzaleaLeaves);
	constexpr cDefaultBlockHandler                          BlockFrogspawnHandler(BlockType::Frogspawn);
	constexpr cDefaultBlockHandler                          BlockGlowLichenHandler(BlockType::GlowLichen);
	constexpr cDefaultBlockHandler                          BlockGrayCandleHandler(BlockType::GrayCandle);
	constexpr cDefaultBlockHandler                          BlockGrayCandleCakeHandler(BlockType::GrayCandleCake);
	constexpr cDefaultBlockHandler                          BlockGreenCandleHandler(BlockType::GreenCandle);
	constexpr cDefaultBlockHandler                          BlockGreenCandleCakeHandler(BlockType::GreenCandleCake);
	constexpr cDefaultBlockHandler                          BlockHangingRootsHandler(BlockType::HangingRoots);
	constexpr cBlockHayBaleHandler                          BlockHayBlockHandler(BlockType::HayBlock);
	constexpr cDefaultBlockHandler                          BlockHeavyCoreHandler(BlockType::HeavyCore);
	constexpr cDefaultBlockHandler                          BlockInfestedDeepslateHandler(BlockType::InfestedDeepslate);
	constexpr cBlockSignPostHandler                         BlockJungleHangingSignHandler(BlockType::JungleHangingSign);
	constexpr cBlockSignPostHandler                         BlockJungleWallHangingSignHandler(BlockType::JungleWallHangingSign);
	constexpr cDefaultBlockHandler                          BlockLargeAmethystBudHandler(BlockType::LargeAmethystBud);
	constexpr cBlockCauldronHandler                         BlockLavaCauldronHandler(BlockType::LavaCauldron);
	constexpr cDefaultBlockHandler                          BlockLightHandler(BlockType::Light);
	constexpr cDefaultBlockHandler                          BlockLightBlueCandleHandler(BlockType::LightBlueCandle);
	constexpr cDefaultBlockHandler                          BlockLightBlueCandleCakeHandler(BlockType::LightBlueCandleCake);
	constexpr cDefaultBlockHandler                          BlockLightGrayCandleHandler(BlockType::LightGrayCandle);
	constexpr cDefaultBlockHandler                          BlockLightGrayCandleCakeHandler(BlockType::LightGrayCandleCake);
	constexpr cDefaultBlockHandler                          BlockLightningRodHandler(BlockType::LightningRod);
	constexpr cDefaultBlockHandler                          BlockLimeCandleHandler(BlockType::LimeCandle);
	constexpr cDefaultBlockHandler                          BlockLimeCandleCakeHandler(BlockType::LimeCandleCake);
	constexpr cDefaultBlockHandler                          BlockMagentaCandleHandler(BlockType::MagentaCandle);
	constexpr cDefaultBlockHandler                          BlockMagentaCandleCakeHandler(BlockType::MagentaCandleCake);
	constexpr cBlockButtonHandler                           BlockMangroveButtonHandler(BlockType::MangroveButton);
	constexpr cBlockDoorHandler                             BlockMangroveDoorHandler(BlockType::MangroveDoor);
	constexpr cBlockFenceHandler                            BlockMangroveFenceHandler(BlockType::MangroveFence);
	constexpr cBlockFenceGateHandler                        BlockMangroveFenceGateHandler(BlockType::MangroveFenceGate);
	constexpr cBlockSignPostHandler                         BlockMangroveHangingSignHandler(BlockType::MangroveHangingSign);
	constexpr cBlockLeavesHandler                           BlockMangroveLeavesHandler(BlockType::MangroveLeaves);
	constexpr cBlockLogHandler                              BlockMangroveLogHandler(BlockType::MangroveLog);
	constexpr cDefaultBlockHandler                          BlockMangrovePlanksHandler(BlockType::MangrovePlanks);
	constexpr cBlockPressurePlateHandler                    BlockMangrovePressurePlateHandler(BlockType::MangrovePressurePlate);
	constexpr cDefaultBlockHandler                          BlockMangrovePropaguleHandler(BlockType::MangrovePropagule);
	constexpr cDefaultBlockHandler                          BlockMangroveRootsHandler(BlockType::MangroveRoots);
	constexpr cBlockSignPostHandler                         BlockMangroveSignHandler(BlockType::MangroveSign);
	constexpr cBlockSlabHandler                             BlockMangroveSlabHandler(BlockType::MangroveSlab);
	constexpr cBlockStairsHandler                           BlockMangroveStairsHandler(BlockType::MangroveStairs);
	constexpr cBlockTrapdoorHandler                         BlockMangroveTrapdoorHandler(BlockType::MangroveTrapdoor);
	constexpr cBlockSignPostHandler                         BlockMangroveWallHangingSignHandler(BlockType::MangroveWallHangingSign);
	constexpr cBlockSignPostHandler                         BlockMangroveWallSignHandler(BlockType::MangroveWallSign);
	constexpr cDefaultBlockHandler                          BlockMangroveWoodHandler(BlockType::MangroveWood);
	constexpr cDefaultBlockHandler                          BlockMediumAmethystBudHandler(BlockType::MediumAmethystBud);
	constexpr cDefaultBlockHandler                          BlockMossBlockHandler(BlockType::MossBlock);
	constexpr cDefaultBlockHandler                          BlockMossCarpetHandler(BlockType::MossCarpet);
	constexpr cDefaultBlockHandler                          BlockMudHandler(BlockType::Mud);
	constexpr cBlockSlabHandler                             BlockMudBrickSlabHandler(BlockType::MudBrickSlab);
	constexpr cBlockStairsHandler                           BlockMudBrickStairsHandler(BlockType::MudBrickStairs);
	constexpr cDefaultBlockHandler                          BlockMudBrickWallHandler(BlockType::MudBrickWall);
	constexpr cDefaultBlockHandler                          BlockMudBricksHandler(BlockType::MudBricks);
	constexpr cDefaultBlockHandler                          BlockMuddyMangroveRootsHandler(BlockType::MuddyMangroveRoots);
	constexpr cBlockSignPostHandler                         BlockOakHangingSignHandler(BlockType::OakHangingSign);
	constexpr cBlockSignPostHandler                         BlockOakWallHangingSignHandler(BlockType::OakWallHangingSign);
	constexpr cDefaultBlockHandler                          BlockOchreFroglightHandler(BlockType::OchreFroglight);
	constexpr cDefaultBlockHandler                          BlockOrangeCandleHandler(BlockType::OrangeCandle);
	constexpr cDefaultBlockHandler                          BlockOrangeCandleCakeHandler(BlockType::OrangeCandleCake);
	constexpr cDefaultBlockHandler                          BlockOxidizedChiseledCopperHandler(BlockType::OxidizedChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockOxidizedCopperHandler(BlockType::OxidizedCopper);
	constexpr cDefaultBlockHandler                          BlockOxidizedCopperBulbHandler(BlockType::OxidizedCopperBulb);
	constexpr cBlockDoorHandler                             BlockOxidizedCopperDoorHandler(BlockType::OxidizedCopperDoor);
	constexpr cDefaultBlockHandler                          BlockOxidizedCopperGrateHandler(BlockType::OxidizedCopperGrate);
	constexpr cBlockTrapdoorHandler                         BlockOxidizedCopperTrapdoorHandler(BlockType::OxidizedCopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockOxidizedCutCopperHandler(BlockType::OxidizedCutCopper);
	constexpr cBlockSlabHandler                             BlockOxidizedCutCopperSlabHandler(BlockType::OxidizedCutCopperSlab);
	constexpr cBlockStairsHandler                           BlockOxidizedCutCopperStairsHandler(BlockType::OxidizedCutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockPackedMudHandler(BlockType::PackedMud);
	constexpr cDefaultBlockHandler                          BlockPearlescentFroglightHandler(BlockType::PearlescentFroglight);
	constexpr cDefaultBlockHandler                          BlockPiglinHeadHandler(BlockType::PiglinHead);
	constexpr cDefaultBlockHandler                          BlockPiglinWallHeadHandler(BlockType::PiglinWallHead);
	constexpr cDefaultBlockHandler                          BlockPinkCandleHandler(BlockType::PinkCandle);
	constexpr cDefaultBlockHandler                          BlockPinkCandleCakeHandler(BlockType::PinkCandleCake);
	constexpr cDefaultBlockHandler                          BlockPinkPetalsHandler(BlockType::PinkPetals);
	constexpr cDefaultBlockHandler                          BlockPitcherCropHandler(BlockType::PitcherCrop);
	constexpr cDefaultBlockHandler                          BlockPitcherPlantHandler(BlockType::PitcherPlant);
	constexpr cDefaultBlockHandler                          BlockPointedDripstoneHandler(BlockType::PointedDripstone);
	constexpr cDefaultBlockHandler                          BlockPolishedDeepslateHandler(BlockType::PolishedDeepslate);
	constexpr cBlockSlabHandler                             BlockPolishedDeepslateSlabHandler(BlockType::PolishedDeepslateSlab);
	constexpr cBlockStairsHandler                           BlockPolishedDeepslateStairsHandler(BlockType::PolishedDeepslateStairs);
	constexpr cDefaultBlockHandler                          BlockPolishedDeepslateWallHandler(BlockType::PolishedDeepslateWall);
	constexpr cDefaultBlockHandler                          BlockPolishedTuffHandler(BlockType::PolishedTuff);
	constexpr cBlockSlabHandler                             BlockPolishedTuffSlabHandler(BlockType::PolishedTuffSlab);
	constexpr cBlockStairsHandler                           BlockPolishedTuffStairsHandler(BlockType::PolishedTuffStairs);
	constexpr cDefaultBlockHandler                          BlockPolishedTuffWallHandler(BlockType::PolishedTuffWall);
	constexpr cDefaultBlockHandler                          BlockPottedAzaleaBushHandler(BlockType::PottedAzaleaBush);
	constexpr cDefaultBlockHandler                          BlockPottedCherrySaplingHandler(BlockType::PottedCherrySapling);
	constexpr cDefaultBlockHandler                          BlockPottedFloweringAzaleaBushHandler(BlockType::PottedFloweringAzaleaBush);
	constexpr cDefaultBlockHandler                          BlockPottedMangrovePropaguleHandler(BlockType::PottedMangrovePropagule);
	constexpr cDefaultBlockHandler                          BlockPottedTorchflowerHandler(BlockType::PottedTorchflower);
	constexpr cDefaultBlockHandler                          BlockPowderSnowHandler(BlockType::PowderSnow);
	constexpr cBlockCauldronHandler                         BlockPowderSnowCauldronHandler(BlockType::PowderSnowCauldron);
	constexpr cDefaultBlockHandler                          BlockPurpleCandleHandler(BlockType::PurpleCandle);
	constexpr cDefaultBlockHandler                          BlockPurpleCandleCakeHandler(BlockType::PurpleCandleCake);
	constexpr cDefaultBlockHandler                          BlockRawCopperBlockHandler(BlockType::RawCopperBlock);
	constexpr cDefaultBlockHandler                          BlockRawGoldBlockHandler(BlockType::RawGoldBlock);
	constexpr cDefaultBlockHandler                          BlockRawIronBlockHandler(BlockType::RawIronBlock);
	constexpr cDefaultBlockHandler                          BlockRedCandleHandler(BlockType::RedCandle);
	constexpr cDefaultBlockHandler                          BlockRedCandleCakeHandler(BlockType::RedCandleCake);
	constexpr cDefaultBlockHandler                          BlockReinforcedDeepslateHandler(BlockType::ReinforcedDeepslate);
	constexpr cDefaultBlockHandler                          BlockRootedDirtHandler(BlockType::RootedDirt);
	constexpr cDefaultBlockHandler                          BlockSculkHandler(BlockType::Sculk);
	constexpr cDefaultBlockHandler                          BlockSculkCatalystHandler(BlockType::SculkCatalyst);
	constexpr cDefaultBlockHandler                          BlockSculkSensorHandler(BlockType::SculkSensor);
	constexpr cDefaultBlockHandler                          BlockSculkShriekerHandler(BlockType::SculkShrieker);
	constexpr cDefaultBlockHandler                          BlockSculkVeinHandler(BlockType::SculkVein);
	constexpr cDefaultBlockHandler                          BlockShortGrassHandler(BlockType::ShortGrass);
	constexpr cDefaultBlockHandler                          BlockSmallAmethystBudHandler(BlockType::SmallAmethystBud);
	constexpr cDefaultBlockHandler                          BlockSmallDripleafHandler(BlockType::SmallDripleaf);
	constexpr cDefaultBlockHandler                          BlockSmoothBasaltHandler(BlockType::SmoothBasalt);
	constexpr cDefaultBlockHandler                          BlockSnifferEggHandler(BlockType::SnifferEgg);
	constexpr cDefaultBlockHandler                          BlockSporeBlossomHandler(BlockType::SporeBlossom);
	constexpr cBlockSignPostHandler                         BlockSpruceHangingSignHandler(BlockType::SpruceHangingSign);
	constexpr cBlockSignPostHandler                         BlockSpruceWallHangingSignHandler(BlockType::SpruceWallHangingSign);
	constexpr cDefaultBlockHandler                          BlockStrippedBambooBlockHandler(BlockType::StrippedBambooBlock);
	constexpr cBlockLogHandler                              BlockStrippedCherryLogHandler(BlockType::StrippedCherryLog);
	constexpr cDefaultBlockHandler                          BlockStrippedCherryWoodHandler(BlockType::StrippedCherryWood);
	constexpr cBlockLogHandler                              BlockStrippedMangroveLogHandler(BlockType::StrippedMangroveLog);
	constexpr cDefaultBlockHandler                          BlockStrippedMangroveWoodHandler(BlockType::StrippedMangroveWood);
	constexpr cDefaultBlockHandler                          BlockSuspiciousGravelHandler(BlockType::SuspiciousGravel);
	constexpr cDefaultBlockHandler                          BlockSuspiciousSandHandler(BlockType::SuspiciousSand);
	constexpr cDefaultBlockHandler                          BlockTintedGlassHandler(BlockType::TintedGlass);
	constexpr cBlockTNTHandler                              BlockTntHandler(BlockType::Tnt);
	constexpr cDefaultBlockHandler                          BlockTorchflowerHandler(BlockType::Torchflower);
	constexpr cDefaultBlockHandler                          BlockTorchflowerCropHandler(BlockType::TorchflowerCrop);
	constexpr cDefaultBlockHandler                          BlockTrialSpawnerHandler(BlockType::TrialSpawner);
	constexpr cDefaultBlockHandler                          BlockTuffHandler(BlockType::Tuff);
	constexpr cBlockSlabHandler                             BlockTuffBrickSlabHandler(BlockType::TuffBrickSlab);
	constexpr cBlockStairsHandler                           BlockTuffBrickStairsHandler(BlockType::TuffBrickStairs);
	constexpr cDefaultBlockHandler                          BlockTuffBrickWallHandler(BlockType::TuffBrickWall);
	constexpr cDefaultBlockHandler                          BlockTuffBricksHandler(BlockType::TuffBricks);
	constexpr cBlockSlabHandler                             BlockTuffSlabHandler(BlockType::TuffSlab);
	constexpr cBlockStairsHandler                           BlockTuffStairsHandler(BlockType::TuffStairs);
	constexpr cDefaultBlockHandler                          BlockTuffWallHandler(BlockType::TuffWall);
	constexpr cDefaultBlockHandler                          BlockVaultHandler(BlockType::Vault);
	constexpr cDefaultBlockHandler                          BlockVerdantFroglightHandler(BlockType::VerdantFroglight);
	constexpr cBlockSignPostHandler                         BlockWarpedHangingSignHandler(BlockType::WarpedHangingSign);
	constexpr cBlockSignPostHandler                         BlockWarpedWallHangingSignHandler(BlockType::WarpedWallHangingSign);
	constexpr cBlockCauldronHandler                         BlockWaterCauldronHandler(BlockType::WaterCauldron);
	constexpr cDefaultBlockHandler                          BlockWaxedChiseledCopperHandler(BlockType::WaxedChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockWaxedCopperBlockHandler(BlockType::WaxedCopperBlock);
	constexpr cDefaultBlockHandler                          BlockWaxedCopperBulbHandler(BlockType::WaxedCopperBulb);
	constexpr cBlockDoorHandler                             BlockWaxedCopperDoorHandler(BlockType::WaxedCopperDoor);
	constexpr cDefaultBlockHandler                          BlockWaxedCopperGrateHandler(BlockType::WaxedCopperGrate);
	constexpr cBlockTrapdoorHandler                         BlockWaxedCopperTrapdoorHandler(BlockType::WaxedCopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockWaxedCutCopperHandler(BlockType::WaxedCutCopper);
	constexpr cBlockSlabHandler                             BlockWaxedCutCopperSlabHandler(BlockType::WaxedCutCopperSlab);
	constexpr cBlockStairsHandler                           BlockWaxedCutCopperStairsHandler(BlockType::WaxedCutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockWaxedExposedChiseledCopperHandler(BlockType::WaxedExposedChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockWaxedExposedCopperHandler(BlockType::WaxedExposedCopper);
	constexpr cDefaultBlockHandler                          BlockWaxedExposedCopperBulbHandler(BlockType::WaxedExposedCopperBulb);
	constexpr cBlockDoorHandler                             BlockWaxedExposedCopperDoorHandler(BlockType::WaxedExposedCopperDoor);
	constexpr cDefaultBlockHandler                          BlockWaxedExposedCopperGrateHandler(BlockType::WaxedExposedCopperGrate);
	constexpr cBlockTrapdoorHandler                         BlockWaxedExposedCopperTrapdoorHandler(BlockType::WaxedExposedCopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockWaxedExposedCutCopperHandler(BlockType::WaxedExposedCutCopper);
	constexpr cBlockSlabHandler                             BlockWaxedExposedCutCopperSlabHandler(BlockType::WaxedExposedCutCopperSlab);
	constexpr cBlockStairsHandler                           BlockWaxedExposedCutCopperStairsHandler(BlockType::WaxedExposedCutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockWaxedOxidizedChiseledCopperHandler(BlockType::WaxedOxidizedChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockWaxedOxidizedCopperHandler(BlockType::WaxedOxidizedCopper);
	constexpr cDefaultBlockHandler                          BlockWaxedOxidizedCopperBulbHandler(BlockType::WaxedOxidizedCopperBulb);
	constexpr cBlockDoorHandler                             BlockWaxedOxidizedCopperDoorHandler(BlockType::WaxedOxidizedCopperDoor);
	constexpr cDefaultBlockHandler                          BlockWaxedOxidizedCopperGrateHandler(BlockType::WaxedOxidizedCopperGrate);
	constexpr cBlockTrapdoorHandler                         BlockWaxedOxidizedCopperTrapdoorHandler(BlockType::WaxedOxidizedCopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockWaxedOxidizedCutCopperHandler(BlockType::WaxedOxidizedCutCopper);
	constexpr cBlockSlabHandler                             BlockWaxedOxidizedCutCopperSlabHandler(BlockType::WaxedOxidizedCutCopperSlab);
	constexpr cBlockStairsHandler                           BlockWaxedOxidizedCutCopperStairsHandler(BlockType::WaxedOxidizedCutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockWaxedWeatheredChiseledCopperHandler(BlockType::WaxedWeatheredChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockWaxedWeatheredCopperHandler(BlockType::WaxedWeatheredCopper);
	constexpr cDefaultBlockHandler                          BlockWaxedWeatheredCopperBulbHandler(BlockType::WaxedWeatheredCopperBulb);
	constexpr cBlockDoorHandler                             BlockWaxedWeatheredCopperDoorHandler(BlockType::WaxedWeatheredCopperDoor);
	constexpr cDefaultBlockHandler                          BlockWaxedWeatheredCopperGrateHandler(BlockType::WaxedWeatheredCopperGrate);
	constexpr cBlockTrapdoorHandler                         BlockWaxedWeatheredCopperTrapdoorHandler(BlockType::WaxedWeatheredCopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockWaxedWeatheredCutCopperHandler(BlockType::WaxedWeatheredCutCopper);
	constexpr cBlockSlabHandler                             BlockWaxedWeatheredCutCopperSlabHandler(BlockType::WaxedWeatheredCutCopperSlab);
	constexpr cBlockStairsHandler                           BlockWaxedWeatheredCutCopperStairsHandler(BlockType::WaxedWeatheredCutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockWeatheredChiseledCopperHandler(BlockType::WeatheredChiseledCopper);
	constexpr cDefaultBlockHandler                          BlockWeatheredCopperHandler(BlockType::WeatheredCopper);
	constexpr cDefaultBlockHandler                          BlockWeatheredCopperBulbHandler(BlockType::WeatheredCopperBulb);
	constexpr cBlockDoorHandler                             BlockWeatheredCopperDoorHandler(BlockType::WeatheredCopperDoor);
	constexpr cDefaultBlockHandler                          BlockWeatheredCopperGrateHandler(BlockType::WeatheredCopperGrate);
	constexpr cBlockTrapdoorHandler                         BlockWeatheredCopperTrapdoorHandler(BlockType::WeatheredCopperTrapdoor);
	constexpr cDefaultBlockHandler                          BlockWeatheredCutCopperHandler(BlockType::WeatheredCutCopper);
	constexpr cBlockSlabHandler                             BlockWeatheredCutCopperSlabHandler(BlockType::WeatheredCutCopperSlab);
	constexpr cBlockStairsHandler                           BlockWeatheredCutCopperStairsHandler(BlockType::WeatheredCutCopperStairs);
	constexpr cDefaultBlockHandler                          BlockWhiteCandleHandler(BlockType::WhiteCandle);
	constexpr cDefaultBlockHandler                          BlockWhiteCandleCakeHandler(BlockType::WhiteCandleCake);
	constexpr cDefaultBlockHandler                          BlockYellowCandleHandler(BlockType::YellowCandle);
	constexpr cDefaultBlockHandler                          BlockYellowCandleCakeHandler(BlockType::YellowCandleCake);
	constexpr cDefaultBlockHandler                          BlockChiseledResinBricksHandler(BlockType::ChiseledResinBricks);
	constexpr cDefaultBlockHandler                          BlockClosedEyeblossomHandler(BlockType::ClosedEyeblossom);
	constexpr cDefaultBlockHandler                          BlockCreakingHeartHandler(BlockType::CreakingHeart);
	constexpr cDefaultBlockHandler                          BlockOpenEyeblossomHandler(BlockType::OpenEyeblossom);
	constexpr cDefaultBlockHandler                          BlockPaleHangingMossHandler(BlockType::PaleHangingMoss);
	constexpr cDefaultBlockHandler                          BlockPaleMossBlockHandler(BlockType::PaleMossBlock);
	constexpr cDefaultBlockHandler                          BlockPaleMossCarpetHandler(BlockType::PaleMossCarpet);
	constexpr cBlockButtonHandler                           BlockPaleOakButtonHandler(BlockType::PaleOakButton);
	constexpr cBlockDoorHandler                             BlockPaleOakDoorHandler(BlockType::PaleOakDoor);
	constexpr cBlockFenceHandler                            BlockPaleOakFenceHandler(BlockType::PaleOakFence);
	constexpr cBlockFenceGateHandler                        BlockPaleOakFenceGateHandler(BlockType::PaleOakFenceGate);
	constexpr cBlockSignPostHandler                         BlockPaleOakHangingSignHandler(BlockType::PaleOakHangingSign);
	constexpr cBlockLeavesHandler                           BlockPaleOakLeavesHandler(BlockType::PaleOakLeaves);
	constexpr cBlockLogHandler                              BlockPaleOakLogHandler(BlockType::PaleOakLog);
	constexpr cBlockPlanksHandler                           BlockPaleOakPlanksHandler(BlockType::PaleOakPlanks);
	constexpr cBlockPressurePlateHandler                    BlockPaleOakPressurePlateHandler(BlockType::PaleOakPressurePlate);
	constexpr cBlockSaplingHandler                          BlockPaleOakSaplingHandler(BlockType::PaleOakSapling);
	constexpr cBlockSignPostHandler                         BlockPaleOakSignHandler(BlockType::PaleOakSign);
	constexpr cBlockSlabHandler                             BlockPaleOakSlabHandler(BlockType::PaleOakSlab);
	constexpr cBlockStairsHandler                           BlockPaleOakStairsHandler(BlockType::PaleOakStairs);
	constexpr cBlockTrapdoorHandler                         BlockPaleOakTrapdoorHandler(BlockType::PaleOakTrapdoor);
	constexpr cBlockSignPostHandler                         BlockPaleOakWallHangingSignHandler(BlockType::PaleOakWallHangingSign);
	constexpr cBlockSignPostHandler                         BlockPaleOakWallSignHandler(BlockType::PaleOakWallSign);
	constexpr cDefaultBlockHandler                          BlockPaleOakWoodHandler(BlockType::PaleOakWood);
	constexpr cDefaultBlockHandler                          BlockPottedClosedEyeblossomHandler(BlockType::PottedClosedEyeblossom);
	constexpr cDefaultBlockHandler                          BlockPottedOpenEyeblossomHandler(BlockType::PottedOpenEyeblossom);
	constexpr cDefaultBlockHandler                          BlockPottedPaleOakSaplingHandler(BlockType::PottedPaleOakSapling);
	constexpr cDefaultBlockHandler                          BlockResinBlockHandler(BlockType::ResinBlock);
	constexpr cBlockSlabHandler                             BlockResinBrickSlabHandler(BlockType::ResinBrickSlab);
	constexpr cBlockStairsHandler                           BlockResinBrickStairsHandler(BlockType::ResinBrickStairs);
	constexpr cDefaultBlockHandler                          BlockResinBrickWallHandler(BlockType::ResinBrickWall);
	constexpr cDefaultBlockHandler                          BlockResinBricksHandler(BlockType::ResinBricks);
	constexpr cDefaultBlockHandler                          BlockResinClumpHandler(BlockType::ResinClump);
	constexpr cBlockLogHandler                              BlockStrippedPaleOakLogHandler(BlockType::StrippedPaleOakLog);
	constexpr cDefaultBlockHandler                          BlockStrippedPaleOakWoodHandler(BlockType::StrippedPaleOakWood);
}





////////////////////////////////////////////////////////////////////////////////
// cBlockHandler:

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
	if (a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk) { return CanBeAt(a_Chunk, cChunkDef::AbsoluteToRelative(a_BlockPos), a_Chunk.GetBlock(cChunkDef::AbsoluteToRelative(a_BlockPos))); }))
	{
		return;
	}

	a_ChunkInterface.DropBlockAsPickups(a_BlockPos);
}





void cBlockHandler::NeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_NeighborPos, eBlockFace a_WhichNeighbor)
{
	if (!cChunkDef::IsValidHeight(a_NeighborPos))
	{
		return;
	}

	cBlockHandler::For(a_ChunkInterface.GetBlock(a_NeighborPos).Type()).OnNeighborChanged(a_ChunkInterface, a_NeighborPos, a_WhichNeighbor);
}





cItems cBlockHandler::ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const
{
	UNUSED(a_Tool);

	// Add self:
	return cItem(BlockItemConverter::FromBlock(m_BlockType));
}





bool cBlockHandler::CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, BlockState a_Self) const
{
	return true;
}





bool cBlockHandler::IsUseable() const
{
	return false;
}





bool cBlockHandler::DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_ClickedBlock, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const
{
	return cBlockAirHandler::IsBlockAir(m_BlockType);
}





bool cBlockHandler::IsInsideBlock(const Vector3d a_RelPosition, const BlockState a_Block) const
{
	// Default functionality: Test the height, since we assume full voxels with varying height
	return (a_RelPosition.y < cBlockInfo::GetBlockHeight(m_BlockType));
}





cBoundingBox cBlockHandler::GetPlacementCollisionBox(BlockState a_XM, BlockState a_XP, BlockState a_YM, BlockState a_YP, BlockState a_ZM, BlockState a_ZP) const
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





ColourID cBlockHandler::GetMapBaseColourID() const
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
	char DropNum = GetRandomProvider().RandInt<char>(a_MinDrop, static_cast<char>(a_DefaultMax + a_BonusMax));

	// Then clamp to within range (clamp instead of min incase of overflow):
	return std::clamp<char>(DropNum, a_MinDrop, a_DropCap);
}





const cBlockHandler & cBlockHandler::For(BlockType a_BlockType)
{
	// Switch on the block type, as an enumeration
	// Clang will fail if any type is unhandled:
	switch (a_BlockType)
	{
		case BlockType::AcaciaButton:                    return BlockAcaciaButtonHandler;
		case BlockType::AcaciaDoor:                      return BlockAcaciaDoorHandler;
		case BlockType::AcaciaFence:                     return BlockAcaciaFenceHandler;
		case BlockType::AcaciaFenceGate:                 return BlockAcaciaFenceGateHandler;
		case BlockType::AcaciaHangingSign:               return BlockAcaciaHangingSignHandler;
		case BlockType::AcaciaLeaves:                    return BlockAcaciaLeavesHandler;
		case BlockType::AcaciaLog:                       return BlockAcaciaLogHandler;
		case BlockType::AcaciaPlanks:                    return BlockAcaciaPlanksHandler;
		case BlockType::AcaciaPressurePlate:             return BlockAcaciaPressurePlateHandler;
		case BlockType::AcaciaSapling:                   return BlockAcaciaSaplingHandler;
		case BlockType::AcaciaSign:                      return BlockAcaciaSignHandler;
		case BlockType::AcaciaSlab:                      return BlockAcaciaSlabHandler;
		case BlockType::AcaciaStairs:                    return BlockAcaciaStairsHandler;
		case BlockType::AcaciaTrapdoor:                  return BlockAcaciaTrapdoorHandler;
		case BlockType::AcaciaWallHangingSign:           return BlockAcaciaWallHangingSignHandler;
		case BlockType::AcaciaWallSign:                  return BlockAcaciaWallSignHandler;
		case BlockType::AcaciaWood:                      return BlockAcaciaWoodHandler;
		case BlockType::ActivatorRail:                   return BlockActivatorRailHandler;
		case BlockType::Air:                             return BlockAirHandler;
		case BlockType::Allium:                          return BlockAlliumHandler;
		case BlockType::AmethystBlock:                   return BlockAmethystBlockHandler;
		case BlockType::AmethystCluster:                 return BlockAmethystClusterHandler;
		case BlockType::AncientDebris:                   return BlockAncientDebrisHandler;
		case BlockType::Andesite:                        return BlockAndesiteHandler;
		case BlockType::AndesiteSlab:                    return BlockAndesiteSlabHandler;
		case BlockType::AndesiteStairs:                  return BlockAndesiteStairsHandler;
		case BlockType::AndesiteWall:                    return BlockAndesiteWallHandler;
		case BlockType::Anvil:                           return BlockAnvilHandler;
		case BlockType::AttachedMelonStem:               return BlockAttachedMelonStemHandler;
		case BlockType::AttachedPumpkinStem:             return BlockAttachedPumpkinStemHandler;
		case BlockType::Azalea:                          return BlockAzaleaHandler;
		case BlockType::AzaleaLeaves:                    return BlockAzaleaLeavesHandler;
		case BlockType::AzureBluet:                      return BlockAzureBluetHandler;
		case BlockType::Bamboo:                          return BlockBambooHandler;
		case BlockType::BambooBlock:                     return BlockBambooBlockHandler;
		case BlockType::BambooButton:                    return BlockBambooButtonHandler;
		case BlockType::BambooDoor:                      return BlockBambooDoorHandler;
		case BlockType::BambooFence:                     return BlockBambooFenceHandler;
		case BlockType::BambooFenceGate:                 return BlockBambooFenceGateHandler;
		case BlockType::BambooHangingSign:               return BlockBambooHangingSignHandler;
		case BlockType::BambooMosaic:                    return BlockBambooMosaicHandler;
		case BlockType::BambooMosaicSlab:                return BlockBambooMosaicSlabHandler;
		case BlockType::BambooMosaicStairs:              return BlockBambooMosaicStairsHandler;
		case BlockType::BambooPlanks:                    return BlockBambooPlanksHandler;
		case BlockType::BambooPressurePlate:             return BlockBambooPressurePlateHandler;
		case BlockType::BambooSapling:                   return BlockBambooSaplingHandler;
		case BlockType::BambooSign:                      return BlockBambooSignHandler;
		case BlockType::BambooSlab:                      return BlockBambooSlabHandler;
		case BlockType::BambooStairs:                    return BlockBambooStairsHandler;
		case BlockType::BambooTrapdoor:                  return BlockBambooTrapdoorHandler;
		case BlockType::BambooWallHangingSign:           return BlockBambooWallHangingSignHandler;
		case BlockType::BambooWallSign:                  return BlockBambooWallSignHandler;
		case BlockType::Barrel:                          return BlockBarrelHandler;
		case BlockType::Barrier:                         return BlockBarrierHandler;
		case BlockType::Basalt:                          return BlockBasaltHandler;
		case BlockType::Beacon:                          return BlockBeaconHandler;
		case BlockType::Bedrock:                         return BlockBedrockHandler;
		case BlockType::BeeNest:                         return BlockBeeNestHandler;
		case BlockType::Beehive:                         return BlockBeehiveHandler;
		case BlockType::Beetroots:                       return BlockBeetrootsHandler;
		case BlockType::Bell:                            return BlockBellHandler;
		case BlockType::BigDripleaf:                     return BlockBigDripleafHandler;
		case BlockType::BigDripleafStem:                 return BlockBigDripleafStemHandler;
		case BlockType::BirchButton:                     return BlockBirchButtonHandler;
		case BlockType::BirchDoor:                       return BlockBirchDoorHandler;
		case BlockType::BirchFence:                      return BlockBirchFenceHandler;
		case BlockType::BirchFenceGate:                  return BlockBirchFenceGateHandler;
		case BlockType::BirchHangingSign:                return BlockBirchHangingSignHandler;
		case BlockType::BirchLeaves:                     return BlockBirchLeavesHandler;
		case BlockType::BirchLog:                        return BlockBirchLogHandler;
		case BlockType::BirchPlanks:                     return BlockBirchPlanksHandler;
		case BlockType::BirchPressurePlate:              return BlockBirchPressurePlateHandler;
		case BlockType::BirchSapling:                    return BlockBirchSaplingHandler;
		case BlockType::BirchSign:                       return BlockBirchSignHandler;
		case BlockType::BirchSlab:                       return BlockBirchSlabHandler;
		case BlockType::BirchStairs:                     return BlockBirchStairsHandler;
		case BlockType::BirchTrapdoor:                   return BlockBirchTrapdoorHandler;
		case BlockType::BirchWallHangingSign:            return BlockBirchWallHangingSignHandler;
		case BlockType::BirchWallSign:                   return BlockBirchWallSignHandler;
		case BlockType::BirchWood:                       return BlockBirchWoodHandler;
		case BlockType::BlackBanner:                     return BlockBlackBannerHandler;
		case BlockType::BlackBed:                        return BlockBlackBedHandler;
		case BlockType::BlackCandle:                     return BlockBlackCandleHandler;
		case BlockType::BlackCandleCake:                 return BlockBlackCandleCakeHandler;
		case BlockType::BlackCarpet:                     return BlockBlackCarpetHandler;
		case BlockType::BlackConcrete:                   return BlockBlackConcreteHandler;
		case BlockType::BlackConcretePowder:             return BlockBlackConcretePowderHandler;
		case BlockType::BlackGlazedTerracotta:           return BlockBlackGlazedTerracottaHandler;
		case BlockType::BlackShulkerBox:                 return BlockBlackShulkerBoxHandler;
		case BlockType::BlackStainedGlass:               return BlockBlackStainedGlassHandler;
		case BlockType::BlackStainedGlassPane:           return BlockBlackStainedGlassPaneHandler;
		case BlockType::BlackTerracotta:                 return BlockBlackTerracottaHandler;
		case BlockType::BlackWallBanner:                 return BlockBlackWallBannerHandler;
		case BlockType::BlackWool:                       return BlockBlackWoolHandler;
		case BlockType::Blackstone:                      return BlockBlackstoneHandler;
		case BlockType::BlackstoneSlab:                  return BlockBlackstoneSlabHandler;
		case BlockType::BlackstoneStairs:                return BlockBlackstoneStairsHandler;
		case BlockType::BlackstoneWall:                  return BlockBlackstoneWallHandler;
		case BlockType::BlastFurnace:                    return BlockBlastFurnaceHandler;
		case BlockType::BlueBanner:                      return BlockBlueBannerHandler;
		case BlockType::BlueBed:                         return BlockBlueBedHandler;
		case BlockType::BlueCandle:                      return BlockBlueCandleHandler;
		case BlockType::BlueCandleCake:                  return BlockBlueCandleCakeHandler;
		case BlockType::BlueCarpet:                      return BlockBlueCarpetHandler;
		case BlockType::BlueConcrete:                    return BlockBlueConcreteHandler;
		case BlockType::BlueConcretePowder:              return BlockBlueConcretePowderHandler;
		case BlockType::BlueGlazedTerracotta:            return BlockBlueGlazedTerracottaHandler;
		case BlockType::BlueIce:                         return BlockBlueIceHandler;
		case BlockType::BlueOrchid:                      return BlockBlueOrchidHandler;
		case BlockType::BlueShulkerBox:                  return BlockBlueShulkerBoxHandler;
		case BlockType::BlueStainedGlass:                return BlockBlueStainedGlassHandler;
		case BlockType::BlueStainedGlassPane:            return BlockBlueStainedGlassPaneHandler;
		case BlockType::BlueTerracotta:                  return BlockBlueTerracottaHandler;
		case BlockType::BlueWallBanner:                  return BlockBlueWallBannerHandler;
		case BlockType::BlueWool:                        return BlockBlueWoolHandler;
		case BlockType::BoneBlock:                       return BlockBoneBlockHandler;
		case BlockType::Bookshelf:                       return BlockBookshelfHandler;
		case BlockType::BrainCoral:                      return BlockBrainCoralHandler;
		case BlockType::BrainCoralBlock:                 return BlockBrainCoralBlockHandler;
		case BlockType::BrainCoralFan:                   return BlockBrainCoralFanHandler;
		case BlockType::BrainCoralWallFan:               return BlockBrainCoralWallFanHandler;
		case BlockType::BrewingStand:                    return BlockBrewingStandHandler;
		case BlockType::BrickSlab:                       return BlockBrickSlabHandler;
		case BlockType::BrickStairs:                     return BlockBrickStairsHandler;
		case BlockType::BrickWall:                       return BlockBrickWallHandler;
		case BlockType::Bricks:                          return BlockBricksHandler;
		case BlockType::BrownBanner:                     return BlockBrownBannerHandler;
		case BlockType::BrownBed:                        return BlockBrownBedHandler;
		case BlockType::BrownCandle:                     return BlockBrownCandleHandler;
		case BlockType::BrownCandleCake:                 return BlockBrownCandleCakeHandler;
		case BlockType::BrownCarpet:                     return BlockBrownCarpetHandler;
		case BlockType::BrownConcrete:                   return BlockBrownConcreteHandler;
		case BlockType::BrownConcretePowder:             return BlockBrownConcretePowderHandler;
		case BlockType::BrownGlazedTerracotta:           return BlockBrownGlazedTerracottaHandler;
		case BlockType::BrownMushroom:                   return BlockBrownMushroomHandler;
		case BlockType::BrownMushroomBlock:              return BlockBrownMushroomBlockHandler;
		case BlockType::BrownShulkerBox:                 return BlockBrownShulkerBoxHandler;
		case BlockType::BrownStainedGlass:               return BlockBrownStainedGlassHandler;
		case BlockType::BrownStainedGlassPane:           return BlockBrownStainedGlassPaneHandler;
		case BlockType::BrownTerracotta:                 return BlockBrownTerracottaHandler;
		case BlockType::BrownWallBanner:                 return BlockBrownWallBannerHandler;
		case BlockType::BrownWool:                       return BlockBrownWoolHandler;
		case BlockType::BubbleColumn:                    return BlockBubbleColumnHandler;
		case BlockType::BubbleCoral:                     return BlockBubbleCoralHandler;
		case BlockType::BubbleCoralBlock:                return BlockBubbleCoralBlockHandler;
		case BlockType::BubbleCoralFan:                  return BlockBubbleCoralFanHandler;
		case BlockType::BubbleCoralWallFan:              return BlockBubbleCoralWallFanHandler;
		case BlockType::BuddingAmethyst:                 return BlockBuddingAmethystHandler;
		case BlockType::Cactus:                          return BlockCactusHandler;
		case BlockType::Cake:                            return BlockCakeHandler;
		case BlockType::Calcite:                         return BlockCalciteHandler;
		case BlockType::CalibratedSculkSensor:           return BlockCalibratedSculkSensorHandler;
		case BlockType::Campfire:                        return BlockCampfireHandler;
		case BlockType::Candle:                          return BlockCandleHandler;
		case BlockType::CandleCake:                      return BlockCandleCakeHandler;
		case BlockType::Carrots:                         return BlockCarrotsHandler;
		case BlockType::CartographyTable:                return BlockCartographyTableHandler;
		case BlockType::CarvedPumpkin:                   return BlockCarvedPumpkinHandler;
		case BlockType::Cauldron:                        return BlockCauldronHandler;
		case BlockType::CaveAir:                         return BlockCaveAirHandler;
		case BlockType::CaveVines:                       return BlockCaveVinesHandler;
		case BlockType::CaveVinesPlant:                  return BlockCaveVinesPlantHandler;
		case BlockType::Chain:                           return BlockChainHandler;
		case BlockType::ChainCommandBlock:               return BlockChainCommandBlockHandler;
		case BlockType::CherryButton:                    return BlockCherryButtonHandler;
		case BlockType::CherryDoor:                      return BlockCherryDoorHandler;
		case BlockType::CherryFence:                     return BlockCherryFenceHandler;
		case BlockType::CherryFenceGate:                 return BlockCherryFenceGateHandler;
		case BlockType::CherryHangingSign:               return BlockCherryHangingSignHandler;
		case BlockType::CherryLeaves:                    return BlockCherryLeavesHandler;
		case BlockType::CherryLog:                       return BlockCherryLogHandler;
		case BlockType::CherryPlanks:                    return BlockCherryPlanksHandler;
		case BlockType::CherryPressurePlate:             return BlockCherryPressurePlateHandler;
		case BlockType::CherrySapling:                   return BlockCherrySaplingHandler;
		case BlockType::CherrySign:                      return BlockCherrySignHandler;
		case BlockType::CherrySlab:                      return BlockCherrySlabHandler;
		case BlockType::CherryStairs:                    return BlockCherryStairsHandler;
		case BlockType::CherryTrapdoor:                  return BlockCherryTrapdoorHandler;
		case BlockType::CherryWallHangingSign:           return BlockCherryWallHangingSignHandler;
		case BlockType::CherryWallSign:                  return BlockCherryWallSignHandler;
		case BlockType::CherryWood:                      return BlockCherryWoodHandler;
		case BlockType::Chest:                           return BlockChestHandler;
		case BlockType::ChippedAnvil:                    return BlockChippedAnvilHandler;
		case BlockType::ChiseledBookshelf:               return BlockChiseledBookshelfHandler;
		case BlockType::ChiseledCopper:                  return BlockChiseledCopperHandler;
		case BlockType::ChiseledDeepslate:               return BlockChiseledDeepslateHandler;
		case BlockType::ChiseledNetherBricks:            return BlockChiseledNetherBricksHandler;
		case BlockType::ChiseledPolishedBlackstone:      return BlockChiseledPolishedBlackstoneHandler;
		case BlockType::ChiseledQuartzBlock:             return BlockChiseledQuartzBlockHandler;
		case BlockType::ChiseledRedSandstone:            return BlockChiseledRedSandstoneHandler;
		case BlockType::ChiseledResinBricks:             return BlockChiseledResinBricksHandler;
		case BlockType::ChiseledSandstone:               return BlockChiseledSandstoneHandler;
		case BlockType::ChiseledStoneBricks:             return BlockChiseledStoneBricksHandler;
		case BlockType::ChiseledTuff:                    return BlockChiseledTuffHandler;
		case BlockType::ChiseledTuffBricks:              return BlockChiseledTuffBricksHandler;
		case BlockType::ChorusFlower:                    return BlockChorusFlowerHandler;
		case BlockType::ChorusPlant:                     return BlockChorusPlantHandler;
		case BlockType::Clay:                            return BlockClayHandler;
		case BlockType::ClosedEyeblossom:                return BlockClosedEyeblossomHandler;
		case BlockType::CoalBlock:                       return BlockCoalBlockHandler;
		case BlockType::CoalOre:                         return BlockCoalOreHandler;
		case BlockType::CoarseDirt:                      return BlockCoarseDirtHandler;
		case BlockType::CobbledDeepslate:                return BlockCobbledDeepslateHandler;
		case BlockType::CobbledDeepslateSlab:            return BlockCobbledDeepslateSlabHandler;
		case BlockType::CobbledDeepslateStairs:          return BlockCobbledDeepslateStairsHandler;
		case BlockType::CobbledDeepslateWall:            return BlockCobbledDeepslateWallHandler;
		case BlockType::Cobblestone:                     return BlockCobblestoneHandler;
		case BlockType::CobblestoneSlab:                 return BlockCobblestoneSlabHandler;
		case BlockType::CobblestoneStairs:               return BlockCobblestoneStairsHandler;
		case BlockType::CobblestoneWall:                 return BlockCobblestoneWallHandler;
		case BlockType::Cobweb:                          return BlockCobwebHandler;
		case BlockType::Cocoa:                           return BlockCocoaHandler;
		case BlockType::CommandBlock:                    return BlockCommandBlockHandler;
		case BlockType::Comparator:                      return BlockComparatorHandler;
		case BlockType::Composter:                       return BlockComposterHandler;
		case BlockType::Conduit:                         return BlockConduitHandler;
		case BlockType::CopperBlock:                     return BlockCopperBlockHandler;
		case BlockType::CopperBulb:                      return BlockCopperBulbHandler;
		case BlockType::CopperDoor:                      return BlockCopperDoorHandler;
		case BlockType::CopperGrate:                     return BlockCopperGrateHandler;
		case BlockType::CopperOre:                       return BlockCopperOreHandler;
		case BlockType::CopperTrapdoor:                  return BlockCopperTrapdoorHandler;
		case BlockType::Cornflower:                      return BlockCornflowerHandler;
		case BlockType::CrackedDeepslateBricks:          return BlockCrackedDeepslateBricksHandler;
		case BlockType::CrackedDeepslateTiles:           return BlockCrackedDeepslateTilesHandler;
		case BlockType::CrackedNetherBricks:             return BlockCrackedNetherBricksHandler;
		case BlockType::CrackedPolishedBlackstoneBricks: return BlockCrackedPolishedBlackstoneBricksHandler;
		case BlockType::CrackedStoneBricks:              return BlockCrackedStoneBricksHandler;
		case BlockType::Crafter:                         return BlockCrafterHandler;
		case BlockType::CraftingTable:                   return BlockCraftingTableHandler;
		case BlockType::CreakingHeart:                   return BlockCreakingHeartHandler;
		case BlockType::CreeperHead:                     return BlockCreeperHeadHandler;
		case BlockType::CreeperWallHead:                 return BlockCreeperWallHeadHandler;
		case BlockType::CrimsonButton:                   return BlockCrimsonButtonHandler;
		case BlockType::CrimsonDoor:                     return BlockCrimsonDoorHandler;
		case BlockType::CrimsonFence:                    return BlockCrimsonFenceHandler;
		case BlockType::CrimsonFenceGate:                return BlockCrimsonFenceGateHandler;
		case BlockType::CrimsonFungus:                   return BlockCrimsonFungusHandler;
		case BlockType::CrimsonHangingSign:              return BlockCrimsonHangingSignHandler;
		case BlockType::CrimsonHyphae:                   return BlockCrimsonHyphaeHandler;
		case BlockType::CrimsonNylium:                   return BlockCrimsonNyliumHandler;
		case BlockType::CrimsonPlanks:                   return BlockCrimsonPlanksHandler;
		case BlockType::CrimsonPressurePlate:            return BlockCrimsonPressurePlateHandler;
		case BlockType::CrimsonRoots:                    return BlockCrimsonRootsHandler;
		case BlockType::CrimsonSign:                     return BlockCrimsonSignHandler;
		case BlockType::CrimsonSlab:                     return BlockCrimsonSlabHandler;
		case BlockType::CrimsonStairs:                   return BlockCrimsonStairsHandler;
		case BlockType::CrimsonStem:                     return BlockCrimsonStemHandler;
		case BlockType::CrimsonTrapdoor:                 return BlockCrimsonTrapdoorHandler;
		case BlockType::CrimsonWallHangingSign:          return BlockCrimsonWallHangingSignHandler;
		case BlockType::CrimsonWallSign:                 return BlockCrimsonWallSignHandler;
		case BlockType::CryingObsidian:                  return BlockCryingObsidianHandler;
		case BlockType::CutCopper:                       return BlockCutCopperHandler;
		case BlockType::CutCopperSlab:                   return BlockCutCopperSlabHandler;
		case BlockType::CutCopperStairs:                 return BlockCutCopperStairsHandler;
		case BlockType::CutRedSandstone:                 return BlockCutRedSandstoneHandler;
		case BlockType::CutRedSandstoneSlab:             return BlockCutRedSandstoneSlabHandler;
		case BlockType::CutSandstone:                    return BlockCutSandstoneHandler;
		case BlockType::CutSandstoneSlab:                return BlockCutSandstoneSlabHandler;
		case BlockType::CyanBanner:                      return BlockCyanBannerHandler;
		case BlockType::CyanBed:                         return BlockCyanBedHandler;
		case BlockType::CyanCandle:                      return BlockCyanCandleHandler;
		case BlockType::CyanCandleCake:                  return BlockCyanCandleCakeHandler;
		case BlockType::CyanCarpet:                      return BlockCyanCarpetHandler;
		case BlockType::CyanConcrete:                    return BlockCyanConcreteHandler;
		case BlockType::CyanConcretePowder:              return BlockCyanConcretePowderHandler;
		case BlockType::CyanGlazedTerracotta:            return BlockCyanGlazedTerracottaHandler;
		case BlockType::CyanShulkerBox:                  return BlockCyanShulkerBoxHandler;
		case BlockType::CyanStainedGlass:                return BlockCyanStainedGlassHandler;
		case BlockType::CyanStainedGlassPane:            return BlockCyanStainedGlassPaneHandler;
		case BlockType::CyanTerracotta:                  return BlockCyanTerracottaHandler;
		case BlockType::CyanWallBanner:                  return BlockCyanWallBannerHandler;
		case BlockType::CyanWool:                        return BlockCyanWoolHandler;
		case BlockType::DamagedAnvil:                    return BlockDamagedAnvilHandler;
		case BlockType::Dandelion:                       return BlockDandelionHandler;
		case BlockType::DarkOakButton:                   return BlockDarkOakButtonHandler;
		case BlockType::DarkOakDoor:                     return BlockDarkOakDoorHandler;
		case BlockType::DarkOakFence:                    return BlockDarkOakFenceHandler;
		case BlockType::DarkOakFenceGate:                return BlockDarkOakFenceGateHandler;
		case BlockType::DarkOakHangingSign:              return BlockDarkOakHangingSignHandler;
		case BlockType::DarkOakLeaves:                   return BlockDarkOakLeavesHandler;
		case BlockType::DarkOakLog:                      return BlockDarkOakLogHandler;
		case BlockType::DarkOakPlanks:                   return BlockDarkOakPlanksHandler;
		case BlockType::DarkOakPressurePlate:            return BlockDarkOakPressurePlateHandler;
		case BlockType::DarkOakSapling:                  return BlockDarkOakSaplingHandler;
		case BlockType::DarkOakSign:                     return BlockDarkOakSignHandler;
		case BlockType::DarkOakSlab:                     return BlockDarkOakSlabHandler;
		case BlockType::DarkOakStairs:                   return BlockDarkOakStairsHandler;
		case BlockType::DarkOakTrapdoor:                 return BlockDarkOakTrapdoorHandler;
		case BlockType::DarkOakWallHangingSign:          return BlockDarkOakWallHangingSignHandler;
		case BlockType::DarkOakWallSign:                 return BlockDarkOakWallSignHandler;
		case BlockType::DarkOakWood:                     return BlockDarkOakWoodHandler;
		case BlockType::DarkPrismarine:                  return BlockDarkPrismarineHandler;
		case BlockType::DarkPrismarineSlab:              return BlockDarkPrismarineSlabHandler;
		case BlockType::DarkPrismarineStairs:            return BlockDarkPrismarineStairsHandler;
		case BlockType::DaylightDetector:                return BlockDaylightDetectorHandler;
		case BlockType::DeadBrainCoral:                  return BlockDeadBrainCoralHandler;
		case BlockType::DeadBrainCoralBlock:             return BlockDeadBrainCoralBlockHandler;
		case BlockType::DeadBrainCoralFan:               return BlockDeadBrainCoralFanHandler;
		case BlockType::DeadBrainCoralWallFan:           return BlockDeadBrainCoralWallFanHandler;
		case BlockType::DeadBubbleCoral:                 return BlockDeadBubbleCoralHandler;
		case BlockType::DeadBubbleCoralBlock:            return BlockDeadBubbleCoralBlockHandler;
		case BlockType::DeadBubbleCoralFan:              return BlockDeadBubbleCoralFanHandler;
		case BlockType::DeadBubbleCoralWallFan:          return BlockDeadBubbleCoralWallFanHandler;
		case BlockType::DeadBush:                        return BlockDeadBushHandler;
		case BlockType::DeadFireCoral:                   return BlockDeadFireCoralHandler;
		case BlockType::DeadFireCoralBlock:              return BlockDeadFireCoralBlockHandler;
		case BlockType::DeadFireCoralFan:                return BlockDeadFireCoralFanHandler;
		case BlockType::DeadFireCoralWallFan:            return BlockDeadFireCoralWallFanHandler;
		case BlockType::DeadHornCoral:                   return BlockDeadHornCoralHandler;
		case BlockType::DeadHornCoralBlock:              return BlockDeadHornCoralBlockHandler;
		case BlockType::DeadHornCoralFan:                return BlockDeadHornCoralFanHandler;
		case BlockType::DeadHornCoralWallFan:            return BlockDeadHornCoralWallFanHandler;
		case BlockType::DeadTubeCoral:                   return BlockDeadTubeCoralHandler;
		case BlockType::DeadTubeCoralBlock:              return BlockDeadTubeCoralBlockHandler;
		case BlockType::DeadTubeCoralFan:                return BlockDeadTubeCoralFanHandler;
		case BlockType::DeadTubeCoralWallFan:            return BlockDeadTubeCoralWallFanHandler;
		case BlockType::DecoratedPot:                    return BlockDecoratedPotHandler;
		case BlockType::Deepslate:                       return BlockDeepslateHandler;
		case BlockType::DeepslateBrickSlab:              return BlockDeepslateBrickSlabHandler;
		case BlockType::DeepslateBrickStairs:            return BlockDeepslateBrickStairsHandler;
		case BlockType::DeepslateBrickWall:              return BlockDeepslateBrickWallHandler;
		case BlockType::DeepslateBricks:                 return BlockDeepslateBricksHandler;
		case BlockType::DeepslateCoalOre:                return BlockDeepslateCoalOreHandler;
		case BlockType::DeepslateCopperOre:              return BlockDeepslateCopperOreHandler;
		case BlockType::DeepslateDiamondOre:             return BlockDeepslateDiamondOreHandler;
		case BlockType::DeepslateEmeraldOre:             return BlockDeepslateEmeraldOreHandler;
		case BlockType::DeepslateGoldOre:                return BlockDeepslateGoldOreHandler;
		case BlockType::DeepslateIronOre:                return BlockDeepslateIronOreHandler;
		case BlockType::DeepslateLapisOre:               return BlockDeepslateLapisOreHandler;
		case BlockType::DeepslateRedstoneOre:            return BlockDeepslateRedstoneOreHandler;
		case BlockType::DeepslateTileSlab:               return BlockDeepslateTileSlabHandler;
		case BlockType::DeepslateTileStairs:             return BlockDeepslateTileStairsHandler;
		case BlockType::DeepslateTileWall:               return BlockDeepslateTileWallHandler;
		case BlockType::DeepslateTiles:                  return BlockDeepslateTilesHandler;
		case BlockType::DetectorRail:                    return BlockDetectorRailHandler;
		case BlockType::DiamondBlock:                    return BlockDiamondBlockHandler;
		case BlockType::DiamondOre:                      return BlockDiamondOreHandler;
		case BlockType::Diorite:                         return BlockDioriteHandler;
		case BlockType::DioriteSlab:                     return BlockDioriteSlabHandler;
		case BlockType::DioriteStairs:                   return BlockDioriteStairsHandler;
		case BlockType::DioriteWall:                     return BlockDioriteWallHandler;
		case BlockType::Dirt:                            return BlockDirtHandler;
		case BlockType::DirtPath:                        return BlockDirtPathHandler;
		case BlockType::Dispenser:                       return BlockDispenserHandler;
		case BlockType::DragonEgg:                       return BlockDragonEggHandler;
		case BlockType::DragonHead:                      return BlockDragonHeadHandler;
		case BlockType::DragonWallHead:                  return BlockDragonWallHeadHandler;
		case BlockType::DriedKelpBlock:                  return BlockDriedKelpBlockHandler;
		case BlockType::DripstoneBlock:                  return BlockDripstoneBlockHandler;
		case BlockType::Dropper:                         return BlockDropperHandler;
		case BlockType::EmeraldBlock:                    return BlockEmeraldBlockHandler;
		case BlockType::EmeraldOre:                      return BlockEmeraldOreHandler;
		case BlockType::EnchantingTable:                 return BlockEnchantingTableHandler;
		case BlockType::EndGateway:                      return BlockEndGatewayHandler;
		case BlockType::EndPortal:                       return BlockEndPortalHandler;
		case BlockType::EndPortalFrame:                  return BlockEndPortalFrameHandler;
		case BlockType::EndRod:                          return BlockEndRodHandler;
		case BlockType::EndStone:                        return BlockEndStoneHandler;
		case BlockType::EndStoneBrickSlab:               return BlockEndStoneBrickSlabHandler;
		case BlockType::EndStoneBrickStairs:             return BlockEndStoneBrickStairsHandler;
		case BlockType::EndStoneBrickWall:               return BlockEndStoneBrickWallHandler;
		case BlockType::EndStoneBricks:                  return BlockEndStoneBricksHandler;
		case BlockType::EnderChest:                      return BlockEnderChestHandler;
		case BlockType::ExposedChiseledCopper:           return BlockExposedChiseledCopperHandler;
		case BlockType::ExposedCopper:                   return BlockExposedCopperHandler;
		case BlockType::ExposedCopperBulb:               return BlockExposedCopperBulbHandler;
		case BlockType::ExposedCopperDoor:               return BlockExposedCopperDoorHandler;
		case BlockType::ExposedCopperGrate:              return BlockExposedCopperGrateHandler;
		case BlockType::ExposedCopperTrapdoor:           return BlockExposedCopperTrapdoorHandler;
		case BlockType::ExposedCutCopper:                return BlockExposedCutCopperHandler;
		case BlockType::ExposedCutCopperSlab:            return BlockExposedCutCopperSlabHandler;
		case BlockType::ExposedCutCopperStairs:          return BlockExposedCutCopperStairsHandler;
		case BlockType::Farmland:                        return BlockFarmlandHandler;
		case BlockType::Fern:                            return BlockFernHandler;
		case BlockType::Fire:                            return BlockFireHandler;
		case BlockType::FireCoral:                       return BlockFireCoralHandler;
		case BlockType::FireCoralBlock:                  return BlockFireCoralBlockHandler;
		case BlockType::FireCoralFan:                    return BlockFireCoralFanHandler;
		case BlockType::FireCoralWallFan:                return BlockFireCoralWallFanHandler;
		case BlockType::FletchingTable:                  return BlockFletchingTableHandler;
		case BlockType::FlowerPot:                       return BlockFlowerPotHandler;
		case BlockType::FloweringAzalea:                 return BlockFloweringAzaleaHandler;
		case BlockType::FloweringAzaleaLeaves:           return BlockFloweringAzaleaLeavesHandler;
		case BlockType::Frogspawn:                       return BlockFrogspawnHandler;
		case BlockType::FrostedIce:                      return BlockFrostedIceHandler;
		case BlockType::Furnace:                         return BlockFurnaceHandler;
		case BlockType::GildedBlackstone:                return BlockGildedBlackstoneHandler;
		case BlockType::Glass:                           return BlockGlassHandler;
		case BlockType::GlassPane:                       return BlockGlassPaneHandler;
		case BlockType::GlowLichen:                      return BlockGlowLichenHandler;
		case BlockType::Glowstone:                       return BlockGlowstoneHandler;
		case BlockType::GoldBlock:                       return BlockGoldBlockHandler;
		case BlockType::GoldOre:                         return BlockGoldOreHandler;
		case BlockType::Granite:                         return BlockGraniteHandler;
		case BlockType::GraniteSlab:                     return BlockGraniteSlabHandler;
		case BlockType::GraniteStairs:                   return BlockGraniteStairsHandler;
		case BlockType::GraniteWall:                     return BlockGraniteWallHandler;
		case BlockType::GrassBlock:                      return BlockGrassBlockHandler;
		case BlockType::Gravel:                          return BlockGravelHandler;
		case BlockType::GrayBanner:                      return BlockGrayBannerHandler;
		case BlockType::GrayBed:                         return BlockGrayBedHandler;
		case BlockType::GrayCandle:                      return BlockGrayCandleHandler;
		case BlockType::GrayCandleCake:                  return BlockGrayCandleCakeHandler;
		case BlockType::GrayCarpet:                      return BlockGrayCarpetHandler;
		case BlockType::GrayConcrete:                    return BlockGrayConcreteHandler;
		case BlockType::GrayConcretePowder:              return BlockGrayConcretePowderHandler;
		case BlockType::GrayGlazedTerracotta:            return BlockGrayGlazedTerracottaHandler;
		case BlockType::GrayShulkerBox:                  return BlockGrayShulkerBoxHandler;
		case BlockType::GrayStainedGlass:                return BlockGrayStainedGlassHandler;
		case BlockType::GrayStainedGlassPane:            return BlockGrayStainedGlassPaneHandler;
		case BlockType::GrayTerracotta:                  return BlockGrayTerracottaHandler;
		case BlockType::GrayWallBanner:                  return BlockGrayWallBannerHandler;
		case BlockType::GrayWool:                        return BlockGrayWoolHandler;
		case BlockType::GreenBanner:                     return BlockGreenBannerHandler;
		case BlockType::GreenBed:                        return BlockGreenBedHandler;
		case BlockType::GreenCandle:                     return BlockGreenCandleHandler;
		case BlockType::GreenCandleCake:                 return BlockGreenCandleCakeHandler;
		case BlockType::GreenCarpet:                     return BlockGreenCarpetHandler;
		case BlockType::GreenConcrete:                   return BlockGreenConcreteHandler;
		case BlockType::GreenConcretePowder:             return BlockGreenConcretePowderHandler;
		case BlockType::GreenGlazedTerracotta:           return BlockGreenGlazedTerracottaHandler;
		case BlockType::GreenShulkerBox:                 return BlockGreenShulkerBoxHandler;
		case BlockType::GreenStainedGlass:               return BlockGreenStainedGlassHandler;
		case BlockType::GreenStainedGlassPane:           return BlockGreenStainedGlassPaneHandler;
		case BlockType::GreenTerracotta:                 return BlockGreenTerracottaHandler;
		case BlockType::GreenWallBanner:                 return BlockGreenWallBannerHandler;
		case BlockType::GreenWool:                       return BlockGreenWoolHandler;
		case BlockType::Grindstone:                      return BlockGrindstoneHandler;
		case BlockType::HangingRoots:                    return BlockHangingRootsHandler;
		case BlockType::HayBlock:                        return BlockHayBlockHandler;
		case BlockType::HeavyCore:                       return BlockHeavyCoreHandler;
		case BlockType::HeavyWeightedPressurePlate:      return BlockHeavyWeightedPressurePlateHandler;
		case BlockType::HoneyBlock:                      return BlockHoneyBlockHandler;
		case BlockType::HoneycombBlock:                  return BlockHoneycombBlockHandler;
		case BlockType::Hopper:                          return BlockHopperHandler;
		case BlockType::HornCoral:                       return BlockHornCoralHandler;
		case BlockType::HornCoralBlock:                  return BlockHornCoralBlockHandler;
		case BlockType::HornCoralFan:                    return BlockHornCoralFanHandler;
		case BlockType::HornCoralWallFan:                return BlockHornCoralWallFanHandler;
		case BlockType::Ice:                             return BlockIceHandler;
		case BlockType::InfestedChiseledStoneBricks:     return BlockInfestedChiseledStoneBricksHandler;
		case BlockType::InfestedCobblestone:             return BlockInfestedCobblestoneHandler;
		case BlockType::InfestedCrackedStoneBricks:      return BlockInfestedCrackedStoneBricksHandler;
		case BlockType::InfestedDeepslate:               return BlockInfestedDeepslateHandler;
		case BlockType::InfestedMossyStoneBricks:        return BlockInfestedMossyStoneBricksHandler;
		case BlockType::InfestedStone:                   return BlockInfestedStoneHandler;
		case BlockType::InfestedStoneBricks:             return BlockInfestedStoneBricksHandler;
		case BlockType::IronBars:                        return BlockIronBarsHandler;
		case BlockType::IronBlock:                       return BlockIronBlockHandler;
		case BlockType::IronDoor:                        return BlockIronDoorHandler;
		case BlockType::IronOre:                         return BlockIronOreHandler;
		case BlockType::IronTrapdoor:                    return BlockIronTrapdoorHandler;
		case BlockType::JackOLantern:                    return BlockJackOLanternHandler;
		case BlockType::Jigsaw:                          return BlockJigsawHandler;
		case BlockType::Jukebox:                         return BlockJukeboxHandler;
		case BlockType::JungleButton:                    return BlockJungleButtonHandler;
		case BlockType::JungleDoor:                      return BlockJungleDoorHandler;
		case BlockType::JungleFence:                     return BlockJungleFenceHandler;
		case BlockType::JungleFenceGate:                 return BlockJungleFenceGateHandler;
		case BlockType::JungleHangingSign:               return BlockJungleHangingSignHandler;
		case BlockType::JungleLeaves:                    return BlockJungleLeavesHandler;
		case BlockType::JungleLog:                       return BlockJungleLogHandler;
		case BlockType::JunglePlanks:                    return BlockJunglePlanksHandler;
		case BlockType::JunglePressurePlate:             return BlockJunglePressurePlateHandler;
		case BlockType::JungleSapling:                   return BlockJungleSaplingHandler;
		case BlockType::JungleSign:                      return BlockJungleSignHandler;
		case BlockType::JungleSlab:                      return BlockJungleSlabHandler;
		case BlockType::JungleStairs:                    return BlockJungleStairsHandler;
		case BlockType::JungleTrapdoor:                  return BlockJungleTrapdoorHandler;
		case BlockType::JungleWallHangingSign:           return BlockJungleWallHangingSignHandler;
		case BlockType::JungleWallSign:                  return BlockJungleWallSignHandler;
		case BlockType::JungleWood:                      return BlockJungleWoodHandler;
		case BlockType::Kelp:                            return BlockKelpHandler;
		case BlockType::KelpPlant:                       return BlockKelpPlantHandler;
		case BlockType::Ladder:                          return BlockLadderHandler;
		case BlockType::Lantern:                         return BlockLanternHandler;
		case BlockType::LapisBlock:                      return BlockLapisBlockHandler;
		case BlockType::LapisOre:                        return BlockLapisOreHandler;
		case BlockType::LargeAmethystBud:                return BlockLargeAmethystBudHandler;
		case BlockType::LargeFern:                       return BlockLargeFernHandler;
		case BlockType::Lava:                            return BlockLavaHandler;
		case BlockType::LavaCauldron:                    return BlockLavaCauldronHandler;
		case BlockType::Lectern:                         return BlockLecternHandler;
		case BlockType::Lever:                           return BlockLeverHandler;
		case BlockType::Light:                           return BlockLightHandler;
		case BlockType::LightBlueBanner:                 return BlockLightBlueBannerHandler;
		case BlockType::LightBlueBed:                    return BlockLightBlueBedHandler;
		case BlockType::LightBlueCandle:                 return BlockLightBlueCandleHandler;
		case BlockType::LightBlueCandleCake:             return BlockLightBlueCandleCakeHandler;
		case BlockType::LightBlueCarpet:                 return BlockLightBlueCarpetHandler;
		case BlockType::LightBlueConcrete:               return BlockLightBlueConcreteHandler;
		case BlockType::LightBlueConcretePowder:         return BlockLightBlueConcretePowderHandler;
		case BlockType::LightBlueGlazedTerracotta:       return BlockLightBlueGlazedTerracottaHandler;
		case BlockType::LightBlueShulkerBox:             return BlockLightBlueShulkerBoxHandler;
		case BlockType::LightBlueStainedGlass:           return BlockLightBlueStainedGlassHandler;
		case BlockType::LightBlueStainedGlassPane:       return BlockLightBlueStainedGlassPaneHandler;
		case BlockType::LightBlueTerracotta:             return BlockLightBlueTerracottaHandler;
		case BlockType::LightBlueWallBanner:             return BlockLightBlueWallBannerHandler;
		case BlockType::LightBlueWool:                   return BlockLightBlueWoolHandler;
		case BlockType::LightGrayBanner:                 return BlockLightGrayBannerHandler;
		case BlockType::LightGrayBed:                    return BlockLightGrayBedHandler;
		case BlockType::LightGrayCandle:                 return BlockLightGrayCandleHandler;
		case BlockType::LightGrayCandleCake:             return BlockLightGrayCandleCakeHandler;
		case BlockType::LightGrayCarpet:                 return BlockLightGrayCarpetHandler;
		case BlockType::LightGrayConcrete:               return BlockLightGrayConcreteHandler;
		case BlockType::LightGrayConcretePowder:         return BlockLightGrayConcretePowderHandler;
		case BlockType::LightGrayGlazedTerracotta:       return BlockLightGrayGlazedTerracottaHandler;
		case BlockType::LightGrayShulkerBox:             return BlockLightGrayShulkerBoxHandler;
		case BlockType::LightGrayStainedGlass:           return BlockLightGrayStainedGlassHandler;
		case BlockType::LightGrayStainedGlassPane:       return BlockLightGrayStainedGlassPaneHandler;
		case BlockType::LightGrayTerracotta:             return BlockLightGrayTerracottaHandler;
		case BlockType::LightGrayWallBanner:             return BlockLightGrayWallBannerHandler;
		case BlockType::LightGrayWool:                   return BlockLightGrayWoolHandler;
		case BlockType::LightWeightedPressurePlate:      return BlockLightWeightedPressurePlateHandler;
		case BlockType::LightningRod:                    return BlockLightningRodHandler;
		case BlockType::Lilac:                           return BlockLilacHandler;
		case BlockType::LilyOfTheValley:                 return BlockLilyOfTheValleyHandler;
		case BlockType::LilyPad:                         return BlockLilyPadHandler;
		case BlockType::LimeBanner:                      return BlockLimeBannerHandler;
		case BlockType::LimeBed:                         return BlockLimeBedHandler;
		case BlockType::LimeCandle:                      return BlockLimeCandleHandler;
		case BlockType::LimeCandleCake:                  return BlockLimeCandleCakeHandler;
		case BlockType::LimeCarpet:                      return BlockLimeCarpetHandler;
		case BlockType::LimeConcrete:                    return BlockLimeConcreteHandler;
		case BlockType::LimeConcretePowder:              return BlockLimeConcretePowderHandler;
		case BlockType::LimeGlazedTerracotta:            return BlockLimeGlazedTerracottaHandler;
		case BlockType::LimeShulkerBox:                  return BlockLimeShulkerBoxHandler;
		case BlockType::LimeStainedGlass:                return BlockLimeStainedGlassHandler;
		case BlockType::LimeStainedGlassPane:            return BlockLimeStainedGlassPaneHandler;
		case BlockType::LimeTerracotta:                  return BlockLimeTerracottaHandler;
		case BlockType::LimeWallBanner:                  return BlockLimeWallBannerHandler;
		case BlockType::LimeWool:                        return BlockLimeWoolHandler;
		case BlockType::Lodestone:                       return BlockLodestoneHandler;
		case BlockType::Loom:                            return BlockLoomHandler;
		case BlockType::MagentaBanner:                   return BlockMagentaBannerHandler;
		case BlockType::MagentaBed:                      return BlockMagentaBedHandler;
		case BlockType::MagentaCandle:                   return BlockMagentaCandleHandler;
		case BlockType::MagentaCandleCake:               return BlockMagentaCandleCakeHandler;
		case BlockType::MagentaCarpet:                   return BlockMagentaCarpetHandler;
		case BlockType::MagentaConcrete:                 return BlockMagentaConcreteHandler;
		case BlockType::MagentaConcretePowder:           return BlockMagentaConcretePowderHandler;
		case BlockType::MagentaGlazedTerracotta:         return BlockMagentaGlazedTerracottaHandler;
		case BlockType::MagentaShulkerBox:               return BlockMagentaShulkerBoxHandler;
		case BlockType::MagentaStainedGlass:             return BlockMagentaStainedGlassHandler;
		case BlockType::MagentaStainedGlassPane:         return BlockMagentaStainedGlassPaneHandler;
		case BlockType::MagentaTerracotta:               return BlockMagentaTerracottaHandler;
		case BlockType::MagentaWallBanner:               return BlockMagentaWallBannerHandler;
		case BlockType::MagentaWool:                     return BlockMagentaWoolHandler;
		case BlockType::MagmaBlock:                      return BlockMagmaBlockHandler;
		case BlockType::MangroveButton:                  return BlockMangroveButtonHandler;
		case BlockType::MangroveDoor:                    return BlockMangroveDoorHandler;
		case BlockType::MangroveFence:                   return BlockMangroveFenceHandler;
		case BlockType::MangroveFenceGate:               return BlockMangroveFenceGateHandler;
		case BlockType::MangroveHangingSign:             return BlockMangroveHangingSignHandler;
		case BlockType::MangroveLeaves:                  return BlockMangroveLeavesHandler;
		case BlockType::MangroveLog:                     return BlockMangroveLogHandler;
		case BlockType::MangrovePlanks:                  return BlockMangrovePlanksHandler;
		case BlockType::MangrovePressurePlate:           return BlockMangrovePressurePlateHandler;
		case BlockType::MangrovePropagule:               return BlockMangrovePropaguleHandler;
		case BlockType::MangroveRoots:                   return BlockMangroveRootsHandler;
		case BlockType::MangroveSign:                    return BlockMangroveSignHandler;
		case BlockType::MangroveSlab:                    return BlockMangroveSlabHandler;
		case BlockType::MangroveStairs:                  return BlockMangroveStairsHandler;
		case BlockType::MangroveTrapdoor:                return BlockMangroveTrapdoorHandler;
		case BlockType::MangroveWallHangingSign:         return BlockMangroveWallHangingSignHandler;
		case BlockType::MangroveWallSign:                return BlockMangroveWallSignHandler;
		case BlockType::MangroveWood:                    return BlockMangroveWoodHandler;
		case BlockType::MediumAmethystBud:               return BlockMediumAmethystBudHandler;
		case BlockType::Melon:                           return BlockMelonHandler;
		case BlockType::MelonStem:                       return BlockMelonStemHandler;
		case BlockType::MossBlock:                       return BlockMossBlockHandler;
		case BlockType::MossCarpet:                      return BlockMossCarpetHandler;
		case BlockType::MossyCobblestone:                return BlockMossyCobblestoneHandler;
		case BlockType::MossyCobblestoneSlab:            return BlockMossyCobblestoneSlabHandler;
		case BlockType::MossyCobblestoneStairs:          return BlockMossyCobblestoneStairsHandler;
		case BlockType::MossyCobblestoneWall:            return BlockMossyCobblestoneWallHandler;
		case BlockType::MossyStoneBrickSlab:             return BlockMossyStoneBrickSlabHandler;
		case BlockType::MossyStoneBrickStairs:           return BlockMossyStoneBrickStairsHandler;
		case BlockType::MossyStoneBrickWall:             return BlockMossyStoneBrickWallHandler;
		case BlockType::MossyStoneBricks:                return BlockMossyStoneBricksHandler;
		case BlockType::MovingPiston:                    return BlockMovingPistonHandler;
		case BlockType::Mud:                             return BlockMudHandler;
		case BlockType::MudBrickSlab:                    return BlockMudBrickSlabHandler;
		case BlockType::MudBrickStairs:                  return BlockMudBrickStairsHandler;
		case BlockType::MudBrickWall:                    return BlockMudBrickWallHandler;
		case BlockType::MudBricks:                       return BlockMudBricksHandler;
		case BlockType::MuddyMangroveRoots:              return BlockMuddyMangroveRootsHandler;
		case BlockType::MushroomStem:                    return BlockMushroomStemHandler;
		case BlockType::Mycelium:                        return BlockMyceliumHandler;
		case BlockType::NetherBrickFence:                return BlockNetherBrickFenceHandler;
		case BlockType::NetherBrickSlab:                 return BlockNetherBrickSlabHandler;
		case BlockType::NetherBrickStairs:               return BlockNetherBrickStairsHandler;
		case BlockType::NetherBrickWall:                 return BlockNetherBrickWallHandler;
		case BlockType::NetherBricks:                    return BlockNetherBricksHandler;
		case BlockType::NetherGoldOre:                   return BlockNetherGoldOreHandler;
		case BlockType::NetherPortal:                    return BlockNetherPortalHandler;
		case BlockType::NetherQuartzOre:                 return BlockNetherQuartzOreHandler;
		case BlockType::NetherSprouts:                   return BlockNetherSproutsHandler;
		case BlockType::NetherWart:                      return BlockNetherWartHandler;
		case BlockType::NetherWartBlock:                 return BlockNetherWartBlockHandler;
		case BlockType::NetheriteBlock:                  return BlockNetheriteBlockHandler;
		case BlockType::Netherrack:                      return BlockNetherrackHandler;
		case BlockType::NoteBlock:                       return BlockNoteBlockHandler;
		case BlockType::OakButton:                       return BlockOakButtonHandler;
		case BlockType::OakDoor:                         return BlockOakDoorHandler;
		case BlockType::OakFence:                        return BlockOakFenceHandler;
		case BlockType::OakFenceGate:                    return BlockOakFenceGateHandler;
		case BlockType::OakHangingSign:                  return BlockOakHangingSignHandler;
		case BlockType::OakLeaves:                       return BlockOakLeavesHandler;
		case BlockType::OakLog:                          return BlockOakLogHandler;
		case BlockType::OakPlanks:                       return BlockOakPlanksHandler;
		case BlockType::OakPressurePlate:                return BlockOakPressurePlateHandler;
		case BlockType::OakSapling:                      return BlockOakSaplingHandler;
		case BlockType::OakSign:                         return BlockOakSignHandler;
		case BlockType::OakSlab:                         return BlockOakSlabHandler;
		case BlockType::OakStairs:                       return BlockOakStairsHandler;
		case BlockType::OakTrapdoor:                     return BlockOakTrapdoorHandler;
		case BlockType::OakWallHangingSign:              return BlockOakWallHangingSignHandler;
		case BlockType::OakWallSign:                     return BlockOakWallSignHandler;
		case BlockType::OakWood:                         return BlockOakWoodHandler;
		case BlockType::Observer:                        return BlockObserverHandler;
		case BlockType::Obsidian:                        return BlockObsidianHandler;
		case BlockType::OchreFroglight:                  return BlockOchreFroglightHandler;
		case BlockType::OpenEyeblossom:                  return BlockOpenEyeblossomHandler;
		case BlockType::OrangeBanner:                    return BlockOrangeBannerHandler;
		case BlockType::OrangeBed:                       return BlockOrangeBedHandler;
		case BlockType::OrangeCandle:                    return BlockOrangeCandleHandler;
		case BlockType::OrangeCandleCake:                return BlockOrangeCandleCakeHandler;
		case BlockType::OrangeCarpet:                    return BlockOrangeCarpetHandler;
		case BlockType::OrangeConcrete:                  return BlockOrangeConcreteHandler;
		case BlockType::OrangeConcretePowder:            return BlockOrangeConcretePowderHandler;
		case BlockType::OrangeGlazedTerracotta:          return BlockOrangeGlazedTerracottaHandler;
		case BlockType::OrangeShulkerBox:                return BlockOrangeShulkerBoxHandler;
		case BlockType::OrangeStainedGlass:              return BlockOrangeStainedGlassHandler;
		case BlockType::OrangeStainedGlassPane:          return BlockOrangeStainedGlassPaneHandler;
		case BlockType::OrangeTerracotta:                return BlockOrangeTerracottaHandler;
		case BlockType::OrangeTulip:                     return BlockOrangeTulipHandler;
		case BlockType::OrangeWallBanner:                return BlockOrangeWallBannerHandler;
		case BlockType::OrangeWool:                      return BlockOrangeWoolHandler;
		case BlockType::OxeyeDaisy:                      return BlockOxeyeDaisyHandler;
		case BlockType::OxidizedChiseledCopper:          return BlockOxidizedChiseledCopperHandler;
		case BlockType::OxidizedCopper:                  return BlockOxidizedCopperHandler;
		case BlockType::OxidizedCopperBulb:              return BlockOxidizedCopperBulbHandler;
		case BlockType::OxidizedCopperDoor:              return BlockOxidizedCopperDoorHandler;
		case BlockType::OxidizedCopperGrate:             return BlockOxidizedCopperGrateHandler;
		case BlockType::OxidizedCopperTrapdoor:          return BlockOxidizedCopperTrapdoorHandler;
		case BlockType::OxidizedCutCopper:               return BlockOxidizedCutCopperHandler;
		case BlockType::OxidizedCutCopperSlab:           return BlockOxidizedCutCopperSlabHandler;
		case BlockType::OxidizedCutCopperStairs:         return BlockOxidizedCutCopperStairsHandler;
		case BlockType::PackedIce:                       return BlockPackedIceHandler;
		case BlockType::PackedMud:                       return BlockPackedMudHandler;
		case BlockType::PaleHangingMoss:                 return BlockPaleHangingMossHandler;
		case BlockType::PaleMossBlock:                   return BlockPaleMossBlockHandler;
		case BlockType::PaleMossCarpet:                  return BlockPaleMossCarpetHandler;
		case BlockType::PaleOakButton:                   return BlockPaleOakButtonHandler;
		case BlockType::PaleOakDoor:                     return BlockPaleOakDoorHandler;
		case BlockType::PaleOakFence:                    return BlockPaleOakFenceHandler;
		case BlockType::PaleOakFenceGate:                return BlockPaleOakFenceGateHandler;
		case BlockType::PaleOakHangingSign:              return BlockPaleOakHangingSignHandler;
		case BlockType::PaleOakLeaves:                   return BlockPaleOakLeavesHandler;
		case BlockType::PaleOakLog:                      return BlockPaleOakLogHandler;
		case BlockType::PaleOakPlanks:                   return BlockPaleOakPlanksHandler;
		case BlockType::PaleOakPressurePlate:            return BlockPaleOakPressurePlateHandler;
		case BlockType::PaleOakSapling:                  return BlockPaleOakSaplingHandler;
		case BlockType::PaleOakSign:                     return BlockPaleOakSignHandler;
		case BlockType::PaleOakSlab:                     return BlockPaleOakSlabHandler;
		case BlockType::PaleOakStairs:                   return BlockPaleOakStairsHandler;
		case BlockType::PaleOakTrapdoor:                 return BlockPaleOakTrapdoorHandler;
		case BlockType::PaleOakWallHangingSign:          return BlockPaleOakWallHangingSignHandler;
		case BlockType::PaleOakWallSign:                 return BlockPaleOakWallSignHandler;
		case BlockType::PaleOakWood:                     return BlockPaleOakWoodHandler;
		case BlockType::PearlescentFroglight:            return BlockPearlescentFroglightHandler;
		case BlockType::Peony:                           return BlockPeonyHandler;
		case BlockType::PetrifiedOakSlab:                return BlockPetrifiedOakSlabHandler;
		case BlockType::PiglinHead:                      return BlockPiglinHeadHandler;
		case BlockType::PiglinWallHead:                  return BlockPiglinWallHeadHandler;
		case BlockType::PinkBanner:                      return BlockPinkBannerHandler;
		case BlockType::PinkBed:                         return BlockPinkBedHandler;
		case BlockType::PinkCandle:                      return BlockPinkCandleHandler;
		case BlockType::PinkCandleCake:                  return BlockPinkCandleCakeHandler;
		case BlockType::PinkCarpet:                      return BlockPinkCarpetHandler;
		case BlockType::PinkConcrete:                    return BlockPinkConcreteHandler;
		case BlockType::PinkConcretePowder:              return BlockPinkConcretePowderHandler;
		case BlockType::PinkGlazedTerracotta:            return BlockPinkGlazedTerracottaHandler;
		case BlockType::PinkPetals:                      return BlockPinkPetalsHandler;
		case BlockType::PinkShulkerBox:                  return BlockPinkShulkerBoxHandler;
		case BlockType::PinkStainedGlass:                return BlockPinkStainedGlassHandler;
		case BlockType::PinkStainedGlassPane:            return BlockPinkStainedGlassPaneHandler;
		case BlockType::PinkTerracotta:                  return BlockPinkTerracottaHandler;
		case BlockType::PinkTulip:                       return BlockPinkTulipHandler;
		case BlockType::PinkWallBanner:                  return BlockPinkWallBannerHandler;
		case BlockType::PinkWool:                        return BlockPinkWoolHandler;
		case BlockType::Piston:                          return BlockPistonHandler;
		case BlockType::PistonHead:                      return BlockPistonHeadHandler;
		case BlockType::PitcherCrop:                     return BlockPitcherCropHandler;
		case BlockType::PitcherPlant:                    return BlockPitcherPlantHandler;
		case BlockType::PlayerHead:                      return BlockPlayerHeadHandler;
		case BlockType::PlayerWallHead:                  return BlockPlayerWallHeadHandler;
		case BlockType::Podzol:                          return BlockPodzolHandler;
		case BlockType::PointedDripstone:                return BlockPointedDripstoneHandler;
		case BlockType::PolishedAndesite:                return BlockPolishedAndesiteHandler;
		case BlockType::PolishedAndesiteSlab:            return BlockPolishedAndesiteSlabHandler;
		case BlockType::PolishedAndesiteStairs:          return BlockPolishedAndesiteStairsHandler;
		case BlockType::PolishedBasalt:                  return BlockPolishedBasaltHandler;
		case BlockType::PolishedBlackstone:              return BlockPolishedBlackstoneHandler;
		case BlockType::PolishedBlackstoneBrickSlab:     return BlockPolishedBlackstoneBrickSlabHandler;
		case BlockType::PolishedBlackstoneBrickStairs:   return BlockPolishedBlackstoneBrickStairsHandler;
		case BlockType::PolishedBlackstoneBrickWall:     return BlockPolishedBlackstoneBrickWallHandler;
		case BlockType::PolishedBlackstoneBricks:        return BlockPolishedBlackstoneBricksHandler;
		case BlockType::PolishedBlackstoneButton:        return BlockPolishedBlackstoneButtonHandler;
		case BlockType::PolishedBlackstonePressurePlate: return BlockPolishedBlackstonePressurePlateHandler;
		case BlockType::PolishedBlackstoneSlab:          return BlockPolishedBlackstoneSlabHandler;
		case BlockType::PolishedBlackstoneStairs:        return BlockPolishedBlackstoneStairsHandler;
		case BlockType::PolishedBlackstoneWall:          return BlockPolishedBlackstoneWallHandler;
		case BlockType::PolishedDeepslate:               return BlockPolishedDeepslateHandler;
		case BlockType::PolishedDeepslateSlab:           return BlockPolishedDeepslateSlabHandler;
		case BlockType::PolishedDeepslateStairs:         return BlockPolishedDeepslateStairsHandler;
		case BlockType::PolishedDeepslateWall:           return BlockPolishedDeepslateWallHandler;
		case BlockType::PolishedDiorite:                 return BlockPolishedDioriteHandler;
		case BlockType::PolishedDioriteSlab:             return BlockPolishedDioriteSlabHandler;
		case BlockType::PolishedDioriteStairs:           return BlockPolishedDioriteStairsHandler;
		case BlockType::PolishedGranite:                 return BlockPolishedGraniteHandler;
		case BlockType::PolishedGraniteSlab:             return BlockPolishedGraniteSlabHandler;
		case BlockType::PolishedGraniteStairs:           return BlockPolishedGraniteStairsHandler;
		case BlockType::PolishedTuff:                    return BlockPolishedTuffHandler;
		case BlockType::PolishedTuffSlab:                return BlockPolishedTuffSlabHandler;
		case BlockType::PolishedTuffStairs:              return BlockPolishedTuffStairsHandler;
		case BlockType::PolishedTuffWall:                return BlockPolishedTuffWallHandler;
		case BlockType::Poppy:                           return BlockPoppyHandler;
		case BlockType::Potatoes:                        return BlockPotatoesHandler;
		case BlockType::PottedAcaciaSapling:             return BlockPottedAcaciaSaplingHandler;
		case BlockType::PottedAllium:                    return BlockPottedAlliumHandler;
		case BlockType::PottedAzaleaBush:                return BlockPottedAzaleaBushHandler;
		case BlockType::PottedAzureBluet:                return BlockPottedAzureBluetHandler;
		case BlockType::PottedBamboo:                    return BlockPottedBambooHandler;
		case BlockType::PottedBirchSapling:              return BlockPottedBirchSaplingHandler;
		case BlockType::PottedBlueOrchid:                return BlockPottedBlueOrchidHandler;
		case BlockType::PottedBrownMushroom:             return BlockPottedBrownMushroomHandler;
		case BlockType::PottedCactus:                    return BlockPottedCactusHandler;
		case BlockType::PottedCherrySapling:             return BlockPottedCherrySaplingHandler;
		case BlockType::PottedClosedEyeblossom:          return BlockPottedClosedEyeblossomHandler;
		case BlockType::PottedCornflower:                return BlockPottedCornflowerHandler;
		case BlockType::PottedCrimsonFungus:             return BlockPottedCrimsonFungusHandler;
		case BlockType::PottedCrimsonRoots:              return BlockPottedCrimsonRootsHandler;
		case BlockType::PottedDandelion:                 return BlockPottedDandelionHandler;
		case BlockType::PottedDarkOakSapling:            return BlockPottedDarkOakSaplingHandler;
		case BlockType::PottedDeadBush:                  return BlockPottedDeadBushHandler;
		case BlockType::PottedFern:                      return BlockPottedFernHandler;
		case BlockType::PottedFloweringAzaleaBush:       return BlockPottedFloweringAzaleaBushHandler;
		case BlockType::PottedJungleSapling:             return BlockPottedJungleSaplingHandler;
		case BlockType::PottedLilyOfTheValley:           return BlockPottedLilyOfTheValleyHandler;
		case BlockType::PottedMangrovePropagule:         return BlockPottedMangrovePropaguleHandler;
		case BlockType::PottedOakSapling:                return BlockPottedOakSaplingHandler;
		case BlockType::PottedOpenEyeblossom:            return BlockPottedOpenEyeblossomHandler;
		case BlockType::PottedOrangeTulip:               return BlockPottedOrangeTulipHandler;
		case BlockType::PottedOxeyeDaisy:                return BlockPottedOxeyeDaisyHandler;
		case BlockType::PottedPaleOakSapling:            return BlockPottedPaleOakSaplingHandler;
		case BlockType::PottedPinkTulip:                 return BlockPottedPinkTulipHandler;
		case BlockType::PottedPoppy:                     return BlockPottedPoppyHandler;
		case BlockType::PottedRedMushroom:               return BlockPottedRedMushroomHandler;
		case BlockType::PottedRedTulip:                  return BlockPottedRedTulipHandler;
		case BlockType::PottedSpruceSapling:             return BlockPottedSpruceSaplingHandler;
		case BlockType::PottedTorchflower:               return BlockPottedTorchflowerHandler;
		case BlockType::PottedWarpedFungus:              return BlockPottedWarpedFungusHandler;
		case BlockType::PottedWarpedRoots:               return BlockPottedWarpedRootsHandler;
		case BlockType::PottedWhiteTulip:                return BlockPottedWhiteTulipHandler;
		case BlockType::PottedWitherRose:                return BlockPottedWitherRoseHandler;
		case BlockType::PowderSnow:                      return BlockPowderSnowHandler;
		case BlockType::PowderSnowCauldron:              return BlockPowderSnowCauldronHandler;
		case BlockType::PoweredRail:                     return BlockPoweredRailHandler;
		case BlockType::Prismarine:                      return BlockPrismarineHandler;
		case BlockType::PrismarineBrickSlab:             return BlockPrismarineBrickSlabHandler;
		case BlockType::PrismarineBrickStairs:           return BlockPrismarineBrickStairsHandler;
		case BlockType::PrismarineBricks:                return BlockPrismarineBricksHandler;
		case BlockType::PrismarineSlab:                  return BlockPrismarineSlabHandler;
		case BlockType::PrismarineStairs:                return BlockPrismarineStairsHandler;
		case BlockType::PrismarineWall:                  return BlockPrismarineWallHandler;
		case BlockType::Pumpkin:                         return BlockPumpkinHandler;
		case BlockType::PumpkinStem:                     return BlockPumpkinStemHandler;
		case BlockType::PurpleBanner:                    return BlockPurpleBannerHandler;
		case BlockType::PurpleBed:                       return BlockPurpleBedHandler;
		case BlockType::PurpleCandle:                    return BlockPurpleCandleHandler;
		case BlockType::PurpleCandleCake:                return BlockPurpleCandleCakeHandler;
		case BlockType::PurpleCarpet:                    return BlockPurpleCarpetHandler;
		case BlockType::PurpleConcrete:                  return BlockPurpleConcreteHandler;
		case BlockType::PurpleConcretePowder:            return BlockPurpleConcretePowderHandler;
		case BlockType::PurpleGlazedTerracotta:          return BlockPurpleGlazedTerracottaHandler;
		case BlockType::PurpleShulkerBox:                return BlockPurpleShulkerBoxHandler;
		case BlockType::PurpleStainedGlass:              return BlockPurpleStainedGlassHandler;
		case BlockType::PurpleStainedGlassPane:          return BlockPurpleStainedGlassPaneHandler;
		case BlockType::PurpleTerracotta:                return BlockPurpleTerracottaHandler;
		case BlockType::PurpleWallBanner:                return BlockPurpleWallBannerHandler;
		case BlockType::PurpleWool:                      return BlockPurpleWoolHandler;
		case BlockType::PurpurBlock:                     return BlockPurpurBlockHandler;
		case BlockType::PurpurPillar:                    return BlockPurpurPillarHandler;
		case BlockType::PurpurSlab:                      return BlockPurpurSlabHandler;
		case BlockType::PurpurStairs:                    return BlockPurpurStairsHandler;
		case BlockType::QuartzBlock:                     return BlockQuartzBlockHandler;
		case BlockType::QuartzBricks:                    return BlockQuartzBricksHandler;
		case BlockType::QuartzPillar:                    return BlockQuartzPillarHandler;
		case BlockType::QuartzSlab:                      return BlockQuartzSlabHandler;
		case BlockType::QuartzStairs:                    return BlockQuartzStairsHandler;
		case BlockType::Rail:                            return BlockRailHandler;
		case BlockType::RawCopperBlock:                  return BlockRawCopperBlockHandler;
		case BlockType::RawGoldBlock:                    return BlockRawGoldBlockHandler;
		case BlockType::RawIronBlock:                    return BlockRawIronBlockHandler;
		case BlockType::RedBanner:                       return BlockRedBannerHandler;
		case BlockType::RedBed:                          return BlockRedBedHandler;
		case BlockType::RedCandle:                       return BlockRedCandleHandler;
		case BlockType::RedCandleCake:                   return BlockRedCandleCakeHandler;
		case BlockType::RedCarpet:                       return BlockRedCarpetHandler;
		case BlockType::RedConcrete:                     return BlockRedConcreteHandler;
		case BlockType::RedConcretePowder:               return BlockRedConcretePowderHandler;
		case BlockType::RedGlazedTerracotta:             return BlockRedGlazedTerracottaHandler;
		case BlockType::RedMushroom:                     return BlockRedMushroomHandler;
		case BlockType::RedMushroomBlock:                return BlockRedMushroomBlockHandler;
		case BlockType::RedNetherBrickSlab:              return BlockRedNetherBrickSlabHandler;
		case BlockType::RedNetherBrickStairs:            return BlockRedNetherBrickStairsHandler;
		case BlockType::RedNetherBrickWall:              return BlockRedNetherBrickWallHandler;
		case BlockType::RedNetherBricks:                 return BlockRedNetherBricksHandler;
		case BlockType::RedSand:                         return BlockRedSandHandler;
		case BlockType::RedSandstone:                    return BlockRedSandstoneHandler;
		case BlockType::RedSandstoneSlab:                return BlockRedSandstoneSlabHandler;
		case BlockType::RedSandstoneStairs:              return BlockRedSandstoneStairsHandler;
		case BlockType::RedSandstoneWall:                return BlockRedSandstoneWallHandler;
		case BlockType::RedShulkerBox:                   return BlockRedShulkerBoxHandler;
		case BlockType::RedStainedGlass:                 return BlockRedStainedGlassHandler;
		case BlockType::RedStainedGlassPane:             return BlockRedStainedGlassPaneHandler;
		case BlockType::RedTerracotta:                   return BlockRedTerracottaHandler;
		case BlockType::RedTulip:                        return BlockRedTulipHandler;
		case BlockType::RedWallBanner:                   return BlockRedWallBannerHandler;
		case BlockType::RedWool:                         return BlockRedWoolHandler;
		case BlockType::RedstoneBlock:                   return BlockRedstoneBlockHandler;
		case BlockType::RedstoneLamp:                    return BlockRedstoneLampHandler;
		case BlockType::RedstoneOre:                     return BlockRedstoneOreHandler;
		case BlockType::RedstoneTorch:                   return BlockRedstoneTorchHandler;
		case BlockType::RedstoneWallTorch:               return BlockRedstoneWallTorchHandler;
		case BlockType::RedstoneWire:                    return BlockRedstoneWireHandler;
		case BlockType::ReinforcedDeepslate:             return BlockReinforcedDeepslateHandler;
		case BlockType::Repeater:                        return BlockRepeaterHandler;
		case BlockType::RepeatingCommandBlock:           return BlockRepeatingCommandBlockHandler;
		case BlockType::ResinBlock:                      return BlockResinBlockHandler;
		case BlockType::ResinBrickSlab:                  return BlockResinBrickSlabHandler;
		case BlockType::ResinBrickStairs:                return BlockResinBrickStairsHandler;
		case BlockType::ResinBrickWall:                  return BlockResinBrickWallHandler;
		case BlockType::ResinBricks:                     return BlockResinBricksHandler;
		case BlockType::ResinClump:                      return BlockResinClumpHandler;
		case BlockType::RespawnAnchor:                   return BlockRespawnAnchorHandler;
		case BlockType::RootedDirt:                      return BlockRootedDirtHandler;
		case BlockType::RoseBush:                        return BlockRoseBushHandler;
		case BlockType::Sand:                            return BlockSandHandler;
		case BlockType::Sandstone:                       return BlockSandstoneHandler;
		case BlockType::SandstoneSlab:                   return BlockSandstoneSlabHandler;
		case BlockType::SandstoneStairs:                 return BlockSandstoneStairsHandler;
		case BlockType::SandstoneWall:                   return BlockSandstoneWallHandler;
		case BlockType::Scaffolding:                     return BlockScaffoldingHandler;
		case BlockType::Sculk:                           return BlockSculkHandler;
		case BlockType::SculkCatalyst:                   return BlockSculkCatalystHandler;
		case BlockType::SculkSensor:                     return BlockSculkSensorHandler;
		case BlockType::SculkShrieker:                   return BlockSculkShriekerHandler;
		case BlockType::SculkVein:                       return BlockSculkVeinHandler;
		case BlockType::SeaLantern:                      return BlockSeaLanternHandler;
		case BlockType::SeaPickle:                       return BlockSeaPickleHandler;
		case BlockType::Seagrass:                        return BlockSeagrassHandler;
		case BlockType::ShortGrass:                      return BlockShortGrassHandler;
		case BlockType::Shroomlight:                     return BlockShroomlightHandler;
		case BlockType::ShulkerBox:                      return BlockShulkerBoxHandler;
		case BlockType::SkeletonSkull:                   return BlockSkeletonSkullHandler;
		case BlockType::SkeletonWallSkull:               return BlockSkeletonWallSkullHandler;
		case BlockType::SlimeBlock:                      return BlockSlimeBlockHandler;
		case BlockType::SmallAmethystBud:                return BlockSmallAmethystBudHandler;
		case BlockType::SmallDripleaf:                   return BlockSmallDripleafHandler;
		case BlockType::SmithingTable:                   return BlockSmithingTableHandler;
		case BlockType::Smoker:                          return BlockSmokerHandler;
		case BlockType::SmoothBasalt:                    return BlockSmoothBasaltHandler;
		case BlockType::SmoothQuartz:                    return BlockSmoothQuartzHandler;
		case BlockType::SmoothQuartzSlab:                return BlockSmoothQuartzSlabHandler;
		case BlockType::SmoothQuartzStairs:              return BlockSmoothQuartzStairsHandler;
		case BlockType::SmoothRedSandstone:              return BlockSmoothRedSandstoneHandler;
		case BlockType::SmoothRedSandstoneSlab:          return BlockSmoothRedSandstoneSlabHandler;
		case BlockType::SmoothRedSandstoneStairs:        return BlockSmoothRedSandstoneStairsHandler;
		case BlockType::SmoothSandstone:                 return BlockSmoothSandstoneHandler;
		case BlockType::SmoothSandstoneSlab:             return BlockSmoothSandstoneSlabHandler;
		case BlockType::SmoothSandstoneStairs:           return BlockSmoothSandstoneStairsHandler;
		case BlockType::SmoothStone:                     return BlockSmoothStoneHandler;
		case BlockType::SmoothStoneSlab:                 return BlockSmoothStoneSlabHandler;
		case BlockType::SnifferEgg:                      return BlockSnifferEggHandler;
		case BlockType::Snow:                            return BlockSnowHandler;
		case BlockType::SnowBlock:                       return BlockSnowBlockHandler;
		case BlockType::SoulCampfire:                    return BlockSoulCampfireHandler;
		case BlockType::SoulFire:                        return BlockSoulFireHandler;
		case BlockType::SoulLantern:                     return BlockSoulLanternHandler;
		case BlockType::SoulSand:                        return BlockSoulSandHandler;
		case BlockType::SoulSoil:                        return BlockSoulSoilHandler;
		case BlockType::SoulTorch:                       return BlockSoulTorchHandler;
		case BlockType::SoulWallTorch:                   return BlockSoulWallTorchHandler;
		case BlockType::Spawner:                         return BlockSpawnerHandler;
		case BlockType::Sponge:                          return BlockSpongeHandler;
		case BlockType::SporeBlossom:                    return BlockSporeBlossomHandler;
		case BlockType::SpruceButton:                    return BlockSpruceButtonHandler;
		case BlockType::SpruceDoor:                      return BlockSpruceDoorHandler;
		case BlockType::SpruceFence:                     return BlockSpruceFenceHandler;
		case BlockType::SpruceFenceGate:                 return BlockSpruceFenceGateHandler;
		case BlockType::SpruceHangingSign:               return BlockSpruceHangingSignHandler;
		case BlockType::SpruceLeaves:                    return BlockSpruceLeavesHandler;
		case BlockType::SpruceLog:                       return BlockSpruceLogHandler;
		case BlockType::SprucePlanks:                    return BlockSprucePlanksHandler;
		case BlockType::SprucePressurePlate:             return BlockSprucePressurePlateHandler;
		case BlockType::SpruceSapling:                   return BlockSpruceSaplingHandler;
		case BlockType::SpruceSign:                      return BlockSpruceSignHandler;
		case BlockType::SpruceSlab:                      return BlockSpruceSlabHandler;
		case BlockType::SpruceStairs:                    return BlockSpruceStairsHandler;
		case BlockType::SpruceTrapdoor:                  return BlockSpruceTrapdoorHandler;
		case BlockType::SpruceWallHangingSign:           return BlockSpruceWallHangingSignHandler;
		case BlockType::SpruceWallSign:                  return BlockSpruceWallSignHandler;
		case BlockType::SpruceWood:                      return BlockSpruceWoodHandler;
		case BlockType::StickyPiston:                    return BlockStickyPistonHandler;
		case BlockType::Stone:                           return BlockStoneHandler;
		case BlockType::StoneBrickSlab:                  return BlockStoneBrickSlabHandler;
		case BlockType::StoneBrickStairs:                return BlockStoneBrickStairsHandler;
		case BlockType::StoneBrickWall:                  return BlockStoneBrickWallHandler;
		case BlockType::StoneBricks:                     return BlockStoneBricksHandler;
		case BlockType::StoneButton:                     return BlockStoneButtonHandler;
		case BlockType::StonePressurePlate:              return BlockStonePressurePlateHandler;
		case BlockType::StoneSlab:                       return BlockStoneSlabHandler;
		case BlockType::StoneStairs:                     return BlockStoneStairsHandler;
		case BlockType::Stonecutter:                     return BlockStonecutterHandler;
		case BlockType::StrippedAcaciaLog:               return BlockStrippedAcaciaLogHandler;
		case BlockType::StrippedAcaciaWood:              return BlockStrippedAcaciaWoodHandler;
		case BlockType::StrippedBambooBlock:             return BlockStrippedBambooBlockHandler;
		case BlockType::StrippedBirchLog:                return BlockStrippedBirchLogHandler;
		case BlockType::StrippedBirchWood:               return BlockStrippedBirchWoodHandler;
		case BlockType::StrippedCherryLog:               return BlockStrippedCherryLogHandler;
		case BlockType::StrippedCherryWood:              return BlockStrippedCherryWoodHandler;
		case BlockType::StrippedCrimsonHyphae:           return BlockStrippedCrimsonHyphaeHandler;
		case BlockType::StrippedCrimsonStem:             return BlockStrippedCrimsonStemHandler;
		case BlockType::StrippedDarkOakLog:              return BlockStrippedDarkOakLogHandler;
		case BlockType::StrippedDarkOakWood:             return BlockStrippedDarkOakWoodHandler;
		case BlockType::StrippedJungleLog:               return BlockStrippedJungleLogHandler;
		case BlockType::StrippedJungleWood:              return BlockStrippedJungleWoodHandler;
		case BlockType::StrippedMangroveLog:             return BlockStrippedMangroveLogHandler;
		case BlockType::StrippedMangroveWood:            return BlockStrippedMangroveWoodHandler;
		case BlockType::StrippedOakLog:                  return BlockStrippedOakLogHandler;
		case BlockType::StrippedOakWood:                 return BlockStrippedOakWoodHandler;
		case BlockType::StrippedPaleOakLog:              return BlockStrippedPaleOakLogHandler;
		case BlockType::StrippedPaleOakWood:             return BlockStrippedPaleOakWoodHandler;
		case BlockType::StrippedSpruceLog:               return BlockStrippedSpruceLogHandler;
		case BlockType::StrippedSpruceWood:              return BlockStrippedSpruceWoodHandler;
		case BlockType::StrippedWarpedHyphae:            return BlockStrippedWarpedHyphaeHandler;
		case BlockType::StrippedWarpedStem:              return BlockStrippedWarpedStemHandler;
		case BlockType::StructureBlock:                  return BlockStructureBlockHandler;
		case BlockType::StructureVoid:                   return BlockStructureVoidHandler;
		case BlockType::SugarCane:                       return BlockSugarCaneHandler;
		case BlockType::Sunflower:                       return BlockSunflowerHandler;
		case BlockType::SuspiciousGravel:                return BlockSuspiciousGravelHandler;
		case BlockType::SuspiciousSand:                  return BlockSuspiciousSandHandler;
		case BlockType::SweetBerryBush:                  return BlockSweetBerryBushHandler;
		case BlockType::TallGrass:                       return BlockTallGrassHandler;
		case BlockType::TallSeagrass:                    return BlockTallSeagrassHandler;
		case BlockType::Target:                          return BlockTargetHandler;
		case BlockType::Terracotta:                      return BlockTerracottaHandler;
		case BlockType::TintedGlass:                     return BlockTintedGlassHandler;
		case BlockType::Tnt:                             return BlockTntHandler;
		case BlockType::Torch:                           return BlockTorchHandler;
		case BlockType::Torchflower:                     return BlockTorchflowerHandler;
		case BlockType::TorchflowerCrop:                 return BlockTorchflowerCropHandler;
		case BlockType::TrappedChest:                    return BlockTrappedChestHandler;
		case BlockType::TrialSpawner:                    return BlockTrialSpawnerHandler;
		case BlockType::Tripwire:                        return BlockTripwireHandler;
		case BlockType::TripwireHook:                    return BlockTripwireHookHandler;
		case BlockType::TubeCoral:                       return BlockTubeCoralHandler;
		case BlockType::TubeCoralBlock:                  return BlockTubeCoralBlockHandler;
		case BlockType::TubeCoralFan:                    return BlockTubeCoralFanHandler;
		case BlockType::TubeCoralWallFan:                return BlockTubeCoralWallFanHandler;
		case BlockType::Tuff:                            return BlockTuffHandler;
		case BlockType::TuffBrickSlab:                   return BlockTuffBrickSlabHandler;
		case BlockType::TuffBrickStairs:                 return BlockTuffBrickStairsHandler;
		case BlockType::TuffBrickWall:                   return BlockTuffBrickWallHandler;
		case BlockType::TuffBricks:                      return BlockTuffBricksHandler;
		case BlockType::TuffSlab:                        return BlockTuffSlabHandler;
		case BlockType::TuffStairs:                      return BlockTuffStairsHandler;
		case BlockType::TuffWall:                        return BlockTuffWallHandler;
		case BlockType::TurtleEgg:                       return BlockTurtleEggHandler;
		case BlockType::TwistingVines:                   return BlockTwistingVinesHandler;
		case BlockType::TwistingVinesPlant:              return BlockTwistingVinesPlantHandler;
		case BlockType::Vault:                           return BlockVaultHandler;
		case BlockType::VerdantFroglight:                return BlockVerdantFroglightHandler;
		case BlockType::Vine:                            return BlockVineHandler;
		case BlockType::VoidAir:                         return BlockVoidAirHandler;
		case BlockType::WallTorch:                       return BlockWallTorchHandler;
		case BlockType::WarpedButton:                    return BlockWarpedButtonHandler;
		case BlockType::WarpedDoor:                      return BlockWarpedDoorHandler;
		case BlockType::WarpedFence:                     return BlockWarpedFenceHandler;
		case BlockType::WarpedFenceGate:                 return BlockWarpedFenceGateHandler;
		case BlockType::WarpedFungus:                    return BlockWarpedFungusHandler;
		case BlockType::WarpedHangingSign:               return BlockWarpedHangingSignHandler;
		case BlockType::WarpedHyphae:                    return BlockWarpedHyphaeHandler;
		case BlockType::WarpedNylium:                    return BlockWarpedNyliumHandler;
		case BlockType::WarpedPlanks:                    return BlockWarpedPlanksHandler;
		case BlockType::WarpedPressurePlate:             return BlockWarpedPressurePlateHandler;
		case BlockType::WarpedRoots:                     return BlockWarpedRootsHandler;
		case BlockType::WarpedSign:                      return BlockWarpedSignHandler;
		case BlockType::WarpedSlab:                      return BlockWarpedSlabHandler;
		case BlockType::WarpedStairs:                    return BlockWarpedStairsHandler;
		case BlockType::WarpedStem:                      return BlockWarpedStemHandler;
		case BlockType::WarpedTrapdoor:                  return BlockWarpedTrapdoorHandler;
		case BlockType::WarpedWallHangingSign:           return BlockWarpedWallHangingSignHandler;
		case BlockType::WarpedWallSign:                  return BlockWarpedWallSignHandler;
		case BlockType::WarpedWartBlock:                 return BlockWarpedWartBlockHandler;
		case BlockType::Water:                           return BlockWaterHandler;
		case BlockType::WaterCauldron:                   return BlockWaterCauldronHandler;
		case BlockType::WaxedChiseledCopper:             return BlockWaxedChiseledCopperHandler;
		case BlockType::WaxedCopperBlock:                return BlockWaxedCopperBlockHandler;
		case BlockType::WaxedCopperBulb:                 return BlockWaxedCopperBulbHandler;
		case BlockType::WaxedCopperDoor:                 return BlockWaxedCopperDoorHandler;
		case BlockType::WaxedCopperGrate:                return BlockWaxedCopperGrateHandler;
		case BlockType::WaxedCopperTrapdoor:             return BlockWaxedCopperTrapdoorHandler;
		case BlockType::WaxedCutCopper:                  return BlockWaxedCutCopperHandler;
		case BlockType::WaxedCutCopperSlab:              return BlockWaxedCutCopperSlabHandler;
		case BlockType::WaxedCutCopperStairs:            return BlockWaxedCutCopperStairsHandler;
		case BlockType::WaxedExposedChiseledCopper:      return BlockWaxedExposedChiseledCopperHandler;
		case BlockType::WaxedExposedCopper:              return BlockWaxedExposedCopperHandler;
		case BlockType::WaxedExposedCopperBulb:          return BlockWaxedExposedCopperBulbHandler;
		case BlockType::WaxedExposedCopperDoor:          return BlockWaxedExposedCopperDoorHandler;
		case BlockType::WaxedExposedCopperGrate:         return BlockWaxedExposedCopperGrateHandler;
		case BlockType::WaxedExposedCopperTrapdoor:      return BlockWaxedExposedCopperTrapdoorHandler;
		case BlockType::WaxedExposedCutCopper:           return BlockWaxedExposedCutCopperHandler;
		case BlockType::WaxedExposedCutCopperSlab:       return BlockWaxedExposedCutCopperSlabHandler;
		case BlockType::WaxedExposedCutCopperStairs:     return BlockWaxedExposedCutCopperStairsHandler;
		case BlockType::WaxedOxidizedChiseledCopper:     return BlockWaxedOxidizedChiseledCopperHandler;
		case BlockType::WaxedOxidizedCopper:             return BlockWaxedOxidizedCopperHandler;
		case BlockType::WaxedOxidizedCopperBulb:         return BlockWaxedOxidizedCopperBulbHandler;
		case BlockType::WaxedOxidizedCopperDoor:         return BlockWaxedOxidizedCopperDoorHandler;
		case BlockType::WaxedOxidizedCopperGrate:        return BlockWaxedOxidizedCopperGrateHandler;
		case BlockType::WaxedOxidizedCopperTrapdoor:     return BlockWaxedOxidizedCopperTrapdoorHandler;
		case BlockType::WaxedOxidizedCutCopper:          return BlockWaxedOxidizedCutCopperHandler;
		case BlockType::WaxedOxidizedCutCopperSlab:      return BlockWaxedOxidizedCutCopperSlabHandler;
		case BlockType::WaxedOxidizedCutCopperStairs:    return BlockWaxedOxidizedCutCopperStairsHandler;
		case BlockType::WaxedWeatheredChiseledCopper:    return BlockWaxedWeatheredChiseledCopperHandler;
		case BlockType::WaxedWeatheredCopper:            return BlockWaxedWeatheredCopperHandler;
		case BlockType::WaxedWeatheredCopperBulb:        return BlockWaxedWeatheredCopperBulbHandler;
		case BlockType::WaxedWeatheredCopperDoor:        return BlockWaxedWeatheredCopperDoorHandler;
		case BlockType::WaxedWeatheredCopperGrate:       return BlockWaxedWeatheredCopperGrateHandler;
		case BlockType::WaxedWeatheredCopperTrapdoor:    return BlockWaxedWeatheredCopperTrapdoorHandler;
		case BlockType::WaxedWeatheredCutCopper:         return BlockWaxedWeatheredCutCopperHandler;
		case BlockType::WaxedWeatheredCutCopperSlab:     return BlockWaxedWeatheredCutCopperSlabHandler;
		case BlockType::WaxedWeatheredCutCopperStairs:   return BlockWaxedWeatheredCutCopperStairsHandler;
		case BlockType::WeatheredChiseledCopper:         return BlockWeatheredChiseledCopperHandler;
		case BlockType::WeatheredCopper:                 return BlockWeatheredCopperHandler;
		case BlockType::WeatheredCopperBulb:             return BlockWeatheredCopperBulbHandler;
		case BlockType::WeatheredCopperDoor:             return BlockWeatheredCopperDoorHandler;
		case BlockType::WeatheredCopperGrate:            return BlockWeatheredCopperGrateHandler;
		case BlockType::WeatheredCopperTrapdoor:         return BlockWeatheredCopperTrapdoorHandler;
		case BlockType::WeatheredCutCopper:              return BlockWeatheredCutCopperHandler;
		case BlockType::WeatheredCutCopperSlab:          return BlockWeatheredCutCopperSlabHandler;
		case BlockType::WeatheredCutCopperStairs:        return BlockWeatheredCutCopperStairsHandler;
		case BlockType::WeepingVines:                    return BlockWeepingVinesHandler;
		case BlockType::WeepingVinesPlant:               return BlockWeepingVinesPlantHandler;
		case BlockType::WetSponge:                       return BlockWetSpongeHandler;
		case BlockType::Wheat:                           return BlockWheatHandler;
		case BlockType::WhiteBanner:                     return BlockWhiteBannerHandler;
		case BlockType::WhiteBed:                        return BlockWhiteBedHandler;
		case BlockType::WhiteCandle:                     return BlockWhiteCandleHandler;
		case BlockType::WhiteCandleCake:                 return BlockWhiteCandleCakeHandler;
		case BlockType::WhiteCarpet:                     return BlockWhiteCarpetHandler;
		case BlockType::WhiteConcrete:                   return BlockWhiteConcreteHandler;
		case BlockType::WhiteConcretePowder:             return BlockWhiteConcretePowderHandler;
		case BlockType::WhiteGlazedTerracotta:           return BlockWhiteGlazedTerracottaHandler;
		case BlockType::WhiteShulkerBox:                 return BlockWhiteShulkerBoxHandler;
		case BlockType::WhiteStainedGlass:               return BlockWhiteStainedGlassHandler;
		case BlockType::WhiteStainedGlassPane:           return BlockWhiteStainedGlassPaneHandler;
		case BlockType::WhiteTerracotta:                 return BlockWhiteTerracottaHandler;
		case BlockType::WhiteTulip:                      return BlockWhiteTulipHandler;
		case BlockType::WhiteWallBanner:                 return BlockWhiteWallBannerHandler;
		case BlockType::WhiteWool:                       return BlockWhiteWoolHandler;
		case BlockType::WitherRose:                      return BlockWitherRoseHandler;
		case BlockType::WitherSkeletonSkull:             return BlockWitherSkeletonSkullHandler;
		case BlockType::WitherSkeletonWallSkull:         return BlockWitherSkeletonWallSkullHandler;
		case BlockType::YellowBanner:                    return BlockYellowBannerHandler;
		case BlockType::YellowBed:                       return BlockYellowBedHandler;
		case BlockType::YellowCandle:                    return BlockYellowCandleHandler;
		case BlockType::YellowCandleCake:                return BlockYellowCandleCakeHandler;
		case BlockType::YellowCarpet:                    return BlockYellowCarpetHandler;
		case BlockType::YellowConcrete:                  return BlockYellowConcreteHandler;
		case BlockType::YellowConcretePowder:            return BlockYellowConcretePowderHandler;
		case BlockType::YellowGlazedTerracotta:          return BlockYellowGlazedTerracottaHandler;
		case BlockType::YellowShulkerBox:                return BlockYellowShulkerBoxHandler;
		case BlockType::YellowStainedGlass:              return BlockYellowStainedGlassHandler;
		case BlockType::YellowStainedGlassPane:          return BlockYellowStainedGlassPaneHandler;
		case BlockType::YellowTerracotta:                return BlockYellowTerracottaHandler;
		case BlockType::YellowWallBanner:                return BlockYellowWallBannerHandler;
		case BlockType::YellowWool:                      return BlockYellowWoolHandler;
		case BlockType::ZombieHead:                      return BlockZombieHeadHandler;
		case BlockType::ZombieWallHead:                  return BlockZombieWallHeadHandler;
	}
	UNREACHABLE("Getting handler for unexpected block type");
}
