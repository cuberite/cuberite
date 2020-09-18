#include "Globals.h"

#include "BlockInfo.h"
#include "BlockType.h"
#include "Blocks/BlockHandler.h"





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

cBlockInfo::cBlockInfo():
	m_BlockType(E_BLOCK_STONE),
	m_LightValue(0x00),
	m_SpreadLightFalloff(0x0f),
	m_Transparent(false),
	m_OneHitDig(false),
	m_PistonBreakable(false),
	m_IsRainBlocker(false),
	m_IsSkylightDispersant(false),
	m_IsSolid(true),
	m_UseableBySpectator(false),
	m_FullyOccupiesVoxel(false),
	m_CanBeTerraformed(false),
	m_BlockHeight(1.0),
	m_Hardness(0.0f)
{
}





cBlockHandler * cBlockInfo::GetHandler(BLOCKTYPE a_Type)
{
	return &cBlockHandler::GetBlockHandler(a_Type);
}





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





bool cBlockInfo::IsSnowable(BLOCKTYPE a_BlockType)
{
	return (
		(a_BlockType == E_BLOCK_ICE) ||
		(a_BlockType == E_BLOCK_LEAVES) ||
		(!IsTransparent(a_BlockType) && (a_BlockType != E_BLOCK_PACKED_ICE))
	);
}





