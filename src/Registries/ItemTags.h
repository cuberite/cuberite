
// ItemTags.h

#pragma once

/*
This file describes the item tags (kind of categories).
Those are used e.g. in the loot tables.
You can export them from the minecraft sever with data generators

There is a function provided to obtain a enum from a string in cuberite NamingConvention.
There is also a function which provides cItems with the items described by the tag.

There are many commented lines because any item that is in a tag in Vanilla 1.16.2 is there.

This list is up to date with 1.16.2 if there are new added please add below:
https://minecraft.gamepedia.com/Tag#Items
*/

namespace ItemTag
{
	enum class eItemTags
	{
		AcaciaLogs,
		Anvil,
		Arrows,
		Banners,
		BeaconPaymentItems,
		Beds,
		BirchLogs,
		Boats,
		Buttons,
		Carpets,
		Coals,
		CreeperDropsMusicDiscs,
		CrimsonStems,
		DarkOakLogs,
		Doors,
		Fences,
		Fishes,
		Flowers,
		GoldOres,
		JungleLogs,
		Leaves,
		LecternBooks,
		Logs,
		LogsThatBurn,
		MusicDiscs,
		NonFlammableWood,
		OakLogs,
		PiglinLoved,
		PiglinRepellents,
		Planks,
		Rails,
		Sand,
		Saplings,
		Signs,
		Slabs,
		SmallFlowers,
		SoulFireBaseBlocks,
		SpruceLogs,
		Stairs,
		StoneBricks,
		StoneCraftingMaterials,
		StoneToolMaterials,
		TallFlowers,
		TrapDoors,
		Walls,
		WarpedStems,
		WoodenButtons,
		WoodenDoors,
		WoodenFences,
		WoodenPressurePlates,
		WoodenSlabs,
		WoodenStairs,
		WoodenTrapDoors,
		Wool,
		None
	};

