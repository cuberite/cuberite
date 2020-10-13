
// ItemTags.cpp

#include "Globals.h"

#include "ItemTags.h"

namespace ItemTag
{

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
	cItems GetItems(enum eItemTags a_Tag)
	{
		switch (a_Tag)
		{
			case eItemTags::AcaciaLogs:             return cItems({ACACIALOGS_TAG});
			case eItemTags::Anvil:                  return cItems({ANVIL_TAG});
			case eItemTags::Arrows:                 return cItems({ARROWS_TAG});
			case eItemTags::Banners:                return cItems({BANNERS_TAG});
			case eItemTags::BeaconPaymentItems:     return cItems({BEACONPAYMENTITEMS_TAG});
			case eItemTags::Beds:                   return cItems({BEDS_TAG});
			case eItemTags::BirchLogs:              return cItems({BIRCHLOGS_TAG});
			case eItemTags::Boats:                  return cItems({BOATS_TAG});
			case eItemTags::Buttons:                return cItems({BUTTONS_TAG});
			case eItemTags::Carpets:                return cItems({CARPETS_TAG});
			case eItemTags::Coals:                  return cItems({COALS_TAG});
			case eItemTags::CreeperDropsMusicDiscs: return cItems({CREEPERDROPMUSICDISCS_TAG});
			case eItemTags::CrimsonStems:           return cItems({CRIMSONSTEMS_TAG});
			case eItemTags::DarkOakLogs:            return cItems({DARKOAKLOGS_TAG});
			case eItemTags::Doors:                  return cItems({DOORS_TAG});
			case eItemTags::Flowers:                return cItems({FLOWERS_TAG});
			case eItemTags::Fences:                 return cItems({FENCES_TAG});
			case eItemTags::Fishes:                 return cItems({FISHES_TAG});
			case eItemTags::GoldOres:               return cItems({GOLDORES_TAG});
			case eItemTags::JungleLogs:             return cItems({JUNGLELOGS_TAG});
			case eItemTags::Leaves:                 return cItems({LEAVES_TAG});
			case eItemTags::LecternBooks:           return cItems({LECTERNBOOKS_TAG});
			case eItemTags::Logs:                   return cItems({LOGS_TAG});
			case eItemTags::LogsThatBurn:           return cItems({LOGSTHATBURN_TAG});
			case eItemTags::MusicDiscs:             return cItems({MUSICDISCS_TAG});
			case eItemTags::NonFlammableWood:       return cItems({NONFLAMEABLEWOOD_TAG});
			case eItemTags::OakLogs:                return cItems({OAKLOGS_TAG});
			case eItemTags::PiglinLoved:            return cItems({PIGLINLOVED_TAG});
			case eItemTags::PiglinRepellents:       return cItems({PIGLINREPELLANTS_TAG});
			case eItemTags::Planks:                 return cItems({PLANKS_TAG});
			case eItemTags::Rails:                  return cItems({RAILS_TAG});
			case eItemTags::Sand:                   return cItems({SAND_TAG});
			case eItemTags::Saplings:               return cItems({SAPLINGS_TAG});
			case eItemTags::Signs:                  return cItems({SIGNS_TAG});
			case eItemTags::Slabs:                  return cItems({SLABS_TAG});
			case eItemTags::SmallFlowers:           return cItems({SMALLFLOWERS_TAG});
			case eItemTags::SoulFireBaseBlocks:     return cItems({SOULFIREBASEBLOCKS_TAG});
			case eItemTags::SpruceLogs:             return cItems({SPRUCELOGS_TAG});
			case eItemTags::Stairs:                 return cItems({STAIRS_TAG});
			case eItemTags::StoneBricks:            return cItems({STONEBRICKS_TAG});
			case eItemTags::StoneCraftingMaterials: return cItems({STONECRAFTINGMATERIALS_TAG});
			case eItemTags::StoneToolMaterials:     return cItems({STONETOOLMATERIALS_TAG});
			case eItemTags::TallFlowers:            return cItems({TALLFLOWERS_TAG});
			case eItemTags::TrapDoors:              return cItems({TRAPDOORS_TAG});
			case eItemTags::Walls:                  return cItems({WALLS_TAG});
			case eItemTags::WarpedStems:            return cItems({WARPEDSTEMS_TAG});
			case eItemTags::WoodenButtons:          return cItems({WOODENBUTTONS_TAG});
			case eItemTags::WoodenDoors:            return cItems({WOODENDOORS_TAG});
			case eItemTags::WoodenFences:           return cItems({WOODENFENCES_TAG});
			case eItemTags::WoodenPressurePlates:   return cItems({WOODENPREASSUREPLATES_TAG});
			case eItemTags::WoodenSlabs:            return cItems({WOODENSLABS_TAG});
			case eItemTags::WoodenStairs:           return cItems({WOODENSTAIRS_TAG});
			case eItemTags::WoodenTrapDoors:        return cItems({WOODENTRAPDOORS_TAG});
			case eItemTags::Wool:                   return cItems({WOOL_TAG});
			default:                                return cItems();
		}
	}
}