cBlockInfo::cBlockInfoArray::cBlockInfoArray()
{
	cBlockInfoArray & Info = *this;

	for (size_t i = 0; i < Info.size(); ++i)
	{
		Info[i].m_BlockType = static_cast<BLOCKTYPE>(i);
	}

	// Emissive blocks
	Info[E_BLOCK_ACTIVE_COMPARATOR            ].m_LightValue = 9;
	Info[E_BLOCK_BEACON                       ].m_LightValue = 15;
	Info[E_BLOCK_BREWING_STAND                ].m_LightValue = 1;
	Info[E_BLOCK_BROWN_MUSHROOM               ].m_LightValue = 1;
	Info[E_BLOCK_BURNING_FURNACE              ].m_LightValue = 13;
	Info[E_BLOCK_DRAGON_EGG                   ].m_LightValue = 1;
	Info[E_BLOCK_END_PORTAL                   ].m_LightValue = 15;
	Info[E_BLOCK_END_PORTAL_FRAME             ].m_LightValue = 1;
	Info[E_BLOCK_END_ROD                      ].m_LightValue = 14;
	Info[E_BLOCK_ENDER_CHEST                  ].m_LightValue = 7;
	Info[E_BLOCK_FIRE                         ].m_LightValue = 15;
	Info[E_BLOCK_GLOWSTONE                    ].m_LightValue = 15;
	Info[E_BLOCK_JACK_O_LANTERN               ].m_LightValue = 15;
	Info[E_BLOCK_LAVA                         ].m_LightValue = 15;
	Info[E_BLOCK_MAGMA                        ].m_LightValue = 3;
	Info[E_BLOCK_NETHER_PORTAL                ].m_LightValue = 11;
	Info[E_BLOCK_REDSTONE_LAMP_ON             ].m_LightValue = 15;
	Info[E_BLOCK_REDSTONE_ORE_GLOWING         ].m_LightValue = 9;
	Info[E_BLOCK_REDSTONE_REPEATER_ON         ].m_LightValue = 9;
	Info[E_BLOCK_REDSTONE_TORCH_ON            ].m_LightValue = 7;
	Info[E_BLOCK_SEA_LANTERN                  ].m_LightValue = 15;
	Info[E_BLOCK_STATIONARY_LAVA              ].m_LightValue = 15;
	Info[E_BLOCK_TORCH                        ].m_LightValue = 14;


	// Spread blocks
	Info[E_BLOCK_ACACIA_DOOR                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_ACACIA_FENCE                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_ACACIA_FENCE_GATE            ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_ACTIVATOR_RAIL               ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_ACTIVE_COMPARATOR            ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_AIR                          ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_ANVIL                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BARRIER                      ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BEACON                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BED                          ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BEETROOTS                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BIG_FLOWER                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BIRCH_DOOR                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BIRCH_FENCE                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BIRCH_FENCE_GATE             ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BROWN_MUSHROOM               ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_BREWING_STAND                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CACTUS                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CAKE                         ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CARPET                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CARROTS                      ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CAULDRON                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CHORUS_FLOWER                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CHORUS_PLANT                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CHEST                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_COBBLESTONE_WALL             ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_COCOA_POD                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_COBWEB                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_CROPS                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DANDELION                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DARK_OAK_DOOR                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DARK_OAK_FENCE               ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DARK_OAK_FENCE_GATE          ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DAYLIGHT_SENSOR              ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DEAD_BUSH                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DETECTOR_RAIL                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_DRAGON_EGG                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_ENCHANTMENT_TABLE            ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_ENDER_CHEST                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_END_PORTAL                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_END_PORTAL_FRAME             ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_END_ROD                      ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_FARMLAND                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_FENCE                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_OAK_FENCE_GATE               ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_FIRE                         ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_FLOWER                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_FLOWER_POT                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_GLASS                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_GLASS_PANE                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_HEAD                         ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_HOPPER                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_INACTIVE_COMPARATOR          ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_INVERTED_DAYLIGHT_SENSOR     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_IRON_BARS                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_IRON_DOOR                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_IRON_TRAPDOOR                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_JUNGLE_DOOR                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_JUNGLE_FENCE                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_JUNGLE_FENCE_GATE            ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_LADDER                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_LEAVES                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_LEVER                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_LILY_PAD                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_MELON_STEM                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_MOB_SPAWNER                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_NETHER_PORTAL                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_NETHER_WART                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_NEW_LEAVES                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_RED_SANDSTONE_SLAB           ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_PISTON                       ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_PISTON_EXTENSION             ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_PISTON_MOVED_BLOCK           ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_POTATOES                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_POWERED_RAIL                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_PUMPKIN_STEM                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_RAIL                         ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_RED_MUSHROOM                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_REDSTONE_REPEATER_OFF        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_REDSTONE_REPEATER_ON         ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_REDSTONE_TORCH_OFF           ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_REDSTONE_TORCH_ON            ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_REDSTONE_WIRE                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_SAPLING                      ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_SIGN_POST                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_SNOW                         ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_SPRUCE_DOOR                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_SPRUCE_FENCE                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_SPRUCE_FENCE_GATE            ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_STAINED_GLASS                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_STAINED_GLASS_PANE           ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_STANDING_BANNER              ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_STICKY_PISTON                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_STONE_BUTTON                 ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_STONE_PRESSURE_PLATE         ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_STONE_SLAB                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_SUGARCANE                    ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_TALL_GRASS                   ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_TORCH                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_TRAPDOOR                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_TRAPPED_CHEST                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_TRIPWIRE                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_TRIPWIRE_HOOK                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_VINES                        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_WALL_BANNER                  ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_WALLSIGN                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_WOODEN_BUTTON                ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_OAK_DOOR                     ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_WOODEN_PRESSURE_PLATE        ].m_SpreadLightFalloff = 1;
	Info[E_BLOCK_WOODEN_SLAB                  ].m_SpreadLightFalloff = 1;

	// Light in ice and water dissapears faster:
	Info[E_BLOCK_ICE                          ].m_SpreadLightFalloff = 3;
	Info[E_BLOCK_STATIONARY_WATER             ].m_SpreadLightFalloff = 3;
	Info[E_BLOCK_WATER                        ].m_SpreadLightFalloff = 3;

	// Light does not pass through these blocks at all:
	Info[E_BLOCK_ACACIA_WOOD_STAIRS           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_BIRCH_WOOD_STAIRS            ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_BRICK_STAIRS                 ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_COBBLESTONE_STAIRS           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_DARK_OAK_WOOD_STAIRS         ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB    ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_DOUBLE_STONE_SLAB            ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_DOUBLE_WOODEN_SLAB           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_FARMLAND                     ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_JUNGLE_WOOD_STAIRS           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_NETHER_BRICK_STAIRS          ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_OAK_WOOD_STAIRS              ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_PURPUR_DOUBLE_SLAB           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_PURPUR_SLAB                  ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_PURPUR_STAIRS                ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_QUARTZ_STAIRS                ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_RED_SANDSTONE_SLAB           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_RED_SANDSTONE_STAIRS         ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_SANDSTONE_STAIRS             ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_SPRUCE_WOOD_STAIRS           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_STONE_BRICK_STAIRS           ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_STONE_SLAB                   ].m_SpreadLightFalloff = 15;
	Info[E_BLOCK_WOODEN_SLAB                  ].m_SpreadLightFalloff = 15;

	// Transparent blocks

	Info[E_BLOCK_ACACIA_DOOR                  ].m_Transparent = true;
	Info[E_BLOCK_ACACIA_FENCE                 ].m_Transparent = true;
	Info[E_BLOCK_ACACIA_FENCE_GATE            ].m_Transparent = true;
	Info[E_BLOCK_ACACIA_WOOD_STAIRS           ].m_Transparent = true;
	Info[E_BLOCK_ACTIVATOR_RAIL               ].m_Transparent = true;
	Info[E_BLOCK_ACTIVE_COMPARATOR            ].m_Transparent = true;
	Info[E_BLOCK_AIR                          ].m_Transparent = true;
	Info[E_BLOCK_ANVIL                        ].m_Transparent = true;
	Info[E_BLOCK_BARRIER                      ].m_Transparent = true;
	Info[E_BLOCK_BEACON                       ].m_Transparent = true;
	Info[E_BLOCK_BED                          ].m_Transparent = true;
	Info[E_BLOCK_BIG_FLOWER                   ].m_Transparent = true;
	Info[E_BLOCK_BIRCH_DOOR                   ].m_Transparent = true;
	Info[E_BLOCK_BIRCH_FENCE                  ].m_Transparent = true;
	Info[E_BLOCK_BIRCH_FENCE_GATE             ].m_Transparent = true;
	Info[E_BLOCK_BIRCH_WOOD_STAIRS            ].m_Transparent = true;
	Info[E_BLOCK_BLACK_SHULKER_BOX            ].m_Transparent = true;
	Info[E_BLOCK_BLOCK_OF_REDSTONE            ].m_Transparent = true;
	Info[E_BLOCK_BLUE_SHULKER_BOX             ].m_Transparent = true;
	Info[E_BLOCK_BREWING_STAND                ].m_Transparent = true;
	Info[E_BLOCK_BRICK_STAIRS                 ].m_Transparent = true;
	Info[E_BLOCK_BROWN_MUSHROOM               ].m_Transparent = true;
	Info[E_BLOCK_BROWN_SHULKER_BOX            ].m_Transparent = true;
	Info[E_BLOCK_CACTUS                       ].m_Transparent = true;
	Info[E_BLOCK_CAKE                         ].m_Transparent = true;
	Info[E_BLOCK_CARPET                       ].m_Transparent = true;
	Info[E_BLOCK_CARROTS                      ].m_Transparent = true;
	Info[E_BLOCK_CAULDRON                     ].m_Transparent = true;
	Info[E_BLOCK_CHEST                        ].m_Transparent = true;
	Info[E_BLOCK_COBBLESTONE_STAIRS           ].m_Transparent = true;
	Info[E_BLOCK_COBBLESTONE_WALL             ].m_Transparent = true;
	Info[E_BLOCK_COBWEB                       ].m_Transparent = true;
	Info[E_BLOCK_COCOA_POD                    ].m_Transparent = true;
	Info[E_BLOCK_CROPS                        ].m_Transparent = true;
	Info[E_BLOCK_CYAN_SHULKER_BOX             ].m_Transparent = true;
	Info[E_BLOCK_DANDELION                    ].m_Transparent = true;
	Info[E_BLOCK_DARK_OAK_DOOR                ].m_Transparent = true;
	Info[E_BLOCK_DARK_OAK_FENCE               ].m_Transparent = true;
	Info[E_BLOCK_DARK_OAK_FENCE_GATE          ].m_Transparent = true;
	Info[E_BLOCK_DARK_OAK_WOOD_STAIRS         ].m_Transparent = true;
	Info[E_BLOCK_DAYLIGHT_SENSOR              ].m_Transparent = true;
	Info[E_BLOCK_DEAD_BUSH                    ].m_Transparent = true;
	Info[E_BLOCK_DETECTOR_RAIL                ].m_Transparent = true;
	Info[E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB    ].m_Transparent = true;
	Info[E_BLOCK_DOUBLE_STONE_SLAB            ].m_Transparent = true;
	Info[E_BLOCK_DOUBLE_WOODEN_SLAB           ].m_Transparent = true;
	Info[E_BLOCK_DRAGON_EGG                   ].m_Transparent = true;
	Info[E_BLOCK_ENCHANTMENT_TABLE            ].m_Transparent = true;
	Info[E_BLOCK_END_PORTAL                   ].m_Transparent = true;
	Info[E_BLOCK_END_PORTAL_FRAME             ].m_Transparent = true;
	Info[E_BLOCK_END_ROD                      ].m_Transparent = true;
	Info[E_BLOCK_ENDER_CHEST                  ].m_Transparent = true;
	Info[E_BLOCK_FARMLAND                     ].m_Transparent = true;
	Info[E_BLOCK_FENCE                        ].m_Transparent = true;
	Info[E_BLOCK_FIRE                         ].m_Transparent = true;
	Info[E_BLOCK_FLOWER                       ].m_Transparent = true;
	Info[E_BLOCK_FLOWER_POT                   ].m_Transparent = true;
	Info[E_BLOCK_GLASS                        ].m_Transparent = true;
	Info[E_BLOCK_GLASS_PANE                   ].m_Transparent = true;
	Info[E_BLOCK_GLOWSTONE                    ].m_Transparent = true;
	Info[E_BLOCK_GRAY_SHULKER_BOX             ].m_Transparent = true;
	Info[E_BLOCK_GREEN_SHULKER_BOX            ].m_Transparent = true;
	Info[E_BLOCK_HEAD                         ].m_Transparent = true;
	Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_Transparent = true;
	Info[E_BLOCK_HOPPER                       ].m_Transparent = true;
	Info[E_BLOCK_ICE                          ].m_Transparent = true;
	Info[E_BLOCK_INACTIVE_COMPARATOR          ].m_Transparent = true;
	Info[E_BLOCK_INVERTED_DAYLIGHT_SENSOR     ].m_Transparent = true;
	Info[E_BLOCK_IRON_BARS                    ].m_Transparent = true;
	Info[E_BLOCK_IRON_DOOR                    ].m_Transparent = true;
	Info[E_BLOCK_IRON_TRAPDOOR                ].m_Transparent = true;
	Info[E_BLOCK_JACK_O_LANTERN               ].m_Transparent = true;
	Info[E_BLOCK_JUNGLE_DOOR                  ].m_Transparent = true;
	Info[E_BLOCK_JUNGLE_FENCE                 ].m_Transparent = true;
	Info[E_BLOCK_JUNGLE_FENCE_GATE            ].m_Transparent = true;
	Info[E_BLOCK_JUNGLE_WOOD_STAIRS           ].m_Transparent = true;
	Info[E_BLOCK_LADDER                       ].m_Transparent = true;
	Info[E_BLOCK_LAVA                         ].m_Transparent = true;
	Info[E_BLOCK_LEAVES                       ].m_Transparent = true;
	Info[E_BLOCK_LEVER                        ].m_Transparent = true;
	Info[E_BLOCK_LIGHT_BLUE_SHULKER_BOX       ].m_Transparent = true;
	Info[E_BLOCK_LIGHT_GRAY_SHULKER_BOX       ].m_Transparent = true;
	Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_Transparent = true;
	Info[E_BLOCK_LILY_PAD                     ].m_Transparent = true;
	Info[E_BLOCK_LIME_SHULKER_BOX             ].m_Transparent = true;
	Info[E_BLOCK_LIT_FURNACE                  ].m_Transparent = true;
	Info[E_BLOCK_MAGENTA_SHULKER_BOX          ].m_Transparent = true;
	Info[E_BLOCK_MELON_STEM                   ].m_Transparent = true;
	Info[E_BLOCK_MOB_SPAWNER                  ].m_Transparent = true;
	Info[E_BLOCK_NETHER_BRICK_FENCE           ].m_Transparent = true;
	Info[E_BLOCK_NETHER_BRICK_STAIRS          ].m_Transparent = true;
	Info[E_BLOCK_NETHER_PORTAL                ].m_Transparent = true;
	Info[E_BLOCK_NETHER_WART                  ].m_Transparent = true;
	Info[E_BLOCK_NEW_LEAVES                   ].m_Transparent = true;
	Info[E_BLOCK_OAK_DOOR                     ].m_Transparent = true;
	Info[E_BLOCK_OAK_FENCE_GATE               ].m_Transparent = true;
	Info[E_BLOCK_OAK_WOOD_STAIRS              ].m_Transparent = true;
	Info[E_BLOCK_ORANGE_SHULKER_BOX           ].m_Transparent = true;
	Info[E_BLOCK_PINK_SHULKER_BOX             ].m_Transparent = true;
	Info[E_BLOCK_PISTON                       ].m_Transparent = true;
	Info[E_BLOCK_PISTON_EXTENSION             ].m_Transparent = true;
	Info[E_BLOCK_PISTON_MOVED_BLOCK           ].m_Transparent = true;
	Info[E_BLOCK_POTATOES                     ].m_Transparent = true;
	Info[E_BLOCK_POWERED_RAIL                 ].m_Transparent = true;
	Info[E_BLOCK_PUMPKIN_STEM                 ].m_Transparent = true;
	Info[E_BLOCK_PURPLE_SHULKER_BOX           ].m_Transparent = true;
	Info[E_BLOCK_PURPUR_DOUBLE_SLAB           ].m_Transparent = true;
	Info[E_BLOCK_PURPUR_SLAB                  ].m_Transparent = true;
	Info[E_BLOCK_PURPUR_STAIRS                ].m_Transparent = true;
	Info[E_BLOCK_QUARTZ_STAIRS                ].m_Transparent = true;
	Info[E_BLOCK_RAIL                         ].m_Transparent = true;
	Info[E_BLOCK_RED_MUSHROOM                 ].m_Transparent = true;
	Info[E_BLOCK_RED_SANDSTONE_SLAB           ].m_Transparent = true;
	Info[E_BLOCK_RED_SANDSTONE_STAIRS         ].m_Transparent = true;
	Info[E_BLOCK_RED_SHULKER_BOX              ].m_Transparent = true;
	Info[E_BLOCK_REDSTONE_ORE_GLOWING         ].m_Transparent = true;
	Info[E_BLOCK_REDSTONE_REPEATER_OFF        ].m_Transparent = true;
	Info[E_BLOCK_REDSTONE_REPEATER_ON         ].m_Transparent = true;
	Info[E_BLOCK_REDSTONE_TORCH_OFF           ].m_Transparent = true;
	Info[E_BLOCK_REDSTONE_TORCH_ON            ].m_Transparent = true;
	Info[E_BLOCK_REDSTONE_WIRE                ].m_Transparent = true;
	Info[E_BLOCK_SANDSTONE_STAIRS             ].m_Transparent = true;
	Info[E_BLOCK_SAPLING                      ].m_Transparent = true;
	Info[E_BLOCK_SEA_LANTERN                  ].m_Transparent = true;
	Info[E_BLOCK_SIGN_POST                    ].m_Transparent = true;
	Info[E_BLOCK_SNOW                         ].m_Transparent = true;
	Info[E_BLOCK_SPRUCE_DOOR                  ].m_Transparent = true;
	Info[E_BLOCK_SPRUCE_FENCE                 ].m_Transparent = true;
	Info[E_BLOCK_SPRUCE_FENCE_GATE            ].m_Transparent = true;
	Info[E_BLOCK_SPRUCE_WOOD_STAIRS           ].m_Transparent = true;
	Info[E_BLOCK_STAINED_GLASS                ].m_Transparent = true;
	Info[E_BLOCK_STAINED_GLASS_PANE           ].m_Transparent = true;
	Info[E_BLOCK_STANDING_BANNER              ].m_Transparent = true;
	Info[E_BLOCK_STATIONARY_LAVA              ].m_Transparent = true;
	Info[E_BLOCK_STATIONARY_WATER             ].m_Transparent = true;
	Info[E_BLOCK_STICKY_PISTON                ].m_Transparent = true;
	Info[E_BLOCK_STONE_BRICK_STAIRS           ].m_Transparent = true;
	Info[E_BLOCK_STONE_BUTTON                 ].m_Transparent = true;
	Info[E_BLOCK_STONE_PRESSURE_PLATE         ].m_Transparent = true;
	Info[E_BLOCK_STONE_SLAB                   ].m_Transparent = true;
	Info[E_BLOCK_SUGARCANE                    ].m_Transparent = true;
	Info[E_BLOCK_TALL_GRASS                   ].m_Transparent = true;
	Info[E_BLOCK_TORCH                        ].m_Transparent = true;
	Info[E_BLOCK_TRAPDOOR                     ].m_Transparent = true;
	Info[E_BLOCK_TRAPPED_CHEST                ].m_Transparent = true;
	Info[E_BLOCK_TRIPWIRE                     ].m_Transparent = true;
	Info[E_BLOCK_TRIPWIRE_HOOK                ].m_Transparent = true;
	Info[E_BLOCK_VINES                        ].m_Transparent = true;
	Info[E_BLOCK_WALL_BANNER                  ].m_Transparent = true;
	Info[E_BLOCK_WALLSIGN                     ].m_Transparent = true;
	Info[E_BLOCK_WATER                        ].m_Transparent = true;
	Info[E_BLOCK_WHITE_SHULKER_BOX            ].m_Transparent = true;
	Info[E_BLOCK_WOODEN_BUTTON                ].m_Transparent = true;
	Info[E_BLOCK_WOODEN_PRESSURE_PLATE        ].m_Transparent = true;
	Info[E_BLOCK_WOODEN_SLAB                  ].m_Transparent = true;
	Info[E_BLOCK_YELLOW_SHULKER_BOX           ].m_Transparent = true;


	// Skylight dispersant blocks:
	Info[E_BLOCK_COBWEB                       ].m_IsSkylightDispersant = true;
	Info[E_BLOCK_LEAVES                       ].m_IsSkylightDispersant = true;


	// One hit break blocks:
	Info[E_BLOCK_ACTIVE_COMPARATOR            ].m_OneHitDig = true;
	Info[E_BLOCK_BEETROOTS                    ].m_OneHitDig = true;
	Info[E_BLOCK_BIG_FLOWER                   ].m_OneHitDig = true;
	Info[E_BLOCK_BROWN_MUSHROOM               ].m_OneHitDig = true;
	Info[E_BLOCK_CARROTS                      ].m_OneHitDig = true;
	Info[E_BLOCK_CROPS                        ].m_OneHitDig = true;
	Info[E_BLOCK_DANDELION                    ].m_OneHitDig = true;
	Info[E_BLOCK_DEAD_BUSH                    ].m_OneHitDig = true;
	Info[E_BLOCK_END_ROD                      ].m_OneHitDig = true;
	Info[E_BLOCK_FIRE                         ].m_OneHitDig = true;
	Info[E_BLOCK_FLOWER                       ].m_OneHitDig = true;
	Info[E_BLOCK_FLOWER_POT                   ].m_OneHitDig = true;
	Info[E_BLOCK_INACTIVE_COMPARATOR          ].m_OneHitDig = true;
	Info[E_BLOCK_LILY_PAD                     ].m_OneHitDig = true;
	Info[E_BLOCK_MELON_STEM                   ].m_OneHitDig = true;
	Info[E_BLOCK_NETHER_WART                  ].m_OneHitDig = true;
	Info[E_BLOCK_POTATOES                     ].m_OneHitDig = true;
	Info[E_BLOCK_PUMPKIN_STEM                 ].m_OneHitDig = true;
	Info[E_BLOCK_REDSTONE_REPEATER_OFF        ].m_OneHitDig = true;
	Info[E_BLOCK_REDSTONE_REPEATER_ON         ].m_OneHitDig = true;
	Info[E_BLOCK_REDSTONE_TORCH_OFF           ].m_OneHitDig = true;
	Info[E_BLOCK_REDSTONE_TORCH_ON            ].m_OneHitDig = true;
	Info[E_BLOCK_REDSTONE_WIRE                ].m_OneHitDig = true;
	Info[E_BLOCK_RED_MUSHROOM                 ].m_OneHitDig = true;
	Info[E_BLOCK_REEDS                        ].m_OneHitDig = true;
	Info[E_BLOCK_SAPLING                      ].m_OneHitDig = true;
	Info[E_BLOCK_SLIME_BLOCK                  ].m_OneHitDig = true;
	Info[E_BLOCK_TNT                          ].m_OneHitDig = true;
	Info[E_BLOCK_TALL_GRASS                   ].m_OneHitDig = true;
	Info[E_BLOCK_TORCH                        ].m_OneHitDig = true;
	Info[E_BLOCK_TRIPWIRE_HOOK                ].m_OneHitDig = true;
	Info[E_BLOCK_TRIPWIRE                     ].m_OneHitDig = true;


	// Blocks that break when pushed by piston:
	Info[E_BLOCK_ACACIA_DOOR                  ].m_PistonBreakable = true;
	Info[E_BLOCK_ACTIVE_COMPARATOR            ].m_PistonBreakable = true;
	Info[E_BLOCK_AIR                          ].m_PistonBreakable = true;
	Info[E_BLOCK_BED                          ].m_PistonBreakable = true;
	Info[E_BLOCK_BEETROOTS                    ].m_PistonBreakable = true;
	Info[E_BLOCK_BIG_FLOWER                   ].m_PistonBreakable = true;
	Info[E_BLOCK_BIRCH_DOOR                   ].m_PistonBreakable = true;
	Info[E_BLOCK_BLACK_SHULKER_BOX            ].m_PistonBreakable = true;
	Info[E_BLOCK_BLUE_SHULKER_BOX             ].m_PistonBreakable = true;
	Info[E_BLOCK_BROWN_MUSHROOM               ].m_PistonBreakable = true;
	Info[E_BLOCK_BROWN_SHULKER_BOX            ].m_PistonBreakable = true;
	Info[E_BLOCK_CACTUS                       ].m_PistonBreakable = true;
	Info[E_BLOCK_CAKE                         ].m_PistonBreakable = true;
	Info[E_BLOCK_CARROTS                      ].m_PistonBreakable = true;
	Info[E_BLOCK_CHORUS_FLOWER                ].m_PistonBreakable = true;
	Info[E_BLOCK_CHORUS_PLANT                 ].m_PistonBreakable = true;
	Info[E_BLOCK_COCOA_POD                    ].m_PistonBreakable = true;
	Info[E_BLOCK_COBWEB                       ].m_PistonBreakable = true;
	Info[E_BLOCK_CROPS                        ].m_PistonBreakable = true;
	Info[E_BLOCK_CYAN_SHULKER_BOX             ].m_PistonBreakable = true;
	Info[E_BLOCK_DANDELION                    ].m_PistonBreakable = true;
	Info[E_BLOCK_DARK_OAK_DOOR                ].m_PistonBreakable = true;
	Info[E_BLOCK_DEAD_BUSH                    ].m_PistonBreakable = true;
	Info[E_BLOCK_DRAGON_EGG                   ].m_PistonBreakable = true;
	Info[E_BLOCK_FIRE                         ].m_PistonBreakable = true;
	Info[E_BLOCK_FLOWER                       ].m_PistonBreakable = true;
	Info[E_BLOCK_FLOWER_POT                   ].m_PistonBreakable = true;
	Info[E_BLOCK_GRAY_SHULKER_BOX             ].m_PistonBreakable = true;
	Info[E_BLOCK_GREEN_SHULKER_BOX            ].m_PistonBreakable = true;
	Info[E_BLOCK_HEAD                         ].m_PistonBreakable = true;
	Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_PistonBreakable = true;
	Info[E_BLOCK_INACTIVE_COMPARATOR          ].m_PistonBreakable = true;
	Info[E_BLOCK_IRON_DOOR                    ].m_PistonBreakable = true;
	Info[E_BLOCK_IRON_TRAPDOOR                ].m_PistonBreakable = true;
	Info[E_BLOCK_JACK_O_LANTERN               ].m_PistonBreakable = true;
	Info[E_BLOCK_JUNGLE_DOOR                  ].m_PistonBreakable = true;
	Info[E_BLOCK_LIGHT_BLUE_SHULKER_BOX       ].m_PistonBreakable = true;
	Info[E_BLOCK_LIGHT_GRAY_SHULKER_BOX       ].m_PistonBreakable = true;
	Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_PistonBreakable = true;
	Info[E_BLOCK_LILY_PAD                     ].m_PistonBreakable = true;
	Info[E_BLOCK_LIME_SHULKER_BOX             ].m_PistonBreakable = true;
	Info[E_BLOCK_LADDER                       ].m_PistonBreakable = true;
	Info[E_BLOCK_LAVA                         ].m_PistonBreakable = true;
	Info[E_BLOCK_LEAVES                       ].m_PistonBreakable = true;
	Info[E_BLOCK_LEVER                        ].m_PistonBreakable = true;
	Info[E_BLOCK_MAGENTA_SHULKER_BOX          ].m_PistonBreakable = true;
	Info[E_BLOCK_MELON                        ].m_PistonBreakable = true;
	Info[E_BLOCK_MELON_STEM                   ].m_PistonBreakable = true;
	Info[E_BLOCK_NETHER_WART                  ].m_PistonBreakable = true;
	Info[E_BLOCK_OAK_DOOR                     ].m_PistonBreakable = true;
	Info[E_BLOCK_ORANGE_SHULKER_BOX           ].m_PistonBreakable = true;
	Info[E_BLOCK_PINK_SHULKER_BOX             ].m_PistonBreakable = true;
	Info[E_BLOCK_POTATOES                     ].m_PistonBreakable = true;
	Info[E_BLOCK_PUMPKIN                      ].m_PistonBreakable = true;
	Info[E_BLOCK_PUMPKIN_STEM                 ].m_PistonBreakable = true;
	Info[E_BLOCK_PURPLE_SHULKER_BOX           ].m_PistonBreakable = true;
	Info[E_BLOCK_REDSTONE_REPEATER_OFF        ].m_PistonBreakable = true;
	Info[E_BLOCK_REDSTONE_REPEATER_ON         ].m_PistonBreakable = true;
	Info[E_BLOCK_REDSTONE_TORCH_OFF           ].m_PistonBreakable = true;
	Info[E_BLOCK_REDSTONE_TORCH_ON            ].m_PistonBreakable = true;
	Info[E_BLOCK_REDSTONE_WIRE                ].m_PistonBreakable = true;
	Info[E_BLOCK_RED_MUSHROOM                 ].m_PistonBreakable = true;
	Info[E_BLOCK_RED_SHULKER_BOX              ].m_PistonBreakable = true;
	Info[E_BLOCK_REEDS                        ].m_PistonBreakable = true;
	Info[E_BLOCK_SAPLING                      ].m_PistonBreakable = true;
	Info[E_BLOCK_SIGN_POST                    ].m_PistonBreakable = true;
	Info[E_BLOCK_SNOW                         ].m_PistonBreakable = true;
	Info[E_BLOCK_SPRUCE_DOOR                  ].m_PistonBreakable = true;
	Info[E_BLOCK_STATIONARY_LAVA              ].m_PistonBreakable = true;
	Info[E_BLOCK_STATIONARY_WATER             ].m_PistonBreakable = true;
	Info[E_BLOCK_STONE_BUTTON                 ].m_PistonBreakable = true;
	Info[E_BLOCK_STONE_PRESSURE_PLATE         ].m_PistonBreakable = true;
	Info[E_BLOCK_TALL_GRASS                   ].m_PistonBreakable = true;
	Info[E_BLOCK_TORCH                        ].m_PistonBreakable = true;
	Info[E_BLOCK_TRAPDOOR                     ].m_PistonBreakable = true;
	Info[E_BLOCK_TRIPWIRE                     ].m_PistonBreakable = true;
	Info[E_BLOCK_TRIPWIRE_HOOK                ].m_PistonBreakable = true;
	Info[E_BLOCK_VINES                        ].m_PistonBreakable = true;
	Info[E_BLOCK_WALLSIGN                     ].m_PistonBreakable = true;
	Info[E_BLOCK_WATER                        ].m_PistonBreakable = true;
	Info[E_BLOCK_WHEAT                        ].m_PistonBreakable = true;
	Info[E_BLOCK_WHITE_SHULKER_BOX            ].m_PistonBreakable = true;
	Info[E_BLOCK_WOODEN_BUTTON                ].m_PistonBreakable = true;
	Info[E_BLOCK_WOODEN_PRESSURE_PLATE        ].m_PistonBreakable = true;
	Info[E_BLOCK_YELLOW_SHULKER_BOX           ].m_PistonBreakable = true;


	/* Blocks that block rain or snow's passage:
	*  All solid blocks are also rain blockers, and they are set automatically
	*  at the end of this function.
	*/
	Info[E_BLOCK_SIGN_POST                    ].m_IsRainBlocker = true;
	Info[E_BLOCK_WALLSIGN                     ].m_IsRainBlocker = true;
	Info[E_BLOCK_WALL_BANNER                  ].m_IsRainBlocker = true;
	Info[E_BLOCK_STANDING_BANNER              ].m_IsRainBlocker = true;


	// Nonsolid blocks:
	Info[E_BLOCK_ACTIVATOR_RAIL               ].m_IsSolid = false;
	Info[E_BLOCK_ACTIVE_COMPARATOR            ].m_IsSolid = false;
	Info[E_BLOCK_AIR                          ].m_IsSolid = false;
	Info[E_BLOCK_BIG_FLOWER                   ].m_IsSolid = false;
	Info[E_BLOCK_BROWN_MUSHROOM               ].m_IsSolid = false;
	Info[E_BLOCK_CARPET                       ].m_IsSolid = false;
	Info[E_BLOCK_CARROTS                      ].m_IsSolid = false;
	Info[E_BLOCK_CHORUS_FLOWER                ].m_IsSolid = false;
	Info[E_BLOCK_CHORUS_PLANT                 ].m_IsSolid = false;
	Info[E_BLOCK_COBWEB                       ].m_IsSolid = false;
	Info[E_BLOCK_CROPS                        ].m_IsSolid = false;
	Info[E_BLOCK_DANDELION                    ].m_IsSolid = false;
	Info[E_BLOCK_DEAD_BUSH                    ].m_IsSolid = false;
	Info[E_BLOCK_DETECTOR_RAIL                ].m_IsSolid = false;
	Info[E_BLOCK_END_GATEWAY                  ].m_IsSolid = false;
	Info[E_BLOCK_END_PORTAL                   ].m_IsSolid = false;
	Info[E_BLOCK_END_ROD                      ].m_IsSolid = false;
	Info[E_BLOCK_FIRE                         ].m_IsSolid = false;
	Info[E_BLOCK_FLOWER                       ].m_IsSolid = false;
	Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_IsSolid = false;
	Info[E_BLOCK_INACTIVE_COMPARATOR          ].m_IsSolid = false;
	Info[E_BLOCK_LADDER                       ].m_IsSolid = false;
	Info[E_BLOCK_LAVA                         ].m_IsSolid = false;
	Info[E_BLOCK_LEVER                        ].m_IsSolid = false;
	Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_IsSolid = false;
	Info[E_BLOCK_MELON_STEM                   ].m_IsSolid = false;
	Info[E_BLOCK_NETHER_PORTAL                ].m_IsSolid = false;
	Info[E_BLOCK_POTATOES                     ].m_IsSolid = false;
	Info[E_BLOCK_POWERED_RAIL                 ].m_IsSolid = false;
	Info[E_BLOCK_RAIL                         ].m_IsSolid = false;
	Info[E_BLOCK_RED_MUSHROOM                 ].m_IsSolid = false;
	Info[E_BLOCK_REDSTONE_REPEATER_OFF        ].m_IsSolid = false;
	Info[E_BLOCK_REDSTONE_REPEATER_ON         ].m_IsSolid = false;
	Info[E_BLOCK_REDSTONE_TORCH_OFF           ].m_IsSolid = false;
	Info[E_BLOCK_REDSTONE_TORCH_ON            ].m_IsSolid = false;
	Info[E_BLOCK_REDSTONE_WIRE                ].m_IsSolid = false;
	Info[E_BLOCK_REEDS                        ].m_IsSolid = false;
	Info[E_BLOCK_SAPLING                      ].m_IsSolid = false;
	Info[E_BLOCK_SIGN_POST                    ].m_IsSolid = false;
	Info[E_BLOCK_SNOW                         ].m_IsSolid = false;
	Info[E_BLOCK_STANDING_BANNER              ].m_IsSolid = false;
	Info[E_BLOCK_STATIONARY_LAVA              ].m_IsSolid = false;
	Info[E_BLOCK_STATIONARY_WATER             ].m_IsSolid = false;
	Info[E_BLOCK_STONE_BUTTON                 ].m_IsSolid = false;
	Info[E_BLOCK_STONE_PRESSURE_PLATE         ].m_IsSolid = false;
	Info[E_BLOCK_TALL_GRASS                   ].m_IsSolid = false;
	Info[E_BLOCK_TORCH                        ].m_IsSolid = false;
	Info[E_BLOCK_TRIPWIRE                     ].m_IsSolid = false;
	Info[E_BLOCK_TRIPWIRE_HOOK                ].m_IsSolid = false;
	Info[E_BLOCK_VINES                        ].m_IsSolid = false;
	Info[E_BLOCK_WALL_BANNER                  ].m_IsSolid = false;
	Info[E_BLOCK_WALLSIGN                     ].m_IsSolid = false;
	Info[E_BLOCK_WATER                        ].m_IsSolid = false;
	Info[E_BLOCK_WOODEN_BUTTON                ].m_IsSolid = false;
	Info[E_BLOCK_WOODEN_PRESSURE_PLATE        ].m_IsSolid = false;


	// Blocks, which a spectator is allowed to interact with
	Info[E_BLOCK_BEACON                       ].m_UseableBySpectator = true;
	Info[E_BLOCK_BREWING_STAND                ].m_UseableBySpectator = true;
	Info[E_BLOCK_CHEST                        ].m_UseableBySpectator = true;
	Info[E_BLOCK_DISPENSER                    ].m_UseableBySpectator = true;
	Info[E_BLOCK_DROPPER                      ].m_UseableBySpectator = true;
	Info[E_BLOCK_HOPPER                       ].m_UseableBySpectator = true;


	// Blocks that fully occupy their voxel - used as a guide for torch placeable blocks, amongst other things:
	Info[E_BLOCK_BARRIER                      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BEDROCK                      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BLOCK_OF_COAL                ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BLOCK_OF_REDSTONE            ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BONE_BLOCK                   ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BOOKCASE                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BRICK                        ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_CHAIN_COMMAND_BLOCK          ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_CLAY                         ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_COAL_ORE                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_COBBLESTONE                  ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_COMMAND_BLOCK                ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_CONCRETE                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_CONCRETE_POWDER              ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_CRAFTING_TABLE               ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DIAMOND_BLOCK                ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DIAMOND_ORE                  ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DIRT                         ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DISPENSER                    ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB    ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DOUBLE_STONE_SLAB            ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DOUBLE_WOODEN_SLAB           ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_DROPPER                      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_EMERALD_BLOCK                ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_EMERALD_ORE                  ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_END_BRICKS                   ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_END_STONE                    ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_FROSTED_ICE                  ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_FURNACE                      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_GLOWSTONE                    ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_GOLD_BLOCK                   ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_GOLD_ORE                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_WHITE_GLAZED_TERRACOTTA      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_ORANGE_GLAZED_TERRACOTTA     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_MAGENTA_GLAZED_TERRACOTTA    ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_YELLOW_GLAZED_TERRACOTTA     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_LIME_GLAZED_TERRACOTTA       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PINK_GLAZED_TERRACOTTA       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_GRAY_GLAZED_TERRACOTTA       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_CYAN_GLAZED_TERRACOTTA       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PURPLE_GLAZED_TERRACOTTA     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BLUE_GLAZED_TERRACOTTA       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BROWN_GLAZED_TERRACOTTA      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_GREEN_GLAZED_TERRACOTTA      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_BLACK_GLAZED_TERRACOTTA      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_RED_GLAZED_TERRACOTTA        ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_GRASS                        ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_GRAVEL                       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_HARDENED_CLAY                ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_HAY_BALE                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_HUGE_BROWN_MUSHROOM          ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_HUGE_RED_MUSHROOM            ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_ICE                          ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_IRON_BLOCK                   ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_IRON_ORE                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_JACK_O_LANTERN               ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_JUKEBOX                      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_LAPIS_BLOCK                  ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_LAPIS_ORE                    ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_LOG                          ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_MAGMA                        ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_MELON                        ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_MOB_SPAWNER                  ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_MOSSY_COBBLESTONE            ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_MYCELIUM                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_NETHERRACK                   ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_NETHER_BRICK                 ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_NETHER_QUARTZ_ORE            ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_NETHER_WART_BLOCK            ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_NEW_LOG                      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_NOTE_BLOCK                   ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_OBSERVER                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_OBSIDIAN                     ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PACKED_ICE                   ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PLANKS                       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PRISMARINE_BLOCK             ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PUMPKIN                      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PURPUR_BLOCK                 ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PURPUR_PILLAR                ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_PURPUR_DOUBLE_SLAB           ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_QUARTZ_BLOCK                 ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_RED_NETHER_BRICK             ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_RED_SANDSTONE                ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_REDSTONE_LAMP_OFF            ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_REDSTONE_LAMP_ON             ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_REDSTONE_ORE                 ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_REDSTONE_ORE_GLOWING         ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_REPEATING_COMMAND_BLOCK      ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_SANDSTONE                    ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_SAND                         ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_SILVERFISH_EGG               ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_SPONGE                       ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_STAINED_CLAY                 ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_STONE                        ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_STONE_BRICKS                 ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_STRUCTURE_BLOCK              ].m_FullyOccupiesVoxel = true;
	Info[E_BLOCK_WOOL                         ].m_FullyOccupiesVoxel = true;


	// Blocks that can be terraformed
	Info[E_BLOCK_COAL_ORE                     ].m_CanBeTerraformed = true;
	Info[E_BLOCK_COBBLESTONE                  ].m_CanBeTerraformed = true;
	Info[E_BLOCK_DIAMOND_ORE                  ].m_CanBeTerraformed = true;
	Info[E_BLOCK_DIRT                         ].m_CanBeTerraformed = true;
	Info[E_BLOCK_GOLD_ORE                     ].m_CanBeTerraformed = true;
	Info[E_BLOCK_GRASS                        ].m_CanBeTerraformed = true;
	Info[E_BLOCK_GRAVEL                       ].m_CanBeTerraformed = true;
	Info[E_BLOCK_HARDENED_CLAY                ].m_CanBeTerraformed = true;
	Info[E_BLOCK_IRON_ORE                     ].m_CanBeTerraformed = true;
	Info[E_BLOCK_MYCELIUM                     ].m_CanBeTerraformed = true;
	Info[E_BLOCK_NETHERRACK                   ].m_CanBeTerraformed = true;
	Info[E_BLOCK_REDSTONE_ORE                 ].m_CanBeTerraformed = true;
	Info[E_BLOCK_REDSTONE_ORE_GLOWING         ].m_CanBeTerraformed = true;
	Info[E_BLOCK_SAND                         ].m_CanBeTerraformed = true;
	Info[E_BLOCK_SANDSTONE                    ].m_CanBeTerraformed = true;
	Info[E_BLOCK_SOULSAND                     ].m_CanBeTerraformed = true;
	Info[E_BLOCK_STAINED_CLAY                 ].m_CanBeTerraformed = true;
	Info[E_BLOCK_STONE                        ].m_CanBeTerraformed = true;


	// Block heights:
	Info[E_BLOCK_BED                          ].m_BlockHeight = 0.5625;  // 9 pixels
	Info[E_BLOCK_CAKE                         ].m_BlockHeight = 0.5;     // 8 pixels
	Info[E_BLOCK_ENCHANTMENT_TABLE            ].m_BlockHeight = 0.75;    // 12 pixels
	Info[E_BLOCK_PURPUR_SLAB                  ].m_BlockHeight = 0.5;
	Info[E_BLOCK_RED_SANDSTONE_SLAB           ].m_BlockHeight = 0.5;
	Info[E_BLOCK_STONE_SLAB                   ].m_BlockHeight = 0.5;
	Info[E_BLOCK_WOODEN_SLAB                  ].m_BlockHeight = 0.5;
	Info[E_BLOCK_SNOW                         ].m_BlockHeight = 0.125;   // one layer is 1 / 8 (2 pixels) tall
	Info[E_BLOCK_ACACIA_FENCE                 ].m_BlockHeight = 1.5;
	Info[E_BLOCK_ACACIA_FENCE_GATE            ].m_BlockHeight = 1.5;
	Info[E_BLOCK_BIRCH_FENCE                  ].m_BlockHeight = 1.5;
	Info[E_BLOCK_BIRCH_FENCE_GATE             ].m_BlockHeight = 1.5;
	Info[E_BLOCK_DARK_OAK_FENCE               ].m_BlockHeight = 1.5;
	Info[E_BLOCK_DARK_OAK_FENCE_GATE          ].m_BlockHeight = 1.5;
	Info[E_BLOCK_FENCE                        ].m_BlockHeight = 1.5;
	Info[E_BLOCK_OAK_FENCE_GATE               ].m_BlockHeight = 1.5;
	Info[E_BLOCK_JUNGLE_FENCE                 ].m_BlockHeight = 1.5;
	Info[E_BLOCK_JUNGLE_FENCE_GATE            ].m_BlockHeight = 1.5;
	Info[E_BLOCK_SPRUCE_FENCE                 ].m_BlockHeight = 1.5;
	Info[E_BLOCK_SPRUCE_FENCE_GATE            ].m_BlockHeight = 1.5;


	// Block hardness:
	Info[E_BLOCK_AIR                          ].m_Hardness = 0.0f;
	Info[E_BLOCK_STONE                        ].m_Hardness = 1.5f;
	Info[E_BLOCK_GRASS                        ].m_Hardness = 0.6f;
	Info[E_BLOCK_DIRT                         ].m_Hardness = 0.5f;
	Info[E_BLOCK_COBBLESTONE                  ].m_Hardness = 2.0f;
	Info[E_BLOCK_PLANKS                       ].m_Hardness = 2.0f;
	Info[E_BLOCK_SAPLING                      ].m_Hardness = 0.0f;
	Info[E_BLOCK_BEDROCK                      ].m_Hardness = -1.0f;
	Info[E_BLOCK_WATER                        ].m_Hardness = 100.0f;
	Info[E_BLOCK_STATIONARY_WATER             ].m_Hardness = 100.0f;
	Info[E_BLOCK_LAVA                         ].m_Hardness = 100.0f;
	Info[E_BLOCK_STATIONARY_LAVA              ].m_Hardness = 100.0f;
	Info[E_BLOCK_SAND                         ].m_Hardness = 0.5f;
	Info[E_BLOCK_GRAVEL                       ].m_Hardness = 0.6f;
	Info[E_BLOCK_GOLD_ORE                     ].m_Hardness = 3.0f;
	Info[E_BLOCK_IRON_ORE                     ].m_Hardness = 3.0f;
	Info[E_BLOCK_COAL_ORE                     ].m_Hardness = 3.0f;
	Info[E_BLOCK_LOG                          ].m_Hardness = 2.0f;
	Info[E_BLOCK_LEAVES                       ].m_Hardness = 0.2f;
	Info[E_BLOCK_SPONGE                       ].m_Hardness = 0.6f;
	Info[E_BLOCK_GLASS                        ].m_Hardness = 0.3f;
	Info[E_BLOCK_LAPIS_ORE                    ].m_Hardness = 3.0f;
	Info[E_BLOCK_LAPIS_BLOCK                  ].m_Hardness = 3.0f;
	Info[E_BLOCK_DISPENSER                    ].m_Hardness = 3.5f;
	Info[E_BLOCK_SANDSTONE                    ].m_Hardness = 0.8f;
	Info[E_BLOCK_NOTE_BLOCK                   ].m_Hardness = 0.8f;
	Info[E_BLOCK_BED                          ].m_Hardness = 0.2f;
	Info[E_BLOCK_POWERED_RAIL                 ].m_Hardness = 0.7f;
	Info[E_BLOCK_DETECTOR_RAIL                ].m_Hardness = 0.7f;
	Info[E_BLOCK_STICKY_PISTON                ].m_Hardness = 0.5f;
	Info[E_BLOCK_COBWEB                       ].m_Hardness = 4.0f;
	Info[E_BLOCK_TALL_GRASS                   ].m_Hardness = 0.0f;
	Info[E_BLOCK_DEAD_BUSH                    ].m_Hardness = 0.0f;
	Info[E_BLOCK_PISTON                       ].m_Hardness = 0.5f;
	Info[E_BLOCK_PISTON_EXTENSION             ].m_Hardness = 0.5f;
	Info[E_BLOCK_WOOL                         ].m_Hardness = 0.8f;
	Info[E_BLOCK_PISTON_MOVED_BLOCK           ].m_Hardness = -1.0f;
	Info[E_BLOCK_DANDELION                    ].m_Hardness = 0.0f;
	Info[E_BLOCK_FLOWER                       ].m_Hardness = 0.0f;
	Info[E_BLOCK_BROWN_MUSHROOM               ].m_Hardness = 0.0f;
	Info[E_BLOCK_RED_MUSHROOM                 ].m_Hardness = 0.0f;
	Info[E_BLOCK_GOLD_BLOCK                   ].m_Hardness = 3.0f;
	Info[E_BLOCK_IRON_BLOCK                   ].m_Hardness = 5.0f;
	Info[E_BLOCK_DOUBLE_STONE_SLAB            ].m_Hardness = 2.0f;
	Info[E_BLOCK_STONE_SLAB                   ].m_Hardness = 2.0f;
	Info[E_BLOCK_BRICK                        ].m_Hardness = 2.0f;
	Info[E_BLOCK_TNT                          ].m_Hardness = 0.0f;
	Info[E_BLOCK_BOOKCASE                     ].m_Hardness = 1.5f;
	Info[E_BLOCK_MOSSY_COBBLESTONE            ].m_Hardness = 2.0f;
	Info[E_BLOCK_OBSIDIAN                     ].m_Hardness = 50.0f;
	Info[E_BLOCK_TORCH                        ].m_Hardness = 0.0f;
	Info[E_BLOCK_FIRE                         ].m_Hardness = 0.0f;
	Info[E_BLOCK_MOB_SPAWNER                  ].m_Hardness = 5.0f;
	Info[E_BLOCK_OAK_WOOD_STAIRS              ].m_Hardness = 2.0f;
	Info[E_BLOCK_CHEST                        ].m_Hardness = 2.5f;
	Info[E_BLOCK_REDSTONE_WIRE                ].m_Hardness = 0.0f;
	Info[E_BLOCK_DIAMOND_ORE                  ].m_Hardness = 3.0f;
	Info[E_BLOCK_DIAMOND_BLOCK                ].m_Hardness = 5.0f;
	Info[E_BLOCK_CRAFTING_TABLE               ].m_Hardness = 2.5f;
	Info[E_BLOCK_CROPS                        ].m_Hardness = 0.0f;
	Info[E_BLOCK_FARMLAND                     ].m_Hardness = 0.6f;
	Info[E_BLOCK_FURNACE                      ].m_Hardness = 3.5f;
	Info[E_BLOCK_LIT_FURNACE                  ].m_Hardness = 3.5f;
	Info[E_BLOCK_SIGN_POST                    ].m_Hardness = 1.0f;
	Info[E_BLOCK_OAK_DOOR                     ].m_Hardness = 3.0f;
	Info[E_BLOCK_LADDER                       ].m_Hardness = 0.4f;
	Info[E_BLOCK_RAIL                         ].m_Hardness = 0.7f;
	Info[E_BLOCK_COBBLESTONE_STAIRS           ].m_Hardness = 2.0f;
	Info[E_BLOCK_WALLSIGN                     ].m_Hardness = 1.0f;
	Info[E_BLOCK_LEVER                        ].m_Hardness = 0.5f;
	Info[E_BLOCK_STONE_PRESSURE_PLATE         ].m_Hardness = 0.5f;
	Info[E_BLOCK_IRON_DOOR                    ].m_Hardness = 5.0f;
	Info[E_BLOCK_WOODEN_PRESSURE_PLATE        ].m_Hardness = 0.5f;
	Info[E_BLOCK_REDSTONE_ORE                 ].m_Hardness = 3.0f;
	Info[E_BLOCK_REDSTONE_ORE_GLOWING         ].m_Hardness = 0.625f;
	Info[E_BLOCK_REDSTONE_TORCH_OFF           ].m_Hardness = 0.0f;
	Info[E_BLOCK_REDSTONE_TORCH_ON            ].m_Hardness = 0.0f;
	Info[E_BLOCK_STONE_BUTTON                 ].m_Hardness = 0.5f;
	Info[E_BLOCK_SNOW                         ].m_Hardness = 0.1f;
	Info[E_BLOCK_ICE                          ].m_Hardness = 0.5f;
	Info[E_BLOCK_SNOW_BLOCK                   ].m_Hardness = 0.2f;
	Info[E_BLOCK_CACTUS                       ].m_Hardness = 0.4f;
	Info[E_BLOCK_CLAY                         ].m_Hardness = 0.6f;
	Info[E_BLOCK_SUGARCANE                    ].m_Hardness = 0.0f;
	Info[E_BLOCK_JUKEBOX                      ].m_Hardness = 2.0f;
	Info[E_BLOCK_FENCE                        ].m_Hardness = 2.0f;
	Info[E_BLOCK_PUMPKIN                      ].m_Hardness = 1.0f;
	Info[E_BLOCK_NETHERRACK                   ].m_Hardness = 0.4f;
	Info[E_BLOCK_SOULSAND                     ].m_Hardness = 0.5f;
	Info[E_BLOCK_GLOWSTONE                    ].m_Hardness = 0.3f;
	Info[E_BLOCK_NETHER_PORTAL                ].m_Hardness = -1.0f;
	Info[E_BLOCK_JACK_O_LANTERN               ].m_Hardness = 1.0f;
	Info[E_BLOCK_CAKE                         ].m_Hardness = 0.5f;
	Info[E_BLOCK_REDSTONE_REPEATER_OFF        ].m_Hardness = 0.0f;
	Info[E_BLOCK_REDSTONE_REPEATER_ON         ].m_Hardness = 0.0f;
	Info[E_BLOCK_STAINED_GLASS                ].m_Hardness = 0.3f;
	Info[E_BLOCK_TRAPDOOR                     ].m_Hardness = 3.0f;
	Info[E_BLOCK_SILVERFISH_EGG               ].m_Hardness = 0.75f;
	Info[E_BLOCK_STONE_BRICKS                 ].m_Hardness = 1.5f;
	Info[E_BLOCK_HUGE_BROWN_MUSHROOM          ].m_Hardness = 0.2f;
	Info[E_BLOCK_HUGE_RED_MUSHROOM            ].m_Hardness = 0.2f;
	Info[E_BLOCK_IRON_BARS                    ].m_Hardness = 5.0f;
	Info[E_BLOCK_GLASS_PANE                   ].m_Hardness = 0.3f;
	Info[E_BLOCK_MELON                        ].m_Hardness = 1.0f;
	Info[E_BLOCK_PUMPKIN_STEM                 ].m_Hardness = 0.0f;
	Info[E_BLOCK_MELON_STEM                   ].m_Hardness = 0.0f;
	Info[E_BLOCK_VINES                        ].m_Hardness = 0.2f;
	Info[E_BLOCK_OAK_FENCE_GATE               ].m_Hardness = 2.0f;
	Info[E_BLOCK_BRICK_STAIRS                 ].m_Hardness = 2.0f;
	Info[E_BLOCK_STONE_BRICK_STAIRS           ].m_Hardness = 1.5f;
	Info[E_BLOCK_MYCELIUM                     ].m_Hardness = 0.6f;
	Info[E_BLOCK_LILY_PAD                     ].m_Hardness = 0.0f;
	Info[E_BLOCK_NETHER_BRICK                 ].m_Hardness = 2.0f;
	Info[E_BLOCK_NETHER_BRICK_FENCE           ].m_Hardness = 2.0f;
	Info[E_BLOCK_NETHER_BRICK_STAIRS          ].m_Hardness = 2.0f;
	Info[E_BLOCK_NETHER_WART                  ].m_Hardness = 0.0f;
	Info[E_BLOCK_ENCHANTMENT_TABLE            ].m_Hardness = 5.0f;
	Info[E_BLOCK_BREWING_STAND                ].m_Hardness = 0.5f;
	Info[E_BLOCK_CAULDRON                     ].m_Hardness = 2.0f;
	Info[E_BLOCK_END_PORTAL                   ].m_Hardness = -1.0f;
	Info[E_BLOCK_END_PORTAL_FRAME             ].m_Hardness = -1.0f;
	Info[E_BLOCK_END_STONE                    ].m_Hardness = 3.0f;
	Info[E_BLOCK_DRAGON_EGG                   ].m_Hardness = 3.0f;
	Info[E_BLOCK_REDSTONE_LAMP_OFF            ].m_Hardness = 0.3f;
	Info[E_BLOCK_REDSTONE_LAMP_ON             ].m_Hardness = 0.3f;
	Info[E_BLOCK_DOUBLE_WOODEN_SLAB           ].m_Hardness = 2.0f;
	Info[E_BLOCK_WOODEN_SLAB                  ].m_Hardness = 2.0f;
	Info[E_BLOCK_COCOA_POD                    ].m_Hardness = 0.2f;
	Info[E_BLOCK_SANDSTONE_STAIRS             ].m_Hardness = 0.8f;
	Info[E_BLOCK_EMERALD_ORE                  ].m_Hardness = 3.0f;
	Info[E_BLOCK_ENDER_CHEST                  ].m_Hardness = 22.5f;
	Info[E_BLOCK_TRIPWIRE_HOOK                ].m_Hardness = 0.0f;
	Info[E_BLOCK_TRIPWIRE                     ].m_Hardness = 0.0f;
	Info[E_BLOCK_EMERALD_BLOCK                ].m_Hardness = 5.0f;
	Info[E_BLOCK_SPRUCE_WOOD_STAIRS           ].m_Hardness = 2.0f;
	Info[E_BLOCK_BIRCH_WOOD_STAIRS            ].m_Hardness = 2.0f;
	Info[E_BLOCK_JUNGLE_WOOD_STAIRS           ].m_Hardness = 2.0f;
	Info[E_BLOCK_COMMAND_BLOCK                ].m_Hardness = -1.0f;
	Info[E_BLOCK_BEACON                       ].m_Hardness = 3.0f;
	Info[E_BLOCK_COBBLESTONE_WALL             ].m_Hardness = 2.0f;
	Info[E_BLOCK_FLOWER_POT                   ].m_Hardness = 0.0f;
	Info[E_BLOCK_CARROTS                      ].m_Hardness = 0.0f;
	Info[E_BLOCK_POTATOES                     ].m_Hardness = 0.0f;
	Info[E_BLOCK_WOODEN_BUTTON                ].m_Hardness = 0.5f;
	Info[E_BLOCK_HEAD                         ].m_Hardness = 1.0f;
	Info[E_BLOCK_ANVIL                        ].m_Hardness = 5.0f;
	Info[E_BLOCK_TRAPPED_CHEST                ].m_Hardness = 2.5f;
	Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_Hardness = 0.5f;
	Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_Hardness = 0.5f;
	Info[E_BLOCK_INACTIVE_COMPARATOR          ].m_Hardness = 0.0f;
	Info[E_BLOCK_ACTIVE_COMPARATOR            ].m_Hardness = 0.0f;
	Info[E_BLOCK_DAYLIGHT_SENSOR              ].m_Hardness = 0.2f;
	Info[E_BLOCK_BLOCK_OF_REDSTONE            ].m_Hardness = 5.0f;
	Info[E_BLOCK_NETHER_QUARTZ_ORE            ].m_Hardness = 3.0f;
	Info[E_BLOCK_HOPPER                       ].m_Hardness = 3.0f;
	Info[E_BLOCK_QUARTZ_BLOCK                 ].m_Hardness = 0.8f;
	Info[E_BLOCK_QUARTZ_STAIRS                ].m_Hardness = 0.8f;
	Info[E_BLOCK_ACTIVATOR_RAIL               ].m_Hardness = 0.7f;
	Info[E_BLOCK_DROPPER                      ].m_Hardness = 3.5f;
	Info[E_BLOCK_STAINED_CLAY                 ].m_Hardness = 4.2f;
	Info[E_BLOCK_STAINED_GLASS_PANE           ].m_Hardness = 0.3f;
	Info[E_BLOCK_NEW_LEAVES                   ].m_Hardness = 0.2f;
	Info[E_BLOCK_NEW_LOG                      ].m_Hardness = 2.0f;
	Info[E_BLOCK_ACACIA_WOOD_STAIRS           ].m_Hardness = 2.0f;
	Info[E_BLOCK_DARK_OAK_WOOD_STAIRS         ].m_Hardness = 2.0f;
	Info[E_BLOCK_SLIME_BLOCK                  ].m_Hardness = 0.0f;
	Info[E_BLOCK_BARRIER                      ].m_Hardness = -1.0f;
	Info[E_BLOCK_IRON_TRAPDOOR                ].m_Hardness = 5.0f;
	Info[E_BLOCK_PRISMARINE_BLOCK             ].m_Hardness = 1.5f;
	Info[E_BLOCK_SEA_LANTERN                  ].m_Hardness = 0.3f;
	Info[E_BLOCK_HAY_BALE                     ].m_Hardness = 0.5f;
	Info[E_BLOCK_CARPET                       ].m_Hardness = 0.1f;
	Info[E_BLOCK_HARDENED_CLAY                ].m_Hardness = 1.25f;
	Info[E_BLOCK_BLOCK_OF_COAL                ].m_Hardness = 5.0f;
	Info[E_BLOCK_PACKED_ICE                   ].m_Hardness = 0.5f;
	Info[E_BLOCK_BIG_FLOWER                   ].m_Hardness = 0.0f;
	Info[E_BLOCK_STANDING_BANNER              ].m_Hardness = 1.0f;
	Info[E_BLOCK_WALL_BANNER                  ].m_Hardness = 1.0f;
	Info[E_BLOCK_INVERTED_DAYLIGHT_SENSOR     ].m_Hardness = 0.2f;
	Info[E_BLOCK_RED_SANDSTONE                ].m_Hardness = 0.8f;
	Info[E_BLOCK_RED_SANDSTONE_STAIRS         ].m_Hardness = 0.8f;
	Info[E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB    ].m_Hardness = 2.0f;
	Info[E_BLOCK_RED_SANDSTONE_SLAB           ].m_Hardness = 2.0f;
	Info[E_BLOCK_SPRUCE_FENCE_GATE            ].m_Hardness = 2.0f;
	Info[E_BLOCK_BIRCH_FENCE_GATE             ].m_Hardness = 2.0f;
	Info[E_BLOCK_JUNGLE_FENCE_GATE            ].m_Hardness = 2.0f;
	Info[E_BLOCK_DARK_OAK_FENCE_GATE          ].m_Hardness = 2.0f;
	Info[E_BLOCK_ACACIA_FENCE_GATE            ].m_Hardness = 2.0f;
	Info[E_BLOCK_SPRUCE_FENCE                 ].m_Hardness = 2.0f;
	Info[E_BLOCK_BIRCH_FENCE                  ].m_Hardness = 2.0f;
	Info[E_BLOCK_JUNGLE_FENCE                 ].m_Hardness = 2.0f;
	Info[E_BLOCK_DARK_OAK_FENCE               ].m_Hardness = 2.0f;
	Info[E_BLOCK_ACACIA_FENCE                 ].m_Hardness = 2.0f;
	Info[E_BLOCK_SPRUCE_DOOR                  ].m_Hardness = 3.0f;
	Info[E_BLOCK_BIRCH_DOOR                   ].m_Hardness = 3.0f;
	Info[E_BLOCK_JUNGLE_DOOR                  ].m_Hardness = 3.0f;
	Info[E_BLOCK_ACACIA_DOOR                  ].m_Hardness = 3.0f;
	Info[E_BLOCK_DARK_OAK_DOOR                ].m_Hardness = 3.0f;
	Info[E_BLOCK_END_ROD                      ].m_Hardness = 0.0f;
	Info[E_BLOCK_CHORUS_PLANT                 ].m_Hardness = 0.4f;
	Info[E_BLOCK_CHORUS_FLOWER                ].m_Hardness = 0.4f;
	Info[E_BLOCK_PURPUR_BLOCK                 ].m_Hardness = 1.5f;
	Info[E_BLOCK_PURPUR_DOUBLE_SLAB           ].m_Hardness = 2.0f;
	Info[E_BLOCK_PURPUR_PILLAR                ].m_Hardness = 1.5f;
	Info[E_BLOCK_PURPUR_SLAB                  ].m_Hardness = 2.0f;
	Info[E_BLOCK_PURPUR_STAIRS                ].m_Hardness = 1.5f;
	Info[E_BLOCK_END_BRICKS                   ].m_Hardness = 0.8f;
	Info[E_BLOCK_BEETROOTS                    ].m_Hardness = 0.0f;
	Info[E_BLOCK_GRASS_PATH                   ].m_Hardness = 0.6f;
	Info[E_BLOCK_END_GATEWAY                  ].m_Hardness = -1.0f;
	Info[E_BLOCK_REPEATING_COMMAND_BLOCK      ].m_Hardness = -1.0f;
	Info[E_BLOCK_CHAIN_COMMAND_BLOCK          ].m_Hardness = -1.0f;
	Info[E_BLOCK_FROSTED_ICE                  ].m_Hardness = 0.5f;
	Info[E_BLOCK_MAGMA                        ].m_Hardness = 0.5f;
	Info[E_BLOCK_NETHER_WART_BLOCK            ].m_Hardness = 1.0f;
	Info[E_BLOCK_RED_NETHER_BRICK             ].m_Hardness = 2.0f;
	Info[E_BLOCK_BONE_BLOCK                   ].m_Hardness = 2.0f;
	Info[E_BLOCK_END_BRICKS                   ].m_Hardness = 0.8f;
	Info[E_BLOCK_STRUCTURE_VOID               ].m_Hardness = 0.0f;
	Info[E_BLOCK_OBSERVER                     ].m_Hardness = 3.5f;
	Info[E_BLOCK_WHITE_SHULKER_BOX            ].m_Hardness = 0.2f;
	Info[E_BLOCK_ORANGE_SHULKER_BOX           ].m_Hardness = 0.2f;
	Info[E_BLOCK_MAGENTA_SHULKER_BOX          ].m_Hardness = 0.2f;
	Info[E_BLOCK_LIGHT_BLUE_SHULKER_BOX       ].m_Hardness = 0.2f;
	Info[E_BLOCK_YELLOW_SHULKER_BOX           ].m_Hardness = 0.2f;
	Info[E_BLOCK_LIME_SHULKER_BOX             ].m_Hardness = 0.2f;
	Info[E_BLOCK_PINK_SHULKER_BOX             ].m_Hardness = 0.2f;
	Info[E_BLOCK_GRAY_SHULKER_BOX             ].m_Hardness = 0.2f;
	Info[E_BLOCK_LIGHT_GRAY_SHULKER_BOX       ].m_Hardness = 0.2f;
	Info[E_BLOCK_CYAN_SHULKER_BOX             ].m_Hardness = 0.2f;
	Info[E_BLOCK_PURPLE_SHULKER_BOX           ].m_Hardness = 0.2f;
	Info[E_BLOCK_BLUE_SHULKER_BOX             ].m_Hardness = 0.2f;
	Info[E_BLOCK_BROWN_SHULKER_BOX            ].m_Hardness = 0.2f;
	Info[E_BLOCK_GREEN_SHULKER_BOX            ].m_Hardness = 0.2f;
	Info[E_BLOCK_RED_SHULKER_BOX              ].m_Hardness = 0.2f;
	Info[E_BLOCK_BLACK_SHULKER_BOX            ].m_Hardness = 0.2f;
	Info[E_BLOCK_WHITE_GLAZED_TERRACOTTA      ].m_Hardness = 1.4f;
	Info[E_BLOCK_ORANGE_GLAZED_TERRACOTTA     ].m_Hardness = 1.4f;
	Info[E_BLOCK_MAGENTA_GLAZED_TERRACOTTA    ].m_Hardness = 1.4f;
	Info[E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA ].m_Hardness = 1.4f;
	Info[E_BLOCK_YELLOW_GLAZED_TERRACOTTA     ].m_Hardness = 1.4f;
	Info[E_BLOCK_LIME_GLAZED_TERRACOTTA       ].m_Hardness = 1.4f;
	Info[E_BLOCK_PINK_GLAZED_TERRACOTTA       ].m_Hardness = 1.4f;
	Info[E_BLOCK_GRAY_GLAZED_TERRACOTTA       ].m_Hardness = 1.4f;
	Info[E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA ].m_Hardness = 1.4f;
	Info[E_BLOCK_CYAN_GLAZED_TERRACOTTA       ].m_Hardness = 1.4f;
	Info[E_BLOCK_PURPLE_GLAZED_TERRACOTTA     ].m_Hardness = 1.4f;
	Info[E_BLOCK_BLUE_GLAZED_TERRACOTTA       ].m_Hardness = 1.4f;
	Info[E_BLOCK_BROWN_GLAZED_TERRACOTTA      ].m_Hardness = 1.4f;
	Info[E_BLOCK_GREEN_GLAZED_TERRACOTTA      ].m_Hardness = 1.4f;
	Info[E_BLOCK_RED_GLAZED_TERRACOTTA        ].m_Hardness = 1.4f;
	Info[E_BLOCK_BLACK_GLAZED_TERRACOTTA      ].m_Hardness = 1.4f;
	Info[E_BLOCK_CONCRETE                     ].m_Hardness = 1.8f;
	Info[E_BLOCK_CONCRETE_POWDER              ].m_Hardness = 0.5f;
	Info[E_BLOCK_STRUCTURE_BLOCK              ].m_Hardness = -1.0f;

	for (size_t i = 0; i < Info.size(); ++i)
	{
		Info[i].m_IsRainBlocker |= Info[i].m_IsSolid;
	}
}