	/** Obtains the eItemTags type from string */
	enum eItemTags eItemTags(const AString & a_String)
	{
		if (NoCaseCompare(a_String, "AcaciaLogs") == 0)
		{
			return eItemTags::AcaciaLogs;
		}
		else if (NoCaseCompare(a_String, "Anvil") == 0)
		{
			return eItemTags::Anvil;
		}
		else if (NoCaseCompare(a_String, "Arrows") == 0)
		{
			return eItemTags::Arrows;
		}
		else if (NoCaseCompare(a_String, "Banners") == 0)
		{
			return eItemTags::Banners;
		}
		else if (NoCaseCompare(a_String, "BeaconPaymentItems") == 0)
		{
			return eItemTags::BeaconPaymentItems;
		}
		else if (NoCaseCompare(a_String, "Beds") == 0)
		{
			return eItemTags::Beds;
		}
		else if (NoCaseCompare(a_String, "BirchLogs") == 0)
		{
			return eItemTags::BirchLogs;
		}
		else if (NoCaseCompare(a_String, "Boats") == 0)
		{
			return eItemTags::Boats;
		}
		else if (NoCaseCompare(a_String, "Buttons") == 0)
		{
			return eItemTags::Buttons;
		}
		else if (NoCaseCompare(a_String, "Carpets") == 0)
		{
			return eItemTags::Carpets;
		}
		else if (NoCaseCompare(a_String, "Coals") == 0)
		{
			return eItemTags::Coals;
		}
		else if (NoCaseCompare(a_String, "CreeperDropsMusicDiscs") == 0)
		{
			return eItemTags::CreeperDropsMusicDiscs;
		}
		else if (NoCaseCompare(a_String, "CrimsonStems") == 0)
		{
			return eItemTags::CrimsonStems;
		}
		else if (NoCaseCompare(a_String, "DarkOakLogs") == 0)
		{
			return eItemTags::DarkOakLogs;
		}
		else if (NoCaseCompare(a_String, "Doors") == 0)
		{
			return eItemTags::Doors;
		}
		else if (NoCaseCompare(a_String, "Fences") == 0)
		{
			return eItemTags::Fences;
		}
		else if (NoCaseCompare(a_String, "Fishes") == 0)
		{
			return eItemTags::Fishes;
		}
		else if (NoCaseCompare(a_String, "Flowers") == 0)
		{
			return eItemTags::Flowers;
		}
		else if (NoCaseCompare(a_String, "GoldOres") == 0)
		{
			return eItemTags::GoldOres;
		}
		else if (NoCaseCompare(a_String, "JungleLogs") == 0)
		{
			return eItemTags::JungleLogs;
		}
		else if (NoCaseCompare(a_String, "Leaves") == 0)
		{
			return eItemTags::Leaves;
		}
		else if (NoCaseCompare(a_String, "Fences") == 0)
		{
			return eItemTags::Fences;
		}
		else if (NoCaseCompare(a_String, "LecternBooks") == 0)
		{
			return eItemTags::LecternBooks;
		}
		else if (NoCaseCompare(a_String, "Logs") == 0)
		{
			return eItemTags::Logs;
		}
		else if (NoCaseCompare(a_String, "LogsThatBurn") == 0)
		{
			return eItemTags::LogsThatBurn;
		}
		else if (NoCaseCompare(a_String, "MusicDiscs") == 0)
		{
			return eItemTags::MusicDiscs;
		}
		else if (NoCaseCompare(a_String, "NonFlammableWood") == 0)
		{
			return eItemTags::NonFlammableWood;
		}
		else if (NoCaseCompare(a_String, "OakLogs") == 0)
		{
			return eItemTags::OakLogs;
		}
		else if (NoCaseCompare(a_String, "PiglinLoved") == 0)
		{
			return eItemTags::PiglinLoved;
		}
		else if (NoCaseCompare(a_String, "PiglinRepellents") == 0)
		{
			return eItemTags::PiglinRepellents;
		}
		else if (NoCaseCompare(a_String, "Planks") == 0)
		{
			return eItemTags::Planks;
		}
		else if (NoCaseCompare(a_String, "Rails") == 0)
		{
			return eItemTags::Rails;
		}
		else if (NoCaseCompare(a_String, "Sand") == 0)
		{
			return eItemTags::Sand;
		}
		else if (NoCaseCompare(a_String, "Saplings") == 0)
		{
			return eItemTags::Saplings;
		}
		else if (NoCaseCompare(a_String, "Signs") == 0)
		{
			return eItemTags::Signs;
		}
		else if (NoCaseCompare(a_String, "Slabs") == 0)
		{
			return eItemTags::Slabs;
		}
		else if (NoCaseCompare(a_String, "SmallFlowers") == 0)
		{
			return eItemTags::SmallFlowers;
		}
		else if (NoCaseCompare(a_String, "SoulFireBaseBlocks") == 0)
		{
			return eItemTags::SoulFireBaseBlocks;
		}
		else if (NoCaseCompare(a_String, "SpruceLogs") == 0)
		{
			return eItemTags::SpruceLogs;
		}
		else if (NoCaseCompare(a_String, "Stairs") == 0)
		{
			return eItemTags::Stairs;
		}
		else if (NoCaseCompare(a_String, "StoneBricks") == 0)
		{
			return eItemTags::StoneBricks;
		}
		else if (NoCaseCompare(a_String, "StoneCraftingMaterials") == 0)
		{
			return eItemTags::StoneCraftingMaterials;
		}
		else if (NoCaseCompare(a_String, "StoneToolMaterials") == 0)
		{
			return eItemTags::StoneToolMaterials;
		}
		else if (NoCaseCompare(a_String, "TallFlowers") == 0)
		{
			return eItemTags::TallFlowers;
		}
		else if (NoCaseCompare(a_String, "TrapDoors") == 0)
		{
			return eItemTags::TrapDoors;
		}
		else if (NoCaseCompare(a_String, "Walls") == 0)
		{
			return eItemTags::Walls;
		}
		else if (NoCaseCompare(a_String, "WarpedStems") == 0)
		{
			return eItemTags::WarpedStems;
		}
		else if (NoCaseCompare(a_String, "WoodenButtons") == 0)
		{
			return eItemTags::WoodenButtons;
		}
		else if (NoCaseCompare(a_String, "WoodenDoors") == 0)
		{
			return eItemTags::WoodenDoors;
		}
		else if (NoCaseCompare(a_String, "WoodenFences") == 0)
		{
			return eItemTags::WoodenFences;
		}
		else if (NoCaseCompare(a_String, "WoodenPressurePlates") == 0)
		{
			return eItemTags::WoodenPressurePlates;
		}
		else if (NoCaseCompare(a_String, "WoodenSlabs") == 0)
		{
			return eItemTags::WoodenSlabs;
		}
		else if (NoCaseCompare(a_String, "WoodenStairs") == 0)
		{
			return eItemTags::WoodenStairs;
		}
		else if (NoCaseCompare(a_String, "WoodenTrapDoors") == 0)
		{
			return eItemTags::WoodenTrapDoors;
		}
		else if (NoCaseCompare(a_String, "Wool") == 0)
		{
			return eItemTags::Wool;
		}
		else
		{
			return eItemTags::None;
		}
	}

// NOTE: Some of the Items are contained in multiple tags. Those are defined in the following macros

// TODO: 07.09.2020 Add when implemented - 12xx12
#define ACACIALOGS \
	cItem(E_BLOCK_NEW_LOG, 1, E_META_NEW_LOG_ACACIA_WOOD),
	// cItem(E_BLOCK_ACACIA_WOOD), \          // The Wood with the outside texture on all sides
	// cItem(E_BLOCK_STRIPPED_ACACIA_LOG), \  // The STRIPPED default log
	// cItem(E_BLOCK_STRIPPED_ACACIA_WOOD) \  // The STRIPPED wood

// TODO: 07.09.2020 Add when implemented - 12xx12
#define BIRCHLOGS \
	cItem(E_BLOCK_LOG, 1, E_META_LOG_BIRCH),
	// cItem(E_BLOCK_BIRCH_WOOD), \          // The Wood with the outside texture on all sides
	// cItem(E_BLOCK_STRIPPED_BIRCH_LOG), \  // The STRIPPED default log
	// cItem(E_BLOCK_STRIPPED_BIRCH_WOOD)    // The STRIPPED wood

#define CREEPERDROPMUSICDISCS \
	cItem(E_ITEM_13_DISC), \
	cItem(E_ITEM_CAT_DISC), \
	cItem(E_ITEM_BLOCKS_DISC), \
	cItem(E_ITEM_CHIRP_DISC), \
	cItem(E_ITEM_FAR_DISC), \
	cItem(E_ITEM_MALL_DISC), \
	cItem(E_ITEM_MELLOHI_DISC), \
	cItem(E_ITEM_STAL_DISC), \
	cItem(E_ITEM_STRAD_DISC), \
	cItem(E_ITEM_WARD_DISC), \
	cItem(E_ITEM_11_DISC), \
	cItem(E_ITEM_WAIT_DISC),

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define CRIMSONSTEMS
	// cItem(E_BLOCK_CRIMSON_STEM),
	// cItem(E_BLOCK_STRIPPED_CRIMSON_STEM),
	// cItem(E_BLOCK_CRIMSON_HYPHAE),
	// cItem(E_BLOCK_STRIPPED_CRIMSON_HYPHAE),

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define DARKOAKLOGS \
	cItem(E_BLOCK_NEW_LOG, 1, E_META_NEW_LOG_DARK_OAK_WOOD),
	// cItem(E_BLOCK_DARK_OAK_WOOD),
	// cItem(E_BLOCK_STRIPPED_DARK_OAK_LOG),
	// cItem(E_BLOCK_STRIPPED_DARK_OAK_WOOD),

#define GOLDORES \
	cItem(E_BLOCK_GOLD_ORE),
	// cItem(E_BLOCK_NETHER_GOLD_ORE),  // TODO: 07.09.2020 - Add when implemented - 12xx12

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define JUNGLELOGS \
	cItem(E_BLOCK_LOG, 1, E_META_LOG_JUNGLE),
	// cItem(E_BLOCK_JUNGLE_WOOD),
	// cItem(E_BLOCK_STRIPPED_JUNGLE_LOG),
	// cItem(E_BLOCK_STRIPPED_JUNGLE_WOOD),

#define LOGSTHATBURN \
	DARKOAKLOGS \
	OAKLOGS \
	ACACIALOGS \
	BIRCHLOGS \
	JUNGLELOGS \
	SPRUCELOGS

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define OAKLOGS \
	cItem(E_BLOCK_LOG, 1, E_META_LOG_APPLE),
	// cItem(E_BLOCK_OAK_WOOD),
	// cItem(E_BLOCK_STRIPPED_OAK_LOG),
	// cItem(E_BLOCK_STRIPPED_OAK_WOOD)

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define SMALLFLOWERS \
	cItem(E_BLOCK_DANDELION), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_POPPY), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_BLUE_ORCHID), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_ALLIUM), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_RED_TULIP), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_ORANGE_TULIP), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_WHITE_TULIP), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_PINK_TULIP), \
	cItem(E_BLOCK_FLOWER, 1, E_META_FLOWER_OXEYE_DAISY),
	// cItem(E_BLOCK_AZURE_BLUET),
	// cItem(E_BLOCK_CORNFLOWER),
	// cItem(E_BLOCK_LILLY_OF_THE_VALLEY),
	// cItem(E_BLOCK_WITHER_ROSE),

