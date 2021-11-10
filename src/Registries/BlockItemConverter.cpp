
#include "Globals.h"
#include "BlockItemConverter.h"

namespace BlockItemConverter
{
	BlockType FromItem(Item a_Item)
	{
		switch (a_Item)
		{
			case Item::AcaciaButton:                    return BlockType::AcaciaButton;
			case Item::AcaciaDoor:                      return BlockType::AcaciaDoor;
			case Item::AcaciaFence:                     return BlockType::AcaciaFence;
			case Item::AcaciaFenceGate:                 return BlockType::AcaciaFenceGate;
			case Item::AcaciaLeaves:                    return BlockType::AcaciaLeaves;
			case Item::AcaciaLog:                       return BlockType::AcaciaLog;
			case Item::AcaciaPlanks:                    return BlockType::AcaciaPlanks;
			case Item::AcaciaPressurePlate:             return BlockType::AcaciaPressurePlate;
			case Item::AcaciaSapling:                   return BlockType::AcaciaSapling;
			case Item::AcaciaSign:                      return BlockType::AcaciaSign;
			case Item::AcaciaSlab:                      return BlockType::AcaciaSlab;
			case Item::AcaciaStairs:                    return BlockType::AcaciaStairs;
			case Item::AcaciaTrapdoor:                  return BlockType::AcaciaTrapdoor;
			case Item::AcaciaWood:                      return BlockType::AcaciaWood;
			case Item::ActivatorRail:                   return BlockType::ActivatorRail;
			case Item::Air:                             return BlockType::Air;
			case Item::Allium:                          return BlockType::Allium;
			case Item::AncientDebris:                   return BlockType::AncientDebris;
			case Item::Andesite:                        return BlockType::Andesite;
			case Item::AndesiteSlab:                    return BlockType::AndesiteSlab;
			case Item::AndesiteStairs:                  return BlockType::AndesiteStairs;
			case Item::AndesiteWall:                    return BlockType::AndesiteWall;
			case Item::Anvil:                           return BlockType::Anvil;
			case Item::AzureBluet:                      return BlockType::AzureBluet;
			case Item::Bamboo:                          return BlockType::Bamboo;
			case Item::Barrel:                          return BlockType::Barrel;
			case Item::Barrier:                         return BlockType::Barrier;
			case Item::Basalt:                          return BlockType::Basalt;
			case Item::Beacon:                          return BlockType::Beacon;
			case Item::Bedrock:                         return BlockType::Bedrock;
			case Item::BeeNest:                         return BlockType::BeeNest;
			case Item::Beehive:                         return BlockType::Beehive;
			case Item::Beetroot:                        return BlockType::Beetroots;
			case Item::Bell:                            return BlockType::Bell;
			case Item::BirchButton:                     return BlockType::BirchButton;
			case Item::BirchDoor:                       return BlockType::BirchDoor;
			case Item::BirchFence:                      return BlockType::BirchFence;
			case Item::BirchFenceGate:                  return BlockType::BirchFenceGate;
			case Item::BirchLeaves:                     return BlockType::BirchLeaves;
			case Item::BirchLog:                        return BlockType::BirchLog;
			case Item::BirchPlanks:                     return BlockType::BirchPlanks;
			case Item::BirchPressurePlate:              return BlockType::BirchPressurePlate;
			case Item::BirchSapling:                    return BlockType::BirchSapling;
			case Item::BirchSign:                       return BlockType::BirchSign;
			case Item::BirchSlab:                       return BlockType::BirchSlab;
			case Item::BirchStairs:                     return BlockType::BirchStairs;
			case Item::BirchTrapdoor:                   return BlockType::BirchTrapdoor;
			case Item::BirchWood:                       return BlockType::BirchWood;
			case Item::BlackBanner:                     return BlockType::BlackBanner;
			case Item::BlackBed:                        return BlockType::BlackBed;
			case Item::BlackCarpet:                     return BlockType::BlackCarpet;
			case Item::BlackConcrete:                   return BlockType::BlackConcrete;
			case Item::BlackConcretePowder:             return BlockType::BlackConcretePowder;
			case Item::BlackGlazedTerracotta:           return BlockType::BlackGlazedTerracotta;
			case Item::BlackShulkerBox:                 return BlockType::BlackShulkerBox;
			case Item::BlackStainedGlass:               return BlockType::BlackStainedGlass;
			case Item::BlackStainedGlassPane:           return BlockType::BlackStainedGlassPane;
			case Item::BlackTerracotta:                 return BlockType::BlackTerracotta;
			case Item::BlackWool:                       return BlockType::BlackWool;
			case Item::Blackstone:                      return BlockType::Blackstone;
			case Item::BlackstoneSlab:                  return BlockType::BlackstoneSlab;
			case Item::BlackstoneStairs:                return BlockType::BlackstoneStairs;
			case Item::BlackstoneWall:                  return BlockType::BlackstoneWall;
			case Item::BlastFurnace:                    return BlockType::BlastFurnace;
			case Item::BlueBanner:                      return BlockType::BlueBanner;
			case Item::BlueBed:                         return BlockType::BlueBed;
			case Item::BlueCarpet:                      return BlockType::BlueCarpet;
			case Item::BlueConcrete:                    return BlockType::BlueConcrete;
			case Item::BlueConcretePowder:              return BlockType::BlueConcretePowder;
			case Item::BlueGlazedTerracotta:            return BlockType::BlueGlazedTerracotta;
			case Item::BlueIce:                         return BlockType::BlueIce;
			case Item::BlueOrchid:                      return BlockType::BlueOrchid;
			case Item::BlueShulkerBox:                  return BlockType::BlueShulkerBox;
			case Item::BlueStainedGlass:                return BlockType::BlueStainedGlass;
			case Item::BlueStainedGlassPane:            return BlockType::BlueStainedGlassPane;
			case Item::BlueTerracotta:                  return BlockType::BlueTerracotta;
			case Item::BlueWool:                        return BlockType::BlueWool;
			case Item::BoneBlock:                       return BlockType::BoneBlock;
			case Item::Bookshelf:                       return BlockType::Bookshelf;
			case Item::BrainCoral:                      return BlockType::BrainCoral;
			case Item::BrainCoralBlock:                 return BlockType::BrainCoralBlock;
			case Item::BrainCoralFan:                   return BlockType::BrainCoralFan;
			case Item::BrewingStand:                    return BlockType::BrewingStand;
			case Item::BrickSlab:                       return BlockType::BrickSlab;
			case Item::BrickStairs:                     return BlockType::BrickStairs;
			case Item::BrickWall:                       return BlockType::BrickWall;
			case Item::Bricks:                          return BlockType::Bricks;
			case Item::BrownBanner:                     return BlockType::BrownBanner;
			case Item::BrownBed:                        return BlockType::BrownBed;
			case Item::BrownCarpet:                     return BlockType::BrownCarpet;
			case Item::BrownConcrete:                   return BlockType::BrownConcrete;
			case Item::BrownConcretePowder:             return BlockType::BrownConcretePowder;
			case Item::BrownGlazedTerracotta:           return BlockType::BrownGlazedTerracotta;
			case Item::BrownMushroom:                   return BlockType::BrownMushroom;
			case Item::BrownMushroomBlock:              return BlockType::BrownMushroomBlock;
			case Item::BrownShulkerBox:                 return BlockType::BrownShulkerBox;
			case Item::BrownStainedGlass:               return BlockType::BrownStainedGlass;
			case Item::BrownStainedGlassPane:           return BlockType::BrownStainedGlassPane;
			case Item::BrownTerracotta:                 return BlockType::BrownTerracotta;
			case Item::BrownWool:                       return BlockType::BrownWool;
			case Item::BubbleCoral:                     return BlockType::BubbleCoral;
			case Item::BubbleCoralBlock:                return BlockType::BubbleCoralBlock;
			case Item::BubbleCoralFan:                  return BlockType::BubbleCoralFan;
			case Item::Cactus:                          return BlockType::Cactus;
			case Item::Cake:                            return BlockType::Cake;
			case Item::Campfire:                        return BlockType::Campfire;
			case Item::Carrot:                          return BlockType::Carrots;
			case Item::CartographyTable:                return BlockType::CartographyTable;
			case Item::CarvedPumpkin:                   return BlockType::CarvedPumpkin;
			case Item::Cauldron:                        return BlockType::Cauldron;
			case Item::Chain:                           return BlockType::Chain;
			case Item::ChainCommandBlock:               return BlockType::ChainCommandBlock;
			case Item::Chest:                           return BlockType::Chest;
			case Item::ChippedAnvil:                    return BlockType::ChippedAnvil;
			case Item::ChiseledNetherBricks:            return BlockType::ChiseledNetherBricks;
			case Item::ChiseledPolishedBlackstone:      return BlockType::ChiseledPolishedBlackstone;
			case Item::ChiseledQuartzBlock:             return BlockType::ChiseledQuartzBlock;
			case Item::ChiseledRedSandstone:            return BlockType::ChiseledRedSandstone;
			case Item::ChiseledSandstone:               return BlockType::ChiseledSandstone;
			case Item::ChiseledStoneBricks:             return BlockType::ChiseledStoneBricks;
			case Item::ChorusFlower:                    return BlockType::ChorusFlower;
			case Item::ChorusPlant:                     return BlockType::ChorusPlant;
			case Item::Clay:                            return BlockType::Clay;
			case Item::CoalBlock:                       return BlockType::CoalBlock;
			case Item::CoalOre:                         return BlockType::CoalOre;
			case Item::CoarseDirt:                      return BlockType::CoarseDirt;
			case Item::Cobblestone:                     return BlockType::Cobblestone;
			case Item::CobblestoneSlab:                 return BlockType::CobblestoneSlab;
			case Item::CobblestoneStairs:               return BlockType::CobblestoneStairs;
			case Item::CobblestoneWall:                 return BlockType::CobblestoneWall;
			case Item::Cobweb:                          return BlockType::Cobweb;
			case Item::CocoaBeans:                      return BlockType::Cocoa;
			case Item::CommandBlock:                    return BlockType::CommandBlock;
			case Item::Comparator:                      return BlockType::Comparator;
			case Item::Composter:                       return BlockType::Composter;
			case Item::Conduit:                         return BlockType::Conduit;
			case Item::Cornflower:                      return BlockType::Cornflower;
			case Item::CrackedNetherBricks:             return BlockType::CrackedNetherBricks;
			case Item::CrackedPolishedBlackstoneBricks: return BlockType::CrackedPolishedBlackstoneBricks;
			case Item::CrackedStoneBricks:              return BlockType::CrackedStoneBricks;
			case Item::CraftingTable:                   return BlockType::CraftingTable;
			case Item::CreeperHead:                     return BlockType::CreeperHead;
			case Item::CrimsonButton:                   return BlockType::CrimsonButton;
			case Item::CrimsonDoor:                     return BlockType::CrimsonDoor;
			case Item::CrimsonFence:                    return BlockType::CrimsonFence;
			case Item::CrimsonFenceGate:                return BlockType::CrimsonFenceGate;
			case Item::CrimsonFungus:                   return BlockType::CrimsonFungus;
			case Item::CrimsonHyphae:                   return BlockType::CrimsonHyphae;
			case Item::CrimsonNylium:                   return BlockType::CrimsonNylium;
			case Item::CrimsonPlanks:                   return BlockType::CrimsonPlanks;
			case Item::CrimsonPressurePlate:            return BlockType::CrimsonPressurePlate;
			case Item::CrimsonRoots:                    return BlockType::CrimsonRoots;
			case Item::CrimsonSign:                     return BlockType::CrimsonSign;
			case Item::CrimsonSlab:                     return BlockType::CrimsonSlab;
			case Item::CrimsonStairs:                   return BlockType::CrimsonStairs;
			case Item::CrimsonStem:                     return BlockType::CrimsonStem;
			case Item::CrimsonTrapdoor:                 return BlockType::CrimsonTrapdoor;
			case Item::CryingObsidian:                  return BlockType::CryingObsidian;
			case Item::CutRedSandstone:                 return BlockType::CutRedSandstone;
			case Item::CutRedSandstoneSlab:             return BlockType::CutRedSandstoneSlab;
			case Item::CutSandstone:                    return BlockType::CutSandstone;
			case Item::CutSandstoneSlab:                return BlockType::CutSandstoneSlab;
			case Item::CyanBanner:                      return BlockType::CyanBanner;
			case Item::CyanBed:                         return BlockType::CyanBed;
			case Item::CyanCarpet:                      return BlockType::CyanCarpet;
			case Item::CyanConcrete:                    return BlockType::CyanConcrete;
			case Item::CyanConcretePowder:              return BlockType::CyanConcretePowder;
			case Item::CyanGlazedTerracotta:            return BlockType::CyanGlazedTerracotta;
			case Item::CyanShulkerBox:                  return BlockType::CyanShulkerBox;
			case Item::CyanStainedGlass:                return BlockType::CyanStainedGlass;
			case Item::CyanStainedGlassPane:            return BlockType::CyanStainedGlassPane;
			case Item::CyanTerracotta:                  return BlockType::CyanTerracotta;
			case Item::CyanWool:                        return BlockType::CyanWool;
			case Item::DamagedAnvil:                    return BlockType::DamagedAnvil;
			case Item::Dandelion:                       return BlockType::Dandelion;
			case Item::DarkOakButton:                   return BlockType::DarkOakButton;
			case Item::DarkOakDoor:                     return BlockType::DarkOakDoor;
			case Item::DarkOakFence:                    return BlockType::DarkOakFence;
			case Item::DarkOakFenceGate:                return BlockType::DarkOakFenceGate;
			case Item::DarkOakLeaves:                   return BlockType::DarkOakLeaves;
			case Item::DarkOakLog:                      return BlockType::DarkOakLog;
			case Item::DarkOakPlanks:                   return BlockType::DarkOakPlanks;
			case Item::DarkOakPressurePlate:            return BlockType::DarkOakPressurePlate;
			case Item::DarkOakSapling:                  return BlockType::DarkOakSapling;
			case Item::DarkOakSign:                     return BlockType::DarkOakSign;
			case Item::DarkOakSlab:                     return BlockType::DarkOakSlab;
			case Item::DarkOakStairs:                   return BlockType::DarkOakStairs;
			case Item::DarkOakTrapdoor:                 return BlockType::DarkOakTrapdoor;
			case Item::DarkOakWood:                     return BlockType::DarkOakWood;
			case Item::DarkPrismarine:                  return BlockType::DarkPrismarine;
			case Item::DarkPrismarineSlab:              return BlockType::DarkPrismarineSlab;
			case Item::DarkPrismarineStairs:            return BlockType::DarkPrismarineStairs;
			case Item::DaylightDetector:                return BlockType::DaylightDetector;
			case Item::DeadBrainCoral:                  return BlockType::DeadBrainCoral;
			case Item::DeadBrainCoralBlock:             return BlockType::DeadBrainCoralBlock;
			case Item::DeadBrainCoralFan:               return BlockType::DeadBrainCoralFan;
			case Item::DeadBubbleCoral:                 return BlockType::DeadBubbleCoral;
			case Item::DeadBubbleCoralBlock:            return BlockType::DeadBubbleCoralBlock;
			case Item::DeadBubbleCoralFan:              return BlockType::DeadBubbleCoralFan;
			case Item::DeadBush:                        return BlockType::DeadBush;
			case Item::DeadFireCoral:                   return BlockType::DeadFireCoral;
			case Item::DeadFireCoralBlock:              return BlockType::DeadFireCoralBlock;
			case Item::DeadFireCoralFan:                return BlockType::DeadFireCoralFan;
			case Item::DeadHornCoral:                   return BlockType::DeadHornCoral;
			case Item::DeadHornCoralBlock:              return BlockType::DeadHornCoralBlock;
			case Item::DeadHornCoralFan:                return BlockType::DeadHornCoralFan;
			case Item::DeadTubeCoral:                   return BlockType::DeadTubeCoral;
			case Item::DeadTubeCoralBlock:              return BlockType::DeadTubeCoralBlock;
			case Item::DeadTubeCoralFan:                return BlockType::DeadTubeCoralFan;
			case Item::DetectorRail:                    return BlockType::DetectorRail;
			case Item::DiamondBlock:                    return BlockType::DiamondBlock;
			case Item::DiamondOre:                      return BlockType::DiamondOre;
			case Item::Diorite:                         return BlockType::Diorite;
			case Item::DioriteSlab:                     return BlockType::DioriteSlab;
			case Item::DioriteStairs:                   return BlockType::DioriteStairs;
			case Item::DioriteWall:                     return BlockType::DioriteWall;
			case Item::Dirt:                            return BlockType::Dirt;
			case Item::Dispenser:                       return BlockType::Dispenser;
			case Item::DragonEgg:                       return BlockType::DragonEgg;
			case Item::DragonHead:                      return BlockType::DragonHead;
			case Item::DriedKelpBlock:                  return BlockType::DriedKelpBlock;
			case Item::Dropper:                         return BlockType::Dropper;
			case Item::EmeraldBlock:                    return BlockType::EmeraldBlock;
			case Item::EmeraldOre:                      return BlockType::EmeraldOre;
			case Item::EnchantingTable:                 return BlockType::EnchantingTable;
			case Item::EndPortalFrame:                  return BlockType::EndPortalFrame;
			case Item::EndRod:                          return BlockType::EndRod;
			case Item::EndStone:                        return BlockType::EndStone;
			case Item::EndStoneBrickSlab:               return BlockType::EndStoneBrickSlab;
			case Item::EndStoneBrickStairs:             return BlockType::EndStoneBrickStairs;
			case Item::EndStoneBrickWall:               return BlockType::EndStoneBrickWall;
			case Item::EndStoneBricks:                  return BlockType::EndStoneBricks;
			case Item::EnderChest:                      return BlockType::EnderChest;
			case Item::Farmland:                        return BlockType::Farmland;
			case Item::Fern:                            return BlockType::Fern;
			case Item::FireCoral:                       return BlockType::FireCoral;
			case Item::FireCoralBlock:                  return BlockType::FireCoralBlock;
			case Item::FireCoralFan:                    return BlockType::FireCoralFan;
			case Item::FletchingTable:                  return BlockType::FletchingTable;
			case Item::FlowerPot:                       return BlockType::FlowerPot;
			case Item::Furnace:                         return BlockType::Furnace;
			case Item::GildedBlackstone:                return BlockType::GildedBlackstone;
			case Item::Glass:                           return BlockType::Glass;
			case Item::GlassPane:                       return BlockType::GlassPane;
			case Item::Glowstone:                       return BlockType::Glowstone;
			case Item::GoldBlock:                       return BlockType::GoldBlock;
			case Item::GoldOre:                         return BlockType::GoldOre;
			case Item::Granite:                         return BlockType::Granite;
			case Item::GraniteSlab:                     return BlockType::GraniteSlab;
			case Item::GraniteStairs:                   return BlockType::GraniteStairs;
			case Item::GraniteWall:                     return BlockType::GraniteWall;
			case Item::Grass:                           return BlockType::Grass;
			case Item::GrassBlock:                      return BlockType::GrassBlock;
			case Item::GrassPath:                       return BlockType::GrassPath;
			case Item::Gravel:                          return BlockType::Gravel;
			case Item::GrayBanner:                      return BlockType::GrayBanner;
			case Item::GrayBed:                         return BlockType::GrayBed;
			case Item::GrayCarpet:                      return BlockType::GrayCarpet;
			case Item::GrayConcrete:                    return BlockType::GrayConcrete;
			case Item::GrayConcretePowder:              return BlockType::GrayConcretePowder;
			case Item::GrayGlazedTerracotta:            return BlockType::GrayGlazedTerracotta;
			case Item::GrayShulkerBox:                  return BlockType::GrayShulkerBox;
			case Item::GrayStainedGlass:                return BlockType::GrayStainedGlass;
			case Item::GrayStainedGlassPane:            return BlockType::GrayStainedGlassPane;
			case Item::GrayTerracotta:                  return BlockType::GrayTerracotta;
			case Item::GrayWool:                        return BlockType::GrayWool;
			case Item::GreenBanner:                     return BlockType::GreenBanner;
			case Item::GreenBed:                        return BlockType::GreenBed;
			case Item::GreenCarpet:                     return BlockType::GreenCarpet;
			case Item::GreenConcrete:                   return BlockType::GreenConcrete;
			case Item::GreenConcretePowder:             return BlockType::GreenConcretePowder;
			case Item::GreenGlazedTerracotta:           return BlockType::GreenGlazedTerracotta;
			case Item::GreenShulkerBox:                 return BlockType::GreenShulkerBox;
			case Item::GreenStainedGlass:               return BlockType::GreenStainedGlass;
			case Item::GreenStainedGlassPane:           return BlockType::GreenStainedGlassPane;
			case Item::GreenTerracotta:                 return BlockType::GreenTerracotta;
			case Item::GreenWool:                       return BlockType::GreenWool;
			case Item::Grindstone:                      return BlockType::Grindstone;
			case Item::HayBale:                         return BlockType::HayBale;
			case Item::HeavyWeightedPressurePlate:      return BlockType::HeavyWeightedPressurePlate;
			case Item::HoneyBlock:                      return BlockType::HoneyBlock;
			case Item::HoneycombBlock:                  return BlockType::HoneycombBlock;
			case Item::Hopper:                          return BlockType::Hopper;
			case Item::HornCoral:                       return BlockType::HornCoral;
			case Item::HornCoralBlock:                  return BlockType::HornCoralBlock;
			case Item::HornCoralFan:                    return BlockType::HornCoralFan;
			case Item::Ice:                             return BlockType::Ice;
			case Item::InfestedChiseledStoneBricks:     return BlockType::InfestedChiseledStoneBricks;
			case Item::InfestedCobblestone:             return BlockType::InfestedCobblestone;
			case Item::InfestedCrackedStoneBricks:      return BlockType::InfestedCrackedStoneBricks;
			case Item::InfestedMossyStoneBricks:        return BlockType::InfestedMossyStoneBricks;
			case Item::InfestedStone:                   return BlockType::InfestedStone;
			case Item::InfestedStoneBricks:             return BlockType::InfestedStoneBricks;
			case Item::IronBars:                        return BlockType::IronBars;
			case Item::IronBlock:                       return BlockType::IronBlock;
			case Item::IronDoor:                        return BlockType::IronDoor;
			case Item::IronOre:                         return BlockType::IronOre;
			case Item::IronTrapdoor:                    return BlockType::IronTrapdoor;
			case Item::JackOLantern:                    return BlockType::JackOLantern;
			case Item::Jigsaw:                          return BlockType::Jigsaw;
			case Item::Jukebox:                         return BlockType::Jukebox;
			case Item::JungleButton:                    return BlockType::JungleButton;
			case Item::JungleDoor:                      return BlockType::JungleDoor;
			case Item::JungleFence:                     return BlockType::JungleFence;
			case Item::JungleFenceGate:                 return BlockType::JungleFenceGate;
			case Item::JungleLeaves:                    return BlockType::JungleLeaves;
			case Item::JungleLog:                       return BlockType::JungleLog;
			case Item::JunglePlanks:                    return BlockType::JunglePlanks;
			case Item::JunglePressurePlate:             return BlockType::JunglePressurePlate;
			case Item::JungleSapling:                   return BlockType::JungleSapling;
			case Item::JungleSign:                      return BlockType::JungleSign;
			case Item::JungleSlab:                      return BlockType::JungleSlab;
			case Item::JungleStairs:                    return BlockType::JungleStairs;
			case Item::JungleTrapdoor:                  return BlockType::JungleTrapdoor;
			case Item::JungleWood:                      return BlockType::JungleWood;
			case Item::Kelp:                            return BlockType::Kelp;
			case Item::Ladder:                          return BlockType::Ladder;
			case Item::Lantern:                         return BlockType::Lantern;
			case Item::LapisBlock:                      return BlockType::LapisBlock;
			case Item::LapisOre:                        return BlockType::LapisOre;
			case Item::LargeFern:                       return BlockType::LargeFern;
			case Item::Lectern:                         return BlockType::Lectern;
			case Item::Lever:                           return BlockType::Lever;
			case Item::LightBlueBanner:                 return BlockType::LightBlueBanner;
			case Item::LightBlueBed:                    return BlockType::LightBlueBed;
			case Item::LightBlueCarpet:                 return BlockType::LightBlueCarpet;
			case Item::LightBlueConcrete:               return BlockType::LightBlueConcrete;
			case Item::LightBlueConcretePowder:         return BlockType::LightBlueConcretePowder;
			case Item::LightBlueGlazedTerracotta:       return BlockType::LightBlueGlazedTerracotta;
			case Item::LightBlueShulkerBox:             return BlockType::LightBlueShulkerBox;
			case Item::LightBlueStainedGlass:           return BlockType::LightBlueStainedGlass;
			case Item::LightBlueStainedGlassPane:       return BlockType::LightBlueStainedGlassPane;
			case Item::LightBlueTerracotta:             return BlockType::LightBlueTerracotta;
			case Item::LightBlueWool:                   return BlockType::LightBlueWool;
			case Item::LightGrayBanner:                 return BlockType::LightGrayBanner;
			case Item::LightGrayBed:                    return BlockType::LightGrayBed;
			case Item::LightGrayCarpet:                 return BlockType::LightGrayCarpet;
			case Item::LightGrayConcrete:               return BlockType::LightGrayConcrete;
			case Item::LightGrayConcretePowder:         return BlockType::LightGrayConcretePowder;
			case Item::LightGrayGlazedTerracotta:       return BlockType::LightGrayGlazedTerracotta;
			case Item::LightGrayShulkerBox:             return BlockType::LightGrayShulkerBox;
			case Item::LightGrayStainedGlass:           return BlockType::LightGrayStainedGlass;
			case Item::LightGrayStainedGlassPane:       return BlockType::LightGrayStainedGlassPane;
			case Item::LightGrayTerracotta:             return BlockType::LightGrayTerracotta;
			case Item::LightGrayWool:                   return BlockType::LightGrayWool;
			case Item::LightWeightedPressurePlate:      return BlockType::LightWeightedPressurePlate;
			case Item::Lilac:                           return BlockType::Lilac;
			case Item::LilyOfTheValley:                 return BlockType::LilyOfTheValley;
			case Item::LilyPad:                         return BlockType::LilyPad;
			case Item::LimeBanner:                      return BlockType::LimeBanner;
			case Item::LimeBed:                         return BlockType::LimeBed;
			case Item::LimeCarpet:                      return BlockType::LimeCarpet;
			case Item::LimeConcrete:                    return BlockType::LimeConcrete;
			case Item::LimeConcretePowder:              return BlockType::LimeConcretePowder;
			case Item::LimeGlazedTerracotta:            return BlockType::LimeGlazedTerracotta;
			case Item::LimeShulkerBox:                  return BlockType::LimeShulkerBox;
			case Item::LimeStainedGlass:                return BlockType::LimeStainedGlass;
			case Item::LimeStainedGlassPane:            return BlockType::LimeStainedGlassPane;
			case Item::LimeTerracotta:                  return BlockType::LimeTerracotta;
			case Item::LimeWool:                        return BlockType::LimeWool;
			case Item::Lodestone:                       return BlockType::Lodestone;
			case Item::Loom:                            return BlockType::Loom;
			case Item::MagentaBanner:                   return BlockType::MagentaBanner;
			case Item::MagentaBed:                      return BlockType::MagentaBed;
			case Item::MagentaCarpet:                   return BlockType::MagentaCarpet;
			case Item::MagentaConcrete:                 return BlockType::MagentaConcrete;
			case Item::MagentaConcretePowder:           return BlockType::MagentaConcretePowder;
			case Item::MagentaGlazedTerracotta:         return BlockType::MagentaGlazedTerracotta;
			case Item::MagentaShulkerBox:               return BlockType::MagentaShulkerBox;
			case Item::MagentaStainedGlass:             return BlockType::MagentaStainedGlass;
			case Item::MagentaStainedGlassPane:         return BlockType::MagentaStainedGlassPane;
			case Item::MagentaTerracotta:               return BlockType::MagentaTerracotta;
			case Item::MagentaWool:                     return BlockType::MagentaWool;
			case Item::MagmaBlock:                      return BlockType::MagmaBlock;
			case Item::Melon:                           return BlockType::Melon;
			case Item::MossyCobblestone:                return BlockType::MossyCobblestone;
			case Item::MossyCobblestoneSlab:            return BlockType::MossyCobblestoneSlab;
			case Item::MossyCobblestoneStairs:          return BlockType::MossyCobblestoneStairs;
			case Item::MossyCobblestoneWall:            return BlockType::MossyCobblestoneWall;
			case Item::MossyStoneBrickSlab:             return BlockType::MossyStoneBrickSlab;
			case Item::MossyStoneBrickStairs:           return BlockType::MossyStoneBrickStairs;
			case Item::MossyStoneBrickWall:             return BlockType::MossyStoneBrickWall;
			case Item::MossyStoneBricks:                return BlockType::MossyStoneBricks;
			case Item::MushroomStem:                    return BlockType::MushroomStem;
			case Item::Mycelium:                        return BlockType::Mycelium;
			case Item::NetherBrickFence:                return BlockType::NetherBrickFence;
			case Item::NetherBrickSlab:                 return BlockType::NetherBrickSlab;
			case Item::NetherBrickStairs:               return BlockType::NetherBrickStairs;
			case Item::NetherBrickWall:                 return BlockType::NetherBrickWall;
			case Item::NetherBricks:                    return BlockType::NetherBricks;
			case Item::NetherGoldOre:                   return BlockType::NetherGoldOre;
			case Item::NetherQuartzOre:                 return BlockType::NetherQuartzOre;
			case Item::NetherSprouts:                   return BlockType::NetherSprouts;
			case Item::NetherWart:                      return BlockType::NetherWart;
			case Item::NetherWartBlock:                 return BlockType::NetherWartBlock;
			case Item::NetheriteBlock:                  return BlockType::NetheriteBlock;
			case Item::Netherrack:                      return BlockType::Netherrack;
			case Item::NoteBlock:                       return BlockType::NoteBlock;
			case Item::OakButton:                       return BlockType::OakButton;
			case Item::OakDoor:                         return BlockType::OakDoor;
			case Item::OakFence:                        return BlockType::OakFence;
			case Item::OakFenceGate:                    return BlockType::OakFenceGate;
			case Item::OakLeaves:                       return BlockType::OakLeaves;
			case Item::OakLog:                          return BlockType::OakLog;
			case Item::OakPlanks:                       return BlockType::OakPlanks;
			case Item::OakPressurePlate:                return BlockType::OakPressurePlate;
			case Item::OakSapling:                      return BlockType::OakSapling;
			case Item::OakSign:                         return BlockType::OakSign;
			case Item::OakSlab:                         return BlockType::OakSlab;
			case Item::OakStairs:                       return BlockType::OakStairs;
			case Item::OakTrapdoor:                     return BlockType::OakTrapdoor;
			case Item::OakWood:                         return BlockType::OakWood;
			case Item::Observer:                        return BlockType::Observer;
			case Item::Obsidian:                        return BlockType::Obsidian;
			case Item::OrangeBanner:                    return BlockType::OrangeBanner;
			case Item::OrangeBed:                       return BlockType::OrangeBed;
			case Item::OrangeCarpet:                    return BlockType::OrangeCarpet;
			case Item::OrangeConcrete:                  return BlockType::OrangeConcrete;
			case Item::OrangeConcretePowder:            return BlockType::OrangeConcretePowder;
			case Item::OrangeGlazedTerracotta:          return BlockType::OrangeGlazedTerracotta;
			case Item::OrangeShulkerBox:                return BlockType::OrangeShulkerBox;
			case Item::OrangeStainedGlass:              return BlockType::OrangeStainedGlass;
			case Item::OrangeStainedGlassPane:          return BlockType::OrangeStainedGlassPane;
			case Item::OrangeTerracotta:                return BlockType::OrangeTerracotta;
			case Item::OrangeTulip:                     return BlockType::OrangeTulip;
			case Item::OrangeWool:                      return BlockType::OrangeWool;
			case Item::OxeyeDaisy:                      return BlockType::OxeyeDaisy;
			case Item::PackedIce:                       return BlockType::PackedIce;
			case Item::Peony:                           return BlockType::Peony;
			case Item::PetrifiedOakSlab:                return BlockType::PetrifiedOakSlab;
			case Item::PinkBanner:                      return BlockType::PinkBanner;
			case Item::PinkBed:                         return BlockType::PinkBed;
			case Item::PinkCarpet:                      return BlockType::PinkCarpet;
			case Item::PinkConcrete:                    return BlockType::PinkConcrete;
			case Item::PinkConcretePowder:              return BlockType::PinkConcretePowder;
			case Item::PinkGlazedTerracotta:            return BlockType::PinkGlazedTerracotta;
			case Item::PinkShulkerBox:                  return BlockType::PinkShulkerBox;
			case Item::PinkStainedGlass:                return BlockType::PinkStainedGlass;
			case Item::PinkStainedGlassPane:            return BlockType::PinkStainedGlassPane;
			case Item::PinkTerracotta:                  return BlockType::PinkTerracotta;
			case Item::PinkTulip:                       return BlockType::PinkTulip;
			case Item::PinkWool:                        return BlockType::PinkWool;
			case Item::Piston:                          return BlockType::Piston;
			case Item::PlayerHead:                      return BlockType::PlayerHead;
			case Item::Podzol:                          return BlockType::Podzol;
			case Item::PolishedAndesite:                return BlockType::PolishedAndesite;
			case Item::PolishedAndesiteSlab:            return BlockType::PolishedAndesiteSlab;
			case Item::PolishedAndesiteStairs:          return BlockType::PolishedAndesiteStairs;
			case Item::PolishedBasalt:                  return BlockType::PolishedBasalt;
			case Item::PolishedBlackstone:              return BlockType::PolishedBlackstone;
			case Item::PolishedBlackstoneBrickSlab:     return BlockType::PolishedBlackstoneBrickSlab;
			case Item::PolishedBlackstoneBrickStairs:   return BlockType::PolishedBlackstoneBrickStairs;
			case Item::PolishedBlackstoneBrickWall:     return BlockType::PolishedBlackstoneBrickWall;
			case Item::PolishedBlackstoneBricks:        return BlockType::PolishedBlackstoneBricks;
			case Item::PolishedBlackstoneButton:        return BlockType::PolishedBlackstoneButton;
			case Item::PolishedBlackstonePressurePlate: return BlockType::PolishedBlackstonePressurePlate;
			case Item::PolishedBlackstoneSlab:          return BlockType::PolishedBlackstoneSlab;
			case Item::PolishedBlackstoneStairs:        return BlockType::PolishedBlackstoneStairs;
			case Item::PolishedBlackstoneWall:          return BlockType::PolishedBlackstoneWall;
			case Item::PolishedDiorite:                 return BlockType::PolishedDiorite;
			case Item::PolishedDioriteSlab:             return BlockType::PolishedDioriteSlab;
			case Item::PolishedDioriteStairs:           return BlockType::PolishedDioriteStairs;
			case Item::PolishedGranite:                 return BlockType::PolishedGranite;
			case Item::PolishedGraniteSlab:             return BlockType::PolishedGraniteSlab;
			case Item::PolishedGraniteStairs:           return BlockType::PolishedGraniteStairs;
			case Item::Poppy:                           return BlockType::Poppy;
			case Item::Potato:                          return BlockType::Potatoes;
			case Item::PoweredRail:                     return BlockType::PoweredRail;
			case Item::Prismarine:                      return BlockType::Prismarine;
			case Item::PrismarineBrickSlab:             return BlockType::PrismarineBrickSlab;
			case Item::PrismarineBrickStairs:           return BlockType::PrismarineBrickStairs;
			case Item::PrismarineBricks:                return BlockType::PrismarineBricks;
			case Item::PrismarineSlab:                  return BlockType::PrismarineSlab;
			case Item::PrismarineStairs:                return BlockType::PrismarineStairs;
			case Item::PrismarineWall:                  return BlockType::PrismarineWall;
			case Item::Pumpkin:                         return BlockType::Pumpkin;
			case Item::PurpleBanner:                    return BlockType::PurpleBanner;
			case Item::PurpleBed:                       return BlockType::PurpleBed;
			case Item::PurpleCarpet:                    return BlockType::PurpleCarpet;
			case Item::PurpleConcrete:                  return BlockType::PurpleConcrete;
			case Item::PurpleConcretePowder:            return BlockType::PurpleConcretePowder;
			case Item::PurpleGlazedTerracotta:          return BlockType::PurpleGlazedTerracotta;
			case Item::PurpleShulkerBox:                return BlockType::PurpleShulkerBox;
			case Item::PurpleStainedGlass:              return BlockType::PurpleStainedGlass;
			case Item::PurpleStainedGlassPane:          return BlockType::PurpleStainedGlassPane;
			case Item::PurpleTerracotta:                return BlockType::PurpleTerracotta;
			case Item::PurpleWool:                      return BlockType::PurpleWool;
			case Item::PurpurBlock:                     return BlockType::PurpurBlock;
			case Item::PurpurPillar:                    return BlockType::PurpurPillar;
			case Item::PurpurSlab:                      return BlockType::PurpurSlab;
			case Item::PurpurStairs:                    return BlockType::PurpurStairs;
			case Item::QuartzBlock:                     return BlockType::QuartzBlock;
			case Item::QuartzBricks:                    return BlockType::QuartzBricks;
			case Item::QuartzPillar:                    return BlockType::QuartzPillar;
			case Item::QuartzSlab:                      return BlockType::QuartzSlab;
			case Item::QuartzStairs:                    return BlockType::QuartzStairs;
			case Item::Rail:                            return BlockType::Rail;
			case Item::RedBanner:                       return BlockType::RedBanner;
			case Item::RedBed:                          return BlockType::RedBed;
			case Item::RedCarpet:                       return BlockType::RedCarpet;
			case Item::RedConcrete:                     return BlockType::RedConcrete;
			case Item::RedConcretePowder:               return BlockType::RedConcretePowder;
			case Item::RedGlazedTerracotta:             return BlockType::RedGlazedTerracotta;
			case Item::RedMushroom:                     return BlockType::RedMushroom;
			case Item::RedMushroomBlock:                return BlockType::RedMushroomBlock;
			case Item::RedNetherBrickSlab:              return BlockType::RedNetherBrickSlab;
			case Item::RedNetherBrickStairs:            return BlockType::RedNetherBrickStairs;
			case Item::RedNetherBrickWall:              return BlockType::RedNetherBrickWall;
			case Item::RedNetherBricks:                 return BlockType::RedNetherBricks;
			case Item::RedSand:                         return BlockType::RedSand;
			case Item::RedSandstone:                    return BlockType::RedSandstone;
			case Item::RedSandstoneSlab:                return BlockType::RedSandstoneSlab;
			case Item::RedSandstoneStairs:              return BlockType::RedSandstoneStairs;
			case Item::RedSandstoneWall:                return BlockType::RedSandstoneWall;
			case Item::RedShulkerBox:                   return BlockType::RedShulkerBox;
			case Item::RedStainedGlass:                 return BlockType::RedStainedGlass;
			case Item::RedStainedGlassPane:             return BlockType::RedStainedGlassPane;
			case Item::RedTerracotta:                   return BlockType::RedTerracotta;
			case Item::RedTulip:                        return BlockType::RedTulip;
			case Item::RedWool:                         return BlockType::RedWool;
			case Item::RedstoneBlock:                   return BlockType::RedstoneBlock;
			case Item::RedstoneLamp:                    return BlockType::RedstoneLamp;
			case Item::RedstoneOre:                     return BlockType::RedstoneOre;
			case Item::RedstoneTorch:                   return BlockType::RedstoneTorch;
			case Item::Repeater:                        return BlockType::Repeater;
			case Item::RepeatingCommandBlock:           return BlockType::RepeatingCommandBlock;
			case Item::RespawnAnchor:                   return BlockType::RespawnAnchor;
			case Item::RoseBush:                        return BlockType::RoseBush;
			case Item::Sand:                            return BlockType::Sand;
			case Item::Sandstone:                       return BlockType::Sandstone;
			case Item::SandstoneSlab:                   return BlockType::SandstoneSlab;
			case Item::SandstoneStairs:                 return BlockType::SandstoneStairs;
			case Item::SandstoneWall:                   return BlockType::SandstoneWall;
			case Item::Scaffolding:                     return BlockType::Scaffolding;
			case Item::SeaLantern:                      return BlockType::SeaLantern;
			case Item::SeaPickle:                       return BlockType::SeaPickle;
			case Item::Seagrass:                        return BlockType::Seagrass;
			case Item::Shroomlight:                     return BlockType::Shroomlight;
			case Item::ShulkerBox:                      return BlockType::ShulkerBox;
			case Item::SkeletonSkull:                   return BlockType::SkeletonSkull;
			case Item::SlimeBlock:                      return BlockType::SlimeBlock;
			case Item::SmithingTable:                   return BlockType::SmithingTable;
			case Item::Smoker:                          return BlockType::Smoker;
			case Item::SmoothQuartz:                    return BlockType::SmoothQuartz;
			case Item::SmoothQuartzSlab:                return BlockType::SmoothQuartzSlab;
			case Item::SmoothQuartzStairs:              return BlockType::SmoothQuartzStairs;
			case Item::SmoothRedSandstone:              return BlockType::SmoothRedSandstone;
			case Item::SmoothRedSandstoneSlab:          return BlockType::SmoothRedSandstoneSlab;
			case Item::SmoothRedSandstoneStairs:        return BlockType::SmoothRedSandstoneStairs;
			case Item::SmoothSandstone:                 return BlockType::SmoothSandstone;
			case Item::SmoothSandstoneSlab:             return BlockType::SmoothSandstoneSlab;
			case Item::SmoothSandstoneStairs:           return BlockType::SmoothSandstoneStairs;
			case Item::SmoothStone:                     return BlockType::SmoothStone;
			case Item::SmoothStoneSlab:                 return BlockType::SmoothStoneSlab;
			case Item::Snow:                            return BlockType::Snow;
			case Item::SnowBlock:                       return BlockType::SnowBlock;
			case Item::SoulCampfire:                    return BlockType::SoulCampfire;
			case Item::SoulLantern:                     return BlockType::SoulLantern;
			case Item::SoulSand:                        return BlockType::SoulSand;
			case Item::SoulSoil:                        return BlockType::SoulSoil;
			case Item::SoulTorch:                       return BlockType::SoulTorch;
			case Item::Spawner:                         return BlockType::Spawner;
			case Item::Sponge:                          return BlockType::Sponge;
			case Item::SpruceButton:                    return BlockType::SpruceButton;
			case Item::SpruceDoor:                      return BlockType::SpruceDoor;
			case Item::SpruceFence:                     return BlockType::SpruceFence;
			case Item::SpruceFenceGate:                 return BlockType::SpruceFenceGate;
			case Item::SpruceLeaves:                    return BlockType::SpruceLeaves;
			case Item::SpruceLog:                       return BlockType::SpruceLog;
			case Item::SprucePlanks:                    return BlockType::SprucePlanks;
			case Item::SprucePressurePlate:             return BlockType::SprucePressurePlate;
			case Item::SpruceSapling:                   return BlockType::SpruceSapling;
			case Item::SpruceSign:                      return BlockType::SpruceSign;
			case Item::SpruceSlab:                      return BlockType::SpruceSlab;
			case Item::SpruceStairs:                    return BlockType::SpruceStairs;
			case Item::SpruceTrapdoor:                  return BlockType::SpruceTrapdoor;
			case Item::SpruceWood:                      return BlockType::SpruceWood;
			case Item::StickyPiston:                    return BlockType::StickyPiston;
			case Item::Stone:                           return BlockType::Stone;
			case Item::StoneBrickSlab:                  return BlockType::StoneBrickSlab;
			case Item::StoneBrickStairs:                return BlockType::StoneBrickStairs;
			case Item::StoneBrickWall:                  return BlockType::StoneBrickWall;
			case Item::StoneBricks:                     return BlockType::StoneBricks;
			case Item::StoneButton:                     return BlockType::StoneButton;
			case Item::StonePressurePlate:              return BlockType::StonePressurePlate;
			case Item::StoneSlab:                       return BlockType::StoneSlab;
			case Item::StoneStairs:                     return BlockType::StoneStairs;
			case Item::Stonecutter:                     return BlockType::Stonecutter;
			case Item::StrippedAcaciaLog:               return BlockType::StrippedAcaciaLog;
			case Item::StrippedAcaciaWood:              return BlockType::StrippedAcaciaWood;
			case Item::StrippedBirchLog:                return BlockType::StrippedBirchLog;
			case Item::StrippedBirchWood:               return BlockType::StrippedBirchWood;
			case Item::StrippedCrimsonHyphae:           return BlockType::StrippedCrimsonHyphae;
			case Item::StrippedCrimsonStem:             return BlockType::StrippedCrimsonStem;
			case Item::StrippedDarkOakLog:              return BlockType::StrippedDarkOakLog;
			case Item::StrippedDarkOakWood:             return BlockType::StrippedDarkOakWood;
			case Item::StrippedJungleLog:               return BlockType::StrippedJungleLog;
			case Item::StrippedJungleWood:              return BlockType::StrippedJungleWood;
			case Item::StrippedOakLog:                  return BlockType::StrippedOakLog;
			case Item::StrippedOakWood:                 return BlockType::StrippedOakWood;
			case Item::StrippedSpruceLog:               return BlockType::StrippedSpruceLog;
			case Item::StrippedSpruceWood:              return BlockType::StrippedSpruceWood;
			case Item::StrippedWarpedHyphae:            return BlockType::StrippedWarpedHyphae;
			case Item::StrippedWarpedStem:              return BlockType::StrippedWarpedStem;
			case Item::StructureBlock:                  return BlockType::StructureBlock;
			case Item::StructureVoid:                   return BlockType::StructureVoid;
			case Item::SugarCane:                       return BlockType::SugarCane;
			case Item::Sunflower:                       return BlockType::Sunflower;
			case Item::TNT:                             return BlockType::TNT;
			case Item::TallGrass:                       return BlockType::TallGrass;
			case Item::Target:                          return BlockType::Target;
			case Item::Terracotta:                      return BlockType::Terracotta;
			case Item::Torch:                           return BlockType::Torch;
			case Item::TrappedChest:                    return BlockType::TrappedChest;
			case Item::TripwireHook:                    return BlockType::TripwireHook;
			case Item::TubeCoral:                       return BlockType::TubeCoral;
			case Item::TubeCoralBlock:                  return BlockType::TubeCoralBlock;
			case Item::TubeCoralFan:                    return BlockType::TubeCoralFan;
			case Item::TurtleEgg:                       return BlockType::TurtleEgg;
			case Item::TwistingVines:                   return BlockType::TwistingVines;
			case Item::Vine:                            return BlockType::Vine;
			case Item::WarpedButton:                    return BlockType::WarpedButton;
			case Item::WarpedDoor:                      return BlockType::WarpedDoor;
			case Item::WarpedFence:                     return BlockType::WarpedFence;
			case Item::WarpedFenceGate:                 return BlockType::WarpedFenceGate;
			case Item::WarpedFungus:                    return BlockType::WarpedFungus;
			case Item::WarpedHyphae:                    return BlockType::WarpedHyphae;
			case Item::WarpedNylium:                    return BlockType::WarpedNylium;
			case Item::WarpedPlanks:                    return BlockType::WarpedPlanks;
			case Item::WarpedPressurePlate:             return BlockType::WarpedPressurePlate;
			case Item::WarpedRoots:                     return BlockType::WarpedRoots;
			case Item::WarpedSign:                      return BlockType::WarpedSign;
			case Item::WarpedSlab:                      return BlockType::WarpedSlab;
			case Item::WarpedStairs:                    return BlockType::WarpedStairs;
			case Item::WarpedStem:                      return BlockType::WarpedStem;
			case Item::WarpedTrapdoor:                  return BlockType::WarpedTrapdoor;
			case Item::WarpedWartBlock:                 return BlockType::WarpedWartBlock;
			case Item::WeepingVines:                    return BlockType::WeepingVines;
			case Item::WetSponge:                       return BlockType::WetSponge;
			case Item::Wheat:                           return BlockType::Wheat;
			case Item::WhiteBanner:                     return BlockType::WhiteBanner;
			case Item::WhiteBed:                        return BlockType::WhiteBed;
			case Item::WhiteCarpet:                     return BlockType::WhiteCarpet;
			case Item::WhiteConcrete:                   return BlockType::WhiteConcrete;
			case Item::WhiteConcretePowder:             return BlockType::WhiteConcretePowder;
			case Item::WhiteGlazedTerracotta:           return BlockType::WhiteGlazedTerracotta;
			case Item::WhiteShulkerBox:                 return BlockType::WhiteShulkerBox;
			case Item::WhiteStainedGlass:               return BlockType::WhiteStainedGlass;
			case Item::WhiteStainedGlassPane:           return BlockType::WhiteStainedGlassPane;
			case Item::WhiteTerracotta:                 return BlockType::WhiteTerracotta;
			case Item::WhiteTulip:                      return BlockType::WhiteTulip;
			case Item::WhiteWool:                       return BlockType::WhiteWool;
			case Item::WitherRose:                      return BlockType::WitherRose;
			case Item::WitherSkeletonSkull:             return BlockType::WitherSkeletonSkull;
			case Item::YellowBanner:                    return BlockType::YellowBanner;
			case Item::YellowBed:                       return BlockType::YellowBed;
			case Item::YellowCarpet:                    return BlockType::YellowCarpet;
			case Item::YellowConcrete:                  return BlockType::YellowConcrete;
			case Item::YellowConcretePowder:            return BlockType::YellowConcretePowder;
			case Item::YellowGlazedTerracotta:          return BlockType::YellowGlazedTerracotta;
			case Item::YellowShulkerBox:                return BlockType::YellowShulkerBox;
			case Item::YellowStainedGlass:              return BlockType::YellowStainedGlass;
			case Item::YellowStainedGlassPane:          return BlockType::YellowStainedGlassPane;
			case Item::YellowTerracotta:                return BlockType::YellowTerracotta;
			case Item::YellowWool:                      return BlockType::YellowWool;
			case Item::ZombieHead:                      return BlockType::ZombieHead;
			case Item::AcaciaBoat:
			case Item::Apple:
			case Item::ArmorStand:
			case Item::Arrow:
			case Item::BakedPotato:
			case Item::BatSpawnEgg:
			case Item::BeeSpawnEgg:
			case Item::Beef:
			case Item::BeetrootSeeds:
			case Item::BeetrootSoup:
			case Item::BirchBoat:
			case Item::BlackDye:
			case Item::BlazePowder:
			case Item::BlazeRod:
			case Item::BlazeSpawnEgg:
			case Item::BlueDye:
			case Item::Bone:
			case Item::BoneMeal:
			case Item::Book:
			case Item::Bow:
			case Item::Bowl:
			case Item::Bread:
			case Item::Brick:
			case Item::BrownDye:
			case Item::Bucket:
			case Item::CarrotOnAStick:
			case Item::CatSpawnEgg:
			case Item::CaveSpiderSpawnEgg:
			case Item::ChainmailBoots:
			case Item::ChainmailChestplate:
			case Item::ChainmailHelmet:
			case Item::ChainmailLeggings:
			case Item::Charcoal:
			case Item::ChestMinecart:
			case Item::Chicken:
			case Item::ChickenSpawnEgg:
			case Item::ChorusFruit:
			case Item::ClayBall:
			case Item::Clock:
			case Item::Coal:
			case Item::Cod:
			case Item::CodBucket:
			case Item::CodSpawnEgg:
			case Item::CommandBlockMinecart:
			case Item::Compass:
			case Item::CookedBeef:
			case Item::CookedChicken:
			case Item::CookedCod:
			case Item::CookedMutton:
			case Item::CookedPorkchop:
			case Item::CookedRabbit:
			case Item::CookedSalmon:
			case Item::Cookie:
			case Item::CowSpawnEgg:
			case Item::CreeperBannerPattern:
			case Item::CreeperSpawnEgg:
			case Item::Crossbow:
			case Item::CyanDye:
			case Item::DarkOakBoat:
			case Item::DebugStick:
			case Item::Diamond:
			case Item::DiamondAxe:
			case Item::DiamondBoots:
			case Item::DiamondChestplate:
			case Item::DiamondHelmet:
			case Item::DiamondHoe:
			case Item::DiamondHorseArmor:
			case Item::DiamondLeggings:
			case Item::DiamondPickaxe:
			case Item::DiamondShovel:
			case Item::DiamondSword:
			case Item::DolphinSpawnEgg:
			case Item::DonkeySpawnEgg:
			case Item::DragonBreath:
			case Item::DriedKelp:
			case Item::DrownedSpawnEgg:
			case Item::Egg:
			case Item::ElderGuardianSpawnEgg:
			case Item::Elytra:
			case Item::Emerald:
			case Item::EnchantedBook:
			case Item::EnchantedGoldenApple:
			case Item::EndCrystal:
			case Item::EnderEye:
			case Item::EnderPearl:
			case Item::EndermanSpawnEgg:
			case Item::EndermiteSpawnEgg:
			case Item::EvokerSpawnEgg:
			case Item::ExperienceBottle:
			case Item::Feather:
			case Item::FermentedSpiderEye:
			case Item::FilledMap:
			case Item::FireCharge:
			case Item::FireworkRocket:
			case Item::FireworkStar:
			case Item::FishingRod:
			case Item::Flint:
			case Item::FlintAndSteel:
			case Item::FlowerBannerPattern:
			case Item::FoxSpawnEgg:
			case Item::FurnaceMinecart:
			case Item::GhastSpawnEgg:
			case Item::GhastTear:
			case Item::GlassBottle:
			case Item::GlisteringMelonSlice:
			case Item::GlobeBannerPattern:
			case Item::GlowstoneDust:
			case Item::GoldIngot:
			case Item::GoldNugget:
			case Item::GoldenApple:
			case Item::GoldenAxe:
			case Item::GoldenBoots:
			case Item::GoldenCarrot:
			case Item::GoldenChestplate:
			case Item::GoldenHelmet:
			case Item::GoldenHoe:
			case Item::GoldenHorseArmor:
			case Item::GoldenLeggings:
			case Item::GoldenPickaxe:
			case Item::GoldenShovel:
			case Item::GoldenSword:
			case Item::GrayDye:
			case Item::GreenDye:
			case Item::GuardianSpawnEgg:
			case Item::Gunpowder:
			case Item::HeartOfTheSea:
			case Item::HoglinSpawnEgg:
			case Item::HoneyBottle:
			case Item::Honeycomb:
			case Item::HopperMinecart:
			case Item::HorseSpawnEgg:
			case Item::HuskSpawnEgg:
			case Item::InkSac:
			case Item::IronAxe:
			case Item::IronBoots:
			case Item::IronChestplate:
			case Item::IronHelmet:
			case Item::IronHoe:
			case Item::IronHorseArmor:
			case Item::IronIngot:
			case Item::IronLeggings:
			case Item::IronNugget:
			case Item::IronPickaxe:
			case Item::IronShovel:
			case Item::IronSword:
			case Item::ItemFrame:
			case Item::JungleBoat:
			case Item::KnowledgeBook:
			case Item::LapisLazuli:
			case Item::LavaBucket:
			case Item::Lead:
			case Item::Leather:
			case Item::LeatherBoots:
			case Item::LeatherChestplate:
			case Item::LeatherHelmet:
			case Item::LeatherHorseArmor:
			case Item::LeatherLeggings:
			case Item::LightBlueDye:
			case Item::LightGrayDye:
			case Item::LimeDye:
			case Item::LingeringPotion:
			case Item::LlamaSpawnEgg:
			case Item::MagentaDye:
			case Item::MagmaCream:
			case Item::MagmaCubeSpawnEgg:
			case Item::Map:
			case Item::MelonSeeds:
			case Item::MelonSlice:
			case Item::MilkBucket:
			case Item::Minecart:
			case Item::MojangBannerPattern:
			case Item::MooshroomSpawnEgg:
			case Item::MuleSpawnEgg:
			case Item::MushroomStew:
			case Item::MusicDiscBlocks:
			case Item::MusicDiscCat:
			case Item::MusicDiscChirp:
			case Item::MusicDiscFar:
			case Item::MusicDiscMall:
			case Item::MusicDiscMellohi:
			case Item::MusicDiscPigstep:
			case Item::MusicDiscStal:
			case Item::MusicDiscStrad:
			case Item::MusicDiscWait:
			case Item::MusicDiscWard:
			case Item::MusicDisc11:
			case Item::MusicDisc13:
			case Item::Mutton:
			case Item::NameTag:
			case Item::NautilusShell:
			case Item::NetherBrick:
			case Item::NetherStar:
			case Item::NetheriteAxe:
			case Item::NetheriteBoots:
			case Item::NetheriteChestplate:
			case Item::NetheriteHelmet:
			case Item::NetheriteHoe:
			case Item::NetheriteIngot:
			case Item::NetheriteLeggings:
			case Item::NetheritePickaxe:
			case Item::NetheriteScrap:
			case Item::NetheriteShovel:
			case Item::NetheriteSword:
			case Item::OakBoat:
			case Item::OcelotSpawnEgg:
			case Item::OrangeDye:
			case Item::Painting:
			case Item::PandaSpawnEgg:
			case Item::Paper:
			case Item::ParrotSpawnEgg:
			case Item::PhantomMembrane:
			case Item::PhantomSpawnEgg:
			case Item::PigSpawnEgg:
			case Item::PiglinBannerPattern:
			case Item::PiglinSpawnEgg:
			case Item::PillagerSpawnEgg:
			case Item::PinkDye:
			case Item::PoisonousPotato:
			case Item::PolarBearSpawnEgg:
			case Item::PoppedChorusFruit:
			case Item::Porkchop:
			case Item::Potion:
			case Item::PrismarineCrystals:
			case Item::PrismarineShard:
			case Item::Pufferfish:
			case Item::PufferfishBucket:
			case Item::PufferfishSpawnEgg:
			case Item::PumpkinPie:
			case Item::PumpkinSeeds:
			case Item::PurpleDye:
			case Item::Quartz:
			case Item::Rabbit:
			case Item::RabbitFoot:
			case Item::RabbitHide:
			case Item::RabbitSpawnEgg:
			case Item::RabbitStew:
			case Item::RavagerSpawnEgg:
			case Item::RedDye:
			case Item::Redstone:
			case Item::RottenFlesh:
			case Item::Saddle:
			case Item::Salmon:
			case Item::SalmonBucket:
			case Item::SalmonSpawnEgg:
			case Item::Scute:
			case Item::Shears:
			case Item::SheepSpawnEgg:
			case Item::Shield:
			case Item::ShulkerShell:
			case Item::ShulkerSpawnEgg:
			case Item::SilverfishSpawnEgg:
			case Item::SkeletonHorseSpawnEgg:
			case Item::SkeletonSpawnEgg:
			case Item::SkullBannerPattern:
			case Item::SlimeBall:
			case Item::SlimeSpawnEgg:
			case Item::Snowball:
			case Item::SpectralArrow:
			case Item::SpiderEye:
			case Item::SpiderSpawnEgg:
			case Item::SplashPotion:
			case Item::SpruceBoat:
			case Item::SquidSpawnEgg:
			case Item::Stick:
			case Item::StoneAxe:
			case Item::StoneHoe:
			case Item::StonePickaxe:
			case Item::StoneShovel:
			case Item::StoneSword:
			case Item::StraySpawnEgg:
			case Item::StriderSpawnEgg:
			case Item::String:
			case Item::Sugar:
			case Item::SuspiciousStew:
			case Item::SweetBerries:
			case Item::TippedArrow:
			case Item::TNTMinecart:
			case Item::TotemOfUndying:
			case Item::TraderLlamaSpawnEgg:
			case Item::Trident:
			case Item::TropicalFish:
			case Item::TropicalFishBucket:
			case Item::TropicalFishSpawnEgg:
			case Item::TurtleHelmet:
			case Item::TurtleSpawnEgg:
			case Item::VexSpawnEgg:
			case Item::VillagerSpawnEgg:
			case Item::VindicatorSpawnEgg:
			case Item::WanderingTraderSpawnEgg:
			case Item::WarpedFungusOnA_stick:
			case Item::WaterBucket:
			case Item::WheatSeeds:
			case Item::WhiteDye:
			case Item::WitchSpawnEgg:
			case Item::WitherSkeletonSpawnEgg:
			case Item::WolfSpawnEgg:
			case Item::WoodenAxe:
			case Item::WoodenHoe:
			case Item::WoodenPickaxe:
			case Item::WoodenShovel:
			case Item::WoodenSword:
			case Item::WritableBook:
			case Item::WrittenBook:
			case Item::YellowDye:
			case Item::ZoglinSpawnEgg:
			case Item::ZombieHorseSpawnEgg:
			case Item::ZombieSpawnEgg:
			case Item::ZombieVillagerSpawnEgg:
			case Item::ZombiePigmanSpawnEgg:
			{
				LOGERROR("{}: Got unintended item to convert to block: {}", __FUNCTION__, a_Item);
				ASSERT(!"Converting unintended item to block");
				return BlockType::Air;
			}
		}
		return BlockType::Air;
	}

