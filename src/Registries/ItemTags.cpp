
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
