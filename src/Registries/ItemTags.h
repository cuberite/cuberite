
#pragma once

// This list is up to date with 1.16.2 if there are new added please add here:
// https://minecraft.gamepedia.com/Tag#Items
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
	};

	enum eItemTags eItemTags(AString & a_String)
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
	}

// TODO: 06.09.2020 Add when implemented - 12xx12
#define ACACIALOGS \
	cItem(E_BLOCK_NEW_LOG, 1, E_META_NEW_LOG_ACACIA_WOOD),
	// cItem(E_BLOCK_ACACIA_WOOD), \          // The Wood with the outside texture on all sides
	// cItem(E_BLOCK_STRIPPED_ACACIA_LOG), \  // The stripped default log
	// cItem(E_BLOCK_STRIPPED_ACACIA_WOOD) \  // The stripped wood

	const std::vector<cItem> AcaciaLogs =
	{
		ACACIALOGS
	};

	const std::vector<cItem> Anvil =
	{
		cItem(E_BLOCK_ANVIL, 1, E_BLOCK_ANVIL_NO_DAMAGE),
		cItem(E_BLOCK_ANVIL, 1, E_BLOCK_ANVIL_LOW_DAMAGE),
		cItem(E_BLOCK_ANVIL, 1, E_BLOCK_ANVIL_HIGH_DAMAGE)
	};

	const std::vector<cItem> Arrows =
	{
		cItem(E_ITEM_ARROW),
		cItem(E_ITEM_SPECTRAL_ARROW),
		cItem(E_ITEM_TIPPED_ARROW)
	};

	const std::vector<cItem> Banners =
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

	const std::vector<cItem> BeaconPaymentItems =
	{
		cItem(E_ITEM_IRON),
		cItem(E_ITEM_GOLD),
		cItem(E_ITEM_DIAMOND),
		cItem(E_ITEM_EMERALD),
		// cItem(E_ITEM_NETHERITE)  // TODO: 06.09.2020 Add when implemented - 12xx12
	};

	const std::vector<cItem> Beds =
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

// TODO: 06.09.2020 Add when implemented - 12xx12
#define BIRCHLOGS \
	cItem(E_BLOCK_LOG, 1, E_META_LOG_BIRCH),
	// cItem(E_BLOCK_BIRCH_WOOD), \          // The Wood with the outside texture on all sides
	// cItem(E_BLOCK_STRIPPED_BIRCH_LOG), \  // The stripped default log
	// cItem(E_BLOCK_STRIPPED_BIRCH_WOOD)    // The stripped wood

	const std::vector<cItem> BirchLogs =
	{
		BIRCHLOGS
	};

	const std::vector<cItem> Boats =
	{
		cItem(E_ITEM_BOAT),
		cItem(E_ITEM_ACACIA_BOAT),
		cItem(E_ITEM_BIRCH_BOAT),
		cItem(E_ITEM_JUNGLE_BOAT),
		cItem(E_ITEM_SPRUCE_BOAT),
		cItem(E_ITEM_DARK_OAK_BOAT)
	};

	const std::vector<cItem> Buttons =
	{
		cItem(E_BLOCK_STONE_BUTTON),
		cItem(E_BLOCK_WOODEN_BUTTON)
		// TODO: 06.09.2020 - Add other buttons when implemented - 12xx12
	};

	const std::vector<cItem> Carpets =
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

	const std::vector<cItem> Coals =
	{
		cItem(E_ITEM_COAL, 1, E_META_COAL_NORMAL),
		cItem(E_ITEM_COAL, 1, E_META_COAL_CHARCOAL)
	};

	const std::vector<cItem> CreeperDropsMusicDiscs =
	{
		cItem(E_ITEM_13_DISC),
		cItem(E_ITEM_CAT_DISC),
		cItem(E_ITEM_BLOCKS_DISC),
		cItem(E_ITEM_CHIRP_DISC),
		cItem(E_ITEM_FAR_DISC),
		cItem(E_ITEM_MALL_DISC),
		cItem(E_ITEM_MELLOHI_DISC),
		cItem(E_ITEM_STAL_DISC),
		cItem(E_ITEM_STRAD_DISC),
		cItem(E_ITEM_WARD_DISC),
		cItem(E_ITEM_11_DISC),
		cItem(E_ITEM_WAIT_DISC),
	};

// TODO: 06.09.2020 - Add when implemented - 12xx12
#define CRIMSONSTEMS \
	// cItem(E_BLOCK_CRIMSON_STEM),
	// cItem(E_BLOCK_STRIPPED_CRIMSON_STEM),
	// cItem(E_BLOCK_CRIMSON_HYPHAE),
	// cItem(E_BLOCK_STRIPPED_CRIMSON_HYPHAE),

	const std::vector<cItem> CrimsonStems =
	{
		CRIMSONSTEMS
	};

