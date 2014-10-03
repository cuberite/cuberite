
// BiomeDef.cpp

// Implements biome helper functions

#include "Globals.h"
#include "BiomeDef.h"




// The "map" used for biome <-> string conversions:
static struct
{
	EMCSBiome    m_Biome;
	const char * m_String;
} g_BiomeMap[] =
{
	{biOcean,            "Ocean"},
	{biPlains,           "Plains"},
	{biDesert,           "Desert"},
	{biExtremeHills,     "ExtremeHills"},
	{biForest,           "Forest"},
	{biTaiga,            "Taiga"},
	{biSwampland,        "Swampland"},
	{biRiver,            "River"},
	{biNether,           "Hell"},
	{biNether,           "Nether"},
	{biEnd,              "Sky"},
	{biEnd,              "End"},
	{biFrozenOcean,      "FrozenOcean"},
	{biFrozenRiver,      "FrozenRiver"},
	{biIcePlains,        "IcePlains"},
	{biIcePlains,        "Tundra"},
	{biIceMountains,     "IceMountains"},
	{biMushroomIsland,   "MushroomIsland"},
	{biMushroomShore,    "MushroomShore"},
	{biBeach,            "Beach"},
	{biDesertHills,      "DesertHills"},
	{biForestHills,      "ForestHills"},
	{biTaigaHills,       "TaigaHills"},
	{biExtremeHillsEdge, "ExtremeHillsEdge"},
	{biJungle,           "Jungle"},
	{biJungleHills,      "JungleHills"},
	
	// Release 1.7 biomes:
	{biJungleEdge,       "JungleEdge"},
	{biDeepOcean,        "DeepOcean"},
	{biStoneBeach,       "StoneBeach"},
	{biColdBeach,        "ColdBeach"},
	{biBirchForest,      "BirchForest"},
	{biBirchForestHills, "BirchForestHills"},
	{biRoofedForest,     "RoofedForest"},
	{biColdTaiga,        "ColdTaiga"},
	{biColdTaigaHills,   "ColdTaigaHills"},
	{biMegaTaiga,        "MegaTaiga"},
	{biMegaTaigaHills,   "MegaTaigaHills"},
	{biExtremeHillsPlus, "ExtremeHillsPlus"},
	{biSavanna,          "Savanna"},
	{biSavannaPlateau,   "SavannaPlateau"},
	{biMesa,             "Mesa"},
	{biMesaPlateauF,     "MesaPlateauF"},
	{biMesaPlateau,      "MesaPlateau"},
	
	// Release 1.7 variants:
	{biSunflowerPlains,      "SunflowerPlains"},
	{biDesertM,              "DesertM"},
	{biExtremeHillsM,        "ExtremeHillsM"},
	{biFlowerForest,         "FlowerForest"},
	{biTaigaM,               "TaigaM"},
	{biSwamplandM,           "SwamplandM"},
	{biIcePlainsSpikes,      "IcePlainsSpikes"},
	{biJungleM,              "JungleM"},
	{biJungleEdgeM,          "JungleEdgeM"},
	{biBirchForestM,         "BirchForestM"},
	{biBirchForestHillsM,    "BirchForestHillsM"},
	{biRoofedForestM,        "RoofedForestM"},
	{biColdTaigaM,           "ColdTaigaM"},
	{biMegaSpruceTaiga,      "MegaSpruceTaiga"},
	{biMegaSpruceTaigaHills, "MegaSpruceTaigaHills"},
	{biExtremeHillsPlusM,    "ExtremeHillsPlusM"},
	{biSavannaM,             "SavannaM"},
	{biSavannaPlateauM,      "SavannaPlateauM"},
	{biMesaBryce,            "MesaBryce"},
	{biMesaPlateauFM,        "MesaPlateauFM"},
	{biMesaPlateauM,         "MesaPlateauM"},
} ;





EMCSBiome StringToBiome(const AString & a_BiomeString)
{
	// If it is a number, return it:
	int res = atoi(a_BiomeString.c_str());
	if ((res != 0) || (a_BiomeString.compare("0") == 0))
	{
		if ((res >= biFirstBiome) && (res < biNumBiomes))
		{
			return (EMCSBiome)res;
		}
		else if ((res >= biFirstVariantBiome) && (res < biNumVariantBiomes))
		{
			return (EMCSBiome)res;
		}
		// It was an invalid number
		return biInvalidBiome;
	}
	
	for (size_t i = 0; i < ARRAYCOUNT(g_BiomeMap); i++)
	{
		if (NoCaseCompare(g_BiomeMap[i].m_String, a_BiomeString) == 0)
		{
			return g_BiomeMap[i].m_Biome;
		}
	}  // for i - BiomeMap[]
	return biInvalidBiome;
}





AString BiomeToString(int a_Biome)
{
	for (size_t i = 0; i < ARRAYCOUNT(g_BiomeMap); i++)
	{
		if (g_BiomeMap[i].m_Biome == a_Biome)
		{
			return g_BiomeMap[i].m_String;
		}
	}
	return AString();
}





bool IsBiomeNoDownfall(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biDesert:
		case biDesertHills:
		case biDesertM:
		case biSavanna:
		case biSavannaM:
		case biSavannaPlateau:
		case biSavannaPlateauM:
		case biNether:
		case biEnd:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBiomeVeryCold(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biFrozenOcean:
		case biFrozenRiver:
		case biIcePlains:
		case biIceMountains:
		case biColdBeach:
		case biColdTaiga:
		case biColdTaigaHills:
		case biIcePlainsSpikes:
		case biColdTaigaM:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}




bool IsBiomeCold(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biExtremeHills:
		case biTaiga:
		case biTaigaHills:
		case biExtremeHillsEdge:
		case biStoneBeach:
		case biMegaTaiga:
		case biMegaTaigaHills:
		case biExtremeHillsPlus:
		case biExtremeHillsM:
		case biTaigaM:
		case biColdTaigaM:
		case biMegaSpruceTaiga:
		case biMegaSpruceTaigaHills:
		case biExtremeHillsPlusM:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}




