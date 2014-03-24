/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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
	return biInvalidBiome;
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