	Item FromBlock(BlockType a_Block)
	{
		switch (a_Block)
		{
			case BlockType::AcaciaButton:                    return Item::AcaciaButton;
			case BlockType::AcaciaDoor:                      return Item::AcaciaDoor;
			case BlockType::AcaciaFence:                     return Item::AcaciaFence;
			case BlockType::AcaciaFenceGate:                 return Item::AcaciaFenceGate;
			case BlockType::AcaciaLeaves:                    return Item::AcaciaLeaves;
			case BlockType::AcaciaLog:                       return Item::AcaciaLog;
			case BlockType::AcaciaPlanks:                    return Item::AcaciaPlanks;
			case BlockType::AcaciaPressurePlate:             return Item::AcaciaPressurePlate;
			case BlockType::AcaciaSapling:                   return Item::AcaciaSapling;
			case BlockType::AcaciaSign:                      return Item::AcaciaSign;
			case BlockType::AcaciaSlab:                      return Item::AcaciaSlab;
			case BlockType::AcaciaStairs:                    return Item::AcaciaStairs;
			case BlockType::AcaciaTrapdoor:                  return Item::AcaciaTrapdoor;
			case BlockType::AcaciaWallSign:                  return Item::AcaciaSign;
			case BlockType::AcaciaWood:                      return Item::AcaciaWood;
			case BlockType::ActivatorRail:                   return Item::ActivatorRail;
			case BlockType::Air:                             return Item::Air;
			case BlockType::Allium:                          return Item::Allium;
			case BlockType::AncientDebris:                   return Item::AncientDebris;
			case BlockType::Andesite:                        return Item::Andesite;
			case BlockType::AndesiteSlab:                    return Item::AndesiteSlab;
			case BlockType::AndesiteStairs:                  return Item::AndesiteStairs;
			case BlockType::AndesiteWall:                    return Item::AndesiteWall;
			case BlockType::Anvil:                           return Item::Anvil;
			case BlockType::AttachedMelonStem:               return Item::Air;
			case BlockType::AttachedPumpkinStem:             return Item::Air;
			case BlockType::AzureBluet:                      return Item::AzureBluet;
			case BlockType::Bamboo:                          return Item::Bamboo;
			case BlockType::BambooSapling:                   return Item::Air;
			case BlockType::Barrel:                          return Item::Barrel;
			case BlockType::Barrier:                         return Item::Barrier;
			case BlockType::Basalt:                          return Item::Basalt;
			case BlockType::Beacon:                          return Item::Beacon;
			case BlockType::Bedrock:                         return Item::Bedrock;
			case BlockType::BeeNest:                         return Item::BeeNest;
			case BlockType::Beehive:                         return Item::Beehive;
			case BlockType::Beetroots:                       return Item::Beetroot;
			case BlockType::Bell:                            return Item::Bell;
			case BlockType::BirchButton:                     return Item::BirchButton;
			case BlockType::BirchDoor:                       return Item::BirchDoor;
			case BlockType::BirchFence:                      return Item::BirchFence;
			case BlockType::BirchFenceGate:                  return Item::BirchFenceGate;
			case BlockType::BirchLeaves:                     return Item::BirchLeaves;
			case BlockType::BirchLog:                        return Item::BirchLog;
			case BlockType::BirchPlanks:                     return Item::BirchPlanks;
			case BlockType::BirchPressurePlate:              return Item::BirchPressurePlate;
			case BlockType::BirchSapling:                    return Item::BirchSapling;
			case BlockType::BirchSign:                       return Item::BirchSign;
			case BlockType::BirchSlab:                       return Item::BirchSlab;
			case BlockType::BirchStairs:                     return Item::BirchStairs;
			case BlockType::BirchTrapdoor:                   return Item::BirchTrapdoor;
			case BlockType::BirchWallSign:                   return Item::BirchSign;
			case BlockType::BirchWood:                       return Item::BirchWood;
			case BlockType::BlackBanner:                     return Item::BlackBanner;
			case BlockType::BlackBed:                        return Item::BlackBed;
			case BlockType::BlackCarpet:                     return Item::BlackCarpet;
			case BlockType::BlackConcrete:                   return Item::BlackConcrete;
			case BlockType::BlackConcretePowder:             return Item::BlackConcretePowder;
			case BlockType::BlackGlazedTerracotta:           return Item::BlackGlazedTerracotta;
			case BlockType::BlackShulkerBox:                 return Item::BlackShulkerBox;
			case BlockType::BlackStainedGlass:               return Item::BlackStainedGlass;
			case BlockType::BlackStainedGlassPane:           return Item::BlackStainedGlassPane;
			case BlockType::BlackTerracotta:                 return Item::BlackTerracotta;
			case BlockType::BlackWallBanner:                 return Item::BlackBanner;
			case BlockType::BlackWool:                       return Item::BlackWool;
			case BlockType::Blackstone:                      return Item::Blackstone;
			case BlockType::BlackstoneSlab:                  return Item::BlackstoneSlab;
			case BlockType::BlackstoneStairs:                return Item::BlackstoneStairs;
			case BlockType::BlackstoneWall:                  return Item::BlackstoneWall;
			case BlockType::BlastFurnace:                    return Item::BlastFurnace;
			case BlockType::BlueBanner:                      return Item::BlueBanner;
			case BlockType::BlueBed:                         return Item::BlueBed;
			case BlockType::BlueCarpet:                      return Item::BlueCarpet;
			case BlockType::BlueConcrete:                    return Item::BlueConcrete;
			case BlockType::BlueConcretePowder:              return Item::BlueConcretePowder;
			case BlockType::BlueGlazedTerracotta:            return Item::BlueGlazedTerracotta;
			case BlockType::BlueIce:                         return Item::BlueIce;
			case BlockType::BlueOrchid:                      return Item::BlueOrchid;
			case BlockType::BlueShulkerBox:                  return Item::BlueShulkerBox;
			case BlockType::BlueStainedGlass:                return Item::BlueStainedGlass;
			case BlockType::BlueStainedGlassPane:            return Item::BlueStainedGlassPane;
			case BlockType::BlueTerracotta:                  return Item::BlueTerracotta;
			case BlockType::BlueWallBanner:                  return Item::BlueBanner;
			case BlockType::BlueWool:                        return Item::BlueWool;
			case BlockType::BoneBlock:                       return Item::BoneBlock;
			case BlockType::Bookshelf:                       return Item::Bookshelf;
			case BlockType::BrainCoral:                      return Item::BrainCoral;
			case BlockType::BrainCoralBlock:                 return Item::BrainCoralBlock;
			case BlockType::BrainCoralFan:                   return Item::BrainCoralFan;
			case BlockType::BrainCoralWallFan:               return Item::BrainCoralFan;
			case BlockType::BrewingStand:                    return Item::BrewingStand;
			case BlockType::BrickSlab:                       return Item::BrickSlab;
			case BlockType::BrickStairs:                     return Item::BrickStairs;
			case BlockType::BrickWall:                       return Item::BrickWall;
			case BlockType::Bricks:                          return Item::Bricks;
			case BlockType::BrownBanner:                     return Item::BrownBanner;
			case BlockType::BrownBed:                        return Item::BrownBed;
			case BlockType::BrownCarpet:                     return Item::BrownCarpet;
			case BlockType::BrownConcrete:                   return Item::BrownConcrete;
			case BlockType::BrownConcretePowder:             return Item::BrownConcretePowder;
			case BlockType::BrownGlazedTerracotta:           return Item::BrownGlazedTerracotta;
			case BlockType::BrownMushroom:                   return Item::BrownMushroom;
			case BlockType::BrownMushroomBlock:              return Item::BrownMushroomBlock;
			case BlockType::BrownShulkerBox:                 return Item::BrownShulkerBox;
			case BlockType::BrownStainedGlass:               return Item::BrownStainedGlass;
			case BlockType::BrownStainedGlassPane:           return Item::BrownStainedGlassPane;
			case BlockType::BrownTerracotta:                 return Item::BrownTerracotta;
			case BlockType::BrownWallBanner:                 return Item::BrownBanner;
			case BlockType::BrownWool:                       return Item::BrownWool;
			case BlockType::BubbleColumn:                    return Item::Air;
			case BlockType::BubbleCoral:                     return Item::BubbleCoral;
			case BlockType::BubbleCoralBlock:                return Item::BubbleCoralBlock;
			case BlockType::BubbleCoralFan:                  return Item::BubbleCoralFan;
			case BlockType::BubbleCoralWallFan:              return Item::BubbleCoralFan;
			case BlockType::Cactus:                          return Item::Cactus;
			case BlockType::Cake:                            return Item::Cake;
			case BlockType::Campfire:                        return Item::Campfire;
			case BlockType::Carrots:                         return Item::Carrot;
			case BlockType::CartographyTable:                return Item::CartographyTable;
			case BlockType::CarvedPumpkin:                   return Item::CarvedPumpkin;
			case BlockType::Cauldron:                        return Item::Cauldron;
			case BlockType::CaveAir:                         return Item::Air;
			case BlockType::Chain:                           return Item::Chain;
			case BlockType::ChainCommandBlock:               return Item::ChainCommandBlock;
			case BlockType::Chest:                           return Item::Chest;
			case BlockType::ChippedAnvil:                    return Item::ChippedAnvil;
			case BlockType::ChiseledNetherBricks:            return Item::ChiseledNetherBricks;
			case BlockType::ChiseledPolishedBlackstone:      return Item::ChiseledPolishedBlackstone;
			case BlockType::ChiseledQuartzBlock:             return Item::ChiseledQuartzBlock;
			case BlockType::ChiseledRedSandstone:            return Item::ChiseledRedSandstone;
			case BlockType::ChiseledSandstone:               return Item::ChiseledSandstone;
			case BlockType::ChiseledStoneBricks:             return Item::ChiseledStoneBricks;
			case BlockType::ChorusFlower:                    return Item::ChorusFlower;
			case BlockType::ChorusPlant:                     return Item::ChorusPlant;
			case BlockType::Clay:                            return Item::Clay;
			case BlockType::CoalBlock:                       return Item::CoalBlock;
			case BlockType::CoalOre:                         return Item::CoalOre;
			case BlockType::CoarseDirt:                      return Item::CoarseDirt;
			case BlockType::Cobblestone:                     return Item::Cobblestone;
			case BlockType::CobblestoneSlab:                 return Item::CobblestoneSlab;
			case BlockType::CobblestoneStairs:               return Item::CobblestoneStairs;
			case BlockType::CobblestoneWall:                 return Item::CobblestoneWall;
			case BlockType::Cobweb:                          return Item::Cobweb;
			case BlockType::Cocoa:                           return Item::CocoaBeans;
			case BlockType::CommandBlock:                    return Item::CommandBlock;
			case BlockType::Comparator:                      return Item::Comparator;
			case BlockType::Composter:                       return Item::Composter;
			case BlockType::Conduit:                         return Item::Conduit;
			case BlockType::Cornflower:                      return Item::Cornflower;
			case BlockType::CrackedNetherBricks:             return Item::CrackedNetherBricks;
			case BlockType::CrackedPolishedBlackstoneBricks: return Item::CrackedPolishedBlackstoneBricks;
			case BlockType::CrackedStoneBricks:              return Item::CrackedStoneBricks;
			case BlockType::CraftingTable:                   return Item::CraftingTable;
			case BlockType::CreeperHead:                     return Item::CreeperHead;
			case BlockType::CreeperWallHead:                 return Item::CreeperHead;
			case BlockType::CrimsonButton:                   return Item::CrimsonButton;
			case BlockType::CrimsonDoor:                     return Item::CrimsonDoor;
			case BlockType::CrimsonFence:                    return Item::CrimsonFence;
			case BlockType::CrimsonFenceGate:                return Item::CrimsonFenceGate;
			case BlockType::CrimsonFungus:                   return Item::CrimsonFungus;
			case BlockType::CrimsonHyphae:                   return Item::CrimsonHyphae;
			case BlockType::CrimsonNylium:                   return Item::CrimsonNylium;
			case BlockType::CrimsonPlanks:                   return Item::CrimsonPlanks;
			case BlockType::CrimsonPressurePlate:            return Item::CrimsonPressurePlate;
			case BlockType::CrimsonRoots:                    return Item::CrimsonRoots;
			case BlockType::CrimsonSign:                     return Item::CrimsonSign;
			case BlockType::CrimsonSlab:                     return Item::CrimsonSlab;
			case BlockType::CrimsonStairs:                   return Item::CrimsonStairs;
			case BlockType::CrimsonStem:                     return Item::CrimsonStem;
			case BlockType::CrimsonTrapdoor:                 return Item::CrimsonTrapdoor;
			case BlockType::CrimsonWallSign:                 return Item::CrimsonSign;
			case BlockType::CryingObsidian:                  return Item::CryingObsidian;
			case BlockType::CutRedSandstone:                 return Item::CutRedSandstone;
			case BlockType::CutRedSandstoneSlab:             return Item::CutRedSandstoneSlab;
			case BlockType::CutSandstone:                    return Item::CutSandstone;
			case BlockType::CutSandstoneSlab:                return Item::CutSandstoneSlab;
			case BlockType::CyanBanner:                      return Item::CyanBanner;
			case BlockType::CyanBed:                         return Item::CyanBed;
			case BlockType::CyanCarpet:                      return Item::CyanCarpet;
			case BlockType::CyanConcrete:                    return Item::CyanConcrete;
			case BlockType::CyanConcretePowder:              return Item::CyanConcretePowder;
			case BlockType::CyanGlazedTerracotta:            return Item::CyanGlazedTerracotta;
			case BlockType::CyanShulkerBox:                  return Item::CyanShulkerBox;
			case BlockType::CyanStainedGlass:                return Item::CyanStainedGlass;
			case BlockType::CyanStainedGlassPane:            return Item::CyanStainedGlassPane;
			case BlockType::CyanTerracotta:                  return Item::CyanTerracotta;
			case BlockType::CyanWallBanner:                  return Item::CyanBanner;
			case BlockType::CyanWool:                        return Item::CyanWool;
			case BlockType::DamagedAnvil:                    return Item::DamagedAnvil;
			case BlockType::Dandelion:                       return Item::Dandelion;
			case BlockType::DarkOakButton:                   return Item::DarkOakButton;
			case BlockType::DarkOakDoor:                     return Item::DarkOakDoor;
			case BlockType::DarkOakFence:                    return Item::DarkOakFence;
			case BlockType::DarkOakFenceGate:                return Item::DarkOakFenceGate;
			case BlockType::DarkOakLeaves:                   return Item::DarkOakLeaves;
			case BlockType::DarkOakLog:                      return Item::DarkOakLog;
			case BlockType::DarkOakPlanks:                   return Item::DarkOakPlanks;
			case BlockType::DarkOakPressurePlate:            return Item::DarkOakPressurePlate;
			case BlockType::DarkOakSapling:                  return Item::DarkOakSapling;
			case BlockType::DarkOakSign:                     return Item::DarkOakSign;
			case BlockType::DarkOakSlab:                     return Item::DarkOakSlab;
			case BlockType::DarkOakStairs:                   return Item::DarkOakStairs;
			case BlockType::DarkOakTrapdoor:                 return Item::DarkOakTrapdoor;
			case BlockType::DarkOakWallSign:                 return Item::DarkOakSign;
			case BlockType::DarkOakWood:                     return Item::DarkOakWood;
			case BlockType::DarkPrismarine:                  return Item::DarkPrismarine;
			case BlockType::DarkPrismarineSlab:              return Item::DarkPrismarineSlab;
			case BlockType::DarkPrismarineStairs:            return Item::DarkPrismarineStairs;
			case BlockType::DaylightDetector:                return Item::DaylightDetector;
			case BlockType::DeadBrainCoral:                  return Item::DeadBrainCoral;
			case BlockType::DeadBrainCoralBlock:             return Item::DeadBrainCoralBlock;
			case BlockType::DeadBrainCoralFan:               return Item::DeadBrainCoralFan;
			case BlockType::DeadBrainCoralWallFan:           return Item::DeadBrainCoralFan;
			case BlockType::DeadBubbleCoral:                 return Item::DeadBubbleCoral;
			case BlockType::DeadBubbleCoralBlock:            return Item::DeadBubbleCoralBlock;
			case BlockType::DeadBubbleCoralFan:              return Item::DeadBubbleCoralFan;
			case BlockType::DeadBubbleCoralWallFan:          return Item::DeadBubbleCoralFan;
			case BlockType::DeadBush:                        return Item::DeadBush;
			case BlockType::DeadFireCoral:                   return Item::DeadFireCoral;
			case BlockType::DeadFireCoralBlock:              return Item::DeadFireCoralBlock;
			case BlockType::DeadFireCoralFan:                return Item::DeadFireCoralFan;
			case BlockType::DeadFireCoralWallFan:            return Item::DeadFireCoralFan;
			case BlockType::DeadHornCoral:                   return Item::DeadHornCoral;
			case BlockType::DeadHornCoralBlock:              return Item::DeadHornCoralBlock;
			case BlockType::DeadHornCoralFan:                return Item::DeadHornCoralFan;
			case BlockType::DeadHornCoralWallFan:            return Item::DeadHornCoralFan;
			case BlockType::DeadTubeCoral:                   return Item::DeadTubeCoral;
			case BlockType::DeadTubeCoralBlock:              return Item::DeadTubeCoralBlock;
			case BlockType::DeadTubeCoralFan:                return Item::DeadTubeCoralFan;
			case BlockType::DeadTubeCoralWallFan:            return Item::DeadTubeCoralFan;
			case BlockType::DetectorRail:                    return Item::DetectorRail;
			case BlockType::DiamondBlock:                    return Item::DiamondBlock;
			case BlockType::DiamondOre:                      return Item::DiamondOre;
			case BlockType::Diorite:                         return Item::Diorite;
			case BlockType::DioriteSlab:                     return Item::DioriteSlab;
			case BlockType::DioriteStairs:                   return Item::DioriteStairs;
			case BlockType::DioriteWall:                     return Item::DioriteWall;
			case BlockType::Dirt:                            return Item::Dirt;
			case BlockType::Dispenser:                       return Item::Dispenser;
			case BlockType::DragonEgg:                       return Item::DragonEgg;
			case BlockType::DragonHead:                      return Item::DragonHead;
			case BlockType::DragonWallHead:                  return Item::DragonHead;
			case BlockType::DriedKelpBlock:                  return Item::DriedKelpBlock;
			case BlockType::Dropper:                         return Item::Dropper;
			case BlockType::EmeraldBlock:                    return Item::EmeraldBlock;
			case BlockType::EmeraldOre:                      return Item::EmeraldOre;
			case BlockType::EnchantingTable:                 return Item::EnchantingTable;
			case BlockType::EndGateway:                      return Item::Air;
			case BlockType::EndPortal:                       return Item::Air;
			case BlockType::EndPortalFrame:                  return Item::EndPortalFrame;
			case BlockType::EndRod:                          return Item::EndRod;
			case BlockType::EndStone:                        return Item::EndStone;
			case BlockType::EndStoneBrickSlab:               return Item::EndStoneBrickSlab;
			case BlockType::EndStoneBrickStairs:             return Item::EndStoneBrickStairs;
			case BlockType::EndStoneBrickWall:               return Item::EndStoneBrickWall;
			case BlockType::EndStoneBricks:                  return Item::EndStoneBricks;
			case BlockType::EnderChest:                      return Item::EnderChest;
			case BlockType::Farmland:                        return Item::Farmland;
			case BlockType::Fern:                            return Item::Fern;
			case BlockType::Fire:                            return Item::Air;
			case BlockType::FireCoral:                       return Item::FireCoral;
			case BlockType::FireCoralBlock:                  return Item::FireCoralBlock;
			case BlockType::FireCoralFan:                    return Item::FireCoralFan;
			case BlockType::FireCoralWallFan:                return Item::FireCoralFan;
			case BlockType::FletchingTable:                  return Item::FletchingTable;
			case BlockType::FlowerPot:                       return Item::FlowerPot;
			case BlockType::FrostedIce:                      return Item::Air;
			case BlockType::Furnace:                         return Item::Furnace;
			case BlockType::GildedBlackstone:                return Item::GildedBlackstone;
			case BlockType::Glass:                           return Item::Glass;
			case BlockType::GlassPane:                       return Item::GlassPane;
			case BlockType::Glowstone:                       return Item::Glowstone;
			case BlockType::GoldBlock:                       return Item::GoldBlock;
			case BlockType::GoldOre:                         return Item::GoldOre;
			case BlockType::Granite:                         return Item::Granite;
			case BlockType::GraniteSlab:                     return Item::GraniteSlab;
			case BlockType::GraniteStairs:                   return Item::GraniteStairs;
			case BlockType::GraniteWall:                     return Item::GraniteWall;
			case BlockType::Grass:                           return Item::Grass;
			case BlockType::GrassBlock:                      return Item::GrassBlock;
			case BlockType::GrassPath:                       return Item::GrassPath;
			case BlockType::Gravel:                          return Item::Gravel;
			case BlockType::GrayBanner:                      return Item::GrayBanner;
			case BlockType::GrayBed:                         return Item::GrayBed;
			case BlockType::GrayCarpet:                      return Item::GrayCarpet;
			case BlockType::GrayConcrete:                    return Item::GrayConcrete;
			case BlockType::GrayConcretePowder:              return Item::GrayConcretePowder;
			case BlockType::GrayGlazedTerracotta:            return Item::GrayGlazedTerracotta;
			case BlockType::GrayShulkerBox:                  return Item::GrayShulkerBox;
			case BlockType::GrayStainedGlass:                return Item::GrayStainedGlass;
			case BlockType::GrayStainedGlassPane:            return Item::GrayStainedGlassPane;
			case BlockType::GrayTerracotta:                  return Item::GrayTerracotta;
			case BlockType::GrayWallBanner:                  return Item::GrayBanner;
			case BlockType::GrayWool:                        return Item::GrayWool;
			case BlockType::GreenBanner:                     return Item::GreenBanner;
			case BlockType::GreenBed:                        return Item::GreenBed;
			case BlockType::GreenCarpet:                     return Item::GreenCarpet;
			case BlockType::GreenConcrete:                   return Item::GreenConcrete;
			case BlockType::GreenConcretePowder:             return Item::GreenConcretePowder;
			case BlockType::GreenGlazedTerracotta:           return Item::GreenGlazedTerracotta;
			case BlockType::GreenShulkerBox:                 return Item::GreenShulkerBox;
			case BlockType::GreenStainedGlass:               return Item::GreenStainedGlass;
			case BlockType::GreenStainedGlassPane:           return Item::GreenStainedGlassPane;
			case BlockType::GreenTerracotta:                 return Item::GreenTerracotta;
			case BlockType::GreenWallBanner:                 return Item::GreenBanner;
			case BlockType::GreenWool:                       return Item::GreenWool;
			case BlockType::Grindstone:                      return Item::Grindstone;
			case BlockType::HayBale:                         return Item::HayBale;
			case BlockType::HeavyWeightedPressurePlate:      return Item::HeavyWeightedPressurePlate;
			case BlockType::HoneyBlock:                      return Item::HoneyBlock;
			case BlockType::HoneycombBlock:                  return Item::HoneycombBlock;
			case BlockType::Hopper:                          return Item::Hopper;
			case BlockType::HornCoral:                       return Item::HornCoral;
			case BlockType::HornCoralBlock:                  return Item::HornCoralBlock;
			case BlockType::HornCoralFan:                    return Item::HornCoralFan;
			case BlockType::HornCoralWallFan:                return Item::HornCoralFan;
			case BlockType::Ice:                             return Item::Ice;
			case BlockType::InfestedChiseledStoneBricks:     return Item::InfestedChiseledStoneBricks;
			case BlockType::InfestedCobblestone:             return Item::InfestedCobblestone;
			case BlockType::InfestedCrackedStoneBricks:      return Item::InfestedCrackedStoneBricks;
			case BlockType::InfestedMossyStoneBricks:        return Item::InfestedMossyStoneBricks;
			case BlockType::InfestedStone:                   return Item::InfestedStone;
			case BlockType::InfestedStoneBricks:             return Item::InfestedStoneBricks;
			case BlockType::IronBars:                        return Item::IronBars;
			case BlockType::IronBlock:                       return Item::IronBlock;
			case BlockType::IronDoor:                        return Item::IronDoor;
			case BlockType::IronOre:                         return Item::IronOre;
			case BlockType::IronTrapdoor:                    return Item::IronTrapdoor;
			case BlockType::JackOLantern:                    return Item::JackOLantern;
			case BlockType::Jigsaw:                          return Item::Jigsaw;
			case BlockType::Jukebox:                         return Item::Jukebox;
			case BlockType::JungleButton:                    return Item::JungleButton;
			case BlockType::JungleDoor:                      return Item::JungleDoor;
			case BlockType::JungleFence:                     return Item::JungleFence;
			case BlockType::JungleFenceGate:                 return Item::JungleFenceGate;
			case BlockType::JungleLeaves:                    return Item::JungleLeaves;
			case BlockType::JungleLog:                       return Item::JungleLog;
			case BlockType::JunglePlanks:                    return Item::JunglePlanks;
			case BlockType::JunglePressurePlate:             return Item::JunglePressurePlate;
			case BlockType::JungleSapling:                   return Item::JungleSapling;
			case BlockType::JungleSign:                      return Item::JungleSign;
			case BlockType::JungleSlab:                      return Item::JungleSlab;
			case BlockType::JungleStairs:                    return Item::JungleStairs;
			case BlockType::JungleTrapdoor:                  return Item::JungleTrapdoor;
			case BlockType::JungleWallSign:                  return Item::JungleSign;
			case BlockType::JungleWood:                      return Item::JungleWood;
			case BlockType::Kelp:                            return Item::Kelp;
			case BlockType::KelpPlant:                       return Item::Kelp;
			case BlockType::Ladder:                          return Item::Ladder;
			case BlockType::Lantern:                         return Item::Lantern;
			case BlockType::LapisBlock:                      return Item::LapisBlock;
			case BlockType::LapisOre:                        return Item::LapisOre;
			case BlockType::LargeFern:                       return Item::LargeFern;
			case BlockType::Lava:                            return Item::Air;
			case BlockType::Lectern:                         return Item::Lectern;
			case BlockType::Lever:                           return Item::Lever;
			case BlockType::LightBlueBanner:                 return Item::LightBlueBanner;
			case BlockType::LightBlueBed:                    return Item::LightBlueBed;
			case BlockType::LightBlueCarpet:                 return Item::LightBlueCarpet;
			case BlockType::LightBlueConcrete:               return Item::LightBlueConcrete;
			case BlockType::LightBlueConcretePowder:         return Item::LightBlueConcretePowder;
			case BlockType::LightBlueGlazedTerracotta:       return Item::LightBlueGlazedTerracotta;
			case BlockType::LightBlueShulkerBox:             return Item::LightBlueShulkerBox;
			case BlockType::LightBlueStainedGlass:           return Item::LightBlueStainedGlass;
			case BlockType::LightBlueStainedGlassPane:       return Item::LightBlueStainedGlassPane;
			case BlockType::LightBlueTerracotta:             return Item::LightBlueTerracotta;
			case BlockType::LightBlueWallBanner:             return Item::LightBlueBanner;
			case BlockType::LightBlueWool:                   return Item::LightBlueWool;
			case BlockType::LightGrayBanner:                 return Item::LightGrayBanner;
			case BlockType::LightGrayBed:                    return Item::LightGrayBed;
			case BlockType::LightGrayCarpet:                 return Item::LightGrayCarpet;
			case BlockType::LightGrayConcrete:               return Item::LightGrayConcrete;
			case BlockType::LightGrayConcretePowder:         return Item::LightGrayConcretePowder;
			case BlockType::LightGrayGlazedTerracotta:       return Item::LightGrayGlazedTerracotta;
			case BlockType::LightGrayShulkerBox:             return Item::LightGrayShulkerBox;
			case BlockType::LightGrayStainedGlass:           return Item::LightGrayStainedGlass;
			case BlockType::LightGrayStainedGlassPane:       return Item::LightGrayStainedGlassPane;
			case BlockType::LightGrayTerracotta:             return Item::LightGrayTerracotta;
			case BlockType::LightGrayWallBanner:             return Item::LightGrayBanner;
			case BlockType::LightGrayWool:                   return Item::LightGrayWool;
			case BlockType::LightWeightedPressurePlate:      return Item::LightWeightedPressurePlate;
			case BlockType::Lilac:                           return Item::Lilac;
			case BlockType::LilyOfTheValley:                 return Item::LilyOfTheValley;
			case BlockType::LilyPad:                         return Item::LilyPad;
			case BlockType::LimeBanner:                      return Item::LimeBanner;
			case BlockType::LimeBed:                         return Item::LimeBed;
			case BlockType::LimeCarpet:                      return Item::LimeCarpet;
			case BlockType::LimeConcrete:                    return Item::LimeConcrete;
			case BlockType::LimeConcretePowder:              return Item::LimeConcretePowder;
			case BlockType::LimeGlazedTerracotta:            return Item::LimeGlazedTerracotta;
			case BlockType::LimeShulkerBox:                  return Item::LimeShulkerBox;
			case BlockType::LimeStainedGlass:                return Item::LimeStainedGlass;
			case BlockType::LimeStainedGlassPane:            return Item::LimeStainedGlassPane;
			case BlockType::LimeTerracotta:                  return Item::LimeTerracotta;
			case BlockType::LimeWallBanner:                  return Item::LimeBanner;
			case BlockType::LimeWool:                        return Item::LimeWool;
			case BlockType::Lodestone:                       return Item::Lodestone;
			case BlockType::Loom:                            return Item::Loom;
			case BlockType::MagentaBanner:                   return Item::MagentaBanner;
			case BlockType::MagentaBed:                      return Item::MagentaBed;
			case BlockType::MagentaCarpet:                   return Item::MagentaCarpet;
			case BlockType::MagentaConcrete:                 return Item::MagentaConcrete;
			case BlockType::MagentaConcretePowder:           return Item::MagentaConcretePowder;
			case BlockType::MagentaGlazedTerracotta:         return Item::MagentaGlazedTerracotta;
			case BlockType::MagentaShulkerBox:               return Item::MagentaShulkerBox;
			case BlockType::MagentaStainedGlass:             return Item::MagentaStainedGlass;
			case BlockType::MagentaStainedGlassPane:         return Item::MagentaStainedGlassPane;
			case BlockType::MagentaTerracotta:               return Item::MagentaTerracotta;
			case BlockType::MagentaWallBanner:               return Item::MagentaBanner;
			case BlockType::MagentaWool:                     return Item::MagentaWool;
			case BlockType::MagmaBlock:                      return Item::MagmaBlock;
			case BlockType::Melon:                           return Item::Melon;
			case BlockType::MelonStem:                       return Item::Air;
			case BlockType::MossyCobblestone:                return Item::MossyCobblestone;
			case BlockType::MossyCobblestoneSlab:            return Item::MossyCobblestoneSlab;
			case BlockType::MossyCobblestoneStairs:          return Item::MossyCobblestoneStairs;
			case BlockType::MossyCobblestoneWall:            return Item::MossyCobblestoneWall;
			case BlockType::MossyStoneBrickSlab:             return Item::MossyStoneBrickSlab;
			case BlockType::MossyStoneBrickStairs:           return Item::MossyStoneBrickStairs;
			case BlockType::MossyStoneBrickWall:             return Item::MossyStoneBrickWall;
			case BlockType::MossyStoneBricks:                return Item::MossyStoneBricks;
			case BlockType::MovingPiston:                    return Item::Air;
			case BlockType::MushroomStem:                    return Item::MushroomStem;
			case BlockType::Mycelium:                        return Item::Mycelium;
			case BlockType::NetherBrickFence:                return Item::NetherBrickFence;
			case BlockType::NetherBrickSlab:                 return Item::NetherBrickSlab;
			case BlockType::NetherBrickStairs:               return Item::NetherBrickStairs;
			case BlockType::NetherBrickWall:                 return Item::NetherBrickWall;
			case BlockType::NetherBricks:                    return Item::NetherBricks;
			case BlockType::NetherGoldOre:                   return Item::NetherGoldOre;
			case BlockType::NetherPortal:                    return Item::Air;
			case BlockType::NetherQuartzOre:                 return Item::NetherQuartzOre;
			case BlockType::NetherSprouts:                   return Item::NetherSprouts;
			case BlockType::NetherWart:                      return Item::NetherWart;
			case BlockType::NetherWartBlock:                 return Item::NetherWartBlock;
			case BlockType::NetheriteBlock:                  return Item::NetheriteBlock;
			case BlockType::Netherrack:                      return Item::Netherrack;
			case BlockType::NoteBlock:                       return Item::NoteBlock;
			case BlockType::OakButton:                       return Item::OakButton;
			case BlockType::OakDoor:                         return Item::OakDoor;
			case BlockType::OakFence:                        return Item::OakFence;
			case BlockType::OakFenceGate:                    return Item::OakFenceGate;
			case BlockType::OakLeaves:                       return Item::OakLeaves;
			case BlockType::OakLog:                          return Item::OakLog;
			case BlockType::OakPlanks:                       return Item::OakPlanks;
			case BlockType::OakPressurePlate:                return Item::OakPressurePlate;
			case BlockType::OakSapling:                      return Item::OakSapling;
			case BlockType::OakSign:                         return Item::OakSign;
			case BlockType::OakSlab:                         return Item::OakSlab;
			case BlockType::OakStairs:                       return Item::OakStairs;
			case BlockType::OakTrapdoor:                     return Item::OakTrapdoor;
			case BlockType::OakWallSign:                     return Item::OakSign;
			case BlockType::OakWood:                         return Item::OakWood;
			case BlockType::Observer:                        return Item::Observer;
			case BlockType::Obsidian:                        return Item::Obsidian;
			case BlockType::OrangeBanner:                    return Item::OrangeBanner;
			case BlockType::OrangeBed:                       return Item::OrangeBed;
			case BlockType::OrangeCarpet:                    return Item::OrangeCarpet;
			case BlockType::OrangeConcrete:                  return Item::OrangeConcrete;
			case BlockType::OrangeConcretePowder:            return Item::OrangeConcretePowder;
			case BlockType::OrangeGlazedTerracotta:          return Item::OrangeGlazedTerracotta;
			case BlockType::OrangeShulkerBox:                return Item::OrangeShulkerBox;
			case BlockType::OrangeStainedGlass:              return Item::OrangeStainedGlass;
			case BlockType::OrangeStainedGlassPane:          return Item::OrangeStainedGlassPane;
			case BlockType::OrangeTerracotta:                return Item::OrangeTerracotta;
			case BlockType::OrangeTulip:                     return Item::OrangeTulip;
			case BlockType::OrangeWallBanner:                return Item::OrangeBanner;
			case BlockType::OrangeWool:                      return Item::OrangeWool;
			case BlockType::OxeyeDaisy:                      return Item::OxeyeDaisy;
			case BlockType::PackedIce:                       return Item::PackedIce;
			case BlockType::Peony:                           return Item::Peony;
			case BlockType::PetrifiedOakSlab:                return Item::PetrifiedOakSlab;
			case BlockType::PinkBanner:                      return Item::PinkBanner;
			case BlockType::PinkBed:                         return Item::PinkBed;
			case BlockType::PinkCarpet:                      return Item::PinkCarpet;
			case BlockType::PinkConcrete:                    return Item::PinkConcrete;
			case BlockType::PinkConcretePowder:              return Item::PinkConcretePowder;
			case BlockType::PinkGlazedTerracotta:            return Item::PinkGlazedTerracotta;
			case BlockType::PinkShulkerBox:                  return Item::PinkShulkerBox;
			case BlockType::PinkStainedGlass:                return Item::PinkStainedGlass;
			case BlockType::PinkStainedGlassPane:            return Item::PinkStainedGlassPane;
			case BlockType::PinkTerracotta:                  return Item::PinkTerracotta;
			case BlockType::PinkTulip:                       return Item::PinkTulip;
			case BlockType::PinkWallBanner:                  return Item::PinkBanner;
			case BlockType::PinkWool:                        return Item::PinkWool;
			case BlockType::Piston:                          return Item::Piston;
			case BlockType::PistonHead:                      return Item::Air;
			case BlockType::PlayerHead:                      return Item::PlayerHead;
			case BlockType::PlayerWallHead:                  return Item::PlayerHead;
			case BlockType::Podzol:                          return Item::Podzol;
			case BlockType::PolishedAndesite:                return Item::PolishedAndesite;
			case BlockType::PolishedAndesiteSlab:            return Item::PolishedAndesiteSlab;
			case BlockType::PolishedAndesiteStairs:          return Item::PolishedAndesiteStairs;
			case BlockType::PolishedBasalt:                  return Item::PolishedBasalt;
			case BlockType::PolishedBlackstone:              return Item::PolishedBlackstone;
			case BlockType::PolishedBlackstoneBrickSlab:     return Item::PolishedBlackstoneBrickSlab;
			case BlockType::PolishedBlackstoneBrickStairs:   return Item::PolishedBlackstoneBrickStairs;
			case BlockType::PolishedBlackstoneBrickWall:     return Item::PolishedBlackstoneBrickWall;
			case BlockType::PolishedBlackstoneBricks:        return Item::PolishedBlackstoneBricks;
			case BlockType::PolishedBlackstoneButton:        return Item::PolishedBlackstoneButton;
			case BlockType::PolishedBlackstonePressurePlate: return Item::PolishedBlackstonePressurePlate;
			case BlockType::PolishedBlackstoneSlab:          return Item::PolishedBlackstoneSlab;
			case BlockType::PolishedBlackstoneStairs:        return Item::PolishedBlackstoneStairs;
			case BlockType::PolishedBlackstoneWall:          return Item::PolishedBlackstoneWall;
			case BlockType::PolishedDiorite:                 return Item::PolishedDiorite;
			case BlockType::PolishedDioriteSlab:             return Item::PolishedDioriteSlab;
			case BlockType::PolishedDioriteStairs:           return Item::PolishedDioriteStairs;
			case BlockType::PolishedGranite:                 return Item::PolishedGranite;
			case BlockType::PolishedGraniteSlab:             return Item::PolishedGraniteSlab;
			case BlockType::PolishedGraniteStairs:           return Item::PolishedGraniteStairs;
			case BlockType::Poppy:                           return Item::Poppy;
			case BlockType::Potatoes:                        return Item::Potato;
			case BlockType::PottedAcaciaSapling:             return Item::Air;
			case BlockType::PottedAllium:                    return Item::Air;
			case BlockType::PottedAzureBluet:                return Item::Air;
			case BlockType::PottedBamboo:                    return Item::Air;
			case BlockType::PottedBirchSapling:              return Item::Air;
			case BlockType::PottedBlueOrchid:                return Item::Air;
			case BlockType::PottedBrownMushroom:             return Item::Air;
			case BlockType::PottedCactus:                    return Item::Air;
			case BlockType::PottedCornflower:                return Item::Air;
			case BlockType::PottedCrimsonFungus:             return Item::Air;
			case BlockType::PottedCrimsonRoots:              return Item::Air;
			case BlockType::PottedDandelion:                 return Item::Air;
			case BlockType::PottedDarkOakSapling:            return Item::Air;
			case BlockType::PottedDeadBush:                  return Item::Air;
			case BlockType::PottedFern:                      return Item::Air;
			case BlockType::PottedJungleSapling:             return Item::Air;
			case BlockType::PottedLilyOfTheValley:           return Item::Air;
			case BlockType::PottedOakSapling:                return Item::Air;
			case BlockType::PottedOrangeTulip:               return Item::Air;
			case BlockType::PottedOxeyeDaisy:                return Item::Air;
			case BlockType::PottedPinkTulip:                 return Item::Air;
			case BlockType::PottedPoppy:                     return Item::Air;
			case BlockType::PottedRedMushroom:               return Item::Air;
			case BlockType::PottedRedTulip:                  return Item::Air;
			case BlockType::PottedSpruceSapling:             return Item::Air;
			case BlockType::PottedWarpedFungus:              return Item::Air;
			case BlockType::PottedWarpedRoots:               return Item::Air;
			case BlockType::PottedWhiteTulip:                return Item::Air;
			case BlockType::PottedWitherRose:                return Item::Air;
			case BlockType::PoweredRail:                     return Item::PoweredRail;
			case BlockType::Prismarine:                      return Item::Prismarine;
			case BlockType::PrismarineBrickSlab:             return Item::PrismarineBrickSlab;
			case BlockType::PrismarineBrickStairs:           return Item::PrismarineBrickStairs;
			case BlockType::PrismarineBricks:                return Item::PrismarineBricks;
			case BlockType::PrismarineSlab:                  return Item::PrismarineSlab;
			case BlockType::PrismarineStairs:                return Item::PrismarineStairs;
			case BlockType::PrismarineWall:                  return Item::PrismarineWall;
			case BlockType::Pumpkin:                         return Item::Pumpkin;
			case BlockType::PumpkinStem:                     return Item::Air;
			case BlockType::PurpleBanner:                    return Item::PurpleBanner;
			case BlockType::PurpleBed:                       return Item::PurpleBed;
			case BlockType::PurpleCarpet:                    return Item::PurpleCarpet;
			case BlockType::PurpleConcrete:                  return Item::PurpleConcrete;
			case BlockType::PurpleConcretePowder:            return Item::PurpleConcretePowder;
			case BlockType::PurpleGlazedTerracotta:          return Item::PurpleGlazedTerracotta;
			case BlockType::PurpleShulkerBox:                return Item::PurpleShulkerBox;
			case BlockType::PurpleStainedGlass:              return Item::PurpleStainedGlass;
			case BlockType::PurpleStainedGlassPane:          return Item::PurpleStainedGlassPane;
			case BlockType::PurpleTerracotta:                return Item::PurpleTerracotta;
			case BlockType::PurpleWallBanner:                return Item::PurpleBanner;
			case BlockType::PurpleWool:                      return Item::PurpleWool;
			case BlockType::PurpurBlock:                     return Item::PurpurBlock;
			case BlockType::PurpurPillar:                    return Item::PurpurPillar;
			case BlockType::PurpurSlab:                      return Item::PurpurSlab;
			case BlockType::PurpurStairs:                    return Item::PurpurStairs;
			case BlockType::QuartzBlock:                     return Item::QuartzBlock;
			case BlockType::QuartzBricks:                    return Item::QuartzBricks;
			case BlockType::QuartzPillar:                    return Item::QuartzPillar;
			case BlockType::QuartzSlab:                      return Item::QuartzSlab;
			case BlockType::QuartzStairs:                    return Item::QuartzStairs;
			case BlockType::Rail:                            return Item::Rail;
			case BlockType::RedBanner:                       return Item::RedBanner;
			case BlockType::RedBed:                          return Item::RedBed;
			case BlockType::RedCarpet:                       return Item::RedCarpet;
			case BlockType::RedConcrete:                     return Item::RedConcrete;
			case BlockType::RedConcretePowder:               return Item::RedConcretePowder;
			case BlockType::RedGlazedTerracotta:             return Item::RedGlazedTerracotta;
			case BlockType::RedMushroom:                     return Item::RedMushroom;
			case BlockType::RedMushroomBlock:                return Item::RedMushroomBlock;
			case BlockType::RedNetherBrickSlab:              return Item::RedNetherBrickSlab;
			case BlockType::RedNetherBrickStairs:            return Item::RedNetherBrickStairs;
			case BlockType::RedNetherBrickWall:              return Item::RedNetherBrickWall;
			case BlockType::RedNetherBricks:                 return Item::RedNetherBricks;
			case BlockType::RedSand:                         return Item::RedSand;
			case BlockType::RedSandstone:                    return Item::RedSandstone;
			case BlockType::RedSandstoneSlab:                return Item::RedSandstoneSlab;
			case BlockType::RedSandstoneStairs:              return Item::RedSandstoneStairs;
			case BlockType::RedSandstoneWall:                return Item::RedSandstoneWall;
			case BlockType::RedShulkerBox:                   return Item::RedShulkerBox;
			case BlockType::RedStainedGlass:                 return Item::RedStainedGlass;
			case BlockType::RedStainedGlassPane:             return Item::RedStainedGlassPane;
			case BlockType::RedTerracotta:                   return Item::RedTerracotta;
			case BlockType::RedTulip:                        return Item::RedTulip;
			case BlockType::RedWallBanner:                   return Item::RedBanner;
			case BlockType::RedWool:                         return Item::RedWool;
			case BlockType::RedstoneBlock:                   return Item::RedstoneBlock;
			case BlockType::RedstoneLamp:                    return Item::RedstoneLamp;
			case BlockType::RedstoneOre:                     return Item::RedstoneOre;
			case BlockType::RedstoneTorch:                   return Item::RedstoneTorch;
			case BlockType::RedstoneWallTorch:               return Item::RedstoneTorch;
			case BlockType::RedstoneWire:                    return Item::Redstone;
			case BlockType::Repeater:                        return Item::Repeater;
			case BlockType::RepeatingCommandBlock:           return Item::RepeatingCommandBlock;
			case BlockType::RespawnAnchor:                   return Item::RespawnAnchor;
			case BlockType::RoseBush:                        return Item::RoseBush;
			case BlockType::Sand:                            return Item::Sand;
			case BlockType::Sandstone:                       return Item::Sandstone;
			case BlockType::SandstoneSlab:                   return Item::SandstoneSlab;
			case BlockType::SandstoneStairs:                 return Item::SandstoneStairs;
			case BlockType::SandstoneWall:                   return Item::SandstoneWall;
			case BlockType::Scaffolding:                     return Item::Scaffolding;
			case BlockType::SeaLantern:                      return Item::SeaLantern;
			case BlockType::SeaPickle:                       return Item::SeaPickle;
			case BlockType::Seagrass:                        return Item::Seagrass;
			case BlockType::Shroomlight:                     return Item::Shroomlight;
			case BlockType::ShulkerBox:                      return Item::ShulkerBox;
			case BlockType::SkeletonSkull:                   return Item::SkeletonSkull;
			case BlockType::SkeletonWallSkull:               return Item::SkeletonSkull;
			case BlockType::SlimeBlock:                      return Item::SlimeBlock;
			case BlockType::SmithingTable:                   return Item::SmithingTable;
			case BlockType::Smoker:                          return Item::Smoker;
			case BlockType::SmoothQuartz:                    return Item::SmoothQuartz;
			case BlockType::SmoothQuartzSlab:                return Item::SmoothQuartzSlab;
			case BlockType::SmoothQuartzStairs:              return Item::SmoothQuartzStairs;
			case BlockType::SmoothRedSandstone:              return Item::SmoothRedSandstone;
			case BlockType::SmoothRedSandstoneSlab:          return Item::SmoothRedSandstoneSlab;
			case BlockType::SmoothRedSandstoneStairs:        return Item::SmoothRedSandstoneStairs;
			case BlockType::SmoothSandstone:                 return Item::SmoothSandstone;
			case BlockType::SmoothSandstoneSlab:             return Item::SmoothSandstoneSlab;
			case BlockType::SmoothSandstoneStairs:           return Item::SmoothSandstoneStairs;
			case BlockType::SmoothStone:                     return Item::SmoothStone;
			case BlockType::SmoothStoneSlab:                 return Item::SmoothStoneSlab;
			case BlockType::Snow:                            return Item::Snow;
			case BlockType::SnowBlock:                       return Item::SnowBlock;
			case BlockType::SoulCampfire:                    return Item::SoulCampfire;
			case BlockType::SoulFire:                        return Item::Air;
			case BlockType::SoulLantern:                     return Item::SoulLantern;
			case BlockType::SoulSand:                        return Item::SoulSand;
			case BlockType::SoulSoil:                        return Item::SoulSoil;
			case BlockType::SoulTorch:                       return Item::SoulTorch;
			case BlockType::SoulWallTorch:                   return Item::SoulTorch;
			case BlockType::Spawner:                         return Item::Spawner;
			case BlockType::Sponge:                          return Item::Sponge;
			case BlockType::SpruceButton:                    return Item::SpruceButton;
			case BlockType::SpruceDoor:                      return Item::SpruceDoor;
			case BlockType::SpruceFence:                     return Item::SpruceFence;
			case BlockType::SpruceFenceGate:                 return Item::SpruceFenceGate;
			case BlockType::SpruceLeaves:                    return Item::SpruceLeaves;
			case BlockType::SpruceLog:                       return Item::SpruceLog;
			case BlockType::SprucePlanks:                    return Item::SprucePlanks;
			case BlockType::SprucePressurePlate:             return Item::SprucePressurePlate;
			case BlockType::SpruceSapling:                   return Item::SpruceSapling;
			case BlockType::SpruceSign:                      return Item::SpruceSign;
			case BlockType::SpruceSlab:                      return Item::SpruceSlab;
			case BlockType::SpruceStairs:                    return Item::SpruceStairs;
			case BlockType::SpruceTrapdoor:                  return Item::SpruceTrapdoor;
			case BlockType::SpruceWallSign:                  return Item::SpruceSign;
			case BlockType::SpruceWood:                      return Item::SpruceWood;
			case BlockType::StickyPiston:                    return Item::StickyPiston;
			case BlockType::Stone:                           return Item::Stone;
			case BlockType::StoneBrickSlab:                  return Item::StoneBrickSlab;
			case BlockType::StoneBrickStairs:                return Item::StoneBrickStairs;
			case BlockType::StoneBrickWall:                  return Item::StoneBrickWall;
			case BlockType::StoneBricks:                     return Item::StoneBricks;
			case BlockType::StoneButton:                     return Item::StoneButton;
			case BlockType::StonePressurePlate:              return Item::StonePressurePlate;
			case BlockType::StoneSlab:                       return Item::StoneSlab;
			case BlockType::StoneStairs:                     return Item::StoneStairs;
			case BlockType::Stonecutter:                     return Item::Stonecutter;
			case BlockType::StrippedAcaciaLog:               return Item::StrippedAcaciaLog;
			case BlockType::StrippedAcaciaWood:              return Item::StrippedAcaciaWood;
			case BlockType::StrippedBirchLog:                return Item::StrippedBirchLog;
			case BlockType::StrippedBirchWood:               return Item::StrippedBirchWood;
			case BlockType::StrippedCrimsonHyphae:           return Item::StrippedCrimsonHyphae;
			case BlockType::StrippedCrimsonStem:             return Item::StrippedCrimsonStem;
			case BlockType::StrippedDarkOakLog:              return Item::StrippedDarkOakLog;
			case BlockType::StrippedDarkOakWood:             return Item::StrippedDarkOakWood;
			case BlockType::StrippedJungleLog:               return Item::StrippedJungleLog;
			case BlockType::StrippedJungleWood:              return Item::StrippedJungleWood;
			case BlockType::StrippedOakLog:                  return Item::StrippedOakLog;
			case BlockType::StrippedOakWood:                 return Item::StrippedOakWood;
			case BlockType::StrippedSpruceLog:               return Item::StrippedSpruceLog;
			case BlockType::StrippedSpruceWood:              return Item::StrippedSpruceWood;
			case BlockType::StrippedWarpedHyphae:            return Item::StrippedWarpedHyphae;
			case BlockType::StrippedWarpedStem:              return Item::StrippedWarpedStem;
			case BlockType::StructureBlock:                  return Item::StructureBlock;
			case BlockType::StructureVoid:                   return Item::StructureVoid;
			case BlockType::SugarCane:                       return Item::SugarCane;
			case BlockType::Sunflower:                       return Item::Sunflower;
			case BlockType::SweetBerryBush:                  return Item::Air;
			case BlockType::TNT:                             return Item::TNT;
			case BlockType::TallGrass:                       return Item::TallGrass;
			case BlockType::TallSeagrass:                    return Item::Air;
			case BlockType::Target:                          return Item::Target;
			case BlockType::Terracotta:                      return Item::Terracotta;
			case BlockType::Torch:                           return Item::Torch;
			case BlockType::TrappedChest:                    return Item::TrappedChest;
			case BlockType::Tripwire:                        return Item::String;
			case BlockType::TripwireHook:                    return Item::TripwireHook;
			case BlockType::TubeCoral:                       return Item::TubeCoral;
			case BlockType::TubeCoralBlock:                  return Item::TubeCoralBlock;
			case BlockType::TubeCoralFan:                    return Item::TubeCoralFan;
			case BlockType::TubeCoralWallFan:                return Item::TubeCoralFan;
			case BlockType::TurtleEgg:                       return Item::TurtleEgg;
			case BlockType::TwistingVines:                   return Item::TwistingVines;
			case BlockType::TwistingVinesPlant:              return Item::TwistingVines;
			case BlockType::Vine:                            return Item::Vine;
			case BlockType::VoidAir:                         return Item::Air;
			case BlockType::WallTorch:                       return Item::Torch;
			case BlockType::WarpedButton:                    return Item::WarpedButton;
			case BlockType::WarpedDoor:                      return Item::WarpedDoor;
			case BlockType::WarpedFence:                     return Item::WarpedFence;
			case BlockType::WarpedFenceGate:                 return Item::WarpedFenceGate;
			case BlockType::WarpedFungus:                    return Item::WarpedFungus;
			case BlockType::WarpedHyphae:                    return Item::WarpedHyphae;
			case BlockType::WarpedNylium:                    return Item::WarpedNylium;
			case BlockType::WarpedPlanks:                    return Item::WarpedPlanks;
			case BlockType::WarpedPressurePlate:             return Item::WarpedPressurePlate;
			case BlockType::WarpedRoots:                     return Item::WarpedRoots;
			case BlockType::WarpedSign:                      return Item::WarpedSign;
			case BlockType::WarpedSlab:                      return Item::WarpedSlab;
			case BlockType::WarpedStairs:                    return Item::WarpedStairs;
			case BlockType::WarpedStem:                      return Item::WarpedStem;
			case BlockType::WarpedTrapdoor:                  return Item::WarpedTrapdoor;
			case BlockType::WarpedWallSign:                  return Item::WarpedSign;
			case BlockType::WarpedWartBlock:                 return Item::WarpedWartBlock;
			case BlockType::Water:                           return Item::Air;
			case BlockType::WeepingVines:                    return Item::WeepingVines;
			case BlockType::WeepingVinesPlant:               return Item::WeepingVines;
			case BlockType::WetSponge:                       return Item::WetSponge;
			case BlockType::Wheat:                           return Item::Wheat;
			case BlockType::WhiteBanner:                     return Item::WhiteBanner;
			case BlockType::WhiteBed:                        return Item::WhiteBed;
			case BlockType::WhiteCarpet:                     return Item::WhiteCarpet;
			case BlockType::WhiteConcrete:                   return Item::WhiteConcrete;
			case BlockType::WhiteConcretePowder:             return Item::WhiteConcretePowder;
			case BlockType::WhiteGlazedTerracotta:           return Item::WhiteGlazedTerracotta;
			case BlockType::WhiteShulkerBox:                 return Item::WhiteShulkerBox;
			case BlockType::WhiteStainedGlass:               return Item::WhiteStainedGlass;
			case BlockType::WhiteStainedGlassPane:           return Item::WhiteStainedGlassPane;
			case BlockType::WhiteTerracotta:                 return Item::WhiteTerracotta;
			case BlockType::WhiteTulip:                      return Item::WhiteTulip;
			case BlockType::WhiteWallBanner:                 return Item::WhiteBanner;
			case BlockType::WhiteWool:                       return Item::WhiteWool;
			case BlockType::WitherRose:                      return Item::WitherRose;
			case BlockType::WitherSkeletonSkull:             return Item::WitherSkeletonSkull;
			case BlockType::WitherSkeletonWallSkull:         return Item::WitherSkeletonSkull;
			case BlockType::YellowBanner:                    return Item::YellowBanner;
			case BlockType::YellowBed:                       return Item::YellowBed;
			case BlockType::YellowCarpet:                    return Item::YellowCarpet;
			case BlockType::YellowConcrete:                  return Item::YellowConcrete;
			case BlockType::YellowConcretePowder:            return Item::YellowConcretePowder;
			case BlockType::YellowGlazedTerracotta:          return Item::YellowGlazedTerracotta;
			case BlockType::YellowShulkerBox:                return Item::YellowShulkerBox;
			case BlockType::YellowStainedGlass:              return Item::YellowStainedGlass;
			case BlockType::YellowStainedGlassPane:          return Item::YellowStainedGlassPane;
			case BlockType::YellowTerracotta:                return Item::YellowTerracotta;
			case BlockType::YellowWallBanner:                return Item::YellowBanner;
			case BlockType::YellowWool:                      return Item::YellowWool;
			case BlockType::ZombieHead:                      return Item::ZombieHead;
			case BlockType::ZombieWallHead:                  return Item::ZombieHead;
		}
		FLOGERROR("{}: Got unintended block to convert to item: {}", __FUNCTION__, a_Block);
		ASSERT(!"Converting unintended block to item");
		return Item::Air;
	}
}