#define SPRUCELOGS \
	cItem(E_BLOCK_LOG, 1, E_META_LOG_CONIFER),
	// cItem(E_BLOCK_SPRUCE_WOOD),
	// cItem(E_BLOCK_STRIPPED_SPRUCE_LOG),
	// cItem(E_BLOCK_STRIPPED_SPRUCE_WOOD),

#define TALLFLOWERS \
	cItem(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_SUNFLOWER), \
	cItem(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_LILAC), \
	cItem(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_PEONY), \
	cItem(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_ROSE_BUSH),

#define WARPEDSTEMS
	// cItem(E_BLOCK_WARPED_STEM),
	// cItem(E_BLOCK_STRIPPED_WARPED_STEM),
	// cItem(E_BLOCK_WARPED_HYPHAE),
	// cItem(E_BLOCK_STRIPPED_WARPED_HYPHAE),

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define WOODENBUTTONS \
	cItem(E_BLOCK_WOODEN_BUTTON),
	// cItem(E_BLOCK_ACACIA_BUTTON),
	// cItem(E_BLOCK_BIRCH_BUTTON),
	// cItem(E_BLOCK_DARK_OAK_BUTTON),
	// cItem(E_BLOCK_JUNGLE_BUTTON),
	// cItem(E_BLOCK_OAK_BUTTON),
	// cItem(E_BLOCK_SPRUCE_BUTTON),

#define WOODENDOORS \
	cItem(E_BLOCK_ACACIA_DOOR), \
	cItem(E_BLOCK_BIRCH_DOOR), \
	cItem(E_BLOCK_DARK_OAK_DOOR), \
	cItem(E_BLOCK_JUNGLE_DOOR), \
	cItem(E_BLOCK_OAK_DOOR), \
	cItem(E_BLOCK_SPRUCE_DOOR),

#define WOODENFENCES \
	cItem(E_BLOCK_FENCE), \
	cItem(E_BLOCK_ACACIA_FENCE), \
	cItem(E_BLOCK_BIRCH_FENCE), \
	cItem(E_BLOCK_JUNGLE_FENCE), \
	cItem(E_BLOCK_SPRUCE_FENCE), \
	cItem(E_BLOCK_DARK_OAK_FENCE), \
	cItem(E_BLOCK_NETHER_BRICK_FENCE),

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define WOODENSLABS \
	cItem(E_BLOCK_WOODEN_SLAB, 1, E_META_WOODEN_SLAB_ACACIA), \
	cItem(E_BLOCK_WOODEN_SLAB, 1, E_META_WOODEN_SLAB_BIRCH), \
	cItem(E_BLOCK_WOODEN_SLAB, 1, E_META_WOODEN_SLAB_DARK_OAK), \
	cItem(E_BLOCK_WOODEN_SLAB, 1, E_META_WOODEN_SLAB_JUNGLE), \
	cItem(E_BLOCK_WOODEN_SLAB, 1, E_META_WOODEN_SLAB_OAK), \
	cItem(E_BLOCK_WOODEN_SLAB, 1, E_META_WOODEN_SLAB_SPRUCE), \
	// cItem(E_BLOCK_CRIMSON_SLAB),
	// cItem(E_BLOCK_WARPED_SLAB),

