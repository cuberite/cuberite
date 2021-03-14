#include "Globals.h"

#include "BlockInfo.h"
#include "BlockType.h"

#include "Blocks/BlockHandler.h"
#include "Blocks/BlockSlab.h"





bool IsBlockWater(BlockType a_BlockType)
{
	return a_BlockType == BlockType::Water;
}





bool IsBlockIce(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::ICE:
		case BlockType::PACKED_ICE:
		case BlockType::FROSTED_ICE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockWaterOrIce(BLOCKTYPE a_BlockType)
{
	return (IsBlockWater(a_BlockType) || IsBlockIce(a_BlockType));
}





bool IsBlockLava(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == BlockType::LAVA) || (a_BlockType == BlockType::STATIONARY_LAVA));
}





bool IsBlockLiquid(BLOCKTYPE a_BlockType)
{
	return IsBlockWater(a_BlockType) || IsBlockLava(a_BlockType);
}





bool IsBlockTypeOfDirt(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::DIRT:
		case BlockType::GRASS:
		case BlockType::FARMLAND:
		case BlockType::GRASS_PATH:
		{
			return true;
		}
	}
	return false;
}





bool IsBlockFence(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::ACACIA_FENCE:
		case BlockType::ACACIA_FENCE_GATE:
		case BlockType::BIRCH_FENCE:
		case BlockType::BIRCH_FENCE_GATE:
		case BlockType::COBBLESTONE_WALL:
		case BlockType::DARK_OAK_FENCE:
		case BlockType::DARK_OAK_FENCE_GATE:
		case BlockType::FENCE:
		case BlockType::JUNGLE_FENCE:
		case BlockType::JUNGLE_FENCE_GATE:
		case BlockType::NETHER_BRICK_FENCE:
		case BlockType::OAK_FENCE_GATE:
		case BlockType::SPRUCE_FENCE:
		case BlockType::SPRUCE_FENCE_GATE:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockShulkerBox(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType >= BlockType::WHITE_SHULKER_BOX) && (a_BlockType <= BlockType::BLACK_SHULKER_BOX));
}





