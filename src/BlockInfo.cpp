#include "Globals.h"

#include "BlockInfo.h"
#include "BlockType.h"





bool IsBlockWater(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == E_BLOCK_WATER) || (a_BlockType == E_BLOCK_STATIONARY_WATER));
}





bool IsBlockIce(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ICE:
		case E_BLOCK_PACKED_ICE:
		case E_BLOCK_FROSTED_ICE:
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
	return ((a_BlockType == E_BLOCK_LAVA) || (a_BlockType == E_BLOCK_STATIONARY_LAVA));
}





bool IsBlockLiquid(BLOCKTYPE a_BlockType)
{
	return IsBlockWater(a_BlockType) || IsBlockLava(a_BlockType);
}





bool IsBlockRail(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_RAIL:
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_POWERED_RAIL:
		{
			return true;
		}
		default: return false;
	}
}





bool IsBlockTypeOfDirt(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_FARMLAND:
		case E_BLOCK_GRASS_PATH:
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
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_FENCE:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
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
	return ((a_BlockType >= E_BLOCK_WHITE_SHULKER_BOX) && (a_BlockType <= E_BLOCK_BLACK_SHULKER_BOX));
}





bool IsBlockMaterialWood(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_PLANKS:
		case E_BLOCK_LOG:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_BOOKCASE:
		case E_BLOCK_OAK_WOOD_STAIRS:
		case E_BLOCK_CHEST:
		case E_BLOCK_CRAFTING_TABLE:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_OAK_DOOR:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_FENCE:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_HUGE_BROWN_MUSHROOM:
		case E_BLOCK_HUGE_RED_MUSHROOM:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_DOUBLE_WOODEN_SLAB:
		case E_BLOCK_WOODEN_SLAB:
		case E_BLOCK_SPRUCE_WOOD_STAIRS:
		case E_BLOCK_BIRCH_WOOD_STAIRS:
		case E_BLOCK_JUNGLE_WOOD_STAIRS:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_DAYLIGHT_SENSOR:
		case E_BLOCK_NEW_LOG:
		case E_BLOCK_ACACIA_WOOD_STAIRS:
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_DARK_OAK_DOOR:
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
		case E_BLOCK_SAPLING:
		case E_BLOCK_DANDELION:
		case E_BLOCK_FLOWER:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_CROPS:
		case E_BLOCK_REEDS:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_NETHER_WART:
		case E_BLOCK_COCOA_POD:
		case E_BLOCK_CARROTS:
		case E_BLOCK_POTATOES:
		case E_BLOCK_CHORUS_PLANT:
		case E_BLOCK_CHORUS_FLOWER:
		case E_BLOCK_BEETROOTS:
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
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_VINES:
		case E_BLOCK_BIG_FLOWER:
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
		case E_BLOCK_LAPIS_BLOCK:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_BARS:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_CAULDRON:
		case E_BLOCK_EMERALD_BLOCK:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_REPEATING_COMMAND_BLOCK:
		case E_BLOCK_CHAIN_COMMAND_BLOCK:
		case E_BLOCK_STRUCTURE_BLOCK:
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
	return (a_BlockType == E_BLOCK_LEAVES) || (a_BlockType == E_BLOCK_NEW_LEAVES);
}





