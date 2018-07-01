
// BiomeDef.cpp

// Implements biome helper functions

#include "Globals.h"
#include <unordered_map>




struct BiomePair
{
	const char *m_String;
	EMCSBiome m_Biome;
};
// The map used for biome <-> string conversions:
static const std::array<BiomePair, 64> g_BiomeMap =
{
	{
		{"Ocean",                biOcean                },
		{"Plains",               biPlains               },
		{"Desert",               biDesert               },
		{"ExtremeHills",         biExtremeHills         },
		{"Forest",               biForest               },
		{"Taiga",                biTaiga                },
		{"Swampland",            biSwampland            },
		{"River",                biRiver                },
		{"Hell",                 biNether               },
		{"Nether",               biNether               },
		{"Sky",                  biEnd                  },
		{"End",                  biEnd                  },
		{"FrozenOcean",          biFrozenOcean          },
		{"FrozenRiver",          biFrozenRiver          },
		{"IcePlains",            biIcePlains            },
		{"Tundra",               biIcePlains            },
		{"IceMountains",         biIceMountains         },
		{"MushroomIsland",       biMushroomIsland       },
		{"MushroomShore",        biMushroomShore        },
		{"Beach",                biBeach                },
		{"DesertHills",          biDesertHills          },
		{"ForestHills",          biForestHills          },
		{"TaigaHills",           biTaigaHills           },
		{"ExtremeHillsEdge",     biExtremeHillsEdge     },
		{"Jungle",               biJungle               },
		{"JungleHills",          biJungleHills          },

		// Release 1.7 biomes:
		{"JungleEdge",           biJungleEdge          },
		{"DeepOcean",            biDeepOcean           },
		{"StoneBeach",           biStoneBeach          },
		{"ColdBeach",            biColdBeach           },
		{"BirchForest",          biBirchForest         },
		{"BirchForestHills",     biBirchForestHills    },
		{"RoofedForest",         biRoofedForest        },
		{"ColdTaiga",            biColdTaiga           },
		{"ColdTaigaHills",       biColdTaigaHills      },
		{"MegaTaiga",            biMegaTaiga           },
		{"MegaTaigaHills",       biMegaTaigaHills      },
		{"ExtremeHillsPlus",     biExtremeHillsPlus    },
		{"Savanna",              biSavanna             },
		{"SavannaPlateau",       biSavannaPlateau      },
		{"Mesa",                 biMesa                },
		{"MesaPlateauF",         biMesaPlateauF        },
		{"MesaPlateau",          biMesaPlateau         },

		// Release 1.7 variants:
		{"SunflowerPlains",      biSunflowerPlains     },
		{"DesertM",              biDesertM             },
		{"ExtremeHillsM",        biExtremeHillsM       },
		{"FlowerForest",         biFlowerForest        },
		{"TaigaM",               biTaigaM              },
		{"SwamplandM",           biSwamplandM          },
		{"IcePlainsSpikes",      biIcePlainsSpikes     },
		{"JungleM",              biJungleM             },
		{"JungleEdgeM",          biJungleEdgeM         },
		{"BirchForestM",         biBirchForestM        },
		{"BirchForestHillsM",    biBirchForestHillsM   },
		{"RoofedForestM",        biRoofedForestM       },
		{"ColdTaigaM",           biColdTaigaM          },
		{"MegaSpruceTaiga",      biMegaSpruceTaiga     },
		{"MegaSpruceTaigaHills", biMegaSpruceTaigaHills},
		{"ExtremeHillsPlusM",    biExtremeHillsPlusM   },
		{"SavannaM",             biSavannaM            },
		{"SavannaPlateauM",      biSavannaPlateauM     },
		{"MesaBryce",            biMesaBryce           },
		{"MesaPlateauFM",        biMesaPlateauFM       },
		{"MesaPlateauM",         biMesaPlateauM        },
	}
};





EMCSBiome StringToBiome(const AString & a_BiomeString)
{
	// If it is a number, return it:
	int res = atoi(a_BiomeString.c_str());
	if ((res != 0) || (a_BiomeString.compare("0") == 0))
	{
		if ((res >= biFirstBiome) && (res < biNumBiomes))
		{
			return static_cast<EMCSBiome>(res);
		}
		else if ((res >= biFirstVariantBiome) && (res < biNumVariantBiomes))
		{
			return static_cast<EMCSBiome>(res);
		}
		// It was an invalid number
		return biInvalidBiome;
	}

	for (const auto & Biome : g_BiomeMap)
	{
		if (NoCaseCompare(Biome.m_String, a_BiomeString) == 0)
		{
			return Biome.m_Biome;
		}
	}
	return biInvalidBiome;
}





AString BiomeToString(int a_Biome)
{
	for (const auto & Biome : g_BiomeMap)
	{
		if (Biome.m_Biome == a_Biome)
		{
			return Biome.m_String;
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





int GetSnowStartHeight(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biIcePlainsSpikes:
		case biIcePlains:
		case biIceMountains:
		case biFrozenRiver:
		case biColdBeach:
		case biColdTaiga:
		case biColdTaigaHills:
		case biColdTaigaM:
		{
			// Always snow
			return 0;
		}

		case biExtremeHills:
		case biExtremeHillsM:
		case biExtremeHillsPlus:
		case biExtremeHillsPlusM:
		case biStoneBeach:
		{
			// Starts snowing at 96
			return 96;
		}

		case biTaiga:
		case biTaigaHills:
		case biTaigaM:
		{
			// Start snowing at 130
			return 130;
		}

		case biMegaTaiga:
		case biMegaSpruceTaiga:
		case biMegaTaigaHills:
		case biMegaSpruceTaigaHills:
		{
			// Start snowing at 160
			return 160;
		}

		case biRiver:
		case biOcean:
		case biDeepOcean:
		{
			// Starts snowing at 280
			return 280;
		}

		case biBirchForest:
		case biBirchForestHills:
		case biBirchForestM:
		case biBirchForestHillsM:
		{
			// Starts snowing at 335
			return 335;
		}

		case biForest:
		case biForestHills:
		case biFlowerForest:
		case biRoofedForest:
		case biRoofedForestM:
		{
			// Starts snowing at 400
			return 400;
		}

		case biPlains:
		case biSunflowerPlains:
		case biSwampland:
		case biSwamplandM:
		case biBeach:
		{
			// Starts snowing at 460
			return 460;
		}

		case biMushroomIsland:
		case biMushroomShore:
		{
			// Starts snowing at 520
			return 520;
		}

		case biJungle:
		case biJungleHills:
		case biJungleM:
		case biJungleEdge:
		case biJungleEdgeM:
		{
			// Starts snowing at 550
			return 550;
		}

		case biDesert:
		case biDesertHills:
		case biDesertM:
		case biSavanna:
		case biSavannaM:
		case biSavannaPlateau:
		case biSavannaPlateauM:
		case biMesa:
		case biMesaBryce:
		case biMesaPlateau:
		case biMesaPlateauF:
		case biMesaPlateauFM:
		case biMesaPlateauM:
		{
			// These biomes don't actualy have any downfall.
			return 1000;
		}

		case biNether:
		case biEnd:
		{
			// These shouldn't get any snow at all.
			return 9999;
		}

		default:
		{
			return 0;
		}
	}
}




