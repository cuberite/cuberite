
#include "Globals.h"

#include "BlockInfo.h"





cBlockInfo cBlockInfo::ms_Info[256];





cBlockInfo::cBlockInfo()
	: m_LightValue(0x00)
	, m_SpreadLightFalloff(0x0f)
	, m_Transparent(false)
	, m_OneHitDig(false)
	, m_PistonBreakable(false)
	, m_IsSnowable(true)
	, m_RequiresSpecialTool(false)
	, m_IsSolid(true)
	, m_FullyOccupiesVoxel(false)
{}





cBlockInfo & cBlockInfo::GetById(unsigned int a_ID)
{
	ASSERT(a_ID < 256);

	return ms_Info[a_ID];
}





void cBlockInfo::Initialize(void)
{
	// Emissive blocks
	ms_Info[E_BLOCK_FIRE                ].m_LightValue = 15;
	ms_Info[E_BLOCK_GLOWSTONE           ].m_LightValue = 15;
	ms_Info[E_BLOCK_JACK_O_LANTERN      ].m_LightValue = 15;
	ms_Info[E_BLOCK_LAVA                ].m_LightValue = 15;
	ms_Info[E_BLOCK_STATIONARY_LAVA     ].m_LightValue = 15;
	ms_Info[E_BLOCK_END_PORTAL          ].m_LightValue = 15;
	ms_Info[E_BLOCK_REDSTONE_LAMP_ON    ].m_LightValue = 15;
	ms_Info[E_BLOCK_TORCH               ].m_LightValue = 14;
	ms_Info[E_BLOCK_BURNING_FURNACE     ].m_LightValue = 13;
	ms_Info[E_BLOCK_NETHER_PORTAL       ].m_LightValue = 11;
	ms_Info[E_BLOCK_REDSTONE_ORE_GLOWING].m_LightValue = 9;
	ms_Info[E_BLOCK_REDSTONE_REPEATER_ON].m_LightValue = 9;
	ms_Info[E_BLOCK_REDSTONE_TORCH_ON   ].m_LightValue = 7;
	ms_Info[E_BLOCK_BREWING_STAND       ].m_LightValue = 1;
	ms_Info[E_BLOCK_BROWN_MUSHROOM      ].m_LightValue = 1;
	ms_Info[E_BLOCK_DRAGON_EGG          ].m_LightValue = 1;


	// Spread blocks
	ms_Info[E_BLOCK_AIR                 ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_CAKE                ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_CHEST               ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_COBWEB              ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_CROPS               ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_FENCE               ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_FENCE_GATE          ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_FIRE                ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_GLASS               ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_GLASS_PANE          ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_GLOWSTONE           ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_IRON_BARS           ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_IRON_DOOR           ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_LEAVES              ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_SIGN_POST           ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_TORCH               ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_VINES               ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_WALLSIGN            ].m_SpreadLightFalloff = 1;
	ms_Info[E_BLOCK_WOODEN_DOOR         ].m_SpreadLightFalloff = 1;

	// Light in water and lava dissapears faster:
	ms_Info[E_BLOCK_LAVA                ].m_SpreadLightFalloff = 3;
	ms_Info[E_BLOCK_STATIONARY_LAVA     ].m_SpreadLightFalloff = 3;
	ms_Info[E_BLOCK_STATIONARY_WATER    ].m_SpreadLightFalloff = 3;
	ms_Info[E_BLOCK_WATER               ].m_SpreadLightFalloff = 3;


	// Transparent blocks
	ms_Info[E_BLOCK_ACTIVATOR_RAIL      ].m_Transparent = true;
	ms_Info[E_BLOCK_AIR                 ].m_Transparent = true;
	ms_Info[E_BLOCK_BIG_FLOWER          ].m_Transparent = true;
	ms_Info[E_BLOCK_BROWN_MUSHROOM      ].m_Transparent = true;
	ms_Info[E_BLOCK_CARROTS             ].m_Transparent = true;
	ms_Info[E_BLOCK_CHEST               ].m_Transparent = true;
	ms_Info[E_BLOCK_COBWEB              ].m_Transparent = true;
	ms_Info[E_BLOCK_CROPS               ].m_Transparent = true;
	ms_Info[E_BLOCK_DANDELION           ].m_Transparent = true;
	ms_Info[E_BLOCK_DETECTOR_RAIL       ].m_Transparent = true;
	ms_Info[E_BLOCK_ENDER_CHEST         ].m_Transparent = true;
	ms_Info[E_BLOCK_FENCE               ].m_Transparent = true;
	ms_Info[E_BLOCK_FENCE_GATE          ].m_Transparent = true;
	ms_Info[E_BLOCK_FIRE                ].m_Transparent = true;
	ms_Info[E_BLOCK_FLOWER              ].m_Transparent = true;
	ms_Info[E_BLOCK_FLOWER_POT          ].m_Transparent = true;
	ms_Info[E_BLOCK_GLASS               ].m_Transparent = true;
	ms_Info[E_BLOCK_GLASS_PANE          ].m_Transparent = true;
	ms_Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_Transparent = true;
	ms_Info[E_BLOCK_ICE                 ].m_Transparent = true;
	ms_Info[E_BLOCK_IRON_DOOR           ].m_Transparent = true;
	ms_Info[E_BLOCK_LAVA                ].m_Transparent = true;
	ms_Info[E_BLOCK_LEAVES              ].m_Transparent = true;
	ms_Info[E_BLOCK_LEVER               ].m_Transparent = true;
	ms_Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_Transparent = true;
	ms_Info[E_BLOCK_MELON_STEM          ].m_Transparent = true;
	ms_Info[E_BLOCK_NETHER_BRICK_FENCE  ].m_Transparent = true;
	ms_Info[E_BLOCK_NEW_LEAVES          ].m_Transparent = true;
	ms_Info[E_BLOCK_POTATOES            ].m_Transparent = true;
	ms_Info[E_BLOCK_POWERED_RAIL        ].m_Transparent = true;
	ms_Info[E_BLOCK_PISTON_EXTENSION    ].m_Transparent = true;
	ms_Info[E_BLOCK_PUMPKIN_STEM        ].m_Transparent = true;
	ms_Info[E_BLOCK_RAIL                ].m_Transparent = true;
	ms_Info[E_BLOCK_RED_MUSHROOM        ].m_Transparent = true;
	ms_Info[E_BLOCK_SIGN_POST           ].m_Transparent = true;
	ms_Info[E_BLOCK_SNOW                ].m_Transparent = true;
	ms_Info[E_BLOCK_STAINED_GLASS       ].m_Transparent = true;
	ms_Info[E_BLOCK_STAINED_GLASS_PANE  ].m_Transparent = true;
	ms_Info[E_BLOCK_STATIONARY_LAVA     ].m_Transparent = true;
	ms_Info[E_BLOCK_STATIONARY_WATER    ].m_Transparent = true;
	ms_Info[E_BLOCK_STONE_BUTTON        ].m_Transparent = true;
	ms_Info[E_BLOCK_STONE_PRESSURE_PLATE].m_Transparent = true;
	ms_Info[E_BLOCK_TALL_GRASS          ].m_Transparent = true;
	ms_Info[E_BLOCK_TORCH               ].m_Transparent = true;
	ms_Info[E_BLOCK_VINES               ].m_Transparent = true;
	ms_Info[E_BLOCK_WALLSIGN            ].m_Transparent = true;
	ms_Info[E_BLOCK_WATER               ].m_Transparent = true;
	ms_Info[E_BLOCK_WOODEN_BUTTON       ].m_Transparent = true;
	ms_Info[E_BLOCK_WOODEN_DOOR         ].m_Transparent = true;
	ms_Info[E_BLOCK_WOODEN_PRESSURE_PLATE].m_Transparent = true;

	// TODO: Any other transparent blocks?


	// One hit break blocks:
	ms_Info[E_BLOCK_ACTIVE_COMPARATOR   ].m_OneHitDig = true;
	ms_Info[E_BLOCK_BIG_FLOWER          ].m_OneHitDig = true;
	ms_Info[E_BLOCK_BROWN_MUSHROOM      ].m_OneHitDig = true;
	ms_Info[E_BLOCK_CARROTS             ].m_OneHitDig = true;
	ms_Info[E_BLOCK_CROPS               ].m_OneHitDig = true;
	ms_Info[E_BLOCK_DANDELION           ].m_OneHitDig = true;
	ms_Info[E_BLOCK_FIRE                ].m_OneHitDig = true;
	ms_Info[E_BLOCK_FLOWER              ].m_OneHitDig = true;
	ms_Info[E_BLOCK_FLOWER_POT          ].m_OneHitDig = true;
	ms_Info[E_BLOCK_INACTIVE_COMPARATOR ].m_OneHitDig = true;
	ms_Info[E_BLOCK_MELON_STEM          ].m_OneHitDig = true;
	ms_Info[E_BLOCK_POTATOES            ].m_OneHitDig = true;
	ms_Info[E_BLOCK_PUMPKIN_STEM        ].m_OneHitDig = true;
	ms_Info[E_BLOCK_REDSTONE_REPEATER_OFF].m_OneHitDig = true;
	ms_Info[E_BLOCK_REDSTONE_REPEATER_ON].m_OneHitDig = true;
	ms_Info[E_BLOCK_REDSTONE_TORCH_OFF  ].m_OneHitDig = true;
	ms_Info[E_BLOCK_REDSTONE_TORCH_ON   ].m_OneHitDig = true;
	ms_Info[E_BLOCK_REDSTONE_WIRE       ].m_OneHitDig = true;
	ms_Info[E_BLOCK_RED_MUSHROOM        ].m_OneHitDig = true;
	ms_Info[E_BLOCK_REEDS               ].m_OneHitDig = true;
	ms_Info[E_BLOCK_SAPLING             ].m_OneHitDig = true;
	ms_Info[E_BLOCK_TNT                 ].m_OneHitDig = true;
	ms_Info[E_BLOCK_TALL_GRASS          ].m_OneHitDig = true;
	ms_Info[E_BLOCK_TORCH               ].m_OneHitDig = true;


	// Blocks that break when pushed by piston:
	ms_Info[E_BLOCK_ACTIVE_COMPARATOR   ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_AIR                 ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_BED                 ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_BIG_FLOWER          ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_BROWN_MUSHROOM      ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_COBWEB              ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_CROPS               ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_DANDELION           ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_DEAD_BUSH           ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_FIRE                ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_FLOWER              ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_PistonBreakable = true;
	ms_Info[E_BLOCK_INACTIVE_COMPARATOR ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_IRON_DOOR           ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_JACK_O_LANTERN      ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_PistonBreakable = true;
	ms_Info[E_BLOCK_LADDER              ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_LAVA                ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_LEVER               ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_MELON               ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_MELON_STEM          ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_PUMPKIN             ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_PUMPKIN_STEM        ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_REDSTONE_REPEATER_OFF].m_PistonBreakable = true;
	ms_Info[E_BLOCK_REDSTONE_REPEATER_ON].m_PistonBreakable = true;
	ms_Info[E_BLOCK_REDSTONE_TORCH_OFF  ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_REDSTONE_TORCH_ON   ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_REDSTONE_WIRE       ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_RED_MUSHROOM        ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_REEDS               ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_SNOW                ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_STATIONARY_LAVA     ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_STATIONARY_WATER    ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_STONE_BUTTON        ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_STONE_PRESSURE_PLATE].m_PistonBreakable = true;
	ms_Info[E_BLOCK_TALL_GRASS          ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_TORCH               ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_VINES               ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_WATER               ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_WOODEN_BUTTON       ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_WOODEN_DOOR         ].m_PistonBreakable = true;
	ms_Info[E_BLOCK_WOODEN_PRESSURE_PLATE].m_PistonBreakable = true;


	// Blocks that cannot be snowed over:
	ms_Info[E_BLOCK_ACTIVE_COMPARATOR   ].m_IsSnowable = false;
	ms_Info[E_BLOCK_AIR                 ].m_IsSnowable = false;
	ms_Info[E_BLOCK_BIG_FLOWER          ].m_IsSnowable = false;
	ms_Info[E_BLOCK_BROWN_MUSHROOM      ].m_IsSnowable = false;
	ms_Info[E_BLOCK_CACTUS              ].m_IsSnowable = false;
	ms_Info[E_BLOCK_CHEST               ].m_IsSnowable = false;
	ms_Info[E_BLOCK_CROPS               ].m_IsSnowable = false;
	ms_Info[E_BLOCK_DANDELION           ].m_IsSnowable = false;
	ms_Info[E_BLOCK_FIRE                ].m_IsSnowable = false;
	ms_Info[E_BLOCK_FLOWER              ].m_IsSnowable = false;
	ms_Info[E_BLOCK_GLASS               ].m_IsSnowable = false;
	ms_Info[E_BLOCK_ICE                 ].m_IsSnowable = false;
	ms_Info[E_BLOCK_INACTIVE_COMPARATOR ].m_IsSnowable = false;
	ms_Info[E_BLOCK_LAVA                ].m_IsSnowable = false;
	ms_Info[E_BLOCK_LILY_PAD            ].m_IsSnowable = false;
	ms_Info[E_BLOCK_REDSTONE_REPEATER_OFF].m_IsSnowable = false;
	ms_Info[E_BLOCK_REDSTONE_REPEATER_ON].m_IsSnowable = false;
	ms_Info[E_BLOCK_REDSTONE_TORCH_OFF  ].m_IsSnowable = false;
	ms_Info[E_BLOCK_REDSTONE_TORCH_ON   ].m_IsSnowable = false;
	ms_Info[E_BLOCK_REDSTONE_WIRE       ].m_IsSnowable = false;
	ms_Info[E_BLOCK_RED_MUSHROOM        ].m_IsSnowable = false;
	ms_Info[E_BLOCK_REEDS               ].m_IsSnowable = false;
	ms_Info[E_BLOCK_SAPLING             ].m_IsSnowable = false;
	ms_Info[E_BLOCK_SIGN_POST           ].m_IsSnowable = false;
	ms_Info[E_BLOCK_SNOW                ].m_IsSnowable = false;
	ms_Info[E_BLOCK_STAINED_GLASS       ].m_IsSnowable = false;
	ms_Info[E_BLOCK_STAINED_GLASS_PANE  ].m_IsSnowable = false;
	ms_Info[E_BLOCK_STATIONARY_LAVA     ].m_IsSnowable = false;
	ms_Info[E_BLOCK_STATIONARY_WATER    ].m_IsSnowable = false;
	ms_Info[E_BLOCK_TALL_GRASS          ].m_IsSnowable = false;
	ms_Info[E_BLOCK_TNT                 ].m_IsSnowable = false;
	ms_Info[E_BLOCK_TORCH               ].m_IsSnowable = false;
	ms_Info[E_BLOCK_VINES               ].m_IsSnowable = false;
	ms_Info[E_BLOCK_WALLSIGN            ].m_IsSnowable = false;
	ms_Info[E_BLOCK_WATER               ].m_IsSnowable = false;


	// Blocks that don't drop without a special tool:
	ms_Info[E_BLOCK_BRICK               ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_CAULDRON            ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_COAL_ORE            ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_COBBLESTONE         ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_COBBLESTONE_STAIRS  ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_COBWEB              ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_DIAMOND_BLOCK       ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_DIAMOND_ORE         ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_DOUBLE_STONE_SLAB   ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_EMERALD_ORE         ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_END_STONE           ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_GOLD_BLOCK          ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_GOLD_ORE            ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_IRON_BLOCK          ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_IRON_ORE            ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_LAPIS_BLOCK         ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_LAPIS_ORE           ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_MOSSY_COBBLESTONE   ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_NETHERRACK          ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_NETHER_BRICK        ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_NETHER_BRICK_STAIRS ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_OBSIDIAN            ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_REDSTONE_ORE        ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_REDSTONE_ORE_GLOWING].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_SANDSTONE           ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_SANDSTONE_STAIRS    ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_SNOW                ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_STONE               ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_STONE_BRICKS        ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_STONE_BRICK_STAIRS  ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_STONE_PRESSURE_PLATE].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_STONE_SLAB          ].m_RequiresSpecialTool = true;
	ms_Info[E_BLOCK_VINES               ].m_RequiresSpecialTool = true;


	// Nonsolid blocks:
	ms_Info[E_BLOCK_ACTIVATOR_RAIL      ].m_IsSolid = false;
	ms_Info[E_BLOCK_AIR                 ].m_IsSolid = false;
	ms_Info[E_BLOCK_BIG_FLOWER          ].m_IsSolid = false;
	ms_Info[E_BLOCK_BROWN_MUSHROOM      ].m_IsSolid = false;
	ms_Info[E_BLOCK_CARROTS             ].m_IsSolid = false;
	ms_Info[E_BLOCK_COBWEB              ].m_IsSolid = false;
	ms_Info[E_BLOCK_CROPS               ].m_IsSolid = false;
	ms_Info[E_BLOCK_DANDELION           ].m_IsSolid = false;
	ms_Info[E_BLOCK_DETECTOR_RAIL       ].m_IsSolid = false;
	ms_Info[E_BLOCK_END_PORTAL          ].m_IsSolid = false;
	ms_Info[E_BLOCK_FIRE                ].m_IsSolid = false;
	ms_Info[E_BLOCK_FLOWER              ].m_IsSolid = false;
	ms_Info[E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE].m_IsSolid = false;
	ms_Info[E_BLOCK_LAVA                ].m_IsSolid = false;
	ms_Info[E_BLOCK_LEVER               ].m_IsSolid = false;
	ms_Info[E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE].m_IsSolid = false;
	ms_Info[E_BLOCK_MELON_STEM          ].m_IsSolid = false;
	ms_Info[E_BLOCK_NETHER_PORTAL       ].m_IsSolid = false;
	ms_Info[E_BLOCK_PISTON_EXTENSION    ].m_IsSolid = false;
	ms_Info[E_BLOCK_POTATOES            ].m_IsSolid = false;
	ms_Info[E_BLOCK_POWERED_RAIL        ].m_IsSolid = false;
	ms_Info[E_BLOCK_RAIL                ].m_IsSolid = false;
	ms_Info[E_BLOCK_REDSTONE_TORCH_OFF  ].m_IsSolid = false;
	ms_Info[E_BLOCK_REDSTONE_TORCH_ON   ].m_IsSolid = false;
	ms_Info[E_BLOCK_REDSTONE_WIRE       ].m_IsSolid = false;
	ms_Info[E_BLOCK_RED_MUSHROOM        ].m_IsSolid = false;
	ms_Info[E_BLOCK_REEDS               ].m_IsSolid = false;
	ms_Info[E_BLOCK_SAPLING             ].m_IsSolid = false;
	ms_Info[E_BLOCK_SIGN_POST           ].m_IsSolid = false;
	ms_Info[E_BLOCK_SNOW                ].m_IsSolid = false;
	ms_Info[E_BLOCK_STATIONARY_LAVA     ].m_IsSolid = false;
	ms_Info[E_BLOCK_STATIONARY_WATER    ].m_IsSolid = false;
	ms_Info[E_BLOCK_STONE_BUTTON        ].m_IsSolid = false;
	ms_Info[E_BLOCK_STONE_PRESSURE_PLATE].m_IsSolid = false;
	ms_Info[E_BLOCK_TALL_GRASS          ].m_IsSolid = false;
	ms_Info[E_BLOCK_TORCH               ].m_IsSolid = false;
	ms_Info[E_BLOCK_TRIPWIRE            ].m_IsSolid = false;
	ms_Info[E_BLOCK_VINES               ].m_IsSolid = false;
	ms_Info[E_BLOCK_WALLSIGN            ].m_IsSolid = false;
	ms_Info[E_BLOCK_WATER               ].m_IsSolid = false;
	ms_Info[E_BLOCK_WOODEN_BUTTON       ].m_IsSolid = false;
	ms_Info[E_BLOCK_WOODEN_PRESSURE_PLATE].m_IsSolid = false;
	ms_Info[E_BLOCK_WOODEN_SLAB         ].m_IsSolid = false;


	// Torch placeable blocks:
	ms_Info[E_BLOCK_NEW_LOG             ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_BEDROCK             ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_BLOCK_OF_COAL       ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_BLOCK_OF_REDSTONE   ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_BOOKCASE            ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_BRICK               ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_CLAY                ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_COAL_ORE            ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_COBBLESTONE         ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_COMMAND_BLOCK       ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_CRAFTING_TABLE      ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_DIAMOND_BLOCK       ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_DIAMOND_ORE         ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_DIRT                ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_DISPENSER           ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_DOUBLE_STONE_SLAB   ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_DOUBLE_WOODEN_SLAB  ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_DROPPER             ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_EMERALD_BLOCK       ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_EMERALD_ORE         ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_END_STONE           ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_FURNACE             ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_GLOWSTONE           ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_GOLD_BLOCK          ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_GOLD_ORE            ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_GRASS               ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_GRAVEL              ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_HARDENED_CLAY       ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_HAY_BALE            ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_HUGE_BROWN_MUSHROOM ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_HUGE_RED_MUSHROOM   ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_IRON_BLOCK          ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_IRON_ORE            ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_JACK_O_LANTERN      ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_JUKEBOX             ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_LAPIS_BLOCK         ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_LAPIS_ORE           ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_LOG                 ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_MELON               ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_MOSSY_COBBLESTONE   ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_MYCELIUM            ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_NETHERRACK          ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_NETHER_BRICK        ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_NETHER_QUARTZ_ORE   ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_NOTE_BLOCK          ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_OBSIDIAN            ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_PACKED_ICE          ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_PLANKS              ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_PUMPKIN             ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_QUARTZ_BLOCK        ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_REDSTONE_LAMP_OFF   ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_REDSTONE_LAMP_ON    ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_REDSTONE_ORE        ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_REDSTONE_ORE_GLOWING].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_SANDSTONE           ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_SAND                ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_SILVERFISH_EGG      ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_SPONGE              ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_STAINED_CLAY        ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_WOOL                ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_STONE               ].m_FullyOccupiesVoxel = true;
	ms_Info[E_BLOCK_STONE_BRICKS        ].m_FullyOccupiesVoxel = true;
}





// This is actually just some code that needs to run at program startup, so it is wrapped into a global var's constructor:
class cBlockInfoInitializer
{
public:
	cBlockInfoInitializer(void)
	{
		cBlockInfo::Initialize();
	}
} BlockInfoInitializer;