bool IsBlockMaterialWood(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::PLANKS:
		case BlockType::LOG:
		case BlockType::NOTE_BLOCK:
		case BlockType::BOOKCASE:
		case BlockType::OAK_WOOD_STAIRS:
		case BlockType::CHEST:
		case BlockType::CRAFTING_TABLE:
		case BlockType::SIGN_POST:
		case BlockType::OAK_DOOR:
		case BlockType::WALLSIGN:
		case BlockType::WOODEN_PRESSURE_PLATE:
		case BlockType::JUKEBOX:
		case BlockType::FENCE:
		case BlockType::TRAPDOOR:
		case BlockType::HUGE_BROWN_MUSHROOM:
		case BlockType::HUGE_RED_MUSHROOM:
		case BlockType::OAK_FENCE_GATE:
		case BlockType::DOUBLE_WOODEN_SLAB:
		case BlockType::WOODEN_SLAB:
		case BlockType::SPRUCE_WOOD_STAIRS:
		case BlockType::BIRCH_WOOD_STAIRS:
		case BlockType::JUNGLE_WOOD_STAIRS:
		case BlockType::TRAPPED_CHEST:
		case BlockType::DAYLIGHT_SENSOR:
		case BlockType::NEW_LOG:
		case BlockType::ACACIA_WOOD_STAIRS:
		case BlockType::DARK_OAK_WOOD_STAIRS:
		case BlockType::STANDING_BANNER:
		case BlockType::WALL_BANNER:
		case BlockType::INVERTED_DAYLIGHT_SENSOR:
		case BlockType::SPRUCE_FENCE_GATE:
		case BlockType::BIRCH_FENCE_GATE:
		case BlockType::JUNGLE_FENCE_GATE:
		case BlockType::DARK_OAK_FENCE_GATE:
		case BlockType::ACACIA_FENCE_GATE:
		case BlockType::SPRUCE_FENCE:
		case BlockType::BIRCH_FENCE:
		case BlockType::JUNGLE_FENCE:
		case BlockType::DARK_OAK_FENCE:
		case BlockType::ACACIA_FENCE:
		case BlockType::SPRUCE_DOOR:
		case BlockType::BIRCH_DOOR:
		case BlockType::JUNGLE_DOOR:
		case BlockType::ACACIA_DOOR:
		case BlockType::DARK_OAK_DOOR:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialPlants(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::SAPLING:
		case BlockType::DANDELION:
		case BlockType::FLOWER:
		case BlockType::BROWN_MUSHROOM:
		case BlockType::RED_MUSHROOM:
		case BlockType::CROPS:
		case BlockType::REEDS:
		case BlockType::PUMPKIN_STEM:
		case BlockType::MELON_STEM:
		case BlockType::LILY_PAD:
		case BlockType::NETHER_WART:
		case BlockType::COCOA_POD:
		case BlockType::CARROTS:
		case BlockType::POTATOES:
		case BlockType::CHORUS_PLANT:
		case BlockType::CHORUS_FLOWER:
		case BlockType::BEETROOTS:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialVine(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::TALL_GRASS:
		case BlockType::DEAD_BUSH:
		case BlockType::VINES:
		case BlockType::BIG_FLOWER:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialIron(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::LAPIS_BLOCK:
		case BlockType::GOLD_BLOCK:
		case BlockType::IRON_BLOCK:
		case BlockType::DIAMOND_BLOCK:
		case BlockType::IRON_DOOR:
		case BlockType::IRON_BARS:
		case BlockType::BREWING_STAND:
		case BlockType::CAULDRON:
		case BlockType::EMERALD_BLOCK:
		case BlockType::COMMAND_BLOCK:
		case BlockType::LIGHT_WEIGHTED_PRESSURE_PLATE:
		case BlockType::HEAVY_WEIGHTED_PRESSURE_PLATE:
		case BlockType::BLOCK_OF_REDSTONE:
		case BlockType::HOPPER:
		case BlockType::IRON_TRAPDOOR:
		case BlockType::REPEATING_COMMAND_BLOCK:
		case BlockType::CHAIN_COMMAND_BLOCK:
		case BlockType::STRUCTURE_BLOCK:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialLeaves(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == BlockType::LEAVES) || (a_BlockType == BlockType::NEW_LEAVES);
}





bool IsBlockMaterialGourd(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::PUMPKIN:
		case BlockType::JACK_O_LANTERN:
		case BlockType::MELON:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialRock(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::STONE:
		case BlockType::COBBLESTONE:
		case BlockType::BEDROCK:
		case BlockType::GOLD_ORE:
		case BlockType::IRON_ORE:
		case BlockType::COAL_ORE:
		case BlockType::LAPIS_ORE:
		case BlockType::DISPENSER:
		case BlockType::SANDSTONE:
		case BlockType::DOUBLE_STONE_SLAB:
		case BlockType::STONE_SLAB:
		case BlockType::BRICK:
		case BlockType::MOSSY_COBBLESTONE:
		case BlockType::OBSIDIAN:
		case BlockType::MOB_SPAWNER:
		case BlockType::DIAMOND_ORE:
		case BlockType::FURNACE:
		case BlockType::LIT_FURNACE:
		case BlockType::COBBLESTONE_STAIRS:
		case BlockType::STONE_PRESSURE_PLATE:
		case BlockType::REDSTONE_ORE:
		case BlockType::REDSTONE_ORE_GLOWING:
		case BlockType::NETHERRACK:
		case BlockType::STONE_BRICKS:
		case BlockType::BRICK_STAIRS:
		case BlockType::STONE_BRICK_STAIRS:
		case BlockType::NETHER_BRICK:
		case BlockType::NETHER_BRICK_FENCE:
		case BlockType::NETHER_BRICK_STAIRS:
		case BlockType::ENCHANTMENT_TABLE:
		case BlockType::END_PORTAL_FRAME:
		case BlockType::END_STONE:
		case BlockType::SANDSTONE_STAIRS:
		case BlockType::EMERALD_ORE:
		case BlockType::ENDER_CHEST:
		case BlockType::COBBLESTONE_WALL:
		case BlockType::NETHER_QUARTZ_ORE:
		case BlockType::QUARTZ_BLOCK:
		case BlockType::QUARTZ_STAIRS:
		case BlockType::DROPPER:
		case BlockType::STAINED_CLAY:
		case BlockType::PRISMARINE_BLOCK:
		case BlockType::HARDENED_CLAY:
		case BlockType::BLOCK_OF_COAL:
		case BlockType::RED_SANDSTONE:
		case BlockType::RED_SANDSTONE_STAIRS:
		case BlockType::DOUBLE_RED_SANDSTONE_SLAB:
		case BlockType::RED_SANDSTONE_SLAB:
		case BlockType::PURPUR_BLOCK:
		case BlockType::PURPUR_PILLAR:
		case BlockType::PURPUR_STAIRS:
		case BlockType::PURPUR_DOUBLE_SLAB:
		case BlockType::PURPUR_SLAB:
		case BlockType::END_BRICKS:
		case BlockType::MAGMA:
		case BlockType::RED_NETHER_BRICK:
		case BlockType::BONE_BLOCK:
		case BlockType::OBSERVER:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBed(BlockType a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::BlackBed:
		case BlockType::BlueBed:
		case BlockType::BrownBed:
		case BlockType::CyanBed:
		case BlockType::GrayBed:
		case BlockType::GreenBed:
		case BlockType::LightBlueBed:
		case BlockType::LightGrayBed:
		case BlockType::LimeBed:
		case BlockType::MagentaBed:
		case BlockType::OrangeBed:
		case BlockType::PinkBed:
		case BlockType::PurpleBed:
		case BlockType::RedBed:
		case BlockType::WhiteBed:
		case BlockType::YellowBed:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsHead(BlockType a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsSign(BlockType a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::AcaciaSign:
		case BlockType::AcaciaWallSign:
		case BlockType::BirchSign:
		case BlockType::BirchWallSign:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonWallSign:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakWallSign:
		case BlockType::JungleSign:
		case BlockType::JungleWallSign:
		case BlockType::OakSign:
		case BlockType::OakWallSign:
		case BlockType::SpruceSign:
		case BlockType::SpruceWallSign:
		case BlockType::WarpedSign:
		case BlockType::WarpedWallSign:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsAnvil(BlockType a_BlockType)
{
	switch (a_BlockType)
	{
		case BlockType::Anvil:
		case BlockType::ChippedAnvil:
		case BlockType::DamagedAnvil:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cBlockInfo:

NIBBLETYPE cBlockInfo::GetLightValue(const BLOCKTYPE Block)
{
	// Emissive blocks:
	switch (Block)
	{
		case BlockType::ACTIVE_COMPARATOR:    return 9;
		case BlockType::BEACON:               return 15;
		case BlockType::BREWING_STAND:        return 1;
		case BlockType::BROWN_MUSHROOM:       return 1;
		case BlockType::BURNING_FURNACE:      return 13;
		case BlockType::DRAGON_EGG:           return 1;
		case BlockType::END_PORTAL:           return 15;
		case BlockType::END_PORTAL_FRAME:     return 1;
		case BlockType::END_ROD:              return 14;
		case BlockType::ENDER_CHEST:          return 7;
		case BlockType::FIRE:                 return 15;
		case BlockType::GLOWSTONE:            return 15;
		case BlockType::JACK_O_LANTERN:       return 15;
		case BlockType::LAVA:                 return 15;
		case BlockType::MAGMA:                return 3;
		case BlockType::NETHER_PORTAL:        return 11;
		case BlockType::REDSTONE_LAMP_ON:     return 15;
		case BlockType::REDSTONE_ORE_GLOWING: return 9;
		case BlockType::REDSTONE_REPEATER_ON: return 9;
		case BlockType::REDSTONE_TORCH_ON:    return 7;
		case BlockType::SEA_LANTERN:          return 15;
		case BlockType::STATIONARY_LAVA:      return 15;
		case BlockType::TORCH:                return 14;
		default:                           return 0;
	}
}





NIBBLETYPE cBlockInfo::GetSpreadLightFalloff(const BLOCKTYPE Block)
{
	switch (Block)
	{
		// Spread blocks:
		case BlockType::ACACIA_DOOR:
		case BlockType::ACACIA_FENCE:
		case BlockType::ACACIA_FENCE_GATE:
		case BlockType::ACTIVATOR_RAIL:
		case BlockType::ACTIVE_COMPARATOR:
		case BlockType::AIR:
		case BlockType::ANVIL:
		case BlockType::BARRIER:
		case BlockType::BEACON:
		case BlockType::BED:
		case BlockType::BEETROOTS:
		case BlockType::BIG_FLOWER:
		case BlockType::BIRCH_DOOR:
		case BlockType::BIRCH_FENCE:
		case BlockType::BIRCH_FENCE_GATE:
		case BlockType::BROWN_MUSHROOM:
		case BlockType::BREWING_STAND:
		case BlockType::CACTUS:
		case BlockType::CAKE:
		case BlockType::CARPET:
		case BlockType::CARROTS:
		case BlockType::CAULDRON:
		case BlockType::CHORUS_FLOWER:
		case BlockType::CHORUS_PLANT:
		case BlockType::CHEST:
		case BlockType::COBBLESTONE_WALL:
		case BlockType::COCOA_POD:
		case BlockType::COBWEB:
		case BlockType::CROPS:
		case BlockType::DANDELION:
		case BlockType::DARK_OAK_DOOR:
		case BlockType::DARK_OAK_FENCE:
		case BlockType::DARK_OAK_FENCE_GATE:
		case BlockType::DAYLIGHT_SENSOR:
		case BlockType::DEAD_BUSH:
		case BlockType::DETECTOR_RAIL:
		case BlockType::DRAGON_EGG:
		case BlockType::ENCHANTMENT_TABLE:
		case BlockType::ENDER_CHEST:
		case BlockType::END_PORTAL:
		case BlockType::END_PORTAL_FRAME:
		case BlockType::END_ROD:
		case BlockType::FENCE:
		case BlockType::OAK_FENCE_GATE:
		case BlockType::FIRE:
		case BlockType::FLOWER:
		case BlockType::FLOWER_POT:
		case BlockType::GLASS:
		case BlockType::GLASS_PANE:
		case BlockType::HEAD:
		case BlockType::HEAVY_WEIGHTED_PRESSURE_PLATE:
		case BlockType::HOPPER:
		case BlockType::INACTIVE_COMPARATOR:
		case BlockType::INVERTED_DAYLIGHT_SENSOR:
		case BlockType::IRON_BARS:
		case BlockType::IRON_DOOR:
		case BlockType::IRON_TRAPDOOR:
		case BlockType::JUNGLE_DOOR:
		case BlockType::JUNGLE_FENCE:
		case BlockType::JUNGLE_FENCE_GATE:
		case BlockType::LADDER:
		case BlockType::LEAVES:
		case BlockType::LEVER:
		case BlockType::LILY_PAD:
		case BlockType::LIGHT_WEIGHTED_PRESSURE_PLATE:
		case BlockType::MELON_STEM:
		case BlockType::MOB_SPAWNER:
		case BlockType::NETHER_PORTAL:
		case BlockType::NETHER_WART:
		case BlockType::NEW_LEAVES:
		case BlockType::PISTON:
		case BlockType::PISTON_EXTENSION:
		case BlockType::PISTON_MOVED_BLOCK:
		case BlockType::POTATOES:
		case BlockType::POWERED_RAIL:
		case BlockType::PUMPKIN_STEM:
		case BlockType::RAIL:
		case BlockType::RED_MUSHROOM:
		case BlockType::REDSTONE_REPEATER_OFF:
		case BlockType::REDSTONE_REPEATER_ON:
		case BlockType::REDSTONE_TORCH_OFF:
		case BlockType::REDSTONE_TORCH_ON:
		case BlockType::REDSTONE_WIRE:
		case BlockType::SAPLING:
		case BlockType::SIGN_POST:
		case BlockType::SNOW:
		case BlockType::SPRUCE_DOOR:
		case BlockType::SPRUCE_FENCE:
		case BlockType::SPRUCE_FENCE_GATE:
		case BlockType::STAINED_GLASS:
		case BlockType::STAINED_GLASS_PANE:
		case BlockType::STANDING_BANNER:
		case BlockType::STICKY_PISTON:
		case BlockType::STONE_BUTTON:
		case BlockType::STONE_PRESSURE_PLATE:
		case BlockType::SUGARCANE:
		case BlockType::TALL_GRASS:
		case BlockType::TORCH:
		case BlockType::TRAPDOOR:
		case BlockType::TRAPPED_CHEST:
		case BlockType::TRIPWIRE:
		case BlockType::TRIPWIRE_HOOK:
		case BlockType::VINES:
		case BlockType::WALL_BANNER:
		case BlockType::WALLSIGN:
		case BlockType::WOODEN_BUTTON:
		case BlockType::OAK_DOOR:
		case BlockType::WOODEN_PRESSURE_PLATE: return 1;

		// Light in ice and water disappears faster:
		case BlockType::ICE:
		case BlockType::STATIONARY_WATER:
		case BlockType::WATER: return 3;

		// Light does not pass through anything else (note: including tilled farmland, stairs, and slabs):
		default: return 15;
	}
}





bool cBlockInfo::CanBeTerraformed(const BLOCKTYPE Block)
{
	// Blocks that can be terraformed:
	switch (Block)
	{
		case BlockType::COAL_ORE:
		case BlockType::COBBLESTONE:
		case BlockType::DIAMOND_ORE:
		case BlockType::DIRT:
		case BlockType::GOLD_ORE:
		case BlockType::GRASS:
		case BlockType::GRAVEL:
		case BlockType::HARDENED_CLAY:
		case BlockType::IRON_ORE:
		case BlockType::MYCELIUM:
		case BlockType::NETHERRACK:
		case BlockType::REDSTONE_ORE:
		case BlockType::REDSTONE_ORE_GLOWING:
		case BlockType::SAND:
		case BlockType::SANDSTONE:
		case BlockType::SOULSAND:
		case BlockType::STAINED_CLAY:
		case BlockType::STONE: return true;
		default: return false;
	}
}





bool cBlockInfo::FullyOccupiesVoxel(const BlockState Block)
{
	return cBlockHandler::For(Block.Type()).FullyOccupiesVoxel(Block);

	// Blocks that fully occupy their voxel - used as a guide for torch placeable blocks, amongst other things:
	switch (Block.Type())
	{
		case BlockType::Barrier:
		case BlockType::Bedrock:
		case BlockType::CoalBlock:
		case BlockType::RedstoneBlock:
		case BlockType::BoneBlock:
		case BlockType::Bookshelf:
		case BlockType::Bricks:
		case BlockType::ChainCommandBlock:
		case BlockType::Clay:
		case BlockType::CoalOre:
		case BlockType::Cobblestone:
		case BlockType::CommandBlock:
		case BlockType::BlackConcrete:
		case BlockType::BlueConcrete:
		case BlockType::BrownConcrete:
		case BlockType::CyanConcrete:
		case BlockType::GrayConcrete:
		case BlockType::GreenConcrete:
		case BlockType::LightBlueConcrete:
		case BlockType::LightGrayConcrete:
		case BlockType::LimeConcrete:
		case BlockType::MagentaConcrete:
		case BlockType::OrangeConcrete:
		case BlockType::PinkConcrete:
		case BlockType::PurpleConcrete:
		case BlockType::RedConcrete:
		case BlockType::WhiteConcrete:
		case BlockType::YellowConcrete:
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
		case BlockType::CraftingTable:
		case BlockType::DiamondBlock:
		case BlockType::DiamondOre:

		case BlockType::Dirt:
		case BlockType::GrassBlock:
		case BlockType::CoarseDirt:

		case BlockType::Dispenser:

		case BlockType::DOUBLEREDSANDSTONESLAB:
		case BlockType::DOUBLESTONESLAB:
		case BlockType::DOUBLEWOODENSLAB:

		case BlockType::Dropper:
		case BlockType::EmeraldBlock:
		case BlockType::EmeraldOre:
		case BlockType::EndStoneBricks:
		case BlockType::EndStone:
		case BlockType::FrostedIce:
		case BlockType::Furnace:
		case BlockType::Glowstone:
		case BlockType::GoldBlock:
		case BlockType::GoldOre:
		case BlockType::WhiteGlazedTerracotta:
		case BlockType::OrangeGlazedTerracotta:
		case BlockType::MagentaGlazedTerracotta:
		case BlockType::LightBlueGlazedTerracotta:
		case BlockType::YellowGlazedTerracotta:
		case BlockType::LimeGlazedTerracotta:
		case BlockType::PinkGlazedTerracotta:
		case BlockType::GrayGlazedTerracotta:
		case BlockType::LightGrayGlazedTerracotta:
		case BlockType::CyanGlazedTerracotta:
		case BlockType::PurpleGlazedTerracotta:
		case BlockType::BlueGlazedTerracotta:
		case BlockType::BrownGlazedTerracotta:
		case BlockType::GreenGlazedTerracotta:
		case BlockType::BlackGlazedTerracotta:
		case BlockType::RedGlazedTerracotta:

		case BlockType::Gravel:
		case BlockType::HARDENEDCLAY:
		case BlockType::HayBale:
		case BlockType::HUGEBROWNMUSHROOM:
		case BlockType::HUGEREDMUSHROOM:
		case BlockType::Ice:
		case BlockType::IRONBLOCK:
		case BlockType::IRONORE:
		case BlockType::JACKOLANTERN:
		case BlockType::JUKEBOX:
		case BlockType::LAPISBLOCK:
		case BlockType::LAPISORE:
		case BlockType::LOG:
		case BlockType::MAGMA:
		case BlockType::MELON:
		case BlockType::MOBSPAWNER:
		case BlockType::MOSSYCOBBLESTONE:
		case BlockType::MYCELIUM:
		case BlockType::NETHERRACK:
		case BlockType::NETHERBRICK:
		case BlockType::NETHERQUARTZORE:
		case BlockType::NETHERWARTBLOCK:
		case BlockType::NEWLOG:
		case BlockType::NOTEBLOCK:
		case BlockType::OBSERVER:
		case BlockType::OBSIDIAN:
		case BlockType::PACKEDICE:
		case BlockType::PLANKS:
		case BlockType::PRISMARINEBLOCK:
		case BlockType::PUMPKIN:
		case BlockType::PURPURBLOCK:
		case BlockType::PURPURPILLAR:
		case BlockType::PURPURDOUBLESLAB:
		case BlockType::QUARTZBLOCK:
		case BlockType::REDNETHERBRICK:
		case BlockType::REDSANDSTONE:
		case BlockType::REDSTONELAMPOFF:
		case BlockType::REDSTONELAMPON:
		case BlockType::REDSTONEORE:
		case BlockType::REDSTONEOREGLOWING:
		case BlockType::REPEATINGCOMMANDBLOCK:
		case BlockType::SANDSTONE:
		case BlockType::SAND:
		case BlockType::SILVERFISHEGG:
		case BlockType::SPONGE:
		case BlockType::STAINEDCLAY:
		case BlockType::STONE:
		case BlockType::STONEBRICKS:
		case BlockType::STRUCTUREBLOCK:
		case BlockType::WOOL: return true;
		default: return false;
	}
}





bool cBlockInfo::IsOneHitDig(const BLOCKTYPE Block)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif

	// GetHardness returns exactly 0 for one hit break blocks:
	return GetHardness(Block) == 0;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}





bool cBlockInfo::IsPistonBreakable(const BLOCKTYPE Block)
{
	// Blocks that break when pushed by piston:
	switch (Block)
	{
		case BlockType::ACACIA_DOOR:
		case BlockType::ACTIVE_COMPARATOR:
		case BlockType::AIR:
		case BlockType::BED:
		case BlockType::BEETROOTS:
		case BlockType::BIG_FLOWER:
		case BlockType::BIRCH_DOOR:
		case BlockType::BLACK_SHULKER_BOX:
		case BlockType::BLUE_SHULKER_BOX:
		case BlockType::BROWN_MUSHROOM:
		case BlockType::BROWN_SHULKER_BOX:
		case BlockType::CACTUS:
		case BlockType::CAKE:
		case BlockType::CARROTS:
		case BlockType::CHORUS_FLOWER:
		case BlockType::CHORUS_PLANT:
		case BlockType::COCOA_POD:
		case BlockType::COBWEB:
		case BlockType::CROPS:
		case BlockType::CYAN_SHULKER_BOX:
		case BlockType::DANDELION:
		case BlockType::DARK_OAK_DOOR:
		case BlockType::DEAD_BUSH:
		case BlockType::DRAGON_EGG:
		case BlockType::FIRE:
		case BlockType::FLOWER:
		case BlockType::FLOWER_POT:
		case BlockType::GRAY_SHULKER_BOX:
		case BlockType::GREEN_SHULKER_BOX:
		case BlockType::HEAD:
		case BlockType::HEAVY_WEIGHTED_PRESSURE_PLATE:
		case BlockType::INACTIVE_COMPARATOR:
		case BlockType::IRON_DOOR:
		case BlockType::IRON_TRAPDOOR:
		case BlockType::JACK_O_LANTERN:
		case BlockType::JUNGLE_DOOR:
		case BlockType::LIGHT_BLUE_SHULKER_BOX:
		case BlockType::LIGHT_GRAY_SHULKER_BOX:
		case BlockType::LIGHT_WEIGHTED_PRESSURE_PLATE:
		case BlockType::LILY_PAD:
		case BlockType::LIME_SHULKER_BOX:
		case BlockType::LADDER:
		case BlockType::LAVA:
		case BlockType::LEAVES:
		case BlockType::LEVER:
		case BlockType::MAGENTA_SHULKER_BOX:
		case BlockType::MELON:
		case BlockType::MELON_STEM:
		case BlockType::NETHER_WART:
		case BlockType::OAK_DOOR:
		case BlockType::ORANGE_SHULKER_BOX:
		case BlockType::PINK_SHULKER_BOX:
		case BlockType::POTATOES:
		case BlockType::PUMPKIN:
		case BlockType::PUMPKIN_STEM:
		case BlockType::PURPLE_SHULKER_BOX:
		case BlockType::REDSTONE_REPEATER_OFF:
		case BlockType::REDSTONE_REPEATER_ON:
		case BlockType::REDSTONE_TORCH_OFF:
		case BlockType::REDSTONE_TORCH_ON:
		case BlockType::REDSTONE_WIRE:
		case BlockType::RED_MUSHROOM:
		case BlockType::RED_SHULKER_BOX:
		case BlockType::REEDS:
		case BlockType::SAPLING:
		case BlockType::SIGN_POST:
		case BlockType::SNOW:
		case BlockType::SPRUCE_DOOR:
		case BlockType::STATIONARY_LAVA:
		case BlockType::STATIONARY_WATER:
		case BlockType::STONE_BUTTON:
		case BlockType::STONE_PRESSURE_PLATE:
		case BlockType::TALL_GRASS:
		case BlockType::TORCH:
		case BlockType::TRAPDOOR:
		case BlockType::TRIPWIRE:
		case BlockType::TRIPWIRE_HOOK:
		case BlockType::VINES:
		case BlockType::WALLSIGN:
		case BlockType::WATER:
		case BlockType::WHITE_SHULKER_BOX:
		case BlockType::WOODEN_BUTTON:
		case BlockType::WOODEN_PRESSURE_PLATE:
		case BlockType::YELLOW_SHULKER_BOX: return true;
		default: return false;
	}
}





bool cBlockInfo::IsRainBlocker(const BLOCKTYPE Block)
{
	// Blocks that block rain or snow's passage:
	switch (Block)
	{
		case BlockType::SIGN_POST:
		case BlockType::WALLSIGN:
		case BlockType::WALL_BANNER:
		case BlockType::STANDING_BANNER: return true;
		default: return IsSolid(Block);
	}
}





bool cBlockInfo::IsSkylightDispersant(const BLOCKTYPE Block)
{
	// Skylight dispersant blocks:
	switch (Block)
	{
		case BlockType::COBWEB:
		case BlockType::LEAVES: return true;
		default: return GetSpreadLightFalloff(Block) > 1;
	}
}





bool cBlockInfo::IsSnowable(BLOCKTYPE a_BlockType)
{
	return (
		(a_BlockType == BlockType::ICE) ||
		(a_BlockType == BlockType::LEAVES) ||
		(!IsTransparent(a_BlockType) && (a_BlockType != BlockType::PACKED_ICE))
	);
}





bool cBlockInfo::IsSolid(const BLOCKTYPE Block)
{
	// Nonsolid blocks:
	switch (Block)
	{
		case BlockType::ACTIVATOR_RAIL:
		case BlockType::ACTIVE_COMPARATOR:
		case BlockType::AIR:
		case BlockType::BIG_FLOWER:
		case BlockType::BROWN_MUSHROOM:
		case BlockType::CARPET:
		case BlockType::CARROTS:
		case BlockType::CHORUS_FLOWER:
		case BlockType::CHORUS_PLANT:
		case BlockType::COBWEB:
		case BlockType::CROPS:
		case BlockType::DANDELION:
		case BlockType::DEAD_BUSH:
		case BlockType::DETECTOR_RAIL:
		case BlockType::END_GATEWAY:
		case BlockType::END_PORTAL:
		case BlockType::END_ROD:
		case BlockType::FIRE:
		case BlockType::FLOWER:
		case BlockType::HEAVY_WEIGHTED_PRESSURE_PLATE:
		case BlockType::INACTIVE_COMPARATOR:
		case BlockType::LADDER:
		case BlockType::LAVA:
		case BlockType::LEVER:
		case BlockType::LIGHT_WEIGHTED_PRESSURE_PLATE:
		case BlockType::MELON_STEM:
		case BlockType::NETHER_PORTAL:
		case BlockType::POTATOES:
		case BlockType::POWERED_RAIL:
		case BlockType::RAIL:
		case BlockType::RED_MUSHROOM:
		case BlockType::REDSTONE_REPEATER_OFF:
		case BlockType::REDSTONE_REPEATER_ON:
		case BlockType::REDSTONE_TORCH_OFF:
		case BlockType::REDSTONE_TORCH_ON:
		case BlockType::REDSTONE_WIRE:
		case BlockType::REEDS:
		case BlockType::SAPLING:
		case BlockType::SIGN_POST:
		case BlockType::SNOW:
		case BlockType::STANDING_BANNER:
		case BlockType::STATIONARY_LAVA:
		case BlockType::STATIONARY_WATER:
		case BlockType::STONE_BUTTON:
		case BlockType::STONE_PRESSURE_PLATE:
		case BlockType::TALL_GRASS:
		case BlockType::TORCH:
		case BlockType::TRIPWIRE:
		case BlockType::TRIPWIRE_HOOK:
		case BlockType::VINES:
		case BlockType::WALL_BANNER:
		case BlockType::WALLSIGN:
		case BlockType::WATER:
		case BlockType::WOODEN_BUTTON:
		case BlockType::WOODEN_PRESSURE_PLATE: return false;
		default: return true;
	}
}





bool cBlockInfo::IsTransparent(const BlockState a_Block)
{
	// Transparent blocks:
	switch (a_Block.Type())
	{
		case BlockType::ACACIA_DOOR:
		case BlockType::ACACIA_FENCE:
		case BlockType::ACACIA_FENCE_GATE:
		case BlockType::ACACIA_WOOD_STAIRS:
		case BlockType::ACTIVATOR_RAIL:
		case BlockType::ACTIVE_COMPARATOR:
		case BlockType::AIR:
		case BlockType::ANVIL:
		case BlockType::BARRIER:
		case BlockType::BEACON:
		case BlockType::BED:
		case BlockType::BIG_FLOWER:
		case BlockType::BIRCH_DOOR:
		case BlockType::BIRCH_FENCE:
		case BlockType::BIRCH_FENCE_GATE:
		case BlockType::BIRCH_WOOD_STAIRS:
		case BlockType::BLACK_SHULKER_BOX:
		case BlockType::BLOCK_OF_REDSTONE:
		case BlockType::BLUE_SHULKER_BOX:
		case BlockType::BREWING_STAND:
		case BlockType::BRICK_STAIRS:
		case BlockType::BROWN_MUSHROOM:
		case BlockType::BROWN_SHULKER_BOX:
		case BlockType::CACTUS:
		case BlockType::CAKE:
		case BlockType::CARPET:
		case BlockType::CARROTS:
		case BlockType::CAULDRON:
		case BlockType::CHEST:
		case BlockType::COBBLESTONE_STAIRS:
		case BlockType::COBBLESTONE_WALL:
		case BlockType::COBWEB:
		case BlockType::COCOA_POD:
		case BlockType::CROPS:
		case BlockType::CYAN_SHULKER_BOX:
		case BlockType::DANDELION:
		case BlockType::DARK_OAK_DOOR:
		case BlockType::DARK_OAK_FENCE:
		case BlockType::DARK_OAK_FENCE_GATE:
		case BlockType::DARK_OAK_WOOD_STAIRS:
		case BlockType::DAYLIGHT_SENSOR:
		case BlockType::DEAD_BUSH:
		case BlockType::DETECTOR_RAIL:
		case BlockType::DOUBLE_RED_SANDSTONE_SLAB:
		case BlockType::DOUBLE_STONE_SLAB:
		case BlockType::DOUBLE_WOODEN_SLAB:
		case BlockType::DRAGON_EGG:
		case BlockType::ENCHANTMENT_TABLE:
		case BlockType::END_PORTAL:
		case BlockType::END_PORTAL_FRAME:
		case BlockType::END_ROD:
		case BlockType::ENDER_CHEST:
		case BlockType::FARMLAND:
		case BlockType::FENCE:
		case BlockType::FIRE:
		case BlockType::FLOWER:
		case BlockType::FLOWER_POT:
		case BlockType::GLASS:
		case BlockType::GLASS_PANE:
		case BlockType::GLOWSTONE:
		case BlockType::GRAY_SHULKER_BOX:
		case BlockType::GREEN_SHULKER_BOX:
		case BlockType::HEAD:
		case BlockType::HEAVY_WEIGHTED_PRESSURE_PLATE:
		case BlockType::HOPPER:
		case BlockType::ICE:
		case BlockType::INACTIVE_COMPARATOR:
		case BlockType::INVERTED_DAYLIGHT_SENSOR:
		case BlockType::IRON_BARS:
		case BlockType::IRON_DOOR:
		case BlockType::IRON_TRAPDOOR:
		case BlockType::JACK_O_LANTERN:
		case BlockType::JUNGLE_DOOR:
		case BlockType::JUNGLE_FENCE:
		case BlockType::JUNGLE_FENCE_GATE:
		case BlockType::JUNGLE_WOOD_STAIRS:
		case BlockType::LADDER:
		case BlockType::LAVA:
		case BlockType::LEAVES:
		case BlockType::LEVER:
		case BlockType::LIGHT_BLUE_SHULKER_BOX:
		case BlockType::LIGHT_GRAY_SHULKER_BOX:
		case BlockType::LIGHT_WEIGHTED_PRESSURE_PLATE:
		case BlockType::LILY_PAD:
		case BlockType::LIME_SHULKER_BOX:
		case BlockType::LIT_FURNACE:
		case BlockType::MAGENTA_SHULKER_BOX:
		case BlockType::MELON_STEM:
		case BlockType::MOB_SPAWNER:
		case BlockType::NETHER_BRICK_FENCE:
		case BlockType::NETHER_BRICK_STAIRS:
		case BlockType::NETHER_PORTAL:
		case BlockType::NETHER_WART:
		case BlockType::NEW_LEAVES:
		case BlockType::OAK_DOOR:
		case BlockType::OAK_FENCE_GATE:
		case BlockType::OAK_WOOD_STAIRS:
		case BlockType::ORANGE_SHULKER_BOX:
		case BlockType::PINK_SHULKER_BOX:
		case BlockType::PISTON:
		case BlockType::PISTON_EXTENSION:
		case BlockType::PISTON_MOVED_BLOCK:
		case BlockType::POTATOES:
		case BlockType::POWERED_RAIL:
		case BlockType::PUMPKIN_STEM:
		case BlockType::PURPLE_SHULKER_BOX:
		case BlockType::PURPUR_DOUBLE_SLAB:
		case BlockType::PURPUR_SLAB:
		case BlockType::PURPUR_STAIRS:
		case BlockType::QUARTZ_STAIRS:
		case BlockType::RAIL:
		case BlockType::RED_MUSHROOM:
		case BlockType::RED_SANDSTONE_SLAB:
		case BlockType::RED_SANDSTONE_STAIRS:
		case BlockType::RED_SHULKER_BOX:
		case BlockType::REDSTONE_ORE_GLOWING:
		case BlockType::REDSTONE_REPEATER_OFF:
		case BlockType::REDSTONE_REPEATER_ON:
		case BlockType::REDSTONE_TORCH_OFF:
		case BlockType::REDSTONE_TORCH_ON:
		case BlockType::REDSTONE_WIRE:
		case BlockType::SANDSTONE_STAIRS:
		case BlockType::SAPLING:
		case BlockType::SEA_LANTERN:
		case BlockType::SIGN_POST:
		case BlockType::SNOW:
		case BlockType::SPRUCE_DOOR:
		case BlockType::SPRUCE_FENCE:
		case BlockType::SPRUCE_FENCE_GATE:
		case BlockType::SPRUCE_WOOD_STAIRS:
		case BlockType::STAINED_GLASS:
		case BlockType::STAINED_GLASS_PANE:
		case BlockType::STANDING_BANNER:
		case BlockType::STATIONARY_LAVA:
		case BlockType::STATIONARY_WATER:
		case BlockType::STICKY_PISTON:
		case BlockType::STONE_BRICK_STAIRS:
		case BlockType::STONE_BUTTON:
		case BlockType::STONE_PRESSURE_PLATE:
		case BlockType::STONE_SLAB:
		case BlockType::SUGARCANE:
		case BlockType::TALL_GRASS:
		case BlockType::TORCH:
		case BlockType::TRAPDOOR:
		case BlockType::TRAPPED_CHEST:
		case BlockType::TRIPWIRE:
		case BlockType::TRIPWIRE_HOOK:
		case BlockType::VINES:
		case BlockType::WALL_BANNER:
		case BlockType::WALLSIGN:
		case BlockType::WATER:
		case BlockType::WHITE_SHULKER_BOX:
		case BlockType::WOODEN_BUTTON:
		case BlockType::WOODEN_PRESSURE_PLATE:
		case BlockType::WOODEN_SLAB:
		case BlockType::YELLOW_SHULKER_BOX: return true;
		default: return false;
	}
}





bool cBlockInfo::IsUseableBySpectator(const BLOCKTYPE Block)
{
	// Blocks, which a spectator is allowed to interact with:
	switch (Block)
	{
		case BlockType::BEACON:
		case BlockType::BREWING_STAND:
		case BlockType::CHEST:
		case BlockType::DISPENSER:
		case BlockType::DROPPER:
		case BlockType::HOPPER: return true;
		default: return false;
	}
}





float cBlockInfo::GetBlockHeight(const BlockState a_Block)
{
	if (cBlockSlabHandler::IsAnySlabType(a_Block))
	{
		if (cBlockSlabHandler::IsSlabFull(a_Block))
		{
			return 1.0;
		}
		if (cBlockSlabHandler::IsSlabTop(a_Block))
		{
			return 1.0f;
		}
		return 0.5f;
	}

	// Block heights:
	switch (a_Block.Type())
	{

			// Fences
		case BlockType::AcaciaFence:
		case BlockType::BirchFence:
		case BlockType::DarkOakFence:
		case BlockType::JungleFence:
		case BlockType::NetherBrickFence:
		case BlockType::OakFence:
		case BlockType::SpruceFence:
		case BlockType::WarpedFence:
			// Fence Gates
		case BlockType::AcaciaFenceGate:
		case BlockType::BirchFenceGate:
		case BlockType::CrimsonFenceGate:
		case BlockType::DarkOakFenceGate:
		case BlockType::JungleFenceGate:
		case BlockType::OakFenceGate:
		case BlockType::SpruceFenceGate:
		case BlockType::WarpedFenceGate:
			return 1.5;
			// Beds
		case BlockType::BlackBed:
		case BlockType::BlueBed:
		case BlockType::BrownBed:
		case BlockType::CyanBed:
		case BlockType::GrayBed:
		case BlockType::GreenBed:
		case BlockType::LightBlueBed:
		case BlockType::LightGrayBed:
		case BlockType::LimeBed:
		case BlockType::MagentaBed:
		case BlockType::OrangeBed:
		case BlockType::PinkBed:
		case BlockType::PurpleBed:
		case BlockType::RedBed:
		case BlockType::WhiteBed:
		case BlockType::YellowBed:
			return 0.5625;  // 9 pixels
		case BlockType::Cake:            return 0.5;     // 8 pixels
		case BlockType::EnchantingTable: return 0.75;    // 12 pixels
		case BlockType::Snow:            return 0.125;   // one layer is 1 / 8 (2 pixels) tall
		default:                         return 1;
	}
}





float cBlockInfo::GetHardness(const BLOCKTYPE Block)
{
	// Block hardness:
	switch (Block)
	{
		case BlockType::ACACIA_DOOR:                   return 3.0f;
		case BlockType::ACACIA_FENCE:                  return 2.0f;
		case BlockType::ACACIA_FENCE_GATE:             return 2.0f;
		case BlockType::ACACIA_WOOD_STAIRS:            return 2.0f;
		case BlockType::ACTIVATOR_RAIL:                return 0.7f;
		case BlockType::ACTIVE_COMPARATOR:             return 0.0f;
		case BlockType::AIR:                           return 0.0f;
		case BlockType::ANVIL:                         return 5.0f;
		case BlockType::BARRIER:                       return INFINITY;
		case BlockType::BEACON:                        return 3.0f;
		case BlockType::BED:                           return 0.2f;
		case BlockType::BEDROCK:                       return INFINITY;
		case BlockType::BEETROOTS:                     return 0.0f;
		case BlockType::BIG_FLOWER:                    return 0.0f;
		case BlockType::BIRCH_DOOR:                    return 3.0f;
		case BlockType::BIRCH_FENCE:                   return 2.0f;
		case BlockType::BIRCH_FENCE_GATE:              return 2.0f;
		case BlockType::BIRCH_WOOD_STAIRS:             return 2.0f;
		case BlockType::BLACK_GLAZED_TERRACOTTA:       return 1.4f;
		case BlockType::BLACK_SHULKER_BOX:             return 2.0f;
		case BlockType::BLOCK_OF_COAL:                 return 5.0f;
		case BlockType::BLOCK_OF_REDSTONE:             return 5.0f;
		case BlockType::BLUE_GLAZED_TERRACOTTA:        return 1.4f;
		case BlockType::BLUE_SHULKER_BOX:              return 2.0f;
		case BlockType::BONE_BLOCK:                    return 2.0f;
		case BlockType::BOOKCASE:                      return 1.5f;
		case BlockType::BREWING_STAND:                 return 0.5f;
		case BlockType::BRICK:                         return 2.0f;
		case BlockType::BRICK_STAIRS:                  return 2.0f;
		case BlockType::BROWN_GLAZED_TERRACOTTA:       return 1.4f;
		case BlockType::BROWN_MUSHROOM:                return 0.0f;
		case BlockType::BROWN_SHULKER_BOX:             return 2.0f;
		case BlockType::CACTUS:                        return 0.4f;
		case BlockType::CAKE:                          return 0.5f;
		case BlockType::CARPET:                        return 0.1f;
		case BlockType::CARROTS:                       return 0.0f;
		case BlockType::CAULDRON:                      return 2.0f;
		case BlockType::CHAIN_COMMAND_BLOCK:           return INFINITY;
		case BlockType::CHEST:                         return 2.5f;
		case BlockType::CHORUS_FLOWER:                 return 0.4f;
		case BlockType::CHORUS_PLANT:                  return 0.4f;
		case BlockType::CLAY:                          return 0.6f;
		case BlockType::COAL_ORE:                      return 3.0f;
		case BlockType::COBBLESTONE:                   return 2.0f;
		case BlockType::COBBLESTONE_STAIRS:            return 2.0f;
		case BlockType::COBBLESTONE_WALL:              return 2.0f;
		case BlockType::COBWEB:                        return 4.0f;
		case BlockType::COCOA_POD:                     return 0.2f;
		case BlockType::COMMAND_BLOCK:                 return INFINITY;
		case BlockType::CONCRETE:                      return 1.8f;
		case BlockType::CONCRETE_POWDER:               return 0.5f;
		case BlockType::CRAFTING_TABLE:                return 2.5f;
		case BlockType::CROPS:                         return 0.0f;
		case BlockType::CYAN_GLAZED_TERRACOTTA:        return 1.4f;
		case BlockType::CYAN_SHULKER_BOX:              return 2.0f;
		case BlockType::DANDELION:                     return 0.0f;
		case BlockType::DARK_OAK_DOOR:                 return 3.0f;
		case BlockType::DARK_OAK_FENCE:                return 2.0f;
		case BlockType::DARK_OAK_FENCE_GATE:           return 2.0f;
		case BlockType::DARK_OAK_WOOD_STAIRS:          return 2.0f;
		case BlockType::DAYLIGHT_SENSOR:               return 0.2f;
		case BlockType::DEAD_BUSH:                     return 0.0f;
		case BlockType::DETECTOR_RAIL:                 return 0.7f;
		case BlockType::DIAMOND_BLOCK:                 return 5.0f;
		case BlockType::DIAMOND_ORE:                   return 3.0f;
		case BlockType::DIRT:                          return 0.5f;
		case BlockType::DISPENSER:                     return 3.5f;
		case BlockType::DOUBLE_RED_SANDSTONE_SLAB:     return 2.0f;
		case BlockType::DOUBLE_STONE_SLAB:             return 2.0f;
		case BlockType::DOUBLE_WOODEN_SLAB:            return 2.0f;
		case BlockType::DRAGON_EGG:                    return 3.0f;
		case BlockType::DROPPER:                       return 3.5f;
		case BlockType::EMERALD_BLOCK:                 return 5.0f;
		case BlockType::EMERALD_ORE:                   return 3.0f;
		case BlockType::ENCHANTMENT_TABLE:             return 5.0f;
		case BlockType::ENDER_CHEST:                   return 22.5f;
		case BlockType::END_BRICKS:                    return 0.8f;
		case BlockType::END_GATEWAY:                   return INFINITY;
		case BlockType::END_PORTAL:                    return INFINITY;
		case BlockType::END_PORTAL_FRAME:              return INFINITY;
		case BlockType::END_ROD:                       return 0.0f;
		case BlockType::END_STONE:                     return 3.0f;
		case BlockType::FARMLAND:                      return 0.6f;
		case BlockType::FENCE:                         return 2.0f;
		case BlockType::FIRE:                          return 0.0f;
		case BlockType::FLOWER:                        return 0.0f;
		case BlockType::FLOWER_POT:                    return 0.0f;
		case BlockType::FROSTED_ICE:                   return 0.5f;
		case BlockType::FURNACE:                       return 3.5f;
		case BlockType::GLASS:                         return 0.3f;
		case BlockType::GLASS_PANE:                    return 0.3f;
		case BlockType::GLOWSTONE:                     return 0.3f;
		case BlockType::GOLD_BLOCK:                    return 3.0f;
		case BlockType::GOLD_ORE:                      return 3.0f;
		case BlockType::GRASS:                         return 0.6f;
		case BlockType::GRASS_PATH:                    return 0.6f;
		case BlockType::GRAVEL:                        return 0.6f;
		case BlockType::GRAY_GLAZED_TERRACOTTA:        return 1.4f;
		case BlockType::GRAY_SHULKER_BOX:              return 2.0f;
		case BlockType::GREEN_GLAZED_TERRACOTTA:       return 1.4f;
		case BlockType::GREEN_SHULKER_BOX:             return 2.0f;
		case BlockType::HARDENED_CLAY:                 return 1.25f;
		case BlockType::HAY_BALE:                      return 0.5f;
		case BlockType::HEAD:                          return 1.0f;
		case BlockType::HEAVY_WEIGHTED_PRESSURE_PLATE: return 0.5f;
		case BlockType::HOPPER:                        return 3.0f;
		case BlockType::HUGE_BROWN_MUSHROOM:           return 0.2f;
		case BlockType::HUGE_RED_MUSHROOM:             return 0.2f;
		case BlockType::ICE:                           return 0.5f;
		case BlockType::INACTIVE_COMPARATOR:           return 0.0f;
		case BlockType::INVERTED_DAYLIGHT_SENSOR:      return 0.2f;
		case BlockType::IRON_BARS:                     return 5.0f;
		case BlockType::IRON_BLOCK:                    return 5.0f;
		case BlockType::IRON_DOOR:                     return 5.0f;
		case BlockType::IRON_ORE:                      return 3.0f;
		case BlockType::IRON_TRAPDOOR:                 return 5.0f;
		case BlockType::JACK_O_LANTERN:                return 1.0f;
		case BlockType::JUKEBOX:                       return 2.0f;
		case BlockType::JUNGLE_DOOR:                   return 3.0f;
		case BlockType::JUNGLE_FENCE:                  return 2.0f;
		case BlockType::JUNGLE_FENCE_GATE:             return 2.0f;
		case BlockType::JUNGLE_WOOD_STAIRS:            return 2.0f;
		case BlockType::LADDER:                        return 0.4f;
		case BlockType::LAPIS_BLOCK:                   return 3.0f;
		case BlockType::LAPIS_ORE:                     return 3.0f;
		case BlockType::LAVA:                          return 100.0f;
		case BlockType::LEAVES:                        return 0.2f;
		case BlockType::LEVER:                         return 0.5f;
		case BlockType::LIGHT_BLUE_GLAZED_TERRACOTTA:  return 1.4f;
		case BlockType::LIGHT_BLUE_SHULKER_BOX:        return 2.0f;
		case BlockType::LIGHT_GRAY_GLAZED_TERRACOTTA:  return 1.4f;
		case BlockType::LIGHT_GRAY_SHULKER_BOX:        return 2.0f;
		case BlockType::LIGHT_WEIGHTED_PRESSURE_PLATE: return 0.5f;
		case BlockType::LILY_PAD:                      return 0.0f;
		case BlockType::LIME_GLAZED_TERRACOTTA:        return 1.4f;
		case BlockType::LIME_SHULKER_BOX:              return 2.0f;
		case BlockType::LIT_FURNACE:                   return 3.5f;
		case BlockType::LOG:                           return 2.0f;
		case BlockType::MAGENTA_GLAZED_TERRACOTTA:     return 1.4f;
		case BlockType::MAGENTA_SHULKER_BOX:           return 2.0f;
		case BlockType::MAGMA:                         return 0.5f;
		case BlockType::MELON:                         return 1.0f;
		case BlockType::MELON_STEM:                    return 0.0f;
		case BlockType::MOB_SPAWNER:                   return 5.0f;
		case BlockType::MOSSY_COBBLESTONE:             return 2.0f;
		case BlockType::MYCELIUM:                      return 0.6f;
		case BlockType::NETHERRACK:                    return 0.4f;
		case BlockType::NETHER_BRICK:                  return 2.0f;
		case BlockType::NETHER_BRICK_FENCE:            return 2.0f;
		case BlockType::NETHER_BRICK_STAIRS:           return 2.0f;
		case BlockType::NETHER_PORTAL:                 return INFINITY;
		case BlockType::NETHER_QUARTZ_ORE:             return 3.0f;
		case BlockType::NETHER_WART:                   return 0.0f;
		case BlockType::NETHER_WART_BLOCK:             return 1.0f;
		case BlockType::NEW_LEAVES:                    return 0.2f;
		case BlockType::NEW_LOG:                       return 2.0f;
		case BlockType::NOTE_BLOCK:                    return 0.8f;
		case BlockType::OAK_DOOR:                      return 3.0f;
		case BlockType::OAK_FENCE_GATE:                return 2.0f;
		case BlockType::OAK_WOOD_STAIRS:               return 2.0f;
		case BlockType::OBSERVER:                      return 3.5f;
		case BlockType::OBSIDIAN:                      return 50.0f;
		case BlockType::ORANGE_GLAZED_TERRACOTTA:      return 1.4f;
		case BlockType::ORANGE_SHULKER_BOX:            return 2.0f;
		case BlockType::PACKED_ICE:                    return 0.5f;
		case BlockType::PINK_GLAZED_TERRACOTTA:        return 1.4f;
		case BlockType::PINK_SHULKER_BOX:              return 2.0f;
		case BlockType::PISTON:                        return 0.5f;
		case BlockType::PISTON_EXTENSION:              return 0.5f;
		case BlockType::PISTON_MOVED_BLOCK:            return INFINITY;
		case BlockType::PLANKS:                        return 2.0f;
		case BlockType::POTATOES:                      return 0.0f;
		case BlockType::POWERED_RAIL:                  return 0.7f;
		case BlockType::PRISMARINE_BLOCK:              return 1.5f;
		case BlockType::PUMPKIN:                       return 1.0f;
		case BlockType::PUMPKIN_STEM:                  return 0.0f;
		case BlockType::PURPLE_GLAZED_TERRACOTTA:      return 1.4f;
		case BlockType::PURPLE_SHULKER_BOX:            return 2.0f;
		case BlockType::PURPUR_BLOCK:                  return 1.5f;
		case BlockType::PURPUR_DOUBLE_SLAB:            return 2.0f;
		case BlockType::PURPUR_PILLAR:                 return 1.5f;
		case BlockType::PURPUR_SLAB:                   return 2.0f;
		case BlockType::PURPUR_STAIRS:                 return 1.5f;
		case BlockType::QUARTZ_BLOCK:                  return 0.8f;
		case BlockType::QUARTZ_STAIRS:                 return 0.8f;
		case BlockType::RAIL:                          return 0.7f;
		case BlockType::REDSTONE_LAMP_OFF:             return 0.3f;
		case BlockType::REDSTONE_LAMP_ON:              return 0.3f;
		case BlockType::REDSTONE_ORE:                  return 3.0f;
		case BlockType::REDSTONE_ORE_GLOWING:          return 0.625f;
		case BlockType::REDSTONE_REPEATER_OFF:         return 0.0f;
		case BlockType::REDSTONE_REPEATER_ON:          return 0.0f;
		case BlockType::REDSTONE_TORCH_OFF:            return 0.0f;
		case BlockType::REDSTONE_TORCH_ON:             return 0.0f;
		case BlockType::REDSTONE_WIRE:                 return 0.0f;
		case BlockType::RED_GLAZED_TERRACOTTA:         return 1.4f;
		case BlockType::RED_MUSHROOM:                  return 0.0f;
		case BlockType::RED_NETHER_BRICK:              return 2.0f;
		case BlockType::RED_SANDSTONE:                 return 0.8f;
		case BlockType::RED_SANDSTONE_SLAB:            return 2.0f;
		case BlockType::RED_SANDSTONE_STAIRS:          return 0.8f;
		case BlockType::RED_SHULKER_BOX:               return 2.0f;
		case BlockType::REPEATING_COMMAND_BLOCK:       return INFINITY;
		case BlockType::SAND:                          return 0.5f;
		case BlockType::SANDSTONE:                     return 0.8f;
		case BlockType::SANDSTONE_STAIRS:              return 0.8f;
		case BlockType::SAPLING:                       return 0.0f;
		case BlockType::SEA_LANTERN:                   return 0.3f;
		case BlockType::SIGN_POST:                     return 1.0f;
		case BlockType::SILVERFISH_EGG:                return 0.75f;
		case BlockType::SLIME_BLOCK:                   return 0.0f;
		case BlockType::SNOW:                          return 0.1f;
		case BlockType::SNOW_BLOCK:                    return 0.2f;
		case BlockType::SOULSAND:                      return 0.5f;
		case BlockType::SPONGE:                        return 0.6f;
		case BlockType::SPRUCE_DOOR:                   return 3.0f;
		case BlockType::SPRUCE_FENCE:                  return 2.0f;
		case BlockType::SPRUCE_FENCE_GATE:             return 2.0f;
		case BlockType::SPRUCE_WOOD_STAIRS:            return 2.0f;
		case BlockType::STAINED_CLAY:                  return 4.2f;
		case BlockType::STAINED_GLASS:                 return 0.3f;
		case BlockType::STAINED_GLASS_PANE:            return 0.3f;
		case BlockType::STANDING_BANNER:               return 1.0f;
		case BlockType::STATIONARY_LAVA:               return 100.0f;
		case BlockType::STATIONARY_WATER:              return 100.0f;
		case BlockType::STICKY_PISTON:                 return 0.5f;
		case BlockType::STONE:                         return 1.5f;
		case BlockType::STONE_BRICKS:                  return 1.5f;
		case BlockType::STONE_BRICK_STAIRS:            return 1.5f;
		case BlockType::STONE_BUTTON:                  return 0.5f;
		case BlockType::STONE_PRESSURE_PLATE:          return 0.5f;
		case BlockType::STONE_SLAB:                    return 2.0f;
		case BlockType::STRUCTURE_BLOCK:               return INFINITY;
		case BlockType::STRUCTURE_VOID:                return 0.0f;
		case BlockType::SUGARCANE:                     return 0.0f;
		case BlockType::TALL_GRASS:                    return 0.0f;
		case BlockType::TNT:                           return 0.0f;
		case BlockType::TORCH:                         return 0.0f;
		case BlockType::TRAPDOOR:                      return 3.0f;
		case BlockType::TRAPPED_CHEST:                 return 2.5f;
		case BlockType::TRIPWIRE:                      return 0.0f;
		case BlockType::TRIPWIRE_HOOK:                 return 0.0f;
		case BlockType::VINES:                         return 0.2f;
		case BlockType::WALLSIGN:                      return 1.0f;
		case BlockType::WALL_BANNER:                   return 1.0f;
		case BlockType::WATER:                         return 100.0f;
		case BlockType::WHITE_GLAZED_TERRACOTTA:       return 1.4f;
		case BlockType::WHITE_SHULKER_BOX:             return 2.0f;
		case BlockType::WOODEN_BUTTON:                 return 0.5f;
		case BlockType::WOODEN_PRESSURE_PLATE:         return 0.5f;
		case BlockType::WOODEN_SLAB:                   return 2.0f;
		case BlockType::WOOL:                          return 0.8f;
		case BlockType::YELLOW_GLAZED_TERRACOTTA:      return 1.4f;
		case BlockType::YELLOW_SHULKER_BOX:            return 2.0f;
		default:                                    return 0;
	}
}