bool IsBlockMaterialGourd(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_PUMPKIN:
		case E_BLOCK_JACK_O_LANTERN:
		case E_BLOCK_MELON:
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
		case E_BLOCK_STONE:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_BEDROCK:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_LAPIS_ORE:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_BRICK:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_OBSIDIAN:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_FURNACE:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_END_PORTAL_FRAME:
		case E_BLOCK_END_STONE:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_EMERALD_ORE:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_NETHER_QUARTZ_ORE:
		case E_BLOCK_QUARTZ_BLOCK:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_DROPPER:
		case E_BLOCK_STAINED_CLAY:
		case E_BLOCK_PRISMARINE_BLOCK:
		case E_BLOCK_HARDENED_CLAY:
		case E_BLOCK_BLOCK_OF_COAL:
		case E_BLOCK_RED_SANDSTONE:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
		case E_BLOCK_RED_SANDSTONE_SLAB:
		case E_BLOCK_PURPUR_BLOCK:
		case E_BLOCK_PURPUR_PILLAR:
		case E_BLOCK_PURPUR_STAIRS:
		case E_BLOCK_PURPUR_DOUBLE_SLAB:
		case E_BLOCK_PURPUR_SLAB:
		case E_BLOCK_END_BRICKS:
		case E_BLOCK_MAGMA:
		case E_BLOCK_RED_NETHER_BRICK:
		case E_BLOCK_BONE_BLOCK:
		case E_BLOCK_OBSERVER:
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

float cBlockInfo::GetExplosionAbsorption(const BLOCKTYPE Block)
{
	switch (Block)
	{
		case E_BLOCK_BEDROCK:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_END_GATEWAY:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_END_PORTAL_FRAME: return 3600000;
		case E_BLOCK_ANVIL:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_OBSIDIAN: return 1200;
		case E_BLOCK_ENDER_CHEST: return 600;
		case E_BLOCK_LAVA:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_WATER:
		case E_BLOCK_STATIONARY_WATER: return 100;
		case E_BLOCK_DRAGON_EGG:
		case E_BLOCK_END_STONE:
		case E_BLOCK_END_BRICKS: return 9;
		case E_BLOCK_STONE:
		case E_BLOCK_BLOCK_OF_COAL:
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_EMERALD_BLOCK:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_BRICK:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_IRON_BARS:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_PRISMARINE_BLOCK:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_COBBLESTONE_WALL: return 6;
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_MOB_SPAWNER: return 5;
		case E_BLOCK_HOPPER: return 4.8f;
		case E_BLOCK_TERRACOTTA: return 4.2f;
		case E_BLOCK_COBWEB: return 4;
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_FURNACE:
		case E_BLOCK_OBSERVER: return 3.5f;
		case E_BLOCK_BEACON:
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_COCOA_POD:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_EMERALD_ORE:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_LAPIS_BLOCK:
		case E_BLOCK_LAPIS_ORE:
		case E_BLOCK_NETHER_QUARTZ_ORE:
		case E_BLOCK_PLANKS:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_FENCE:
		case E_BLOCK_FENCE_GATE:
		case E_BLOCK_WOODEN_DOOR:
		case E_BLOCK_WOODEN_SLAB:
		case E_BLOCK_WOODEN_STAIRS:
		case E_BLOCK_TRAPDOOR: return 3;
		case E_BLOCK_CHEST:
		case E_BLOCK_WORKBENCH:
		case E_BLOCK_TRAPPED_CHEST: return 2.5f;
		case E_BLOCK_BONE_BLOCK:
		case E_BLOCK_CAULDRON:
		case E_BLOCK_LOG: return 2;
		case E_BLOCK_CONCRETE: return 1.8f;
		case E_BLOCK_BOOKCASE: return 1.5f;
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_JACK_O_LANTERN:
		case E_BLOCK_MELON:
		case E_BLOCK_HEAD:
		case E_BLOCK_NETHER_WART_BLOCK:
		case E_BLOCK_PUMPKIN:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN: return 1;
		case E_BLOCK_QUARTZ_BLOCK:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_RED_SANDSTONE:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_WOOL: return 0.8f;
		case E_BLOCK_SILVERFISH_EGG: return 0.75f;
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_RAIL: return 0.7f;
		case E_BLOCK_GRASS_PATH:
		case E_BLOCK_CLAY:
		case E_BLOCK_FARMLAND:
		case E_BLOCK_GRASS:
		case E_BLOCK_GRAVEL:
		case E_BLOCK_SPONGE: return 0.6f;
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_CAKE:
		case E_BLOCK_CONCRETE_POWDER:
		case E_BLOCK_DIRT:
		case E_BLOCK_FROSTED_ICE:
		case E_BLOCK_HAY_BALE:
		case E_BLOCK_ICE: return 0.5f;
		default: return 0;
	}
}





NIBBLETYPE cBlockInfo::GetLightValue(const BLOCKTYPE Block)
{
	// Emissive blocks:
	switch (Block)
	{
		case E_BLOCK_ACTIVE_COMPARATOR:    return 9;
		case E_BLOCK_BEACON:               return 15;
		case E_BLOCK_BREWING_STAND:        return 1;
		case E_BLOCK_BROWN_MUSHROOM:       return 1;
		case E_BLOCK_BURNING_FURNACE:      return 13;
		case E_BLOCK_DRAGON_EGG:           return 1;
		case E_BLOCK_END_PORTAL:           return 15;
		case E_BLOCK_END_PORTAL_FRAME:     return 1;
		case E_BLOCK_END_ROD:              return 14;
		case E_BLOCK_ENDER_CHEST:          return 7;
		case E_BLOCK_FIRE:                 return 15;
		case E_BLOCK_GLOWSTONE:            return 15;
		case E_BLOCK_JACK_O_LANTERN:       return 15;
		case E_BLOCK_LAVA:                 return 15;
		case E_BLOCK_MAGMA:                return 3;
		case E_BLOCK_NETHER_PORTAL:        return 11;
		case E_BLOCK_REDSTONE_LAMP_ON:     return 15;
		case E_BLOCK_REDSTONE_ORE_GLOWING: return 9;
		case E_BLOCK_REDSTONE_REPEATER_ON: return 9;
		case E_BLOCK_REDSTONE_TORCH_ON:    return 7;
		case E_BLOCK_SEA_LANTERN:          return 15;
		case E_BLOCK_STATIONARY_LAVA:      return 15;
		case E_BLOCK_TORCH:                return 14;
		default:                           return 0;
	}
}





NIBBLETYPE cBlockInfo::GetSpreadLightFalloff(const BLOCKTYPE Block)
{
	switch (Block)
	{
		// Spread blocks:
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_AIR:
		case E_BLOCK_ANVIL:
		case E_BLOCK_BARRIER:
		case E_BLOCK_BEACON:
		case E_BLOCK_BED:
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_BIG_FLOWER:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_CACTUS:
		case E_BLOCK_CAKE:
		case E_BLOCK_CARPET:
		case E_BLOCK_CARROTS:
		case E_BLOCK_CAULDRON:
		case E_BLOCK_CHORUS_FLOWER:
		case E_BLOCK_CHORUS_PLANT:
		case E_BLOCK_CHEST:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_COCOA_POD:
		case E_BLOCK_COBWEB:
		case E_BLOCK_CROPS:
		case E_BLOCK_DANDELION:
		case E_BLOCK_DARK_OAK_DOOR:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_DAYLIGHT_SENSOR:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_DRAGON_EGG:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_END_PORTAL_FRAME:
		case E_BLOCK_END_ROD:
		case E_BLOCK_FENCE:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_GLASS:
		case E_BLOCK_GLASS_PANE:
		case E_BLOCK_HEAD:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
		case E_BLOCK_IRON_BARS:
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_LADDER:
		case E_BLOCK_LEAVES:
		case E_BLOCK_LEVER:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_NETHER_PORTAL:
		case E_BLOCK_NETHER_WART:
		case E_BLOCK_NEW_LEAVES:
		case E_BLOCK_PISTON:
		case E_BLOCK_PISTON_EXTENSION:
		case E_BLOCK_PISTON_MOVED_BLOCK:
		case E_BLOCK_POTATOES:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_RAIL:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_SAPLING:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_SNOW:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_STAINED_GLASS:
		case E_BLOCK_STAINED_GLASS_PANE:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_STICKY_PISTON:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_SUGARCANE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_VINES:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_OAK_DOOR:
		case E_BLOCK_WOODEN_PRESSURE_PLATE: return 1;

		// Light in ice and water disappears faster:
		case E_BLOCK_ICE:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_WATER: return 3;

		// Light does not pass through these blocks at all:
		case E_BLOCK_ACACIA_WOOD_STAIRS:
		case E_BLOCK_BIRCH_WOOD_STAIRS:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_DOUBLE_WOODEN_SLAB:
		case E_BLOCK_FARMLAND:
		case E_BLOCK_JUNGLE_WOOD_STAIRS:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_OAK_WOOD_STAIRS:
		case E_BLOCK_PURPUR_DOUBLE_SLAB:
		case E_BLOCK_PURPUR_SLAB:
		case E_BLOCK_PURPUR_STAIRS:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_RED_SANDSTONE_SLAB:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_SPRUCE_WOOD_STAIRS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_WOODEN_SLAB: return 15;

		default: return 0;
	}
}





bool cBlockInfo::CanBeTerraformed(const BLOCKTYPE Block)
{
	// Blocks that can be terraformed:
	switch (Block)
	{
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_DIRT:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_GRASS:
		case E_BLOCK_GRAVEL:
		case E_BLOCK_HARDENED_CLAY:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_MYCELIUM:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_SAND:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_SOULSAND:
		case E_BLOCK_STAINED_CLAY:
		case E_BLOCK_STONE: return true;
		default: return false;
	}
}





bool cBlockInfo::FullyOccupiesVoxel(const BLOCKTYPE Block)
{
	// Blocks that fully occupy their voxel - used as a guide for torch placeable blocks, amongst other things:
	switch (Block)
	{
		case E_BLOCK_BARRIER:
		case E_BLOCK_BEDROCK:
		case E_BLOCK_BLOCK_OF_COAL:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_BONE_BLOCK:
		case E_BLOCK_BOOKCASE:
		case E_BLOCK_BRICK:
		case E_BLOCK_CHAIN_COMMAND_BLOCK:
		case E_BLOCK_CLAY:
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_CONCRETE:
		case E_BLOCK_CONCRETE_POWDER:
		case E_BLOCK_CRAFTING_TABLE:
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_DIRT:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_DOUBLE_WOODEN_SLAB:
		case E_BLOCK_DROPPER:
		case E_BLOCK_EMERALD_BLOCK:
		case E_BLOCK_EMERALD_ORE:
		case E_BLOCK_END_BRICKS:
		case E_BLOCK_END_STONE:
		case E_BLOCK_FROSTED_ICE:
		case E_BLOCK_FURNACE:
		case E_BLOCK_GLOWSTONE:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_WHITE_GLAZED_TERRACOTTA:
		case E_BLOCK_ORANGE_GLAZED_TERRACOTTA:
		case E_BLOCK_MAGENTA_GLAZED_TERRACOTTA:
		case E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA:
		case E_BLOCK_YELLOW_GLAZED_TERRACOTTA:
		case E_BLOCK_LIME_GLAZED_TERRACOTTA:
		case E_BLOCK_PINK_GLAZED_TERRACOTTA:
		case E_BLOCK_GRAY_GLAZED_TERRACOTTA:
		case E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA:
		case E_BLOCK_CYAN_GLAZED_TERRACOTTA:
		case E_BLOCK_PURPLE_GLAZED_TERRACOTTA:
		case E_BLOCK_BLUE_GLAZED_TERRACOTTA:
		case E_BLOCK_BROWN_GLAZED_TERRACOTTA:
		case E_BLOCK_GREEN_GLAZED_TERRACOTTA:
		case E_BLOCK_BLACK_GLAZED_TERRACOTTA:
		case E_BLOCK_RED_GLAZED_TERRACOTTA:
		case E_BLOCK_GRASS:
		case E_BLOCK_GRAVEL:
		case E_BLOCK_HARDENED_CLAY:
		case E_BLOCK_HAY_BALE:
		case E_BLOCK_HUGE_BROWN_MUSHROOM:
		case E_BLOCK_HUGE_RED_MUSHROOM:
		case E_BLOCK_ICE:
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_JACK_O_LANTERN:
		case E_BLOCK_JUKEBOX:
		case E_BLOCK_LAPIS_BLOCK:
		case E_BLOCK_LAPIS_ORE:
		case E_BLOCK_LOG:
		case E_BLOCK_MAGMA:
		case E_BLOCK_MELON:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_MYCELIUM:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_NETHER_QUARTZ_ORE:
		case E_BLOCK_NETHER_WART_BLOCK:
		case E_BLOCK_NEW_LOG:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_OBSERVER:
		case E_BLOCK_OBSIDIAN:
		case E_BLOCK_PACKED_ICE:
		case E_BLOCK_PLANKS:
		case E_BLOCK_PRISMARINE_BLOCK:
		case E_BLOCK_PUMPKIN:
		case E_BLOCK_PURPUR_BLOCK:
		case E_BLOCK_PURPUR_PILLAR:
		case E_BLOCK_PURPUR_DOUBLE_SLAB:
		case E_BLOCK_QUARTZ_BLOCK:
		case E_BLOCK_RED_NETHER_BRICK:
		case E_BLOCK_RED_SANDSTONE:
		case E_BLOCK_REDSTONE_LAMP_OFF:
		case E_BLOCK_REDSTONE_LAMP_ON:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_REPEATING_COMMAND_BLOCK:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_SAND:
		case E_BLOCK_SILVERFISH_EGG:
		case E_BLOCK_SPONGE:
		case E_BLOCK_STAINED_CLAY:
		case E_BLOCK_STONE:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_STRUCTURE_BLOCK:
		case E_BLOCK_WOOL: return true;
		default: return false;
	}
}





bool cBlockInfo::IsOneHitDig(const BLOCKTYPE Block)
{
	// One hit break blocks:
	switch (Block)
	{
		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_BIG_FLOWER:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_CARROTS:
		case E_BLOCK_CROPS:
		case E_BLOCK_DANDELION:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_END_ROD:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_NETHER_WART:
		case E_BLOCK_POTATOES:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_REEDS:
		case E_BLOCK_SAPLING:
		case E_BLOCK_SLIME_BLOCK:
		case E_BLOCK_TNT:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_TRIPWIRE: return true;
		default: return false;
	}
}





bool cBlockInfo::IsPistonBreakable(const BLOCKTYPE Block)
{
	// Blocks that break when pushed by piston:
	switch (Block)
	{
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_AIR:
		case E_BLOCK_BED:
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_BIG_FLOWER:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_BLACK_SHULKER_BOX:
		case E_BLOCK_BLUE_SHULKER_BOX:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_BROWN_SHULKER_BOX:
		case E_BLOCK_CACTUS:
		case E_BLOCK_CAKE:
		case E_BLOCK_CARROTS:
		case E_BLOCK_CHORUS_FLOWER:
		case E_BLOCK_CHORUS_PLANT:
		case E_BLOCK_COCOA_POD:
		case E_BLOCK_COBWEB:
		case E_BLOCK_CROPS:
		case E_BLOCK_CYAN_SHULKER_BOX:
		case E_BLOCK_DANDELION:
		case E_BLOCK_DARK_OAK_DOOR:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DRAGON_EGG:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_GRAY_SHULKER_BOX:
		case E_BLOCK_GREEN_SHULKER_BOX:
		case E_BLOCK_HEAD:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_JACK_O_LANTERN:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_LIME_SHULKER_BOX:
		case E_BLOCK_LADDER:
		case E_BLOCK_LAVA:
		case E_BLOCK_LEAVES:
		case E_BLOCK_LEVER:
		case E_BLOCK_MAGENTA_SHULKER_BOX:
		case E_BLOCK_MELON:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_NETHER_WART:
		case E_BLOCK_OAK_DOOR:
		case E_BLOCK_ORANGE_SHULKER_BOX:
		case E_BLOCK_PINK_SHULKER_BOX:
		case E_BLOCK_POTATOES:
		case E_BLOCK_PUMPKIN:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_PURPLE_SHULKER_BOX:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_RED_SHULKER_BOX:
		case E_BLOCK_REEDS:
		case E_BLOCK_SAPLING:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_SNOW:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_VINES:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WATER:
		case E_BLOCK_WHITE_SHULKER_BOX:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_YELLOW_SHULKER_BOX: return true;
		default: return false;
	}
}





bool cBlockInfo::IsRainBlocker(const BLOCKTYPE Block)
{
	// Blocks that block rain or snow's passage:
	switch (Block)
	{
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_STANDING_BANNER: return true;
		default: return IsSolid(Block);
	}
}





bool cBlockInfo::IsSkylightDispersant(const BLOCKTYPE Block)
{
	// Skylight dispersant blocks:
	switch (Block)
	{
		case E_BLOCK_COBWEB:
		case E_BLOCK_LEAVES: return true;
		default: return GetSpreadLightFalloff(Block) > 1;
	}
}





bool cBlockInfo::IsSnowable(BLOCKTYPE a_BlockType)
{
	return (
		(a_BlockType == E_BLOCK_ICE) ||
		(a_BlockType == E_BLOCK_LEAVES) ||
		(!IsTransparent(a_BlockType) && (a_BlockType != E_BLOCK_PACKED_ICE))
	);
}





bool cBlockInfo::IsSolid(const BLOCKTYPE Block)
{
	// Nonsolid blocks:
	switch (Block)
	{
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_AIR:
		case E_BLOCK_BIG_FLOWER:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_CARPET:
		case E_BLOCK_CARROTS:
		case E_BLOCK_CHORUS_FLOWER:
		case E_BLOCK_CHORUS_PLANT:
		case E_BLOCK_COBWEB:
		case E_BLOCK_CROPS:
		case E_BLOCK_DANDELION:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_END_GATEWAY:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_END_ROD:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_LADDER:
		case E_BLOCK_LAVA:
		case E_BLOCK_LEVER:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_NETHER_PORTAL:
		case E_BLOCK_POTATOES:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_RAIL:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_REEDS:
		case E_BLOCK_SAPLING:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_SNOW:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_VINES:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WATER:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_WOODEN_PRESSURE_PLATE: return false;
		default: return true;
	}
}





bool cBlockInfo::IsTransparent(const BLOCKTYPE Block)
{
	// Transparent blocks:
	switch (Block)
	{
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_ACACIA_WOOD_STAIRS:
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_AIR:
		case E_BLOCK_ANVIL:
		case E_BLOCK_BARRIER:
		case E_BLOCK_BEACON:
		case E_BLOCK_BED:
		case E_BLOCK_BIG_FLOWER:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_BIRCH_WOOD_STAIRS:
		case E_BLOCK_BLACK_SHULKER_BOX:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_BLUE_SHULKER_BOX:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_BROWN_SHULKER_BOX:
		case E_BLOCK_CACTUS:
		case E_BLOCK_CAKE:
		case E_BLOCK_CARPET:
		case E_BLOCK_CARROTS:
		case E_BLOCK_CAULDRON:
		case E_BLOCK_CHEST:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_COBBLESTONE_WALL:
		case E_BLOCK_COBWEB:
		case E_BLOCK_COCOA_POD:
		case E_BLOCK_CROPS:
		case E_BLOCK_CYAN_SHULKER_BOX:
		case E_BLOCK_DANDELION:
		case E_BLOCK_DARK_OAK_DOOR:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:
		case E_BLOCK_DAYLIGHT_SENSOR:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_DOUBLE_WOODEN_SLAB:
		case E_BLOCK_DRAGON_EGG:
		case E_BLOCK_ENCHANTMENT_TABLE:
		case E_BLOCK_END_PORTAL:
		case E_BLOCK_END_PORTAL_FRAME:
		case E_BLOCK_END_ROD:
		case E_BLOCK_ENDER_CHEST:
		case E_BLOCK_FARMLAND:
		case E_BLOCK_FENCE:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_GLASS:
		case E_BLOCK_GLASS_PANE:
		case E_BLOCK_GLOWSTONE:
		case E_BLOCK_GRAY_SHULKER_BOX:
		case E_BLOCK_GREEN_SHULKER_BOX:
		case E_BLOCK_HEAD:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_ICE:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
		case E_BLOCK_IRON_BARS:
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_JACK_O_LANTERN:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_JUNGLE_WOOD_STAIRS:
		case E_BLOCK_LADDER:
		case E_BLOCK_LAVA:
		case E_BLOCK_LEAVES:
		case E_BLOCK_LEVER:
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_LILY_PAD:
		case E_BLOCK_LIME_SHULKER_BOX:
		case E_BLOCK_LIT_FURNACE:
		case E_BLOCK_MAGENTA_SHULKER_BOX:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_MOB_SPAWNER:
		case E_BLOCK_NETHER_BRICK_FENCE:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_NETHER_PORTAL:
		case E_BLOCK_NETHER_WART:
		case E_BLOCK_NEW_LEAVES:
		case E_BLOCK_OAK_DOOR:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_OAK_WOOD_STAIRS:
		case E_BLOCK_ORANGE_SHULKER_BOX:
		case E_BLOCK_PINK_SHULKER_BOX:
		case E_BLOCK_PISTON:
		case E_BLOCK_PISTON_EXTENSION:
		case E_BLOCK_PISTON_MOVED_BLOCK:
		case E_BLOCK_POTATOES:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_PURPLE_SHULKER_BOX:
		case E_BLOCK_PURPUR_DOUBLE_SLAB:
		case E_BLOCK_PURPUR_SLAB:
		case E_BLOCK_PURPUR_STAIRS:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_RAIL:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_RED_SANDSTONE_SLAB:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_RED_SHULKER_BOX:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_SAPLING:
		case E_BLOCK_SEA_LANTERN:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_SNOW:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_SPRUCE_WOOD_STAIRS:
		case E_BLOCK_STAINED_GLASS:
		case E_BLOCK_STAINED_GLASS_PANE:
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_STICKY_PISTON:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_SUGARCANE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_VINES:
		case E_BLOCK_WALL_BANNER:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WATER:
		case E_BLOCK_WHITE_SHULKER_BOX:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_WOODEN_SLAB:
		case E_BLOCK_YELLOW_SHULKER_BOX: return true;
		default: return false;
	}
}





bool cBlockInfo::IsUseableBySpectator(const BLOCKTYPE Block)
{
	// Blocks, which a spectator is allowed to interact with:
	switch (Block)
	{
		case E_BLOCK_BEACON:
		case E_BLOCK_BREWING_STAND:
		case E_BLOCK_CHEST:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_HOPPER: return true;
		default: return false;
	}
}





float cBlockInfo::GetBlockHeight(const BLOCKTYPE Block)
{
	// Block heights:
	switch (Block)
	{
		case E_BLOCK_ACACIA_FENCE:        return 1.5;
		case E_BLOCK_ACACIA_FENCE_GATE:   return 1.5;
		case E_BLOCK_BED:                 return 0.5625;  // 9 pixels
		case E_BLOCK_BIRCH_FENCE:         return 1.5;
		case E_BLOCK_BIRCH_FENCE_GATE:    return 1.5;
		case E_BLOCK_CAKE:                return 0.5;     // 8 pixels
		case E_BLOCK_DARK_OAK_FENCE:      return 1.5;
		case E_BLOCK_DARK_OAK_FENCE_GATE: return 1.5;
		case E_BLOCK_ENCHANTMENT_TABLE:   return 0.75;    // 12 pixels
		case E_BLOCK_FENCE:               return 1.5;
		case E_BLOCK_JUNGLE_FENCE:        return 1.5;
		case E_BLOCK_JUNGLE_FENCE_GATE:   return 1.5;
		case E_BLOCK_OAK_FENCE_GATE:      return 1.5;
		case E_BLOCK_PURPUR_SLAB:         return 0.5;
		case E_BLOCK_RED_SANDSTONE_SLAB:  return 0.5;
		case E_BLOCK_SNOW:                return 0.125;   // one layer is 1 / 8 (2 pixels) tall
		case E_BLOCK_SPRUCE_FENCE:        return 1.5;
		case E_BLOCK_SPRUCE_FENCE_GATE:   return 1.5;
		case E_BLOCK_STONE_SLAB:          return 0.5;
		case E_BLOCK_WOODEN_SLAB:         return 0.5;
		default:                          return 1;
	}
}





float cBlockInfo::GetHardness(const BLOCKTYPE Block)
{
	// Block hardness:
	switch (Block)
	{
		case E_BLOCK_ACACIA_DOOR:                   return 3.0f;
		case E_BLOCK_ACACIA_FENCE:                  return 2.0f;
		case E_BLOCK_ACACIA_FENCE_GATE:             return 2.0f;
		case E_BLOCK_ACACIA_WOOD_STAIRS:            return 2.0f;
		case E_BLOCK_ACTIVATOR_RAIL:                return 0.7f;
		case E_BLOCK_ACTIVE_COMPARATOR:             return 0.0f;
		case E_BLOCK_AIR:                           return 0.0f;
		case E_BLOCK_ANVIL:                         return 5.0f;
		case E_BLOCK_BARRIER:                       return -1.0f;
		case E_BLOCK_BEACON:                        return 3.0f;
		case E_BLOCK_BED:                           return 0.2f;
		case E_BLOCK_BEDROCK:                       return -1.0f;
		case E_BLOCK_BEETROOTS:                     return 0.0f;
		case E_BLOCK_BIG_FLOWER:                    return 0.0f;
		case E_BLOCK_BIRCH_DOOR:                    return 3.0f;
		case E_BLOCK_BIRCH_FENCE:                   return 2.0f;
		case E_BLOCK_BIRCH_FENCE_GATE:              return 2.0f;
		case E_BLOCK_BIRCH_WOOD_STAIRS:             return 2.0f;
		case E_BLOCK_BLACK_GLAZED_TERRACOTTA:       return 1.4f;
		case E_BLOCK_BLACK_SHULKER_BOX:             return 2.0f;
		case E_BLOCK_BLOCK_OF_COAL:                 return 5.0f;
		case E_BLOCK_BLOCK_OF_REDSTONE:             return 5.0f;
		case E_BLOCK_BLUE_GLAZED_TERRACOTTA:        return 1.4f;
		case E_BLOCK_BLUE_SHULKER_BOX:              return 2.0f;
		case E_BLOCK_BONE_BLOCK:                    return 2.0f;
		case E_BLOCK_BOOKCASE:                      return 1.5f;
		case E_BLOCK_BREWING_STAND:                 return 0.5f;
		case E_BLOCK_BRICK:                         return 2.0f;
		case E_BLOCK_BRICK_STAIRS:                  return 2.0f;
		case E_BLOCK_BROWN_GLAZED_TERRACOTTA:       return 1.4f;
		case E_BLOCK_BROWN_MUSHROOM:                return 0.0f;
		case E_BLOCK_BROWN_SHULKER_BOX:             return 2.0f;
		case E_BLOCK_CACTUS:                        return 0.4f;
		case E_BLOCK_CAKE:                          return 0.5f;
		case E_BLOCK_CARPET:                        return 0.1f;
		case E_BLOCK_CARROTS:                       return 0.0f;
		case E_BLOCK_CAULDRON:                      return 2.0f;
		case E_BLOCK_CHAIN_COMMAND_BLOCK:           return -1.0f;
		case E_BLOCK_CHEST:                         return 2.5f;
		case E_BLOCK_CHORUS_FLOWER:                 return 0.4f;
		case E_BLOCK_CHORUS_PLANT:                  return 0.4f;
		case E_BLOCK_CLAY:                          return 0.6f;
		case E_BLOCK_COAL_ORE:                      return 3.0f;
		case E_BLOCK_COBBLESTONE:                   return 2.0f;
		case E_BLOCK_COBBLESTONE_STAIRS:            return 2.0f;
		case E_BLOCK_COBBLESTONE_WALL:              return 2.0f;
		case E_BLOCK_COBWEB:                        return 4.0f;
		case E_BLOCK_COCOA_POD:                     return 0.2f;
		case E_BLOCK_COMMAND_BLOCK:                 return -1.0f;
		case E_BLOCK_CONCRETE:                      return 1.8f;
		case E_BLOCK_CONCRETE_POWDER:               return 0.5f;
		case E_BLOCK_CRAFTING_TABLE:                return 2.5f;
		case E_BLOCK_CROPS:                         return 0.0f;
		case E_BLOCK_CYAN_GLAZED_TERRACOTTA:        return 1.4f;
		case E_BLOCK_CYAN_SHULKER_BOX:              return 2.0f;
		case E_BLOCK_DANDELION:                     return 0.0f;
		case E_BLOCK_DARK_OAK_DOOR:                 return 3.0f;
		case E_BLOCK_DARK_OAK_FENCE:                return 2.0f;
		case E_BLOCK_DARK_OAK_FENCE_GATE:           return 2.0f;
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:          return 2.0f;
		case E_BLOCK_DAYLIGHT_SENSOR:               return 0.2f;
		case E_BLOCK_DEAD_BUSH:                     return 0.0f;
		case E_BLOCK_DETECTOR_RAIL:                 return 0.7f;
		case E_BLOCK_DIAMOND_BLOCK:                 return 5.0f;
		case E_BLOCK_DIAMOND_ORE:                   return 3.0f;
		case E_BLOCK_DIRT:                          return 0.5f;
		case E_BLOCK_DISPENSER:                     return 3.5f;
		case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:     return 2.0f;
		case E_BLOCK_DOUBLE_STONE_SLAB:             return 2.0f;
		case E_BLOCK_DOUBLE_WOODEN_SLAB:            return 2.0f;
		case E_BLOCK_DRAGON_EGG:                    return 3.0f;
		case E_BLOCK_DROPPER:                       return 3.5f;
		case E_BLOCK_EMERALD_BLOCK:                 return 5.0f;
		case E_BLOCK_EMERALD_ORE:                   return 3.0f;
		case E_BLOCK_ENCHANTMENT_TABLE:             return 5.0f;
		case E_BLOCK_ENDER_CHEST:                   return 22.5f;
		case E_BLOCK_END_BRICKS:                    return 0.8f;
		case E_BLOCK_END_GATEWAY:                   return -1.0f;
		case E_BLOCK_END_PORTAL:                    return -1.0f;
		case E_BLOCK_END_PORTAL_FRAME:              return -1.0f;
		case E_BLOCK_END_ROD:                       return 0.0f;
		case E_BLOCK_END_STONE:                     return 3.0f;
		case E_BLOCK_FARMLAND:                      return 0.6f;
		case E_BLOCK_FENCE:                         return 2.0f;
		case E_BLOCK_FIRE:                          return 0.0f;
		case E_BLOCK_FLOWER:                        return 0.0f;
		case E_BLOCK_FLOWER_POT:                    return 0.0f;
		case E_BLOCK_FROSTED_ICE:                   return 0.5f;
		case E_BLOCK_FURNACE:                       return 3.5f;
		case E_BLOCK_GLASS:                         return 0.3f;
		case E_BLOCK_GLASS_PANE:                    return 0.3f;
		case E_BLOCK_GLOWSTONE:                     return 0.3f;
		case E_BLOCK_GOLD_BLOCK:                    return 3.0f;
		case E_BLOCK_GOLD_ORE:                      return 3.0f;
		case E_BLOCK_GRASS:                         return 0.6f;
		case E_BLOCK_GRASS_PATH:                    return 0.6f;
		case E_BLOCK_GRAVEL:                        return 0.6f;
		case E_BLOCK_GRAY_GLAZED_TERRACOTTA:        return 1.4f;
		case E_BLOCK_GRAY_SHULKER_BOX:              return 2.0f;
		case E_BLOCK_GREEN_GLAZED_TERRACOTTA:       return 1.4f;
		case E_BLOCK_GREEN_SHULKER_BOX:             return 2.0f;
		case E_BLOCK_HARDENED_CLAY:                 return 1.25f;
		case E_BLOCK_HAY_BALE:                      return 0.5f;
		case E_BLOCK_HEAD:                          return 1.0f;
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return 0.5f;
		case E_BLOCK_HOPPER:                        return 3.0f;
		case E_BLOCK_HUGE_BROWN_MUSHROOM:           return 0.2f;
		case E_BLOCK_HUGE_RED_MUSHROOM:             return 0.2f;
		case E_BLOCK_ICE:                           return 0.5f;
		case E_BLOCK_INACTIVE_COMPARATOR:           return 0.0f;
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:      return 0.2f;
		case E_BLOCK_IRON_BARS:                     return 5.0f;
		case E_BLOCK_IRON_BLOCK:                    return 5.0f;
		case E_BLOCK_IRON_DOOR:                     return 5.0f;
		case E_BLOCK_IRON_ORE:                      return 3.0f;
		case E_BLOCK_IRON_TRAPDOOR:                 return 5.0f;
		case E_BLOCK_JACK_O_LANTERN:                return 1.0f;
		case E_BLOCK_JUKEBOX:                       return 2.0f;
		case E_BLOCK_JUNGLE_DOOR:                   return 3.0f;
		case E_BLOCK_JUNGLE_FENCE:                  return 2.0f;
		case E_BLOCK_JUNGLE_FENCE_GATE:             return 2.0f;
		case E_BLOCK_JUNGLE_WOOD_STAIRS:            return 2.0f;
		case E_BLOCK_LADDER:                        return 0.4f;
		case E_BLOCK_LAPIS_BLOCK:                   return 3.0f;
		case E_BLOCK_LAPIS_ORE:                     return 3.0f;
		case E_BLOCK_LAVA:                          return 100.0f;
		case E_BLOCK_LEAVES:                        return 0.2f;
		case E_BLOCK_LEVER:                         return 0.5f;
		case E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA:  return 1.4f;
		case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:        return 2.0f;
		case E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA:  return 1.4f;
		case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:        return 2.0f;
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return 0.5f;
		case E_BLOCK_LILY_PAD:                      return 0.0f;
		case E_BLOCK_LIME_GLAZED_TERRACOTTA:        return 1.4f;
		case E_BLOCK_LIME_SHULKER_BOX:              return 2.0f;
		case E_BLOCK_LIT_FURNACE:                   return 3.5f;
		case E_BLOCK_LOG:                           return 2.0f;
		case E_BLOCK_MAGENTA_GLAZED_TERRACOTTA:     return 1.4f;
		case E_BLOCK_MAGENTA_SHULKER_BOX:           return 2.0f;
		case E_BLOCK_MAGMA:                         return 0.5f;
		case E_BLOCK_MELON:                         return 1.0f;
		case E_BLOCK_MELON_STEM:                    return 0.0f;
		case E_BLOCK_MOB_SPAWNER:                   return 5.0f;
		case E_BLOCK_MOSSY_COBBLESTONE:             return 2.0f;
		case E_BLOCK_MYCELIUM:                      return 0.6f;
		case E_BLOCK_NETHERRACK:                    return 0.4f;
		case E_BLOCK_NETHER_BRICK:                  return 2.0f;
		case E_BLOCK_NETHER_BRICK_FENCE:            return 2.0f;
		case E_BLOCK_NETHER_BRICK_STAIRS:           return 2.0f;
		case E_BLOCK_NETHER_PORTAL:                 return -1.0f;
		case E_BLOCK_NETHER_QUARTZ_ORE:             return 3.0f;
		case E_BLOCK_NETHER_WART:                   return 0.0f;
		case E_BLOCK_NETHER_WART_BLOCK:             return 1.0f;
		case E_BLOCK_NEW_LEAVES:                    return 0.2f;
		case E_BLOCK_NEW_LOG:                       return 2.0f;
		case E_BLOCK_NOTE_BLOCK:                    return 0.8f;
		case E_BLOCK_OAK_DOOR:                      return 3.0f;
		case E_BLOCK_OAK_FENCE_GATE:                return 2.0f;
		case E_BLOCK_OAK_WOOD_STAIRS:               return 2.0f;
		case E_BLOCK_OBSERVER:                      return 3.5f;
		case E_BLOCK_OBSIDIAN:                      return 50.0f;
		case E_BLOCK_ORANGE_GLAZED_TERRACOTTA:      return 1.4f;
		case E_BLOCK_ORANGE_SHULKER_BOX:            return 2.0f;
		case E_BLOCK_PACKED_ICE:                    return 0.5f;
		case E_BLOCK_PINK_GLAZED_TERRACOTTA:        return 1.4f;
		case E_BLOCK_PINK_SHULKER_BOX:              return 2.0f;
		case E_BLOCK_PISTON:                        return 0.5f;
		case E_BLOCK_PISTON_EXTENSION:              return 0.5f;
		case E_BLOCK_PISTON_MOVED_BLOCK:            return -1.0f;
		case E_BLOCK_PLANKS:                        return 2.0f;
		case E_BLOCK_POTATOES:                      return 0.0f;
		case E_BLOCK_POWERED_RAIL:                  return 0.7f;
		case E_BLOCK_PRISMARINE_BLOCK:              return 1.5f;
		case E_BLOCK_PUMPKIN:                       return 1.0f;
		case E_BLOCK_PUMPKIN_STEM:                  return 0.0f;
		case E_BLOCK_PURPLE_GLAZED_TERRACOTTA:      return 1.4f;
		case E_BLOCK_PURPLE_SHULKER_BOX:            return 2.0f;
		case E_BLOCK_PURPUR_BLOCK:                  return 1.5f;
		case E_BLOCK_PURPUR_DOUBLE_SLAB:            return 2.0f;
		case E_BLOCK_PURPUR_PILLAR:                 return 1.5f;
		case E_BLOCK_PURPUR_SLAB:                   return 2.0f;
		case E_BLOCK_PURPUR_STAIRS:                 return 1.5f;
		case E_BLOCK_QUARTZ_BLOCK:                  return 0.8f;
		case E_BLOCK_QUARTZ_STAIRS:                 return 0.8f;
		case E_BLOCK_RAIL:                          return 0.7f;
		case E_BLOCK_REDSTONE_LAMP_OFF:             return 0.3f;
		case E_BLOCK_REDSTONE_LAMP_ON:              return 0.3f;
		case E_BLOCK_REDSTONE_ORE:                  return 3.0f;
		case E_BLOCK_REDSTONE_ORE_GLOWING:          return 0.625f;
		case E_BLOCK_REDSTONE_REPEATER_OFF:         return 0.0f;
		case E_BLOCK_REDSTONE_REPEATER_ON:          return 0.0f;
		case E_BLOCK_REDSTONE_TORCH_OFF:            return 0.0f;
		case E_BLOCK_REDSTONE_TORCH_ON:             return 0.0f;
		case E_BLOCK_REDSTONE_WIRE:                 return 0.0f;
		case E_BLOCK_RED_GLAZED_TERRACOTTA:         return 1.4f;
		case E_BLOCK_RED_MUSHROOM:                  return 0.0f;
		case E_BLOCK_RED_NETHER_BRICK:              return 2.0f;
		case E_BLOCK_RED_SANDSTONE:                 return 0.8f;
		case E_BLOCK_RED_SANDSTONE_SLAB:            return 2.0f;
		case E_BLOCK_RED_SANDSTONE_STAIRS:          return 0.8f;
		case E_BLOCK_RED_SHULKER_BOX:               return 2.0f;
		case E_BLOCK_REPEATING_COMMAND_BLOCK:       return -1.0f;
		case E_BLOCK_SAND:                          return 0.5f;
		case E_BLOCK_SANDSTONE:                     return 0.8f;
		case E_BLOCK_SANDSTONE_STAIRS:              return 0.8f;
		case E_BLOCK_SAPLING:                       return 0.0f;
		case E_BLOCK_SEA_LANTERN:                   return 0.3f;
		case E_BLOCK_SIGN_POST:                     return 1.0f;
		case E_BLOCK_SILVERFISH_EGG:                return 0.75f;
		case E_BLOCK_SLIME_BLOCK:                   return 0.0f;
		case E_BLOCK_SNOW:                          return 0.1f;
		case E_BLOCK_SNOW_BLOCK:                    return 0.2f;
		case E_BLOCK_SOULSAND:                      return 0.5f;
		case E_BLOCK_SPONGE:                        return 0.6f;
		case E_BLOCK_SPRUCE_DOOR:                   return 3.0f;
		case E_BLOCK_SPRUCE_FENCE:                  return 2.0f;
		case E_BLOCK_SPRUCE_FENCE_GATE:             return 2.0f;
		case E_BLOCK_SPRUCE_WOOD_STAIRS:            return 2.0f;
		case E_BLOCK_STAINED_CLAY:                  return 4.2f;
		case E_BLOCK_STAINED_GLASS:                 return 0.3f;
		case E_BLOCK_STAINED_GLASS_PANE:            return 0.3f;
		case E_BLOCK_STANDING_BANNER:               return 1.0f;
		case E_BLOCK_STATIONARY_LAVA:               return 100.0f;
		case E_BLOCK_STATIONARY_WATER:              return 100.0f;
		case E_BLOCK_STICKY_PISTON:                 return 0.5f;
		case E_BLOCK_STONE:                         return 1.5f;
		case E_BLOCK_STONE_BRICKS:                  return 1.5f;
		case E_BLOCK_STONE_BRICK_STAIRS:            return 1.5f;
		case E_BLOCK_STONE_BUTTON:                  return 0.5f;
		case E_BLOCK_STONE_PRESSURE_PLATE:          return 0.5f;
		case E_BLOCK_STONE_SLAB:                    return 2.0f;
		case E_BLOCK_STRUCTURE_BLOCK:               return -1.0f;
		case E_BLOCK_STRUCTURE_VOID:                return 0.0f;
		case E_BLOCK_SUGARCANE:                     return 0.0f;
		case E_BLOCK_TALL_GRASS:                    return 0.0f;
		case E_BLOCK_TNT:                           return 0.0f;
		case E_BLOCK_TORCH:                         return 0.0f;
		case E_BLOCK_TRAPDOOR:                      return 3.0f;
		case E_BLOCK_TRAPPED_CHEST:                 return 2.5f;
		case E_BLOCK_TRIPWIRE:                      return 0.0f;
		case E_BLOCK_TRIPWIRE_HOOK:                 return 0.0f;
		case E_BLOCK_VINES:                         return 0.2f;
		case E_BLOCK_WALLSIGN:                      return 1.0f;
		case E_BLOCK_WALL_BANNER:                   return 1.0f;
		case E_BLOCK_WATER:                         return 100.0f;
		case E_BLOCK_WHITE_GLAZED_TERRACOTTA:       return 1.4f;
		case E_BLOCK_WHITE_SHULKER_BOX:             return 2.0f;
		case E_BLOCK_WOODEN_BUTTON:                 return 0.5f;
		case E_BLOCK_WOODEN_PRESSURE_PLATE:         return 0.5f;
		case E_BLOCK_WOODEN_SLAB:                   return 2.0f;
		case E_BLOCK_WOOL:                          return 0.8f;
		case E_BLOCK_YELLOW_GLAZED_TERRACOTTA:      return 1.4f;
		case E_BLOCK_YELLOW_SHULKER_BOX:            return 2.0f;
		default:                                    return 0;
	}
}
