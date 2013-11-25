
// BiomeColors.cpp

// Implements the g_BiomeColors[] array preparation based on a stored biome-to-color map

#include "Globals.h"
#include "BiomeColors.h"





int g_BiomeColors[256];





static struct
{
	EMCSBiome Biome;
	int Color;
} g_BiomeColorMap[] =
{
	{ biOcean,                0x000070 },
	{ biPlains,               0x8db360 },
	{ biDesert,               0xfa9418 },
	{ biExtremeHills,         0x606060 },
	{ biForest,               0x056621 },
	{ biTaiga,                0x0b6659 },
	{ biSwampland,            0x2fffda },
	{ biRiver,                0x3030af },
	{ biHell,                 0x7f0000 },
	{ biSky,                  0x007fff },
	{ biFrozenOcean,          0xa0a0df },
	{ biFrozenRiver,          0xa0a0ff },
	{ biIcePlains,            0xffffff },
	{ biIceMountains,         0xa0a0a0 },
	{ biMushroomIsland,       0xff00ff },
	{ biMushroomShore,        0xa000ff },
	{ biBeach,                0xfade55 },
	{ biDesertHills,          0xd25f12 },
	{ biForestHills,          0x22551c },
	{ biTaigaHills,           0x163933 },
	{ biExtremeHillsEdge,     0x7f8f7f },
	{ biJungle,               0x537b09 },
	{ biJungleHills,          0x2c4205 },
	
	{ biJungleEdge,           0x628b17 },
	{ biDeepOcean,            0x000030 },
	{ biStoneBeach,           0xa2a284 },
	{ biColdBeach,            0xfaf0c0 },
	{ biBirchForest,          0x307444 },
	{ biBirchForestHills,     0x1f5f32 },
	{ biRoofedForest,         0x40511a },
	{ biColdTaiga,            0x31554a },
	{ biColdTaigaHills,       0x597d72 },
	{ biMegaTaiga,            0x596651 },
	{ biMegaTaigaHills,       0x596659 },
	{ biExtremeHillsPlus,     0x507050 },
	{ biSavanna,              0xbdb25f },
	{ biSavannaPlateau,       0xa79d64 },
	{ biMesa,                 0xd94515 },
	{ biMesaPlateauF,         0xb09765 },
	{ biMesaPlateau,          0xca8c65 },
	
	// M variants:
	{ biSunflowerPlains,      0xb5db88 },
	{ biDesertM,              0xffbc40 },
	{ biExtremeHillsM,        0x888888 },
	{ biFlowerForest,         0x2d8e49 },
	{ biTaigaM,               0x338e81 },
	{ biSwamplandM,           0x07f9b2 },
	{ biIcePlainsSpikes,      0xb4dcdc },
	{ biJungleM,              0x7ba331 },
	{ biJungleEdgeM,          0x628b17 },
	{ biBirchForestM,         0x589c6c },
	{ biBirchForestHillsM,    0x47875a },
	{ biRoofedForestM,        0x687942 },
	{ biColdTaigaM,           0x243f36 },
	{ biMegaSpruceTaiga,      0x454f3e },
	{ biMegaSpruceTaigaHills, 0x454f4e },
	{ biExtremeHillsPlusM,    0x789878 },
	{ biSavannaM,             0xe5da87 },
	{ biSavannaPlateauM,      0xa79d74 },
	{ biMesaBryce,            0xff6d3d },
	{ biMesaPlateauFM,        0xd8bf8d },
	{ biMesaPlateauM,         0xf2b48d },
} ;





static class cBiomeColorsInitializer
{
public:
	cBiomeColorsInitializer(void)
	{
		// Reset all colors to gray:
		for (size_t i = 0; i < ARRAYCOUNT(g_BiomeColors); i++)
		{
			g_BiomeColors[i] = 0x7f7f7f;
		}
		for (size_t i = 0; i < ARRAYCOUNT(g_BiomeColorMap); i++)
		{
			g_BiomeColors[g_BiomeColorMap[i].Biome] = g_BiomeColorMap[i].Color;
		}
	}
} g_Initializer;




