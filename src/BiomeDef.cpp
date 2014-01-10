
// BiomeDef.cpp

// Implements biome helper functions

#include "Globals.h"
#include "BiomeDef.h"


EMCSBiome StringToBiome(const AString & a_BiomeString)
{
	// If it is a number, return it:
	int res = atoi(a_BiomeString.c_str());
	if ((res != 0) || (a_BiomeString.compare("0") == 0))
	{
		// It was a valid number
		return (EMCSBiome)res;
	}
	
	// Convert using the built-in map:
	static struct {
		EMCSBiome    m_Biome;
		const char * m_String;
	} BiomeMap[] =
	{
		{biOcean,            "Ocean"} ,
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
	
	for (size_t i = 0; i < ARRAYCOUNT(BiomeMap); i++)
	{
		if (NoCaseCompare(BiomeMap[i].m_String, a_BiomeString) == 0)
		{
			return BiomeMap[i].m_Biome;
		}
	}  // for i - BiomeMap[]
	return (EMCSBiome)-1;
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