#define WOODENSTAIRS \
	cItem(E_BLOCK_JUNGLE_WOOD_STAIRS), \
	cItem(E_BLOCK_SPRUCE_WOOD_STAIRS), \
	cItem(E_BLOCK_OAK_WOOD_STAIRS), \
	cItem(E_BLOCK_BIRCH_WOOD_STAIRS), \
	cItem(E_BLOCK_ACACIA_WOOD_STAIRS), \
	cItem(E_BLOCK_DARK_OAK_WOOD_STAIRS),

// TODO: 07.09.2020 - Add when implemented - 12xx12
#define WOODENTRAPDOORS \
	cItem(E_BLOCK_TRAPDOOR),
	// cItem(E_BLOCK_ACACIA_TRAPDOOR),
	// cItem(E_BLOCK_BIRCH_TRAPDOOR),
	// cItem(E_BLOCK_DARK_OAK_TRAPDOOR),
	// cItem(E_BLOCK_JUNGLE_TRAPDOOR),
	// cItem(E_BLOCK_SPRUCE_TRAPDOOR),
	// cItem(E_BLOCK_CRIMSON_TRAPDOOR),
	// cItem(E_BLOCK_WARPED_TRAPDOOR),


	const std::initializer_list<cItem> AcaciaLogs =
	{
		ACACIALOGS
	};

	const std::initializer_list<cItem> Anvil =
	{
		cItem(E_BLOCK_ANVIL, 1, E_BLOCK_ANVIL_NO_DAMAGE),
		cItem(E_BLOCK_ANVIL, 1, E_BLOCK_ANVIL_LOW_DAMAGE),
		cItem(E_BLOCK_ANVIL, 1, E_BLOCK_ANVIL_HIGH_DAMAGE)
	};

	const std::initializer_list<cItem> Arrows =
	{
		cItem(E_ITEM_ARROW),
		cItem(E_ITEM_SPECTRAL_ARROW),
		cItem(E_ITEM_TIPPED_ARROW)
	};

	const std::initializer_list<cItem> Banners =
	{
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_BLACK),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_BLUE),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_BROWN),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_CYAN),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_GRAY),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_GREEN),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_LIGHTBLUE),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_LIGHTGRAY),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_LIGHTGREEN),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_MAGENTA),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_ORANGE),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_PINK),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_PURPLE),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_RED),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_WHITE),
		cItem(E_ITEM_BANNER, 1, E_META_BANNER_YELLOW),
	};

	const std::initializer_list<cItem> BeaconPaymentItems =
	{
		cItem(E_ITEM_IRON),
		cItem(E_ITEM_GOLD),
		cItem(E_ITEM_DIAMOND),
		cItem(E_ITEM_EMERALD),
		// cItem(E_ITEM_NETHERITE)  // TODO: 07.09.2020 Add when implemented - 12xx12
	};

	const std::initializer_list<cItem> Beds =
	{
		cItem(E_ITEM_BED, 1, 0x00),
		cItem(E_ITEM_BED, 1, 0x01),
		cItem(E_ITEM_BED, 1, 0x02),
		cItem(E_ITEM_BED, 1, 0x03),
		cItem(E_ITEM_BED, 1, 0x04),
		cItem(E_ITEM_BED, 1, 0x05),
		cItem(E_ITEM_BED, 1, 0x06),
		cItem(E_ITEM_BED, 1, 0x07),
		cItem(E_ITEM_BED, 1, 0x08),
		cItem(E_ITEM_BED, 1, 0x09),
		cItem(E_ITEM_BED, 1, 0x0a),
		cItem(E_ITEM_BED, 1, 0x0b),
		cItem(E_ITEM_BED, 1, 0x0c),
		cItem(E_ITEM_BED, 1, 0x0d),
		cItem(E_ITEM_BED, 1, 0x0e),
		cItem(E_ITEM_BED, 1, 0x0f),
	};

	const std::initializer_list<cItem> BirchLogs =
	{
		BIRCHLOGS
	};

	const std::initializer_list<cItem> Boats =
	{
		cItem(E_ITEM_BOAT),
		cItem(E_ITEM_ACACIA_BOAT),
		cItem(E_ITEM_BIRCH_BOAT),
		cItem(E_ITEM_JUNGLE_BOAT),
		cItem(E_ITEM_SPRUCE_BOAT),
		cItem(E_ITEM_DARK_OAK_BOAT)
	};

	const std::initializer_list<cItem> Buttons =
	{
		WOODENBUTTONS
		cItem(E_BLOCK_STONE_BUTTON),
	};

	const std::initializer_list<cItem> Carpets =
	{
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_BLACK),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_BLUE),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_BROWN),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_CYAN),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_GRAY),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_GREEN),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_LIGHTBLUE),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_LIGHTGRAY),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_LIGHTGREEN),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_MAGENTA),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_ORANGE),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_PINK),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_PURPLE),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_RED),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_WHITE),
		cItem(E_BLOCK_CARPET, 1, E_META_CARPET_YELLOW),
	};

	const std::initializer_list<cItem> Coals =
	{
		cItem(E_ITEM_COAL, 1, E_META_COAL_NORMAL),
		cItem(E_ITEM_COAL, 1, E_META_COAL_CHARCOAL)
	};

	const std::initializer_list<cItem> CreeperDropsMusicDiscs =
	{
		CREEPERDROPMUSICDISCS
	};

	const std::initializer_list<cItem> CrimsonStems =
	{
		CRIMSONSTEMS
	};

	const std::initializer_list<cItem> DarkOakLogs =
	{
		DARKOAKLOGS
	};

	const std::initializer_list<cItem> Doors =
	{
		WOODENDOORS
		cItem(E_BLOCK_IRON_DOOR),
	};

	const std::initializer_list<cItem> Fences =
	{
		WOODENFENCES
		cItem(E_BLOCK_NETHER_BRICK_FENCE),
	};

	const std::initializer_list<cItem> Fishes =
	{
		cItem(E_ITEM_RAW_FISH, 1, E_META_COOKED_FISH_FISH),
		cItem(E_ITEM_RAW_FISH, 1, E_META_COOKED_FISH_SALMON),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_FISH),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_SALMON),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_CLOWNFISH),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_CLOWNFISH)
	};

	const std::initializer_list<cItem> Flowers =
	{
		SMALLFLOWERS
		TALLFLOWERS
	};

	const std::initializer_list<cItem> GoldOres =
	{
		GOLDORES
	};

	const std::initializer_list<cItem> JungleLogs =
	{
		JUNGLELOGS
	};

	const std::initializer_list<cItem> Leaves =
	{
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_APPLE),
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_BIRCH),
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_CONIFER),
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_JUNGLE),
		cItem(E_BLOCK_NEW_LEAVES, 1, E_META_NEWLEAVES_DARK_OAK),
		cItem(E_BLOCK_NEW_LEAVES, 1, E_META_NEWLEAVES_ACACIA),
	};

	const std::initializer_list<cItem> LecternBooks =
	{
		cItem(E_ITEM_BOOK_AND_QUILL),
		cItem(E_ITEM_WRITTEN_BOOK)
	};

	const std::initializer_list<cItem> Logs =
	{
		LOGSTHATBURN
		CRIMSONSTEMS
		WARPEDSTEMS
	};

	const std::initializer_list<cItem> LogsThatBurn =
	{
		LOGSTHATBURN
	};

	const std::initializer_list<cItem> MusicDiscs =
	{
		CREEPERDROPMUSICDISCS
		// cItem(E_ITEM_DISC_PIGSTEP)
	};
	const std::initializer_list<cItem> NonFlammableWood =
	{
		/* TODO: 07.09.2020 - Add when implemented - 12xx12
		cItem(E_BLOCK_WARPED_STEM),
		cItem(E_BLOCK_STRIPPED_WARPED_STEM),
		cItem(E_BLOCK_WARPED_HYPHAE),
		cItem(E_BLOCK_STRIPPED_WARPED_HYPHAE),
		cItem(E_BLOCK_CRIMSON_STEM),
		cItem(E_BLOCK_STRIPPED_CRIMSON_STEM),
		cItem(E_BLOCK_CRIMSON_HYPHAE),
		cItem(E_BLOCK_STRIPPED_CRIMSON_HYPHAE),
		cItem(E_BLOCK_CRIMSON_PLANKS),
		cItem(E_BLOCK_WARPED_PLANKS),
		cItem(E_BLOCK_CRIMSON_SLAB),
		cItem(E_BLOCK_WARPED_SLAB),
		cItem(E_BLOCK_CRIMSON_PRESSURE_PLATE),
		cItem(E_BLOCK_WARPED_PRESSURE_PLATE),
		cItem(E_BLOCK_CRIMSON_FENCE),
		cItem(E_BLOCK_WARPED_FENCE),
		cItem(E_BLOCK_CRIMSON_TRAPDOOR),
		cItem(E_BLOCK_WARPED_TRAPDOOR),
		cItem(E_BLOCK_CRIMSON_FENCE_GATE),
		cItem(E_BLOCK_WARPED_FENCE_GATE),
		cItem(E_BLOCK_CRIMSON_STAIRS),
		cItem(E_BLOCK_WARPED_STAIRS),
		cItem(E_BLOCK_CRIMSON_BUTTON),
		cItem(E_BLOCK_WARPED_BUTTON),
		cItem(E_BLOCK_CRIMSON_DOOR),
		cItem(E_BLOCK_WARPED_DOOR),
		cItem(E_BLOCK_CRIMSON_SIGN),
		cItem(E_BLOCK_WARPED_SIGN),
		*/
	};

	const std::initializer_list<cItem> OakLogs =
	{
		OAKLOGS
	};

	const std::initializer_list<cItem> PiglinLoved =
	{
		GOLDORES
		cItem(E_BLOCK_GOLD_BLOCK),
		// cItem(E_BOCK_GILDED_BLACKSTONE),
		cItem(E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE),
		cItem(E_ITEM_GOLD),
		// cItem(E_BLOCK_BELL),
		cItem(E_ITEM_CLOCK),
		cItem(E_ITEM_GOLDEN_CARROT),
		cItem(E_ITEM_GLISTERING_MELON),
		cItem(E_ITEM_GOLDEN_APPLE, 1, E_META_GOLDEN_APPLE_NORMAL),
		cItem(E_ITEM_GOLDEN_APPLE, 1, E_META_GOLDEN_APPLE_ENCHANTED),
		cItem(E_ITEM_GOLD_HELMET),
		cItem(E_ITEM_GOLD_CHESTPLATE),
		cItem(E_ITEM_GOLD_LEGGINGS),
		cItem(E_ITEM_GOLD_BOOTS),
		cItem(E_ITEM_GOLD_HORSE_ARMOR),
		cItem(E_ITEM_GOLD_SWORD),
		cItem(E_ITEM_GOLD_PICKAXE),
		cItem(E_ITEM_GOLD_SHOVEL),
		cItem(E_ITEM_GOLD_AXE),
		cItem(E_ITEM_GOLD_HOE),
	};

	const std::initializer_list<cItem> PiglinRepellents =
	{
		// cItem(E_BLOCK_SOUL_TORCH),
		// cItem(E_BLOCK_SOUL_LANTERN),
		// cItem(C_BLOCK_SOUL_CAMPFIRE),
	};

	const std::initializer_list<cItem> Planks =
	{
		cItem(E_BLOCK_PLANKS, 1, E_META_PLANKS_ACACIA),
		cItem(E_BLOCK_PLANKS, 1, E_META_PLANKS_BIRCH),
		cItem(E_BLOCK_PLANKS, 1, E_META_PLANKS_DARK_OAK),
		cItem(E_BLOCK_PLANKS, 1, E_META_PLANKS_JUNGLE),
		cItem(E_BLOCK_PLANKS, 1, E_META_PLANKS_OAK),
		cItem(E_BLOCK_PLANKS, 1, E_META_PLANKS_SPRUCE),
	};

	const std::initializer_list<cItem> Rails =
	{
		cItem(E_BLOCK_RAIL),
		cItem(E_BLOCK_ACTIVATOR_RAIL),
		cItem(E_BLOCK_DETECTOR_RAIL),
		cItem(E_BLOCK_POWERED_RAIL),
	};

	const std::initializer_list<cItem> Sand =
	{
		cItem(E_BLOCK_SAND, 1, E_META_SAND_NORMAL),
		cItem(E_BLOCK_SAND, 1, E_META_SAND_RED),
	};

	const std::initializer_list<cItem> Saplings =
	{
		cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_ACACIA),
		cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_APPLE),
		cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_BIRCH),
		cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_CONIFER),
		cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_DARK_OAK),
		cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_JUNGLE),
	};

	const std::initializer_list<cItem> Signs =
	{
		// TODO: 07.09.2020 - Add when implemented - 12xx12
		cItem(E_ITEM_SIGN),
		// cItem(E_ITEM_OAK_SIGN),
		// cItem(E_ITEM_SPRUCE_SIGN),
		// cItem(E_ITEM_BIRCH_SIGN),
		// cItem(E_ITEM_ACACIA_SIGN),
		// cItem(E_ITEM_JUNGLE_SIGN),
		// cItem(E_ITEM_DARK_OAK_SIGN),
		// cItem(E_ITEM_CRIMSON_SIGN),
		// cItem(E_ITEM_WARPED_SIGN),
	};

	const std::initializer_list<cItem> Slabs =
	{
		// TODO: 07.09.2020 - Add when implemented - 12xx12
		WOODENSLABS
		cItem(E_BLOCK_STONE_SLAB, 1, E_META_STONE_SLAB_STONE),
		// cItem(E_BLOCK_SMOOTHE_STONE_SLAB),
		cItem(E_BLOCK_STONE_SLAB, 1, E_META_STONE_SLAB_STONE_BRICK),
		cItem(E_BLOCK_STONE_SLAB, 1, E_META_STONE_SLAB_SANDSTONE),
		cItem(E_BLOCK_PURPUR_SLAB),
		cItem(E_BLOCK_STONE_SLAB, 1, E_META_STONE_SLAB_QUARTZ),
		cItem(E_BLOCK_RED_SANDSTONE_SLAB),
		cItem(E_BLOCK_STONE_SLAB, 1, E_META_STONE_SLAB_BRICK),
		cItem(E_BLOCK_STONE_SLAB, 1, E_META_STONE_SLAB_COBBLESTONE),
		cItem(E_BLOCK_STONE_SLAB, 1, E_META_STONE_SLAB_NETHER_BRICK),
		// cItem(E_BLOCK_PETRIFIED_OAK_SLAB)
		// cItem(E_BLOCK_PRISMARINE_SLAB),
		// cItem(E_BLOCK_PRISMARINE_BRICK_SLAB),
		// cItem(E_BLOCK_DARK_PRISMARINE_BRICK_SLAB),
		// cItem(E_BLOCK_POLISHED_GRANITE_SLAB),
		// cItem(E_BLOCK_SMOOTH_RED_SANDSOTNE_SLAB),
		// cItem(E_BLOCK_MOSSY_STONE_BRICK_SLAB),
		// cItem(E_BLOCK_POLISHED_DIORITE_SLAB),
		// cItem(E_BLOCK_MOSSY_COBBLESTONE_SLAB),
		// cItem(E_BLOCK_END_STONE_BRICK_SLAB),
		// cItem(E_BLOCK_SMOOTH_SANDSTONE_SLAB),
		// cItem(E_BLOCK_SMOOTH_QUARTZ_SLAB),
		// cItem(E_BLOCK_GRANITE_SLAB),
		// cItem(E_BLOCK_ANDESITE_SLAB),
		// cItem(E_BLOCK_RED_NETHER_BRICK_SLAB),
		// cItem(E_BLOCK_POLISHED_ANDESITE_SLAB),
		// cItem(E_BLOCK_DIORITE_SLAB),
		// cItem(E_BLOCK_CUT_SANDSTONE_SLAB),
		// cItem(E_BLOCK_CUT_RED_SANDSTONE_SLAB),
		// cItem(E_BLOCK_CUT_RED_SANDSTONE_SLAB),
		// cItem(E_BLOCK_BLACKSTONE_SLAB),
		// cItem(E_BLOCK_POLISHED_BLACKSTONE_BRICK_SLAB),
		// cItem(E_BLOCK_POLISHED_BLACKSTONE_SLAB),
	};

	const std::initializer_list<cItem> SmallFlowers =
	{
		SMALLFLOWERS
	};

	const std::initializer_list<cItem> SoulFireBaseBlocks =
	{
		cItem(E_BLOCK_SOULSAND),
		// cItem(E_BLOCK_SOUL_SOIL),  // TODO: 07.09.2020 - Add when implemented - 12xx12
	};

	const std::initializer_list<cItem> SpruceLogs =
	{
		SPRUCELOGS
	};

	const std::initializer_list<cItem> Stairs =
	{
		// TODO: 07.09.2020 - Add when implemented - 12xx12
		WOODENSTAIRS
		cItem(E_BLOCK_COBBLESTONE_STAIRS),
		cItem(E_BLOCK_SANDSTONE_STAIRS),
		cItem(E_BLOCK_NETHER_BRICK_STAIRS),
		cItem(E_BLOCK_STONE_BRICK_STAIRS),
		cItem(E_BLOCK_BRICK_STAIRS),
		cItem(E_BLOCK_PURPUR_STAIRS),
		cItem(E_BLOCK_QUARTZ_STAIRS),
		cItem(E_BLOCK_RED_SANDSTONE_STAIRS),
		// cItem(E_BLOCK_PRISMARINE_BRICK_STAIRS),
		// cItem(E_BLOCK_PRISMARINE_STAIRS),
		// cItem(E_BLOCK_DARK_PRISMARINE_STAIRS),
		// cItem(E_BLOCK_POLISHED_GRANITE_STAIRS),
		// cItem(E_BLOCK_SMOOTH_RED_SANDSTONE_STAIRS),
		// cItem(E_BLOCK_MOSSY_STONE_BRICK_STAIRS),
		// cItem(E_BLOCK_POLISHED_DIORITE_STAIRS),
		// cItem(E_BLOCK_MOSSY_COBBLESTONE_STAIRS),
		// cItem(E_BLOCK_END_STONE_BRICK_STAIRS),
		// cItem(E_BLOCK_STONE_STAIRS),
		// cItem(E_BLOCK_SMOOTH_SANDSTONE_STAIRS),
		// cItem(E_BLOCK_SMOOTH_QUARTZ_STAIRS),
		// cItem(E_BLOCK_GRANITE_STAIRS),
		// cItem(E_BLOCK_ANDESITE_STAIRS),
		// cItem(E_BLOCK_RED_NETHER_BRICK_STAIRS),
		// cItem(E_BLOCK_POLISHED_ANDESITE_STAIRS),
		// cItem(E_BLOCK_DIORITE_STAIRS),
		// cItem(E_BLOCK_BLACKSTONE_STAIRS),
		// cItem(E_BLOCK_POLISHED_BLACKSTONE_BRICK_STAIRS),
		// cItem(E_BLOCK_POLISHED_BLACKSTONE_STAIRS),
	};

	const std::initializer_list<cItem> StoneBricks =
	{
		// TODO: 07.09.2020 - Add when implemented - 12xx12
		cItem(E_BLOCK_STONE_BRICKS),
		// cItem(E_BLOCK_MOSSY_STONE_BRICKS),
		// cItem(E_BLOCK_CRACKED_STONE_BRICKS),
		// cItem(E_BLOCK_CHISELED_STONE_BRICKS),
	};

	const std::initializer_list<cItem> StoneCraftingMaterials =
	{
		cItem(E_BLOCK_COBBLESTONE),
		// cItem(E_BLOCK_BLACKSTONE),  // TODO: 07.09.2020 - Add when implemented - 12xx12
	};

	const std::initializer_list<cItem> StoneToolMaterials =
	{
		cItem(E_BLOCK_COBBLESTONE),
		// cItem(E_BLOCK_BLACKSTONE),  // TODO: 07.09.2020 - Add when implemented - 12xx12
	};

	const std::initializer_list<cItem> TallFlowers =
	{
		TALLFLOWERS
	};

	const std::initializer_list<cItem> Trapdoors =
	{
		WOODENTRAPDOORS
		cItem(E_BLOCK_IRON_TRAPDOOR),
	};

	const std::initializer_list<cItem> Walls =
	{
		// TODO: 07.09.2020 - Add when implemented - 12xx12
		cItem(E_BLOCK_COBBLESTONE_WALL),
		// cItem(E_BLOCK_MOSSY_COBBLESTONE_WALL),
		// cItem(E_BLOCK_BRICK_WALL),
		// cItem(E_BLOCK_PRISMARINE_WALL),
		// cItem(E_BLOCK_RED_SANDSTONE_WALL),
		// cItem(E_BLOCK_MOSSY_STONE_BRICK_WALL),
		// cItem(E_BLOCK_GRANITE_WALL),
		// cItem(E_BLOCK_STONE_BRICK_WALL),
		// cItem(E_BLOCK_GRANITE_WALL),
		// cItem(E_BLOCK_STONE_BRICK_WALL),
		// cItem(E_BLOCK_NETHER_BRICK_WALL),
		// cItem(E_BLOCK_ANDESITE_WALL),
		// cItem(E_BLOCK_RED_NETHER_BRICK_WALL),
		// cItem(E_BLOCK_SANDSTONE_WALL),
		// cItem(E_BLOCK_END_STONE_BRICK_WALL),
		// cItem(E_BLOCK_DIORITE_WALL),
		// cItem(E_BLOCK_BLACKSTONE_WALL),
		// cItem(E_BLOCK_POLISHED_BLACKSTONE_BRICK_WALL),
		// cItem(E_BLOCK_POLISHED_BLACKSTONE_WALL),
	};

	const std::initializer_list<cItem> WarpedStems =
	{
		WARPEDSTEMS
	};

	const std::initializer_list<cItem> WoodenButtons =
	{
		WOODENBUTTONS
	};

	const std::initializer_list<cItem> WoodenDoors =
	{
		WOODENDOORS
	};

	const std::initializer_list<cItem> WoodenFences =
	{
		WOODENFENCES
	};

	const std::initializer_list<cItem> WoodenPressurePlates =
	{
		// TODO: 07.09.2020 - Add when implemented - 12xx12
		cItem(E_BLOCK_WOODEN_PRESSURE_PLATE),
		// cItem(E_BLOCK_OAK_PRESSURE_PLATE),
		// cItem(E_BLOCK_SPRUCE_PRESSURE_PLATE),
		// cItem(E_BLOCK_BIRCH_PRESSURE_PLATE),
		// cItem(E_BLOCK_JUNGLE_PRESSURE_PLATE),
		// cItem(E_BLOCK_ACACIA_PRESSURE_PLATE),
		// cItem(E_BLOCK_DARK_OAK_PRESSURE_PLATE),
		// cItem(E_BLOCK_CRIMSON_PRESSURE_PLATE),
		// cItem(E_BLOCK_WARPED_PRESSURE_PLATE),
	};

	const std::initializer_list<cItem> WoodenSlabs =
	{
		WOODENSLABS
	};

	const std::initializer_list<cItem> WoodenStairs =
	{
		WOODENSTAIRS
	};

	const std::initializer_list<cItem> WoodenTrapdoors =
	{
		WOODENTRAPDOORS
	};

	const std::initializer_list<cItem> Wool =
	{
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_BLACK),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_BLUE),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_BROWN),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_CYAN),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_GRAY),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_GREEN),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_LIGHTBLUE),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_LIGHTGRAY),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_LIGHTGREEN),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_MAGENTA),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_ORANGE),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_PINK),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_PURPLE),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_RED),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_WHITE),
		cItem(E_BLOCK_WOOL, 1, E_META_WOOL_YELLOW),
	};

	/** Returns a cItemObject which contains all the items described by the tag */
	cItems GetItems(enum eItemTags a_Tag)
	{
		switch (a_Tag)
		{
			case eItemTags::AcaciaLogs:             return cItems(AcaciaLogs);
			case eItemTags::Anvil:                  return cItems(Anvil);
			case eItemTags::Arrows:                 return cItems(Arrows);
			case eItemTags::Banners:                return cItems(Banners);
			case eItemTags::BeaconPaymentItems:     return cItems(BeaconPaymentItems);
			case eItemTags::Beds:                   return cItems(Beds);
			case eItemTags::BirchLogs:              return cItems(BirchLogs);
			case eItemTags::Boats:                  return cItems(Boats);
			case eItemTags::Buttons:                return cItems(Buttons);
			case eItemTags::Carpets:                return cItems(Carpets);
			case eItemTags::Coals:                  return cItems(Coals);
			case eItemTags::CreeperDropsMusicDiscs: return cItems(CreeperDropsMusicDiscs);
			case eItemTags::CrimsonStems:           return cItems(CrimsonStems);
			case eItemTags::DarkOakLogs:            return cItems(DarkOakLogs);
			case eItemTags::Doors:                  return cItems(Doors);
			case eItemTags::Flowers:                return cItems(Flowers);
			case eItemTags::Fences:                 return cItems(Fences);
			case eItemTags::Fishes:                 return cItems(Fishes);
			case eItemTags::GoldOres:               return cItems(GoldOres);
			case eItemTags::JungleLogs:             return cItems(JungleLogs);
			case eItemTags::Leaves:                 return cItems(Leaves);
			case eItemTags::LecternBooks:           return cItems(LecternBooks);
			case eItemTags::Logs:                   return cItems(Logs);
			case eItemTags::LogsThatBurn:           return cItems(LogsThatBurn);
			case eItemTags::MusicDiscs:             return cItems(MusicDiscs);
			case eItemTags::NonFlammableWood:       return cItems(NonFlammableWood);
			case eItemTags::OakLogs:                return cItems(OakLogs);
			case eItemTags::PiglinLoved:            return cItems(PiglinLoved);
			case eItemTags::PiglinRepellents:       return cItems(PiglinRepellents);
			case eItemTags::Planks:                 return cItems(Planks);
			case eItemTags::Rails:                  return cItems(Rails);
			case eItemTags::Sand:                   return cItems(Sand);
			case eItemTags::Saplings:               return cItems(Saplings);
			case eItemTags::Signs:                  return cItems(Signs);
			case eItemTags::Slabs:                  return cItems(Slabs);
			case eItemTags::SmallFlowers:           return cItems(SmallFlowers);
			case eItemTags::SoulFireBaseBlocks:     return cItems(SoulFireBaseBlocks);
			case eItemTags::SpruceLogs:             return cItems(SpruceLogs);
			case eItemTags::Stairs:                 return cItems(Stairs);
			case eItemTags::StoneBricks:            return cItems(StoneBricks);
			case eItemTags::StoneCraftingMaterials: return cItems(StoneCraftingMaterials);
			case eItemTags::StoneToolMaterials:     return cItems(StoneToolMaterials);
			case eItemTags::TallFlowers:            return cItems(TallFlowers);
			case eItemTags::TrapDoors:              return cItems(Trapdoors);
			case eItemTags::Walls:                  return cItems(Walls);
			case eItemTags::WarpedStems:            return cItems(WarpedStems);
			case eItemTags::WoodenButtons:          return cItems(WoodenButtons);
			case eItemTags::WoodenDoors:            return cItems(WoodenDoors);
			case eItemTags::WoodenFences:           return cItems(WoodenFences);
			case eItemTags::WoodenPressurePlates:   return cItems(WoodenPressurePlates);
			case eItemTags::WoodenSlabs:            return cItems(WoodenSlabs);
			case eItemTags::WoodenStairs:           return cItems(WoodenStairs);
			case eItemTags::WoodenTrapDoors:        return cItems(WoodenTrapdoors);
			case eItemTags::Wool:                   return cItems(Wool);
			default:                                return cItems();
		}
	}
}
