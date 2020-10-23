
// ItemTags.cpp

#include "Globals.h"

#include "ItemTags.h"

namespace ItemTag
{
	enum eItemTags eItemTags(std::string_view a_String)
	{
		const static std::map<std::string_view, enum eItemTags> ItemTags =
		{
			{ "acacia_logs",               eItemTags::AcaciaLogs },
			{ "anvil",                     eItemTags::Anvil },
			{ "arrows",                    eItemTags::Arrows },
			{ "banners",                   eItemTags::Banners },
			{ "beacon_payment_items",      eItemTags::BeaconPaymentItems },
			{ "beds",                      eItemTags::Beds },
			{ "birch_logs",                eItemTags::BirchLogs },
			{ "boats",                     eItemTags::Boats },
			{ "buttons",                   eItemTags::Buttons },
			{ "carpets",                   eItemTags::Carpets },
			{ "coals",                     eItemTags::Coals },
			{ "creeper_drops_music_discs", eItemTags::CreeperDropsMusicDiscs },
			{ "crimson_stems",             eItemTags::CrimsonStems },
			{ "dark_oak_logs",             eItemTags::DarkOakLogs },
			{ "doors",                     eItemTags::Doors },
			{ "fences",                    eItemTags::Fences },
			{ "fishes",                    eItemTags::Fishes },
			{ "flowers",                   eItemTags::Flowers },
			{ "gold_ores",                 eItemTags::GoldOres },
			{ "jungle_logs",               eItemTags::JungleLogs },
			{ "leaves",                    eItemTags::Leaves },
			{ "fences",                    eItemTags::Fences },
			{ "lectern_books",             eItemTags::LecternBooks },
			{ "logs",                      eItemTags::Logs },
			{ "logs_that_burn",            eItemTags::LogsThatBurn },
			{ "music_discs",               eItemTags::MusicDiscs },
			{ "non_flammable_wood",        eItemTags::NonFlammableWood },
			{ "oak_logs",                  eItemTags::OakLogs },
			{ "piglin_loved",              eItemTags::PiglinLoved },
			{ "piglin_repellents",         eItemTags::PiglinRepellents },
			{ "planks",                    eItemTags::Planks },
			{ "rails",                     eItemTags::Rails },
			{ "sand",                      eItemTags::Sand },
			{ "saplings",                  eItemTags::Saplings },
			{ "signs",                     eItemTags::Signs },
			{ "slabs",                     eItemTags::Slabs },
			{ "small_flowers",             eItemTags::SmallFlowers },
			{ "soul_fire_base_blocks",     eItemTags::SoulFireBaseBlocks },
			{ "spruce_logs",               eItemTags::SpruceLogs },
			{ "stairs",                    eItemTags::Stairs },
			{ "stone_bricks",              eItemTags::StoneBricks },
			{ "stone_crafting_materials",  eItemTags::StoneCraftingMaterials },
			{ "stone_tool_materials",      eItemTags::StoneToolMaterials },
			{ "tall__flowers",             eItemTags::TallFlowers },
			{ "trap_doors",                eItemTags::TrapDoors },
			{ "walls",                     eItemTags::Walls },
			{ "warped_stems",              eItemTags::WarpedStems },
			{ "wooden_buttons",            eItemTags::WoodenButtons },
			{ "wooden_doors",              eItemTags::WoodenDoors },
			{ "wooden_fences",             eItemTags::WoodenFences },
			{ "wooden_pressure_plates",    eItemTags::WoodenPressurePlates },
			{ "wooden_slabs",              eItemTags::WoodenSlabs },
			{ "wooden_stairs",             eItemTags::WoodenStairs },
			{ "wooden_trap_doors",         eItemTags::WoodenTrapDoors },
			{ "wool",                      eItemTags::Wool },
		};
		try
		{
			return ItemTags.at(a_String);
		}
		catch (...)
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