// TODO: 06.09.2020 - Add when implemented - 12xx12
#define DARKOAKLOGS \
	cItem(E_BLOCK_NEW_LOG, 1, E_META_NEW_LOG_DARK_OAK_WOOD),
	// cItem(E_BLOCK_STRIPPED_DARK_OAK),
	// cItem(E_BLOCK_DARK_OAK_WOOD),
	// cItem(E_BLOCK_STRIPPED_DARK_OAK_WOOD),                \

	const std::vector<cItem> DarkOakLogs =
	{
		DARKOAKLOGS
	};

	const std::vector<cItem> Doors =
	{
		cItem(E_BLOCK_ACACIA_DOOR),
		cItem(E_BLOCK_BIRCH_DOOR),
		cItem(E_BLOCK_IRON_DOOR),
		cItem(E_BLOCK_JUNGLE_DOOR),
		cItem(E_BLOCK_OAK_DOOR),
		cItem(E_BLOCK_SPRUCE_DOOR),
		cItem(E_BLOCK_WOODEN_DOOR),
	};

	const std::vector<cItem> Fences =
	{  // TODO: 06.09.2020 - Add other when implemented - 12xx12
		cItem(E_BLOCK_FENCE),
		cItem(E_BLOCK_ACACIA_FENCE),
		cItem(E_BLOCK_BIRCH_FENCE),
		cItem(E_BLOCK_JUNGLE_FENCE),
		cItem(E_BLOCK_SPRUCE_FENCE),
		cItem(E_BLOCK_DARK_OAK_FENCE),
		cItem(E_BLOCK_NETHER_BRICK_FENCE),
	};

	const std::vector<cItem> Fishes =
	{
		cItem(E_ITEM_RAW_FISH, 1, E_META_COOKED_FISH_FISH),
		cItem(E_ITEM_RAW_FISH, 1, E_META_COOKED_FISH_SALMON),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_FISH),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_SALMON),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_CLOWNFISH),
		cItem(E_ITEM_COOKED_FISH, 1, E_META_RAW_FISH_CLOWNFISH)
	};

	const std::vector<cItem> GoldOres =
	{
		cItem(E_BLOCK_GOLD_ORE),
		// cItem(E_BLOCK_NETHER_GOLD_ORE),  // TODO: 06.09.2020 - Add when implemented - 12xx12
	};

// TODO: 06.09.2020 - Add when implemented - 12xx12
#define JUNGLELOGS \
	cItem(E_BLOCK_LOG, 1, E_META_LOG_JUNGLE),
	// cItem(E_BLOCK_STRIPPED_JUNGLE_LOG), \
	// cItem(E_BLOCK_JUNGLE_WOOD), \
	// cItem(E_BLOCK_STRIPPED_JUNGLE_WOOD), \

	const std::vector<cItem> JungleLogs =
	{
		JUNGLELOGS
	};

	const std::vector<cItem> Leaves =
	{
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_APPLE),
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_BIRCH),
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_CONIFER),
		cItem(E_BLOCK_LEAVES, 1, E_META_LEAVES_JUNGLE),
		cItem(E_BLOCK_NEW_LEAVES, 1, E_META_NEWLEAVES_DARK_OAK),
		cItem(E_BLOCK_NEW_LEAVES, 1, E_META_NEWLEAVES_ACACIA),
	};

	const std::vector<cItem> LecternBooks =
	{
		cItem(E_ITEM_BOOK_AND_QUILL),
		cItem(E_ITEM_WRITTEN_BOOK)
	};

	const std::vector<cItem> Logs =
	{
		ACACIALOGS
		BIRCHLOGS
		JUNGLELOGS

	};

	const std::vector<cItem> * ItemTags(enum eItemTags a_Tag)
	{
		switch (a_Tag)
		{
			case eItemTags::AcaciaLogs:             return & AcaciaLogs;
			case eItemTags::Anvil:                  return & Anvil;
			case eItemTags::Arrows:                 return & Arrows;
			case eItemTags::Banners:                return & Banners;
			case eItemTags::BeaconPaymentItems:     return & BeaconPaymentItems;
			case eItemTags::Beds:                   return & Beds;
			case eItemTags::BirchLogs:              return & BirchLogs;
			case eItemTags::Boats:                  return & Boats;
			case eItemTags::Buttons:                return & Buttons;
			case eItemTags::Carpets:                return & Carpets;
			case eItemTags::Coals:                  return & Coals;
			case eItemTags::CreeperDropsMusicDiscs: return & CreeperDropsMusicDiscs;
			case eItemTags::CrimsonStems:           return & CrimsonStems;
			case eItemTags::DarkOakLogs:            return & DarkOakLogs;
			case eItemTags::Doors:                  return & Doors;
			case eItemTags::Fences:                 return & Fences;
		}
	}
}
